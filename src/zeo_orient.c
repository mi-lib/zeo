/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_orient - exact orientation predicates.
 */

#include <zeo/zeo_orient.h>

/* The orientation predicates return the exact sign of determinants. A determinant is first computed
 * by floating-point arithmetics, and its sign is adopted if its absolute value is larger than the
 * error bound. Otherwise, it is computed exactly by the arithmetics of expansions (provided by ZM),
 * namely, sums of nonoverlapping floating-point numbers, based on a method by Shewchuk:
 *   J. R. Shewchuk, Adaptive Precision Floating-Point Arithmetic and Fast Robust Geometric Predicates,
 *   Discrete & Computational Geometry, 18(3):305-363, 1997.
 * IEEE 754 double precision with round-to-nearest, no extended precision for intermediate values,
 * no fused multiply-add contraction, and neither overflow nor underflow are assumed. */

/* unit roundoff */
#define ZEO_ORIENT_U          ( 0.5 * DBL_EPSILON )
/* error bounds of the floating-point filters (see doc) */
#define ZEO_ORIENT2D_ERRBOUND ( ( 4.0 +  64.0 * ZEO_ORIENT_U ) * ZEO_ORIENT_U )
#define ZEO_ORIENT3D_ERRBOUND ( ( 8.0 + 256.0 * ZEO_ORIENT_U ) * ZEO_ORIENT_U )

/* maximum sizes of expansions: sums of 6 and 24 triple products, respectively, each of which has
 * at most 4 components. */
#define ZEO_ORIENT2D_EXPANSION_MAX 24
#define ZEO_ORIENT3D_EXPANSION_MAX 96

/* exact orientation of three 2D points. */
static int _zOrient2DExact(const double a[], const double b[], const double c[])
{
  double e[ZEO_ORIENT2D_EXPANSION_MAX];
  int n = 0;

  /* det[ b 1; c 1; a 1 ] expanded along the last column */
  n = zAddExpansionTripleProd( n, e,  c[0], a[1], 1 );
  n = zAddExpansionTripleProd( n, e, -c[1], a[0], 1 );
  n = zAddExpansionTripleProd( n, e, -b[0], a[1], 1 );
  n = zAddExpansionTripleProd( n, e,  b[1], a[0], 1 );
  n = zAddExpansionTripleProd( n, e,  b[0], c[1], 1 );
  n = zAddExpansionTripleProd( n, e, -b[1], c[0], 1 );
  return zSgnExpansion( n, e );
}

/* orientation of three 2D points. */
int zOrient2D(const double a[], const double b[], const double c[])
{
  double l, r, d;

  l = ( b[0] - a[0] ) * ( c[1] - a[1] );
  r = ( b[1] - a[1] ) * ( c[0] - a[0] );
  d = l - r;
  if( fabs( d ) > ZEO_ORIENT2D_ERRBOUND * ( fabs( l ) + fabs( r ) ) )
    return d > 0 ? 1 : -1;
  return _zOrient2DExact( a, b, c );
}

/* add a 3x3 determinant of row vectors to an expansion. */
static int _zAddExpansionDet3(int n, double e[], const double r0[], const double r1[], const double r2[])
{
  n = zAddExpansionTripleProd( n, e,  r0[0], r1[1], r2[2] );
  n = zAddExpansionTripleProd( n, e, -r0[0], r1[2], r2[1] );
  n = zAddExpansionTripleProd( n, e, -r0[1], r1[0], r2[2] );
  n = zAddExpansionTripleProd( n, e,  r0[1], r1[2], r2[0] );
  n = zAddExpansionTripleProd( n, e,  r0[2], r1[0], r2[1] );
  n = zAddExpansionTripleProd( n, e, -r0[2], r1[1], r2[0] );
  return n;
}

/* exact orientation of four 3D points. */
static int _zOrient3DExact(const double a[], const double b[], const double c[], const double d[])
{
  double e[ZEO_ORIENT3D_EXPANSION_MAX];
  int n = 0;

  /* det[ b 1; c 1; d 1; a 1 ] expanded along the last column, where the rows of the minors with
   * negative signs are swapped instead. */
  n = _zAddExpansionDet3( n, e, d, c, a );
  n = _zAddExpansionDet3( n, e, b, d, a );
  n = _zAddExpansionDet3( n, e, c, b, a );
  n = _zAddExpansionDet3( n, e, b, c, d );
  return zSgnExpansion( n, e );
}

/* orientation of four 3D points. */
int zOrient3D(const double a[], const double b[], const double c[], const double d[])
{
  double p[3], q[3], r[3], pq[6], det, perm;
  int i;

  for( i=0; i<3; i++ ){
    p[i] = b[i] - a[i];
    q[i] = c[i] - a[i];
    r[i] = d[i] - a[i];
  }
  pq[0] = p[0] * q[1]; pq[1] = p[1] * q[0];
  pq[2] = p[1] * q[2]; pq[3] = p[2] * q[1];
  pq[4] = p[2] * q[0]; pq[5] = p[0] * q[2];
  det = r[2] * ( pq[0] - pq[1] ) + r[0] * ( pq[2] - pq[3] ) + r[1] * ( pq[4] - pq[5] );
  perm = fabs( r[2] ) * ( fabs( pq[0] ) + fabs( pq[1] ) )
       + fabs( r[0] ) * ( fabs( pq[2] ) + fabs( pq[3] ) )
       + fabs( r[1] ) * ( fabs( pq[4] ) + fabs( pq[5] ) );
  if( fabs( det ) > ZEO_ORIENT3D_ERRBOUND * perm )
    return det > 0 ? 1 : -1;
  return _zOrient3DExact( a, b, c, d );
}
