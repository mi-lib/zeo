#include <zeo/zeo_mat3d.h>

#define V    0.001
#define STEP 100

int main(void)
{
  zVec3D zyzvel, angvel, zyz;
  zMat3D m;

  zRandInit();
  zVec3DCreate( &zyzvel, zRandF(-V,V), zRandF(-V,V), zRandF(-V,V) );
  zMat3DZYZ( &m, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DToZYZ( &m, &zyz );

  printf( "attitude)\n" );
  zMat3DWrite( &m );
  printf( " -> " );
  zVec3DWrite( &zyz );
  printf( "z-y-z Eulerian angle velocity)\n" );
  zVec3DWrite( &zyzvel );
  /* z-y-x Eulerian angle velocity -> angular velocity */
  zVec3DZYZVel2AngVel( &zyzvel, &zyz, &angvel );
  printf( "equivalent angular velocity)\n" );
  zVec3DWrite( &angvel );
  /* angular velocity -> z-y-x Eulerian angle velocity */
  zVec3DAngVel2ZYZVel( &angvel, &zyz, &zyzvel );
  printf( "inverse conversion)\n" );
  zVec3DWrite( &zyzvel );

  return 0;
}
