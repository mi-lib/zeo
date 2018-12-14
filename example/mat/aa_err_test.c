#include <zeo/zeo_mat3d.h>

int main(int argc, char *argv[])
{
  zMat3D m1, m2;
  zVec3D a1, a2, em, ea;

  zRandInit();
  zMat3DZYX( &m1, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DZYX( &m2, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DToAA( &m1, &a1 );
  zMat3DToAA( &m2, &a2 );

  zMat3DError( &m1, &m2, &em );
  zAAError( &a1, &a2, &ea );

  zVec3DWrite( &em );
  zVec3DWrite( &ea );
  return 0;
}
