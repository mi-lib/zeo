/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_frame3d - 3D coordinate frame.
 */

#ifndef __ZEO_FRAME3D_H__
#define __ZEO_FRAME3D_H__

#include <zeo/zeo_mat3d.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zFrame3D
 * 3D coordinate frame class
 * ********************************************************** */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zFrame3D ){
  zVec3D pos;
  zMat3D att;
#ifdef __cplusplus
  zVec3D &vec(zAxis axis);
  zVec3D &setPos(zVec3D &_pos){ zVec3DCopy( &_pos, &pos ); return pos; }
  zMat3D &setAtt(zMat3D &_att){ zMat3DCopy( &_att, &att ); return att; }
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
  bool match(zFrame3D &f);
  bool isEqual(zFrame3D &f);
  bool isIdent();
  zVec3D operator*(zVec3D &v);
  zFrame3D operator*(zFrame3D &f);
  zFrame3D &operator*=(zFrame3D &f);
  zVec3D xform(zVec3D &v);
  zVec3D xformInv(zVec3D &v);
  zVec3D &xformDRC(zVec3D &v);
  zVec3D &xformInvDRC(zVec3D &v);
  zFrame3D inv();
  zFrame3D casecade(zFrame3D &f);
  zVec6D xformLin(zVec6D &v);
  zVec6D xformAng(zVec6D &v);
  zFrame3D twist(zVec6D &t);
  zVec6D toZYX();
  zVec6D toZYZ();
  zVec6D toAA();
  static const zFrame3D zframe3Dident;
#endif /* __cplusplus */
};

/*! \brief the 3D identity transformation frame. */
#ifdef __cplusplus
#define ZFRAME3DIDENT ( (zFrame3D *)&zFrame3D::zframe3Dident )
#else
__ZEO_EXPORT const zFrame3D zframe3Dident;
#define ZFRAME3DIDENT ( (zFrame3D *)&zframe3Dident )
#endif /* __cplusplus */

#define zFrame3DPos(frame) ( &(frame)->pos )
#define zFrame3DAtt(frame) ( &(frame)->att )

#define zFrame3DVec(frame,axis) zMat3DVec( zFrame3DAtt(frame), axis )

#define zFrame3DSetPos(frame,pos) zVec3DCopy( pos, zFrame3DPos(frame) )
#define zFrame3DSetAtt(frame,att) zMat3DCopy( att, zFrame3DAtt(frame) )

/*! \brief create, copy and initialize a 3D coordinate frame.
 *
 * zFrame3DCreate() creates a 3D coordinate frame \a frame from a position vector \a p and an attitude
 * matrix \a m.
 *
 * zFrame3DCopy() copies a 3D coordinate frame \a src to \a dest.
 *
 * zFrame3DIdent() initializes \a frame as the 3D identity frame, namely, a frame whose position is at
 * the original point (0,0,0) and attitude is identical with the absolute coordinate frame.
 * \return
 * zFrame3DCreate() returns a pointer \a frame.
 *
 * zFrame3DCopy() and zFrame3DIdent() return no value.
 */
__ZEO_EXPORT zFrame3D *zFrame3DCreate(zFrame3D *frame, zVec3D *p, zMat3D *m);
#define zFrame3DCopy(src,dest) ( *(dest) = *(src) )
#define zFrame3DIdent(frame) zFrame3DCopy( ZFRAME3DIDENT, frame )

/*! \brief check if two 3D coordinate frames match. */
#define _zFrame3DMatch(frame1,frame2) ( _zVec3DMatch( zFrame3DPos(frame1), zFrame3DPos(frame2) ) && _zMat3DMatch( zFrame3DAtt(frame1), zFrame3DAtt(frame2) ) )
__ZEO_EXPORT bool zFrame3DMatch(const zFrame3D *frame1, const zFrame3D *frame2);
/*! \brief check if two 3D coordinate frames are equal. */
#define _zFrame3DEqual(frame1,frame2) ( _zVec3DEqual( zFrame3DPos(frame1), zFrame3DPos(frame2) ) && _zMat3DEqual( zFrame3DAtt(frame1), zFrame3DAtt(frame2) ) )
__ZEO_EXPORT bool zFrame3DEqual(const zFrame3D *frame1, const zFrame3D *frame2);

/*! \brief check if a 3D coordinate frame is the identity frame. */
#define _zFrame3DIsIdent(frame) _zFrame3DEqual( frame, ZFRAME3DIDENT )
__ZEO_EXPORT bool zFrame3DIsIdent(const zFrame3D *frame);

/*! \brief transform coordinates of a 3D vector.
 *
 * zXform3D() transforms a 3D vector \a v by a 3D transformation frame \a frame. The result is put
 * into \a tv. Suppose \a frame is for the transformation from a frame S1 to S0, \a tv points where
 * \a v points in S1 with respect to S0.
 *
 * On the same assumption for \a frame, zXform3DInv() transforms a 3D vector \a v in S0 to that with
 * respect to S1 by the inverse transformation of \a frame. The result is put into \a tv.
 *
 * zXform3DDRC() directly transforms the 3D vector \a v by a 3D transformation frame \a frame.
 *
 * zXform3DInvDRC() directly transforms a 3D vector \a v by the inverse of \a frame.
 * \return
 * zXform3D(), zXform3DInv(), zXform3DDRC() and zXform3DInvDRC() return a pointer to the result.
 * \notes
 * zXform3DInv() expects that the attitude matrix of \a frame is an orthonormal matrix.
 */
#define _zXform3D( frame, v, tv ) do{\
  _zMulMat3DVec3D( zFrame3DAtt(frame), v, tv );\
  _zVec3DAddDRC( tv, zFrame3DPos(frame) );\
} while(0)
__ZEO_EXPORT zVec3D *zXform3D(const zFrame3D *frame, const zVec3D *v, zVec3D *tv);

#define _zXform3DInv( frame, v, tv ) do{\
  _zVec3DSub( v, zFrame3DPos(frame), tv );\
  _zMulMat3DTVec3DDRC( zFrame3DAtt(frame), tv );\
} while(0)
__ZEO_EXPORT zVec3D *zXform3DInv(const zFrame3D *frame, const zVec3D *v, zVec3D *tv);

#define zXform3DDRC(frame,v)    zXform3D(frame,v,v)
#define zXform3DInvDRC(frame,v) zXform3DInv(frame,v,v)

/*! \brief inverse and cascaded transformations of a 3D coordinate frame.
 *
 * zFrame3DInv() calculates the inverse transformation of a 3D frame \a frame. The result is put into
 * \a invframe.
 *
 * zFrame3DCascade() cascades a 3D frame \a frame1 to the other \a frame2. The result is put into \a frame.
 * Suppose \a frame1 is a transformation from a frame S1 to another S0, and \a frame2 for that from a frame
 * S2 to S1, \a frame is the transformation from S2 to S0.
 *
 * zFrame3DXform() calculates the 3D transformation from a frame \a frame1 to another \a frame2. The result
 * is put into \a frame.
 * Suppose \a frame1 is the transformation from a frame S1 to another S0, and \a frame2 is that from a frame
 * S2 to S0, \a frame is the transformation from S2 to S1, namely, the cascade of the inverse of \a frame1
 * and \a frame2.
 * \return
 * These functions return a pointer to the resulted frame.
 * \notes
 * zFrame3DInv() expects that the attitude matrix of \a frame is an orthonormal matrix. For these three
 * functions, it is not permitted to let any argument point to the same address with the other arguments.
 * When some of them are equal, anything might happen.
 */
#define _zFrame3DInv( frame, invframe ) do{\
  _zMat3DT( zFrame3DAtt(frame), zFrame3DAtt(invframe) );\
  _zMulMat3DVec3D( zFrame3DAtt(invframe), zFrame3DPos(frame), zFrame3DPos(invframe) );\
  zVec3DRevDRC( zFrame3DPos(invframe) );\
} while(0)
__ZEO_EXPORT zFrame3D *zFrame3DInv(const zFrame3D *frame, zFrame3D *invframe);

#define _zFrame3DCascade( frame1, frame2, frame ) do{\
  zMulMat3DMat3D( zFrame3DAtt(frame1), zFrame3DAtt(frame2), zFrame3DAtt(frame) );\
  _zXform3D( frame1, zFrame3DPos(frame2), zFrame3DPos(frame) );\
} while(0)
__ZEO_EXPORT zFrame3D *zFrame3DCascade(const zFrame3D *frame1, const zFrame3D *frame2, zFrame3D *frame);

#define _zFrame3DXform( frame1, frame2, frame ) do{\
  zMulMat3DTMat3D( zFrame3DAtt(frame1), zFrame3DAtt(frame2), zFrame3DAtt(frame) );\
  _zXform3DInv( frame1, zFrame3DPos(frame2), zFrame3DPos(frame) );\
} while(0)
__ZEO_EXPORT zFrame3D *zFrame3DXform(const zFrame3D *frame1, const zFrame3D *frame2, zFrame3D *frame);

/*! \brief transform a 6D vector.
 */
__ZEO_EXPORT zVec6D *zXform6DLin(const zFrame3D *frame, const zVec6D *v, zVec6D *vc);
__ZEO_EXPORT zVec6D *zXform6DAng(const zFrame3D *frame, const zVec6D *v, zVec6D *vc);

/*! \brief twist a frame by a torsion vector (position offset & angle-axis rotation).
 */
__ZEO_EXPORT zFrame3D *zFrame3DTwist(const zFrame3D *frame1, const zVec6D *twist, zFrame3D *frame2);

/*! \brief error between two 3D coordinate frames.
 *
 * zFrame3DError() calculates the error vector of a 3D coordinate frame \a frame1 from another \a frame2
 * (note the order). The result is put into \a error. Intuitively, \a error equals to \a frame1 minus \a frame2.
 * \retval \a error
 * \sa zMat3DError
 */
__ZEO_EXPORT zVec6D *zFrame3DError(const zFrame3D *frame1, const zFrame3D *frame2, zVec6D *error);

/*! \brief create a frame from a handy expression.
 *
 * zFrame3DFromZYX() creates a 3D coordinate frame whose original point is at ( \a x, \a y, \a z ) and
 * attitude is expressed by z-y-x Eulerian angle.
 *
 * zFrame3DFromZYZ() creates a 3D coordinate frame whose original point is at ( \a x, \a y, \a z ) and
 * attitude is expressed by z-y-z Eulerian angle.
 *
 * zFrame3DFromAA() creates a 3D coordinate frame whose original point is at ( \a x, \a y, \a z ) and
 * attitude is expressed by an angle-axis vector.
 *
 * zFrame3DFromDH() creates a 3D coordinate frame from modified Denaviet-Hartenberg (DH) parameters
 * (\a a, \a alpha, \a d, \a theta).
 *
 * All these functions put the result into \a frame.
 * \return
 * All these functions return a pointer \a frame.
 * \sa
 * zMat3DFromZYX, zMat3DFromZYZ, zMat3DFromAA
 */
__ZEO_EXPORT zFrame3D *zFrame3DFromZYX(zFrame3D *frame, double x, double y, double z, double azim, double elev, double tilt);
__ZEO_EXPORT zFrame3D *zFrame3DFromZYZ(zFrame3D *frame, double x, double y, double z, double heading, double pitch, double bank);
__ZEO_EXPORT zFrame3D *zFrame3DFromAA(zFrame3D *frame, double x, double y, double z, double xa, double ya, double za);
__ZEO_EXPORT zFrame3D *zFrame3DFromDH(zFrame3D *frame, double a, double alpha, double d, double theta);

/*! \brief translate a 3D view frame with respect to the current view.
 *
 * zFrame3DTranslateView() translates a 3D view frame \a frame at (\a x, \a y, \a z) with respect to
 * the current view, i.e., \a frame itself.
 * \return
 * zFrame3DTranslateView() always returns the pointer \a frame.
 */
__ZEO_EXPORT zFrame3D *zFrame3DTranslateView(zFrame3D *frame, double x, double y, double z);

/*! \brief rotate a 3D view frame with respect to the current view.
 *
 * zFrame3DRotateView() rotates a 3D view frame \a frame at an angle \a angle in radian about an axis pointed
 * by (\a x, \a y, \a z) with respect to the current view, i.e., \a frame itself.
 * \return
 * zFrame3DRotateView() always returns the pointer \a frame.
 */
__ZEO_EXPORT zFrame3D *zFrame3DRotateView(zFrame3D *frame, double angle, double x, double y, double z);

/*! \brief locate a 3D view frame as to view a 3D point from a another specified point.
 *
 * zFrame3DLookAtView() locates a 3D view frame \a frame as to look at a specified 3D point
 * (\a centerx, \a centery, \a centerz) from another point (\a eyex, \a eyey, \a eyez), where a 3D vector
 * (\a upx, \a upy, \a upz) points the upper direction for the viewer. As the result, the x-axis of the
 * updated \a frame is in parallel to (\a centerx - \a eyex, \a centery - \a eyey, \a centerz - \a eyez),
 * the z-axis of \a frame is on a plane spanned by the x-axis and (\a upx, \a upy, \a upz) and perpendicular
 * to the x-axis, and the y-axis is determined from the z- and x-axes based on the right-hand coordinate rule.
 * \return
 * zFrame3DLookAtView() always returns the pointer \a frame.
 */
__ZEO_EXPORT zFrame3D *zFrame3DLookAtView(zFrame3D *frame, double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz);

/*! \brief rotate a 3D view frame as to keep gazing a 3D point at a distance.
 *
 * zFrame3DGazeAndRotateView() rotates a 3D view frame \a frame as to look at a specified 3D point
 * (\a centerx, \a centery, \a centerz) at a distance \a distance. The view direction is determined from
 * \a pan and \a tilt angles in radian, where \a roll is the roll angle.
 * \return
 * zFrame3DGazeAndRotateView() always returns the pointer \a frame.
 */
__ZEO_EXPORT zFrame3D *zFrame3DGazeAndRotateView(zFrame3D *frame, double centerx, double centery, double centerz, double distance, double pan, double tilt, double roll);

/*! \brief convert a 3D coordinate frame to an array or a 6D vector.
 *
 * zArrayToFrame3DZYX() converts an array of double-precision floating point values with
 * [ x, y, z, azimuth, elevation, tilt ] pointed by \a array to a 3D coordinate frame. The contents of
 * \a array is for the position of the original point and z-y-x Eulerian angle.
 * The result is put into \a frame.
 *
 * zFrame3DZYXToArray() converts a 3D coordinate frame \a frame to an array of double-precision floating
 * point values [ x, y, z, azimuth, elevation, tilt ] of which the first three values are for the position
 * of the original point and the last three for z-y-x Eulerian angle. The result is put into a buffer
 * pointed by \a array.
 *
 * zVec6DToFrame3DZYX() converts a 6D vector \a v to a 3D coordinate frame. \a v is a quasi vector for
 * [ x, y, z, azimuth, elevation, tilt ], which is for the position of the original point and z-y-x
 * Eulerian angle. The result is put into \a frame.
 *
 * zFrame3DZYXToVec6D() converts a 3D coordinate frame \a frame to a 6D quasi vector which is for
 * [ x, y, z, azimuth, elevation, tilt ], of which the first three values are for the position of the
 * original point and the last three are for z-y-x Eulerian angle. The result is put into \a v.
 *
 * zArrayToFrame3DZYZ() converts an array of double-precision floating point values with
 * [ x, y, z, heading, pitch, bank ] pointed by \a array to a 3D coordinate frame. The components of
 * \a array are for the position of the original point and z-y-z Eulerian angle. The result is put into
 * \a frame.
 *
 * zFrame3DZYZToArray() converts a 3D coordinate frame \a frame to an array of double-precision floating
 * point values [ x, y, z, heading, pitch, bank ], of which the first three values are for the position
 * of the original point and the last three are for z-y-z Eulerian angle. The result is put into a buffer
 * pointed by \a array.
 *
 * zVec6DToFrame3DZYZ() converts a 6D vector \a v to a 3D coordinate frame \a frame. \a v is a quasi vector
 * for [ x, y, z, heading, pitch, bank ], which is for the position of the original point and z-y-z
 * Eulerian angle. The result is put into \a frame.
 *
 * zFrame3DZYZToVec6D() converts a 3D coordinate frame \a frame to a 6D quasi vector \a v, which is for
 * [ x, y, z, heading, pitch, bank ], of which the first three values are for the position of the
 * original point and the last three are for z-y-z Eulerian angle. The result is put into \a v.
 * \return
 * zArrayToFrame3DZYX(), zVec6DToFrame3DZYX(), zArrayToFrame3DZYZ() and zVec6DToFrame3DZYZ() return
 * a pointer \a frame.
 *
 * zFrame3DZYXToArray() and zFrame3DZYZToArray() return a pointer \a array.
 *
 * zFrame3DZYXToVec6D() and zFrame3DZYZToVec6D() return a pointer \a v.
 * \notes
 * For zFrame3DZYXToArray() and zFrame3DZYZToArray(), the buffer pointed by \a array must have enough
 * size more than or equal to six. If not, anything may happen.
 */
__ZEO_EXPORT zFrame3D *zArrayToFrame3DZYX(const double *array, zFrame3D *frame);
__ZEO_EXPORT double *zFrame3DToArrayZYX(const zFrame3D *frame, double *array);
__ZEO_EXPORT zFrame3D *zVec6DToFrame3DZYX(const zVec6D *v, zFrame3D *frame);
__ZEO_EXPORT zVec6D *zFrame3DToVec6DZYX(const zFrame3D *frame, zVec6D *v);

__ZEO_EXPORT zFrame3D *zArrayToFrame3DZYZ(const double *array, zFrame3D *frame);
__ZEO_EXPORT double *zFrame3DToArrayZYZ(const zFrame3D *frame, double *array);
__ZEO_EXPORT zFrame3D *zVec6DToFrame3DZYZ(const zVec6D *v, zFrame3D *frame);
__ZEO_EXPORT zVec6D *zFrame3DToVec6DZYZ(const zFrame3D *frame, zVec6D *v);

__ZEO_EXPORT zFrame3D *zArrayToFrame3DAA(const double *array, zFrame3D *frame);
__ZEO_EXPORT double *zFrame3DToArrayAA(const zFrame3D *frame, double *array);
__ZEO_EXPORT zFrame3D *zVec6DToFrame3DAA(const zVec6D *v, zFrame3D *frame);
__ZEO_EXPORT zVec6D *zFrame3DToVec6DAA(const zFrame3D *frame, zVec6D *v);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief read a 3D coordinate frame from a ZTK format processor. */
__ZEO_EXPORT zFrame3D *zFrame3DFromZTK(zFrame3D *frame, ZTK *ztk);
/*! \brief add a 3D frame to a ZTK format processor. */
__ZEO_EXPORT ZTK *zFrame3DToZTK(const zFrame3D *frame, ZTK *ztk);
/*! \brief read DH parameters for a 3D coordinate frame from a ZTK format processor. */
__ZEO_EXPORT zFrame3D *zFrame3DDHFromZTK(zFrame3D *frame, ZTK *ztk);

/*! \brief scan and print a 3D coordinate frame.
 *
 * zFrame3DFScan() scans 12 values from the current position of a file \a fp and creates a 3D coordinate
 * frame from them. The meaning of the sequential values is for
 *  a11, a12, a13, x,
 *  a21, a22, a23, y,
 *  a31, a32, a33, z
 * The left 3x3 values are for the 3D attitude matrix. Namely, { a11, a21, a31 }, { a12, a22, a32 } and
 * { a13, a23, a33 } mean the three base vectors of the frame, while the right 3x1 values are for
 * the position of the original point { x, y, z }. The result is put into \a frame.
 * zFrame3DScan() scans 12 values from the standard input and creates a 3D coordinate frame \a frame from them.
 *
 * zFrame3DFPrint() prints a 3D coordinate frame \a frame out to the current position of a file \a fp
 * in the following style.
 *  {
 *   a11, a12, a13, x,
 *   a21, a22, a23, y,
 *   a31, a32, a33, z
 *  }
 * When the null pointer is given, it prints the following string:
 *  (null 3D frame)
 * zFrame3DPrint() prints a 3D coordinate frame \a frame out to the standard output.
 * \return
 * zFrame3DFScan(), zFrame3DScan(), zFrame3DDHFScan() and zFrame3DDHScan() return a pointer \a frame.
 *
 * zFrame3DFPrint() and zFrame3DPrint() return no value.
 */
__ZEO_EXPORT zFrame3D *zFrame3DFScan(FILE *fp, zFrame3D *frame);
#define zFrame3DScan(frame) zFrame3DFScan( stdin, (frame) )

__ZEO_EXPORT void zFrame3DFPrint(FILE *fp, const zFrame3D *frame);
#define zFrame3DPrint(frame) zFrame3DFPrint( stdout, (frame) )

__END_DECLS

#ifdef __cplusplus
inline zVec3D &zFrame3D::vec(zAxis axis){ return *zFrame3DVec( this, axis ); }
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
inline bool zFrame3D::match(zFrame3D &f){ return _zFrame3DMatch( this, &f ); }
inline bool zFrame3D::isEqual(zFrame3D &f){ return _zFrame3DEqual( this, &f ); }
inline bool zFrame3D::isIdent(){ return _zFrame3DIsIdent( this ); }
inline zVec3D zFrame3D::operator*(zVec3D &v){ zVec3D ret; zXform3D( this, &v, &ret ); return ret; }
inline zFrame3D zFrame3D::operator*(zFrame3D &f){ zFrame3D ret; zFrame3DCascade( this, &f, &ret ); return ret; }
inline zFrame3D &zFrame3D::operator*=(zFrame3D &f){ zFrame3D tmp; zFrame3DCascade( this, &f, &tmp ); zFrame3DCopy( &tmp, this ); return *this; }
inline zVec3D zFrame3D::xform(zVec3D &v){ zVec3D ret; _zXform3D( this, &v, &ret ); return ret; }
inline zVec3D zFrame3D::xformInv(zVec3D &v){ zVec3D ret; _zXform3DInv( this, &v, &ret ); return ret; }
inline zVec3D &zFrame3D::xformDRC(zVec3D &v){ zXform3DDRC( this, &v ); return v; }
inline zVec3D &zFrame3D::xformInvDRC(zVec3D &v){ zXform3DInvDRC( this, &v ); return v; }
inline zFrame3D zFrame3D::inv(){ zFrame3D ret; _zFrame3DInv( this, &ret ); return ret; }
inline zFrame3D zFrame3D::casecade(zFrame3D &f){ zFrame3D ret; zFrame3DCascade( this, &f, &ret ); return ret; }
inline zVec6D zFrame3D::xformLin(zVec6D &v){ zVec6D ret; zXform6DLin( this, &v, &ret ); return ret; }
inline zVec6D zFrame3D::xformAng(zVec6D &v){ zVec6D ret; zXform6DAng( this, &v, &ret ); return ret; }
inline zFrame3D zFrame3D::twist(zVec6D &t){ zFrame3D ret; zFrame3DTwist( this, &t, &ret ); return ret; }
inline zVec6D zFrame3D::toZYX(){ zVec6D v; zFrame3DToVec6DZYX( this, &v ); return v; }
inline zVec6D zFrame3D::toZYZ(){ zVec6D v; zFrame3DToVec6DZYZ( this, &v ); return v; }
inline zVec6D zFrame3D::toAA(){ zVec6D v; zFrame3DToVec6DAA( this, &v ); return v; }
#endif /* __cplusplus */

#endif /* __ZEO_FRAME3D_H__ */
