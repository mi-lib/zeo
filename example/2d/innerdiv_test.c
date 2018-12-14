#include <zeo/zeo_vec2d.h>

#define N 10
int main(void)
{
  zVec2D v1, v2, v;
  register int i;

  zVec2DCreate( v1, 1, 2 );
  zVec2DCreate( v2, -3, -6 );
  eprintf( "v1: " ); zVec2DFWrite( stderr, v1 );
  eprintf( "v2: " ); zVec2DFWrite( stderr, v2 );

  for( i=0; i<=N; i++ ){
    zVec2DInnerDiv( v1, v2, (double)i/N, v );
    zVec2DDataNLWrite( v );
  }
  return 0;
}
