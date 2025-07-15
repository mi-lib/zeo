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
    zEqual( zCoord2DAngle(coord1), zCoord2DAngle(coord2), zTOL ) )
__ZEO_EXPORT bool zCoord2DEqual(const zCoord2D *coord1, const zCoord2D *coord2);

/*! \brief copy a 2D coordinate to another. */
#define zCoord2DCopy(src,dest) zCopy( zCoord2D, src, dest )

/*! \brief add two 2D coordinates.
 *
 * zCoord2DAdd() adds two 2D coordinates \a coord1 and \a coord2, and puts the result into \a coord.
 * \return
 * zCoord2DAdd() returns a pointer \a coord.
 */
#define _zCoord2DAdd(coord1,coord2,coord) do{ \
  _zVec2DAdd( zCoord2DPos(coord1), zCoord2DPos(coord2), zCoord2DPos(coord) ); \
  zCoord2DAngle(coord) = zCoord2DAngle(coord1) + zCoord2DAngle(coord2); \
} while(0)
__ZEO_EXPORT zCoord2D *zCoord2DAdd(const zCoord2D *coord1, const zCoord2D *coord2, zCoord2D *coord);

/*! \brief subtract a 2D coordinate from another.
 *
 * zCoord2DSub() subtracts a 2D coordinate \a coord2 from another \a coord1, and puts the result into \a coord.
 * \return
 * zCoord2DSub() returns a pointer \a coord.
 */
#define _zCoord2DSub(coord1,coord2,coord) do{ \
  _zVec2DSub( zCoord2DPos(coord1), zCoord2DPos(coord2), zCoord2DPos(coord) ); \
  zCoord2DAngle(coord) = zCoord2DAngle(coord1) - zCoord2DAngle(coord2); \
} while(0)
__ZEO_EXPORT zCoord2D *zCoord2DSub(const zCoord2D *coord1, const zCoord2D *coord2, zCoord2D *coord);

/*! \brief multiply a 2D coordinate by a scalar value.
 *
 * zCoord2DMul() multiplies a 2D coordinate \a coord1 by a scalar value \a k, and puts the result into \a coord.
 * \return
 * zCoord2DMul() returns a pointer \a coord.
 */
#define _zCoord2DMul(coord1,k,coord) do{ \
  _zVec2DMul( zCoord2DPos(coord1), k, zCoord2DPos(coord) ); \
  zCoord2DAngle(coord) = (k) * zCoord2DAngle(coord1); \
} while(0)
__ZEO_EXPORT zCoord2D *zCoord2DMul(const zCoord2D *coord1, double k, zCoord2D *coord);

/*! \brief divide a 2D coordinate by a scalar value.
 *
 * zCoord2DDiv() divides a 2D coordinate \a coord1 by a scalar value \a k, and puts the result into \a coord.
 * \return
 * zCoord2DDiv() returns a pointer \a coord.
 * if \a k is zero, zCoord2DDiv() returns the null pointer.
 */
__ZEO_EXPORT zCoord2D *zCoord2DDiv(const zCoord2D *coord1, double k, zCoord2D *coord);

/*! \brief concatenate a 2D coordinate to another.
 *
 * zCoord2DCat() concatenates a 2D coordinates \a coord2 multiplied by \a k to another \a coord1, and puts the result into \a coord.
 * \return
 * zCoord2DCat() returns a pointer \a coord.
 */
#define _zCoord2DCat(coord1,k,coord2,coord) do{ \
  _zVec2DCat( zCoord2DPos(coord1), k, zCoord2DPos(coord2), zCoord2DPos(coord) ); \
  zCoord2DAngle(coord) = zCoord2DAngle(coord1) + (k) * zCoord2DAngle(coord2); \
} while(0)
__ZEO_EXPORT zCoord2D *zCoord2DCat(const zCoord2D *coord1, double k, const zCoord2D *coord2, zCoord2D *coord);

/*! \brief add a 2D coordinate directly to another.
 *
 * zCoord2DAddDRC() directly adds a 2D coordinate \a coord2 to another \a coord1.
 * \return
 * zCoord2DAddDRC() returns a pointer \a coord1.
 */
#define _zCoord2DAddDRC(coord1,coord2) do{ \
  _zVec2DAddDRC( zCoord2DPos(coord1), zCoord2DPos(coord2) ); \
  zCoord2DAngle(coord1) += zCoord2DAngle(coord2); \
} while(0)
__ZEO_EXPORT zCoord2D *zCoord2DAddDRC(zCoord2D *coord1, const zCoord2D *coord2);

/*! \brief subtract a 2D coordinate directly from another.
 *
 * zCoord2DSubDRC() subtracts a 2D coordinate \a coord2 directly from another \a coord1.
 * \return
 * zCoord2DSubDRC() returns a pointer \a coord1.
 */
#define _zCoord2DSubDRC(coord1,coord2) do{ \
  _zVec2DSubDRC( zCoord2DPos(coord1), zCoord2DPos(coord2) ); \
  zCoord2DAngle(coord1) -= zCoord2DAngle(coord2); \
} while(0)
__ZEO_EXPORT zCoord2D *zCoord2DSubDRC(zCoord2D *coord1, const zCoord2D *coord2);

/*! \brief multiply a 2D coordinate directly by a scalar value.
 *
 * zCoord2DMulDRC() multiplies a 2D coordinate \a coord directly by a scalar value \a k.
 * \return
 * zCoord2DMulDRC() returns a pointer \a coord.
 */
#define _zCoord2DMulDRC(coord,k) do{ \
  _zVec2DMulDRC( zCoord2DPos(coord), k ); \
  zCoord2DAngle(coord) *= (k); \
} while(0)
__ZEO_EXPORT zCoord2D *zCoord2DMulDRC(zCoord2D *coord, double k);

/*! \brief divide a 2D coordinate directly by a scalar value.
 *
 * zCoord2DDivDRC() divides a 2D coordinate \a coord directly by a scalar value \a k.
 * \return
 * zCoord2DDivDRC() returns a pointer \a coord.
 */
__ZEO_EXPORT zCoord2D *zCoord2DDivDRC(zCoord2D *coord, double k);

/*! \brief concatenate a 2D coordinate directly to another.
 *
 * zCoord2DCatDRC() concatenates a 2D coordinates \a coord2 multiplied by \a k directly to another \a coord1.
 * \return
 * zCoord2DCatDRC() returns a pointer \a coord1.
 */
#define _zCoord2DCatDRC(coord1,k,coord2) do{ \
  _zVec2DCatDRC( zCoord2DPos(coord1), k, zCoord2DPos(coord2) ); \
  zCoord2DAngle(coord1) += (k) * zCoord2DAngle(coord2); \
} while(0)
__ZEO_EXPORT zCoord2D *zCoord2DCatDRC(zCoord2D *coord1, double k, const zCoord2D *coord2);

/*! \brief interior division of two 2D coordinates. */
__ZEO_EXPORT zCoord2D *zCoord2DInterDiv(const zCoord2D *coord1, const zCoord2D *coord2, double ratio, zCoord2D *coord);

/*! \brief find a transformation of a 2D coordinate to another. */
__ZEO_EXPORT zCoord2D *zCoord2DXform(const zCoord2D *src, const zCoord2D *target, zCoord2D *xform);

/*! \brief cascade a 2D coordinate to another. */
__ZEO_EXPORT zCoord2D *zCoord2DCascade(const zCoord2D *src, const zCoord2D *xform, zCoord2D *dest);
#define zCoord2DCascadeDRC(coord,dc) zCoord2DCascade( coord, dc, coord );

/*! \brief print a 2D coordinate out to a file. */
__ZEO_EXPORT void zCoord2DFPrint(FILE *fp, const zCoord2D *coord);
#define zCoord2DPrint(coord) zCoord2DFPrint( stdout, (coord) )

__END_DECLS

#endif /* __ZEO_COORD2D_H__ */
