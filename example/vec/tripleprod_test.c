#include <zeo/zeo_vec3d.h>

zVec3D *triple_prod_test(zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v)
{
  zVec3D tmp;

  zVec3DOuterProd( v2, v3, &tmp );
  return zVec3DOuterProd( v1, &tmp, v );
}

int main(void)
{
  zVec3D a, b, c, d;

  zRandInit();
  zVec3DCreate( &a, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &b, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &c, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

  printf( "vector a = " ); zVec3DWrite( &a );
  printf( "vector b = " ); zVec3DWrite( &b );
  printf( "vector c = " ); zVec3DWrite( &c );
  zVec3DTripleProd( &a, &b, &c, &d );
  printf( "ax (bx c)= " ); zVec3DWrite( &d );
  printf( "<confirmation>\n" );
  triple_prod_test( &a, &b, &c, &d );
  printf( "ax (bx c)= " ); zVec3DWrite( &d );
  return 0;
}
