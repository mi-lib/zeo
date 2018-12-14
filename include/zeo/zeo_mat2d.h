/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat2d.h - 2D matrix.
 */

#ifndef __ZEO_MAT2D_H__
#define __ZEO_MAT2D_H__

#include <zeo/zeo_vec2d.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zMat2D
 * 2D matrix class
 * ********************************************************** */

typedef double zMat2D[4];

/*! \brief 2D zero matrix and identity matrix */
extern const zMat2D zmat2Dzero;
extern const zMat2D zmat2Dident;
#define ZMAT2DZERO  ( (zMat2D)zmat2Dzero )
#define ZMAT2DIDENT ( (zMat2D)zmat2Dident )

/*! \brief create, copy and cleanup a 2D matrix.
 *
 * zMat2DCreate() creates a 2-by-2 matrix from \a a11, \a a12, \a a13 and
 * \a a14 as follows.
 *  | \a a11 \a a12 |
 *  | \a a21 \a a22 |
 *
 * zMat2DCopy() copies a 2D matrix \a src to \a dest.
 *
 * zMat2DClear() sets all components of a 2D matrix \a m for zero.
 *
 * zMat2DIdent() makes a 2D matrix \a m the identity matrix.
 * \return
 * zMat2DCreate(), zMat2DCopy(), zMat2DClear() and zMat2DIdent() return
 * a pointer \a m.
 */
__EXPORT double *zMat2DCreate(zMat2D m, double a11, double a12, double a21, double a22);
__EXPORT double *zMat2DCopy(zMat2D src, zMat2D dest);
#define zMat2DClear(m) zMat2DCreate( (m), 0, 0, 0, 0 )
#define zMat2DIdent(m) zMat2DCreate( (m), 1, 0, 0, 1 )

/*! \brief transpose a 2D matrix.
 *
 * zMat2DT() transposes a 2D matrix \a m and puts it into \a tm.
 * \retval \a tm
 */
__EXPORT double *zMat2DT(zMat2D m, zMat2D tm);

/*! \brief abstract row/column vector of a 2D matrix.
 *
 * zMat2DRow() abstracts the row vectors of a 2D matrix \a m. The first
 * and second rows of \a m are abstracted to \a r1 and \a r2, respectively.
 * When 'r1' or 'r2' is the null pointer, it is ignored.
 *
 * zMat2DCol() abstracts the column vectors of a 2D matrix \a m. The first
 * and second columns of \a m are abstracted to \a c1 and \a c2, respectively.
 * When \a c1 or \a c2 is the null pointer, it is ignored.
 * \return
 * zMat2DRow() and zMat2DCol() return no value.
 */
__EXPORT void zMat2DRow(zMat2D m, zVec2D r1, zVec2D r2);
__EXPORT void zMat2DCol(zMat2D m, zVec2D c1, zVec2D c2);

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/*! \brief the four rules of the arithmetics for 2D matrix.
 *
 * 'zMat2DAdd()' adds the two 2D matrices, 'm1' and 'm2'.
 * The result is put into 'm'.
 *
 * 'zMat2DSub()' subtracts 'm2' from 'm1'.
 * The result is put into 'm'.
 *
 * 'zMat2DRev()' reverses 'm'. The result is put into 'rm'.
 *
 * 'zMat2DMul()' multiplies 'm' by a scalar value 'k'.
 * The result is put into 'mm'.
 *
 * 'zMat2DDiv()' divides 'm' by 'k'.
 * The result is put into 'dm'.
 *
 * 'zMat2DCat()' concatenates multiplied 'm2' by 'k' to 'm1'.
 * The result is put into 'm'.
 *
 * 'zMat2DAddDRC()' adds 'm2' directly to the 'm1'.
 *
 * 'zMat2DSubDRC()' subtracts 'm2' directly from 'm1'.
 *
 * 'zMat2DRevDRC()' reverses 'm' directly.
 *
 * 'zMat2DMulDRC()' multiplies 'm' directly by 'k'.
 *
 * 'zMat2DDivDRC()' divides 'm' directly  by 'k'.
 *
 * 'zMat2DCatDRC()' concatenates multiplied 'm2' by 'k'
 * directly to 'm1'.
 * \return
 * Each function returns a pointer the result matrix.
 *
 * zMat2DDiv() and zMat2DDivDRC() return the null pointer if \a k is zero.
 */
__EXPORT double *zMat2DAdd(zMat2D m1, zMat2D m2, zMat2D m);
__EXPORT double *zMat2DSub(zMat2D m1, zMat2D m2, zMat2D m);
__EXPORT double *zMat2DRev(zMat2D m, zMat2D rm);
__EXPORT double *zMat2DMul(zMat2D m, double k, zMat2D mm);
__EXPORT double *zMat2DDiv(zMat2D m, double k, zMat2D dm);
__EXPORT double *zMat2DCat(zMat2D m1, double k, zMat2D m2, zMat2D m);

#define zMat2DAddDRC(m1,m2)   zMat2DAdd(m1,m2,m1)
#define zMat2DSubDRC(m1,m2)   zMat2DSub(m1,m2,m1)
#define zMat2DRevDRC(m)       zMat2DRev(m,m)
#define zMat2DMulDRC(m,k)     zMat2DMul(m,k,m)
#define zMat2DDivDRC(m,k)     zMat2DDiv(m,k,m)
#define zMat2DCatDRC(m1,k,m2) zMat2DCat(m1,k,m2,m1)

/*! \brief dyadic product of two 2D vectors.
 *
 * zMat2DDyad() calculates the dyadic product of two 2D vectors \a v1
 * and \a v2 and puts it into \a dyad. Namely,
 *   \a dyad = \a v1 \a v2^T.
 * \return
 * zMat2DDyad() returns a pointer \a dyad.
 */
__EXPORT double *zMat2DDyad(zVec2D v1, zVec2D v2, zMat2D dyad);

/* ********************************************************** */
/* inverse of a 2x2 matrix
 * ********************************************************** */

/*! \brief determinant of a 2D matrix.
 *
 * zMat2DDet() computes the determinant of a 2D matrix \a m.
 * \return
 * zMat2DDet() returns the determinant of \a m.
 */
__EXPORT double zMat2DDet(zMat2D m);

/*! \brief inverse of a 2D matrix.
 *
 * zMat2DInv() computes the inverse of an arbitrary 2x2 matrix \a m and
 * puts it into \a im.
 * \return
 * zMat2DInv() returns a pointer \a im.
 * \notes
 * \a im has to be different from \a m. If \a tm is equal to \a m,
 * anything might happen.
 */
__EXPORT double *zMat2DInv(zMat2D m, zMat2D im);

/* ********************************************************** */
/* multiplication of a 2D vector by a 2x2 matrix
 * ********************************************************** */

/*! \brief multiply a 2D vector and a 2D matrix.
 *
 * zMulMatVec2D() multiplies a 2D vector \a v by a 2D matrix \a m and
 * puts it into \a mv.
 *
 * zMulMatTVec2D() multiplies \a v by the transpose matrix of a 2D matrix
 * \a m and puts it into \a mv.
 *
 * zMulInvMatVec2D() multiplies \a v by the inverse matrix of a 2D matrix
 * \a m and puts it into \a mv.
 * \return
 * Each function returns the pointer to the result.
 */
__EXPORT double *zMulMatVec2D(zMat2D m, zVec2D v, zVec2D mv);
__EXPORT double *zMulMatTVec2D(zMat2D m, zVec2D v, zVec2D mv);
__EXPORT double *zMulInvMatVec2D(zMat2D m, zVec2D v, zVec2D mv);

/* ********************************************************** */
/* multiplication of a 2x2 matrix by another 2x2 matrix
 * ********************************************************** */

/*! \brief multiply two 2D matrices.
 *
 * zMulMatMat2D() multiplies a 2D matrix \a m2 by the other \a m1 from
 * the leftside and puts it into \a m.
 *
 * zMulMatTMat2D() multiplies a 2D matrix \a m2 by the transpose matrix
 * of the other \a m1 from the leftside and puts it into \a m.
 *
 * zMulMatMatT2D() multiplies a 2D matrix \a m1 by the transpose of the
 * other \a m2 from the rightside and puts it into \a m.
 *
 * zMulMatMatT2D() multiplies a 2D matrix \a m2 by the inverse of the
 * other \a m1 from the leftside and puts it into \a m.
 * \return
 * Each function returns the pointer to the result.
 */
__EXPORT double *zMulMatMat2D(zMat2D m1, zMat2D m2, zMat2D m);
__EXPORT double *zMulMatTMat2D(zMat2D m1, zMat2D m2, zMat2D m);
__EXPORT double *zMulMatMatT2D(zMat2D m1, zMat2D m2, zMat2D m);
__EXPORT double *zMulInvMatMat2D(zMat2D m1, zMat2D m2, zMat2D m);

/* ********************************************************** */
/* rotation
 * ********************************************************** */

/*! \brief rotate matrix.
 *
 * zMat2DRot() rotates a 2D matrix \a m with an angle \a angle in radian
 * and puts it into \a rm.
 *
 * zMat2DRotSC() rotates a 2D matrix \a m not with an angle but with
 * trigonometric values. \a s and \a c are for sine and cosine values
 * of the rotation angle, respectively.
 * \return
 * zMat2DRot() and zMat2DRotSC() return a pointer \a rm.
 */
__EXPORT double *zMat2DRot(zMat2D m, double angle, zMat2D rm);
__EXPORT double *zMat2DRotSC(zMat2D m, double s, double c, zMat2D rm);

/*! \brief error vector between two attitude matrices.
 *
 * zMat2DError() calculates the error angle from \a m2 to \a m1
 * (note the order).
 * \return
 * zMat2DError() returns the error angle computed.
 */
__EXPORT double zMat2DError(zMat2D m1, zMat2D m2);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief input and output of a 2D matrix.
 *
 * zMat2DFRead() reads four values from the current position of the file
 * \a fp and creates a 2D matrix \a m from them.
 * zMat2DRead() reads the values from the standard input.
 *
 * zMat2DFWrite() outputs a 2D matrix \a m to the current position of
 * the file \a fp in the following format:
 *  {
 *    a11, a12
 *    a21, a22
 *  }
 * When the null pointer is given, it outputs the following string.
 *  (null 2D matrix)
 * zMat2DWrite() outputs a 2D matrix \a m to the standard output.
 * \return
 * zMat2DFRead() and zMat2DRead() return a pointer \a m.
 *
 * zMat2DFWrite() and zMat2DWrite() return no value.
 */
__EXPORT double *zMat2DFRead(FILE *fp, zMat2D m);
#define zMat2DRead(m) zMat2DFRead( stdin, (m) )
__EXPORT void zMat2DFWrite(FILE *fp, zMat2D m);
#define zMat2DWrite(m) zMat2DFWrite( stdout, (m) )

__END_DECLS

#endif /* __ZEO_MAT2D_H__ */
