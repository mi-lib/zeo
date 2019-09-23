/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv_ch2 - bounding volume: planar convex hull.
 */

#include <zeo/zeo_bv.h>

/* ********************************************************** */
/* planar (2D) convex hull for a list of points
 * ********************************************************** */

/* find two independent base vectors on a plane. */
static zVec3D *_zCH2DBasePL(zVec3DList *pl, zVec3D s[])
{
  zVec3DListCell *pc;
  zVec3D n;

  for( pc=zListCellNext(zListTail(pl));
       pc!=zListRoot(pl); pc=zListCellNext(pc) )
    if( !zVec3DIsTiny( zVec3DSub( pc->data, zListTail(pl)->data, &s[0] ) ) )
      goto STEP2;
  ZRUNERROR( ZEO_ERR_CH_DEG1 );
  return NULL;
 STEP2:
  for( pc=zListCellNext(pc);
       pc!=zListRoot(pl); pc=zListCellNext(pc) ){
    if( !zVec3DIsTiny( zVec3DSub( pc->data, zListTail(pl)->data, &s[1] ) ) &&
        !zVec3DIsTiny( zVec3DOuterProd( &s[0], &s[1], &n ) ) )
      goto STEP3;
  }
  ZRUNERROR( ZEO_ERR_CH_DEG2 );
  return NULL;
 STEP3:
  zVec3DOuterProd( &n, &s[0], &s[1] );
  zVec3DNormalizeDRC( &s[0] );
  zVec3DNormalizeDRC( &s[1] );
  return s;
}

/* comparison function of two vertices for sorting. */
static int __z_ch2d_pl_cmp(void *v1, void *v2, void *priv)
{
  double d1, d2;
  zVec3D *s1, *s2;

  s1 = (zVec3D *)priv;
  s2 = (zVec3D *)priv + 1;
  d1 = zVec3DInnerProd( ((zVec3DListCell *)v1)->data, s1 );
  d2 = zVec3DInnerProd( ((zVec3DListCell *)v2)->data, s1 );
  if( zIsTiny( d1 - d2 ) ){
    d1 = zVec3DInnerProd( ((zVec3DListCell *)v1)->data, s2 );
    d2 = zVec3DInnerProd( ((zVec3DListCell *)v2)->data, s2 );
    if( zIsTiny( d1 - d2 ) ) return 0;
    return d1 > d2 ? 1 : -1;
  }
  return d1 > d2 ? 1 : -1;
}

/* planar convex hull of a list of vertices. */
zLoop3D *zCH2DPL(zLoop3D *ch, zVec3DList *pl)
{
  zVec3D s[2], d;
  zVec3DListCell *p0, *p1, *p;
  double t, t_max;

  zListInit( ch );
  if( !_zCH2DBasePL( pl, s ) ){
    ZRUNERROR( ZEO_ERR_CH_DEG1 );
    return NULL;
  }
  zVec3DListQuickSort( pl, __z_ch2d_pl_cmp, s );
  /* upper bound */
  for( p0=zListTail(pl); p0!=zListHead(pl); zLoop3DInsert( ch, p0->data ), p0=p1 )
    for( t_max=-zPI_2, p1=p=zListCellNext(p0); p!=zListRoot(pl); p=zListCellNext(p) )
      if( !zVec3DIsTiny( zVec3DSub( p->data, p0->data, &d ) ) &&
          ( t = atan2( zVec3DInnerProd(&d,&s[1]), zVec3DInnerProd(&d,&s[0]) ) ) >= t_max ){
        t_max = t;
        p1 = p;
      }
  /* reverse bases */
  zVec3DRevDRC( &s[0] );
  zVec3DRevDRC( &s[1] );
  /* lower bound */
  for( ; p0!=zListTail(pl); zLoop3DInsert( ch, p0->data ), p0=p1 )
    for( t_max=-zPI_2, p1=p=zListCellPrev(p0); p!=zListRoot(pl); p=zListCellPrev(p) )
      if( !zVec3DIsTiny( zVec3DSub( p->data, p0->data, &d ) ) &&
          ( t = atan2( zVec3DInnerProd(&d,&s[1]), zVec3DInnerProd(&d,&s[0]) ) ) >= t_max ){
        t_max = t;
        p1 = p;
      }
  return ch;
}

/* ********************************************************** */
/* planar (2D) convex hull for an array of points
 * ********************************************************** */

/* find two independent base vectors on a plane. */
static zVec3D *_zCH2DBase(zVec3D p[], int num, zVec3D s[])
{
  register int i;
  zVec3D n;

  for( i=1; i<num; i++ )
    if( !zVec3DIsTiny( zVec3DSub( &p[i], &p[0], &s[0] ) ) )
      goto STEP2;
  ZRUNERROR( ZEO_ERR_CH_DEG1 );
  return NULL;
 STEP2:
  for( i++; i<num; i++ ){
    if( !zVec3DIsTiny( zVec3DSub( &p[i], &p[0], &s[1] ) ) &&
        !zVec3DIsTiny( zVec3DOuterProd( &s[0], &s[1], &n ) ) )
      goto STEP3;
  }
  ZRUNERROR( ZEO_ERR_CH_DEG2 );
  return NULL;
 STEP3:
  zVec3DOuterProd( &n, &s[0], &s[1] );
  zVec3DNormalizeDRC( &s[0] );
  zVec3DNormalizeDRC( &s[1] );
  return s;
}

/* comparison function of two vertices for sorting. */
static int __z_ch2d_cmp(void *v1, void *v2, void *priv)
{
  double d1, d2;
  zVec3D *s1, *s2;

  s1 = (zVec3D *)priv;
  s2 = (zVec3D *)priv + 1;
  d1 = zVec3DInnerProd( (zVec3D *)v1, s1 );
  d2 = zVec3DInnerProd( (zVec3D *)v2, s1 );
  if( zIsTiny( d1 - d2 ) ){
    d1 = zVec3DInnerProd( (zVec3D *)v1, s2 );
    d2 = zVec3DInnerProd( (zVec3D *)v2, s2 );
    if( zIsTiny( d1 - d2 ) ) return 0;
    return d1 > d2 ? 1 : -1;
  }
  return d1 > d2 ? 1 : -1;
}

/* planar convex hull of an array of vertices. */
zLoop3D *zCH2D(zLoop3D *ch, zVec3D p[], int num)
{
  zVec3D s[2], d;
  register int i, j, k;
  double t, t_max;

  zListInit( ch );
  if( !_zCH2DBase( p, num, s ) ){
    ZRUNERROR( ZEO_ERR_CH_DEG1 );
    return NULL;
  }
  zQuickSort( p, num, sizeof(zVec3D), __z_ch2d_cmp, s );

  /* upper bound */
  for( i=0; i<num-1; zLoop3DInsert( ch, &p[i] ), i=j )
    for( t_max=-zPI_2, j=k=i+1; k<num; k++ )
      if( !zVec3DIsTiny( zVec3DSub( &p[k], &p[i], &d ) ) &&
          ( t = atan2( zVec3DInnerProd(&d,&s[1]), zVec3DInnerProd(&d,&s[0]) ) ) >= t_max ){
        t_max = t;
        j = k;
      }
  /* reverse bases */
  zVec3DRevDRC( &s[0] );
  zVec3DRevDRC( &s[1] );
  /* lower bound */
  for( ; i>0; zLoop3DInsert( ch, &p[i] ), i=j )
    for( t_max=-zPI_2, j=k=i-1; k>=0; k-- )
      if( !zVec3DIsTiny( zVec3DSub( &p[k], &p[i], &d ) ) &&
          ( t = atan2( zVec3DInnerProd(&d,&s[1]), zVec3DInnerProd(&d,&s[0]) ) ) >= t_max ){
        t_max = t;
        j = k;
      }
  return ch;
}

/* convert a planar convex hull to a polyhedron. */
static zPH3D *_zCH2D2PH3D(zPH3D *ch, zLoop3D *vl)
{
  zLoop3DCell *vc;
  register int vn, i;

  vn = zListSize(vl);
  if( !( ch = zPH3DAlloc( ch, vn, 2*(vn-2) ) ) ) return NULL;
  i = 0;
  /* vertices */
  zListForEach( vl, vc )
    zVec3DCopy( vc->data, zPH3DVert(ch,i++) );
  /* faces */
  for( i=2; i<vn; i++ ){
    zTri3DCreate( zPH3DFace(ch,i-2),
      zPH3DVert(ch,0), zPH3DVert(ch,i-1), zPH3DVert(ch,i) );
    zTri3DCreate( zPH3DFace(ch,i+vn-4),
      zPH3DVert(ch,0), zPH3DVert(ch,i),   zPH3DVert(ch,i-1) );
  }
  return ch;
}

/* a planar convex hull to a polyhedron. */
zPH3D *zCH2D2PH3D(zPH3D *ch, zVec3D vert[], int n)
{
  zLoop3D vl;

  if( !zCH2D( &vl, vert, n ) || !_zCH2D2PH3D( ch, &vl ) ) return NULL;
  zLoop3DDestroy( &vl );
  return ch;
}

/* a planar convex hull of a list of vertices to a polyhedron. */
zPH3D *zCH2DPL2PH3D(zPH3D *ch, zVec3DList *pl)
{
  zLoop3D vl;

  if( !zCH2DPL( &vl, pl ) || !_zCH2D2PH3D( ch, &vl ) ) return NULL;
  zLoop3DDestroy( &vl );
  return ch;
}

/* the closest point in a convex hull to a point. */
double zCH2DClosest(zLoop3D *ch, zVec3D *p, zVec3D *cp)
{
  zLoop3DCell *vc, *vcp;
  zVec3D norm, v1, v2, tmp;
  zEdge3D e;
  double d = HUGE_VAL, d_new;

  if( zListSize(ch) == 0 ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return HUGE_VAL;
  }
  vcp = zListHead(ch);
  if( zListSize(ch) == 1 ){
    zVec3DCopy( vcp->data, cp );
    return zVec3DDist( cp, p );;
  }
  if( zListSize(ch) == 2 ){
    zEdge3DCreate( &e, zListCellPrev(vcp)->data, vcp->data );
    return zEdge3DClosest( &e, p, cp );
  }
  /* normal vector */
  zVec3DSub( zListCellPrev(vcp)->data, vcp->data, &v1 );
  zVec3DSub( zListCellPrev(zListCellPrev(vcp))->data, vcp->data, &v2 );
  zVec3DOuterProd( &v2, &v1, &norm );
  /* step-by-step edge check */
  zVec3DSub( vcp->data, p, &v1 );
  zVec3DCopy( p, cp );
  zListForEach( ch, vc ){
    zVec3DSub( vc->data, p, &v2 );
    zVec3DOuterProd( &v1, &v2, &tmp );
    /* if the point is visible from the current edge, the outer product
       faces in the opposite direction with the normal vector. */
    if( zVec3DInnerProd( &norm, &tmp ) < 0 ){ /* visible case */
      zEdge3DCreate( &e, vc->data, vcp->data );
      d_new = zEdge3DClosest( &e, p, &tmp );
      if( d_new > d ) return d; /* distance has a minimum peak. */
      zVec3DCopy( &tmp, cp );
      d = d_new;
    }
    zVec3DCopy( &v2, &v1 );
    vcp = vc;
  }
  return d;
}
