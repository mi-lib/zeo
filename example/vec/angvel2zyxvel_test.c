#include <zeo/zeo_mat3d.h>

#define V    0.001
#define STEP 100

int main(void)
{
  zVec3D zyxvel, angvel, zyx;
  zMat3D m;

  zRandInit();
  zVec3DCreate( &zyxvel, zRandF(-V,V), zRandF(-V,V), zRandF(-V,V) );
  zMat3DZYX( &m, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DToZYX( &m, &zyx );

  printf( "attitude)\n" );
  zMat3DWrite( &m );
  printf( " -> " );
  zVec3DWrite( &zyx );
  printf( "z-y-z Eulerian angle velocity)\n" );
  zVec3DWrite( &zyxvel );
  /* z-y-x Eulerian angle velocity -> angular velocity */
  zVec3DZYXVel2AngVel( &zyxvel, &zyx, &angvel );
  printf( "equivalent angular velocity)\n" );
  zVec3DWrite( &angvel );
  /* angular velocity -> z-y-x Eulerian angle velocity */
  zVec3DAngVel2ZYXVel( &angvel, &zyx, &zyxvel );
  printf( "inverse conversion)\n" );
  zVec3DWrite( &zyxvel );

  return 0;
}
