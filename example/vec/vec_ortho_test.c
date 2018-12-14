#include <zeo/zeo_vec3d.h>

int main(void)
{
  zVec3D v1, v2, v3;

  zVec3DCopy( Z_UNITXVEC3D, &v1 );
  zVec3DOrthoSpace( &v1, &v2, &v3 );
  printf( "(ex.1)\n" );
  printf( "v1:" ); zVec3DWrite( &v1 );
  printf( "v2:" ); zVec3DWrite( &v2 );
  printf( "v3:" ); zVec3DWrite( &v3 );
  printf( "(ex.2)\n" );
  zVec3DOrthoSpace( &v2, &v3, &v1 );
  printf( "v2:" ); zVec3DWrite( &v2 );
  printf( "v3:" ); zVec3DWrite( &v3 );
  printf( "v1:" ); zVec3DWrite( &v1 );
  return 0;
}
