#include <zeo/zeo_mat3d.h>

int main(void)
{
  zVec6D v1 = { { 1, 2, 3, 0.1, 0.2, 0.3 } }, v2;
  zMat3D m;

  printf( "v1:\n" );
  zVec6DWrite( &v1 ); zEndl();
  zMat3DCopy( Z_IDENTMAT3D, &m );
  zMat3DCreate( &m, 0, 0, 1, 1, 0, 0, 0, 1, 0 );
  printf( "m:\n" );
  zMat3DWrite( &m );

  printf( "v2 = mul(m,v1):\n" );
  zMulMatVec6D( &m, &v1, &v2 );
  zVec6DWrite( &v2 ); zEndl();
  printf( "v1 = mul(m,v1) (direct):\n" );
  zMulMatVec6DDRC( &m, &v1 );
  zVec6DWrite( &v1 ); zEndl();
  return 0;
}
