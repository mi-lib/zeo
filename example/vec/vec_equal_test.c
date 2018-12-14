#include <zeo/zeo_vec3d.h>

int main(void)
{
  zVec3D v1, v2, e;

  zVec3DCreate( &v1, 1.0, 2.0, 3.0 );
  zVec3DCopy( &v1, &v2 );
  printf( "vec1: " ); zVec3DWrite( &v1 );
  printf( "vec2: " ); zVec3DWrite( &v2 );
  printf( "%s\n", zVec3DEqual( &v1, &v2 ) ? "equal" : "inequal" );
  v2.e[zX] += zTOL;
  printf( "vec1: " ); zVec3DWrite( &v1 );
  printf( "vec2: " ); zVec3DWrite( &v2 );
  printf( "%s\n", zVec3DEqual( &v1, &v2 ) ? "equal" : "inequal" );
  zVec3DSub( &v2, &v1, &e );
  printf( "error: " ); zVec3DWrite( &e );
  printf( "%s\n", zVec3DIsTiny( &e ) ? "tiny" : "not tiny" );
  return 0;
}
