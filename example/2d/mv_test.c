#include <zeo/zeo_mat2d.h>

int main(void)
{
  zMat2D m;
  zVec2D v1, v2;

  zRandInit();
  zMat2DCreate( m, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zVec2DCreate( v1, zRandF(-5,5), zRandF(-5,5) );
  printf( "v1        : (%f, %f)\n", v1[0], v1[1] );
  zMulInvMatVec2D( m, v1, v2 );
  printf( "v2=M^-1 v1: (%f, %f)\n", v2[0], v2[1] );
  zMulMatVec2D( m, v2, v1 );
  printf( "v1=M v2   : (%f, %f)\n", v1[0], v1[1] );
  return 0;
}
