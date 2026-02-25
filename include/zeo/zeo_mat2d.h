/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat2d - 2x2 matrix.
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
ZDEF_UNION( __ZEO_CLASS_EXPORT, zMat2D ){
  double e[2][2]; /*!< 2x2 matrix */
  zVec2D v[2];    /*!< 2 column vectors */
  struct{
    zVec2D x, y;
  } b;            /*!< x-y base vectors */
  struct{
    double xx, xy, yx, yy;
  } c;            /*!< 4 components */
#ifdef __cplusplus
  zMat2D();
  zMat2D(double a11, double a12, double a21, double a22);
  zMat2D(const zVec2D &v1, const zVec2D &v2);
  zMat2D(const zMat2D &mat);
  ~zMat2D(){}
  zVec2D &vec(zAxis a);
  zMat2D &create(double a11, double a12, double a21, double a22);
  zMat2D &create(const zVec2D &v1, const zVec2D &v2);
  zMat2D &copy(zMat2D &m);
  zMat2D &zero();
  zMat2D &ident();
  bool isEqual(zMat2D &m);
  bool isIdent();
  bool isTol(double tol);
  zVec2D &row(int i, zVec2D &v);
  zVec2D &col(int i, zVec2D &v);
  zMat2D &transpose(zMat2D &m);
  zMat2D transpose();
  zMat2D operator-();
  zMat2D &operator+=(zMat2D &m);
  zMat2D &operator-=(zMat2D &m);
  zMat2D &operator*=(double k);
  zMat2D &operator/=(double k);
  zMat2D &createDyad(zVec2D &v1, zVec2D &v2);
  double squareNorm();
  double norm();
  double det();
  zMat2D &inv(zMat2D &m);
  zVec2D mulT(zVec2D &v);
  zVec2D mulInv(zVec2D &v);
  zMat2D mulT(zMat2D &m);
  zMat2D mulInv(zMat2D &m);
  zMat2D &createAngle(double angle);
  /*! \brief 2D zero matrix and identity matrix */
  static const zMat2D zmat2Dzero;
  static const zMat2D zmat2Dident;
#endif /* __cplusplus */
};

/*! \brief a column vector of a 2x2 matrix. */
#define zMat2DVec(mat,axis) ( &(mat)->v[(axis)] )

/*! \brief 2D zero matrix and identity matrix */
#ifdef __cplusplus
#define ZMAT2DZERO  ( (zMat2D *)&zMat2D::zmat2Dzero )
#define ZMAT2DIDENT ( (zMat2D *)&zMat2D::zmat2Dident )
#else
__ZEO_EXPORT const zMat2D zmat2Dzero;
__ZEO_EXPORT const zMat2D zmat2Dident;
#define ZMAT2DZERO  ( (zMat2D *)&zmat2Dzero )
#define ZMAT2DIDENT ( (zMat2D *)&zmat2Dident )
#endif /* __cplusplus */

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
__ZEO_EXPORT zMat2D *zMat2DCreate(zMat2D *m, double a11, double a12, double a21, double a22);
__ZEO_EXPORT zMat2D *zMat2DCreateAngle(zMat2D *m, double angle);
#define _zMat2DCopy(src,dest) zCopy( zMat2D, src, dest )
__ZEO_EXPORT zMat2D *zMat2DCopy(const zMat2D *src, zMat2D *dest);
#define zMat2DZero(m)  zMat2DCopy( ZMAT2DZERO, (m) )
#define zMat2DIdent(m) zMat2DCopy( ZMAT2DIDENT, (m) )

/*! \brief check if two 2x2 matrices are equal.
 *
 * zMat2DMatch() and zMat2DEqual() check if two 2x2 matrices \a m1 and \a m2 are equal.
 * They return a boolean value.
 *
 * zMat2DMatch() strictly compares the two matrices, while zMat2DEqual() checks if the error between
 * \a m1 and \a m2 are sufficiently small.
 * \return
 * zMat2DMatch() and zMat2DEqual() return the true value if \a m1 and \a m2 are equal, or false otherwise.
 */
#define _zMat2DMatch(m1,m2) ( _zVec2DMatch(&(m1)->b.x,&(m2)->b.x) && _zVec2DMatch(&(m1)->b.y,&(m2)->b.y) )
__ZEO_EXPORT bool zMat2DMatch(const zMat2D *m1, const zMat2D *m2);
#define _zMat2DEqual(m1,m2) ( _zVec2DEqual(&(m1)->b.x,&(m2)->b.x) && _zVec2DEqual(&(m1)->b.y,&(m2)->b.y) )
__ZEO_EXPORT bool zMat2DEqual(const zMat2D *m1, const zMat2D *m2);

/*! \brief check if a 2x2 matrix is the identity matrix. */
#define _zMat2DIsIdent(m) _zMat2DEqual( m, ZMAT2DIDENT )
__ZEO_EXPORT bool zMat2DIsIdent(const zMat2D *m);

/*! \brief check if a 2x2 matrix is tiny.
 *
 * zMat2DIsTol() checks if the absolute values of all components of a 2x2 matrix \a m are smaller than \a tol.
 *
 * zMat2DIsTiny() applies zTOL (defined in zm_misc.h) to the tolerance of zMat2DIsTol().
 * \return
 * zMat2DIsTol() and zMat2DIsTiny() return the true value if the absolute values of all components of \a m
 * are smaller than \a tol and zTOL, respectively, or the false value, otherwise.
 * \notes
 * \a tol must be positive.
 * \sa
 * zIsTol, zIsTiny
 */
#define _zMat2DIsTol(m,tol) ( _zVec2DIsTol( &(m)->b.x, tol ) && _zVec2DIsTol( &(m)->b.y, tol ) )
__ZEO_EXPORT bool zMat2DIsTol(const zMat2D *m, double tol);
#define _zMat2DIsTiny(m) _zMat2DIsTol( m, zTOL )
#define zMat2DIsTiny(m)  zMat2DIsTol( m, zTOL )

/*! \brief abstract row/column vectors from a 2x2 matrix.
 *
 * zMat2DRow() abstracts the \a i th row from a 2x2 matrix \a m and puts it into \a v.
 *
 * zMat2DCol() abstracts the \a i th column from a 2x2 matrix \a m and puts it into \a v.
 * \return
 * zMat2DRow() and zMat2DCol() return a pointer \a v.
 */
#define _zMat2DRow(m,i,r) _zVec2DCreate( (r), (m)->e[0][(i)], (m)->e[1][(i)] )
__ZEO_EXPORT zVec2D *zMat2DRow(const zMat2D *m, int i, zVec2D *v);
#define zMat2DCol(m,i,c)  zVec2DCopy( (&(m)->v[(i)]), c )

/*! \brief transpose a 2x2 matrix.
 *
 * zMat2DT() transposes a 2x2 matrix \a m and puts it into \a tm.
 * \retval \a tm
 */
#define _zMat2DT(m,tm) do{\
  _zMat2DRow( m, 0, &(tm)->b.x );\
  _zMat2DRow( m, 1, &(tm)->b.y );\
} while(0)
#define _zMat2DTDRC(m) zSwap( double, (m)->e[0][1], (m)->e[1][0] )
__ZEO_EXPORT zMat2D *zMat2DT(const zMat2D *m, zMat2D *tm);
__ZEO_EXPORT zMat2D *zMat2DTDRC(zMat2D *m);

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

__ZEO_EXPORT zMat2D *zMat2DAdd(const zMat2D *m1, const zMat2D *m2, zMat2D *m);
__ZEO_EXPORT zMat2D *zMat2DSub(const zMat2D *m1, const zMat2D *m2, zMat2D *m);
__ZEO_EXPORT zMat2D *zMat2DRev(const zMat2D *m, zMat2D *rm);
__ZEO_EXPORT zMat2D *zMat2DMul(const zMat2D *m, double k, zMat2D *mm);
__ZEO_EXPORT zMat2D *zMat2DDiv(const zMat2D *m, double k, zMat2D *dm);
__ZEO_EXPORT zMat2D *zMat2DCat(const zMat2D *m1, double k, const zMat2D *m2, zMat2D *m);

#define _zMat2DAddDRC(m1,m2)   _zMat2DAdd(m1,m2,m1)
#define _zMat2DSubDRC(m1,m2)   _zMat2DSub(m1,m2,m1)
#define _zMat2DRevDRC(m)       _zMat2DRev(m,m)
#define _zMat2DMulDRC(m,k)     _zMat2DMul(m,k,m)
#define _zMat2DDivDRC(m,k)     _zMat2DDiv(m,k,m)
#define _zMat2DCatDRC(m1,k,m2) _zMat2DCat(m1,k,m2,m1)

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
__ZEO_EXPORT zMat2D *zMat2DDyad(zMat2D *dyad, const zVec2D *v1, const zVec2D *v2);

/*! \brief calculate norm of a 2x2 matrix.
 *
 * zMat2DNorm() calculates a norm of a 2x2 matrix \a m.
 * zMat2DSqrNorm() calculates a squared norm of \a m.
 * \return
 * zMat2DNorm() returns a norm of \a m.
 * zMat2DSqrNorm() returns a squared norm of \a m.
 */
#define _zMat2DSqrNorm(m) ( _zVec2DSqrNorm(&(m)->b.x) + _zVec2DSqrNorm(&(m)->b.y) )
__ZEO_EXPORT double zMat2DSqrNorm(const zMat2D *m);
#define _zMat2DNorm(m) sqrt( _zMat2DSqrNorm(m) )
#define zMat2DNorm(m) sqrt( zMat2DSqrNorm(m) )

/* ********************************************************** */
/* inverse of a 2x2 matrix
 * ********************************************************** */

/*! \brief determinant of a 2x2 matrix.
 *
 * zMat2DDet() computes the determinant of a 2x2 matrix \a mat.
 * \return
 * zMat2DDet() returns the determinant of \a mat.
 */
#define _zMat2DDet(mat) ( (mat)->c.xx*(mat)->c.yy - (mat)->c.yx*(mat)->c.xy )
__ZEO_EXPORT double zMat2DDet(const zMat2D *mat);

/*! \brief adjugate of a 2x2 matrix.
 *
 * zMat2DAdj() computes the adjugate matrix of a 2x2 matrix \a mat. The rseult is put into \a adj.
 * \return
 * zMat2DAdj() returns a pointer \a adj.
 */
#define _zMat2DAdj(mat,adj) _zMat2DCreate( adj, (mat)->c.yy, -(mat)->c.yx, -(mat)->c.xy, (mat)->c.xx )
__ZEO_EXPORT zMat2D *zMat2DAdj(const zMat2D *mat, zMat2D *adj);

/*! \brief inverse of a 2x2 matrix.
 *
 * zMat2DInv() computes the inverse of a 2x2 matrix \a mat. The result is put into \a invmat.
 * \return
 * zMat2DInv() returns a pointer \a invmat.
 * \notes
 * \a invmat has to be different from \a mat. If \a invmat is identical with \a mat, anything might happen.
 */
__ZEO_EXPORT zMat2D *zMat2DInv(const zMat2D *mat, zMat2D *invmat);

/* ********************************************************** */
/* multiplication of a 2D vector by a 2x2 matrix
 * ********************************************************** */

/*! \brief multiply a 2D vector and a 2x2 matrix.
 *
 * zMulMat2DVec2D() multiplies a 2D vector \a v by a 2x2 matrix \a m and puts it into \a mv.
 * zMulMat2DVec2DDRC() directly multiplies a 2D vector \a v by a 2x2 matrix \a m.
 *
 * zMulMat2DTVec2D() multiplies \a v by the transpose matrix of a 2x2 matrix \a m and puts it into \a mv.
 * zMulMat2DTVec2DDRC() directly multiplies a 2D vector \a v by the transpose of a 2x2 matrix \a m.
 * \return
 * zMulMat2DVec2D() and zMulMat2DTVec2D() return the pointer \a mv.
 * zMulMat2DVec2DDRC() and zMulMat2DTVec2DDRC() return the pointer \a v.
 */
#define _zMulMat2DVec2D(m,v,mv) do{\
  double __x, __y;\
  __x = (m)->c.xx*(v)->c.x + (m)->c.yx*(v)->c.y;\
  __y = (m)->c.xy*(v)->c.x + (m)->c.yy*(v)->c.y;\
  _zVec2DCreate( mv, __x, __y );\
} while(0)
__ZEO_EXPORT zVec2D *zMulMat2DVec2D(const zMat2D *m, const zVec2D *v, zVec2D *mv);
#define _zMulMat2DVec2DDRC(m,v) _zMulMat2DVec2D(m,v,v)
__ZEO_EXPORT zVec2D *zMulMat2DVec2DDRC(const zMat2D *m, zVec2D *v);
#define _zMulMat2DTVec2D(m,v,mv) do{\
  double __x, __y;\
  __x = _zVec2DInnerProd( &(m)->b.x, v );\
  __y = _zVec2DInnerProd( &(m)->b.y, v );\
  _zVec2DCreate( mv, __x, __y );\
} while(0)
__ZEO_EXPORT zVec2D *zMulMat2DTVec2D(const zMat2D *m, const zVec2D *v, zVec2D *mv);
#define _zMulMat2DTVec2DDRC(m,v) _zMulMat2DTVec2D(m,v,v)
__ZEO_EXPORT zVec2D *zMulMat2DTVec2DDRC(const zMat2D *m, zVec2D *v);

/*! \brief multiply a 2D vector by the adjugate of a 2x2 matrix.
 *
 * zMulAdjMat2DVec2D() multiplies a 2D vector \a vec by the adjugate matrix of a 2x2 matrix \a mat.
 * The result is put into \a adj_vec.
 * \return
 * zMulAdjMat2DVec2D() returns a pointer \a adj_vec.
 */
#define _zMulAdjMat2DVec2D(mat,vec,adj_vec) do{\
  double __x, __y;\
  __x = (mat)->c.yy*(vec)->c.x - (mat)->c.yx*(vec)->c.y; \
  __y =-(mat)->c.xy*(vec)->c.x + (mat)->c.xx*(vec)->c.y; \
  _zVec2DCreate( adj_vec, __x, __y );\
} while(0)
__ZEO_EXPORT zVec2D *zMulAdjMat2DVec2D(const zMat2D *mat, const zVec2D *vec, zVec2D *adj_vec);

/*! \brief multiply a 2D vector and the inverse of a 2x2 matrix.
 *
 * zMulInvMat2DVec2D() multiplies a 2D \a vec by the inverse matrix of a 2x2 matrix \a mat.
 * The result is put into \a inv_vec.
 * \return
 * zMulInvMat2DVec2D() returns a pointer \a inv_vec.
 */
__ZEO_EXPORT zVec2D *zMulInvMat2DVec2D(const zMat2D *mat, const zVec2D *vec, zVec2D *inv_vec);

/* ********************************************************** */
/* multiplication of a 2x2 matrix by another 2x2 matrix
 * ********************************************************** */

/*! \brief multiply 2x2 matrices.
 *
 * zMulMat2DMat2D() multiplies a 2x2 matrix \a m2 by the other \a m1 from the leftside and puts it into \a m.
 *
 * zMulMat2DTMat2D() multiplies a 2x2 matrix m2 by the transpose of a 2x2 matrix \a m1 from the leftside,
 * and puts it into \a m.
 *
 * zMulMat2DMat2DT() multiplies a 2x2 matrix \a m1 by the transpose of a 2x2 matrix \a m2 from the rightside,
 * and puts it into \a m.
 *
 * zMulMat2DMat2DDRC() directly multiplies a 2x2 matrix \a m2 by the other \a m1 from the leftside.
 *
 * zMulMat2DTMat2DDRC() directly multiplies a 2x2 matrix \a m2 by the transpose of the other \a m1 from the
 * leftside.
 *
 * zMulMat2DMat2DTDRC() directly multiplies a 2x2 matrix \a m1 by the transpose of the other \a m2 from the
 * rightside.
 * \return
 * zMulMat2DMat2D(), zMulMat2DTMat2D(), and zMulMat2DMat2DT() return the pointer \a m.
 *
 * zMulMat2DMat2DDRC() and zMulMat2DTMat2DDRC() return the pointer \a m2.
 * zMulMat2DMat2DTDRC() returns the pointer \a m1.
 */
__ZEO_EXPORT zMat2D *zMulMat2DMat2D(const zMat2D *m1, const zMat2D *m2, zMat2D *m);
__ZEO_EXPORT zMat2D *zMulMat2DTMat2D(const zMat2D *m1, const zMat2D *m2, zMat2D *m);
__ZEO_EXPORT zMat2D *zMulMat2DMat2DT(const zMat2D *m1, const zMat2D *m2, zMat2D *m);

#define zMulMat2DMat2DDRC(m1,m2)  zMulMat2DMat2D(m1,m2,m2)
#define zMulMat2DTMat2DDRC(m1,m2) zMulMat2DTMat2D(m1,m2,m2)
#define zMulMat2DMat2DTDRC(m1,m2) zMulMat2DMat2DT(m1,m2,m1)

/*! \brief multiply the inverse of a 2x2 matrix to another.
 *
 * zMulInvMat2DMat2D() multiplies a 2x2 matrix \a m2 by the inverse of the other 2x2 matrix \a m1
 * from the leftside, and puts it into \a m.
 * \return
 * zMulInvMat2DMat2D() returns the pointer \a m.
 */
__ZEO_EXPORT zMat2D *zMulInvMat2DMat2D(const zMat2D *m1, const zMat2D *m2, zMat2D *m);

/* ********************************************************** */
/* rotation
 * ********************************************************** */

/*! \brief equivalent rotation angle of a 2x2 matrix.
 *
 * zMat2DAngle() finds the angle that is equivalent with a 2x2 matrix \a mat.
 * It assumes that \a mat is an orthonormal matrix; if not, the result does not make sense.
 * \return
 * zMat2DAngle() returns the counterclockwise angle between the x/y-axis of the original coordinate frame
 * and that of \a mat.
 */
#define _zMat2DAngle(mat) atan2( (mat)->c.xy - (mat)->c.yx, (mat)->c.xx + (mat)->c.yy )
__ZEO_EXPORT double zMat2DAngle(const zMat2D *mat);

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
__ZEO_EXPORT zMat2D *zMat2DRotSC(const zMat2D *m, double s, double c, zMat2D *rm);
__ZEO_EXPORT zMat2D *zMat2DRot(const zMat2D *m, double angle, zMat2D *rm);

/*! \brief error vector between two attitude matrices.
 *
 * zMat2DError() calculates the error angle from \a m2 to \a m1 (note the order).
 * \return
 * zMat2DError() returns the error angle computed.
 */
#define _zMat2DError(m1,m2) \
  atan2( (m2)->c.xx*(m1)->c.xy+(m2)->c.yx*(m1)->c.yy-(m2)->c.xy*(m1)->c.xx-(m2)->c.yy*(m1)->c.yx,\
         (m2)->c.xx*(m1)->c.xx+(m2)->c.yx*(m1)->c.yx+(m2)->c.xy*(m1)->c.xy+(m2)->c.yy*(m1)->c.yy )
__ZEO_EXPORT double zMat2DError(const zMat2D *m1, const zMat2D *m2);

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
__ZEO_EXPORT zMat2D *zMat2DFScan(FILE *fp, zMat2D *m);
#define zMat2DScan(m) zMat2DFScan( stdin, (m) )
__ZEO_EXPORT void zMat2DFPrint(FILE *fp, const zMat2D *m);
#define zMat2DPrint(m) zMat2DFPrint( stdout, (m) )

__END_DECLS

#ifdef __cplusplus
inline zMat2D::zMat2D(){ zMat2DZero( this ); }
inline zMat2D::zMat2D(double a11, double a12, double a21, double a22){ _zMat2DCreate( this, a11, a12, a21, a22 ); }
inline zMat2D::zMat2D(const zVec2D &v1, const zVec2D &v2){ _zVec2DCopy( &v1, &this->v[0] ); _zVec2DCopy( &v2, &this->v[1] ); }
inline zMat2D::zMat2D(const zMat2D &mat){ _zMat2DCopy( &mat, this ); }
inline zVec2D &zMat2D::vec(zAxis a){ return *zMat2DVec( this, a ); }
inline zMat2D &zMat2D::create(double a11, double a12, double a21, double a22){
  _zMat2DCreate( this, a11, a12, a21, a22 );
  return *this;
}
inline zMat2D &zMat2D::create(const zVec2D &v1, const zVec2D &v2){
  _zVec2DCopy( &v1, &this->v[0] );
  _zVec2DCopy( &v2, &this->v[1] );
  return *this;
}
inline zMat2D &zMat2D::copy(zMat2D &m){ _zMat2DCopy( &m, this ); return *this; }
inline zMat2D &zMat2D::zero(){ zMat2DZero( this ); return *this; }
inline zMat2D &zMat2D::ident(){ zMat2DIdent( this ); return *this; }
inline bool zMat2D::isEqual(zMat2D &m){ return _zMat2DEqual( this, &m ); }
inline bool zMat2D::isIdent(){ return _zMat2DIsIdent( this ); }
inline bool zMat2D::isTol(double tol=zTOL){ return _zMat2DIsTol( this, tol ); }
inline zVec2D &zMat2D::row(int i, zVec2D &v){ _zMat2DRow( this, i, &v ); return v; }
inline zVec2D &zMat2D::col(int i, zVec2D &v){ zMat2DCol( this, i, &v ); return v; }
inline zMat2D &zMat2D::transpose(zMat2D &m){ _zMat2DT( this, &m ); return m; }
inline zMat2D zMat2D::transpose(){ _zMat2DTDRC( this ); return *this; }
inline zMat2D zMat2D::operator-(){ zMat2D ret; _zMat2DRev( this, &ret ); return ret; }
inline zMat2D &zMat2D::operator+=(zMat2D &m){ _zMat2DAddDRC( this, &m ); return *this; }
inline zMat2D &zMat2D::operator-=(zMat2D &m){ _zMat2DSubDRC( this, &m ); return *this; }
inline zMat2D &zMat2D::operator*=(double k){ _zMat2DMulDRC( this, k ); return *this; }
inline zMat2D &zMat2D::operator/=(double k){ zMat2DDivDRC( this, k ); return *this; }
inline zMat2D &zMat2D::createDyad(zVec2D &v1, zVec2D &v2){ _zMat2DDyad( this, &v1, &v2 ); return *this; }
inline double zMat2D::squareNorm(){ return _zMat2DSqrNorm( this ); }
inline double zMat2D::norm(){ return _zMat2DNorm( this ); }
inline double zMat2D::det(){ return _zMat2DDet( this ); }
inline zMat2D &zMat2D::inv(zMat2D &m){ zMat2DInv( &m, this ); return *this; }
inline zVec2D zMat2D::mulT(zVec2D &v){ zVec2D mv; _zMulMat2DTVec2D( this, &v, &mv ); return mv; }
inline zVec2D zMat2D::mulInv(zVec2D &v){ zVec2D mv; zMulInvMat2DVec2D( this, &v, &mv ); return mv; }
inline zMat2D zMat2D::mulT(zMat2D &m){ zMat2D mm; zMulMat2DTMat2D( this, &m, &mm ); return mm; }
inline zMat2D zMat2D::mulInv(zMat2D &m){ zMat2D mm; zMulInvMat2DMat2D( this, &m, &mm ); return mm; }
inline zMat2D &zMat2D::createAngle(double angle){ return *zMat2DCreateAngle( this, angle ); }

inline zMat2D operator+(const zMat2D &m1, const zMat2D &m2){ zMat2D ret; _zMat2DAdd( &m1, &m2, &ret ); return ret; }
inline zMat2D operator-(const zMat2D &m1, const zMat2D &m2){ zMat2D ret; _zMat2DSub( &m1, &m2, &ret ); return ret; }
inline zMat2D operator*(const zMat2D &m, double k){ zMat2D ret; _zMat2DMul( &m, k, &ret ); return ret; }
inline zMat2D operator*(double k, const zMat2D &m){ zMat2D ret; _zMat2DMul( &m, k, &ret ); return ret; }
inline zVec2D operator*(const zMat2D &m, const zVec2D &v){ zVec2D ret; zMulMat2DVec2D( &m, &v, &ret ); return ret; }
inline zMat2D operator*(const zMat2D &m1, const zMat2D &m2){ zMat2D ret; zMulMat2DMat2D( &m1, &m2, &ret ); return ret; }
inline zMat2D operator/(const zMat2D &m, double k){ zMat2D ret; zMat2DDiv( &m, k, &ret ); return ret; }
inline bool operator==(const zMat2D &m1, const zMat2D &m2){ return _zMat2DMatch( &m1, &m2 ); }
__ZEO_EXPORT std::ostream &operator<<(std::ostream &stream, zMat2D &mat);
#endif /* __cplusplus */

#endif /* __ZEO_MAT2D_H__ */
