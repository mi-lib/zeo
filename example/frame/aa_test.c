#include <zeo/zeo_frame3d.h>

int main(void)
{
  zFrame3D f1, f2;
  zVec6D v, err;

  zRandInit();
  zVec3DCreate( zVec6DLin(&v), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( zVec6DAng(&v), zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DNormalizeDRC( zVec6DAng(&v) );
  zVec3DMulDRC( zVec6DAng(&v), zRandF(0,zPI) );

  printf( "[original 6D vector]\n" ); zVec6DPrint( &v );
  zVec6DToFrame3DAA( &v, &f1 );
  printf( "[frame]\n" );
  zFrame3DPrint( &f1 );
  zFrame3DToVec6DAA( &f1, &v );
  printf( "[recovered 6D vector]\n" ); zVec6DPrint( &v );
  zVec6DToFrame3DAA( &v, &f2 );
  printf( "(assert)\n" );
  zFrame3DPrint( &f2 );
  zFrame3DError( &f1, &f2, &err );
  printf( "(error)\n" ); zVec6DPrint( &err );
  return 0;
}
