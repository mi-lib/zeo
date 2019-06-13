/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec2d - 2D vector.
 */

#ifndef __ZEO_VEC2D_H__
#define __ZEO_VEC2D_H__

#include <zeo/zeo_misc.h>

__BEGIN_DECLS

/*! \struct zVec2D
 * \brief 2D vector.
 */
typedef double zVec2D[2];

/*! \brief create, copy and cleanup a 2D vector.
 *
 * zVec2DCreate() creates a 2D vector \a v which consists of
 * \a x and \a y.
 *
 * zVec2DCopy() copies a 2D vector \a src to another \a dest.
 *
 * zVec2DClear() sets all the components of a 2D vector \a v for 0.
 * \return
 * zVec2DCreate() and zVec2DClear() return a pointer \a v.
 *
 * zVec2DCopy() returns a pointer \a dest.
 */
__EXPORT double *zVec2DCreate(zVec2D v, double a1, double a2);
__EXPORT double *zVec2DCopy(zVec2D src, zVec2D dest);
#define zVec2DClear(v) zVec2DCreate( v, 0, 0 )

/*! \brief create a 2D vector by the set of value for a polar expression.
 *
 * zVec2DCreatePolar() creates a 2D vector \a v from a set of values
 * for a polar coordinate ( \a r, \a theta ), where \a r is for the
 * radius from the original point, and \a theta is for the longitudinal
 * angle.
 * \return
 * zVec2DCreatePolar() returns a pointer \a v.
 */
__EXPORT double *zVec2DCreatePolar(zVec2D v, double r, double theta);

/*! \brief check if two 2D vectors are equal.
 *
 * zVec2DMatch() checks if two 2D vectors \a v1 and \a v2 are exactly
 * the same.
 *
 * zVec2DEqual() checks if two 2D vectors \a v1 and \a v2 are equal.
 * \return
 * zVec2DMatch() returns the true value if \a v1 and \a v2 are exactly
 * the same. Otherwise, the false value is returned.
 *
 * zVec2DEqual() returns the true value if \a v1 and \a v2 are equal.
 * Otherwise, the false value is returned.
 */
__EXPORT bool zVec2DMatch(zVec2D v1, zVec2D v2);
__EXPORT bool zVec2DEqual(zVec2D v1, zVec2D v2);

/*! \brief check if a 2D vector is negligibly small.
 *
 * zVec2DIsTol() checks if the absolute values of every components of
 * a 2D vector \a v are smaller than \a tol.
 *
 * zVec2DIsTiny() applies zTOL to the tolerance of zVec2DIsTol().
 * \return
 * zVec2DIsTol() and zVec2DIsTiny() return the true value when the
 * absolute values of every components of \a v are smaller than \a tol
 * and zTOL, respectively.
 * Otherwise, the false value is returned.
 * \notes
 * \a tol must be positive.
 * \sa
 * zIsTol, zIsTiny
 */
__EXPORT bool zVec2DIsTol(zVec2D v, double tol);
#define zVec2DIsTiny(v) zVec2DIsTol( v, zTOL )

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/*! \brief the four rules of the arithmetics for 2D vector.
 *
 * zVec2DAdd() adds two 2D vectors \a v1 and \a v2 and puts it into \a v.
 *
 * zVec2DSub() subtracts a 2D vector \a v2 from the other \a v1 and
 * puts it into \a v.
 *
 * zVec2DRev() reverses a 2D vector \a v and puts it into \a rv.
 *
 * zVec2DMul() multiplies a 2D vector \a v by a scalar value \a k and
 * puts it into \a mv.
 *
 * zVec2DDiv() divides a 2D vector \a v by a scalar value \a k and
 * puts it into \a dv.
 *
 * zVec2DCat() concatenates a 2D vector \a v2 to \a v1 multiplied by
 * a scalar value \a k and puts it into \a v.
 *
 * zVec2DAddDRC() directly adds \a v2 to \a v1.
 *
 * zVec2DSubDRC() directly subtracts \a v2 from \a v1.
 *
 * zVec2DRevDRC() directly reverses \a v.
 *
 * zVec2DMulDRC() directly multiplies \a v by \a k.
 *
 * zVec2DDivDRC() directly divides \a v by \a k.
 *
 * zVec2DCat() directly concatenates \a v2 multiplied \a v2 by \a k to \a v1.
 * \return
 * Each function returns a pointer to the result vector.
 *
 * zVec2DDiv() and zVec2DDivDRC() return the null pointer if \a k is zero.
 */
__EXPORT double *zVec2DAdd(zVec2D v1, zVec2D v2, zVec2D v);
__EXPORT double *zVec2DSub(zVec2D v1, zVec2D v2, zVec2D v);
__EXPORT double *zVec2DRev(zVec2D v, zVec2D rv);
__EXPORT double *zVec2DMul(zVec2D v, double k, zVec2D mv);
__EXPORT double *zVec2DDiv(zVec2D v, double k, zVec2D dv);
__EXPORT double *zVec2DCat(zVec2D v1, double k, zVec2D v2, zVec2D v);

#define zVec2DAddDRC(v1,v2)   zVec2DAdd(v1,v2,v1)
#define zVec2DSubDRC(v1,v2)   zVec2DSub(v1,v2,v1)
#define zVec2DRevDRC(v)       zVec2DRev(v,v)
#define zVec2DMulDRC(v,k)     zVec2DMul(v,k,v)
#define zVec2DDivDRC(v,k)     zVec2DDiv(v,k,v)
#define zVec2DCatDRC(v1,k,v2) zVec2DCat(v1,k,v2,v1)

/*! \brief norm of a 2D vector.
 *
 * zVec2DNorm() calculates the norm of a 2D vector \a v.
 *
 * zVec2DSqrNorm() calculates the squared norm of a 2D vector \a v.
 *
 * zVec2DDist() calculates the distance between two points positioned
 * by 2D vectors \a v1 and \a v2.
 *
 * zVec2DSqrDist() calculates the squared distance between two points
 * positioned by 2D vectors \a v1 and \a v2.
 * \return
 * zVec2DNorm() returns the norm of \a v.
 *
 * zVec2DSqrNorm() returns the squared norm of \a v.
 *
 * zVec2DDist() returns the distance between \a v1 and \a v2.
 *
 * zVec2DSqrDist() returns the squared distance between \a v1 and \a v2.
 */
__EXPORT double zVec2DSqrNorm(zVec2D v);
#define zVec2DNorm(v) sqrt(zVec2DSqrNorm((v)))

__EXPORT double zVec2DSqrDist(zVec2D v1, zVec2D v2);
#define zVec2DDist(v1,v2) sqrt(zVec2DSqrDist((v1),(v2)))

/*! \brief normalize a 2D vector.
 *
 * zVec2DNormalize() normalizes a 2D vector \a v and puts it into \a nv.
 *
 * zVec2DNormalizeDRC() directly normalizes a 2D vector \a v.
 * \return
 * zVec2DNormalize() returns a pointer \a nv.
 * zVec2DNormalizeDRC() returns a pointer \a v.
 * If the norm of \a v is zero, the null pointer is returned.
 */
__EXPORT double *zVec2DNormalize(zVec2D v, zVec2D nv);
#define zVec2DNormalizeDRC(v) zVec2DNormalize(v,v)

/*! \brief inner andouter products.
 *
 * zVec2DInnerProd() calculates the inner product of two 2D vectors
 * \a v1 and \a v2.
 *
 * zVec2DOuterProd() calculates the outer product of two 2D vectors
 * \a v1 and \a v2. Since \a v1 and \a v2 are 2-dimensional vectors,
 * the result is a scalar value.
 * \return
 * zVec2DInnerProd() and zVec2DOuterProd() return the resulted scalar
 * values.
 */
__EXPORT double zVec2DInnerProd(zVec2D v1, zVec2D v2);
__EXPORT double zVec2DOuterProd(zVec2D v1, zVec2D v2);

/* ********************************************************** */
/* geometry
 * ********************************************************** */

/*! \brief interior division.
 *
 * zVec2DInterDiv() calculates the interior division vector of
 * \a v1 and \a v2 with a division ratio \a ratio. The result is
 * put into \a v, namely:
 *   \a v = ( 1 - \a ratio ) * \a v1 + \a ratio * \a v2.
 *
 * zVec2DMid() calculates the middle point vector of \a v1 and \a v2.
 * The result is put into \a v, namely:
 *   \a v = ( \a v1 + \a v2 ) / 2 .
 * \return
 * zVec2DInterDiv() and zVec2DMid() return a pointer \a v.
 */
__EXPORT double *zVec2DInterDiv(zVec2D v1, zVec2D v2, double ratio, zVec2D v);
__EXPORT double *zVec2DMid(zVec2D v1, zVec2D v2, zVec2D v);

/*! \brief angle between two 2D vectors.
 *
 * zVec2DAngle() calculates the angle between two 2D vectors \a v1 and \a v2.
 * \return
 * The angle between \a v1 and \a v2 is returned.
 */
__EXPORT double zVec2DAngle(zVec2D v1, zVec2D v2);

/*! \brief projection and rotation of a 2D vector.
 *
 * zVec2DProj() projects a 2D vector \a v onto the line directed
 * by \a n and puts the result into \a pv. \a pv is parallel to \a n
 * and the subtraction vector from \a pv to \a v is orthogonal to \a n.
 *
 * zVec2DRot() rotates \a v with an angle \a angle. The result is put
 * into \a rv.
 * \return
 * zVec2DProj() returns a pointer \a pv.
 * If \a n is the zero vector, the null pointer is returned.
 *
 * zVec2DRot() returns a pointer \a rv.
 */
__EXPORT double *zVec2DProj(zVec2D v, zVec2D n, zVec2D pv);
__EXPORT double *zVec2DRot(zVec2D v, double angle, zVec2D rv);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief input and output of a 2D vector.
 *
 * zVec2DFScan() scans two values from the current position of
 * a file \a fp and creates a 2D vector \a v from them.
 * zVec2DScan() scans two values from the standard input.
 *
 * zVec2DFPrint() prints a 2D vector \a v out to the current position
 * of a file \a fp in the following style.
 *  ( x, y )
 * When the null pointer is given, the following string is printed.
 *  (null 2D vector)
 * zVec2DPrint() prints \a v out to the standard output.
 *
 * zVec2DDataFPrint() prints a 2D vector \a v out to the current
 * position of a file \a fp in the following style.
 *  x y
 * When the null pointer is given, nothing is printed.
 * zVec2DDataPrint() prints \a v out to the standard output in the
 * same style with zVec2DDataFPrint().
 * \return
 * zVec2DFScan() and zVec2DScan() return a pointer \a v.
 *
 * zVec2DFPrint(), zVec2DPrint(), zVec2DDataFPrint() and zVec2DDataPrint()
 * return no value.
 */
__EXPORT double *zVec2DFScan(FILE *fp, zVec2D v);
#define zVec2DScan(v) zVec2DFScan( stdin, (v) )
__EXPORT void zVec2DDataFPrint(FILE *fp, zVec2D v);
#define zVec2DDataPrint(v) zVec2DDataFPrint( stdout, (v) )
__EXPORT void zVec2DDataNLFPrint(FILE *fp, zVec2D v);
#define zVec2DDataNLPrint(v) zVec2DDataNLFPrint( stdout, (v) )
__EXPORT void zVec2DFPrint(FILE *fp, zVec2D v);
#define zVec2DPrint(v) zVec2DFPrint( stdout, (v) )

__END_DECLS

#endif /* __ZEO_VEC2D_H__ */
