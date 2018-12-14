#include <zeo/zeo.h>

int main(void)
{
  zCone3D con;
  zPH3D ph;
  zVec3D c1, c2;
  zMat3D i;

  zVec3DCreate( &c1, 1, 0, -1 );
  zVec3DCreate( &c2, 1, 1, -1 );
  zCone3DCreate( &con, &c1, &c2, 0.5, 0 );
  zCone3DWrite( &con );
  printf( "height = %f\n", zCone3DHeight(&con) );
  printf( "volume = %f\n", zCone3DVolume(&con) );
  zCone3DInertia( &con, &i );
  printf( "inertia:\n" );
  zMat3DWrite( &i );

  printf( "+++ polyhedron approximation\n" );
  zCone3DToPH( &con, &ph );
  printf( "volume = %f\n", zPH3DVolume( &ph ) );
  zPH3DBaryInertia( &ph, &c1, &i );
  printf( "inertia:\n" );
  zMat3DWrite( &i );
  return 0;
}
