#include <zeo/zeo_vec2d.h>

#define N 10
int main(void)
{
  zVec2D v1, v2;
  register int i;

  zVec2DCreate( v1, 2, 0 );
  for( i=0; i<=N; i++ ){
    zVec2DCreatePolar( v2, i+1, 0.5*zPI*i/N );
    printf( "%f\n", zRad2Deg( zVec2DAngle(v1,v2) ) );
  }
  return 0;
}
