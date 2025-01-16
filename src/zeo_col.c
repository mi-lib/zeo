/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col - collision checking.
 */

#include <zeo/zeo_col.h>

/* check if an edge and a plane intersect with each other. */
bool zColChkEdgePlane3D(const zEdge3D *edge, const zPlane3D *plane)
{
  zVec3D v[2];

  zVec3DSub( zEdge3DVert(edge,0), zPlane3DOrg(plane), &v[0] );
  zVec3DSub( zEdge3DVert(edge,1), zPlane3DOrg(plane), &v[1] );
  return zVec3DInnerProd( &v[0], zPlane3DNorm(plane) )
       * zVec3DInnerProd( &v[1], zPlane3DNorm(plane) ) <= 0 ? true : false;
}

/* intersection between 3D edge and 3D plane. */
int zIntersectEdgePlane3D(const zEdge3D *edge, const zPlane3D *plane, zVec3D ip[])
{
  double d;
  zVec3D v;

  if( !zColChkEdgePlane3D( edge, plane ) ) return 0;
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

/* check if a plane and a triangle intersect with each other. */
bool zColChkTriPlane3D(const zTri3D *tri, const zPlane3D *plane)
{
  int i;
  zEdge3D edge;

  for( i=0; i<3; i++ ){
    zEdge3DCreate( &edge, zTri3DVert(tri,i), zTri3DVertNext(tri,i) );
    if( zColChkEdgePlane3D( &edge, plane ) ) return true;
  }
  return false;
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

/* check if an edge and a triangle intersect with each other. */
bool zColChkEdgeTri3D(const zEdge3D *edge, const zTri3D *tri)
{
  zVec3D ip[2];

  return zIntersectEdgeTri3D( edge, tri, ip ) > 0 ? true : false;
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

/* check if two triangles intersect with each other. */
bool zColChkTri3D(const zTri3D *tri1, const zTri3D *tri2)
{
  zVec3D ip[2];

  return zIntersectTri3D( tri1, tri2, ip ) > 0 ? true : false;
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
