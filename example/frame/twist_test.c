#include <zeo/zeo_frame.h>

int main(int argc, char *argv[])
{
  zFrame3D f1, f2;
  zVec6D t, d, err;

  zRandInit();
  /* twist */
  zVec6DCreate( &t, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1), zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DMulDRC( zVec6DAng(&t), zRandF(0,zPI)/zVec3DNorm(zVec6DAng(&t)) );
  /* original frame */
  zFrame3DZYX( &f1, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1), zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );

  zFrame3DTwist( &f1, &t, &f2 );
  zFrame3DError( &f2, &f1, &d );
  printf( "twist:\n" );
  zVec6DWrite( &t );
  printf( "deviation:\n" );
  zVec6DWrite( &d );
  printf( "(error):\n" );
  zVec6DWrite( zVec6DSub(&t,&d,&err) );
  return 0;
}
