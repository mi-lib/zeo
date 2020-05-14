/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat2d.h - 2x2 matrix.
 */

#ifndef __ZEO_MAT2D_H__
#define __ZEO_MAT2D_H__

#include <zeo/zeo_vec2d.h>

__BEGIN_DECLS

/*! \struct zMat2D
 * \brief 2x2 matrix.
 */
/* The order of matrix components are:
 * |~ e[0][0]  e[1][0] ~| = |~ c.xx  c.yx ~| = [ v[0] v[1] ] = [ b.x b.y ]
 * |_ e[0][1]  e[1][1] _|   |_ c.xy  c.yy _|
 */
typedef union{
  double e[2][2]; /*!< 2x2 matrix */
  zVec2D v[2];    /*!< 2 column vectors */
  struct{
    zVec2D x, y;
  } b;            /*!< x-y base vectors */
  struct{
    double xx, xy, yx, yy;
  } c;            /*!< 4 components */
} zMat2D;

/*! \brief create, copy and zero a 2x2 matrix.
 *
 * zMat2DCreate() creates a 2-by-2 matrix from \a a11, \a a12, \a a13 and
 * \a a14 as follows.
 *  | \a a11 \a a12 |
 *  | \a a21 \a a22 |
 *
 * zMat2DCopy() copies a 2x2 matrix \a src to \a dest.
 *
 * zMat2DZero() sets all components of a 2x2 matrix \a m for zeros.
 *
 * zMat2DIdent() makes a 2x2 matrix \a m the identity matrix.
 * \return
 * zMat2DCreate(), zMat2DCopy(), zMat2DZero() and zMat2DIdent() return
 * a pointer \a m.
 */
#define _zMat2DCreate(m,a11,a12,a21,a22) do{\
  _zVec2DCreate( &(m)->b.x, a11, a21 );\
  _zVec2DCreate( &(m)->b.y, a12, a22 );\
} while(0)
__EXPORT zMat2D *zMat2DCreate(zMat2D *m, double a11, double a12, double a21, double a22);
#define _zMat2DCopy(s,d) zCopy( zMat2D, s, d )
__EXPORT zMat2D *zMat2DCopy(zMat2D *src, zMat2D *dest);
#define zMat2DZero(m)  zMat2DCreate( (m), 0, 0, 0, 0 )
#define zMat2DIdent(m) zMat2DCreate( (m), 1, 0, 0, 1 )

/*! \brief abstract row/column vector of a 2x2 matrix.
 *
 * zMat2DRow() abstracts the row vectors of a 2x2 matrix \a m. The first
 * and second rows of \a m are abstracted to \a r1 and \a r2, respectively.
 * When \a r1 or \a r2 is the null pointer, it is ignored.
 *
 * zMat2DCol() abstracts the column vectors of a 2x2 matrix \a m. The first
 * and second columns of \a m are abstracted to \a c1 and \a c2, respectively.
 * When \a c1 or \a c2 is the null pointer, it is ignored.
 * \return
 * zMat2DRow() and zMat2DCol() return no value.
 */
__EXPORT void zMat2DRow(zMat2D *m, zVec2D *r1, zVec2D *r2);
__EXPORT void zMat2DCol(zMat2D *m, zVec2D *c1, zVec2D *c2);

/*! \brief transpose a 2x2 matrix.
 *
 * zMat2DT() transposes a 2x2 matrix \a m and puts it into \a tm.
 * \retval \a tm
 */
#define _zMat2DT(m,tm) zMat2DRow( m, &(tm)->b.x, &(tm)->b.y )
__EXPORT zMat2D *zMat2DT(zMat2D *m, zMat2D *tm);

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/*! \brief the four rules of the arithmetics for 2x2 matrix.
 *
 * zMat2DAdd() adds the two 2x2 matrices, \a m1 and \a m2.
 * The result is put into \a m.
 *
 * zMat2DSub() subtracts \a m2 from \a m1.
 * The result is put into \a m.
 *
 * zMat2DRev() reverses \a m. The result is put into \a rm.
 *
 * zMat2DMul() multiplies \a m by a scalar value \a k.
 * The result is put into \a mm.
 *
 * zMat2DDiv() divides \a m by \a k.
 * The result is put into \a dm.
 *
 * zMat2DCat() concatenates multiplied \a m2 by \a k to \a m1.
 * The result is put into \a m.
 *
 * zMat2DAddDRC() adds \a m2 directly to \a m1.
 *
 * zMat2DSubDRC() subtracts \a m2 directly from \a m1.
 *
 * zMat2DRevDRC() reverses \a m directly.
 *
 * zMat2DMulDRC() multiplies \a m directly by \a k.
 *
 * zMat2DDivDRC() divides \a m directly by \a k.
 *
 * zMat2DCatDRC() concatenates multiplied \a m2 by \a k
 * directly to \a m1.
 * \return
 * Each function returns a pointer the result matrix.
 *
 * zMat2DDiv() and zMat2DDivDRC() return the null pointer if
 * \a k is zero.
 */
#define _zMat2DAdd(m1,m2,m) \
  _zMat2DCreate( m, (m1)->c.xx+(m2)->c.xx, (m1)->c.yx+(m2)->c.yx, (m1)->c.xy+(m2)->c.xy, (m1)->c.yy+(m2)->c.yy )
#define _zMat2DSub(m1,m2,m) \
  _zMat2DCreate( m, (m1)->c.xx-(m2)->c.xx, (m1)->c.yx-(m2)->c.yx, (m1)->c.xy-(m2)->c.xy, (m1)->c.yy-(m2)->c.yy )
#define _zMat2DRev(m,rm) \
  _zMat2DCreate( rm, -(m)->c.xx, -(m)->c.yx, -(m)->c.xy, -(m)->c.yy )
#define _zMat2DMul(m,k,mm) \
  _zMat2DCreate( mm, (k)*(m)->c.xx, (k)*(m)->c.yx, (k)*(m)->c.xy, (k)*(m)->c.yy )
#define _zMat2DCat(m1,k,m2,m) \
  _zMat2DCreate( m, (m1)->c.xx+(k)*(m2)->c.xx, (m1)->c.yx+(k)*(m2)->c.yx, (m1)->c.xy+(k)*(m2)->c.xy, (m1)->c.yy+(k)*(m2)->c.yy )

__EXPORT zMat2D *zMat2DAdd(zMat2D *m1, zMat2D *m2, zMat2D *m);
__EXPORT zMat2D *zMat2DSub(zMat2D *m1, zMat2D *m2, zMat2D *m);
__EXPORT zMat2D *zMat2DRev(zMat2D *m, zMat2D *rm);
__EXPORT zMat2D *zMat2DMul(zMat2D *m, double k, zMat2D *mm);
__EXPORT zMat2D *zMat2DDiv(zMat2D *m, double k, zMat2D *dm);
__EXPORT zMat2D *zMat2DCat(zMat2D *m1, double k, zMat2D *m2, zMat2D *m);

#define zMat2DAddDRC(m1,m2)   zMat2DAdd(m1,m2,m1)
#define zMat2DSubDRC(m1,m2)   zMat2DSub(m1,m2,m1)
#define zMat2DRevDRC(m)       zMat2DRev(m,m)
#define zMat2DMulDRC(m,k)     zMat2DMul(m,k,m)
#define zMat2DDivDRC(m,k)     zMat2DDiv(m,k,m)
#define zMat2DCatDRC(m1,k,m2) zMat2DCat(m1,k,m2,m1)

/*! \brief dyadic product of two 2D vectors.
 *
 * zMat2DDyad() calculates dyadic product of two 2D vectors \a v1
 * and \a v2 and puts it into \a dyad. Namely,
 *   \a dyad = \a v1 \a v2^T.
 * \return
 * zMat2DDyad() returns a pointer \a dyad.
 */
#define _zMat2DDyad(d,v1,v2) \
  _zMat2DCreate( (d), (v1)->c.x*(v2)->c.x, (v1)->c.x*(v2)->c.y, (v1)->c.y*(v2)->c.x, (v1)->c.y*(v2)->c.y )
__EXPORT zMat2D *zMat2DDyad(zMat2D *dyad, zVec2D *v1, zVec2D *v2);

/* ********************************************************** */
/* inverse of a 2x2 matrix
 * ********************************************************** */

/*! \brief determinant of a 2x2 matrix.
 *
 * zMat2DDet() computes the determinant of a 2x2 matrix \a m.
 * \return
 * zMat2DDet() returns the determinant of \a m.
 */
#define _zMat2DDet(m) ( m->c.xx*m->c.yy - m->c.yx*m->c.xy )
__EXPORT double zMat2DDet(zMat2D *m);

/*! \brief inverse of a 2x2 matrix.
 *
 * zMat2DInv() computes the inverse of an arbitrary 2x2 matrix \a m and
 * puts it into \a im.
 * \return
 * zMat2DInv() returns a pointer \a im.
 * \notes
 * \a im has to be different from \a m. If \a tm is equal to \a m,
 * anything might happen.
 */
__EXPORT zMat2D *zMat2DInv(zMat2D *m, zMat2D *im);

/* ********************************************************** */
/* multiplication of a 2D vector by a 2x2 matrix
 * ********************************************************** */

/*! \brief multiply a 2D vector and a 2x2 matrix.
 *
 * zMulMat2DVec2D() multiplies a 2D vector \a v by a 2x2 matrix \a m and
 * puts it into \a mv.
 *
 * zMulMat2DTVec2D() multiplies \a v by the transpose matrix of a 2x2 matrix
 * \a m and puts it into \a mv.
 *
 * zMulInvMat2DVec2D() multiplies \a v by the inverse matrix of a 2x2 matrix
 * \a m and puts it into \a mv.
 * \return
 * Each function returns the pointer to the result.
 */
#define _zMulMat2DVec2D(m,v,mv) \
  _zVec2DCreate( mv, (m)->c.xx*(v)->c.x+(m)->c.yx*(v)->c.y, (m)->c.xy*(v)->c.x+(m)->c.yy*(v)->c.y )
#define _zMulMat2DTVec2D(m,v,mv) \
  _zVec2DCreate( mv, (m)->c.xx*(v)->c.x+(m)->c.xy*(v)->c.y, (m)->c.yx*(v)->c.x+(m)->c.yy*(v)->c.y )

__EXPORT zVec2D *zMulMat2DVec2D(zMat2D *m, zVec2D *v, zVec2D *mv);
__EXPORT zVec2D *zMulMat2DTVec2D(zMat2D *m, zVec2D *v, zVec2D *mv);
__EXPORT zVec2D *zMulInvMat2DVec2D(zMat2D *m, zVec2D *v, zVec2D *mv);

/* ********************************************************** */
/* multiplication of a 2x2 matrix by another 2x2 matrix
 * ********************************************************** */

/*! \brief multiply two 2x2 matrices.
 *
 * zMulMat2DMat2D() multiplies a 2x2 matrix \a m2 by the other \a m1 from
 * the leftside and puts it into \a m.
 *
 * zMulMat2DTMat2D() multiplies a 2x2 matrix \a m2 by the transpose matrix
 * of the other \a m1 from the leftside and puts it into \a m.
 *
 * zMulMat2DMat2DT() multiplies a 2x2 matrix \a m1 by the transpose of the
 * other \a m2 from the rightside and puts it into \a m.
 *
 * zMulInvMat2DMat2D() multiplies a 2x2 matrix \a m2 by the inverse of the
 * other \a m1 from the leftside and puts it into \a m.
 * \return
 * Each function returns the pointer to the result.
 */
#define _zMulMat2DMat2D(m1,m2,m) \
  _zMat2DCreate( m,\
    (m1)->c.xx*(m2)->c.xx+(m1)->c.yx*(m2)->c.xy,\
    (m1)->c.xx*(m2)->c.yx+(m1)->c.yx*(m2)->c.yy,\
    (m1)->c.xy*(m2)->c.xx+(m1)->c.yy*(m2)->c.xy,\
    (m1)->c.xy*(m2)->c.yx+(m1)->c.yy*(m2)->c.yy )
#define _zMulMat2DTMat2D(m1,m2,m) \
  _zMat2DCreate( m,\
    (m1)->c.xx*(m2)->c.xx+(m1)->c.xy*(m2)->c.xy,\
    (m1)->c.xx*(m2)->c.yx+(m1)->c.xy*(m2)->c.yy,\
    (m1)->c.yx*(m2)->c.xx+(m1)->c.yy*(m2)->c.xy,\
    (m1)->c.yx*(m2)->c.yx+(m1)->c.yy*(m2)->c.yy )
#define _zMulMat2DMat2DT(m1,m2,m) \
  _zMat2DCreate( m,\
    (m1)->c.xx*(m2)->c.xx+(m1)->c.yx*(m2)->c.yx,\
    (m1)->c.xx*(m2)->c.xy+(m1)->c.yx*(m2)->c.yy,\
    (m1)->c.xy*(m2)->c.xx+(m1)->c.yy*(m2)->c.yx,\
    (m1)->c.xy*(m2)->c.xy+(m1)->c.yy*(m2)->c.yy )

__EXPORT zMat2D *zMulMat2DMat2D(zMat2D *m1, zMat2D *m2, zMat2D *m);
__EXPORT zMat2D *zMulMat2DTMat2D(zMat2D *m1, zMat2D *m2, zMat2D *m);
__EXPORT zMat2D *zMulMat2DMat2DT(zMat2D *m1, zMat2D *m2, zMat2D *m);
__EXPORT zMat2D *zMulInvMat2DMat2D(zMat2D *m1, zMat2D *m2, zMat2D *m);

/* ********************************************************** */
/* rotation
 * ********************************************************** */

/*! \brief rotate matrix.
 *
 * zMat2DRotSC() rotates a 2x2 matrix \a m not with an angle but with
 * trigonometric values. \a s and \a c are for sine and cosine values
 * of the rotation angle, respectively.
 *
 * zMat2DRot() rotates a 2x2 matrix \a m with an angle \a angle in radian
 * and puts it into \a rm.
 * \return
 * zMat2DRotSC() and zMat2DRot() return a pointer \a rm.
 */
#define _zMat2DRotSC(m,s,c,rm) \
  _zMat2DCreate( rm,\
    (c)*(m)->c.xx-(s)*(m)->c.xy,\
    (c)*(m)->c.yx-(s)*(m)->c.yy,\
    (s)*(m)->c.xx+(c)*(m)->c.xy,\
    (s)*(m)->c.yx+(c)*(m)->c.yy )
__EXPORT zMat2D *zMat2DRotSC(zMat2D *m, double s, double c, zMat2D *rm);
__EXPORT zMat2D *zMat2DRot(zMat2D *m, double angle, zMat2D *rm);

/*! \brief error vector between two attitude matrices.
 *
 * zMat2DError() calculates the error angle from \a m2 to \a m1
 * (note the order).
 * \return
 * zMat2DError() returns the error angle computed.
 */
#define _zMat2DError(m1,m2) \
  atan2( (m2)->c.xx*(m1)->c.xy+(m2)->c.yx*(m1)->c.yy-(m2)->c.xy*(m1)->c.xx-(m2)->c.yy*(m1)->c.yx,\
         (m2)->c.xx*(m1)->c.xx+(m2)->c.yx*(m1)->c.yx+(m2)->c.xy*(m1)->c.xy+(m2)->c.yy*(m1)->c.yy )
__EXPORT double zMat2DError(zMat2D *m1, zMat2D *m2);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief scan and print a 2x2 matrix.
 *
 * zMat2DFScan() scans four values from the current position of
 * a file \a fp and creates a 2x2 matrix \a m from them.
 * zMat2DScan() scans the values from the standard input.
 *
 * zMat2DFPrint() prints a 2x2 matrix \a m to the current position
 * of a file \a fp in the following format:
 *  {
 *    a11, a12
 *    a21, a22
 *  }
 * When the null pointer is given, the following string is printed.
 *  (null 2x2 matrix)
 * zMat2DPrint() prints a 2x2 matrix \a m to the standard output.
 * \return
 * zMat2DFScan() and zMat2DScan() return a pointer \a m.
 *
 * zMat2DFPrint() and zMat2DPrint() return no value.
 */
__EXPORT zMat2D *zMat2DFScan(FILE *fp, zMat2D *m);
#define zMat2DScan(m) zMat2DFScan( stdin, (m) )
__EXPORT void zMat2DFPrint(FILE *fp, zMat2D *m);
#define zMat2DPrint(m) zMat2DFPrint( stdout, (m) )

__END_DECLS

#endif /* __ZEO_MAT2D_H__ */
