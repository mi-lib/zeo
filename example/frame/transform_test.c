#include <zeo/zeo.h>

int main(void)
{
  zFrame3D f;
  zVec3D v1, v2, v, err;

  zRandInit();
  zFrame3DFromZYX( &f, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zVec3DCreate( &v1, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );

  printf( "v1: " ); zVec3DPrint( &v1 );
  printf( "f:\n" ); zFrame3DPrint( &f );
  zXfer3D( &f, &v1, &v2 );
  printf( "v2=f v1: " ); zVec3DPrint( &v2 );
  zXfer3DInv( &f, &v2, &v );
  printf( "v=f^-1 v2: " ); zVec3DPrint( &v );

  printf( "(assert):\n" );
  printf( "v1: " ); zVec3DPrint( &v1 );
  zVec3DSub( &v, &v1, &err );
  printf( "(error): " ); zVec3DPrint( &err );
  return 0;
}
