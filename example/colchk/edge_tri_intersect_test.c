#include <zeo/zeo_col.h>

int main(void)
{
  zVec3D v[5], point, proj;
  zEdge3D e;
  zTri3D t;

  zVec3DCreate( &v[0], 0, 0, 0 );
  zVec3DCreate( &v[1], 1, 0, 0 );
  zVec3DCreate( &v[2], 0, 1, 0 );
  zVec3DCreate( &v[3], 0, 0, 1 );
  zVec3DCreate( &v[4], 1, 1, 1 );
  zEdge3DCreate( &e, &v[0], &v[4] );
  zTri3DCreate( &t, &v[1], &v[2], &v[3] );

  printf( "<point>\n" );
  zVec3DPrint( &v[3] );
  printf( "<edge>\n" );
  zEdge3DPrint( &e );
  printf( "<triangle>\n" );
  zTri3DPrint( &t );

  printf( "\n+++ intersection test +++\n" );
  printf( "case1:\n(edge)\n" );
  zEdge3DPrint( &e );
  printf( "(triangle)\n" );
  zTri3DPrint( &t );
  printf( "result = %s\n", zBoolStr( zIntersectEdgeTri3D( &e, &t, &proj ) ) );
  zVec3DPrint( &proj );

  zEdge3DCreate( &e, &v[4], &v[3] );
  printf( "case2:\n(edge)\n" );
  zEdge3DPrint( &e );
  printf( "(triangle)\n" );
  zTri3DPrint( &t );
  printf( "result = %s\n", zBoolStr( zIntersectEdgeTri3D( &e, &t, &proj ) ) );
  zVec3DPrint( &proj );

  zEdge3DCreate( &e, &v[4], &point );
  printf( "case3:\n(edge)\n" );
  zEdge3DPrint( &e );
  printf( "(triangle)\n" );
  zTri3DPrint( &t );
  printf( "result = %s\n", zBoolStr( zIntersectEdgeTri3D( &e, &t, &proj ) ) );
  zVec3DPrint( &proj );

  return 0;
}
