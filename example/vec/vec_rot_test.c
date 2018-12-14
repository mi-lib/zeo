#include <zeo/zeo_vec3d.h>

int main(void)
{
  zVec3D v, rv, aa, n;
  double theta;
  int i;

  printf( "enter rotation axis: " );
  zVec3DRead( &n );
  zVec3DNormalizeDRC( &n );
  printf( "entered axis vector (normalized):\n" );
  zVec3DWrite( &n );
  zVec3DCopy( Z_UNITXVEC3D, &v );
  printf( "<original vector>\n" );
  zVec3DWrite( &v );
  for( i=1; i<=8; i++ ){
    theta = zDeg2Rad( 45.0*i );
    printf( "<%d[deg] rotation>\n", 45*i );
    zVec3DMul( &n, theta, &aa );
    zVec3DRot( &v, &aa, &rv );
    zVec3DWrite( &rv );
    getchar();
  }
  return 0;
}
