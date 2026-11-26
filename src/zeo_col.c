/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col - collision checking.
 */

#include <zeo/zeo_col.h>

/* check if convex hulls of two sets of 3D points apart from each other are within a margin. */
bool _zColChkVec3DArray(zVec3D v1[], int n1, zVec3D v2[], int n2, double margin)
{
  zVec3DArray array1, array2;
  zVec3DData data1, data2;
  zVec3D c1, c2;

  if( margin <= 0 ) return false;
  zArrayAssign( &array1, n1, v1 );
  zArrayAssign( &array2, n2, v2 );
  zVec3DDataAssignArray( &data1, &array1 );
  zVec3DDataAssignArray( &data2, &array2 );
  return zGJK( &data1, &data2, &c1, &c2 ) || zVec3DSqrDist( &c1, &c2 ) < margin * margin;
}

/* separation of convex hulls of two sets of 3D points along an axis, which is negative if their
 * projections onto the axis overlap. -HUGE_VAL is returned for a zero axis. */
static double _zColChkVec3DArraySep(const zVec3D v1[], int n1, const zVec3D v2[], int n2, const zVec3D *a)
{
  double l, p, min1, max1, min2, max2;
  int i;

  if( ( l = zVec3DNorm( a ) ) == 0 ) return -HUGE_VAL;
  min1 = max1 = zVec3DInnerProd( &v1[0], a );
  for( i=1; i<n1; i++ ){
    if( ( p = zVec3DInnerProd( &v1[i], a ) ) < min1 ) min1 = p;
    if( p > max1 ) max1 = p;
  }
  min2 = max2 = zVec3DInnerProd( &v2[0], a );
  for( i=1; i<n2; i++ ){
    if( ( p = zVec3DInnerProd( &v2[i], a ) ) < min2 ) min2 = p;
    if( p > max2 ) max2 = p;
  }
  return _zMax( min2 - max1, min1 - max2 ) / l;
}

/* check if convex hulls of two sets of 3D points are in collision with each other within a margin.
 * The maximum separation of them along the given axes, which include the normal vectors of all faces
 * of the Minkowski difference of them, is the signed distance between them if it is non-positive
 * (separating axis theorem), and is a lower bound of the distance otherwise. In the latter case, the
 * distance is computed by the GJK algorithm only if the lower bound is less than the margin. */
static bool _zColChkVec3DArraySAT(zVec3D v1[], int n1, zVec3D v2[], int n2, const zVec3D a[], int na, double margin)
{
  double s, smax = -HUGE_VAL;
  int i;

  for( i=0; i<na; i++ ){
    if( ( s = _zColChkVec3DArraySep( v1, n1, v2, n2, &a[i] ) ) >= margin ) return false;
    if( s > smax ) smax = s;
  }
  return smax <= 0 ? smax < margin : _zColChkVec3DArray( v1, n1, v2, n2, margin );
}

/* check if convex hulls of two sets of 3D points, either of which has no volume, are in collision
 * with each other within a margin. The signed distance between them is zero if they intersect. */
static bool _zColChkVec3DArrayFlat(zVec3D v1[], int n1, zVec3D v2[], int n2, double margin)
{
  zVec3DArray array1, array2;
  zVec3DData data1, data2;
  zVec3D c1, c2;

  zArrayAssign( &array1, n1, v1 );
  zArrayAssign( &array2, n2, v2 );
  zVec3DDataAssignArray( &data1, &array1 );
  zVec3DDataAssignArray( &data2, &array2 );
  if( zGJK( &data1, &data2, &c1, &c2 ) ) return margin > 0;
  return margin > 0 && zVec3DSqrDist( &c1, &c2 ) < margin * margin;
}

/* check if an edge and a plane are in collision with each other.
 * The signed distance between them is max(smin,-smax), where smin and smax are the minimum and
 * maximum signed distances of the endpoints of the edge from the plane, respectively. */
bool zColChkEdgePlane3D(const zEdge3D *edge, const zPlane3D *plane, double margin)
{
  double s0, s1;

  s0 = zPlane3DDistFromPoint( plane, zEdge3DVert(edge,0) );
  s1 = zPlane3DDistFromPoint( plane, zEdge3DVert(edge,1) );
  return _zMax( _zMin(s0,s1), -_zMax(s0,s1) ) < margin;
}

/* intersection between 3D edge and 3D plane. */
int zIntersectEdgePlane3D(const zEdge3D *edge, const zPlane3D *plane, zVec3D ip[])
{
  double s0, s1, d;
  zVec3D v;

  s0 = zPlane3DDistFromPoint( plane, zEdge3DVert(edge,0) );
  s1 = zPlane3DDistFromPoint( plane, zEdge3DVert(edge,1) );
  if( ( s0 > 0 && s1 > 0 ) || ( s0 < 0 && s1 < 0 ) ) return 0;
  zVec3DSub( zPlane3DOrg(plane), zEdge3DVert(edge,0), &v );
  d = zVec3DInnerProd( zEdge3DVec(edge), zPlane3DNorm(plane) );
  if( zIsTiny( d ) ){
    zVec3DCopy( zEdge3DVert(edge,0), &ip[0] );
    zVec3DCopy( zEdge3DVert(edge,1), &ip[1] );
    return 2;
  }
  zVec3DMul( zEdge3DVec(edge),
    zVec3DInnerProd(&v,zPlane3DNorm(plane) )/d, &v );
  zVec3DAdd( zEdge3DVert(edge,0), &v, &ip[0] );
  return 1;
}

/* intersection between two 3D planes */
int zIntersectPlane3D(const zPlane3D *plane1, const zPlane3D *plane2, zLine3D *line)
{
  zVec3D p, d, dp;
  double n, det, s1, s2, k1, k2;

  zVec3DOuterProd( zPlane3DNorm(plane1), zPlane3DNorm(plane2), &d );
  zVec3DSub( zPlane3DOrg(plane1), zPlane3DOrg(plane2), &dp );
  if( zVec3DIsTiny( &d ) ){ /* exception */
    if( zIsTiny( zVec3DInnerProd( zPlane3DNorm(plane1), &dp ) ) ){
      ZRUNWARN( ZEO_ERR_COLCHK_PLANE_IDENT );
      return -1;
    }
    ZRUNWARN( ZEO_ERR_COLCHK_PLANE_PARALLEL );
    return 0;
  }
  zVec3DMid( zPlane3DOrg(plane1), zPlane3DOrg(plane2), &p );
  n = zVec3DInnerProd( zPlane3DNorm(plane1), zPlane3DNorm(plane2) );
  det = 2 * ( 1 - n * n );
  s1 = zVec3DInnerProd( zPlane3DNorm(plane1), &dp );
  s2 = zVec3DInnerProd( zPlane3DNorm(plane2), &dp );
  k1 = ( s1 + n * s2 ) / det;
  k2 =-( n * s1 + s2 ) / det;
  zVec3DCatDRC( &p, k1, zPlane3DNorm(plane1) );
  zVec3DCatDRC( &p, k2, zPlane3DNorm(plane2) );
  zLine3DCreate( line, &p, &d );
  return 1;
}

/* check if a plane and a triangle are in collision with each other.
 * The signed distance between them is max(smin,-smax), where smin and smax are the minimum and
 * maximum signed distances of the vertices of the triangle from the plane, respectively. */
bool zColChkTriPlane3D(const zTri3D *tri, const zPlane3D *plane, double margin)
{
  double s, smin, smax;
  int i;

  smin = smax = zPlane3DDistFromPoint( plane, zTri3DVert(tri,0) );
  for( i=1; i<3; i++ ){
    s = zPlane3DDistFromPoint( plane, zTri3DVert(tri,i) );
    if( s < smin ) smin = s;
    if( s > smax ) smax = s;
  }
  return _zMax( smin, -smax ) < margin;
}

/* intersection of 3D plane and triangle. */
int zIntersectTriPlane3D(const zTri3D *tri, const zPlane3D *plane, zVec3D ip[])
{
  int i, j, k;
  int n, ne;
  zVec3D v[2];
  zEdge3D edge;

  for( n=0, i=0; i<3; i++ ){
    zEdge3DCreate( &edge, zTri3DVert(tri,i), zTri3DVertNext(tri,i) );
    ne = zIntersectEdgePlane3D( &edge, plane, v );
    for( j=0; j<ne; j++ ){
      for( k=0; k<n; k++ )
        if( zVec3DEqual( &v[j], &ip[k] ) ) break;
      if( k == n )
        zVec3DCopy( &v[j], &ip[n++] );
    }
  }
  return n;
}

/* check if an edge and a triangle are in collision with each other.
 * The candidate separating axes are the normal vector of the triangle, the outer products of the
 * edge and the edges of the triangle, and the axes perpendicular to the edges in the plane of the
 * triangle for the case where the edge is on the plane. */
bool zColChkEdgeTri3D(const zEdge3D *edge, const zTri3D *tri, double margin)
{
  zVec3D v1[2], v2[3], e, a[8];
  int i;

  zVec3DCopy( zEdge3DVert(edge,0), &v1[0] );
  zVec3DCopy( zEdge3DVert(edge,1), &v1[1] );
  for( i=0; i<3; i++ ) zVec3DCopy( zTri3DVert(tri,i), &v2[i] );
  zVec3DSub( &v1[1], &v1[0], &a[7] );
  zVec3DSub( &v2[1], &v2[0], &e );
  zVec3DSub( &v2[2], &v2[1], &a[0] );
  zVec3DOuterProd( &e, &a[0], &a[0] );
  if( zVec3DIsTiny( &a[0] ) ) /* degenerate triangle */
    return _zColChkVec3DArrayFlat( v1, 2, v2, 3, margin );
  for( i=0; i<3; i++ ){
    zVec3DSub( &v2[(i+1)%3], &v2[i], &e );
    zVec3DOuterProd( &e, &a[7], &a[i+1] );
    zVec3DOuterProd( &a[0], &e, &a[i+4] );
  }
  zVec3DOuterProd( &a[0], &a[7], &a[7] );
  return _zColChkVec3DArraySAT( v1, 2, v2, 3, a, 8, margin );
}

/* intersection of 3D edge and triangle. */
int zIntersectEdgeTri3D(const zEdge3D *edge, const zTri3D *tri, zVec3D ip[])
{
  int i, ne, n = 0;
  zVec3D v[2];
  zPlane3D plane;

  if( zTri3DToPlane3D( tri, &plane ) ){
    ne = zIntersectEdgePlane3D( edge, &plane, v );
    for( i=0; i<ne; i++ )
      if( zTri3DPointIsInside( tri, &v[i], zTOL ) )
        zVec3DCopy( &v[i], &ip[n++] );
  }
  return n;
}

/* check if two triangles are in collision with each other.
 * The candidate separating axes are the normal vectors of the triangles, the outer products of the
 * edges of them, and the axes perpendicular to the edges in the planes of the triangles for the case
 * where they are on the same plane. */
bool zColChkTri3D(const zTri3D *tri1, const zTri3D *tri2, double margin)
{
  zVec3D v1[3], v2[3], e1[3], e2[3], a[23];
  int i, j;

  for( i=0; i<3; i++ ){
    zVec3DCopy( zTri3DVert(tri1,i), &v1[i] );
    zVec3DCopy( zTri3DVert(tri2,i), &v2[i] );
  }
  for( i=0; i<3; i++ ){
    zVec3DSub( &v1[(i+1)%3], &v1[i], &e1[i] );
    zVec3DSub( &v2[(i+1)%3], &v2[i], &e2[i] );
  }
  zVec3DOuterProd( &e1[0], &e1[1], &a[0] );
  zVec3DOuterProd( &e2[0], &e2[1], &a[1] );
  if( zVec3DIsTiny( &a[0] ) || zVec3DIsTiny( &a[1] ) ) /* degenerate triangle */
    return _zColChkVec3DArrayFlat( v1, 3, v2, 3, margin );
  for( i=0; i<3; i++ ){
    for( j=0; j<3; j++ )
      zVec3DOuterProd( &e1[i], &e2[j], &a[2+i*3+j] );
    zVec3DOuterProd( &a[0], &e1[i], &a[11+i] );
    zVec3DOuterProd( &a[1], &e1[i], &a[14+i] );
    zVec3DOuterProd( &a[0], &e2[i], &a[17+i] );
    zVec3DOuterProd( &a[1], &e2[i], &a[20+i] );
  }
  return _zColChkVec3DArraySAT( v1, 3, v2, 3, a, 23, margin );
}

/* intersection of two 3D triangles. */
int zIntersectTri3D(const zTri3D *tri1, const zTri3D *tri2, zVec3D ip[])
{
  int i, j;
  int np[2], n = 0;
  zVec3D v[2][3];
  zPlane3D plane;

  zTri3DToPlane3D( tri2, &plane );
  np[0] = zIntersectTriPlane3D( tri1, &plane, v[0] );
  zTri3DToPlane3D( tri1, &plane );
  np[1] = zIntersectTriPlane3D( tri2, &plane, v[1] );
  for( i=0; i<np[0]; i++ ){
    if( !zTri3DPointIsInside( tri2, &v[0][i], zTOL ) )
      continue;
    for( j=0; j<n; j++ )
      if( zVec3DEqual( &v[0][i], &ip[j] ) ) break;
    if( j == n )
      zVec3DCopy( &v[0][i], &ip[n++] );
  }
  for( i=0; i<np[1]; i++ ){
    if( !zTri3DPointIsInside( tri1, &v[1][i], zTOL ) )
      continue;
    for( j=0; j<n; j++ )
      if( zVec3DEqual( &v[1][i], &ip[j] ) ) break;
    if( j == n )
      zVec3DCopy( &v[1][i], &ip[n++] );
  }
  return n;
}
