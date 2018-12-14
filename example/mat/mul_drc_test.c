#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m;
  zVec3D v1, v2, v3;

  zRandInit();
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m,
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

  zMat3DWrite( &m );
  zVec3DWrite( &v1 );
  zMulMatVec3D( &m, &v1, &v2 );
  zVec3DCopy( &v1, &v3 );
  zMulMatVec3DDRC( &m, &v3 );
  printf( "Mv = " ); zVec3DWrite( &v2 );
  printf( "(drc)" ); zVec3DWrite( &v3 );

  zMulMatTVec3D( &m, &v1, &v2 );
  zVec3DCopy( &v1, &v3 );
  zMulMatTVec3DDRC( &m, &v3 );
  printf( "M^Tv = " ); zVec3DWrite( &v2 );
  printf( "  (drc)" ); zVec3DWrite( &v3 );

  return 0;
}
