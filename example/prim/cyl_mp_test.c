#include <zeo/zeo_prim.h>

int main(void)
{
  zCyl3D cyl;
  zPH3D ph;
  zVec3D c1, c2;
  zMat3D i;

  zVec3DCreate( &c1, 1, 0, -1 );
  zVec3DCreate( &c2, 1, 1, -1 );
  zCyl3DCreate( &cyl, &c1, &c2, 0.5, 0 );
  zCyl3DPrint( &cyl );
  printf( "height = %f\n", zCyl3DHeight(&cyl) );
  printf( "volume = %f\n", zCyl3DVolume(&cyl) );
  zCyl3DInertia( &cyl, &i );
  printf( "inertia:\n" );
  zMat3DPrint( &i );

  printf( "+++ polyhedron approximation\n" );
  zCyl3DToPH( &cyl, &ph );
  printf( "volume = %f\n", zPH3DVolume( &ph ) );
  zPH3DBaryInertia( &ph, &c1, &i );
  printf( "inertia:\n" );
  zMat3DPrint( &i );
  return 0;
}
