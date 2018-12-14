#include <zeo/zeo_mat3d.h>

/* answer:
  m1.m2 = 12  7 16
          27 13 31
          42 19 46
  m1^T.m2 = 26 17 38
            31 19 43
            36 21 48
  m1.m2^T =  -7 12 19
            -13 27 46
            -19 42 73
 */

int main(void)
{
  zMat3D m1, m2, m;

  zMat3DCreate( &m1, 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  zMat3DCreate( &m2, 1,-1,-2, 1, 1, 3, 3, 2, 4 );
  printf( "m1=" );
  zMat3DWrite( &m1 );
  printf( "m2=" );
  zMat3DWrite( &m2 );
  printf( "m1.m2=" );
  zMulMatMat3D( &m1, &m2, &m );
  zMat3DWrite( &m );
  printf( "m1^T.m2=" );
  zMulMatTMat3D( &m1, &m2, &m );
  zMat3DWrite( &m );
  printf( "m1.m2^T=" );
  zMulMatMatT3D( &m1, &m2, &m );
  zMat3DWrite( &m );
  printf( "see source for verification!\n" );
  return 0;
}
