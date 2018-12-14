#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m1, m2;
  double theta;
  zVec3D err;

  printf( "enter rotation angle [deg]: " );
  scanf( "%lf", &theta );
  getchar();
  theta = zDeg2Rad( theta );

  printf( "(around X axis)\n" );
  zMat3DRotRoll( Z_IDENTMAT3D, theta, &m1 );
  zMat3DZYX( &m2, 0, 0, theta );
  printf( "(by RotRoll) " ); zMat3DWrite( &m1 );
  printf( "(by ZYX) " ); zMat3DWrite( &m2 );
  zMat3DError( &m1, &m2, &err );
  printf( "err: " ); zVec3DWrite( &err );
  printf( "hit enter key." ); fflush( stdout );
  getchar();

  printf( "(around Y axis)\n" );
  zMat3DRotPitch( Z_IDENTMAT3D, theta, &m1 );
  zMat3DZYX( &m2, 0, theta, 0 );
  printf( "(by RotPitch) " ); zMat3DWrite( &m1 );
  printf( "(by ZYX) " ); zMat3DWrite( &m2 );
  zMat3DError( &m1, &m2, &err );
  printf( "err: " ); zVec3DWrite( &err );
  printf( "hit enter key." ); fflush( stdout );
  getchar();

  printf( "(around Z axis)\n" );
  zMat3DRotYaw( Z_IDENTMAT3D, theta, &m1 );
  zMat3DZYX( &m2, theta, 0, 0 );
  printf( "(by RotYaw) " ); zMat3DWrite( &m1 );
  printf( "(by ZYX) " ); zMat3DWrite( &m2 );
  zMat3DError( &m1, &m2, &err );
  printf( "err: " ); zVec3DWrite( &err );

  return 0;
}
