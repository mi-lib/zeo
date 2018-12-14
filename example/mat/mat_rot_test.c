#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m, rm;
  zVec3D aa, n;
  double theta;
  int i;

  printf( "enter rotation axis: " );
  zVec3DRead( &n );
  zVec3DNormalizeDRC( &n );
  printf( "entered axis vector (normalized):\n" );
  zVec3DWrite( &n );
  zMat3DIdent( &m );
  printf( "<original matrix>\n" );
  zMat3DWrite( &m );
  for( i=1; i<=8; i++ ){
    theta = zDeg2Rad( 45.0*i );
    printf( "<%d[deg] rotation>\n", 45*i );
    zVec3DMul( &n, theta, &aa );
    zMat3DRot( &m, &aa, &rm );
    zMat3DWrite( &rm );
    getchar();
  }
  return 0;
}
