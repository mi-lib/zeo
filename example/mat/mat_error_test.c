#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m1, m2, m;
  zVec3D err;

  zRandInit();
  zMat3DZYX( &m1, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  printf( "matrix1\n" );
  zMat3DWrite( &m1 );
  zMat3DZYX( &m2, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  printf( "matrix2\n" );
  zMat3DWrite( &m2 );

  zMat3DError( &m2, &m1, &err );
  printf( "error\n" );
  zVec3DWrite( &err );
  zMat3DRot( &m1, &err, &m );
  printf( "err x matrix1 = \n" );
  zMat3DWrite( &m );

  zMulMatTMat3D( &m, &m2, &m );
  zMat3DWrite( &m );
  return 0;
}
