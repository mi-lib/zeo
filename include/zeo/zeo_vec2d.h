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
typedef union{
  struct{
    double x, y;
  } c;
  double e[2];
} zVec2D;

/*! \brief 2D zero vector and unit vectors */
extern const zVec2D zvec2Dzero;
extern const zVec2D zvec2Dx;
extern const zVec2D zvec2Dy;
#define ZVEC2DZERO ( (zVec2D *)&zvec2Dzero )
#define ZVEC2DX    ( (zVec2D *)&zvec2Dx )
#define ZVEC2DY    ( (zVec2D *)&zvec2Dy )

/*! \brief create, copy and zero a 2D vector.
 *
 * zVec2DCreate() creates a 2D vector \a v which consists of
 * \a x and \a y.
 *
 * zVec2DCopy() copies a 2D vector \a src to another \a dest.
 *
 * zVec2DZero() sets all components of a 2D vector \a v for zeros.
 * \return
 * zVec2DCreate() and zVec2DZero() return a pointer \a v.
 *
 * zVec2DCopy() returns a pointer \a dest.
 */
#define _zVec2DCreate(v,a1,a2) do{\
  (v)->c.x = (a1);\
  (v)->c.y = (a2);\
} while(0)
__EXPORT zVec2D *zVec2DCreate(zVec2D *v, double a1, double a2);
#define _zVec2DCopy(s,d) _zVec2DCreate( d, (s)->c.x, (s)->c.y )
__EXPORT zVec2D *zVec2DCopy(zVec2D *src, zVec2D *dest);
#define zVec2DZero(v) _zVec2DCreate( v, 0, 0 )

/*! \brief create a 2D vector by the set of value for a polar expression.
 *
 * zVec2DCreatePolar() creates a 2D vector \a v from a set of values
 * for a polar coordinate ( \a r, \a theta ), where \a r is for the
 * radius from the original point, and \a theta is for the longitudinal
 * angle.
 * \return
 * zVec2DCreatePolar() returns a pointer \a v.
 */
#define _zVec2DCreatePolar(v,r,t) _zVec2DCreate( (v), (r)*cos((t)), (r)*sin((t)) )
__EXPORT zVec2D *zVec2DCreatePolar(zVec2D *v, double r, double theta);

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
#define _zVec2DMatch(v1,v2) ( (v1)->c.x == (v2)->c.x && (v1)->c.y == (v2)->c.y )
__EXPORT bool zVec2DMatch(zVec2D *v1, zVec2D *v2);
#define _zVec2DEqual(v1,v2) ( zIsTiny( (v1)->c.x - (v2)->c.x ) && zIsTiny( (v1)->c.y - (v2)->c.y ) )
__EXPORT bool zVec2DEqual(zVec2D *v1, zVec2D *v2);

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
#define _zVec2DIsTol(v,tol) ( zIsTol( (v)->c.x, tol ) && zIsTol( (v)->c.y, tol ) )
__EXPORT bool zVec2DIsTol(zVec2D *v, double tol);
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
#define _zVec2DAdd(v1,v2,v)   _zVec2DCreate( v, (v1)->c.x + (v2)->c.x, (v1)->c.y + (v2)->c.y )
#define _zVec2DSub(v1,v2,v)   _zVec2DCreate( v, (v1)->c.x - (v2)->c.x, (v1)->c.y - (v2)->c.y )
#define _zVec2DRev(v,rv)      _zVec2DCreate( rv, -(v)->c.x, -(v)->c.y )
#define _zVec2DMul(v,k,mv)    _zVec2DCreate( mv, (k) * (v)->c.x, (k) * (v)->c.y )
#define _zVec2DCat(v1,k,v2,v) _zVec2DCreate( v, (v1)->c.x + (k) * (v2)->c.x, (v1)->c.y + (k) * (v2)->c.y )

__EXPORT zVec2D *zVec2DAdd(zVec2D *v1, zVec2D *v2, zVec2D *v);
__EXPORT zVec2D *zVec2DSub(zVec2D *v1, zVec2D *v2, zVec2D *v);
__EXPORT zVec2D *zVec2DRev(zVec2D *v, zVec2D *rv);
__EXPORT zVec2D *zVec2DMul(zVec2D *v, double k, zVec2D *mv);
__EXPORT zVec2D *zVec2DDiv(zVec2D *v, double k, zVec2D *dv);
__EXPORT zVec2D *zVec2DCat(zVec2D *v1, double k, zVec2D *v2, zVec2D *v);

#define zVec2DAddDRC(v1,v2)   zVec2DAdd(v1,v2,v1)
#define zVec2DSubDRC(v1,v2)   zVec2DSub(v1,v2,v1)
#define zVec2DRevDRC(v)       zVec2DRev(v,v)
#define zVec2DMulDRC(v,k)     zVec2DMul(v,k,v)
#define zVec2DDivDRC(v,k)     zVec2DDiv(v,k,v)
#define zVec2DCatDRC(v1,k,v2) zVec2DCat(v1,k,v2,v1)

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
#define _zVec2DInnerProd(v1,v2) ( (v1)->c.x * (v2)->c.x + (v1)->c.y * (v2)->c.y )
__EXPORT double zVec2DInnerProd(zVec2D *v1, zVec2D *v2);
#define _zVec2DOuterProd(v1,v2) ( (v1)->c.x * (v2)->c.y - (v1)->c.y * (v2)->c.x )
__EXPORT double zVec2DOuterProd(zVec2D *v1, zVec2D *v2);

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
#define _zVec2DSqrNorm(v) _zVec2DInnerProd( v, v )
__EXPORT double zVec2DSqrNorm(zVec2D *v);
#define zVec2DNorm(v) sqrt( zVec2DSqrNorm( (v) ) )
__EXPORT double zVec2DSqrDist(zVec2D *v1, zVec2D *v2);
#define zVec2DDist(v1,v2) sqrt( zVec2DSqrDist( (v1), (v2) ) )

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
__EXPORT zVec2D *zVec2DNormalize(zVec2D *v, zVec2D *nv);
#define zVec2DNormalizeDRC(v) zVec2DNormalize( (v), (v) )

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
__EXPORT zVec2D *zVec2DInterDiv(zVec2D *v1, zVec2D *v2, double ratio, zVec2D *v);
__EXPORT zVec2D *zVec2DMid(zVec2D *v1, zVec2D *v2, zVec2D *v);

/*! \brief angle between two 2D vectors.
 *
 * zVec2DAngle() calculates the angle between two 2D vectors \a v1 and \a v2.
 * \return
 * The angle between \a v1 and \a v2 is returned.
 */
__EXPORT double zVec2DAngle(zVec2D *v1, zVec2D *v2);

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
__EXPORT zVec2D *zVec2DProj(zVec2D *v, zVec2D *n, zVec2D *pv);
__EXPORT zVec2D *zVec2DRot(zVec2D *v, double angle, zVec2D *rv);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief read a 2D vector from a ZTK format processor. */
__EXPORT zVec2D *zVec2DFromZTK(zVec2D *v, ZTK *ztk);

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
__EXPORT zVec2D *zVec2DFScan(FILE *fp, zVec2D *v);
#define zVec2DScan(v) zVec2DFScan( stdin, (v) )
__EXPORT void zVec2DDataFPrint(FILE *fp, zVec2D *v);
#define zVec2DDataPrint(v) zVec2DDataFPrint( stdout, (v) )
__EXPORT void zVec2DDataNLFPrint(FILE *fp, zVec2D *v);
#define zVec2DDataNLPrint(v) zVec2DDataNLFPrint( stdout, (v) )
__EXPORT void zVec2DFPrint(FILE *fp, zVec2D *v);
#define zVec2DPrint(v) zVec2DFPrint( stdout, (v) )

/*! \struct zVec2DArray
 * \brief array class of 2D vectors.
 */
zArrayClass( zVec2DArray, zVec2D );

__END_DECLS

#include <zeo/zeo_vec2d_list.h>  /* 2D vector list */

#endif /* __ZEO_VEC2D_H__ */
