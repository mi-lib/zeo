/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d - 3D vector.
 */

#ifndef __ZEO_VEC3D_H__
#define __ZEO_VEC3D_H__

#include <zeo/zeo_misc.h>

__BEGIN_DECLS

/*! \struct zVec3D
 * \brief 3D vector.
 */
ZDEF_UNION( __ZEO_CLASS_EXPORT, zVec3D ){
  struct{
    double x, y, z;
  } c;
  double e[3];
#ifdef __cplusplus
  zVec3D &create(double x, double y, double z);
  zVec3D &createPolar(double r, double theta, double phi);
  zVec3D &copy(zVec3D &src);
  zVec3D &zero();
  bool operator==(zVec3D &v);
  bool isEqual(zVec3D &v);
  bool isTol(double tol);
  bool isTiny();
  bool isNan();
  zVec3D operator+(zVec3D &v);
  zVec3D operator-(zVec3D &v);
  zVec3D operator-();
  zVec3D operator*(double k);
  zVec3D operator*(zVec3D &v);
  zVec3D operator/(double k);
  zVec3D operator/(zVec3D &v);
  zVec3D &add(zVec3D &v);
  zVec3D &sub(zVec3D &v);
  zVec3D &rev();
  zVec3D &mul(double k);
  zVec3D &div(double k);
  zVec3D &amp(zVec3D &v);
  zVec3D &dem(zVec3D &v);
  zVec3D &cat(double k, zVec3D &v);
  double sqrNorm();
  double norm();
  zVec3D &normalize();
  static const zVec3D zvec3Dzero;
  static const zVec3D zvec3Dx;
  static const zVec3D zvec3Dy;
  static const zVec3D zvec3Dz;
#endif /* __cplusplus */
};

/*! \brief 3D zero vector and unit vectors */
#ifdef __cplusplus
#define ZVEC3DZERO ( (zVec3D *)&zVec3D::zvec3Dzero )
#define ZVEC3DX    ( (zVec3D *)&zVec3D::zvec3Dx )
#define ZVEC3DY    ( (zVec3D *)&zVec3D::zvec3Dy )
#define ZVEC3DZ    ( (zVec3D *)&zVec3D::zvec3Dz )
#else
__ZEO_EXPORT const zVec3D zvec3Dzero;
__ZEO_EXPORT const zVec3D zvec3Dx;
__ZEO_EXPORT const zVec3D zvec3Dy;
__ZEO_EXPORT const zVec3D zvec3Dz;
#define ZVEC3DZERO ( (zVec3D *)&zvec3Dzero )
#define ZVEC3DX    ( (zVec3D *)&zvec3Dx )
#define ZVEC3DY    ( (zVec3D *)&zvec3Dy )
#define ZVEC3DZ    ( (zVec3D *)&zvec3Dz )
#endif /* __cplusplus */

/*! \brief create a 3D vector.
 *
 * zVec3DCreate() creates a 3D vector \a v which consists of \a x, \a y and \a z
 * for its components.
 * \return
 * zVec3DCreate() returns a pointer \a v.
 */
#define _zVec3DCreate(v,_x,_y,_z) do{\
  (v)->c.x = (_x);\
  (v)->c.y = (_y);\
  (v)->c.z = (_z);\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DCreate(zVec3D *v, double x, double y, double z);

/*! \brief create a 3D vector by the set of value for a polar expression.
 *
 * zVec3DCreatePolar() creates a vector \a v from a set of values
 * for a polar coordinate ( \a r, \a theta, \a phi ), where
 * \a r is for the radius from the original point, \a theta is for
 * the longitudinal angle, and \a phi is for the latitudinal angle.
 * \retval \a v
 */
__ZEO_EXPORT zVec3D *zVec3DCreatePolar(zVec3D *v, double r, double theta, double phi);

/*! \brief copy a 3D vector to another.
 *
 * zVec3DCopy() copies a 3D vector \a src to another \a dest.
 * \return
 * zVec3DCopy() returns a pointer \a dest.
 */
#define zVec3DCopy(s,d) zCopy( zVec3D, s, d )

/*! \brief zero a 3D vector.
 *
 * zVec3DZero() sets all components of \a v for zeros.
 * \return
 * zVec3DZero() returns a pointer \a v.
 */
#define _zVec3DZero(v) _zVec3DCreate( v, 0, 0, 0 )
#define zVec3DZero(v)  zVec3DCreate( v, 0, 0, 0 )

/*! \brief check if the two 3D vectors are equal.
 *
 * zVec3DMatch() and zVec3DEqual() check if the two 3D vectors
 * \a v1 and \a v2 are equal. They return a boolean value.
 *
 * zVec3DMatch() strictly compares the two vectors, while
 * zVec3DEqual() checks if the error between \a v1 and \a v2
 * are sufficiently small.
 * \return
 * zVec3DMatch() and zVec3DEqual() return the true value if
 * \a v1 and \a v2 are equal, or false otherwise.
 */
#define _zVec3DMatch(v1,v2) ( (v1)->c.x == (v2)->c.x && (v1)->c.y == (v2)->c.y && (v1)->c.z == (v2)->c.z )
__ZEO_EXPORT bool zVec3DMatch(zVec3D *v1, zVec3D *v2);
#define _zVec3DEqual(v1,v2) ( zIsTiny( (v1)->c.x - (v2)->c.x ) && zIsTiny( (v1)->c.y - (v2)->c.y ) && zIsTiny( (v1)->c.z - (v2)->c.z ) )
__ZEO_EXPORT bool zVec3DEqual(zVec3D *v1, zVec3D *v2);

/*! \brief check if a 3D vector is tiny.
 *
 * zVec3DIsTol() checks if the absolute values of all
 * components of 3D vector \a v are smaller than \a tol.
 *
 * zVec3DIsTiny() applies zTOL (defined in zm_misc.h) to
 * the tolerance of zVec3DIsTol().
 * \return
 * zVec3DIsTol() and zVec3DIsTiny() return the true value if
 * the absolute values of all components of \a v are smaller
 * than \a tol and zTOL, respectively, or the false value,
 * otherwise.
 * \notes
 * \a tol must be positive.
 * \sa
 * zIsTol, zIsTiny
 */
#define _zVec3DIsTol(v,tol) ( zIsTol( (v)->c.x, (tol) ) && zIsTol( (v)->c.y, (tol) ) && zIsTol( (v)->c.z, (tol) ) )
__ZEO_EXPORT bool zVec3DIsTol(zVec3D *v, double tol);
#define _zVec3DIsTiny(v) _zVec3DIsTol( v, zTOL )
#define zVec3DIsTiny(v)  zVec3DIsTol( v, zTOL )

/*! \brief check if a 3D vector includes NaN or Inf components. */
__ZEO_EXPORT bool zVec3DIsNan(zVec3D *v);

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/*! \brief the four rules of the arithmetics for 3D vector.
 *
 * zVec3DAdd() adds two 3D vectors, \a v1 and \a v2.
 * The result is put into \a v.
 *
 * zVec3DSub() subtracts a 3D vector \a v2 from the other \a v1.
 * The result is put into \a v.
 *
 * zVec3DRev() reverses a 3D vector \a v. The result is put into
 * \a rv.
 *
 * zVec3DMul() multiplies a 3D vector \a v by a scalar value \a k.
 * The result is put into \a mv.
 *
 * zVec3DDiv() divides a 3D vector \a v by a scalar value \a k.
 * The result is put into \a dv.
 *
 * zVec3DAmp() amplifies each component of a 3D vector \a v1 by
 * the corresponding component of another \a v2.
 * The result is put into \a v.
 *
 * zVec3DDem() demagnifies each component of a 3D vector \a v1 by
 * the corresponding component of another \a v2.
 * The result is put into \a v.
 *
 * zVec3DCat() concatenates a 3D vector \a v2 with \a v1 multiplied
 * by a scalar value \a k. The result is put into \a v.
 *
 * zVec3DAddDRC() directly adds \a v2 to \a v1.
 *
 * zVec3DSubDRC() directly subtracts \a v2 from \a v1.
 *
 * zVec3DRevDRC() directly reverses \a v.
 *
 * zVec3DMulDRC() directly multiplies \a v by \a k.
 *
 * zVec3DDivDRC() directly divides \a v by \a k.
 *
 * zVec3DAmpDRC() directly amplifies \a v1 by \a v2.
 *
 * zVec3DDemDRC() directly demagnifies \a v1 by \a v2.
 *
 * zVec3DCat() directly concatenates \a v2 with \a v1 multiplied
 * by \a k.
 * \return
 * Each function returns a pointer to the resultant vector.
 *
 * zVec3DDiv() and zVec3DDivDRC() return the null pointer if
 * the given scalar value is zero.
 */
#define _zVec3DAdd(v1,v2,v) do{\
  (v)->c.x = (v1)->c.x + (v2)->c.x;\
  (v)->c.y = (v1)->c.y + (v2)->c.y;\
  (v)->c.z = (v1)->c.z + (v2)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DAdd(zVec3D *v1, zVec3D *v2, zVec3D *v);

#define _zVec3DSub(v1,v2,v) do{\
  (v)->c.x = (v1)->c.x - (v2)->c.x;\
  (v)->c.y = (v1)->c.y - (v2)->c.y;\
  (v)->c.z = (v1)->c.z - (v2)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DSub(zVec3D *v1, zVec3D *v2, zVec3D *v);

#define _zVec3DRev(v,rv) do{\
  (rv)->c.x = -(v)->c.x;\
  (rv)->c.y = -(v)->c.y;\
  (rv)->c.z = -(v)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DRev(zVec3D *v, zVec3D *rv);

#define _zVec3DMul(v,k,mv) do{\
  (mv)->c.x = (k) * (v)->c.x;\
  (mv)->c.y = (k) * (v)->c.y;\
  (mv)->c.z = (k) * (v)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DMul(zVec3D *v, double k, zVec3D *mv);

__ZEO_EXPORT zVec3D *zVec3DDiv(zVec3D *v, double k, zVec3D *dv);

#define _zVec3DAmp(v,a,av) do{\
  (av)->c.x = (a)->c.x * (v)->c.x;\
  (av)->c.y = (a)->c.y * (v)->c.y;\
  (av)->c.z = (a)->c.z * (v)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DAmp(zVec3D *v1, zVec3D *v2, zVec3D *v);

#define _zVec3DDem(v,d,dv) do{\
  (dv)->c.x = (v)->c.x / (d)->c.x;\
  (dv)->c.y = (v)->c.y / (d)->c.y;\
  (dv)->c.z = (v)->c.z / (d)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DDem(zVec3D *v1, zVec3D *v2, zVec3D *v);

#define _zVec3DCat(v1,k,v2,v) do{\
  (v)->c.x = (v1)->c.x + (k) * (v2)->c.x;\
  (v)->c.y = (v1)->c.y + (k) * (v2)->c.y;\
  (v)->c.z = (v1)->c.z + (k) * (v2)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DCat(zVec3D *v1, double k, zVec3D *v2, zVec3D *v);

/*! \brief directly add a 3D vector to another. */
#define _zVec3DAddDRC(v1,v2) do{\
  (v1)->c.x += (v2)->c.x;\
  (v1)->c.y += (v2)->c.y;\
  (v1)->c.z += (v2)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DAddDRC(zVec3D *v1, zVec3D *v2);
/*! \brief directly subtract a 3D vector from another. */
#define _zVec3DSubDRC(v1,v2) do{\
  (v1)->c.x -= (v2)->c.x;\
  (v1)->c.y -= (v2)->c.y;\
  (v1)->c.z -= (v2)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DSubDRC(zVec3D *v1, zVec3D *v2);
/*! \brief directly reverse a 3D vector. */
#define _zVec3DRevDRC(v) do{\
  (v)->c.x = -(v)->c.x;\
  (v)->c.y = -(v)->c.y;\
  (v)->c.z = -(v)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DRevDRC(zVec3D *v);
/*! \brief directly multiply a 3D vector by a scalar value. */
#define _zVec3DMulDRC(v,k) do{\
  (v)->c.x *= (k);\
  (v)->c.y *= (k);\
  (v)->c.z *= (k);\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DMulDRC(zVec3D *v, double k);
/*! \brief directly divide a 3D vector by a scalar value. */
__ZEO_EXPORT zVec3D *zVec3DDivDRC(zVec3D *v, double k);
/*! \brief directly amplify a 3D vector by another. */
#define _zVec3DAmpDRC(v,a) do{\
  (v)->c.x *= (a)->c.x;\
  (v)->c.y *= (a)->c.y;\
  (v)->c.z *= (a)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DAmpDRC(zVec3D *v1, zVec3D *v2);
/*! \brief directly demagnify a 3D vector by another. */
#define _zVec3DDemDRC(v,d) do{\
  (v)->c.x /= (d)->c.x;\
  (v)->c.y /= (d)->c.y;\
  (v)->c.z /= (d)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DDemDRC(zVec3D *v1, zVec3D *v2);
/*! \brief directly concatenate a 3D vector multiplied by a scalar to another. */
#define _zVec3DCatDRC(v1,k,v2) do{\
  (v1)->c.x += (k) * (v2)->c.x;\
  (v1)->c.y += (k) * (v2)->c.y;\
  (v1)->c.z += (k) * (v2)->c.z;\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DCatDRC(zVec3D *v1, double k, zVec3D *v2);

/*! \brief inner/outer products.
 *
 * zVec3DInnerProd() calculates the inner product of two 3D
 * vectors, \a v1 and \a v2.
 *
 * zVec3DOuterProd() calculates the outer product of two 3D
 * vectors \a v1 and \a v2. The outer product is a 3D vector,
 * and put it into \a v, i.e. \a v = \a v1 x \a v2.
 *
 * zVec3DOuterProdNorm() calculates only the norm of the outer
 * product of \a v1 and \a v2.
 *
 * zVec3DGrassmannProd() calculates the scalar triple product
 * of \a v1, \a v2 and \a v3, i.e. \a v1.(\a v2 x \a v3), which
 * is also expressed as [ \a v1 \a v2 \a v3 ].
 *
 * zVec3DTripleProd() calculates the vector triple product of
 * \a v1, \a v2 and \a v3, and put it into \a v, i.e.
 * \a v = \a v1 x (\a v2 x \a v3).
 * \return
 * zVec3DInnerProd(), zVec3DOuterProdNorm() and
 * zVec3DGrassmannProd() return scalar values as results.
 *
 * zVec3DOuterProd() and zVec3DTripleProd() return a pointer \a v.
 * \notes
 * For zVec3DOuterProd() and zVec3DTripleProd(), it is allowed
 * to let \a v point to the same address with \a v1 or \a v2.
 */
#define _zVec3DInnerProd(v1,v2) ( (v1)->c.x*(v2)->c.x + (v1)->c.y*(v2)->c.y + (v1)->c.z*(v2)->c.z )
__ZEO_EXPORT double zVec3DInnerProd(zVec3D *v1, zVec3D *v2);
#define __zVec3DOuterProd(v1,v2,__x,__y,__z) do{\
  __x = (v1)->c.y * (v2)->c.z - (v1)->c.z * (v2)->c.y;\
  __y = (v1)->c.z * (v2)->c.x - (v1)->c.x * (v2)->c.z;\
  __z = (v1)->c.x * (v2)->c.y - (v1)->c.y * (v2)->c.x;\
} while(0)
#define _zVec3DOuterProd(v1,v2,v) do{\
  double __x, __y, __z;\
  __zVec3DOuterProd( v1, v2, __x, __y, __z );\
 _zVec3DCreate( v, __x, __y, __z );\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DOuterProd(zVec3D *v1, zVec3D *v2, zVec3D *v);
__ZEO_EXPORT double zVec3DOuterProdNorm(zVec3D *v1, zVec3D *v2);
__ZEO_EXPORT double zVec3DGrassmannProd(zVec3D *v1, zVec3D *v2, zVec3D *v3);
#define _zVec3DTripleProd(v1,v2,v3,v) do{\
  zVec3D __v;\
  __zVec3DOuterProd( v2, v3, __v.c.x, __v.c.y, __v.c.z );\
  _zVec3DOuterProd( v1, &__v, v );\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DTripleProd(zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v);

/*! \brief calculate norm of a 3D vector.
 *
 * zVec3DNorm() calculates a norm of a 3D vector \a v.
 * zVec3DSqrNorm() calculates a squared norm of \a v.
 * \return
 * zVec3DNorm() returns a norm of \a v.
 * zVec3DSqrNorm() returns a squared norm of \a v.
 */
#define _zVec3DSqrNorm(v) _zVec3DInnerProd( v, v )
__ZEO_EXPORT double zVec3DSqrNorm(zVec3D *v);
#define _zVec3DNorm(v) sqrt( _zVec3DSqrNorm(v) )
#define zVec3DNorm(v) sqrt( zVec3DSqrNorm(v) )

#define _zVec3DWSqrNorm(v,w) ( (v)->c.x*(v)->c.x*(w)->c.x + (v)->c.y*(v)->c.y*(w)->c.y + (v)->c.z*(v)->c.z*(w)->c.z )
__ZEO_EXPORT double zVec3DWSqrNorm(zVec3D *v, zVec3D *w);
#define _zVec3DWNorm(v,w) sqrt( _zVec3DWSqrNorm(v,w) )
#define zVec3DWNorm(v,w) sqrt( zVec3DWSqrNorm(v,w) )

/*! \brief calculate distance between two 3D vectors.
 *
 * zVec3DDist() calculates a distance between two points located
 * two position 3D vectors by \a v1 and \a v2.
 *
 * zVec3DSqrDist() calculates a squared distance between two points
 * located by \a v1 and \a v2.
 * \return
 * zVec3DDist() returns a distance between \a v1 and \a v2.
 * zVec3DSqrDist() returns a squared distance between \a v1 and \a v2.
 */
__ZEO_EXPORT double zVec3DSqrDist(zVec3D *v1, zVec3D *v2);
#define zVec3DDist(v1,v2) sqrt( zVec3DSqrDist((v1),(v2)) )

/*! \brief normalize a 3D vector.
 *
 * zVec3DNormalize() normalizes a 3D vector \a v and put it into \a nv.
 * zVec3DNormalizeDRC() normalizes the vector \a v directly.
 *
 * As a result of nomalization, the norm of \a nv will be 1.
 * \return
 * zVec3DNormalize() and zVec3DNormalizeNC() return the norm of the
 * originala pointer \a v.
 * If the norm of \a v is less than zTOL, -1is returned.
 */
__ZEO_EXPORT double zVec3DNormalizeNC(zVec3D *v, zVec3D *nv);
__ZEO_EXPORT double zVec3DNormalize(zVec3D *v, zVec3D *nv);
#define zVec3DNormalizeNCDRC(v) zVec3DNormalizeNC(v,v)
#define zVec3DNormalizeDRC(v)   zVec3DNormalize(v,v)

/* ********************************************************** */
/* geometry
 * ********************************************************** */

/*! \brief interior division.
 *
 * zVec3DInterDiv() calculates the interior division vector
 * of \a v1 and \a v2 with a division ratio \a ratio.
 * The result is put into \a v.
 *
 * i.e. \a v = (1-\a ratio)* \a v1 + \a ratio * \a v2.
 *
 * zVec3DMid() calculates the middle point vector of \a v1
 * and \a v2. The result is put into \a v.
 * i.e. \a v = ( \a v1 + \a v2 ) / 2 .
 * \return
 * Both functions return a pointer to \a v.
 */
#define _zVec3DInterDiv(v1,v2,r,v) do{\
  (v)->c.x = (v1)->c.x + (r) * ( (v2)->c.x - (v1)->c.x );\
  (v)->c.y = (v1)->c.y + (r) * ( (v2)->c.y - (v1)->c.y );\
  (v)->c.z = (v1)->c.z + (r) * ( (v2)->c.z - (v1)->c.z );\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DInterDiv(zVec3D *v1, zVec3D *v2, double ratio, zVec3D *v);

#define _zVec3DMid(v1,v2,v) do{\
  (v)->c.x = 0.5 * ( (v1)->c.x + (v2)->c.x );\
  (v)->c.y = 0.5 * ( (v1)->c.y + (v2)->c.y );\
  (v)->c.z = 0.5 * ( (v1)->c.z + (v2)->c.z );\
} while(0)
__ZEO_EXPORT zVec3D *zVec3DMid(zVec3D *v1, zVec3D *v2, zVec3D *v);

/*! \brief angle between the two vectors.
 *
 * zVec3DAngle() calculates the angle between the two vectors
 * \a v1 and \a v2. When \a n is not the null vector, signed
 * angle (i.e. the angle from \a v1 to \a v2 about the axis
 * along \a n) is computed.
 * \return
 * The value returned is the angle between \a v1 and \a v2.
 */
__ZEO_EXPORT double zVec3DAngle(zVec3D *v1, zVec3D *v2, zVec3D *n);

/*! \brief angle-axis error of two vectors.
 *
 * zVec3DAAError() calculates the angle-axis error of two vectors
 * \a v1 and \a v2. The result is stored in \a aa. Namely, \a v1
 * rotated by \a aa is parallel to \a v2.
 * \return
 * zVec3DAAError() returns a pointer \a aa.
 */
__ZEO_EXPORT zVec3D *zVec3DAAError(zVec3D *v1, zVec3D *v2, zVec3D *aa);

/*! \brief projection, orthogonalization and rotation of a 3D vector.
 *
 * zVec3DProj() projects vector \a v onto the line directed by
 * \a n, and put the result into \a pv; \a pv is parallel to
 * \a n and the subtraction vector from \a pv to \a v is
 * orthogonal to \a n.
 *
 * zVec3DOrthogonalize() orthogonalizes \a v with respect to \a n,
 * and put it into \a ov; \a ov is orthogonal to \a n.
 *
 * zVec3DOrthoSpace() creates the orthogonal space to \a v, and
 * put them into \a sv1 and \a sv2; \a v, \a sv1 and \a sv2 are
 * orthogonal with each other, and are normalized.
 * Note that the orthogonal is not unique in nature. This
 * function only creates "one of" them.
 *
 * zVec3DRot() rotates \a v by angle-axis vector \a aa, whose
 * direction is that of the rotation axis and norm is the
 * rotation angle. The result is set into \a rv.
 * \return
 * zVec3DProj() and zVec3DOrthogonalize() return a pointer to
 * \a pv and \a ov, respectively, or the null pointer if they
 * fails because of \a n being the zero vector.
 *
 * zVec3DOrthoSpace() returns the true value if it succeeds to
 * create the orthogonal space, or the false value, otherwise.
 *
 * zVec3DRot() returns a pointer to \a rv.
 */
__ZEO_EXPORT zVec3D *zVec3DProj(zVec3D *v, zVec3D *n, zVec3D *pv);
__ZEO_EXPORT zVec3D *zVec3DOrthogonalize(zVec3D *v, zVec3D *n, zVec3D *ov);
__ZEO_EXPORT bool zVec3DOrthoSpace(zVec3D *v, zVec3D *sv1, zVec3D *sv2);
__ZEO_EXPORT bool zVec3DOrthoNormalSpace(zVec3D *v, zVec3D *sv1, zVec3D *sv2);
__ZEO_EXPORT zVec3D *zVec3DRot(zVec3D *v, zVec3D *aa, zVec3D *rv);

/* ********************************************************** */
/* differential kinematics
 * ********************************************************** */

/*! \brief compute average velocity of a 3D vector.
 *
 * zVec3DDif() computes the avarage velocity when a 3D vector
 * \a v changes to \a vnew in the duration \a dt.
 * \a vel is a velocity vector computed.
 * \retval \a vel
 */
__ZEO_EXPORT zVec3D *zVec3DDif(zVec3D *v, zVec3D *vnew, double dt, zVec3D *vel);

/*! \brief convert from/to Eulerian angle differential to/from angular velocity.
 *
 * zZYXVelToAngVel() converts a set of differential values of z-y-x
 * Eulerian angles \a zyxvel at the attitude represented by z-y-x Eulerian
 * angles \a zyx to the equivalent angular velocity vector and puts it into
 * \a angvel.
 *
 * zZYXVelToAngVelSC() directly accepts sets of sine/cosine values for
 * z-y-x Eulerian angles. The set of \a sa/\a ca is for the first angle,
 * while that of \a sb/\a cb for the second.
 *
 * zAngVelToZYXVel() and zAngVelToZYXVelSC() are the inverse
 * conversions of zZYXVelToAngVel() and zZYXVelToAngVelSC(),
 * respectively.
 *
 * zZYZVelToAngVel() converts a set of differential values of z-y-z
 * Eulerian angles \a zyxvel at the attitude represented by z-y-z Eulerian
 * angles \a zyz to the equivalent angular velocity vector and puts it into
 * \a angvel.
 *
 * zZYZVelToAngVelSC() directly accepts sets of sine/cosine values for
 * z-y-z Eulerian angles. The set of \a sa/\a ca is for the first angle,
 * while that of \a sb/\a cb for the second.
 *
 * zAngVelToZYZVel() and zAngVelToZYZVelSC() are the inverse
 * conversions of zZYZVelToAngVel() and zZYZVelToAngVelSC(),
 * respectively.
 *
 * Note that the conversion from an angular velocity to the derivatives
 * of Eulerian angles has singular points due to the mathematical
 * representation. In the case of z-y-x Eulerian angle, points where
 * cosine of the second value is zero are singular. In the case of z-y-z
 * Eulerian angle, points where sine of the second value is zero are
 * singular. At such singular points, zAngVelToZYXVel(),
 * zAngVelToZYXVelSC(), zAngVelToZYZVel() and
 * zAngVelToZYZVelSC() do nothing.
 * \return
 * zZYXVelToAngVel(), zZYXVelToAngVelSC(), zZYZVelToAngVel()
 * and zZYZVelToAngVelSC() return a pointer \a angvel.
 *
 * zAngVelToZYXVel() and zAngVelToZYXVelSC() return a pointer
 * \a zyxvel.
 *
 * zAngVelToZYZVel() and zAngVelToZYZVelSC() return a pointer
 * \a zyzvel.
 * \sa
 * zMat3DZYX, zMat3DToZYX, zMat3DZYZ, zMat3DToZYZ
 */
__ZEO_EXPORT zVec3D *zZYXVelToAngVel(zVec3D *zyxvel, zVec3D *zyx, zVec3D *angvel);
__ZEO_EXPORT zVec3D *zZYXVelToAngVelSC(zVec3D *zyxvel, double sa, double ca, double sb, double cb, zVec3D *angvel);
__ZEO_EXPORT zVec3D *zAngVelToZYXVel(zVec3D *angvel, zVec3D *zyx, zVec3D *zyxvel);
__ZEO_EXPORT zVec3D *zAngVelToZYXVelSC(zVec3D *angvel, double sa, double ca, double sb, double cb, zVec3D *zyxvel);
__ZEO_EXPORT zVec3D *zZYZVelToAngVel(zVec3D *zyzvel, zVec3D *zyz, zVec3D *angvel);
__ZEO_EXPORT zVec3D *zZYZVelToAngVelSC(zVec3D *zyzvel, double sa, double ca, double sb, double cb, zVec3D *angvel);
__ZEO_EXPORT zVec3D *zAngVelToZYZVel(zVec3D *angvel, zVec3D *zyz, zVec3D *zyzvel);
__ZEO_EXPORT zVec3D *zAngVelToZYZVelSC(zVec3D *angvel, double sa, double ca, double sb, double cb, zVec3D *zyzvel);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief read a 3D vector from a ZTK format processor. */
__ZEO_EXPORT zVec3D *zVec3DFromZTK(zVec3D *v, ZTK *ztk);

/*! \brief scan and print a 3D vector.
 *
 * zVec3DFScan() scans three values from the current position of
 * a file \a fp and creates a 3D vector \a v from them.
 * zVec3DScan() scans three values from the standard input.
 *
 * zVec3DFPrint() prints a 3D vector \a v out to the current
 * position of a file \a fp in the following format:
 *  ( x, y, z )
 * When the null pointer is given, the following string is printed.
 *  (null 3D vector)
 * zVec3DPrint() prints \a v out to the standard output.
 *
 * zVec3DDataFPrint() prints a 3D vector \a v out to the current
 * position of a file \a fp in the following format:
 *  x y z
 * When the null pointer is given, nothing is printed.
 * zVec3DDataPrint() prints \a v out to the standard output in
 * the same format with zVec3DDataFPrint().
 *
 * zVec3DDataNLFPrint() prints a 3D vector \a v out to the current
 * position of a file \a fp in the same format with zVec3DDataFPrint()
 * and terminates it by the new line.
 * zVec3DDataNLPrint() prints \a v out to the standard output in
 * the same format with zVec3DDataNLFPrint().
 * \return
 * zVec3DFScan() and zVec3DScan() return a pointer to \a v.
 *
 * zVec3DFPrint(), zVec3DPrint(), zVec3DDataFPrint(), zVec3DDataPrint(),
 * zVec3DDataNLFPrint() and zVec3DDataNLPrint() return a pointer \a v.
 */
__ZEO_EXPORT zVec3D *zVec3DFScan(FILE *fp, zVec3D *v);
#define zVec3DScan(v) zVec3DFScan( stdin, (v) )
__ZEO_EXPORT zVec3D *zVec3DDataFPrint(FILE *fp, zVec3D *v);
#define zVec3DDataPrint(v) zVec3DDataFPrint( stdout, (v) )
__ZEO_EXPORT zVec3D *zVec3DDataNLFPrint(FILE *fp, zVec3D *v);
#define zVec3DDataNLPrint(v) zVec3DDataNLFPrint( stdout, (v) )
__ZEO_EXPORT zVec3D *zVec3DFPrint(FILE *fp, zVec3D *v);
#define zVec3DPrint(v) zVec3DFPrint( stdout, (v) )

#ifdef __cplusplus
inline zVec3D &zVec3D::create(double x, double y, double z){ _zVec3DCreate( this, x, y, z ); return *this; }
inline zVec3D &zVec3D::createPolar(double r, double theta, double phi){ zVec3DCreatePolar( this, r, theta, phi ); return *this; }
inline zVec3D &zVec3D::copy(zVec3D &src){ zVec3DCopy( &src, this ); return *this; }
inline zVec3D &zVec3D::zero(){ _zVec3DZero( this ); return *this; }
inline bool zVec3D::operator==(zVec3D &v){ return _zVec3DMatch( this, &v ); }
inline bool zVec3D::isEqual(zVec3D &v){ return _zVec3DEqual( this, &v ); }
inline bool zVec3D::isTol(double tol){ return _zVec3DIsTol( this, tol ); }
inline bool zVec3D::isTiny(){ return _zVec3DIsTiny( this ); }
inline bool zVec3D::isNan(){ return zVec3DIsNan( this ); }
inline zVec3D zVec3D::operator+(zVec3D &v){ zVec3D ret; _zVec3DAdd( this, &v, &ret ); return ret; }
inline zVec3D zVec3D::operator-(zVec3D &v){ zVec3D ret; _zVec3DSub( this, &v, &ret ); return ret; }
inline zVec3D zVec3D::operator-(){ zVec3D ret; _zVec3DRev( this, &ret ); return ret; }
inline zVec3D zVec3D::operator*(double k){ zVec3D ret; _zVec3DMul( this, k, &ret ); return ret; }
inline zVec3D zVec3D::operator*(zVec3D &v){ zVec3D ret; _zVec3DAmp( this, &v, &ret ); return ret; }
inline zVec3D zVec3D::operator/(double k){ zVec3D ret; zVec3DDiv( this, k, &ret ); return ret; }
inline zVec3D zVec3D::operator/(zVec3D &v){ zVec3D ret; _zVec3DDem( this, &v, &ret ); return ret; }
inline zVec3D &zVec3D::add(zVec3D &v){ _zVec3DAddDRC( this, &v ); return *this; }
inline zVec3D &zVec3D::sub(zVec3D &v){ _zVec3DSubDRC( this, &v ); return *this; }
inline zVec3D &zVec3D::rev(){ _zVec3DRevDRC( this ); return *this; }
inline zVec3D &zVec3D::mul(double k){ _zVec3DMulDRC( this, k ); return *this; }
inline zVec3D &zVec3D::div(double k){ zVec3DDivDRC( this, k ); return *this; }
inline zVec3D &zVec3D::amp(zVec3D &v){ _zVec3DAmpDRC( this, &v ); return *this; }
inline zVec3D &zVec3D::dem(zVec3D &v){ _zVec3DDemDRC( this, &v ); return *this; }
inline zVec3D &zVec3D::cat(double k, zVec3D &v){ _zVec3DCatDRC( this, k, &v ); return *this; }
inline double zVec3D::sqrNorm(){ return _zVec3DSqrNorm( this ); }
inline double zVec3D::norm(){ return sqrt( sqrNorm() ); }
inline zVec3D &zVec3D::normalize(){ zVec3DNormalizeNCDRC( this ); return *this; }
#endif /* __cplusplus */

/*! \struct zVec3DArray
 * \brief array class of 3D vectors.
 */
zArrayClass( zVec3DArray, zVec3D );

__END_DECLS

#include <zeo/zeo_vec3d_list.h>  /* 3D vector list */
#include <zeo/zeo_vec3d_tree.h>  /* 3D vector tree */

#endif /* __ZEO_VEC3D_H__ */
