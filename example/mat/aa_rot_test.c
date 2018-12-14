#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m;
  zVec3D aa, p, rp;

  zRandInit();
  zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  printf( "<point> " ); zVec3DWrite( &p );
  zVec3DCreate( &aa, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  printf( "<AA> " ); zVec3DWrite( &aa );

  printf( "+++ rotate point by AA +++\n" );
  zVec3DRot( &p, &aa, &rp );
  zVec3DWrite( &rp );

  printf( "+++ rotate point by equivalent 3x3 matrix +++\n" );
  zMat3DAA( &m, &aa );
  zMulMatVec3D( &m, &p, &rp );
  zVec3DWrite( &rp );
  return 0;
}
