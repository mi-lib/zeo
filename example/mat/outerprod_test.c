#include <zeo/zeo_mat3d.h>

int main(void)
{
  zVec3D a, b, c, v;
/* = { { 1,-2, 0.003 } }, b = { { 5, 0.04,-9 } }, c = { { 2, -7, 30 } }, v;*/
  zMat3D m;

  zRandInit();
  zVec3DCreate( &a, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &b, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &c, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  printf( "vector a: " ); zVec3DWrite( &a );
  printf( "vector b: " ); zVec3DWrite( &b );
  printf( "vector c: " ); zVec3DWrite( &c );

  printf( "a x c = " );
  zVec3DWrite( zVec3DOuterProd( &a, &c, &v ) );
  printf( "[ax]c = " );
  zVec3DOuterProdMat3D( &a, &m );
  zVec3DWrite( zMulMatVec3D( &m, &c, &v ) );

  printf( "a x ( b x c ) = " );
  zVec3DOuterProd( &b, &c, &v );
  zVec3DOuterProd( &a, &v, &v );
  zVec3DWrite( &v );
  printf( " (triple-prod)= " );
  zVec3DWrite( zVec3DTripleProd( &a, &b, &c, &v ) );
  printf( "[a x][b x] c  = " );
  zVec3DOuterProd2Mat3D( &a, &b, &m );
  zVec3DWrite( zMulMatVec3D( &m, &c, &v ) );

  return 0;
}
