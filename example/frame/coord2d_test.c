#include <zeo/zeo_coord2d.h>

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

int main(void)
{
  zRandInit();
  assert_coord2d_copy();
  assert_coord2d_arithmatics();
  return 0;
}
