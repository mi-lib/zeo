#include <zeo/zeo.h>

int main(void)
{
  zBox3D box;
  zPH3D ph;
  zVec3D center, ax, ay, az;
  zMat3D i;

  zVec3DCreate( &center, 1, 2, 3 );
  zVec3DCreate( &ax, 1, 0, 0 );
  zVec3DCreate( &ay, 0,-1.0/sqrt(2), 1.0/sqrt(2) );
  zVec3DOuterProd( &ax, &ay, &az );

  zBox3DCreate( &box, &center, &ax, &ay, &az, 5.0, 1.0, 3.0 );
  zBox3DPrint( &box );
  printf( "volume = %f\n", zBox3DVolume(&box) );
  zBox3DInertia( &box, &i );
  printf( "inertia:\n" );
  zMat3DPrint( &i );

  printf( "+++ polyhedron approximation\n" );
  zBox3DToPH( &box, &ph );
  printf( "volume = %f\n", zPH3DVolume( &ph ) );
  zPH3DBaryInertia( &ph, &center, &i );
  printf( "inertia:\n" );
  zMat3DPrint( &i );
  return 0;
}
