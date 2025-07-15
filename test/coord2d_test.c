#include <zeo/zeo_frame2d.h>

#define N 1000

void assert_coord2d_arithmatics(void)
{
  zCoord2D coord1, coord2, coord3, coord;
  double k;

  zCoord2DCreate( &coord1, zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI) );
  zCoord2DCreate( &coord2, zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI) );
  k = zRandF(-10,10);
  zCoord2DAdd( &coord1, &coord2, &coord );
  zAssert( zCoord2DAdd,
    zEqual( coord1.pos.c.x + coord2.pos.c.x, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y + coord2.pos.c.y, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle + coord2.angle, coord.angle, zTOL ) );
  zCoord2DSub( &coord1, &coord2, &coord );
  zAssert( zCoord2DSub,
    zEqual( coord1.pos.c.x - coord2.pos.c.x, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y - coord2.pos.c.y, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle - coord2.angle, coord.angle, zTOL ) );
  zCoord2DAdd( &coord, &coord2, &coord3 );
  zAssert( zCoord2DAdd + zCoord2DSub, zCoord2DEqual( &coord1, &coord3 ) );
  zCoord2DMul( &coord1, k, &coord );
  zAssert( zCoord2DMul,
    zEqual( coord1.pos.c.x * k, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y * k, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle * k, coord.angle, zTOL ) );
  zCoord2DDiv( &coord1, k, &coord );
  zAssert( zCoord2DDiv,
    zEqual( coord1.pos.c.x / k, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y / k, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle / k, coord.angle, zTOL ) );
  zAssert( zCoord2DDiv (zero-division), !zCoord2DDiv( &coord1, 0, &coord ) );
  zCoord2DCat( &coord1, k, &coord2, &coord );
  zAssert( zCoord2DCat,
    zEqual( coord1.pos.c.x + k * coord2.pos.c.x, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y + k * coord2.pos.c.y, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle + k * coord2.angle, coord.angle, zTOL ) );

  zCoord2DCopy( &coord1, &coord );
  zCoord2DAddDRC( &coord, &coord2 );
  zAssert( zCoord2DAddDRC,
    zEqual( coord1.pos.c.x + coord2.pos.c.x, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y + coord2.pos.c.y, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle + coord2.angle, coord.angle, zTOL ) );
  zCoord2DCopy( &coord1, &coord );
  zCoord2DSubDRC( &coord, &coord2 );
  zAssert( zCoord2DSubDRC,
    zEqual( coord1.pos.c.x - coord2.pos.c.x, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y - coord2.pos.c.y, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle - coord2.angle, coord.angle, zTOL ) );
  zCoord2DCopy( &coord1, &coord );
  zCoord2DMulDRC( &coord, k );
  zAssert( zCoord2DMulDRC,
    zEqual( coord1.pos.c.x * k, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y * k, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle * k, coord.angle, zTOL ) );
  zCoord2DCopy( &coord1, &coord );
  zCoord2DDivDRC( &coord, k );
  zAssert( zCoord2DDivDRC,
    zEqual( coord1.pos.c.x / k, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y / k, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle / k, coord.angle, zTOL ) );
  zCoord2DCopy( &coord1, &coord );
  zAssert( zCoord2DDivDRC (zero-division), !zCoord2DDivDRC( &coord, 0 ) );
  zCoord2DCopy( &coord1, &coord );
  zCoord2DCatDRC( &coord, k, &coord2 );
  zAssert( zCoord2DCatDRC,
    zEqual( coord1.pos.c.x + k * coord2.pos.c.x, coord.pos.c.x, zTOL ) &&
    zEqual( coord1.pos.c.y + k * coord2.pos.c.y, coord.pos.c.y, zTOL ) &&
    zEqual( coord1.angle + k * coord2.angle, coord.angle, zTOL ) );
}

void assert_coord2d_copy(void)
{
  zCoord2D coord1, coord2;

  zCoord2DCreate( &coord1, zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI) );
  zCoord2DSetPos( &coord2, zCoord2DPos( &coord1 ) );
  zCoord2DSetAngle( &coord2, zCoord2DAngle( &coord1 ) );
  zAssert( zCoord2DEqual (zCoord2DSetPos + zCoord2DSetAngle), zCoord2DEqual( &coord1, &coord2 ) );
  zCoord2DCreate( &coord2, zCoord2DX(&coord1), zCoord2DY(&coord1), zCoord2DAngle(&coord1) );
  zAssert( zCoord2DEqual (zCoord2DCreate), zCoord2DEqual( &coord1, &coord2 ) );

  zCoord2DCopy( &coord1, &coord2 );
  zAssert( zCoord2DCopy, zCoord2DEqual( &coord1, &coord2 ) );
}

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
  assert_coord2d_copy();
  assert_coord2d_arithmatics();
  assert_xform_cascade();
  assert_frame2d_conv();
  assert_frame2d_cascade();
  return 0;
}
