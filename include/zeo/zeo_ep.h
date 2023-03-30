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
ZDEF_UNION( zEP ){
  struct{
    double w;
    zVec3D v;
  } ex;
  double e[4];
#ifdef __cplusplus
  zEP() : e{ 1, 0, 0, 0 } {}
  zEP &create(double w, double x, double y, double z);
  zEP &createAA(double theta, zVec3D *axis);
  zEP &ident();
  zEP &copy(zEP &src);
  bool isIdent();
  zVec3D &toAA(zVec3D &aa);
  zEP &fromAA(zVec3D &aa);
  zMat3D &toMat3D(zMat3D &m);
  zEP &fromMat3D(zMat3D &m);
  zVec3D &rot(zVec3D &from, zVec3D &to);
  friend zEP operator+(zEP &ep1, zEP &ep2);
  friend zEP operator-(zEP &ep1, zEP &ep2);
  zEP operator*(double k);
  zEP &sub(zEP &e);
  zEP &rev();
  zEP &mul(double k);
  zEP &cat(double k, zEP &e);
  double norm();
  zEP &normalize();
#endif /* __cplusplus */
};

/*! \brief create and copy Euler parameter.
 *
 * zEPCreate() creates Euler parameter \a ep from four values. It is
 * automatically normalized.
 *
 * zEPCreateAA() creates Euler parameter \a ep from a combination of angle and axis.
 *
 * zEPIdent() creates Euler parameter which is equivalent to the
 * identity transformation, namely, \a ep equals to [ 1, 0, 0, 0 ].
 *
 * zEPCopy() copies Euler parameter \a src to the other \a dest.
 * \return
 * zEPCreate(), zEPCreateAA() and zEPIdent() return a pointer \a ep.
 * zEPCopy() returns a pointer \a dest.
 */
__EXPORT zEP *zEPCreate(zEP *ep, double w, double x, double y, double z);
__EXPORT zEP *zEPCreateAA(zEP *ep, double theta, zVec3D *axis);
#define zEPIdent(e)  zEPCreate( e, 1, 0, 0, 0 );
#define zEPCopy(s,d) zCopy( zEP, (s), (d) )

__EXPORT bool zEPIsIdent(zEP *ep);

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
__EXPORT zVec3D *zEP2AA(zEP *ep, zVec3D *aa);
__EXPORT zEP *zAA2EP(zVec3D *aa, zEP *ep);
__EXPORT zMat3D *zMat3DFromEP(zMat3D *m, zEP *ep);
__EXPORT zEP *zMat3DToEP(zMat3D *m, zEP *ep);

/*! \brief rotate a 3D vector by Euler parameter.
 *
 * zEPRotVec3D() rotates a 3D vector \a v by Euler parameter \a ep
 * and puts it into \a rv.
 * \return
 * zEPRotVec3D() returns a pointer \a rv.
 * \sa
 * zMat3DFromEP
 */
__EXPORT zVec3D *zEPRotVec3D(zEP *ep, zVec3D *v, zVec3D *rv);

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
__EXPORT zVec3D *zEPVel2AngVel(zEP *epvel, zEP *ep, zVec3D *angvel);
__EXPORT zEP *zAngVel2EPVel(zVec3D *angvel, zEP *ep, zEP *epvel);

/*! \brief Euler parameter arithmetics.
 *
 * zEPAdd() add two Euler parameters \a ep1 and \a ep2 and puts it into \a ep.
 * zEPSub() subtracts an Euler parameter \a ep2 from another \a ep1 and puts it into \a ep.
 *
 * zEPRev() reverses Euler parameter \a ep1 and puts it into \a ep.
 * zEPRevDRC() directly reverses Euler parameter \a ep.
 *
 * zEPMul() multiplies Euler parameter \a ep1 by a scalar value \a k and
 * puts it into \a ep.
 * zEPMulDRC() directly multiplies Euler parameter \a ep by a scalar value \a k.
 *
 * zEPCat() multiplies Euler parameter \a ep2 by a scalar value \a k,
 * concatenates it to the other \a ep1 and puts it into \a ep.
 * zEPCatDRC() directly concatenates Euler parameter \a ep2 multiplied by
 * a scalar value \a k to the other \a ep1.
 *
 * zEPInnerProd() calculates the inner products of two Euler parameters
 * \a ep1 and \a ep2.
 * zEPNorm() calculates the norm of Euler parameter \a ep.
 *
 * zEPNormalize() directly normalizes Euler parameter \a ep.
 * \notes
 * Since Euler parameter is a class of unit quaternions, zEPNormalize() is
 * automatically called in zEPCreateAA() and some other functions.
 * \return
 * zEPRev(), zEPMul() and zEPCat() return a pointer \a ep.
 *
 * zEPRevDRC(), zEPMulDRC(), zEPCatDRC() and zEPNormalize() return a pointer
 * to the result Euler parameter.
 *
 * zEPInnerProd() and zEPNorm() return the result value.
 */
__EXPORT zEP *zEPAdd(zEP *ep1, zEP *ep2, zEP *ep);
__EXPORT zEP *zEPSub(zEP *ep1, zEP *ep2, zEP *ep);
__EXPORT zEP *zEPRev(zEP *ep1, zEP *ep);
__EXPORT zEP *zEPMul(zEP *ep1, double k, zEP *ep);
__EXPORT zEP *zEPCat(zEP *ep1, double k, zEP *ep2, zEP *ep);
#define zEPSubDRC(e1,e)      zEPSub( e1, e, e1 )
#define zEPRevDRC(e)         zEPRev( e, e )
#define zEPMulDRC(e,k)       zEPMul( e, k, e )
#define zEPCatDRC(ep1,k,ep2) zEPCat( ep1, k, ep2, ep1 )

__EXPORT zEP *zEPDif(zEP *ep1, zEP *ep2, double dt, zEP *ep_vel);

__EXPORT double zEPInnerProd(zEP *ep1, zEP *ep2);
__EXPORT double zEPNorm(zEP *ep);
__EXPORT zEP *zEPNormalize(zEP *ep);

/*! \brief cascade a Euler parameter to another.
 */
__EXPORT zEP *zEPCascade(zEP *e1, zEP *e2, zEP *e);

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
__EXPORT zEP *zEPInterDiv(zEP *ep1, zEP *ep2, double t, zEP *ep);

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
__EXPORT zMat3D *zMat3DInterDiv(zMat3D *m1, zMat3D *m2, double t, zMat3D *m);

/*! \brief print Euler parameter.
 *
 * zEPFPrint() prints Euler parameter to the current position of
 * a file \a fp in the following format.
 *  e0 { e1, e2, e3 }
 * zEPPrint() prints Euler parameter \a ep out to the standard output.
 * \return
 * zEPFPrint() and zEPPrint() return no value.
 */
__EXPORT void zEPFPrint(FILE *fp, zEP *ep);
#define zEPPrint(e) zEPFPrint( stdout, e )

#ifdef __cplusplus
inline zEP &zEP::create(double w, double x, double y, double z){ return *zEPCreate( this, w, x, y, z ); }
inline zEP &zEP::createAA(double theta, zVec3D *axis){ return *zEPCreateAA( this, theta, axis ); }
inline zEP &zEP::ident(){ return *zEPIdent( this ); }
inline zEP &zEP::copy(zEP &src){ zEPCopy( &src, this ); return *this; }
inline bool zEP::isIdent(){ return zEPIsIdent( this ); }
inline zVec3D &zEP::toAA(zVec3D &aa){ return *zEP2AA( this, &aa ); }
inline zEP &zEP::fromAA(zVec3D &aa){ return *zAA2EP( &aa, this ); }
inline zMat3D &zEP::toMat3D(zMat3D &m){ return *zMat3DFromEP( &m, this ); }
inline zEP &zEP::fromMat3D(zMat3D &m){ return *zMat3DToEP( &m, this ); }
inline zVec3D &zEP::rot(zVec3D &from, zVec3D &to){ return *zEPRotVec3D( this, &from, &to ); }
inline zEP operator+(zEP &ep1, zEP &ep2){
  zEP e;
  zEPAdd( &ep1, &ep2, &e );
  return e;
}
inline zEP operator-(zEP &ep1, zEP &ep2){
  zEP e;
  zEPSub( &ep1, &ep2, &e );
  return e;
}
inline zEP zEP::operator*(double k){
  zEP e;
  zEPMul( this, k, &e );
  return e;
}
inline zEP &zEP::sub(zEP &e){ return *zEPSubDRC( this, &e ); }
inline zEP &zEP::rev(){ return *zEPRevDRC( this ); }
inline zEP &zEP::mul(double k){ return *zEPMulDRC( this, k ); }
inline zEP &zEP::cat(double k, zEP &e){ return *zEPCatDRC( this, k, &e ); }
inline double zEP::norm(){ return zEPNorm( this ); }
inline zEP &zEP::normalize(){ return *zEPNormalize( this ); }
#endif /* __cplusplus */

__END_DECLS

#endif /* __ZEO_EP_H__ */
