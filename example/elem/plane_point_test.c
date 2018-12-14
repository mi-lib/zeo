#include <zeo/zeo.h>

int main(void)
{
  zPlane3D plane;
  zVec3D po, n, p, v;

  zVec3DCreate( &po, 1, 1, 0 );
  zVec3DCreate( &n, 1, 0, 1 );
  zPlane3DCreate( &plane, &po, &n );
  printf( "plane :\n" ); zPlane3DWrite( &plane );

  printf( "[test 1]\n" );
  zVec3DCreate( &p, 0, 2, 0 );
  printf( "point :" ); zVec3DWrite( &p );
  zPlane3DProj( &plane, &p, &v );
  printf( "proj. :" ); zVec3DWrite( &v );

  printf( "[test 2]\n" );
  zVec3DCreate( &p, 1, 1, 1 );
  printf( "point :" ); zVec3DWrite( &p );
  zPlane3DProj( &plane, &p, &v );
  printf( "proj. :" ); zVec3DWrite( &v );
  return 0;
}
