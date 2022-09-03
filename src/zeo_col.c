/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col - collision checking.
 */

#include <zeo/zeo_col.h>

/* check if an edge and a plane intersect with each other. */
bool zColChkEdgePlane3D(zEdge3D *e, zPlane3D *p)
{
  zVec3D v[2];

  zVec3DSub( zEdge3DVert(e,0), zPlane3DVert(p), &v[0] );
  zVec3DSub( zEdge3DVert(e,1), zPlane3DVert(p), &v[1] );
  return zVec3DInnerProd( &v[0], zPlane3DNorm(p) )
       * zVec3DInnerProd( &v[1], zPlane3DNorm(p) ) <= 0 ? true : false;
}

/* intersection between 3D edge and 3D plane. */
int zIntersectEdgePlane3D(zEdge3D *e, zPlane3D *p, zVec3D ip[])
{
  double d;
  zVec3D v;

  if( !zColChkEdgePlane3D( e, p ) ) return 0;
  zVec3DSub( zPlane3DVert(p), zEdge3DVert(e,0), &v );
  d = zVec3DInnerProd( zEdge3DVec(e), zPlane3DNorm(p) );
  if( zIsTiny( d ) ){
    zVec3DCopy( zEdge3DVert(e,0), &ip[0] );
    zVec3DCopy( zEdge3DVert(e,1), &ip[1] );
    return 2;
  }
  zVec3DMul( zEdge3DVec(e),
    zVec3DInnerProd(&v,zPlane3DNorm(p) )/d, &v );
  zVec3DAdd( zEdge3DVert(e,0), &v, &ip[0] );
  return 1;
}

/* check if a plane and a triangle intersect with each other. */
bool zColChkTriPlane3D(zTri3D *t, zPlane3D *p)
{
  int i;
  zEdge3D e;

  for( i=0; i<3; i++ ){
    zEdge3DCreate( &e, zTri3DVert(t,i), zTri3DVertNext(t,i) );
    if( zColChkEdgePlane3D( &e, p ) ) return true;
  }
  return false;
}

/* intersection of 3D plane and triangle. */
int zIntersectTriPlane3D(zTri3D *t, zPlane3D *p, zVec3D ip[])
{
  int i, j, k;
  int n, ne;
  zVec3D v[2];
  zEdge3D e;

  for( n=0, i=0; i<3; i++ ){
    zEdge3DCreate( &e, zTri3DVert(t,i), zTri3DVertNext(t,i) );
    ne = zIntersectEdgePlane3D( &e, p, v );
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
bool zColChkEdgeTri3D(zEdge3D *e, zTri3D *t)
{
  zVec3D ip[2];

  return zIntersectEdgeTri3D( e, t, ip ) > 0 ? true : false;
}

/* intersection of 3D edge and triangle. */
int zIntersectEdgeTri3D(zEdge3D *e, zTri3D *t, zVec3D ip[])
{
  int i, ne, n = 0;
  zVec3D v[2];
  zPlane3D p;

  if( zTri3DToPlane3D( t, &p ) ){
    ne = zIntersectEdgePlane3D( e, &p, v );
    for( i=0; i<ne; i++ )
      if( zTri3DPointIsInside( t, &v[i], true ) )
        zVec3DCopy( &v[i], &ip[n++] );
  }
  return n;
}

/* check if two triangles intersect with each other. */
bool zColChkTri3D(zTri3D *t1, zTri3D *t2)
{
  zVec3D ip[2];

  return zIntersectTri3D( t1, t2, ip ) > 0 ? true : false;
}

/* intersection of two 3D triangles. */
int zIntersectTri3D(zTri3D *t1, zTri3D *t2, zVec3D ip[])
{
  int i, j;
  int np[2], n = 0;
  zVec3D v[2][3];
  zPlane3D p;

  zTri3DToPlane3D( t2, &p );
  np[0] = zIntersectTriPlane3D( t1, &p, v[0] );
  zTri3DToPlane3D( t1, &p );
  np[1] = zIntersectTriPlane3D( t2, &p, v[1] );
  for( i=0; i<np[0]; i++ ){
    if( !zTri3DPointIsInside( t2, &v[0][i], true ) )
      continue;
    for( j=0; j<n; j++ )
      if( zVec3DEqual( &v[0][i], &ip[j] ) ) break;
    if( j == n )
      zVec3DCopy( &v[0][i], &ip[n++] );
  }
  for( i=0; i<np[1]; i++ ){
    if( !zTri3DPointIsInside( t1, &v[1][i], true ) )
      continue;
    for( j=0; j<n; j++ )
      if( zVec3DEqual( &v[1][i], &ip[j] ) ) break;
    if( j == n )
      zVec3DCopy( &v[1][i], &ip[n++] );
  }
  return n;
}
