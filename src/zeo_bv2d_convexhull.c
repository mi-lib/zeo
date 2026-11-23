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
  zVec3D *p0, d, dmax, n;
  double l, lmax;

  p0 = zListTail(addrlist)->data;
  /* the first base is directed to the farthest point from p0, which is chosen for accuracy. */
  for( lmax=0, cp=zListCellNext(zListTail(addrlist)); cp!=zListRoot(addrlist); cp=zListCellNext(cp) ){
    _zVec3DSub( cp->data, p0, &d );
    if( ( l = _zVec3DSqrNorm( &d ) ) > lmax ){
      lmax = l;
      zVec3DCopy( &d, &s[0] );
    }
  }
  if( !( lmax > zTOL * zTOL ) ){ /* all points are within zTOL from p0. */
    ZRUNERROR( ZEO_ERR_CH_DEG1 );
    return NULL;
  }
  /* the farthest point from the line along the first base. The distance is |s[0] x d| / |s[0]|. */
  for( lmax=0, cp=zListCellNext(zListTail(addrlist)); cp!=zListRoot(addrlist); cp=zListCellNext(cp) ){
    _zVec3DSub( cp->data, p0, &d );
    _zVec3DOuterProd( &s[0], &d, &n );
    if( ( l = _zVec3DSqrNorm( &n ) ) > lmax ){
      lmax = l;
      zVec3DCopy( &d, &dmax );
    }
  }
  /* the ratio of the distance to the length of the first base is compared with zTOL, so that the
   * decision does not depend on the size of the point set, and collinear points with rounding errors
   * are detected. */
  l = _zVec3DSqrNorm( &s[0] );
  if( !( lmax > zTOL * zTOL * l * l ) ){
    ZRUNERROR( ZEO_ERR_CH_DEG2 );
    return NULL;
  }
  /* the second base is the component orthogonal to the first base of the vector to the farthest
   * point from the line, the norm of which is the distance from the line. */
  zVec3DNormalizeDRC( &s[0], 0 );
  _zVec3DCat( &dmax, -_zVec3DInnerProd( &dmax, &s[0] ), &s[0], &s[1] );
  zVec3DNormalizeDRC( &s[1], 0 );
  return s;
}

/* comparison function of two vertices for sorting. */
static int __z_ch2d_cmp(zVec3DAddrListCell *v1, zVec3DAddrListCell *v2, void *priv)
{
  double d1, d2;
  zVec3D *s1, *s2;

  s1 = (zVec3D *)priv;
  s2 = (zVec3D *)priv + 1;
  d1 = zVec3DInnerProd( v1->data, s1 );
  d2 = zVec3DInnerProd( v2->data, s1 );
  if( zIsTiny( d1 - d2 ) ){
    d1 = zVec3DInnerProd( v1->data, s2 );
    d2 = zVec3DInnerProd( v2->data, s2 );
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

/* convex hull of 2D points by the monotone chain algorithm by A. M. Andrew, Another Efficient
 * Algorithm for Convex Hulls in Two Dimensions, Information Processing Letters, 9(5):216-219, 1979.
 * The points are sorted lexicographically, which is exact, and the lower and upper chains are
 * traced with the exact orientation predicate, where a point making a non-left turn is removed.
 * Hence, the result is never broken by rounding errors, and points on edges are not vertices. */

/* lexicographic comparison of two 2D points. */
static int _zVec2DDataConvexHullCmp(void *p1, void *p2, void *util)
{
  zVec2D *a = *(zVec2D**)p1, *b = *(zVec2D**)p2;

  if( a->c.x < b->c.x ) return -1;
  if( a->c.x > b->c.x ) return 1;
  return a->c.y < b->c.y ? -1 : ( a->c.y > b->c.y ? 1 : 0 );
}

/* add a point to a monotone chain with n points, where the last points not making a left turn are
 * removed while the chain has at least nmin points. nmin has to be 2 or more. */
static int _zVec2DDataConvexHullPush(zVec2D **chain, int n, int nmin, zVec2D *p)
{
  while( n >= nmin && zVec2DOrient2D( chain[n-2], chain[n-1], p ) <= 0 ) n--;
  chain[n++] = p;
  return n;
}

/* convex hull of 2D points. */
zPG2D *zVec2DDataConvexHull(zVec2DData *data, zPG2D *ch)
{
  zVec2D **p, **chain, *v;
  int n, k, kl, i;
  zPG2D *ret = NULL;

  zPG2DInit( ch );
  if( ( n = zVec2DDataSize(data) ) == 0 ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  p = zAlloc( zVec2D*, n );
  chain = zAlloc( zVec2D*, 2*n );
  if( !p || !chain ){
    ZALLOCERROR();
    goto TERMINATE;
  }
  zVec2DDataRewind( data );
  for( i=0; ( v = zVec2DDataFetch( data ) ); i++ ){
    if( zVec2DIsNan( v ) ) goto TERMINATE;
    p[i] = v;
  }
  zQuickSort( p, n, sizeof(zVec2D*), _zVec2DDataConvexHullCmp, NULL );
  /* lower chain from the leftmost point to the rightmost point */
  for( k=0, i=0; i<n; i++ )
    k = _zVec2DDataConvexHullPush( chain, k, 2, p[i] );
  /* upper chain back to the leftmost point, which does not remove the lower chain */
  for( kl=k+1, i=n-2; i>=0; i-- )
    k = _zVec2DDataConvexHullPush( chain, k, kl, p[i] );
  k--; /* the last point is the leftmost point */
  if( k < 3 ){
    if( p[0]->c.x == p[n-1]->c.x && p[0]->c.y == p[n-1]->c.y ) /* the first and the last are the same */
      ZRUNERROR( ZEO_ERR_CH_DEG1 );
    else
      ZRUNERROR( ZEO_ERR_CH_DEG2 );
    goto TERMINATE;
  }
  if( !zPG2DAlloc( ch, k, k-2 ) ) goto TERMINATE;
  for( i=0; i<k; i++ )
    zVec2DCopy( chain[i], zPG2DVert(ch,i) );
  for( i=0; i<k-2; i++ ) /* triangles fanning out from the first vertex */
    zTri2DCreate( zPG2DFace(ch,i), zPG2DVert(ch,0), zPG2DVert(ch,i+1), zPG2DVert(ch,i+2) );
  ret = ch;
 TERMINATE:
  zFree( p );
  zFree( chain );
  return ret;
}
