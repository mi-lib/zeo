#include <zeo/zeo_frame2d.h>

#define N 1000

void assert_xform_cascade(void)
{
  zCoord2D c1, c2, c3, c4;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    zCoord2DCreate( &c1, zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI) );
    zCoord2DCreate( &c2, zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI) );
    zCoord2DXform( &c1, &c2, &c3 );
    zCoord2DCascade( &c1, &c3, &c4 );
    if( !zCoord2DEqual( &c2, &c4 ) ) result = false;
  }
  zAssert( zCoord2DXform + zCoord2DCascade, result );
}

void assert_frame2d_conv(void)
{
  zCoord2D c1, c2;
  zFrame2D f;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    zCoord2DCreate( &c1, zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI) );
    zFrame2DFromCoord2D( &f, &c1 );
    zCoord2DFromFrame2D( &c2, &f );
    if( !zCoord2DEqual( &c1, &c2 ) ) result = false;
  }
  zAssert( zFrame2DFromCoord2D + zCoord2DFromFrame2D, result );
}

void assert_frame2d_cascade(void)
{
  zCoord2D c1, c2, c3, c4;
  zFrame2D f1, f2, f3;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    zCoord2DCreate( &c1, zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI) );
    zCoord2DCreate( &c2, zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI) );
    zCoord2DCascade( &c1, &c2, &c3 );
    zCoord2DPhaseNormalize( &c3 );
    zFrame2DFromCoord2D( &f1, &c1 );
    zFrame2DFromCoord2D( &f2, &c2 );
    zFrame2DCascade( &f1, &f2, &f3 );
    zCoord2DFromFrame2D( &c4, &f3 );
    if( !zCoord2DEqual( &c3, &c4 ) ) result = false;
  }
  zAssert( zCoord2DCascade / zFrame2DFromCoord2D, result );
}

int main(void)
{
  zRandInit();
  assert_xform_cascade();
  assert_frame2d_conv();
  assert_frame2d_cascade();
  return 0;
}
