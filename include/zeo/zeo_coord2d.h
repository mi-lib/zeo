/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_coord2d - 2D coordinate.
 */

#ifndef __ZEO_COORD2D_H__
#define __ZEO_COORD2D_H__

#include <zeo/zeo_mat2d.h>

__BEGIN_DECLS

/*! \brief 2D coordinate class.
 * zCoord2D is a combination of a planar position vector and a rotation angle on the same plane.
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zCoord2D ){
  zVec2D pos;   /*!< 2D position vector. */
  double angle; /*!< 2D rotation angle. */
};

#define zCoord2DX(coord)     (coord)->pos.c.x
#define zCoord2DY(coord)     (coord)->pos.c.y
#define zCoord2DPos(coord)   ( &(coord)->pos )
#define zCoord2DAngle(coord) (coord)->angle

#define zCoord2DSetPos(coord,pos)     zVec2DCopy( pos, zCoord2DPos(coord) )
#define zCoord2DSetAngle(coord,angle) ( zCoord2DAngle(coord) = (angle) )

#define zCoord2DPhaseNormalize(coord) zCoord2DSetAngle( coord, zPhaseNormalize( zCoord2DAngle(coord) ) )

/*! \brief create a 2D coordinate. */
__ZEO_EXPORT zCoord2D *zCoord2DCreate(zCoord2D *coord, double x, double y, double angle);

/*! \brief check if two 2D coordinates are same or not. */
#define _zCoord2DEqual(coord1,coord2) \
  ( _zVec2DEqual( zCoord2DPos(coord1), zCoord2DPos(coord2) ) && \
    zIsEqual( zCoord2DAngle(coord1), zCoord2DAngle(coord2), zTOL ) )
__ZEO_EXPORT bool zCoord2DEqual(zCoord2D *coord1, zCoord2D *coord2);

/*! \brief interior division of two 2D coordinates. */
__ZEO_EXPORT zCoord2D *zCoord2DInterDiv(zCoord2D *coord1, zCoord2D *coord2, double ratio, zCoord2D *coord);

/*! \brief find a transformation of a 2D coordinate to another. */
__ZEO_EXPORT zCoord2D *zCoord2DXform(zCoord2D *src, zCoord2D *dest, zCoord2D *xform);

/*! \brief cascade a 2D coordinate to another. */
__ZEO_EXPORT zCoord2D *zCoord2DCascade(zCoord2D *src, zCoord2D *xform, zCoord2D *dest);
#define zCoord2DCascadeDRC(coord,dc) zCoord2DCascade( coord, dc, coord );

/*! \brief print a 2D coordinate out to a file. */
__ZEO_EXPORT void zCoord2DFPrint(FILE *fp, zCoord2D *coord);
#define zCoord2DPrint(coord) zCoord2DFPrint( stdout, (coord) )

__END_DECLS

#endif /* __ZEO_COORD2D_H__ */
