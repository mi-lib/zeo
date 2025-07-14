/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_coord2d - 2D coordinate.
 */

#include <zeo/zeo_coord2d.h>

/* create a 2D coordinate. */
zCoord2D *zCoord2DCreate(zCoord2D *coord, double x, double y, double angle)
{
  zCoord2DX(coord) = x;
  zCoord2DY(coord) = y;
  zCoord2DSetAngle( coord, angle );
  return coord;
}

/* check if two 2D coordinates are same or not. */
bool zCoord2DEqual(const zCoord2D *coord1, const zCoord2D *coord2)
{
  return _zCoord2DEqual( coord1, coord2 );
}

/* add two 2D coordinates. */
zCoord2D *zCoord2DAdd(const zCoord2D *coord1, const zCoord2D *coord2, zCoord2D *coord)
{
  _zCoord2DAdd( coord1, coord2, coord );
  return coord;
}

/* subtract a 2D coordinate from another. */
zCoord2D *zCoord2DSub(const zCoord2D *coord1, const zCoord2D *coord2, zCoord2D *coord)
{
  _zCoord2DSub( coord1, coord2, coord );
  return coord;
}

/* concatenate a 2D coordinate to another. */
zCoord2D *zCoord2DCat(const zCoord2D *coord1, double k, const zCoord2D *coord2, zCoord2D *coord)
{
  _zCoord2DCat( coord1, k, coord2, coord );
  return coord;
}

/* add a 2D coordinate directly to another. */
zCoord2D *zCoord2DAddDRC(zCoord2D *coord1, const zCoord2D *coord2)
{
  _zCoord2DAddDRC( coord1, coord2 );
  return coord1;
}

/* subtract a 2D coordinate directly from another. */
zCoord2D *zCoord2DSubDRC(zCoord2D *coord1, const zCoord2D *coord2)
{
  _zCoord2DSubDRC( coord1, coord2 );
  return coord1;
}

/* concatenate a 2D coordinate directly to another. */
zCoord2D *zCoord2DCatDRC(zCoord2D *coord1, double k, const zCoord2D *coord2)
{
  _zCoord2DCatDRC( coord1, k, coord2 );
  return coord1;
}

/* interior division of two 2D coordinates. */
zCoord2D *zCoord2DInterDiv(const zCoord2D *coord1, const zCoord2D *coord2, double ratio, zCoord2D *coord)
{
  zVec2DInterDiv( zCoord2DPos(coord1), zCoord2DPos(coord2), ratio, zCoord2DPos(coord) );
  zCoord2DSetAngle( coord, zCoord2DAngle(coord1)+ratio*(zCoord2DAngle(coord2)-zCoord2DAngle(coord1)) );
  return coord;
}

/* find a transformation of a 2D coordinate to another. */
zCoord2D *zCoord2DXform(const zCoord2D *src, const zCoord2D *target, zCoord2D *xform)
{
  zVec2D d;

  zCoord2DSetAngle( xform, zCoord2DAngle(target) - zCoord2DAngle(src) );
  zVec2DSub( zCoord2DPos(target), zCoord2DPos(src), &d );
  zVec2DRot( &d, -zCoord2DAngle(src), zCoord2DPos(xform) );
  return xform;
}

/* cascade a 2D coordinate to another. */
zCoord2D *zCoord2DCascade(const zCoord2D *src, const zCoord2D *xform, zCoord2D *dest)
{
  zVec2DRot( zCoord2DPos(xform), zCoord2DAngle(src), zCoord2DPos(dest) );
  zVec2DAddDRC( zCoord2DPos(dest), zCoord2DPos(src) );
  zCoord2DSetAngle( dest, zCoord2DAngle(src) + zCoord2DAngle(xform) );
  return dest;
}

/* print a 2D coordinate out to a file. */
void zCoord2DFPrint(FILE *fp, const zCoord2D *coord)
{
  if( !coord ){
    fprintf( fp, "(null 2D coord)\n" );
    return;
  }
  zVec2DFPrint( fp, zCoord2DPos(coord) );
  fprintf( fp, "%.10g\n", zCoord2DAngle(coord) );
}
