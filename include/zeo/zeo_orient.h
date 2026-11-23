/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_orient - exact orientation predicates.
 */

#ifndef __ZEO_ORIENT_H__
#define __ZEO_ORIENT_H__

#include <zeo/zeo_misc.h>

__BEGIN_DECLS

/*! \brief exact orientation of points.
 *
 * zOrient2D() judges the orientation of three 2D points \a a, \a b and \a c given by arrays of
 * coordinates (x,y), namely, the sign of ( \a b - \a a ) x ( \a c - \a a ).
 *
 * zOrient3D() judges on which side of the plane passing three 3D points \a a, \a b and \a c a 3D
 * point \a d lies, where the points are given by arrays of coordinates (x,y,z), namely, the sign of
 * ( \a b - \a a ) x ( \a c - \a a ) . ( \a d - \a a ).
 * \return
 * zOrient2D() and zOrient3D() return 1 if the value is positive, -1 if negative, and 0 if zero.
 * In particular, zOrient2D() returns 1 if \a a, \a b and \a c are counterclockwise, -1 if clockwise,
 * and 0 if they are collinear. The sign is exact in spite of rounding errors.
 * \notes
 * IEEE 754 double precision arithmetics without extended precision and fused multiply-add is assumed.
 * The products of two coordinates for zOrient2D() and those of three coordinates for zOrient3D()
 * have to be neither overflowed nor underflowed.
 */
__ZEO_EXPORT int zOrient2D(const double a[], const double b[], const double c[]);
__ZEO_EXPORT int zOrient3D(const double a[], const double b[], const double c[], const double d[]);

__END_DECLS

#endif /* __ZEO_ORIENT_H__ */
