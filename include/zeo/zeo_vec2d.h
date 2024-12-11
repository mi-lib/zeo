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
ZDEF_UNION( __ZEO_CLASS_EXPORT, zVec2D ){
  struct{
    double x, y;
  } c;
  double e[2];
#ifdef __cplusplus
  zVec2D &create(double x, double y);
  zVec2D &createPolar(double r, double theta);
  zVec2D &copy(zVec2D &src);
  zVec2D &zero();
  bool operator==(zVec2D &v);
  bool isEqual(zVec2D &v);
  bool isTol(double tol);
  bool isTiny();
  bool isNan();
  zVec2D operator+(zVec2D &v);
  zVec2D operator-(zVec2D &v);
  zVec2D operator-();
  zVec2D operator*(double k);
  zVec2D operator*(zVec2D &v);
  zVec2D operator/(double k);
  zVec2D operator/(zVec2D &v);
  zVec2D &add(zVec2D &v);
  zVec2D &sub(zVec2D &v);
  zVec2D &rev();
  zVec2D &mul(double k);
  zVec2D &div(double k);
  zVec2D &amp(zVec2D &v);
  zVec2D &dem(zVec2D &v);
  zVec2D &cat(double k, zVec2D &v);
  double sqrNorm();
  double norm();
  zVec2D &normalize();
  static const zVec2D zvec2Dzero;
  static const zVec2D zvec2Dx;
  static const zVec2D zvec2Dy;
#endif /* __cplusplus */
};

/*! \brief 2D zero vector and unit vectors */
#ifdef __cplusplus
#define ZVEC2DZERO ( (zVec2D *)&zVec2D::zvec2Dzero )
#define ZVEC2DX    ( (zVec2D *)&zVec2D::zvec2Dx )
#define ZVEC2DY    ( (zVec2D *)&zVec2D::zvec2Dy )
#else
__ZEO_EXPORT const zVec2D zvec2Dzero;
__ZEO_EXPORT const zVec2D zvec2Dx;
__ZEO_EXPORT const zVec2D zvec2Dy;
#define ZVEC2DZERO ( (zVec2D *)&zvec2Dzero )
#define ZVEC2DX    ( (zVec2D *)&zvec2Dx )
#define ZVEC2DY    ( (zVec2D *)&zvec2Dy )
#endif /* __cplusplus */

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
__ZEO_EXPORT zVec2D *zVec2DCreate(zVec2D *v, double a1, double a2);
#define _zVec2DCopy(s,d) _zVec2DCreate( d, (s)->c.x, (s)->c.y )
__ZEO_EXPORT zVec2D *zVec2DCopy(const zVec2D *src, zVec2D *dest);
#define _zVec2DZero(v) _zVec2DCreate( v, 0, 0 )
#define zVec2DZero(v)  zVec2DCreate( v, 0, 0 )

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
__ZEO_EXPORT zVec2D *zVec2DCreatePolar(zVec2D *v, double r, double theta);

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
__ZEO_EXPORT bool zVec2DMatch(const zVec2D *v1, const zVec2D *v2);
#define _zVec2DEqual(v1,v2) ( zIsTiny( (v1)->c.x - (v2)->c.x ) && zIsTiny( (v1)->c.y - (v2)->c.y ) )
__ZEO_EXPORT bool zVec2DEqual(const zVec2D *v1, const zVec2D *v2);

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
__ZEO_EXPORT bool zVec2DIsTol(const zVec2D *v, double tol);
#define _zVec2DIsTiny(v) _zVec2DIsTol( v, zTOL )
#define zVec2DIsTiny(v)  zVec2DIsTol( v, zTOL )

/*! \brief check if a 2D vector includes NaN or Inf components. */
__ZEO_EXPORT bool zVec2DIsNan(const zVec2D *v);

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
#define _zVec2DAdd(v1,v2,v) do{\
  (v)->c.x = (v1)->c.x + (v2)->c.x;\
  (v)->c.y = (v1)->c.y + (v2)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DAdd(const zVec2D *v1, const zVec2D *v2, zVec2D *v);

#define _zVec2DSub(v1,v2,v) do{\
  (v)->c.x = (v1)->c.x - (v2)->c.x;\
  (v)->c.y = (v1)->c.y - (v2)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DSub(const zVec2D *v1, const zVec2D *v2, zVec2D *v);

#define _zVec2DRev(v,rv) do{\
  (rv)->c.x = -(v)->c.x;\
  (rv)->c.y = -(v)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DRev(const zVec2D *v, zVec2D *rv);

#define _zVec2DMul(v,k,mv) do{\
  (mv)->c.x = (k) * (v)->c.x;\
  (mv)->c.y = (k) * (v)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DMul(const zVec2D *v, double k, zVec2D *mv);

__ZEO_EXPORT zVec2D *zVec2DDiv(const zVec2D *v, double k, zVec2D *dv);

#define _zVec2DAmp(v,a,av) do{\
  (av)->c.x = (a)->c.x * (v)->c.x;\
  (av)->c.y = (a)->c.y * (v)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DAmp(const zVec2D *v1, const zVec2D *v2, zVec2D *v);

#define _zVec2DDem(v,d,dv) do{\
  (dv)->c.x = (v)->c.x / (d)->c.x;\
  (dv)->c.y = (v)->c.y / (d)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DDem(const zVec2D *v1, const zVec2D *v2, zVec2D *v);

#define _zVec2DCat(v1,k,v2,v) do{\
  (v)->c.x = (v1)->c.x + (k) * (v2)->c.x;\
  (v)->c.y = (v1)->c.y + (k) * (v2)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DCat(const zVec2D *v1, double k, const zVec2D *v2, zVec2D *v);

/*! \brief directly add a 2D vector to another. */
#define _zVec2DAddDRC(v1,v2) do{\
  (v1)->c.x += (v2)->c.x;\
  (v1)->c.y += (v2)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DAddDRC(zVec2D *v1, const zVec2D *v2);
/*! \brief directly subtract a 2D vector from another. */
#define _zVec2DSubDRC(v1,v2) do{\
  (v1)->c.x -= (v2)->c.x;\
  (v1)->c.y -= (v2)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DSubDRC(zVec2D *v1, const zVec2D *v2);
/*! \brief directly reverse a 2D vector. */
#define _zVec2DRevDRC(v) do{\
  (v)->c.x = -(v)->c.x;\
  (v)->c.y = -(v)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DRevDRC(zVec2D *v);
/*! \brief directly multiply a 2D vector by a scalar value. */
#define _zVec2DMulDRC(v,k) do{\
  (v)->c.x *= (k);\
  (v)->c.y *= (k);\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DMulDRC(zVec2D *v, double k);
/*! \brief directly divide a 2D vector by a scalar value. */
__ZEO_EXPORT zVec2D *zVec2DDivDRC(zVec2D *v, double k);
/*! \brief directly amplify a 2D vector by another. */
#define _zVec2DAmpDRC(v,a) do{\
  (v)->c.x *= (a)->c.x;\
  (v)->c.y *= (a)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DAmpDRC(zVec2D *v1, const zVec2D *v2);
/*! \brief directly demagnify a 2D vector by another. */
#define _zVec2DDemDRC(v,d) do{\
  (v)->c.x /= (d)->c.x;\
  (v)->c.y /= (d)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DDemDRC(zVec2D *v1, const zVec2D *v2);
/*! \brief directly concatenate a 2D vector multiplied by a scalar to another. */
#define _zVec2DCatDRC(v1,k,v2) do{\
  (v1)->c.x += (k) * (v2)->c.x;\
  (v1)->c.y += (k) * (v2)->c.y;\
} while(0)
__ZEO_EXPORT zVec2D *zVec2DCatDRC(zVec2D *v1, double k, const zVec2D *v2);

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
__ZEO_EXPORT double zVec2DInnerProd(const zVec2D *v1, const zVec2D *v2);
#define _zVec2DOuterProd(v1,v2) ( (v1)->c.x * (v2)->c.y - (v1)->c.y * (v2)->c.x )
__ZEO_EXPORT double zVec2DOuterProd(const zVec2D *v1, const zVec2D *v2);
#define zVec2DOuterProdNorm(v1,v2) fabs( zVec2DOuterProd( v1, v2 ) )

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
__ZEO_EXPORT double zVec2DSqrNorm(const zVec2D *v);
#define zVec2DNorm(v) sqrt( zVec2DSqrNorm( (v) ) )
__ZEO_EXPORT double zVec2DSqrDist(const zVec2D *v1, const zVec2D *v2);
#define zVec2DDist(v1,v2) sqrt( zVec2DSqrDist( (v1), (v2) ) )

/*! \brief normalize a 2D vector.
 *
 * zVec2DNormalize() normalizes a 2D vector \a v and puts it into \a nv.
 *
 * zVec2DNormalizeDRC() directly normalizes a 2D vector \a v.
 * \return
 * zVec2DNormalizeNC() and zVec2DNormalize() return the norm of \a v.
 * If the norm of \a v is zero, -1 is returned.
 */
__ZEO_EXPORT double zVec2DNormalizeNC(const zVec2D *v, zVec2D *nv);
__ZEO_EXPORT double zVec2DNormalize(const zVec2D *v, zVec2D *nv);
#define zVec2DNormalizeNCDRC(v) zVec2DNormalizeNC(v,v)
#define zVec2DNormalizeDRC(v)   zVec2DNormalize(v,v)

/* ********************************************************** */
/* geometry
 * ********************************************************** */

/*! \brief interior division of two 2D vectors.
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
__ZEO_EXPORT zVec2D *zVec2DInterDiv(const zVec2D *v1, const zVec2D *v2, double ratio, zVec2D *v);
__ZEO_EXPORT zVec2D *zVec2DMid(const zVec2D *v1, const zVec2D *v2, zVec2D *v);

/*! \brief angle between two 2D vectors.
 *
 * zVec2DAngle() calculates the angle between two 2D vectors \a v1 and \a v2.
 * \return
 * The angle between \a v1 and \a v2 is returned.
 */
__ZEO_EXPORT double zVec2DAngle(const zVec2D *v1, const zVec2D *v2);

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
__ZEO_EXPORT zVec2D *zVec2DProj(const zVec2D *v, const zVec2D *n, zVec2D *pv);
__ZEO_EXPORT zVec2D *zVec2DRot(const zVec2D *v, double angle, zVec2D *rv);

/*! \brief rotate a 2D vector at 90 degree clockwize/counterclockwize. */
#define _zVec2DRot90CW(s,d)  _zVec2DCreate( d, (s)->c.y, -(s)->c.x )
#define _zVec2DRot90CCW(s,d) _zVec2DCreate( d, -(s)->c.y, (s)->c.x )
__ZEO_EXPORT zVec2D *zVec2DRot90CW(const zVec2D *src, zVec2D *dest);
__ZEO_EXPORT zVec2D *zVec2DRot90CCW(const zVec2D *src, zVec2D *dest);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief read a 2D vector from a ZTK format processor. */
__ZEO_EXPORT zVec2D *zVec2DFromZTK(zVec2D *v, ZTK *ztk);

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
 * zVec2DValueFPrint() prints a 2D vector \a v out to the current
 * position of a file \a fp in the following style.
 *  x y
 * When the null pointer is given, nothing is printed.
 * zVec2DValuePrint() prints \a v out to the standard output in the
 * same style with zVec2DValueFPrint().
 * \return
 * zVec2DFScan() and zVec2DScan() return a pointer \a v.
 *
 * zVec2DFPrint(), zVec2DPrint(), zVec2DValueFPrint() and zVec2DValuePrint()
 * return no value.
 */
__ZEO_EXPORT zVec2D *zVec2DFScan(FILE *fp, zVec2D *v);
#define zVec2DScan(v) zVec2DFScan( stdin, (v) )
__ZEO_EXPORT const zVec2D *zVec2DValueFPrint(FILE *fp, const zVec2D *v);
#define zVec2DValuePrint(v) zVec2DValueFPrint( stdout, (v) )
__ZEO_EXPORT const zVec2D *zVec2DValueNLFPrint(FILE *fp, const zVec2D *v);
#define zVec2DValueNLPrint(v) zVec2DValueNLFPrint( stdout, (v) )
__ZEO_EXPORT const zVec2D *zVec2DFPrint(FILE *fp, const zVec2D *v);
#define zVec2DPrint(v) zVec2DFPrint( stdout, (v) )

__END_DECLS

#ifdef __cplusplus
inline zVec2D &zVec2D::create(double x, double y){ _zVec2DCreate( this, x, y ); return *this; }
inline zVec2D &zVec2D::createPolar(double r, double theta){ zVec2DCreatePolar( this, r, theta ); return *this; }
inline zVec2D &zVec2D::copy(zVec2D &src){ zVec2DCopy( &src, this ); return *this; }
inline zVec2D &zVec2D::zero(){ _zVec2DZero( this ); return *this; }
inline bool zVec2D::operator==(zVec2D &v){ return _zVec2DMatch( this, &v ); }
inline bool zVec2D::isEqual(zVec2D &v){ return _zVec2DEqual( this, &v ); }
inline bool zVec2D::isTol(double tol){ return _zVec2DIsTol( this, tol ); }
inline bool zVec2D::isTiny(){ return _zVec2DIsTiny( this ); }
inline bool zVec2D::isNan(){ return zVec2DIsNan( this ); }
inline zVec2D zVec2D::operator+(zVec2D &v){ zVec2D ret; _zVec2DAdd( this, &v, &ret ); return ret; }
inline zVec2D zVec2D::operator-(zVec2D &v){ zVec2D ret; _zVec2DSub( this, &v, &ret ); return ret; }
inline zVec2D zVec2D::operator-(){ zVec2D ret; _zVec2DRev( this, &ret ); return ret; }
inline zVec2D zVec2D::operator*(double k){ zVec2D ret; _zVec2DMul( this, k, &ret ); return ret; }
inline zVec2D zVec2D::operator*(zVec2D &v){ zVec2D ret; _zVec2DAmp( this, &v, &ret ); return ret; }
inline zVec2D zVec2D::operator/(double k){ zVec2D ret; zVec2DDiv( this, k, &ret ); return ret; }
inline zVec2D zVec2D::operator/(zVec2D &v){ zVec2D ret; _zVec2DDem( this, &v, &ret ); return ret; }
inline zVec2D &zVec2D::add(zVec2D &v){ _zVec2DAddDRC( this, &v ); return *this; }
inline zVec2D &zVec2D::sub(zVec2D &v){ _zVec2DSubDRC( this, &v ); return *this; }
inline zVec2D &zVec2D::rev(){ _zVec2DRevDRC( this ); return *this; }
inline zVec2D &zVec2D::mul(double k){ _zVec2DMulDRC( this, k ); return *this; }
inline zVec2D &zVec2D::div(double k){ zVec2DDivDRC( this, k ); return *this; }
inline zVec2D &zVec2D::amp(zVec2D &v){ _zVec2DAmpDRC( this, &v ); return *this; }
inline zVec2D &zVec2D::dem(zVec2D &v){ _zVec2DDemDRC( this, &v ); return *this; }
inline zVec2D &zVec2D::cat(double k, zVec2D &v){ _zVec2DCatDRC( this, k, &v ); return *this; }
inline double zVec2D::sqrNorm(){ return _zVec2DSqrNorm( this ); }
inline double zVec2D::norm(){ return sqrt( sqrNorm() ); }
inline zVec2D &zVec2D::normalize(){ zVec2DNormalizeNCDRC( this ); return *this; }
#endif /* __cplusplus */

/*! \struct zVec2DArray
 * \brief array class of 2D vectors.
 */
zArrayClass( zVec2DArray, zVec2D );

/*! \brief allocate an array of 2D vectors. */
#define zVec2DArrayAlloc(array,size) zArrayAlloc( array, zVec2D, size )
/*! \brief free an array of 2D vectors. */
#define zVec2DArrayFree(array)       zArrayFree( array )

#include <zeo/zeo_vec2d_list.h>  /* 2D vector list */

#include <zeo/zeo_vec2d_data.h>  /* abstract class of a set of 2D vectors. */

#endif /* __ZEO_VEC2D_H__ */
