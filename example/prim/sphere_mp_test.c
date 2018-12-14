#include <zeo/zeo_prim.h>

int main(void)
{
  zSphere3D sphere;
  zPH3D ph;
  zVec3D center;
  zMat3D i;

  zVec3DCreate( &center, 0, 0, 2 );
  zSphere3DCreate( &sphere, &center, 1, 0 );
  printf( "volume = %f\n", zSphere3DVolume( &sphere ) );
  zSphere3DInertia( &sphere, &i );
  printf( "inertia:\n" );
  zMat3DWrite( &i );

  printf( "+++ polyhedron approximation\n" );
  zSphere3DToPH( &sphere, &ph );
  printf( "volume = %f\n", zPH3DVolume( &ph ) );
  zPH3DBaryInertia( &ph, &center, &i );
  printf( "inertia:\n" );
  zMat3DWrite( &i );
  return 0;
}
