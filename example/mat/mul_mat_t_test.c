#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m, tm;
  zVec3D v1, v2, v3;

  zRandInit();
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m,
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DT( &m, &tm );

  zMulMatTVec3D( &m, &v1, &v2 );
  zMulMatVec3D( &tm, &v1, &v3 );
  zVec3DWrite( &v2 );
  zVec3DWrite( &v3 );

  return 0;
}
