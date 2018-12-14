#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m1, m;
  double theta;
  zVec3D aa;

  zRandInit();
  zMat3DZYX( &m1, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  printf( "<original matrix> (generated at random)\n" );
  zMat3DWrite( &m1 );
  printf( "enter rotation angle [deg]: " );
  scanf( "%lf", &theta );
  getchar();
  theta = zDeg2Rad( theta );

  printf( "(around X axis)\n" );
  zMat3DCopy( &m1, &m );
  zMat3DRotRollDRC( &m, theta );
  zMat3DWrite( &m );
  zMat3DError( &m, &m1, &aa );
  printf( "<<ensurance>> %f[deg] ", zRad2Deg(zVec3DNorm(&aa)) );
  zVec3DWrite( zVec3DNormalizeDRC(&aa) );
  printf( "hit enter key." ); fflush( stdout );
  getchar();

  printf( "(around Y axis)\n" );
  zMat3DCopy( &m1, &m );
  zMat3DRotPitchDRC( &m, theta );
  zMat3DWrite( &m );
  zMat3DError( &m, &m1, &aa );
  printf( "<<ensurance>> %f[deg] ", zRad2Deg(zVec3DNorm(&aa)) );
  zVec3DWrite( zVec3DNormalizeDRC(&aa) );
  printf( "hit enter key." ); fflush( stdout );
  getchar();

  printf( "(around Z axis)\n" );
  zMat3DCopy( &m1, &m );
  zMat3DRotYawDRC( &m, theta );
  zMat3DWrite( &m );
  zMat3DError( &m, &m1, &aa );
  printf( "<<ensurance>> %f[deg] ", zRad2Deg(zVec3DNorm(&aa)) );
  zVec3DWrite( zVec3DNormalizeDRC(&aa) );

  return 0;
}
