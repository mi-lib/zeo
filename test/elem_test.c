#include <zeo/zeo.h>

#define N 100000

void create_plane_rand(zVec3D *v, zPlane3D *p)
{
  zVec3D n;

  zVec3DCreate( v, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zVec3DCreate( &n, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DNormalizeDRC( &n );
  zPlane3DCreate( p, v, &n );
}

void create_edge_rand(zVec3D v[], zEdge3D *e)
{
  zVec3DCreate( &v[0], zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zVec3DCreate( &v[1], zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zEdge3DCreate( e, &v[0], &v[1] );
}

void create_tri_rand(zVec3D v[], zTri3D *t)
{
  zVec3DCreate( &v[0], zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zVec3DCreate( &v[1], zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zVec3DCreate( &v[2], zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zTri3DCreate( t, &v[0], &v[1], &v[2] );
}

void assert_edge_contig(void)
{
  zVec3D v[2], p;
  zEdge3D e;
  double d[2], dc;
  register int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_edge_rand( v, &e );
    zVec3DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zEdge3DContigVert( &e, &p, &dc );
    d[0] = zVec3DDist( &v[0], &p );
    d[1] = zVec3DDist( &v[1], &p );
    if( dc != zMin( d[0], d[1] ) ) result = false;
  }
  zAssert( zEdge3DContigVert, result );
}

void assert_tri_contig(void)
{
  zVec3D v[3], p;
  zTri3D t;
  double d[3], dc;
  register int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_tri_rand( v, &t );
    zVec3DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zTri3DContigVert( &t, &p, &dc );
    d[0] = zVec3DDist( &v[0], &p );
    d[1] = zVec3DDist( &v[1], &p );
    d[2] = zVec3DDist( &v[2], &p );
    if( dc != zMin( d[0], zMin( d[1], d[2] ) ) ) result = false;
  }
  zAssert( zTri3DContigVert, result );
}

void assert_plane_proj(void)
{
  zVec3D v, p, cp;
  zPlane3D pl;
  register int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_plane_rand( &v, &pl );
    zVec3DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zPlane3DProj( &pl, &p, &cp );
    if( !zPlane3DPointIsOn( &pl, &cp ) ) result = false;
  }
  zAssert( zPlane3DProj + zPlane3DPointIsOn, result );
}

void assert_tri_proj(void)
{
  zVec3D v[3], p, cp;
  zTri3D t;
  register int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_tri_rand( v, &t );
    zVec3DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zTri3DProj( &t, &p, &cp );
    if( !zTri3DPointIsOn( &t, &cp ) ) result = false;
  }
  zAssert( zTri3DProj + zTri3DPointIsOn, result );
}

void assert_tri_point_is_inside(void)
{
  zVec3D v[3], p;
  zTri3D t;
  register int i;
  double r1, r2;
  bool result1, result2, result3;

  result1 = result2 = result3 = true;
  for( i=0; i<N; i++ ){
    create_tri_rand( v, &t );
    /* inside case */
    zVec3DZero( &p );
    zVec3DCatDRC( &p, ( r1 = zRandF(0,1) ), &v[0] );
    zVec3DCatDRC( &p, ( r2 = zRandF(0,1-r1) ), &v[1] );
    zVec3DCatDRC( &p, 1 - r1 - r2, &v[2] );
    if( !zTri3DPointIsInside( &t, &p, false ) ) result1 = false;
    /* outside case */
    zVec3DZero( &p );
    zVec3DCatDRC( &p, ( r1 = zRandF(0,-1) ), &v[0] );
    zVec3DCatDRC( &p, ( r2 = zRandF(0,1-r1) ), &v[1] );
    zVec3DCatDRC( &p, 1 - r1 - r2, &v[2] );
    if( zTri3DPointIsInside( &t, &p, false ) ) result1 = false;
    /* midpoint case */
    zVec3DMid( &v[0], &v[1], &p );
    if( zTri3DPointIsInside( &t, &p, false ) ) result2 = false;
    /* vertex case */
    zVec3DCopy( &v[2], &p );
    if( zTri3DPointIsInside( &t, &p, false ) ) result3 = false;
  }
  zAssert( zTri3DPointIsInside, result1 );
  zAssert( zTri3DPointIsInside (midpoint case), result2 );
  zAssert( zTri3DPointIsInside (vertex case), result3 );
}

int main(void)
{
  zRandInit();
  assert_plane_proj();
  assert_edge_contig();
  assert_tri_contig();
  assert_tri_proj();
  assert_tri_point_is_inside();
  return 0;
}
