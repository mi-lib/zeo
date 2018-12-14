#include <zeo/zeo_vec3d.h>

#define N 10
int main(void)
{
  zVec3D v1, v2, v;
  register int i;
  double r;

  zVec3DCreate( &v1,-2, 4, 6 );
  zVec3DCreate( &v2, 6, 0, 2 );
  eprintf( "point1: " );
  zVec3DFWrite( stderr, &v1 );
  eprintf( "point2: " );
  zVec3DFWrite( stderr, &v2 );

  for( i=0; i<=N; i++ ){
    r = (double)i / N;
    zVec3DInnerDiv( &v1, &v2, r, &v );
    printf( "ratio=%f: ", r ); zVec3DDataNLWrite( &v );
  }
  return 0;
}
