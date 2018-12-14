#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m, im, mm;

  zMat3DCreate( &m, 0, 1, 2, -1, 0, 2, 1, 1, -2 );
  zMat3DInv( &m, &im );
  zMulMatMat3D( &m, &im, &mm );
  zMat3DWrite( &mm );

  zMat3DCreate( &m, 0, 0, 3, 4, 1, 0, 1, 0, -5 );
  zMat3DInv( &m, &im );
  zMulMatMat3D( &m, &im, &mm );
  zMat3DWrite( &mm );

  zMat3DCreate( &m, 3,-1, 4,-2,-1, 0,-5, -4, 3 );
  zMat3DInv( &m, &im );
  zMulMatMat3D( &m, &im, &mm );
  zMat3DWrite( &mm );

  return 0;
}
