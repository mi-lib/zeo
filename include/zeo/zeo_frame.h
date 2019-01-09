/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_frame - 3D frame.
 */

#ifndef __ZEO_FRAME_H__
#define __ZEO_FRAME_H__

#include <zeo/zeo_mat3d.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zFrame3D
 * 3D frame class
 * ********************************************************** */

typedef struct{
  zVec3D pos;
  zMat3D att;
} zFrame3D;

#define zFrame3DPos(f) ( &(f)->pos )
#define zFrame3DAtt(f) ( &(f)->att )

#define zFrame3DSetPos(f,p) zVec3DCopy( p, zFrame3DPos(f) )
#define zFrame3DSetAtt(f,r) zMat3DCopy( r, zFrame3DAtt(f) )

/*! \brief the identity frame. */
extern const zFrame3D zframe3Dident;
#define ZFRAME3DIDENT ( (zFrame3D *)&zframe3Dident )

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
 * zFrame3DXfer() calculates the transformation frame from the
 * frame \a f1 to the other \a f2. The result is put into \a f.
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
__EXPORT zFrame3D *zFrame3DInv(zFrame3D *f, zFrame3D *fi);
__EXPORT zFrame3D *zFrame3DCascade(zFrame3D *f1, zFrame3D *f2, zFrame3D *f);
__EXPORT zFrame3D *zFrame3DXfer(zFrame3D *f1, zFrame3D *f2, zFrame3D *f);

/*! \brief transform a 3D vector.
 *
 * zXfer3D() transforms a 3D vector \a v by a frame \a f. The
 * result is put into \a tv. Suppose \a f is for the
 * transformation from a frame S1 to S0, \a tv points where \a v
 * points in S1 with respect to S0.
 *
 * On the same assumption for \a f, zXfer3DInv() transforms a
 * 3D vector \a v in S0 to what is with respect to S1 by the
 * inverse frame of \a f. The result is put into \a tv.
 *
 * zXfer3DDRC() directly transforms the 3D vector \a v by a
 * frame \a f.
 *
 * zXfer3DInvDRC() directly transforms a 3D vector \a v by the
 * inverse of \a f.
 * \return
 * zXfer3D(), zXfer3DInv(), zXfer3DDRC() and zXfer3DInvDRC()
 * return a pointer to the result.
 * \notes
 * zXfer3DInv() expects that the attitude matrix of \a f is an
 * orthonormal matrix.
 */
__EXPORT zVec3D *zXfer3D(zFrame3D *f, zVec3D *v, zVec3D *tv);
__EXPORT zVec3D *zXfer3DInv(zFrame3D *f, zVec3D *v, zVec3D *tv);

#define zXfer3DDRC(f,v)    zXfer3D(f,v,v)
#define zXfer3DInvDRC(f,v) zXfer3DInv(f,v,v)

/*! \brief transfer a 6D vector
 */
__EXPORT zVec6D *zXfer6DLin(zFrame3D *f, zVec6D *v, zVec6D *vc);
__EXPORT zVec6D *zXfer6DAng(zFrame3D *f, zVec6D *v, zVec6D *vc);

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

/*! \brief input/output a 3D frame.
 *
 * zFrame3DFRead() reads 12 values from the current position of
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
 * zFrame3DRead() reads 12 values from the standard input and
 * creates a 3D frame \a f from them.
 *
 * zFrame3DDHFRead() reads 4 values from the current position
 * of a file \a fp and creates a 3D frame \a f from them. The
 * values mean DH parameters ( a, alpha, d, theta ).
 * The unit of both \a alpha and \a theta is degree.
 * zFrame3DDHRead() reads 4 values from the standard input and
 * creates a 3D frame \a f from them.
 *
 * zFrame3DFWrite() writes a 3D frame \a f to the current position
 * of a file \a fp in the following style.
 *  {
 *   a11, a12, a13, x,
 *   a21, a22, a23, y
 *   a31, a32, a33, z
 *  }
 * When the null pointer is given, it writes the following string.
 *  (null 3D frame)
 * zFrame3DWrite() writes a 3D frame \a f to the standard output.
 * \return
 * zFrame3DFRead(), zFrame3DRead(), zFrame3DDHFRead() and
 * zFrame3DDHRead() return a pointer \a f.
 *
 * zFrame3DFWrite() and zFrame3DWrite() return no value.
 */
__EXPORT zFrame3D *zFrame3DFRead(FILE *fp, zFrame3D *f);
#define zFrame3DRead(f) zFrame3DFRead( stdin, (f) )
__EXPORT zFrame3D *zFrame3DDHFRead(FILE *fp, zFrame3D *f);
#define zFrame3DDHRead(f) zFrame3DDHFRead( stdin, (f) )
__EXPORT void zFrame3DFWrite(FILE *fp, zFrame3D *f);
#define zFrame3DWrite(f) zFrame3DFWrite( stdout, (f) )

__END_DECLS

#endif /* __ZEO_FRAME_H__ */
