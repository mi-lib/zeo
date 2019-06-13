#include <zeo/zeo_prim.h>

int main(void)
{
  zEllips3D el;
  zPH3D ph;
  zVec3D center, ax, ay, az;
  zMat3D i;

  zVec3DCreate( &center, 0, 0, 0 );
  zVec3DCreate( &ax, 1, 0, 0 );
  zVec3DCreate( &ay, 0,-1.0/sqrt(2), 1.0/sqrt(2) );
  zVec3DOuterProd( &ax, &ay, &az );

  zEllips3DCreate( &el, &center, &ax, &ay, &az, 2, 3, 1, 0 );
  printf( "volume = %f\n", zEllips3DVolume( &el ) );
  zEllips3DInertia( &el, &i );
  printf( "inertia:\n" );
  zMat3DPrint( &i );

  printf( "+++ polyhedron approximation\n" );
  zEllips3DToPH( &el, &ph );
  printf( "volume = %f\n", zPH3DVolume( &ph ) );
  zPH3DBaryInertia( &ph, &center, &i );
  printf( "inertia:\n" );
  zMat3DPrint( &i );

  return 0;
}
