/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_frame3d - 3D frame.
 */

#ifndef __ZEO_FRAME3D_H__
#define __ZEO_FRAME3D_H__

#include <zeo/zeo_mat3d.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zFrame3D
 * 3D frame class
 * ********************************************************** */

#ifdef __cplusplus
struct zFrame3D{
#else
typedef struct{
#endif /* __cplusplus */
  zVec3D pos;
  zMat3D att;
#ifdef __cplusplus
  zVec3D &setPos(zVec3D &p){ zVec3DCopy( &p, &pos ); return pos; }
  zMat3D &setAtt(zMat3D &a){ zMat3DCopy( &a, &att ); return att; }
  zFrame3D &create(zVec3D &p, zMat3D &m);
  zFrame3D &createZYX(double x, double y, double z, double azim, double elev, double tilt);
  zFrame3D &createZYX(double array[6]);
  zFrame3D &createZYX(zVec6D &v);
  zFrame3D &createZYZ(double x, double y, double z, double bearing, double pitch, double bank);
  zFrame3D &createZYZ(double array[6]);
  zFrame3D &createZYZ(zVec6D &v);
  zFrame3D &createAA(double x, double y, double z, double xa, double ya, double za);
  zFrame3D &createAA(double array[6]);
  zFrame3D &createAA(zVec6D &v);
  zFrame3D &createDH(double a, double alpha, double d, double theta);
  zFrame3D &copy(zFrame3D &src);
  zFrame3D &ident();
  bool isEqual(zFrame3D &f);
  bool isIdent();
  zVec3D xform(zVec3D &v);
  zVec3D xformInv(zVec3D &v);
  zVec3D &xformDRC(zVec3D &v);
  zVec3D &xformInvDRC(zVec3D &v);
  zFrame3D inv();
  zVec6D xformLin(zVec6D &v);
  zVec6D xformAng(zVec6D &v);
  zFrame3D twist(zVec6D &t);
  zVec6D toZYX();
  zVec6D toZYZ();
  zVec6D toAA();
  static const zFrame3D zframe3Dident;
};
#define ZFRAME3DIDENT ( (zFrame3D *)&zFrame3D::zframe3Dident )
#else
} zFrame3D;
/*! \brief the identity frame. */
extern const zFrame3D zframe3Dident;
#define ZFRAME3DIDENT ( (zFrame3D *)&zframe3Dident )
#endif /* __cplusplus */

#define zFrame3DPos(f) ( &(f)->pos )
#define zFrame3DAtt(f) ( &(f)->att )

#define zFrame3DSetPos(f,p) zVec3DCopy( p, zFrame3DPos(f) )
#define zFrame3DSetAtt(f,r) zMat3DCopy( r, zFrame3DAtt(f) )

/*! \brief create, copy and initialize 3D frame.
 *
 * zFrame3DCreate() creates a 3D frame \a f with a position
 * of the original point \a p and the attitude matrix of \a m.
 *
 * zFrame3DCopy() copies a 3D frame \a src to \a dest.
 *
 * zFrame3DIdent() initializes \a f as the identity frame,
 * namely, a frame with the zero position vector and the identity
 * attitude matrix.
 * \return
 * zFrame3DCreate() returns a pointer \a f.
 *
 * zFrame3DCopy() and zFrame3DIdent() return no value.
 */
__EXPORT zFrame3D *zFrame3DCreate(zFrame3D *f, zVec3D *p, zMat3D *m);
#define zFrame3DCopy(src,dest) ( *(dest) = *(src) )
#define zFrame3DIdent(f) zFrame3DCopy( ZFRAME3DIDENT, f )

/*! \brief check if two 3D frames are equal. */
#define _zFrame3DEqual(f1,f2) ( _zVec3DEqual( zFrame3DPos(f1), zFrame3DPos(f2) ) && _zMat3DEqual( zFrame3DAtt(f1), zFrame3DAtt(f2) ) )
__EXPORT bool zFrame3DEqual(zFrame3D *f1, zFrame3D *f2);

/*! \brief check if a 3D frame is the identity frame. */
#define _zFrame3DIsIdent(f) _zFrame3DEqual( f, ZFRAME3DIDENT )
__EXPORT bool zFrame3DIsIdent(zFrame3D *f);

/*! \brief transform coordinates of a 3D vector.
 *
 * zXform3D() transforms a 3D vector \a v by a frame \a f. The
 * result is put into \a tv. Suppose \a f is for the
 * transformation from a frame S1 to S0, \a tv points where \a v
 * points in S1 with respect to S0.
 *
 * On the same assumption for \a f, zXform3DInv() transforms
 * a 3D vector \a v in S0 to that with respect to S1 by the
 * inverse frame of \a f. The result is put into \a tv.
 *
 * zXform3DDRC() directly transforms the 3D vector \a v by a
 * frame \a f.
 *
 * zXform3DInvDRC() directly transforms a 3D vector \a v by the
 * inverse of \a f.
 * \return
 * zXform3D(), zXform3DInv(), zXform3DDRC() and zXform3DInvDRC()
 * return a pointer to the result.
 * \notes
 * zXform3DInv() expects that the attitude matrix of \a f is an
 * orthonormal matrix.
 */
#define _zXform3D( f, v, tv ) do{\
  _zMulMat3DVec3D( zFrame3DAtt(f), v, tv );\
  _zVec3DAddDRC( tv, zFrame3DPos(f) );\
} while(0)
__EXPORT zVec3D *zXform3D(zFrame3D *f, zVec3D *v, zVec3D *tv);

#define _zXform3DInv( f, v, tv ) do{\
  _zVec3DSub( v, zFrame3DPos(f), tv );\
  _zMulMat3DTVec3DDRC( zFrame3DAtt(f), tv );\
} while(0)
__EXPORT zVec3D *zXform3DInv(zFrame3D *f, zVec3D *v, zVec3D *tv);

#define zXform3DDRC(f,v)    zXform3D(f,v,v)
#define zXform3DInvDRC(f,v) zXform3DInv(f,v,v)

/*! \brief inverse and cascaded transformations of a frame.
 *
 * zFrame3DInv() calculates the inverse transformation frame
 * of the given 3D frame \a f. The result is put into \a fi.
 *
 * zFrame3DCascade() cascades the 3D frame \a f1 to the other
 * \a f2. The result is put into \a f.
 * That is, suppose \a c1 is the transformation from a frame
 * named S1 to another frame S0, and \a f2 for that from a
 * frame S2 to S1, \a f is the transformation from S2 to S0.
 *
 * zFrame3DXform() calculates the transformation frame from
 * a frame \a f1 to another \a f2. The result is put into \a f.
 * Suppose \a f1 is the transformation from a frame named S1
 * to another frame S0, and \a f2 is that from a frame S2 to
 * S0, \a f is the transformation from S2 to S1, namely, the
 * cascade of the inverse of \a f1 and \a f2.
 * \return
 * These functions return a pointer to the resulted frame.
 * \notes
 * zFrame3DInv() expects that the attitude matrix of \a f is
 * an orthonormal matrix. For these three functions, it is not
 * permitted to let any argument point to the same address with
 * the other arguments. When some of them are equal, anything
 * might happen.
 */
#define _zFrame3DInv( f, fi ) do{\
  _zMat3DT( zFrame3DAtt(f), zFrame3DAtt(fi) );\
  _zMulMat3DVec3D( zFrame3DAtt(fi), zFrame3DPos(f), zFrame3DPos(fi) );\
  zVec3DRevDRC( zFrame3DPos(fi) );\
} while(0)
__EXPORT zFrame3D *zFrame3DInv(zFrame3D *f, zFrame3D *fi);

#define _zFrame3DCascade( f1, f2, f ) do{\
  zMulMat3DMat3D( zFrame3DAtt(f1), zFrame3DAtt(f2), zFrame3DAtt(f) );\
  _zXform3D( f1, zFrame3DPos(f2), zFrame3DPos(f) );\
} while(0)
__EXPORT zFrame3D *zFrame3DCascade(zFrame3D *f1, zFrame3D *f2, zFrame3D *f);

#define _zFrame3DXform( f1, f2, f ) do{\
  zMulMat3DTMat3D( zFrame3DAtt(f1), zFrame3DAtt(f2), zFrame3DAtt(f) );\
  _zXform3DInv( f1, zFrame3DPos(f2), zFrame3DPos(f) );\
} while(0)
__EXPORT zFrame3D *zFrame3DXform(zFrame3D *f1, zFrame3D *f2, zFrame3D *f);

/*! \brief transform a 6D vector.
 */
__EXPORT zVec6D *zXform6DLin(zFrame3D *f, zVec6D *v, zVec6D *vc);
__EXPORT zVec6D *zXform6DAng(zFrame3D *f, zVec6D *v, zVec6D *vc);

/*! \brief twist a frame by a torsion vector
 *   (position offset & angle-axis rotation).
 */
__EXPORT zFrame3D *zFrame3DTwist(zFrame3D *f1, zVec6D *t, zFrame3D *f2);

/*! \brief error between two frames.
 *
 * zFrame3DError() calculates the error vector between two frames
 * from \a f2 to \a f1 (note the order).
 * The result is put into \a err. Intuitively, \a err equals to
 * \a f1 minus \a f2.
 * \retval \a err
 * \sa zMat3DError
 */
__EXPORT zVec6D *zFrame3DError(zFrame3D *f1, zFrame3D *f2, zVec6D *err);

/*! \brief create a frame from a handy expression.
 *
 * zFrame3DFromZYX() creates a 3D frame whose original point is at
 * ( \a x, \a y, \a z ) and attitude is expressed by z-y-x
 * Eulerian angle.
 *
 * zFrame3DFromZYZ() creates a 3D frame whose original point is at
 * ( \a x, \a y, \a z ) and attitude is expressed by z-y-z
 * Eulerian angle.
 *
 * zFrame3DFromAA() creates a 3D frame whose original point is at
 * ( \a x, \a y, \a z ) and attitude is expressed by the equivalent
 * angle-axis vector.
 *
 * zFrame3DFromDH() creates a 3D frame from modified Denaviet-Hartenberg
 * (DH) parameters (\a a, \a alpha, \a d, \a theta).
 *
 * For any of these functions, the result is put into \a f.
 * \return
 * All these functions return a pointer \a f.
 * \sa
 * zMat3DFromZYX, zMat3DFromZYZ, zMat3DFromAA
 */
__EXPORT zFrame3D *zFrame3DFromZYX(zFrame3D *f, double x, double y, double z, double azim, double elev, double tilt);
__EXPORT zFrame3D *zFrame3DFromZYZ(zFrame3D *f, double x, double y, double z, double heading, double pitch, double bank);
__EXPORT zFrame3D *zFrame3DFromAA(zFrame3D *f, double x, double y, double z, double xa, double ya, double za);
__EXPORT zFrame3D *zFrame3DFromDH(zFrame3D *f, double a, double alpha, double d, double theta);

/*! \brief convert a 3D frame to an array or a 6D vector.
 *
 * zArrayToFrame3DZYX() converts an array of double-precision
 * floating point values with [ x, y, z, azimuth, elevation, tilt ]
 * pointed by \a array to a 3D frame. The contents of \a array
 * is for the position of the original point and z-y-x Eulerian
 * angle. The result is put into \a f.
 *
 * zFrame3DZYXToArray() converts a 3D frame \a f to an array
 * of double-precision floating point values
 * [ x, y, z, azimuth, elevation, tilt ] of which the first
 * three values are for the position of the original point and
 * the last three for z-y-x Eulerian angle. The result is put
 * into a buffer pointed by \a array.
 *
 * zVec6DToFrame3DZYX() converts a 6D vector \a v to a 3D frame.
 * \a v is a quasi vector for [ x, y, z, azimuth, elevation, tilt ],
 * which is for the position of the original point and z-y-x
 * Eulerian angle. The result is put into \a f.
 *
 * zFrame3DZYXToVec6D() converts a 3D frame \a f to a 6D quasi
 * vector which is for [ x, y, z, azimuth, elevation, tilt ],
 * of which the first three values are for the position of the
 * original point and the last three are for z-y-x Eulerian angle.
 * The result is put into \a v.
 *
 * zArrayToFrame3DZYZ() converts an array of double-precision
 * floating point values with [ x, y, z, heading, pitch, bank ]
 * pointed by \a array to a 3D frame. The components of \a array
 * is for the position of the original point and z-y-z Eulerian
 * angle. The result is put into \a f.
 *
 * zFrame3DZYZToArray() converts a 3D frame \a f to an array of
 * double-precision floating point values [ x, y, z, heading, pitch, bank ],
 * of which the first three values are for the position of the
 * original point and the last three are for z-y-z Eulerian angle.
 * The result is put into a buffer pointed by \a array.
 *
 * zVec6DToFrame3DZYZ() converts a 6D vector \a v to a 3D frame
 * \a f. \a v is a quasi vector for [ x, y, z, heading, pitch, bank ],
 * which is for the position of the original point and z-y-z
 * Eulerian angle. The result is put into \a f.
 *
 * zFrame3DZYZToVec6D() converts a 3D frame \a f to a 6D quasi
 * vector \a v, which is for [ x, y, z, heading, pitch, bank ],
 * of which the first three values are for the position of the
 * original point and the last three are for z-y-z Eulerian angle.
 * The result is put into \a v.
 * \return
 * zArrayToFrame3DZYX(), zVec6DToFrame3DZYX(), zArrayToFrame3DZYZ()
 * and zVec6DToFrame3DZYZ() return a pointer \a f.
 *
 * zFrame3DZYXToArray() and zFrame3DZYZToArray() return a pointer
 * \a array.
 *
 * zFrame3DZYXToVec6D() and zFrame3DZYZToVec6D() return a pointer
 * \a v.
 * \notes
 * For zFrame3DZYXToArray() and zFrame3DZYZToArray(), the buffer
 * pointed by \a array must have enough size more than or equal to
 * six. If not, anything may happen.
 */
__EXPORT zFrame3D *zArrayToFrame3DZYX(double *array, zFrame3D *f);
__EXPORT double *zFrame3DToArrayZYX(zFrame3D *f, double *array);
__EXPORT zFrame3D *zVec6DToFrame3DZYX(zVec6D *v, zFrame3D *f);
__EXPORT zVec6D *zFrame3DToVec6DZYX(zFrame3D *f, zVec6D *v);

__EXPORT zFrame3D *zArrayToFrame3DZYZ(double *array, zFrame3D *f);
__EXPORT double *zFrame3DToArrayZYZ(zFrame3D *f, double *array);
__EXPORT zFrame3D *zVec6DToFrame3DZYZ(zVec6D *v, zFrame3D *f);
__EXPORT zVec6D *zFrame3DToVec6DZYZ(zFrame3D *f, zVec6D *v);

__EXPORT zFrame3D *zArrayToFrame3DAA(double *array, zFrame3D *f);
__EXPORT double *zFrame3DToArrayAA(zFrame3D *f, double *array);
__EXPORT zFrame3D *zVec6DToFrame3DAA(zVec6D *v, zFrame3D *f);
__EXPORT zVec6D *zFrame3DToVec6DAA(zFrame3D *f, zVec6D *v);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 3D frame from a ZTK format processor. */
__EXPORT zFrame3D *zFrame3DFromZTK(zFrame3D *f, ZTK *ztk);
/* read DH parameters from a ZTK format processor. */
__EXPORT zFrame3D *zFrame3DDHFromZTK(zFrame3D *f, ZTK *ztk);

/*! \brief scan and print a 3D frame.
 *
 * zFrame3DFScan() scans 12 values from the current position of
 * a file \a fp and creates a 3D frame from them. The meaning of
 * the sequencial value is for
 *  a11, a12, a13, x,
 *  a21, a22, a23, y,
 *  a31, a32, a33, z
 * The left 3x3 values are for the 3D attitude matrix. Namely,
 * each { a11, a21, a31 }, { a12, a22, a32 } and { a13, a23, a33 }
 * means a base of the frame, while the right 3x1 values are for
 * the position of the original point { x, y, z }. The result is
 * put into \a f.
 * zFrame3DScan() scans 12 values from the standard input and
 * creates a 3D frame \a f from them.
 *
 * zFrame3DFPrint() prints a 3D frame \a f out to the current
 * position of a file \a fp in the following style.
 *  {
 *   a11, a12, a13, x,
 *   a21, a22, a23, y
 *   a31, a32, a33, z
 *  }
 * When the null pointer is given, it prints the following string.
 *  (null 3D frame)
 * zFrame3DPrint() prints a 3D frame \a f out to the standard output.
 * \return
 * zFrame3DFScan(), zFrame3DScan(), zFrame3DDHFScan() and
 * zFrame3DDHScan() return a pointer \a f.
 *
 * zFrame3DFPrint() and zFrame3DPrint() return no value.
 */
__EXPORT zFrame3D *zFrame3DFScan(FILE *fp, zFrame3D *f);
#define zFrame3DScan(f) zFrame3DFScan( stdin, (f) )

__EXPORT void zFrame3DFPrint(FILE *fp, zFrame3D *f);
#define zFrame3DPrint(f) zFrame3DFPrint( stdout, (f) )

#ifdef __cplusplus
inline zFrame3D &zFrame3D::create(zVec3D &p, zMat3D &m){ return *zFrame3DCreate( this, &p, &m ); }
inline zFrame3D &zFrame3D::createZYX(double x, double y, double z, double azim, double elev, double tilt){ return *zFrame3DFromZYX( this, x, y, z, azim, elev, tilt ); }
inline zFrame3D &zFrame3D::createZYX(double array[6]){ return *zArrayToFrame3DZYX( array, this ); }
inline zFrame3D &zFrame3D::createZYX(zVec6D &v){ return *zVec6DToFrame3DZYX( &v, this ); }
inline zFrame3D &zFrame3D::createZYZ(double x, double y, double z, double bearing, double pitch, double bank){ return *zFrame3DFromZYZ( this, x, y, z, bearing, pitch, bank ); }
inline zFrame3D &zFrame3D::createZYZ(double array[6]){ return *zArrayToFrame3DZYZ( array, this ); }
inline zFrame3D &zFrame3D::createZYZ(zVec6D &v){ return *zVec6DToFrame3DZYZ( &v, this ); }
inline zFrame3D &zFrame3D::createAA(double x, double y, double z, double xa, double ya, double za){ return *zFrame3DFromAA( this, x, y, z, xa, ya, za ); }
inline zFrame3D &zFrame3D::createAA(double array[6]){ return *zArrayToFrame3DAA( array, this ); }
inline zFrame3D &zFrame3D::createAA(zVec6D &v){ return *zVec6DToFrame3DAA( &v, this ); }
inline zFrame3D &zFrame3D::createDH(double a, double alpha, double d, double theta){ return *zFrame3DFromDH( this, a, alpha, d, theta ); }
inline zFrame3D &zFrame3D::copy(zFrame3D &src){ zFrame3DCopy( &src, this ); return *this; }
inline zFrame3D &zFrame3D::ident(){ zFrame3DIdent( this ); return *this; }
inline bool zFrame3D::isEqual(zFrame3D &f){ return _zFrame3DEqual( this, &f ); }
inline bool zFrame3D::isIdent(){ return _zFrame3DIsIdent( this ); }
inline zVec3D zFrame3D::xform(zVec3D &v){ zVec3D ret; _zXform3D( this, &v, &ret ); return ret; }
inline zVec3D zFrame3D::xformInv(zVec3D &v){ zVec3D ret; _zXform3DInv( this, &v, &ret ); return ret; }
inline zVec3D &zFrame3D::xformDRC(zVec3D &v){ zXform3DDRC( this, &v ); return v; }
inline zVec3D &zFrame3D::xformInvDRC(zVec3D &v){ zXform3DInvDRC( this, &v ); return v; }
inline zFrame3D zFrame3D::inv(){ zFrame3D ret; _zFrame3DInv( this, &ret ); return ret; }
inline zVec6D zFrame3D::xformLin(zVec6D &v){ zVec6D ret; zXform6DLin( this, &v, &ret ); return ret; }
inline zVec6D zFrame3D::xformAng(zVec6D &v){ zVec6D ret; zXform6DAng( this, &v, &ret ); return ret; }
inline zFrame3D zFrame3D::twist(zVec6D &t){ zFrame3D ret; zFrame3DTwist( this, &t, &ret ); return ret; }
inline zVec6D zFrame3D::toZYX(){ zVec6D v; zFrame3DToVec6DZYX( this, &v ); return v; }
inline zVec6D zFrame3D::toZYZ(){ zVec6D v; zFrame3DToVec6DZYZ( this, &v ); return v; }
inline zVec6D zFrame3D::toAA(){ zVec6D v; zFrame3DToVec6DAA( this, &v ); return v; }
#endif /* __cplusplus */

__END_DECLS

#endif /* __ZEO_FRAME3D_H__ */
