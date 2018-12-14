#include <zeo/zeo_vec3d.h>

#define N 500
int main(void)
{
  zVec3D v1, v2, n, aa;
  register int i;
  double t1, t2, t3;

  zRandInit();
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DOuterProd( &v1, &v2, &n );
  zVec3DNormalizeDRC( &n );
  for( i=0; i<=N; i++ ){
    t1 = 2 * zPI * i/N;
    zVec3DMul( &n, t1, &aa );
    zVec3DRot( &v1, &aa, &v2 );
    t1 = zVec3DAngle( &v1, &v2, NULL );
    t2 = zVec3DAngle( &v1, &v2, &n );
    t3 = t2 > 0 ? t2 : t2 + 2*zPI;
    printf( "%f %f %f\n", t1, t2, t3 );
  }
  return 0;
}
