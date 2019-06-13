#include <zeo/zeo_prim.h>

int main(void)
{
  zECyl3D ecyl;
  zVec3D c[2], center;
  zPH3D ph;
  zMat3D i;

  zVec3DCreate( &c[0],-0.5,-0.5,-0.5 );
  zVec3DCreate( &c[1], 0.5, 0.5, 0.5 );
  zVec3DMid( &c[0], &c[1], &center );
  zECyl3DCreate( &ecyl, &c[0], &c[1], 0.6, 1.0, ZVEC3DX, 128 );

  printf( "volume = %f\n", zECyl3DVolume( &ecyl ) );
  zECyl3DInertia( &ecyl, &i );
  printf( "inertia:\n" );
  zMat3DPrint( &i );

  printf( "+++ polyhedron approximation\n" );
  zECyl3DToPH( &ecyl, &ph );
  printf( "volume = %f\n", zPH3DVolume( &ph ) );
  zPH3DBaryInertia( &ph, &center, &i );
  printf( "inertia:\n" );
  zMat3DPrint( &i );

  return 0;
}
