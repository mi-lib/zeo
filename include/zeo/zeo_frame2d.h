/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_frame2d - 2D coordinate frame.
 */

#ifndef __ZEO_FRAME2D_H__
#define __ZEO_FRAME2D_H__

#include <zeo/zeo_coord2d.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zFrame2D
 * 2D coordinate frame class
 * ********************************************************** */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zFrame2D ){
  zVec2D pos;
  zMat2D att;
#ifdef __cplusplus
  zVec2D &vec(zAxis axis);
  zVec2D &setPos(zVec2D &_pos){ zVec2DCopy( &_pos, &pos ); return pos; }
  zMat2D &setAtt(zMat2D &_att){ zMat2DCopy( &_att, &att ); return att; }
  zFrame2D &create(zVec2D &p, zMat2D &m);
  zFrame2D &copy(zFrame2D &src);
  zFrame2D &ident();
  bool isEqual(zFrame2D &f);
  bool isIdent();
  zVec2D xform(zVec2D &v);
  zVec2D xformInv(zVec2D &v);
  zVec2D &xformDRC(zVec2D &v);
  zVec2D &xformInvDRC(zVec2D &v);
  zFrame2D inv();
  static const zFrame2D zframe2Dident;
#endif /* __cplusplus */
};

/*! \brief the 2D identity transformation frame. */
#ifdef __cplusplus
#define ZFRAME2DIDENT ( (zFrame2D *)&zFrame2D::zframe2Dident )
#else
__ZEO_EXPORT const zFrame2D zframe2Dident;
#define ZFRAME2DIDENT ( (zFrame2D *)&zframe2Dident )
#endif /* __cplusplus */

#define zFrame2DPos(frame) ( &(frame)->pos )
#define zFrame2DAtt(frame) ( &(frame)->att )

#define zFrame2DVec(frame,axis) zMat2DVec( zFrame2DAtt(frame), axis )

#define zFrame2DSetPos(frame,pos) zVec2DCopy( pos, zFrame2DPos(frame) )
#define zFrame2DSetAtt(frame,att) zMat2DCopy( att, zFrame2DAtt(frame) )

/*! \brief create, copy and initialize 2D coordinate frame.
 *
 * zFrame2DCreate() creates a 2D coordinate frame \a frame from a position vector \a p and an attitude
 * matrix \a m.
 * zFrame2DCreateAngle() creates a 2D coordinate frame \a frame from a position vector and a rotation
 * angle \a angle.
 *
 * zFrame2DCopy() copies a 2D coordinate frame \a src to \a dest.
 *
 * zFrame2DIdent() initializes \a frame as the 2D identity frame, namely, a frame whose position is at
 * the original point (0,0,0) and attitude is identical with the absolute coordinate frame.
 * \return
 * zFrame2DCreate() returns a pointer \a frame.
 *
 * zFrame2DCopy() and zFrame2DIdent() return no value.
 */
__ZEO_EXPORT zFrame2D *zFrame2DCreate(zFrame2D *frame, zVec2D *p, zMat2D *m);
__ZEO_EXPORT zFrame2D *zFrame2DCreateAngle(zFrame2D *frame, zVec2D *p, double angle);
#define zFrame2DCopy(src,dest) ( *(dest) = *(src) )
#define zFrame2DIdent(frame) zFrame2DCopy( ZFRAME2DIDENT, frame )

/*! \brief check if two 2D coordinate frames are equal. */
#define _zFrame2DEqual(frame1,frame2) ( _zVec2DEqual( zFrame2DPos(frame1), zFrame2DPos(frame2) ) && _zMat2DEqual( zFrame2DAtt(frame1), zFrame2DAtt(frame2) ) )
__ZEO_EXPORT bool zFrame2DEqual(zFrame2D *frame1, zFrame2D *frame2);

/*! \brief check if a 2D coordinate frame is the identity frame. */
#define _zFrame2DIsIdent(frame) _zFrame2DEqual( frame, ZFRAME2DIDENT )
__ZEO_EXPORT bool zFrame2DIsIdent(zFrame2D *frame);

/*! \brief transform coordinates of a 2D vector.
 *
 * zXform2D() transforms a 2D vector \a v by a 2D transformation frame \a frame. The result is put
 * into \a tv. Suppose \a frame is for the transformation from a frame S1 to S0, \a tv points where
 * \a v points in S1 with respect to S0.
 *
 * On the same assumption for \a frame, zXform2DInv() transforms a 2D vector \a v in S0 to that with
 * respect to S1 by the inverse transformation of \a frame. The result is put into \a tv.
 *
 * zXform2DDRC() directly transforms the 2D vector \a v by a 2D transformation frame \a frame.
 *
 * zXform2DInvDRC() directly transforms a 2D vector \a v by the inverse of \a frame.
 * \return
 * zXform2D(), zXform2DInv(), zXform2DDRC() and zXform2DInvDRC() return a pointer to the result.
 * \notes
 * zXform2DInv() expects that the attitude matrix of \a frame is an orthonormal matrix.
 */
#define _zXform2D( frame, v, tv ) do{\
  _zMulMat2DVec2D( zFrame2DAtt(frame), v, tv );\
  _zVec2DAddDRC( tv, zFrame2DPos(frame) );\
} while(0)
__ZEO_EXPORT zVec2D *zXform2D(zFrame2D *frame, zVec2D *v, zVec2D *tv);

#define _zXform2DInv( frame, v, tv ) do{\
  _zVec2DSub( v, zFrame2DPos(frame), tv );\
  _zMulMat2DTVec2DDRC( zFrame2DAtt(frame), tv );\
} while(0)
__ZEO_EXPORT zVec2D *zXform2DInv(zFrame2D *frame, zVec2D *v, zVec2D *tv);

#define zXform2DDRC(frame,v)    zXform2D(frame,v,v)
#define zXform2DInvDRC(frame,v) zXform2DInv(frame,v,v)

/*! \brief inverse and cascaded transformations of a 2D coordinate frame.
 *
 * zFrame2DInv() calculates the inverse transformation of a 2D frame \a frame. The result is put into
 * \a invframe.
 *
 * zFrame2DCascade() cascades a 2D frame \a frame1 to the other \a frame2. The result is put into \a frame.
 * Suppose \a frame1 is a transformation from a frame S1 to another S0, and \a frame2 for that from a frame
 * S2 to S1, \a frame is the transformation from S2 to S0.
 *
 * zFrame2DXform() calculates the 2D transformation from a frame \a frame1 to another \a frame2. The result
 * is put into \a frame.
 * Suppose \a frame1 is the transformation from a frame S1 to another S0, and \a frame2 is that from a frame
 * S2 to S0, \a frame is the transformation from S2 to S1, namely, the cascade of the inverse of \a frame1
 * and \a frame2.
 * \return
 * These functions return a pointer to the resulted frame.
 * \notes
 * zFrame2DInv() expects that the attitude matrix of \a frame is an orthonormal matrix. For these three
 * functions, it is not permitted to let any argument point to the same address with the other arguments.
 * When some of them are equal, anything might happen.
 */
#define _zFrame2DInv( frame, invframe ) do{\
  _zMat2DT( zFrame2DAtt(frame), zFrame2DAtt(invframe) );\
  _zMulMat2DVec2D( zFrame2DAtt(invframe), zFrame2DPos(frame), zFrame2DPos(invframe) );\
  zVec2DRevDRC( zFrame2DPos(invframe) );\
} while(0)
__ZEO_EXPORT zFrame2D *zFrame2DInv(zFrame2D *frame, zFrame2D *invframe);

#define _zFrame2DCascade( frame1, frame2, frame ) do{\
  zMulMat2DMat2D( zFrame2DAtt(frame1), zFrame2DAtt(frame2), zFrame2DAtt(frame) );\
  _zXform2D( frame1, zFrame2DPos(frame2), zFrame2DPos(frame) );\
} while(0)
__ZEO_EXPORT zFrame2D *zFrame2DCascade(zFrame2D *frame1, zFrame2D *frame2, zFrame2D *frame);

#define _zFrame2DXform( frame1, frame2, frame ) do{\
  zMulMat2DTMat2D( zFrame2DAtt(frame1), zFrame2DAtt(frame2), zFrame2DAtt(frame) );\
  _zXform2DInv( frame1, zFrame2DPos(frame2), zFrame2DPos(frame) );\
} while(0)
__ZEO_EXPORT zFrame2D *zFrame2DXform(zFrame2D *frame1, zFrame2D *frame2, zFrame2D *frame);

/*! \brief convert a 2D coordinate to a 2D frame. */
__ZEO_EXPORT zFrame2D *zFrame2DFromCoord2D(zFrame2D *frame, zCoord2D *coord);
/*! \brief convert a 2D frame to a 2D coordinate. */
__ZEO_EXPORT zCoord2D *zCoord2DFromFrame2D(zCoord2D *coord, zFrame2D *frame);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief read a 2D coordinate frame from a ZTK format processor. */
__ZEO_EXPORT zFrame2D *zFrame2DFromZTK(zFrame2D *frame, ZTK *ztk);

/*! \brief scan and print a 2D coordinate frame.
 *
 * zFrame2DFScan() scans 6 values from the current position of a file \a fp and creates a 2D coordinate
 * frame from them. The meaning of the sequential values is for
 *  a11, a12, x,
 *  a21, a22, y
 * The left 2x2 values are for the 2D attitude matrix. Namely, { a11, a21 } and { a12, a22 } mean the
 * two base vectors of the frame, while the right 2x1 values are for the position of the original point
 * { x, y }. The result is put into \a frame.
 * zFrame2DScan() scans 6 values from the standard input and creates a 2D coordinate frame \a frame from them.
 *
 * zFrame2DFPrint() prints a 2D coordinate frame \a frame out to the current position of a file \a fp
 * in the following style.
 *  {
 *   a11, a12, x,
 *   a21, a22, y
 *  }
 * When the null pointer is given, it prints the following string:
 *  (null 2D frame)
 * zFrame2DPrint() prints a 2D coordinate frame \a frame out to the standard output.
 * \return
 * zFrame2DFScan(), zFrame2DScan(), zFrame2DDHFScan() and zFrame2DDHScan() return a pointer \a frame.
 *
 * zFrame2DFPrint() and zFrame2DPrint() return no value.
 */
__ZEO_EXPORT zFrame2D *zFrame2DFScan(FILE *fp, zFrame2D *frame);
#define zFrame2DScan(frame) zFrame2DFScan( stdin, (frame) )

__ZEO_EXPORT void zFrame2DFPrint(FILE *fp, zFrame2D *frame);
#define zFrame2DPrint(frame) zFrame2DFPrint( stdout, (frame) )

__END_DECLS

#ifdef __cplusplus
inline zVec2D &zFrame2D::vec(zAxis axis){ return *zFrame2DVec( this, axis ); }
inline zFrame2D &zFrame2D::create(zVec2D &p, zMat2D &m){ return *zFrame2DCreate( this, &p, &m ); }
inline zFrame2D &zFrame2D::copy(zFrame2D &src){ zFrame2DCopy( &src, this ); return *this; }
inline zFrame2D &zFrame2D::ident(){ zFrame2DIdent( this ); return *this; }
inline bool zFrame2D::isEqual(zFrame2D &f){ return _zFrame2DEqual( this, &f ); }
inline bool zFrame2D::isIdent(){ return _zFrame2DIsIdent( this ); }
inline zVec2D zFrame2D::xform(zVec2D &v){ zVec2D ret; _zXform2D( this, &v, &ret ); return ret; }
inline zVec2D zFrame2D::xformInv(zVec2D &v){ zVec2D ret; _zXform2DInv( this, &v, &ret ); return ret; }
inline zVec2D &zFrame2D::xformDRC(zVec2D &v){ zXform2DDRC( this, &v ); return v; }
inline zVec2D &zFrame2D::xformInvDRC(zVec2D &v){ zXform2DInvDRC( this, &v ); return v; }
inline zFrame2D zFrame2D::inv(){ zFrame2D ret; _zFrame2DInv( this, &ret ); return ret; }
#endif /* __cplusplus */

#endif /* __ZEO_FRAME2D_H__ */
