#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m1, m2, m;
  zVec3D aa, waa;

  zRandInit();
  /* source matrix */
  zVec3DCreate( &aa, zRandF(-zPI,zPI)/3, zRandF(-zPI,zPI)/3, zRandF(-zPI,zPI)/3 );
  zMat3DAA( &m1, &aa );
  /* deviation matrix */
  zVec3DCreate( &aa, zRandF(-zPI,zPI)/3, zRandF(-zPI,zPI)/3, zRandF(-zPI,zPI)/3 );
  printf( "rotation vector a = " );
  zVec3DWrite( &aa );
  /* local rotation */
  printf( "R_A R_xi(a) = " );
  zMat3DAA( &m2, &aa );
  zMulMatMat3D( &m1, &m2, &m );
  zMat3DWrite( &m );
  /* world rotation */
  printf( "R_xi(R_A a) R_A = " );
  zMulMatVec3D( &m1, &aa, &waa );
  zMat3DAA( &m2, &waa );
  zMulMatMat3D( &m2, &m1, &m );
  zMat3DWrite( &m );

  return 0;
}
