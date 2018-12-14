#include <zeo/zeo_vec2d.h>

#define N 10
int main(void)
{
  zVec2D v1, v2, p;
  register int i;

  zVec2DCreate( v1, 1, 2 );
  for( i=0; i<=N; i++ ){
    zVec2DRot( v1, 0.5*zPI*i/N, v2 );
    zVec2DProject( v2, v1, p );
    printf( "%f %f %f\n", zRad2Deg( zVec2DAngle(v1,v2) ), zRad2Deg( zVec2DAngle(v1,p) ), zVec2DNorm(p) );
  }
  return 0;
}
