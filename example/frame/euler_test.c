#include <zeo/zeo_frame3d.h>

void output_vec6d(zVec6D *v, const char *title, const char *lin, const char *ang)
{
  printf( "[%s]\n", title );
  printf( "%s: ", lin ); zVec3DPrint( zVec6DLin(v) );
  printf( "%s: ", ang ); zVec3DPrint( zVec6DAng(v) );
}

void zyx_test(void)
{
  zFrame3D f;
  zVec6D v;

  zVec6DCreate( &v,
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );

  output_vec6d( &v, "original values", "x-y-z", "azimuth-elevation-tilt" );
  zVec6DToFrame3DZYX( &v, &f );
  printf( "[frame]\n" );
  zFrame3DPrint( &f );
  zFrame3DToVec6DZYX( &f, &v );
  output_vec6d( &v, "recovered values", "x-y-z", "azimuth-elevation-tilt" );
  zVec6DToFrame3DZYX( &v, &f );
  printf( "(assert)\n" );
  zFrame3DPrint( &f );
}

void zyz_test(void)
{
  zFrame3D f;
  zVec6D v;

  zVec6DCreate( &v,
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI) );

  output_vec6d( &v, "original values", "z-y-z", "heading-pitch-bank" );
  zVec6DToFrame3DZYZ( &v, &f );
  printf( "[frame]\n" );
  zFrame3DPrint( &f );
  zFrame3DToVec6DZYZ( &f, &v );
  output_vec6d( &v, "recovered values", "z-y-z", "heading-pitch-bank" );
  zVec6DToFrame3DZYZ( &v, &f );
  printf( "(assert)\n" );
  zFrame3DPrint( &f );
}

int main(void)
{
  zRandInit();
  zyx_test();
  zEndl();
  zyz_test();
  return 0;
}
