#include <zeo/zeo_mat3d.h>

void output_mat(zMat3D *m, char *str)
{
  printf( ">>%s<<\n", str );
  printf( "[component]\n" );
  zMat3DWrite( m );
}

int main(void)
{
  zMat3D m1, m2, m;

  zMat3DZYX( &m1, zDeg2Rad(45), 0, 0 );
  zMat3DZYX( &m2, -zDeg2Rad(45), zDeg2Rad(30), 0 );
  output_mat( &m1, "m1" );
  output_mat( &m2, "m2" );
  zMat3DAdd( &m1, &m2, &m );
  output_mat( &m, "m1+m2" );
  zMat3DSub( &m1, &m2, &m );
  output_mat( &m, "m1-m2" );
  zMat3DRev( &m1, &m );
  output_mat( &m, "-m1" );
  zMat3DMul( &m1, -2, &m );
  output_mat( &m, "m1*(-2)" );
  zMat3DDiv( &m1, -2, &m );
  output_mat( &m, "m1/(-2)" );
  return 0;
}
