#include <zeo/zeo_mat2d.h>

int main(void)
{
  zMat2D m1, m2, m3, m4;

  zRandInit();
  zMat2DCreate( m1, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zMat2DCreate( m2, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zMat2DWrite( m1 );
  zMulMatMat2D( m2, m1, m3 );
  zMat2DWrite( m3 );
  zMulInvMatMat2D( m2, m3, m4 );
  zMat2DWrite( m4 );
  zMulInvMatMat2D( m1, m4, m3 );
  zMat2DWrite( m3 );
  return 0;
}
