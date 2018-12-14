#include <zeo/zeo_vec3d.h>

int main(void)
{
  zVec3D v;

  zVec3DCreatePolar( &v, 2, zPI/6, 0 );
  printf( "<2,pi/6,0>    " ); zVec3DWrite( &v );
  zVec3DCreatePolar( &v, 2, zPI/2, zPI/4 );
  printf( "<2,pi/2,pi/4> " ); zVec3DWrite( &v );
  return 0;
}
