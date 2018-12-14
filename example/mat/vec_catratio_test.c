#include <zeo/zeo_mat3d.h>

int main(void)
{
  double ratio[3];
  zVec3D u[3], v;

  zRandInit();
  zVec3DCreate( &u[0], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &u[1], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &u[2], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );

  printf( "enter three values: " );
  ratio[0] = zFDouble( stdin );
  ratio[1] = zFDouble( stdin );
  ratio[2] = zFDouble( stdin );
  zVec3DMul( &u[0], ratio[0], &v );
  zVec3DCatDRC( &v, ratio[1], &u[1] );
  zVec3DCatDRC( &v, ratio[2], &u[2] );

  ratio[0] = ratio[1] = ratio[2] = 0; /* dummy */

  zVec3DCatRatio( &u[0], &u[1], &u[2], &v, ratio );
  printf( "concatenate ratio: %g %g %g\n", ratio[0], ratio[1], ratio[2] );

  return 0;
}
