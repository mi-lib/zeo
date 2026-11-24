/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_convexhull - 2D bounding volume: planar convex hull.
 */

#include <zeo/zeo_bv2d.h>

/* convex hull of 2D points by the monotone chain algorithm by Andrew:
 *   A. M. Andrew, Another Efficient Algorithm for Convex Hulls in Two Dimensions, Information
 *   Processing Letters, 9(5):216-219, 1979.
 * The points are exactly sorted lexicographically, and the lower and upper chains are traced with
 * the exact orientation predicate, where a point making a non-left turn is removed.
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
