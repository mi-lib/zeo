#include <zeo/zeo_vec3d.h>

int main(void)
{
  zVec3D v, n;

  printf( "normalize " );
  zVec3DWrite( zVec3DCreate( &v, 1, 2, 3 ) );
  printf( "-> " );
  zVec3DWrite( zVec3DNormalize( &v, &n ) );
  printf( "(norm) = %g\n", zVec3DNorm(&n) );

  printf( "\n directly normalize " );
  zVec3DWrite( &v );
  printf( "-> " );
  zVec3DWrite( zVec3DNormalizeDRC( &v ) );
  printf( "(norm) = %g\n", zVec3DNorm(&v) );
  return 0;
}
