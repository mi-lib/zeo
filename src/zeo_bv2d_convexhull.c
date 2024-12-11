/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_convexhull - 2D bounding volume: planar convex hull.
 */

#include <zeo/zeo_bv2d.h>

/* NOTE: THIS FILE WILL BE COMPLETELY REPLACED TO 2D CONVEX HULL COMPUTATION! */

/* find two independent base vectors on a plane. */
static zVec3D *_zConvexHull2DBase(zVec3DAddrList *addrlist, zVec3D s[])
{
  zVec3DAddrListCell *cp;
  zVec3D n;

  for( cp=zListCellNext(zListTail(addrlist)); cp!=zListRoot(addrlist); cp=zListCellNext(cp) )
    if( !zVec3DIsTiny( zVec3DSub( cp->data, zListTail(addrlist)->data, &s[0] ) ) )
      goto STEP2;
  ZRUNERROR( ZEO_ERR_CH_DEG1 );
  return NULL;
 STEP2:
  for( cp=zListCellNext(cp);
       cp!=zListRoot(addrlist); cp=zListCellNext(cp) ){
    if( !zVec3DIsTiny( zVec3DSub( cp->data, zListTail(addrlist)->data, &s[1] ) ) &&
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
  d1 = zVec3DInnerProd( ((zVec3DAddrListCell *)v1)->data, s1 );
  d2 = zVec3DInnerProd( ((zVec3DAddrListCell *)v2)->data, s1 );
  if( zIsTiny( d1 - d2 ) ){
    d1 = zVec3DInnerProd( ((zVec3DAddrListCell *)v1)->data, s2 );
    d2 = zVec3DInnerProd( ((zVec3DAddrListCell *)v2)->data, s2 );
    if( zIsTiny( d1 - d2 ) ) return 0;
    return d1 > d2 ? 1 : -1;
  }
  return d1 > d2 ? 1 : -1;
}

/* planar convex hull of a list of vertices. */
static zLoop3D *_zConvexHull2D(zVec3DAddrList *addrlist, zLoop3D *chloop)
{
  zVec3D s[2], d;
  zVec3DAddrListCell *p0, *p1, *p;
  double t, t_max;

  zListInit( chloop );
  if( !_zConvexHull2DBase( addrlist, s ) ){
    ZRUNERROR( ZEO_ERR_CH_DEG1 );
    return NULL;
  }
  zVec3DAddrListQuickSort( addrlist, __z_ch2d_cmp, s );
  /* upper bound */
  for( p0=zListTail(addrlist); p0!=zListHead(addrlist); zLoop3DAdd( chloop, p0->data ), p0=p1 )
    for( t_max=-zPI_2, p1=p=zListCellNext(p0); p!=zListRoot(addrlist); p=zListCellNext(p) )
      if( !zVec3DIsTiny( zVec3DSub( p->data, p0->data, &d ) ) &&
          ( t = atan2( zVec3DInnerProd(&d,&s[1]), zVec3DInnerProd(&d,&s[0]) ) ) >= t_max ){
        t_max = t;
        p1 = p;
      }
  /* reverse bases */
  zVec3DRevDRC( &s[0] );
  zVec3DRevDRC( &s[1] );
  /* lower bound */
  for( ; p0!=zListTail(addrlist); zLoop3DAdd( chloop, p0->data ), p0=p1 )
    for( t_max=-zPI_2, p1=p=zListCellPrev(p0); p!=zListRoot(addrlist); p=zListCellPrev(p) )
      if( !zVec3DIsTiny( zVec3DSub( p->data, p0->data, &d ) ) &&
          ( t = atan2( zVec3DInnerProd(&d,&s[1]), zVec3DInnerProd(&d,&s[0]) ) ) >= t_max ){
        t_max = t;
        p1 = p;
      }
  return chloop;
}

/* convert a planar convex hull to a polyhedron. */
static zPH3D *_zConvexHull2DToPH3D(zPH3D *ch, zLoop3D *chloop)
{
  zLoop3DCell *vc;
  int vn, i;

  vn = zListSize( chloop );
  if( !( ch = zPH3DAlloc( ch, vn, 2*(vn-2) ) ) ) return NULL;
  i = 0;
  /* vertices */
  zListForEach( chloop, vc )
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

/* create the convex hull of 3D points on a plane. */
zLoop3D *zVec3DDataConvexHull2D(zVec3DData *data, zLoop3D *chloop)
{
  zVec3DAddrList addrlist;
  zLoop3D *ret;

  if( !zVec3DDataToAddrList( data, &addrlist ) ){
    zVec3DAddrListDestroy( &addrlist );
    return NULL;
  }
  ret = _zConvexHull2D( &addrlist, chloop );
  zVec3DAddrListDestroy( &addrlist );
  if( !ret )
    zLoop3DDestroy( chloop );
  return ret;
}

/* convert a planar convex hull to a polyhedron. */
zPH3D *zVec3DDataConvexHull2DPH3D(zVec3DData *data, zPH3D *ch)
{
  zLoop3D chloop;

  zPH3DInit( ch );
  if( !zVec3DDataConvexHull2D( data, &chloop ) ) return NULL;
  ch = _zConvexHull2DToPH3D( ch, &chloop );
  zLoop3DDestroy( &chloop );
  return ch;
}

/* the closest point in a convex hull to a point. */
double zConvexHull2DClosest(zLoop3D *ch, zVec3D *p, zVec3D *cp)
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
