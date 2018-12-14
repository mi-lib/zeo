#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m, im, mi1, mi2;

  zRandInit();
  zMat3DCreate( &m,
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DInv( &m, &im );
  zMulMatMat3D( &m, &im, &mi1 );
  zMulInvMatMat3D( &m, &m, &mi2 );
  zMat3DWrite( &mi1 );
  zMat3DWrite( &mi2 );
  return 0;
}
