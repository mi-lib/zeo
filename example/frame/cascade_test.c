#include <zeo/zeo.h>

int main(void)
{
  zFrame3D f1, f2, f3, f13, f;
  zVec6D err;

  zRandInit();
  zFrame3DFromZYX( &f1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zFrame3DFromZYX( &f2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  printf( "f1:\n" ); zFrame3DPrint( &f1 );
  printf( "f2:\n" ); zFrame3DPrint( &f2 );

  zFrame3DCascade( &f1, &f2, &f3 );
  printf( "f3 = f1 f2:\n" ); zFrame3DPrint( &f3 );
  zFrame3DXform( &f1, &f3, &f13 );
  printf( "f13= f3 f1^-1:\n" ); zFrame3DPrint( &f13 );
  zFrame3DCascade( &f1, &f13, &f );

  printf( "(assert):\n" );
  printf( "f = f1 f13:\n" ); zFrame3DPrint( &f );
  zFrame3DError( &f, &f3, &err );
  printf( "(error):\n" ); zVec6DPrint( &err );
  return 0;
}
