#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m, im;
  zVec3D v, miv1, miv2;

  zVec3DCreate( &v, 3, -4, 7 );

  zMat3DCreate( &m, 0, 1, 2, -1, 0, 2, 1, 1, -2 );
  zMat3DInv( &m, &im );
  zMulMatVec3D( &im, &v, &miv1 );
  zMulInvMatVec3D( &m, &v, &miv2 );
  zVec3DWrite( &miv1 );
  zVec3DWrite( &miv2 );

  zMat3DCreate( &m, 0, 0, 3, 4, 1, 0, 1, 0, -5 );
  zMat3DInv( &m, &im );
  zMulMatVec3D( &im, &v, &miv1 );
  zMulInvMatVec3D( &m, &v, &miv2 );
  zVec3DWrite( &miv1 );
  zVec3DWrite( &miv2 );

  zMat3DCreate( &m, 3,-1, 4,-2,-1, 0,-5, -4, 3 );
  zMat3DInv( &m, &im );
  zMulMatVec3D( &im, &v, &miv1 );
  zMulInvMatVec3D( &m, &v, &miv2 );
  zVec3DWrite( &miv1 );
  zVec3DWrite( &miv2 );
  return 0;
}
