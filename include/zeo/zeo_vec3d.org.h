/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d - 3D vector.
 */

#ifndef __ZEO_VEC3D_H__
#define __ZEO_VEC3D_H__

#include <zeo/zeo_misc.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zVec3D
 * 3D vector class
 * ********************************************************** */

typedef union{
  struct{
    double e[3];
  } a;
  struct{
    double x, y, z;
  } c;
} zVec3D;

/*! \brief 3D zero vector and unit vectors */
extern const zVec3D zvec3Dzero;
extern const zVec3D zvec3Dx;
extern const zVec3D zvec3Dy;
extern const zVec3D zvec3Dz;
#define ZVEC3DZERO ( (zVec3D *)&zvec3Dzero )
#define ZVEC3DX    ( (zVec3D *)&zvec3Dx )
#define ZVEC3DY    ( (zVec3D *)&zvec3Dy )
#define ZVEC3DZ    ( (zVec3D *)&zvec3Dz )

/*! \brief create a 3D vector.
 *
 * zVec3DCreate() creates a 3D vector \a v which consists of \a x, \a y and \a z
 * for its components.
 * \return
 * zVec3DCreate() returns a pointer \a v.
 */
#define _zVec3DCreate(v,x,y,z) do{\
  (v)->e[zX] = (x);\
  (v)->e[zY] = (y);\
  (v)->e[zZ] = (z);\
} while(0)
__EXPORT zVec3D *zVec3DCreate(zVec3D *v, double x, double y, double z);

/*! \brief create a 3D vector by the set of value for a polar expression.
 *
 * zVec3DCreatePolar() creates a vector \a v from a set of values
 * for a polar coordinate ( \a r, \a theta, \a phi ), where
 * \a r is for the radius from the original point, \a theta is for
 * the longitudinal angle, and \a phi is for the latitudinal angle.
 * \retval \a v
 */
__EXPORT zVec3D *zVec3DCreatePolar(zVec3D *v, double r, double theta, double phi);

/*! \brief copy a 3D vector to another.
 *
 * zVec3DCopy() copies a 3D vector \a src to another \a dest.
 * \return
 * zVec3DCopy() returns a pointer \a dest.
 */
#define _zVec3DCopy(s,d) do{\
  (d)->e[zX] = (s)->e[zX];\
  (d)->e[zY] = (s)->e[zY];\
  (d)->e[zZ] = (s)->e[zZ];\
} while(0)
__EXPORT zVec3D *zVec3DCopy(zVec3D *src, zVec3D *dest);

/*! \brief clear a 3D vector to zero.
 *
 * zVec3DClear() sets all components of \a v for zero.
 * \return
 * zVec3DClear() returns a pointer \a v.
 */
#define _zVec3DClear(v) _zVec3DCreate( v, 0, 0, 0 )
#define zVec3DClear(v)  zVec3DCreate( v, 0, 0, 0 )

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
#define _zVec3DMatch(v1,v2) ( (v1)->e[zX] == (v2)->e[zX] && (v1)->e[zY] == (v2)->e[zY] && (v1)->e[zZ] == (v2)->e[zZ] )
__EXPORT bool zVec3DMatch(zVec3D *v1, zVec3D *v2);
#define _zVec3DEqual(v1,v2) ( zIsTiny( (v1)->e[zX] - (v2)->e[zX] ) && zIsTiny( (v1)->e[zY] - (v2)->e[zY] ) && zIsTiny( (v1)->e[zZ] - (v2)->e[zZ] ) )
__EXPORT bool zVec3DEqual(zVec3D *v1, zVec3D *v2);

/*! \brief check if 3D vector is tiny.
 *
 * zVec3DIsTol() checks if the absolute values of every
 * components of 3D vector \a v are smaller than \a tol.
 *
 * zVec3DIsTiny() applies zTOL (defined in "zm_misc.h") to
 * the tolerance of zVec3DIsTol().
 * \return
 * zVec3DIsTol() and zVec3DIsTiny() return the true value when
 * the absolute values of every components of \a v are smaller
 * than \a tol and zTOL, respectively, or the false value,
 * otherwise.
 * \notes
 * \a tol must be positive.
 * \sa
 * zIsTol, zIsTiny
 */
#define _zVec3DIsTol(v,tol) ( zIsTol( (v)->e[zX], (tol) ) && zIsTol( (v)->e[zY], (tol) ) && zIsTol( (v)->e[zZ], (tol) ) )
__EXPORT bool zVec3DIsTol(zVec3D *v, double tol);
#define _zVec3DIsTiny(v) _zVec3DIsTol( v, zTOL )
#define zVec3DIsTiny(v)  zVec3DIsTol( v, zTOL )

/*! \brief check if a 3D vector includes NaN or Inf components. */
__EXPORT bool zVec3DIsNan(zVec3D *v);

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/*! \brief the four rules of the arithmetics for 3D vector.
 *
 * zVec3DAdd() adds the two 3D vectors, \a v1 and \a v2.
 * The result is put into \a v.
 *
 * zVec3DSub() subtracts the 3D vector \a v2 from the other
 * \a v1. The result is put into \a v.
 *
 * zVec3DRev() reverses the 3D vector \a v. The result is put
 * into \a rv.
 *
 * zVec3DMul() multiplies the 3D vector \a v by value \a k.
 * The result is put into \a mv.
 *
 * zVec3DDiv() divides the 3D vector \a v by \a k.
 * The result is put into \a dv.
 *
 * zVec3DCat() concatenates the 3D vector \a v2 to \a v1,
 * multiplied by a scalar value \a k. The result is put into \a v.
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
 * zVec3DCat() directly concatenates \a v2 multiplied \a v2
 * by \a k to \a v1.
 * \return
 * Each function returns a pointer to the resultant vector.
 *
 * zVec3DDiv() and zVec3DDivDRC() return the null pointer if
 * the given scalar value is zero.
 */
#define _zVec3DAdd(v1,v2,v) do{\
  (v)->e[zX] = (v1)->e[zX] + (v2)->e[zX];\
  (v)->e[zY] = (v1)->e[zY] + (v2)->e[zY];\
  (v)->e[zZ] = (v1)->e[zZ] + (v2)->e[zZ];\
} while(0)
__EXPORT zVec3D *zVec3DAdd(zVec3D *v1, zVec3D *v2, zVec3D *v);

#define _zVec3DSub(v1,v2,v) do{\
  (v)->e[zX] = (v1)->e[zX] - (v2)->e[zX];\
  (v)->e[zY] = (v1)->e[zY] - (v2)->e[zY];\
  (v)->e[zZ] = (v1)->e[zZ] - (v2)->e[zZ];\
} while(0)
__EXPORT zVec3D *zVec3DSub(zVec3D *v1, zVec3D *v2, zVec3D *v);

#define _zVec3DRev(v,rv) do{\
  (rv)->e[zX] = -(v)->e[zX];\
  (rv)->e[zY] = -(v)->e[zY];\
  (rv)->e[zZ] = -(v)->e[zZ];\
} while(0)
__EXPORT zVec3D *zVec3DRev(zVec3D *v, zVec3D *rv);

#define _zVec3DMul(v,k,mv) do{\
  (mv)->e[zX] = (k) * (v)->e[zX];\
  (mv)->e[zY] = (k) * (v)->e[zY];\
  (mv)->e[zZ] = (k) * (v)->e[zZ];\
} while(0)
__EXPORT zVec3D *zVec3DMul(zVec3D *v, double k, zVec3D *mv);

__EXPORT zVec3D *zVec3DDiv(zVec3D *v, double k, zVec3D *dv);

#define _zVec3DAmp(v,a,av) do{\
  (av)->e[zX] = (a)->e[zX] * (v)->e[zX];\
  (av)->e[zY] = (a)->e[zY] * (v)->e[zY];\
  (av)->e[zZ] = (a)->e[zZ] * (v)->e[zZ];\
} while(0)
__EXPORT zVec3D *zVec3DAmp(zVec3D *v, zVec3D *a, zVec3D *av);

#define _zVec3DCat(v1,k,v2,v) do{\
  (v)->e[zX] = (v1)->e[zX] + (k) * (v2)->e[zX];\
  (v)->e[zY] = (v1)->e[zY] + (k) * (v2)->e[zY];\
  (v)->e[zZ] = (v1)->e[zZ] + (k) * (v2)->e[zZ];\
} while(0)
__EXPORT zVec3D *zVec3DCat(zVec3D *v1, double k, zVec3D *v2, zVec3D *v);

#define _zVec3DAddDRC(v1,v2) do{\
  (v1)->e[zX] += (v2)->e[zX];\
  (v1)->e[zY] += (v2)->e[zY];\
  (v1)->e[zZ] += (v2)->e[zZ];\
} while(0)
#define zVec3DAddDRC(v1,v2) zVec3DAdd(v1,v2,v1)
#define _zVec3DSubDRC(v1,v2) do{\
  (v1)->e[zX] -= (v2)->e[zX];\
  (v1)->e[zY] -= (v2)->e[zY];\
  (v1)->e[zZ] -= (v2)->e[zZ];\
} while(0)
#define zVec3DSubDRC(v1,v2) zVec3DSub(v1,v2,v1)
#define _zVec3DRevDRC(v) do{\
  (v)->e[zX] = -(v)->e[zX];\
  (v)->e[zY] = -(v)->e[zY];\
  (v)->e[zZ] = -(v)->e[zZ];\
} while(0)
#define zVec3DRevDRC(v) zVec3DRev(v,v)
#define _zVec3DMulDRC(v,k) do{\
  (v)->e[zX] *= (k);\
  (v)->e[zY] *= (k);\
  (v)->e[zZ] *= (k);\
} while(0)
#define zVec3DMulDRC(v,k) zVec3DMul(v,k,v)
#define zVec3DDivDRC(v,k) zVec3DDiv(v,k,v)
#define _zVec3DAmpDRC(v,a) do{\
  (v)->e[zX] *= (a)->e[zX];\
  (v)->e[zY] *= (a)->e[zY];\
  (v)->e[zZ] *= (a)->e[zZ];\
} while(0)
#define zVec3DAmpDRC(v,a) zVec3DAmp(v,a,v)
#define _zVec3DCatDRC(v1,k,v2) do{\
  (v1)->e[zX] += (k) * (v2)->e[zX];\
  (v1)->e[zY] += (k) * (v2)->e[zY];\
  (v1)->e[zZ] += (k) * (v2)->e[zZ];\
} while(0)
#define zVec3DCatDRC(v1,k,v2) zVec3DCat(v1,k,v2,v1)

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
#define _zVec3DInnerProd(v1,v2) ( (v1)->e[zX]*(v2)->e[zX] + (v1)->e[zY]*(v2)->e[zY] + (v1)->e[zZ]*(v2)->e[zZ] )
__EXPORT double zVec3DInnerProd(zVec3D *v1, zVec3D *v2);
#define _zVec3DOuterProd(v1,v2,v) do{\
 (v)->e[zX] = (v1)->e[zY] * (v2)->e[zZ] - (v1)->e[zZ] * (v2)->e[zY];\
 (v)->e[zY] = (v1)->e[zZ] * (v2)->e[zX] - (v1)->e[zX] * (v2)->e[zZ];\
 (v)->e[zZ] = (v1)->e[zX] * (v2)->e[zY] - (v1)->e[zY] * (v2)->e[zX];\
} while(0)
__EXPORT zVec3D *zVec3DOuterProd(zVec3D *v1, zVec3D *v2, zVec3D *v);
__EXPORT double zVec3DOuterProdNorm(zVec3D *v1, zVec3D *v2);
__EXPORT double zVec3DGrassmannProd(zVec3D *v1, zVec3D *v2, zVec3D *v3);
__EXPORT zVec3D *zVec3DTripleProd(zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v);

/*! \brief calculate norm of a 3D vector.
 *
 * zVec3DNorm() calculates a norm of a 3D vector \a v.
 * zVec3DSqrNorm() calculates a squared norm of \a v.
 * \return
 * zVec3DNorm() returns a norm of \a v.
 * zVec3DSqrNorm() returns a squared norm of \a v.
 */
#define _zVec3DSqrNorm(v) _zVec3DInnerProd( v, v )
__EXPORT double zVec3DSqrNorm(zVec3D *v);
#define _zVec3DNorm(v) sqrt( _zVec3DSqrNorm(v) )
#define zVec3DNorm(v) sqrt( zVec3DSqrNorm(v) )

#define _zVec3DWSqrNorm(v,w) ( (v)->e[zX]*(v)->e[zX]*(w)->e[zX] + (v)->e[zY]*(v)->e[zY]*(w)->e[zY] + (v)->e[zZ]*(v)->e[zZ]*(w)->e[zZ] )
__EXPORT double zVec3DWSqrNorm(zVec3D *v, zVec3D *w);
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
__EXPORT double zVec3DSqrDist(zVec3D *v1, zVec3D *v2);
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
__EXPORT double zVec3DNormalizeNC(zVec3D *v, zVec3D *nv);
__EXPORT double zVec3DNormalize(zVec3D *v, zVec3D *nv);
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
  (v)->e[zX] = (v1)->e[zX] + (r) * ( (v2)->e[zX] - (v1)->e[zX] );\
  (v)->e[zY] = (v1)->e[zY] + (r) * ( (v2)->e[zY] - (v1)->e[zY] );\
  (v)->e[zZ] = (v1)->e[zZ] + (r) * ( (v2)->e[zZ] - (v1)->e[zZ] );\
} while(0)
__EXPORT zVec3D *zVec3DInterDiv(zVec3D *v1, zVec3D *v2, double ratio, zVec3D *v);

#define _zVec3DMid(v1,v2,v) do{\
  (v)->e[zX] = 0.5 * ( (v1)->e[zX] + (v2)->e[zX] );\
  (v)->e[zY] = 0.5 * ( (v1)->e[zY] + (v2)->e[zY] );\
  (v)->e[zZ] = 0.5 * ( (v1)->e[zZ] + (v2)->e[zZ] );\
} while(0)
__EXPORT zVec3D *zVec3DMid(zVec3D *v1, zVec3D *v2, zVec3D *v);

/*! \brief angle between the two vectors.
 *
 * zVec3DAngle() calculates the angle between the two vectors
 * \a v1 and \a v2. When \a n is not the null vector, signed
 * angle (i.e. the angle from \a v1 to \a v2 about the axis
 * along \a n) is computed.
 * \return
 * The value returned is the angle between \a v1 and \a v2.
 */
__EXPORT double zVec3DAngle(zVec3D *v1, zVec3D *v2, zVec3D *n);

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
__EXPORT zVec3D *zVec3DProj(zVec3D *v, zVec3D *n, zVec3D *pv);
__EXPORT zVec3D *zVec3DOrthogonalize(zVec3D *v, zVec3D *n, zVec3D *ov);
__EXPORT bool zVec3DOrthoSpace(zVec3D *v, zVec3D *sv1, zVec3D *sv2);
__EXPORT zVec3D *zVec3DRot(zVec3D *v, zVec3D *aa, zVec3D *rv);

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
__EXPORT zVec3D *zVec3DDif(zVec3D *v, zVec3D *vnew, double dt, zVec3D *vel);

/*! \brief convert from/to Eulerian angle differential to/from angular velocity.
 *
 * zVec3DZYXVel2AngVel() converts a set of differential values of z-y-x
 * Eulerian angles \a zyxvel at the attitude represented by z-y-x Eulerian
 * angles \a zyx to the equivalent angular velocity vector and puts it into
 * \a angvel.
 *
 * zVec3DZYXVel2AngVelSC() directly accepts sets of sine/cosine values for
 * z-y-x Eulerian angles. The set of \a sa/\a ca is for the first angle,
 * while that of \a sb/\a cb for the second.
 *
 * zVec3DAngVel2ZYXVel() and zVec3DAngVel2ZYXVelSC() are the inverse
 * conversions of zVec3DZYXVel2AngVel() and zVec3DZYXVel2AngVelSC(),
 * respectively.
 *
 * zVec3DZYZVel2AngVel() converts a set of differential values of z-y-z
 * Eulerian angles \a zyxvel at the attitude represented by z-y-z Eulerian
 * angles \a zyz to the equivalent angular velocity vector and puts it into
 * \a angvel.
 *
 * zVec3DZYZVel2AngVelSC() directly accepts sets of sine/cosine values for
 * z-y-z Eulerian angles. The set of \a sa/\a ca is for the first angle,
 * while that of \a sb/\a cb for the second.
 *
 * zVec3DAngVel2ZYZVel() and zVec3DAngVel2ZYZVelSC() are the inverse
 * conversions of zVec3DZYZVel2AngVel() and zVec3DZYZVel2AngVelSC(),
 * respectively.
 *
 * Note that the conversion from an angular velocity to the derivatives
 * of Eulerian angles has singular points due to the mathematical
 * representation. In the case of z-y-x Eulerian angle, points where
 * cosine of the second value is zero are singular. In the case of z-y-z
 * Eulerian angle, points where sine of the second value is zero are
 * singular. At such singular points, zVec3DAngVel2ZYXVel(),
 * zVec3DAngVel2ZYXVelSC(), zVec3DAngVel2ZYZVel() and
 * zVec3DAngVel2ZYZVelSC() do nothing.
 * \return
 * zVec3DZYXVel2AngVel(), zVec3DZYXVel2AngVelSC(), zVec3DZYZVel2AngVel()
 * and zVec3DZYZVel2AngVelSC() return a pointer \a angvel.
 *
 * zVec3DAngVel2ZYXVel() and zVec3DAngVel2ZYXVelSC() return a pointer
 * \a zyxvel.
 *
 * zVec3DAngVel2ZYZVel() and zVec3DAngVel2ZYZVelSC() return a pointer
 * \a zyzvel.
 * \sa
 * zMat3DZYX, zMat3DToZYX, zMat3DZYZ, zMat3DToZYZ
 */
__EXPORT zVec3D *zVec3DZYXVel2AngVel(zVec3D *zyxvel, zVec3D *zyx, zVec3D *angvel);
__EXPORT zVec3D *zVec3DZYXVel2AngVelSC(zVec3D *zyxvel, double sa, double ca, double sb, double cb, zVec3D *angvel);
__EXPORT zVec3D *zVec3DAngVel2ZYXVel(zVec3D *angvel, zVec3D *zyx, zVec3D *zyxvel);
__EXPORT zVec3D *zVec3DAngVel2ZYXVelSC(zVec3D *angvel, double sa, double ca, double sb, double cb, zVec3D *zyxvel);
__EXPORT zVec3D *zVec3DZYZVel2AngVel(zVec3D *zyzvel, zVec3D *zyz, zVec3D *angvel);
__EXPORT zVec3D *zVec3DZYZVel2AngVelSC(zVec3D *zyzvel, double sa, double ca, double sb, double cb, zVec3D *angvel);
__EXPORT zVec3D *zVec3DAngVel2ZYZVel(zVec3D *angvel, zVec3D *zyz, zVec3D *zyzvel);
__EXPORT zVec3D *zVec3DAngVel2ZYZVelSC(zVec3D *angvel, double sa, double ca, double sb, double cb, zVec3D *zyzvel);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief input and output a 3D vector.
 *
 * zVec3DFRead() reads three values from the current position of a file
 * \a fp and creates a 3D vector \a v from them.
 * zVec3DRead() reads three values from the standard input.
 *
 * zVec3DFWrite() outputs a 3D vector \a v to the current position of
 * the file \a fp in the following format:
 *  ( x, y, z )
 * When the null pointer is given, the following string is output.
 *  (null 3D vector)
 * zVec3DWrite() outputs \a v to the standard output.
 *
 * zVec3DDataFWrite() outputs a 3D vector \a v to the current position
 * of the file \a fp in the following format:
 *  x y z
 * When the null pointer is given, it outputs nothing.
 * zVec3DDataWrite() outputs \a v to the standard output in the same
 * format with zVec3DDataFWrite().
 *
 * zVec3DDataNLFWrite() outputs a 3D vector \a v to the current position
 * of the file \a fp in the same format with zVec3DDataFWrite() and
 * terminates it by the new line.
 * zVec3DDataNLWrite() outputs \a v to the standard output in the same
 * format with zVec3DDataNLFWrite().
 * \return
 * zVec3DFRead() and zVec3DRead() return a pointer to \a v.
 *
 * zVec3DFWrite(), zVec3DWrite(), zVec3DDataFWrite(), zVec3DDataWrite(),
 * zVec3DDataNLFWrite() and zVec3DDataNLWrite() return a pointer \a v.
 */
__EXPORT zVec3D *zVec3DFRead(FILE *fp, zVec3D *v);
#define zVec3DRead(v) zVec3DFRead( stdin, (v) )
__EXPORT zVec3D *zVec3DDataFWrite(FILE *fp, zVec3D *v);
#define zVec3DDataWrite(v) zVec3DDataFWrite( stdout, (v) )
__EXPORT zVec3D *zVec3DDataNLFWrite(FILE *fp, zVec3D *v);
#define zVec3DDataNLWrite(v) zVec3DDataNLFWrite( stdout, (v) )
__EXPORT zVec3D *zVec3DFWrite(FILE *fp, zVec3D *v);
#define zVec3DWrite(v) zVec3DFWrite( stdout, (v) )

/*! \brief XML output.
 * ... yet testing.
 */
__EXPORT void zVec3DFWriteXML(FILE *fp, zVec3D *v);

__END_DECLS

#include <zeo/zeo_vec3d_list.h>  /* 3D vector list */
#include <zeo/zeo_vec3d_tree.h>  /* 3D vector tree */

#endif /* __ZEO_VEC3D_H__ */
