/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec6d - 6D spatial vector.
 */

#ifndef __ZEO_VEC6D_H__
#define __ZEO_VEC6D_H__

#include <zeo/zeo_vec3d.h>

__BEGIN_DECLS

/*! \struct zVec6D
 * \brief 6D spatial vector for linear/angular quantities.
 */
ZDEF_UNION( __ZEO_CLASS_EXPORT, zVec6D ){
  double e[6];
  zVec3D v[2];
  struct{
    zVec3D lin, ang;
  } t;
#ifdef __cplusplus
/*! \brief 6D zero vector and unit vectors */
  zVec6D &create(double x, double y, double z, double xa, double ya, double za);
  zVec6D &create(zVec3D &vl, zVec3D &va);
  zVec6D &copy(zVec6D &src);
  zVec6D &zero();
  bool operator==(zVec6D &v);
  bool isEqual(zVec6D &v);
  bool isTol(double tol);
  bool isTiny();
  zVec6D operator+(zVec6D &v);
  zVec6D operator-(zVec6D &v);
  zVec6D operator-();
  zVec6D operator*(double k);
  zVec6D operator/(double k);
  zVec6D add(zVec6D &v);
  zVec6D sub(zVec6D &v);
  zVec6D rev();
  zVec6D mul(double k);
  zVec6D div(double k);
  static const zVec6D zvec6Dzero;
  static const zVec6D zvec6Dlinx;
  static const zVec6D zvec6Dliny;
  static const zVec6D zvec6Dlinz;
  static const zVec6D zvec6Dangx;
  static const zVec6D zvec6Dangy;
  static const zVec6D zvec6Dangz;
#endif /* __cplusplus */
};

/*! \brief 6D zero vector and unit vectors */
#ifdef __cplusplus
#define ZVEC6DZERO ( (zVec6D *)&zVec6D::zvec6Dzero )
#define ZVEC6DLINX ( (zVec6D *)&zVec6D::zvec6Dlinx )
#define ZVEC6DLINY ( (zVec6D *)&zVec6D::zvec6Dliny )
#define ZVEC6DLINZ ( (zVec6D *)&zVec6D::zvec6Dlinz )
#define ZVEC6DANGX ( (zVec6D *)&zVec6D::zvec6Dangx )
#define ZVEC6DANGY ( (zVec6D *)&zVec6D::zvec6Dangy )
#define ZVEC6DANGZ ( (zVec6D *)&zVec6D::zvec6Dangz )
#else
__ZEO_EXPORT const zVec6D zvec6Dzero;
__ZEO_EXPORT const zVec6D zvec6Dlinx;
__ZEO_EXPORT const zVec6D zvec6Dliny;
__ZEO_EXPORT const zVec6D zvec6Dlinz;
__ZEO_EXPORT const zVec6D zvec6Dangx;
__ZEO_EXPORT const zVec6D zvec6Dangy;
__ZEO_EXPORT const zVec6D zvec6Dangz;
#define ZVEC6DZERO ( (zVec6D *)&zvec6Dzero )
#define ZVEC6DLINX ( (zVec6D *)&zvec6Dlinx )
#define ZVEC6DLINY ( (zVec6D *)&zvec6Dliny )
#define ZVEC6DLINZ ( (zVec6D *)&zvec6Dlinz )
#define ZVEC6DANGX ( (zVec6D *)&zvec6Dangx )
#define ZVEC6DANGY ( (zVec6D *)&zvec6Dangy )
#define ZVEC6DANGZ ( (zVec6D *)&zvec6Dangz )
#endif /* __cplusplus */

#define zVec6DLin(u)         ( &(u)->t.lin )
#define zVec6DAng(u)         ( &(u)->t.ang )
#define zVec6DSetLin(u,l)    zVec3DCopy( l, zVec6DLin(u) )
#define zVec6DSetAng(u,r)    zVec3DCopy( r, zVec6DAng(u) )

/*! \brief create a 6D vector.
 *
 * zVec6DCreate() creates a 6D vector \a v from 6 components \a x, \a y,
 * \a z, \a xa, \a ya and \a za.
 * \return
 * zVec6DCreate() returns a pointer \a v.
 */
#define _zVec6DCreate(v,x,y,z,xa,ya,za) do{\
  _zVec3DCreate( zVec6DLin(v), x, y, z );\
  _zVec3DCreate( zVec6DAng(v), xa, ya, za );\
} while(0)
__ZEO_EXPORT zVec6D *zVec6DCreate(zVec6D *v, double x, double y, double z, double xa, double ya, double za);

/*! \brief create a 6D vector from two 3D vectors.
 *
 * zVec6DFromVec3D creates a 6D vector \a v from two 3D vectors \a vl
 * and \a va.
 * \return
 * zVec6DFromVec3D returns a pointer \a v.
 */
#define _zVec6DFromVec3D(v,vl,va) do{\
  zVec3DCopy( vl, zVec6DLin(v) );\
  zVec3DCopy( va, zVec6DAng(v) );\
} while(0)
__ZEO_EXPORT zVec6D *zVec6DFromVec3D(zVec6D *v, zVec3D *vl, zVec3D *va);

/*! \brief copy a 6D vector to another.
 *
 * zVec6DCopy() copies a 6D vector \a src to another 6D vector \a dest.
 * \return
 * zVec6DCopy() returns a pointer \a dest.
 */
#define zVec6DCopy(s,d) zCopy( zVec6D, s, d )

/*! \brief zero a 6D vector.
 *
 * zVec6DZero() sets all components of a 6D vector \a v for zeros.
 * \return
 * zVec6DZero() returns a pointer \a v.
 */
#define _zVec6DZero(v) _zVec6DCreate( v, 0, 0, 0, 0, 0, 0 )
#define zVec6DZero(v)  zVec6DCreate( v, 0, 0, 0, 0, 0, 0 )

/*! \brief check if the two 6D vectors are equal.
 *
 * zVec6DMatch() and zVec6DEqual() check if two 6D vectors \a v1 and \a v2
 * are equal. They return a boolean value as a result.
 *
 * zVec6DMatch() strictly compares two vectors \a v1 and \a v2 while
 * zVec6DEqual() checks if the error between \a v1 and \a v2 are
 * sufficiently small.
 * \return
 * zVec6DMatch() and zVec6DEqual() return the true value if \a v1 and \a v2
 * are equal. Otherwise, the false value is returned.
 */
#define _zVec6DMatch(v1,v2) ( _zVec3DMatch( zVec6DLin(v1), zVec6DLin(v2) ) && _zVec3DMatch( zVec6DAng(v1), zVec6DAng(v2) ) )
__ZEO_EXPORT bool zVec6DMatch(zVec6D *v1, zVec6D *v2);
#define _zVec6DEqual(v1,v2) ( _zVec3DEqual( zVec6DLin(v1), zVec6DLin(v2) ) && _zVec3DEqual( zVec6DAng(v1), zVec6DAng(v2) ) )
__ZEO_EXPORT bool zVec6DEqual(zVec6D *v1, zVec6D *v2);

/*! \brief check if a 6D vector is tiny.
 *
 * zVec6DIsTol() checks if the absolute values of every components of
 * a 6D vector \a v are smaller than \a tol.
 *
 * zVec6DIsTiny() applies zTOL (defined in "zeo_misc.h") to the tolerance
 * of zVec6DIsTol().
 * \return
 * zVec6DIsTol() and zVec6DIsTiny() return the true value when the absolute
 * values of every components of \a v are smaller than \a tol and zTOL,
 * respectively. Otherwise, the false value is returned.
 * \notes
 * \a tol must be positive.
 * \sa
 * zIsTol, zIsTiny
 */
#define _zVec6DIsTol(v,tol) ( _zVec3DIsTol( zVec6DLin(v), tol ) && _zVec3DIsTol( zVec6DAng(v), tol ) )
__ZEO_EXPORT bool zVec6DIsTol(zVec6D *v, double tol);
#define _zVec6DIsTiny(v) _zVec6DIsTol( v, zTOL )
#define zVec6DIsTiny(v) zVec6DIsTol( v, zTOL )

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/*! \brief the four rules of the arithmetics for 6D vector.
 *
 * zVec6DAdd() adds two 6D vectors \a v1 and \a v2 and puts it into \a v.
 *
 * zVec6DSub() subtracts a 6D vector \a v2 from the other \a v1 and puts
 * it into \a v.
 *
 * zVec6DRev() reverses a 6D vector \a v and puts it into \a rv.
 *
 * zVec6DMul() multiplies a 6D vector \a v by a scalar value \a k and
 * puts it into \a mv.
 *
 * zVec6DDiv() divides a 6D vector \a v by a scalar value \a k and puts
 * it into \a dv.
 *
 * zVec6DCat() multiplies a 6D vector \a v2 by a scalar value \a k,
 * concatenates it to the other \a v1 and puts it into \a v.
 *
 * zVec6DAddDRC() directly adds \a v2 to \a v1.
 *
 * zVec6DSubDRC() directly subtracts \a v2 from \a v1.
 *
 * zVec6DRevDRC() directly reverses \a v.
 *
 * zVec6DMulDRC() directly multiplies \a v by \a k.
 *
 * zVec6DDivDRC() directly divides \a v by \a k.
 *
 * zVec6DCat() directly concatenates \a v2 multiplied \a v2 by \a k to \a v1.
 * \return
 * Each function returns a pointer to the result vector.
 *
 * zVec6DDiv() and zVec6DDivDRC() return the null pointer if \a k is zero.
 */
#define _zVec6DAdd(v1,v2,v) do{\
  _zVec3DAdd( zVec6DLin(v1), zVec6DLin(v2), zVec6DLin(v) );\
  _zVec3DAdd( zVec6DAng(v1), zVec6DAng(v2), zVec6DAng(v) );\
} while(0)
__ZEO_EXPORT zVec6D *zVec6DAdd(zVec6D *v1, zVec6D *v2, zVec6D *v);
#define _zVec6DSub(v1,v2,v) do{\
  _zVec3DSub( zVec6DLin(v1), zVec6DLin(v2), zVec6DLin(v) );\
  _zVec3DSub( zVec6DAng(v1), zVec6DAng(v2), zVec6DAng(v) );\
} while(0)
__ZEO_EXPORT zVec6D *zVec6DSub(zVec6D *v1, zVec6D *v2, zVec6D *v);
#define _zVec6DRev(v,rv) do{\
  _zVec3DRev( zVec6DLin(v), zVec6DLin(rv) );\
  _zVec3DRev( zVec6DAng(v), zVec6DAng(rv) );\
} while(0)
__ZEO_EXPORT zVec6D *zVec6DRev(zVec6D *v, zVec6D *rv);
#define _zVec6DMul(v,k,mv) do{\
  _zVec3DMul( zVec6DLin(v), k, zVec6DLin(mv) );\
  _zVec3DMul( zVec6DAng(v), k, zVec6DAng(mv) );\
} while(0)
__ZEO_EXPORT zVec6D *zVec6DMul(zVec6D *v, double k, zVec6D *mv);
__ZEO_EXPORT zVec6D *zVec6DDiv(zVec6D *v, double k, zVec6D *dv);
#define _zVec6DCat(v1,k,v2,v) do{\
  _zVec3DCat( zVec6DLin(v1), k, zVec6DLin(v2), zVec6DLin(v) );\
  _zVec3DCat( zVec6DAng(v1), k, zVec6DAng(v2), zVec6DAng(v) );\
} while(0)
__ZEO_EXPORT zVec6D *zVec6DCat(zVec6D *v1, double k, zVec6D *v2, zVec6D *v);

#define _zVec6DAddDRC(v1,v2) do{\
  _zVec3DAddDRC( zVec6DLin(v1), zVec6DLin(v2) );\
  _zVec3DAddDRC( zVec6DAng(v1), zVec6DAng(v2) );\
} while(0)
#define zVec6DAddDRC(v1,v2) zVec6DAdd(v1,v2,v1)
#define _zVec6DSubDRC(v1,v2) do{\
  _zVec3DSubDRC( zVec6DLin(v1), zVec6DLin(v2) );\
  _zVec3DSubDRC( zVec6DAng(v1), zVec6DAng(v2) );\
} while(0)
#define zVec6DSubDRC(v1,v2) zVec6DSub(v1,v2,v1)
#define _zVec6DRevDRC(v) do{\
  _zVec3DRevDRC( zVec6DLin(v) );\
  _zVec3DRevDRC( zVec6DAng(v) );\
} while(0)
#define zVec6DRevDRC(v) zVec6DRev(v,v)
#define _zVec6DMulDRC(v,k) do{\
  _zVec3DMulDRC( zVec6DLin(v), k );\
  _zVec3DMulDRC( zVec6DAng(v), k );\
} while(0)
#define zVec6DMulDRC(v,k) zVec6DMul(v,k,v)
#define zVec6DDivDRC(v,k) zVec6DDiv(v,k,v)
#define _zVec6DCatDRC(v1,k,v2) do{\
  _zVec3DCatDRC( zVec6DLin(v1), k, zVec6DLin(v2) );\
  _zVec3DCatDRC( zVec6DAng(v1), k, zVec6DAng(v2) );\
} while(0)
#define zVec6DCatDRC(v1,k,v2) zVec6DCat(v1,k,v2,v1)

/*! \brief inner product of two 6D vectors.
 *
 * zVec6DInnerProd() returns the inner product of two 6D vectors
 * \a v1 and \a v2.
 * \retval \a v1 ^T \a v2
 */
#define _zVec6DInnerProd(v1,v2) ( _zVec3DInnerProd( zVec6DLin(v1), zVec6DLin(v2) ) + _zVec3DInnerProd( zVec6DAng(v1), zVec6DAng(v2) ) )
__ZEO_EXPORT double zVec6DInnerProd(zVec6D *v1, zVec6D *v2);

/*! \brief shift a 6D vector in 3D space.
 *
 * zVec6DLinShift() shifts the velocity type of a 6D vector \a src at
 * the original point to the equivalent 6D vector \a dest at the point
 * \a pos, namely:
 *  v1 = v0 + w0 x \a pos
 *  w1 = w0
 * where \a src=[v0 w0]^T and \a dest=[v1 w1]^T.
 *
 * zVec6DLinShiftDRC() directly shifts the velocity type of a 6D vector
 * \a vec at the original point to the equivalent 6D vector at the point
 * \a pos.
 *
 * zVec6DAngShift() shifts the force type of a 6D vector \a src which
 * works at \a pos to the equivalent 6D vector \a dest which works at
 * the original point, namely:
 *  f1 = f0
 *  n1 = n0 + \a pos x f0
 * where \a src=[f0 n0]^T and \a dest=[f1 n1]^T.
 *
 * zVec6DAngShiftDRC() directly shifts the force type of a 6D vector \a vec
 * which works at \a pos to the equivalent 6D vector which works at the
 * original point.
 * \return
 * zVec6DLinShift() and zVec6DAngShift() return a pointer \a dest.
 * zVec6DLinShiftDRC() and zVec6DAngShift() return a pointer \a vec.
 */
__ZEO_EXPORT zVec6D *zVec6DLinShiftDRC(zVec6D *vec, zVec3D *pos);
__ZEO_EXPORT zVec6D *zVec6DLinShift(zVec6D *src, zVec3D *pos, zVec6D *dest);
__ZEO_EXPORT zVec6D *zVec6DAngShiftDRC(zVec6D *vec, zVec3D *pos);
__ZEO_EXPORT zVec6D *zVec6DAngShift(zVec6D *src, zVec3D *pos, zVec6D *dest);

/* ********************************************************** */
/* differential kinematics
 * ********************************************************** */

__ZEO_EXPORT zVec6D *zVec6DDif(zVec6D *v, zVec6D *vnew, double dt, zVec6D *vel);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 6D spatial vector from a ZTK format processor. */
__ZEO_EXPORT zVec6D *zVec6DFromZTK(zVec6D *v, ZTK *ztk);

/*! \brief scan and print a 6D vector.
 *
 * zVec6DFScan() scans six values from the current position of
 * a file \a fp and creates a 6D vector \a v from them.
 * zVec6DScan() scans six values from the standard input.
 *
 * zVec6DFPrint() prints a 6D vector \a v out to the current position
 * of a file \a fp in the following format:
 *  ( x, y, z, xa, ya, za )
 * When the null pointer is given, the following string is printed.
 *  (null 6D vector)
 * zVec6DPrint() prints \a v out to the standard output.
 *
 * zVec6DDataFPrint() prints a 6D vector \a v out to the current
 * position of a file \a fp in the following format:
 *  x y z xa ya za
 * When the null pointer is given, nothing is printed.
 * zVec6DDataPrint() prints \a v out to the standard output in
 * the same format with zVec6DDataFPrint().
 * \return
 * zVec6DFScan() and zVec6DScan() return a pointer \a v.
 *
 * zVec6DFPrint(), zVec6DPrint(), zVec6DDataFPrint() and zVec6DDataPrint()
 * return no value.
 */
__ZEO_EXPORT zVec6D *zVec6DFScan(FILE *fp, zVec6D *v);
#define zVec6DScan(v) zVec6DFScan( stdin, (v) )
__ZEO_EXPORT zVec6D *zVec6DDataFPrint(FILE *fp, zVec6D *v);
#define zVec6DDataPrint(v) zVec6DDataFPrint( stdout, (v) )
__ZEO_EXPORT zVec6D *zVec6DDataNLFPrint(FILE *fp, zVec6D *v);
#define zVec6DDataNLPrint(v) zVec6DDataNLFPrint( stdout, (v) )
__ZEO_EXPORT zVec6D *zVec6DFPrint(FILE *fp, zVec6D *v);
#define zVec6DPrint(v) zVec6DFPrint( stdout, (v) )

#ifdef __cplusplus
/*! \brief 6D zero vector and unit vectors */
inline zVec6D &zVec6D::create(double x, double y, double z, double xa, double ya, double za){ _zVec6DCreate( this, x, y, z, xa, ya, za ); return *this; }
inline zVec6D &zVec6D::create(zVec3D &vl, zVec3D &va){ _zVec6DFromVec3D( this, &vl, &va ); return *this; }
inline zVec6D &zVec6D::copy(zVec6D &src){ zVec6DCopy( &src, this ); return *this; }
inline zVec6D &zVec6D::zero(){ _zVec6DZero( this ); return *this; }
inline bool zVec6D::operator==(zVec6D &v){ return _zVec6DMatch( this, &v ); }
inline bool zVec6D::isEqual(zVec6D &v){ return _zVec6DEqual( this, &v ); }
inline bool zVec6D::isTol(double tol){ return _zVec6DIsTol( this, tol ); }
inline bool zVec6D::isTiny(){ return _zVec6DIsTiny( this ); }
inline zVec6D zVec6D::operator+(zVec6D &v){ zVec6D ret; _zVec6DAdd( this, &v, &ret ); return ret; }
inline zVec6D zVec6D::operator-(zVec6D &v){ zVec6D ret; _zVec6DSub( this, &v, &ret ); return ret; }
inline zVec6D zVec6D::operator-(){ zVec6D ret; _zVec6DRev( this, &ret ); return ret; }
inline zVec6D zVec6D::operator*(double k){ zVec6D ret; _zVec6DMul( this, k, &ret ); return ret; }
inline zVec6D zVec6D::operator/(double k){ zVec6D ret; zVec6DDiv( this, k, &ret ); return ret; }
inline zVec6D zVec6D::add(zVec6D &v){ _zVec6DAddDRC( this, &v ); return *this; }
inline zVec6D zVec6D::sub(zVec6D &v){ _zVec6DSubDRC( this, &v ); return *this; }
inline zVec6D zVec6D::rev(){ _zVec6DRevDRC( this ); return *this; }
inline zVec6D zVec6D::mul(double k){ _zVec6DMulDRC( this, k ); return *this; }
inline zVec6D zVec6D::div(double k){ return *zVec6DDivDRC( this, k ); }
#endif /* __cplusplus */

__END_DECLS

#endif /* __ZEO_VEC6D_H__ */
