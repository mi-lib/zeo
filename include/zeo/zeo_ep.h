/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ep - Euler parameter (unit quaternion) class.
 */

#ifndef __ZEO_EP_H__
#define __ZEO_EP_H__

#include <zeo/zeo_mat3d.h>

__BEGIN_DECLS

/*! \struct zEP
 * \brief Euler parameter.
 */
ZDEF_UNION( __ZEO_CLASS_EXPORT, zEP ){
  struct{
    double w;
    zVec3D v;
  } ex;
  double e[4];
#ifdef __cplusplus
  zEP() : e{ 1, 0, 0, 0 } {}
  zEP(const zEP &_ep);
  ~zEP(){}
  zEP &create(double w, double x, double y, double z);
  zEP &createAA(double theta, const zVec3D *axis);
  zEP &ident();
  zEP &copy(const zEP &src);
  bool isIdent();
  zVec3D &toAA(zVec3D &aa);
  zEP &fromAA(const zVec3D &aa);
  zMat3D &toMat3D(zMat3D &m);
  zEP &fromMat3D(const zMat3D &m);
  zVec3D &rot(const zVec3D &from, zVec3D &to);
  zEP &sub(const zEP &e);
  zEP &rev();
  zEP &mul(double k);
  zEP &cat(double k, const zEP &e);
  double norm();
  zEP &normalize();
#endif /* __cplusplus */
};

/*! \brief create and copy Euler parameter.
 *
 * zEPCreate() creates Euler parameter \a ep from four values.
 * zEPCreateAA() creates Euler parameter \a ep from a combination of angle and axis.
 * For both functions, \a ep is automatically normalized by zEPNormalize().
 *
 * zEPIdent() creates Euler parameter which is equivalent to the
 * identity transformation, namely, \a ep equals to [ 1, 0, 0, 0 ].
 *
 * zEPCopy() copies Euler parameter \a src to the other \a dest.
 * \return
 * zEPCreate(), zEPCreateAA() and zEPIdent() return a pointer \a ep.
 * zEPCopy() returns a pointer \a dest.
 * \sa
 * zEPNormalize
 */
__ZEO_EXPORT zEP *zEPCreate(zEP *ep, double w, double x, double y, double z);
__ZEO_EXPORT zEP *zEPCreateAA(zEP *ep, double theta, const zVec3D *axis);
#define zEPIdent(e)  zEPCreate( e, 1, 0, 0, 0 );
#define _zEPCopy(src,dest) zCopy( zEP, src, dest )
__ZEO_EXPORT zEP *zEPCopy(const zEP *src, zEP *dest);

__ZEO_EXPORT bool zEPIsIdent(const zEP *ep);

/*! \brief alternate angle-axis vector, attitude matrix and Euler parameter.
 *
 * zEP2AA() converts Euler parameter \a ep to the equivalent angle-axis
 * vector \a aa.
 *
 * zAA2EP() converts an angle-axis vector \a aa to the equivalent Euler
 * parameter \a ep.
 *
 * zMat3DFromEP() converts Euler parameter \a ep to the equivalent a 3x3
 * attitude matrix \a m.
 *
 * zMat3DToEP() converts a 3x3 attitude matrix \a m to the equivalent
 * Euler parameter \a ep.
 * \return
 * zAA2EP() returns a pointer \a ep.
 * zEP2AA() returns a pointer \a aa.
 * zMat3DFromEP() returns a pointer \a m.
 * zMat3DToEP() returns a pointer \a ep.
 */
__ZEO_EXPORT zVec3D *zEP2AA(const zEP *ep, zVec3D *aa);
__ZEO_EXPORT zEP *zAA2EP(const zVec3D *aa, zEP *ep);
__ZEO_EXPORT zMat3D *zMat3DFromEP(zMat3D *m, const zEP *ep);
__ZEO_EXPORT zEP *zMat3DToEP(const zMat3D *m, zEP *ep);

/*! \brief rotate a 3D vector by Euler parameter.
 *
 * zEPRotVec3D() rotates a 3D vector \a v by Euler parameter \a ep
 * and puts it into \a rv.
 * \return
 * zEPRotVec3D() returns a pointer \a rv.
 * \sa
 * zMat3DFromEP
 */
__ZEO_EXPORT zVec3D *zEPRotVec3D(const zEP *ep, const zVec3D *v, zVec3D *rv);

/*! \brief convert rotation velocity to Euler parameter derivative.
 *
 * zEPVel2AngVel() converts the derivative of Euler parameter \a epvel
 * at the attitude represented by \a ep to the equivalent angular velocity
 * \a angvel.
 *
 * zAngVel2EPVel() converts an angular velocity \a angvel at the attitude
 * \a ep to the equivalent derivative of Euler parameter \a epvel, where
 * \a epvel is perpendicular to \a ep.
 * \return
 * zEPVel2AngVel() returns a pointer \a angvel.
 * zAngVel2EPVel() returns a pointer \a epvel.
 */
__ZEO_EXPORT zVec3D *zEPVel2AngVel(const zEP *epvel, const zEP *ep, zVec3D *angvel);
__ZEO_EXPORT zEP *zAngVel2EPVel(const zVec3D *angvel, const zEP *ep, zEP *epvel);

/*! \brief Euler parameter arithmetics.
 *
 * zEPAdd() adds two Euler parameters \a ep1 and \a ep2. The result is put into \a ep.
 * zEPAddDRC() directly adds an Euler parameter \a ep2 to another \a ep1.
 * zEPSub() subtracts an Euler parameter \a ep2 from another \a ep1. The result is put into \a ep.
 * zEPSubDRC() directly subtracts an Euler parameter \a ep2 from another \a ep1.
 *
 * zEPRev() reverses an Euler parameter \a ep1. The result is put into \a ep.
 * zEPRevDRC() directly reverses an Euler parameter \a ep.
 *
 * zEPMul() multiplies an Euler parameter \a ep1 by a scalar value \a k. The result is put into \a ep.
 * zEPMulDRC() directly multiplies an Euler parameter \a ep by a scalar value \a k.
 *
 * zEPCat() multiplies an Euler parameter \a ep2 by a scalar value \a k. Namely, it concatenates \a ep2
 * multiplied by \a k to the other \a ep1. The result is put into \a ep.
 * zEPCatDRC() directly concatenates an Euler parameter \a ep2 multiplied by a scalar value \a k to the
 * other \a ep1.
 * \return
 * zEPAdd(), zEPSub(), zEPRev(), zEPMul(), and zEPCat() return a pointer \a ep.
 * zEPAddDRC(), zEPSubDRC(), and zEPCatDRC() return a pointer \a ep1.
 * zEPRevDRC() and zEPMulDRC() return a pointer \a e.
 */
__ZEO_EXPORT zEP *zEPAdd(const zEP *ep1, const zEP *ep2, zEP *ep);
__ZEO_EXPORT zEP *zEPSub(const zEP *ep1, const zEP *ep2, zEP *ep);
__ZEO_EXPORT zEP *zEPRev(const zEP *ep1, zEP *ep);
__ZEO_EXPORT zEP *zEPMul(const zEP *ep1, double k, zEP *ep);
__ZEO_EXPORT zEP *zEPCat(const zEP *ep1, double k, const zEP *ep2, zEP *ep);
#define zEPAddDRC(ep1,ep2)   zEPAdd( ep1, ep2, ep1 )
#define zEPSubDRC(ep1,ep2)   zEPSub( ep1, ep2, ep1 )
#define zEPRevDRC(ep)        zEPRev( ep, ep )
#define zEPMulDRC(ep,k)      zEPMul( ep, k, ep )
#define zEPCatDRC(ep1,k,ep2) zEPCat( ep1, k, ep2, ep1 )

__ZEO_EXPORT zEP *zEPDif(const zEP *ep1, const zEP *ep2, double dt, zEP *ep_vel);

/*! \brief inner product and normalization of the Euler parameter.
 *
 * zEPInnerProd() calculates the inner product of two Euler parameters \a ep1 and \a ep2.
 * zEPNorm() calculates the norm of an Euler parameter \a ep.
 *
 * zEPNormalize() directly normalizes an Euler parameter \a ep.
 * \return
 * zEPInnerProd() and zEPNorm() return the result value.
 * zEPNormalize() returns a pointer \a ep if succeeding. If the norm of \a ep is zero, it returns
 * the null pointer.
 */
__ZEO_EXPORT double zEPInnerProd(const zEP *ep1, const zEP *ep2);
__ZEO_EXPORT double zEPNorm(const zEP *ep);
__ZEO_EXPORT zEP *zEPNormalize(zEP *ep);

/*! \brief cascade a Euler parameter to another.
 */
__ZEO_EXPORT zEP *zEPCascade(const zEP *ep1, const zEP *ep2, zEP *ep);

/*! \brief interior division of Euler parameter.
 *
 * zEPInterDiv() calculates the interior division of two Euler parameters
 * \a ep1 and \a ep2 in accordance with the spherical interpolation.
 * \a t is the dividing ratio, which is regularly chosen within the range
 * from 0 to 1. When \a t is out of the range, zEPInterDiv() computes the
 * outer division for extrapolation. The result is put into \a ep.
 *
 * The computation is based on SLERP(spherical linear interpolation
 * proposed by K. Shoemake, 1985).
 * \return
 * zEPInterDiv() returns a pointer \a ep.
 * \sa
 * zMat3DInterDiv
 */
__ZEO_EXPORT zEP *zEPInterDiv(const zEP *ep1, const zEP *ep2, double t, zEP *ep);

/*! \brief interior division of two attitude matrices for SLERP.
 *
 * zMat3DInterDiv() calculates the interior division of two 3x3 attitude
 * matrices \a m1 and \a m2 in accordance with the spherical interpolation.
 * \a t is the dividing ratio, which is regularly chosen within the range
 * from 0 to 1. When \a t is out of the range, zMat3DInterDiv() computes
 * the outer division for extrapolation. The result is put into \a m.
 * The computation enables SLERP(spherical linear interpolation).
 * \return
 * zMat3DInterDiv() returns a pointer \a m.
 * \sa
 * zEPInterDiv
 */
__ZEO_EXPORT zMat3D *zMat3DInterDiv(const zMat3D *m1, const zMat3D *m2, double t, zMat3D *m);

/*! \brief print Euler parameter.
 *
 * zEPFPrint() prints Euler parameter to the current position of
 * a file \a fp in the following format.
 *  e0 { e1, e2, e3 }
 * zEPPrint() prints Euler parameter \a ep out to the standard output.
 * \return
 * zEPFPrint() and zEPPrint() return no value.
 */
__ZEO_EXPORT void zEPFPrint(FILE *fp, const zEP *ep);
#define zEPPrint(e) zEPFPrint( stdout, e )

__END_DECLS

#ifdef __cplusplus
inline zEP::zEP(const zEP &_ep){ _zEPCopy( &_ep, this ); }
inline zEP &zEP::create(double w, double x, double y, double z){ return *zEPCreate( this, w, x, y, z ); }
inline zEP &zEP::createAA(double theta, const zVec3D *axis){ return *zEPCreateAA( this, theta, axis ); }
inline zEP &zEP::ident(){ return *zEPIdent( this ); }
inline zEP &zEP::copy(const zEP &src){ zEPCopy( &src, this ); return *this; }
inline bool zEP::isIdent(){ return zEPIsIdent( this ); }
inline zVec3D &zEP::toAA(zVec3D &aa){ return *zEP2AA( this, &aa ); }
inline zEP &zEP::fromAA(const zVec3D &aa){ return *zAA2EP( &aa, this ); }
inline zMat3D &zEP::toMat3D(zMat3D &m){ return *zMat3DFromEP( &m, this ); }
inline zEP &zEP::fromMat3D(const zMat3D &m){ return *zMat3DToEP( &m, this ); }
inline zVec3D &zEP::rot(const zVec3D &from, zVec3D &to){ return *zEPRotVec3D( this, &from, &to ); }
inline zEP &zEP::sub(const zEP &e){ return *zEPSubDRC( this, &e ); }
inline zEP &zEP::rev(){ return *zEPRevDRC( this ); }
inline zEP &zEP::mul(double k){ return *zEPMulDRC( this, k ); }
inline zEP &zEP::cat(double k, const zEP &e){ return *zEPCatDRC( this, k, &e ); }
inline double zEP::norm(){ return zEPNorm( this ); }
inline zEP &zEP::normalize(){ return *zEPNormalize( this ); }

inline zEP operator+(const zEP &ep1, const zEP &ep2){
  zEP ret;
  zEPAdd( &ep1, &ep2, &ret );
  return ret;
}
inline zEP operator-(const zEP &ep1, const zEP &ep2){
  zEP ret;
  zEPSub( &ep1, &ep2, &ret );
  return ret;
}
inline zEP operator*(const zEP &ep, double k){
  zEP ret;
  zEPMul( &ep, k, &ret );
  return ret;
}
inline zEP operator*(double k, const zEP &ep){
  zEP ret;
  zEPMul( &ep, k, &ret );
  return ret;
}
#endif /* __cplusplus */

#endif /* __ZEO_EP_H__ */
