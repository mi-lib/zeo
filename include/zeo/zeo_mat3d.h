/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat3d - 3x3 matrix.
 */

#ifndef __ZEO_MAT3D_H__
#define __ZEO_MAT3D_H__

#include <zeo/zeo_vec6d.h>

__BEGIN_DECLS

/*! \struct zMat3D
 * \brief 3x3 matrix for attitude / rotation representation.
 */
/* The order of matrix components are:
 * |~ e[0][0]  e[1][0]  e[2][0] ~|   |~ c.xx  c.yx  c.zx ~|
 * |  e[0][1]  e[1][1]  e[2][1]  | = |  c.xy  c.yy  c.zy  | = [ v[0]  v[1]  v[2] ] = [ b.x  b.y  b.z ]
 * |_ e[0][2]  e[1][2]  e[2][2] _|   |_ c.xz  c.yz  c.zz _|
 */
typedef union{
  double e[3][3];   /*!< 3x3 matrix */
  zVec3D v[3];      /*!< 3 column vectors */
  struct{
    zVec3D x, y, z;
  } b;              /*!< x-y-z base vectors */
  struct{
    double xx, xy, xz, yx, yy, yz, zx, zy, zz;
  } c;             /*!< 9 components */
} zMat3D;

/*! \brief 3D zero matrix and identity matrix */
extern const zMat3D zmat3Dzero;
extern const zMat3D zmat3Dident;
#define ZMAT3DZERO  ( (zMat3D *)&zmat3Dzero )
#define ZMAT3DIDENT ( (zMat3D *)&zmat3Dident )

/*! \brief create, copy and cleanup a 3x3 matrix.
 *
 * zMat3DCreate() creates a 3x3 matrix from nine values as follows.
 *  |~ \a a11  \a a12  \a a13 ~|
 *  |  \a a21  \a a22  \a a23  |
 *  |_ \a a31  \a a32  \a a33 _|
 *
 * zMat3DCopy() copies a 3x3 matrix \a src to \a dest.
 *
 * zMat3DZero() sets all components of a 3x3 matrix \a m for zeros.
 *
 * zMat3DIdent() makes a 3x3 matrix \a m the identity matrix.
 * \return
 * zMat3DCreate(), zMat3DZero() and zMat3DIdent() return a pointer \a m.
 *
 * zMat3DCopy() returns a pointer \a m.
 */
#define _zMat3DCreate(m,a11,a12,a13,a21,a22,a23,a31,a32,a33) do{\
  _zVec3DCreate( &(m)->b.x, a11, a21, a31 );\
  _zVec3DCreate( &(m)->b.y, a12, a22, a32 );\
  _zVec3DCreate( &(m)->b.z, a13, a23, a33 );\
} while(0)
__EXPORT zMat3D *zMat3DCreate(zMat3D *m,
  double a11, double a12, double a13,
  double a21, double a22, double a23,
  double a31, double a32, double a33);
#define zMat3DCopy(s,d) zCopy( zMat3D, s, d )
#define zMat3DZero(m)   zMat3DCopy( ZMAT3DZERO, m )
#define zMat3DIdent(m)  zMat3DCopy( ZMAT3DIDENT, m )

/*! \brief check if the two 3x3 matrices are equal.
 *
 * zMat3DMatch() and zMat3DEqual() check if the two 3x3 matrices
 * \a m1 and \a m2 are equal. They return a boolean value.
 *
 * zMat3DMatch() strictly compares the two matrices, while
 * zMat3DEqual() checks if the error between \a m1 and \a m2
 * are sufficiently small.
 * \return
 * zMat3DMatch() and zMat3DEqual() return the true value if
 * \a m1 and \a m2 are equal, or false otherwise.
 */
#define _zMat3DMatch(m1,m2) ( _zVec3DMatch(&(m1)->b.x,&(m2)->b.x) && _zVec3DMatch(&(m1)->b.y,&(m2)->b.y) && _zVec3DMatch(&(m1)->b.z,&(m2)->b.z) )
__EXPORT bool zMat3DMatch(zMat3D *m1, zMat3D *m2);
#define _zMat3DEqual(m1,m2) ( _zVec3DEqual(&(m1)->b.x,&(m2)->b.x) && _zVec3DEqual(&(m1)->b.y,&(m2)->b.y) && _zVec3DEqual(&(m1)->b.z,&(m2)->b.z) )
__EXPORT bool zMat3DEqual(zMat3D *m1, zMat3D *m2);

/*! \brief check if a 3D matrix is tiny.
 *
 * zMat3DIsTol() checks if the absolute values of all
 * components of a 3x3 matrix \a m are smaller than \a tol.
 *
 * zMat3DIsTiny() applies zTOL (defined in zm_misc.h) to
 * the tolerance of zMat3DIsTol().
 * \return
 * zMat3DIsTol() and zMat3DIsTiny() return the true value if
 * the absolute values of all components of \a m are smaller
 * than \a tol and zTOL, respectively, or the false value,
 * otherwise.
 * \notes
 * \a tol must be positive.
 * \sa
 * zIsTol, zIsTiny
 */
#define _zMat3DIsTol(m,tol) ( _zVec3DIsTol( &(m)->b.x, tol ) && _zVec3DIsTol( &(m)->b.y, tol ) && _zVec3DIsTol( &(m)->b.z, tol ) )
__EXPORT bool zMat3DIsTol(zMat3D *m, double tol);
#define _zMat3DIsTiny(m) _zMat3DIsTol( m, zTOL )
#define zMat3DIsTiny(m)  zMat3DIsTol( m, zTOL )

/*! \brief abstract row/column vectors from a 3x3 matrix.
 *
 * zMat3DRow() abstracts the \a i'th row from a 3x3 matrix \a m and puts
 * it into \a v.
 *
 * zMat3DCol() abstracts the \a i'th column from a 3x3 matrix \a m and
 * puts it into \a v.
 * \return
 * zMat3DRow() and zMat3DCol() return a pointer \a v.
 */
#define _zMat3DRow(m,i,r) _zVec3DCreate( (r), (m)->e[0][(i)], (m)->e[1][(i)], (m)->e[2][(i)] )
__EXPORT zVec3D *zMat3DRow(zMat3D *m, int i, zVec3D *v);
#define zMat3DCol(m,i,c)  zVec3DCopy( (&(m)->v[(i)]), c )

/*! \brief transpose of a 3x3 matrix.
 *
 * zMat3DT() transposes a 3x3 matrix \a m and puts it into \a tm.
 * \return
 * zMat3DT() returns a pointer \a tm.
 * \notes
 * It is not allowed to let \a tm point to the same address with \a m.
 * When \a tm is the same with \a m, anything might happen.
 */
#define _zMat3DT(m,tm) do{\
  _zMat3DRow( m, 0, &(tm)->b.x );\
  _zMat3DRow( m, 1, &(tm)->b.y );\
  _zMat3DRow( m, 2, &(tm)->b.z );\
} while(0)
#define _zMat3DTDRC(m) do{\
  double __tmp;\
  __tmp = (m)->e[1][0]; (m)->e[1][0] = (m)->e[0][1]; (m)->e[0][1] = __tmp;\
  __tmp = (m)->e[2][0]; (m)->e[2][0] = (m)->e[0][2]; (m)->e[0][2] = __tmp;\
  __tmp = (m)->e[2][1]; (m)->e[2][1] = (m)->e[1][2]; (m)->e[1][2] = __tmp;\
} while(0)
__EXPORT zMat3D *zMat3DT(zMat3D *m, zMat3D *tm);
__EXPORT zMat3D *zMat3DTDRC(zMat3D *m);

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/*! \brief the four rules of the arithmetics for 3x3 matrix.
 *
 * zMat3DAdd() adds two 3x3 matrices \a m1 and \a m2 and puts it into \a m.
 *
 * zMat3DSub() subtracts a 3x3 matrix \a m2 from the other \a m1 and puts
 * it into \a m.
 *
 * zMat3DRev() reverses a 3x3 matrix \a m and puts it into \a rm.
 *
 * zMat3DMul() multiplies a 3x3 matrix \a m by a scalar value \a k and
 * puts it into \a mm.
 *
 * zMat3DDiv() divides a 3x3 matrix \a m by a scalar value \a k and puts
 * it into \a dm.
 *
 * zMat3DCat() multiplies a 3x3 matrix \a m2 by a scalar value \a k,
 * concatenates it to \a m1 and puts it into \a m.
 *
 * zMat3DAddDRC() adds a 3x3 matrix \a m2 directly to the other \a m1.
 *
 * zMat3DSubDRC() subtracts a 3x3 matrix \a m2 directly from the other \a m1.
 *
 * zMat3DRevDRC() reverses a 3x3 matrix \a m directly.
 *
 * zMat3DMulDRC() multiplies a 3x3 matrix \a m directly by a scalar value \a k.
 *
 * zMat3DDivDRC() divides a 3x3 matrix \a m directly by a scalar value \a k.
 *
 * zMat3DCatDRC() multiplies a 3x3 matrix \a m2 by a scalar value \a k and
 * adds it directly to \a m1.
 * \return
 * Each function returns a pointer to the result matrix.
 *
 * zMat3DDiv() and zMat3DDivDRC() return the null pointer if \a k is zero.
 */
#define _zMat3DAdd(m1,m2,m) do{\
  _zVec3DAdd( &(m1)->b.x, &(m2)->b.x, &(m)->b.x );\
  _zVec3DAdd( &(m1)->b.y, &(m2)->b.y, &(m)->b.y );\
  _zVec3DAdd( &(m1)->b.z, &(m2)->b.z, &(m)->b.z );\
} while(0)
__EXPORT zMat3D *zMat3DAdd(zMat3D *m1, zMat3D *m2, zMat3D *m);
#define _zMat3DSub(m1,m2,m) do{\
  _zVec3DSub( &(m1)->b.x, &(m2)->b.x, &(m)->b.x );\
  _zVec3DSub( &(m1)->b.y, &(m2)->b.y, &(m)->b.y );\
  _zVec3DSub( &(m1)->b.z, &(m2)->b.z, &(m)->b.z );\
} while(0)
__EXPORT zMat3D *zMat3DSub(zMat3D *m1, zMat3D *m2, zMat3D *m);
#define _zMat3DRev(m,rm) do{\
  _zVec3DRev( &(m)->b.x, &(rm)->b.x );\
  _zVec3DRev( &(m)->b.y, &(rm)->b.y );\
  _zVec3DRev( &(m)->b.z, &(rm)->b.z );\
} while(0)
__EXPORT zMat3D *zMat3DRev(zMat3D *m, zMat3D *rm);
#define _zMat3DMul(m,k,mm) do{\
  _zVec3DMul( &(m)->b.x, k, &(mm)->b.x );\
  _zVec3DMul( &(m)->b.y, k, &(mm)->b.y );\
  _zVec3DMul( &(m)->b.z, k, &(mm)->b.z );\
} while(0)
__EXPORT zMat3D *zMat3DMul(zMat3D *m, double k, zMat3D *mm);
__EXPORT zMat3D *zMat3DDiv(zMat3D *m, double k, zMat3D *dm);
#define _zMat3DCat(m1,k,m2,m) do{\
  _zVec3DCat( &(m1)->b.x, k, &(m2)->b.x, &(m)->b.x );\
  _zVec3DCat( &(m1)->b.y, k, &(m2)->b.y, &(m)->b.y );\
  _zVec3DCat( &(m1)->b.z, k, &(m2)->b.z, &(m)->b.z );\
} while(0)
__EXPORT zMat3D *zMat3DCat(zMat3D *m1, double k, zMat3D *m2, zMat3D *m);

#define _zMat3DAddDRC(m1,m2) do{\
  _zVec3DAddDRC( &(m1)->b.x, &(m2)->b.x );\
  _zVec3DAddDRC( &(m1)->b.y, &(m2)->b.y );\
  _zVec3DAddDRC( &(m1)->b.z, &(m2)->b.z );\
} while(0)
#define zMat3DAddDRC(m1,m2)    zMat3DAdd(m1,m2,m1)
#define _zMat3DSubDRC(m1,m2) do{\
  _zVec3DSubDRC( &(m1)->b.x, &(m2)->b.x );\
  _zVec3DSubDRC( &(m1)->b.y, &(m2)->b.y );\
  _zVec3DSubDRC( &(m1)->b.z, &(m2)->b.z );\
} while(0)
#define zMat3DSubDRC(m1,m2) zMat3DSub(m1,m2,m1)
#define _zMat3DRevDRC(m) do{\
  _zVec3DRevDRC( &(m)->b.x );\
  _zVec3DRevDRC( &(m)->b.y );\
  _zVec3DRevDRC( &(m)->b.z );\
} while(0)
#define zMat3DRevDRC(m) zMat3DRev(m,m)
#define _zMat3DMulDRC(m,k) do{\
  _zVec3DMulDRC( &(m)->b.x, k );\
  _zVec3DMulDRC( &(m)->b.y, k );\
  _zVec3DMulDRC( &(m)->b.z, k );\
} while(0)
#define zMat3DMulDRC(m,k) zMat3DMul(m,k,m)
#define zMat3DDivDRC(m,k) zMat3DDiv(m,k,m)
#define _zMat3DCatDRC(m1,k,m2) do{\
  _zVec3DCatDRC( &(m1)->b.x, k, &(m2)->b.x );\
  _zVec3DCatDRC( &(m1)->b.y, k, &(m2)->b.y );\
  _zVec3DCatDRC( &(m1)->b.z, k, &(m2)->b.z );\
} while(0)
#define zMat3DCatDRC(m1,k,m2) zMat3DCat(m1,k,m2,m1)

/*! \brief dyadic product of two 3D vectors.
 *
 * zMat3DDyad() calculates dyadic product of two 3D vectors \a v1
 * and \a v2, and puts it into \a dyad. Namely,
 *   \a dyad = \a v1 \a v2 ^T.
 * \return
 * zMat3DDyad() returns a pointer \a dyad.
 */
#define _zMat3DDyad(m,v1,v2) do{\
  _zVec3DMul( v1, (v2)->c.x, &(m)->b.x );\
  _zVec3DMul( v1, (v2)->c.y, &(m)->b.y );\
  _zVec3DMul( v1, (v2)->c.z, &(m)->b.z );\
} while(0)
__EXPORT zMat3D *zMat3DDyad(zMat3D *m, zVec3D *v1, zVec3D *v2);

/*! \brief add dyadic product of two 3D vectors to a 3x3 matrix.
 *
 * zMat3DAddDyad() addes dyadic product of two 3D vectors \a v1 and
 * \a v2 to a 3x3 matrix \a m. Namely,
 *   \a m = \a m + \a v1 \a v2 ^T.
 * \return
 * zMat3DAddDyad() returns a pointer \a m.
 */
#define _zMat3DAddDyad(m,v1,v2) do{\
  _zVec3DCatDRC( &(m)->b.x, (v2)->c.x, v1 );\
  _zVec3DCatDRC( &(m)->b.y, (v2)->c.y, v1 );\
  _zVec3DCatDRC( &(m)->b.z, (v2)->c.z, v1 );\
} while(0)
__EXPORT zMat3D *zMat3DAddDyad(zMat3D *m, zVec3D *v1, zVec3D *v2);

/*! \brief subtract dyadic product of two 3D vectors to a 3x3 matrix.
 *
 * zMat3DSubDyad() subtracts dyadic product of two 3D vectors \a v1
 * and \a v2 from a 3x3 matrix \a m. Namely,
 *   \a m = \a m - \a v1 \a v2 ^T.
 * \return
 * zMat3DSubDyad() returns a pointer \a m.
 */
#define _zMat3DSubDyad(m,v1,v2) do{\
  _zVec3DCatDRC( &(m)->b.x, -(v2)->c.x, v1 );\
  _zVec3DCatDRC( &(m)->b.y, -(v2)->c.y, v1 );\
  _zVec3DCatDRC( &(m)->b.z, -(v2)->c.z, v1 );\
} while(0)
__EXPORT zMat3D *zMat3DSubDyad(zMat3D *m, zVec3D *v1, zVec3D *v2);

/*! \brief create a 3x3 matrix equivalent to the outer product of a 3D vector.
 *
 * zVec3DOuterProdMat3D() computes the outer-product skew-symmetric matrix
 * of a 3D vector \a v and puts it into \a m. Namely, \a m a is equivalent
 * with \a v x a with respect to an arbitrary 3D vector a.
 *
 * zVec3DOuterProd2Mat3D() computes the twice-outer-product matrix of a 3D
 * vector \a v1 and \a v2 and puts it into \a m. Namely, \a m a is equivalent
 * with \a v1 x ( \a v2 x a ) ) with respect to an arbitrary 3D vector a.
 * \return
 * zVec3DOuterProdMat3D() and zVec3DOuterProd2Mat3D() return a pointer \a m.
 */
#define _zVec3DOuterProd2Mat3D(vo,m) \
  _zMat3DCreate( m, 0.0,-(vo)->c.z, (vo)->c.y,\
                    (vo)->c.z, 0.0,-(vo)->c.x,\
                   -(vo)->c.y, (vo)->c.x, 0.0 )
__EXPORT zMat3D *zVec3DOuterProd2Mat3D(zVec3D *v, zMat3D *m);
#define _zVec3DTripleProd2Mat3D(v1,v2,m) \
  _zMat3DCreate( m,-(v1)->c.y*(v2)->c.y-(v1)->c.z*(v2)->c.z, (v1)->c.y*(v2)->c.x, (v1)->c.z*(v2)->c.x,\
                    (v1)->c.x*(v2)->c.y,-(v1)->c.z*(v2)->c.z-(v1)->c.x*(v2)->c.x, (v1)->c.z*(v2)->c.y,\
                    (v1)->c.x*(v2)->c.z, (v1)->c.y*(v2)->c.z,-(v1)->c.x*(v2)->c.x-(v1)->c.y*(v2)->c.y )
__EXPORT zMat3D *zVec3DTripleProd2Mat3D(zVec3D *v1, zVec3D *v2, zMat3D *m);

/*! \brief multiply cross product of a 3D vector and a 3x3 matrix.
 *
 * zMulVec3DOPMat3D() multiplies a 3x3 matrix \a m by the outer product of
 * a 3D vector \a ohm and puts it into \a mv. Namely,
 *   \a mv = \a ohm x \a m.
 *
 * zMulVec3DOPMat3DDRC() directly multiplies a 3x3 matrix \a m by the
 * outer product of a 3D vector \a ohm.
 * \return
 * zMulVec3DOPMat3D() returns a pointer \a mv.
 * zMulVec3DOPMat3DDRC() returns a pointer \a m.
 */
__EXPORT zMat3D *zMulVec3DOPMat3D(zVec3D *ohm, zMat3D *m, zMat3D *mv);
#define zMulVec3DOPMat3DDRC(o,m) zMulVec3DOPMat3D( o, m, m )

/*! \brief calculate norm of a 3D matrix.
 *
 * zMat3DNorm() calculates a norm of a 3D matrix \a m.
 * zMat3DSqrNorm() calculates a squared norm of \a m.
 * \return
 * zMat3DNorm() returns a norm of \a m.
 * zMat3DSqrNorm() returns a squared norm of \a m.
 */
#define _zMat3DSqrNorm(m) ( _zVec3DSqrNorm(&(m)->b.x) + _zVec3DSqrNorm(&(m)->b.y) + _zVec3DSqrNorm(&(m)->b.z) )
__EXPORT double zMat3DSqrNorm(zMat3D *m);
#define _zMat3DNorm(m) sqrt( _zMat3DSqrNorm(m) )
#define zMat3DNorm(m) sqrt( zMat3DSqrNorm(m) )

/* ********************************************************** */
/* inverse of a 3x3 matrix
 * ********************************************************** */

/*! \brief determinant of a 3x3 matrix.
 *
 * zMat3DDet() computes the determinant of an arbitrary 3x3 matrix \a m.
 * \retval the determinant of \a m
 */
#define _zMat3DDet(m) ( ( m->c.yy*m->c.zz - m->c.yz*m->c.zy ) * m->c.xx \
                      + ( m->c.xz*m->c.zy - m->c.xy*m->c.zz ) * m->c.yx \
                      + ( m->c.xy*m->c.yz - m->c.xz*m->c.yy ) * m->c.zx )
__EXPORT double zMat3DDet(zMat3D *m);

/*! \brief inverse of a 3x3 matrix.
 *
 * zMat3DInv() computes the inverse matrix of an arbitrary 3x3 matrix \a m
 * and puts it into \a im. It does not assume that \a m is an orthonormal
 * matrix.
 * \return
 * zMat3DInv() returns a pointer \a im, if it succeeds. If \a m is singular,
 * it returns the null pointer.
 * \notes
 * \a im has to point to a different address from \a m. If \a im is the
 * same with \a m, anything might happen.
 */
__EXPORT zMat3D *zMat3DInv(zMat3D *m, zMat3D *im);

/* ********************************************************** */
/* multiplication of a 3D vector by a 3x3 matrix
 * ********************************************************** */

/*! \brief multiply a 3D vector and a 3x3 matrix.
 *
 * zMulMat3DVec3D() multiplies a 3D vector \a v by a 3x3 matrix \a m and puts
 * it into \a mv.
 *
 * zMulMat3DTVec3D() multiplies a 3D vector \a v by the transpose matrix of
 * a 3x3 matrix \a m and puts it into \a mv.
 *
 * zMulMat3DVec3DDRC() directly multiplies a 3D vector \a v by a 3x3 matrix \a m.
 *
 * zMulMat3DTVec3DDRC() directly multiplies a 3D vector v by the transpose
 * of a 3x3 matrix \a m.
 *
 * zMulInvMat3DVec3D() multiplies a 3D vector \a v by the inverse of a 3x3
 * matrix \a m and puts it into \a miv.
 * \return
 * Each function returns a pointer to the result.
 */
#define _zMulMat3DVec3D(m,v,mv) do{\
  double __x, __y, __z;\
  __x = (m)->c.xx*(v)->c.x + (m)->c.yx*(v)->c.y + (m)->c.zx*(v)->c.z;\
  __y = (m)->c.xy*(v)->c.x + (m)->c.yy*(v)->c.y + (m)->c.zy*(v)->c.z;\
  __z = (m)->c.xz*(v)->c.x + (m)->c.yz*(v)->c.y + (m)->c.zz*(v)->c.z;\
  _zVec3DCreate( mv, __x, __y, __z );\
} while(0)
__EXPORT zVec3D *zMulMat3DVec3D(zMat3D *m, zVec3D *v, zVec3D *mv);
#define _zMulMat3DTVec3D(m,v,mv) do{\
  double __x, __y, __z;\
  __x = _zVec3DInnerProd( &(m)->b.x, v );\
  __y = _zVec3DInnerProd( &(m)->b.y, v );\
  __z = _zVec3DInnerProd( &(m)->b.z, v );\
  _zVec3DCreate( mv, __x, __y, __z );\
} while(0)
__EXPORT zVec3D *zMulMat3DTVec3D(zMat3D *m, zVec3D *v, zVec3D *mv);

/*! \brief directly multiply a 3D vector by a 3x3 matrix.
 */
#define _zMulMat3DVec3DDRC(m,v) _zMulMat3DVec3D(m,v,v)
__EXPORT zVec3D *zMulMat3DVec3DDRC(zMat3D *m, zVec3D *v);

/*! \brief directly multiply a 3D vector by transpose of a 3x3 matrix.
 */
#define _zMulMat3DTVec3DDRC(m,v) _zMulMat3DTVec3D(m,v,v)
__EXPORT zVec3D *zMulMat3DTVec3DDRC(zMat3D *m, zVec3D *v);

/*! \brief multiply a 3D vector by inverse of a 3x3 matrix.
 */
__EXPORT zVec3D *zMulInvMat3DVec3D(zMat3D *m, zVec3D *v, zVec3D *imv);

/*! \brief inversely compute the concatenate ratio of a 3D vector.
 *
 * zVec3DCatRatio() calculates the concatenate ratio of a 3D vector \a v
 * for three bases \a v1, \a v2 and \a v3. Namely, an arbitrary vector \a v
 * is represented by a concatenation of three bases vectors \a v1, \a v2
 * and \a v3 as follows.
 *  \a v = \a r1 * \a v1 + \a r2 * \a v2 + \a r3 * \a v3
 * zVec3DCatRatio() computes \a r1, \a r2 and \a r3 in the above equation
 * and puts them into \a ratio.
 * \notes
 * Consequently, the array \a ratio must have three components at least.
 * If not, anything might happen.
 *
 * This function fails if \a v1, \a v2 and \a v3 are not independent with
 * each other.
 * \return
 * zVec3DCatRatio() returns a pointer \a ratio.
 */
__EXPORT double *zVec3DCatRatio(zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v, double ratio[]);

/* ********************************************************** */
/* multiplication of a 3x3 matrix by another 3x3 matrix
 * ********************************************************** */

/*! \brief multiply 3x3 matrices.
 *
 * zMulMat3DMat3D() multiplies a 3x3 matrix \a m2 by the other \a m1
 * from the leftside and puts it into \a m.
 *
 * zMulMat3DTMat3D() multiplies a 3x3 matrix m2 by the transpose of
 * a 3x3 matrix \a m1 from the leftside and puts it into \a m.
 *
 * zMulMat3DMat3DT() multiplies a 3x3 matrix \a m1 by the transpose
 * of a 3x3 matrix \a m2 from the rightside and puts it into \a m.
 *
 * zMulMat3DMat3DDRC() directly multiplies a 3x3 matrix \a m2 by the
 * other \a m1 from the leftside.
 *
 * zMulMat3DTMat3DDRC() directly multiplies a 3x3 matrix \a m2 by the
 * transpose of the other \a m1 from the leftside.
 *
 * zMulMat3DMat3DTDRC() directly multiplies a 3x3 matrix \a m1 by the
 * transpose of the other \a m2 from the rightside.
 *
 * zMulInvMat3DMat3D() multiplies \a m2 by the inverse of a 3x3 matrix
 * \a m1 from the leftside and puts it into \a m.
 * \return
 * Each function returns a pointer to the result.
 */
__EXPORT zMat3D *zMulMat3DMat3D(zMat3D *m1, zMat3D *m2, zMat3D *m);
__EXPORT zMat3D *zMulMat3DTMat3D(zMat3D *m1, zMat3D *m2, zMat3D *m);
__EXPORT zMat3D *zMulMat3DMat3DT(zMat3D *m1, zMat3D *m2, zMat3D *m);

#define zMulMat3DMat3DDRC(m1,m2)  zMulMat3DMat3D(m1,m2,m2)
#define zMulMat3DTMat3DDRC(m1,m2) zMulMat3DTMat3D(m1,m2,m2)
#define zMulMat3DMat3DTDRC(m1,m2) zMulMat3DMat3DT(m1,m2,m1)

__EXPORT zMat3D *zMulInvMat3DMat3D(zMat3D *m1, zMat3D *m2, zMat3D *m);

/* ********************************************************** */
/* multiplication of a 6D spatial vector by a 3x3 matrix
 * ********************************************************** */

/*! \brief multiply a 6D vector by a 3x3 matrix.
 *
 * zMulMat3DVec6D() multiplies a 6D vector \a v by a 3x3 matrix
 * \a m and puts it into \a mv.
 *
 * zMulMat3DTVec6D() multiplies a 6D vector \a v by the transpose
 * of a 3x3 matrix \a m and puts it into \a mv.
 *
 * zMulMat3DVec6DDRC() directly multiplies a 6D vector \a v by a
 * 3x3 matrix \a m.
 *
 * zMulMat3DTVec6DDRC() directly multiplies a 6D vector \a v by
 * the transpose of a 3x3 matrix \a m.
 * \return
 * Each function returns the pointer to the result vector.
 */
__EXPORT zVec6D *zMulMat3DVec6D(zMat3D *m, zVec6D *v, zVec6D *mv);
__EXPORT zVec6D *zMulMat3DTVec6D(zMat3D *m, zVec6D *v, zVec6D *mv);

#define zMulMat3DVec6DDRC(m,v)  zMulMat3DVec6D(m,v,v)
#define zMulMat3DTVec6DDRC(m,v) zMulMat3DTVec6D(m,v,v)

/* ********************************************************** */
/* rotation
 * ********************************************************** */

/*! \brief rotate a 3x3 matrix about a base in the specified axis.
 *
 * zMat3DRotRoll(), zMat3DRotPitch() and zMat3DRotYaw() rotate a 3x3 matrix
 * \a m with an angle \a angle about x-axis, y-axis and z-axis, respectively,
 * and puts it into \a rm. \a angle is in radian.
 *
 * zMat3DRotRollSC(), zMat3DRotPitchSC() and zMat3DRotYawSC() rotate a 3x3
 * matrix \a m not with an angle but with its trigonometric values.
 * \a s and \a c are for sine and cosine values, respectively.
 *
 * zMat3DRotRollDRC(), zMat3DRotPitchDRC(), zMat3DRotYawDRC(),
 * zMat3DRotRollSCDRC(), zMat3DRotPitchSCDRC() and zMat3DRotYawSCDRC() are
 * destructive versions of the above functions, directly updating a given
 * 3x3 matrix \a m.
 * \return
 * zMat3DRotRoll(), zMat3DRotPitch(), zMat3DRotYaw(), zMat3DRotRollSC(),
 * zMat3DRotPitchSC() and zMat3DRotYawSC() return a pointer \a rm.
 *
 * zMat3DRotRollDRC(), zMat3DRotPitchDRC(), zMat3DRotYawDRC()
 * zMat3DRotRollSCDRC(), zMat3DRotPitchSCDRC() and zMat3DRotYawSCDRC()
 * return a pointer \a m.
 */
__EXPORT zMat3D *zMat3DRotRoll(zMat3D *m, double angle, zMat3D *rm);
__EXPORT zMat3D *zMat3DRotPitch(zMat3D *m, double angle, zMat3D *rm);
__EXPORT zMat3D *zMat3DRotYaw(zMat3D *m, double angle, zMat3D *rm);
__EXPORT zMat3D *zMat3DRotRollSC(zMat3D *m, double s, double c, zMat3D *rm);
__EXPORT zMat3D *zMat3DRotPitchSC(zMat3D *m, double s, double c, zMat3D *rm);
__EXPORT zMat3D *zMat3DRotYawSC(zMat3D *m, double s, double c, zMat3D *rm);
__EXPORT zMat3D *zMat3DRotRollDRC(zMat3D *m, double theta);
__EXPORT zMat3D *zMat3DRotPitchDRC(zMat3D *m, double theta);
__EXPORT zMat3D *zMat3DRotYawDRC(zMat3D *m, double theta);
__EXPORT zMat3D *zMat3DRotRollSCDRC(zMat3D *m, double s, double c);
__EXPORT zMat3D *zMat3DRotPitchSCDRC(zMat3D *m, double s, double c);
__EXPORT zMat3D *zMat3DRotYawSCDRC(zMat3D *m, double s, double c);

/*! \brief 3D attitude alternation with matrix.
 *
 * zMat3DFromZYX() calculates a 3D attitude matrix from z-y-x Eulerian angles.
 * The identity matrix is rotated firstly by \a azim about z-axis, secondly
 * by \a elev about y-axis, and finally by \a tilt about x-axis.
 * The result is put into \a m.
 *
 * zMat3DFromZYXSC() directly accepts trigonometric values of z-y-x Eulerian
 * angles. \a sa/\a ca, \a se/\a ce and \a st/\a ct are for azimuth,
 * elevation and tilt angles, respectively.
 *
 * zMat3DToZYX() is the inverse transformation of zMat3DZYX() from a 3D
 * attitude matrix to a quasi 3D vector which respresents z-y-x Eulerian
 * angles \a angle in the order of azimuth, elevation and tilt. The result
 * is put into \a angle.
 *
 * zMat3DFromZYZ() calculates a 3D attitude matrix from z-y-z Eulerian angles.
 * The identity matrix is rotated firstly by \a heading about z-axis,
 * secondly by \a pitch about y-axis, and finally by \a bank about z-axis.
 * The result is put into \a m.
 *
 * zMat3DFromZYZSC() directly accepts trigonometric values of z-y-x Eulerian
 * angles. \a sh/\a ch, \a sp/\a cp and \a sb/\a cb are for heading, pitch
 * and bank angles, respectively.
 *
 * zMat3DToZYZ() is the inverse transformation of zMat3DZYZ() from a 3D
 * attitude matrix to a quasi 3D vector which respresents z-y-z Eulerian
 * angles \a angle in the order of heading, pitch and bank. The result is
 * put into \a angle.
 *
 * zMat3DFromAA() calculates a 3D attitude matrix from the equivalent angle-axis
 * vector \a aa. The identity matrix is rotated about the direction of
 * \a aa with the angle equal to the norm of \a aa. The result is put into
 * \a m.
 *
 * zMat3DToAA() is the inverse transformation of zMat3DAA() from a 3D
 * attitude matrix to the equivalent angle-axis vector. The result is put
 * into \a aa.
 * \return
 * zMat3DFromZYX(), zMat3DFromZYXSC(), zMat3DFromZYZ(), zMat3DFromZYZSC() and zMat3DFromAA()
 * return a pointer \a m.
 *
 * zMat3DToZYX() and zMat3DToZYZ() return a pointer \a angle.
 *
 * zMat3DToAA() returns a pointer \a aa.
 */
__EXPORT zMat3D *zMat3DFromZYX(zMat3D *m, double azim, double elev, double tilt);
__EXPORT zMat3D *zMat3DFromZYXSC(zMat3D *m, double sa, double ca, double se, double ce, double st, double ct);
__EXPORT zVec3D *zMat3DToZYX(zMat3D *m, zVec3D *angle);
__EXPORT zMat3D *zMat3DFromZYZ(zMat3D *m, double heading, double pitch, double bank);
__EXPORT zMat3D *zMat3DFromZYZSC(zMat3D *m, double sh, double ch, double sp, double cp, double sb, double cb);
__EXPORT zVec3D *zMat3DToZYZ(zMat3D *m, zVec3D *angle);
__EXPORT zMat3D *zMat3DFromAA(zMat3D *m, zVec3D *aa);
__EXPORT zVec3D *zMat3DToAA(zMat3D *m, zVec3D *aa);

/*! \brief rotational multiplication of a 3D attitude matrix.
 *
 * zRotMat3D() multiplies a 3x3 matrix \a m by the other \a r from the
 * leftside, then does it by the transpose of \a r from the rightside,
 * and puts the result into \a rm. Namely,
 *   \a rm = \a r \a m \a r^T.
 *
 * zRotMat3DInv() is the opposite computation of zRotMat3D(). Namely,
 *   \a rm = \a r^T \a m \a r.
 *
 * zRotMat3DDRC() is the same computation with zRotMat3D() except that
 * it puts the result directly into \a m (destructive).
 * zRotMat3DInvDRC() is the same computation with zRotMat3DInv() except
 * that it puts the result directly into \a m (destructive).
 * \return
 * Each function returns the pointer to the result.
 */
__EXPORT zMat3D *zRotMat3D(zMat3D *r, zMat3D *m, zMat3D *rm);
__EXPORT zMat3D *zRotMat3DInv(zMat3D *r, zMat3D *m, zMat3D *rm);

#define zRotMat3DDRC(r,m)    zRotMat3D(r,m,m)
#define zRotMat3DInvDRC(r,m) zRotMat3DInv(r,m,m)

/*! \brief rotate a 3D attitude matrix about an arbitrary axis.
 *
 * zMat3DRot() rotates a 3D attitude matrix \a m by an angle-axis vector
 * \a aa. The rotation axis is in parallel to \a aa and the norm of \a aa
 * is the rotation angle in radian. The direction of rotation is according
 * to right-handed screw rule. The result is put into \a rm.
 *
 * zMat3DRotDRC() directly rotates a 3D attitude matrix \a m by an angle-axis
 * vector \a aa.
 * \return
 * zMat3DRot() returns a pointer \a rm.
 * zMat3DRotDRC() returns a pointer \a m.
 */
__EXPORT zMat3D *zMat3DRot(zMat3D *m, zVec3D *aa, zMat3D *rm);

#define zMat3DRotDRC(m,aa) zMat3DRot( m, aa, m )

__EXPORT zMat3D *zMat3DRotCat(zMat3D *m, zVec3D *omega, double dt, zMat3D *rm);

/*! \brief cascade an angle-axis vector to another.
 */
__EXPORT zVec3D *zAACascade(zVec3D *aa1, zVec3D *aa2, zVec3D *aa);

/*! \brief error vector between two 3D attitude matrices.
 *
 * zMat3DError() calculates the error vector, namely, the equivalent
 * angle-axis vector from a 3D attitude matrix \a m2 to the other \a m1
 * (notice the order) and puts it into \a err.
 * i.e., R(\a err) \a m2 = \a m1.
 * \return
 * zMat3DError() returns a pointer \a err.
 */
__EXPORT zVec3D *zMat3DError(zMat3D *m1, zMat3D *m2, zVec3D *err);

/*! \brief error between two angle-axis vectors.
 *
 * zAAError() calculates the error vector, namely, the angle-axis vector
 * from an attitude represented by \a a2 to another \a a1 (note the order).
 * The result is put into \a err.
 * \return
 * zAAError() returns a pointer \a err.
 */
__EXPORT zVec3D *zAAError(zVec3D *a1, zVec3D *a2, zVec3D *err);

/* ********************************************************** */
/* differential kinematics
 * ********************************************************** */

__EXPORT zVec3D *zMat3DDif(zMat3D *m, zMat3D *mnew, double dt, zVec3D *omega);

/* ********************************************************** */
/* eigensystem
 * ********************************************************** */

/*! \brief eigensystem of a symmetric 3x3 matrix by Jacobi's method.
 *
 * zMat3DSymEig() calculates eigenvalues and eigenvectors of a symmetric
 * 3x3 matrix \a m with Jacobi's method. Each eigenvalue and eigenvector
 * are stored in \a eval and \a evec in the corresponding order.
 * \return
 * zMat3DSymEig() returns no value.
 * \notes
 * \a m must be symmetric. Otherwise, the correct result will not be expected.
 */
__EXPORT void zMat3DSymEig(zMat3D *m, double eval[], zVec3D evec[]);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 3x3 matrix from a ZTK format processor. */
__EXPORT zMat3D *zMat3DFromZTK(zMat3D *m, ZTK *ztk);

/*! \brief scan and print a 3x3 matrix.
 *
 * zMat3DFScan() scans nine values from the current position of a file
 * \a fp and creates a 3x3 matrix \a m from them.
 * zMat3DScan() scans nine values from the standard input.
 *
 * zMat3DFPrint() prints a 3x3 matrix \a m out to the current position of
 * a file \a fp in the following format.
 *  {
 *   a11, a12, a13
 *   a21, a22, a23
 *   a31, a32, a33
 *  }
 * When the null pointer is given, the following string is printed.
 *  (null 3x3 matrix)
 * zMat3DPrint() prints a 3x3 matrix \a m out to the standard output.
 * \return
 * zMat3DFScan() and zMat3DScan() return a pointer \a m.
 *
 * zMat3DFPrint() and zMat3DPrint() return no value.
 */
__EXPORT zMat3D *zMat3DFScan(FILE *fp, zMat3D *m);
#define zMat3DScan(m) zMat3DFScan( stdin, (m) )
__EXPORT void zMat3DFPrint(FILE *fp, zMat3D *m);
#define zMat3DPrint(m) zMat3DFPrint( stdout, (m) )

/*! \brief read a rotation axis and angle from a ZTK format processor and make a 3x3 matrix.
 *
 * zAAFromZTK() reads a 3D vector and a scalar from a ZTK format
 * processor \a ztk, and creates an angle-axis vector from them.
 * \a aa is a pointer to the 3D vector in which the angle-axis
 * vector is stored.
 * \return
 * zAAFromZTK() returns the result angle-axis vector.
 * \sa
 * zMat3DFromZTK
 */
__EXPORT zVec3D *zAAFromZTK(zVec3D *aa, ZTK *ztk);

__END_DECLS

#include <zeo/zeo_vec3d_pca.h> /* principal component analysis */

#endif /* __ZEO_MAT3D_H__ */
