#include <zeo/zeo_mat2d.h>

int main(void)
{
  zMat2D m1, m2, m3, m4;

  zRandInit();
  zMat2DCreate( m1, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zMulInvMatMat2D( m1, m1, m3 );
  zMat2DInv( m1, m2 );
  zMulMatMat2D( m2, m1, m4 );
  zMat2DWrite( m3 );
  zMat2DWrite( m4 );
  return 0;
}
