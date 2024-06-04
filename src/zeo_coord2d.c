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
bool zCoord2DEqual(zCoord2D *coord1, zCoord2D *coord2)
{
  return _zCoord2DEqual( coord1, coord2 );
}

/* interior division of two 2D coordinates. */
zCoord2D *zCoord2DInterDiv(zCoord2D *coord1, zCoord2D *coord2, double ratio, zCoord2D *coord)
{
  zVec2DInterDiv( zCoord2DPos(coord1), zCoord2DPos(coord2), ratio, zCoord2DPos(coord) );
  zCoord2DSetAngle( coord, zCoord2DAngle(coord1)+ratio*(zCoord2DAngle(coord2)-zCoord2DAngle(coord1)) );
  return coord;
}

/* find a transformation of a 2D coordinate to another. */
zCoord2D *zCoord2DXform(zCoord2D *src, zCoord2D *dest, zCoord2D *xform)
{
  zVec2D d;

  zCoord2DSetAngle( xform, zCoord2DAngle(dest) - zCoord2DAngle(src) );
  zVec2DSub( zCoord2DPos(dest), zCoord2DPos(src), &d );
  zVec2DRot( &d, -zCoord2DAngle(src), zCoord2DPos(xform) );
  return xform;
}

/* cascade a 2D coordinate to another. */
zCoord2D *zCoord2DCascade(zCoord2D *src, zCoord2D *xform, zCoord2D *dest)
{
  zVec2DRot( zCoord2DPos(xform), zCoord2DAngle(src), zCoord2DPos(dest) );
  zVec2DAddDRC( zCoord2DPos(dest), zCoord2DPos(src) );
  zCoord2DSetAngle( dest, zCoord2DAngle(src) + zCoord2DAngle(xform) );
  return dest;
}

/* print a 2D coordinate out to a file. */
void zCoord2DFPrint(FILE *fp, zCoord2D *coord)
{
  if( !coord ){
    fprintf( fp, "(null 2D coord)\n" );
    return;
  }
  zVec2DFPrint( fp, zCoord2DPos(coord) );
  fprintf( fp, "%.10g\n", zCoord2DAngle(coord) );
}
