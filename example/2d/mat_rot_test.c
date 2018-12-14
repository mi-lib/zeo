#include <zeo/zeo_mat2d.h>

#define N 10
int main(void)
{
  zMat2D m1, m2;
  register int i;

  zMat2DIdent( m1 );
  for( i=0; i<=N; i++ ){
    zMat2DRot( m1, 0.5*zPI*i/N, m2 );
    printf( "%f\n", zRad2Deg(zMat2DError(m2,m1)) );
  }
  return 0;
}
