#include <zeo/zeo.h>

#define TEST_N 1000

int main(void)
{
  zEP ep;
  zMat3D m1, m2;
  zVec3D axis, aa, err;
  double theta;
  int i;

  zRandInit();
  for( i=0; i<TEST_N; i++ ){
    zVec3DCreate( &axis, zRandF(-1.0,1.0), zRandF(-1.0,1.0), zRandF(-1.0,1.0) );
    zVec3DNormalizeDRC( &axis );
    theta = zRandF(-zPI,zPI);
    zVec3DMul( &axis, theta, &aa );
    zMat3DAA( &m1, &aa );
    printf( "original  mat: " ); zMat3DWrite( &m1 );
    zMat3DToEP( &m1, &ep );
    zMat3DEP( &m2, &ep );
    printf( "converted mat: " ); zMat3DWrite( &m2 );
    zMat3DError( &m1, &m2, &err );
    if( !zVec3DIsTiny( &err ) )
      ZRUNWARN( "EP<->SO(3) conversion might have a bug: err=%g ", zVec3DNorm(&err) );
  }
  return 0;
}
