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
ZDEF_UNION( __ZEO_CLASS_EXPORT, zMat3D ){
  double e[3][3];   /*!< 3x3 matrix */
  zVec3D v[3];      /*!< 3 column vectors */
  struct{
    zVec3D x, y, z;
  } b;              /*!< x-y-z base vectors */
  struct{
    double xx, xy, xz, yx, yy, yz, zx, zy, zz;
  } c;             /*!< 9 components */
#ifdef __cplusplus
  zVec3D &vec(zAxis axis);
  zMat3D &create(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33);
  zMat3D &copy(const zMat3D &m);
  zMat3D &zero();
  zMat3D &ident();
  bool isEqual(const zMat3D &m);
  bool isIdent();
  bool isTol(double tol);
  bool isTiny();
  zVec3D &row(int i, zVec3D &v);
  zVec3D &col(int i, zVec3D &v);
  zVec3D row(int i);
  zVec3D col(int i);
  zMat3D &transpose(zMat3D &m);
  zMat3D transpose();
  zMat3D &transposeDirect();
  zMat3D operator+(const zMat3D &m);
  zMat3D operator-(const zMat3D &m);
  zMat3D operator-();
  zMat3D operator*(double k);
  zMat3D operator/(double k);
  zMat3D &operator+=(const zMat3D &m);
  zMat3D &operator-=(const zMat3D &m);
  zMat3D &operator*=(double k);
  zMat3D &operator/=(double k);
  zMat3D &createDyad(const zVec3D &v1, const zVec3D &v2);
  zMat3D &createOuterprod(const zVec3D &v);
  zMat3D &createTripleprod(const zVec3D &v1, const zVec3D &v2);
  zMat3D &createDoubleOuterprod(const zVec3D &v);
  zMat3D &orthonormalize(zAxis axis1, zAxis axis2);
  double squareNorm();
  double norm();
  double det();
  zMat3D &inv(zMat3D &m);
  zMat3D inv();
  zVec3D operator*(const zVec3D &v);
  zVec3D mulT(const zVec3D &v);
  zVec3D mulInv(const zVec3D &v);
  zMat3D operator*(const zMat3D &m);
  zMat3D mulT(const zMat3D &m);
  zMat3D mulInv(const zMat3D &m);
  zVec6D operator*(const zVec6D &v);
  zVec6D mulT(const zVec6D &v);
  zMat3D &rotRoll(double angle);
  zMat3D &rotPitch(double angle);
  zMat3D &rotYaw(double angle);
  zMat3D &createFromZYX(double azim, double elev, double tilt);
  zMat3D &createFromZYZ(double heading, double pitch, double bank);
  zMat3D &createFromAA(const zVec3D &aa);
  zMat3D &rot(const zMat3D &r);
  zMat3D &rotInv(const zMat3D &r);
  zMat3D &rot(const zVec3D &aa);
  static const zMat3D zmat3Dzero;
  static const zMat3D zmat3Dident;
#endif /* __cplusplus */
};

/*! \brief a column vector of a 3x3 matrix. */
#define zMat3DVec(mat,axis) ( &(mat)->v[(axis)] )

/*! \brief 3D zero matrix and identity matrix */
#ifdef __cplusplus
#define ZMAT3DZERO  ( (zMat3D *)&zMat3D::zmat3Dzero )
#define ZMAT3DIDENT ( (zMat3D *)&zMat3D::zmat3Dident )
#else
__ZEO_EXPORT const zMat3D zmat3Dzero;
__ZEO_EXPORT const zMat3D zmat3Dident;
#define ZMAT3DZERO  ( (zMat3D *)&zmat3Dzero )
#define ZMAT3DIDENT ( (zMat3D *)&zmat3Dident )
#endif /* __cplusplus */

/*! \brief create, copy and cleanup a 3x3 matrix.
 *
 * zMat3DCreate() creates a 3x3 matrix \a mat from nine values as follows.
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
#define _zMat3DCreate(mat,a11,a12,a13,a21,a22,a23,a31,a32,a33) do{\
  _zVec3DCreate( &(mat)->b.x, a11, a21, a31 );\
  _zVec3DCreate( &(mat)->b.y, a12, a22, a32 );\
  _zVec3DCreate( &(mat)->b.z, a13, a23, a33 );\
} while(0)
__ZEO_EXPORT zMat3D *zMat3DCreate(zMat3D *mat,
  double a11, double a12, double a13,
  double a21, double a22, double a23,
  double a31, double a32, double a33);
#define zMat3DCopy(src,dest) zCopy( zMat3D, src, dest )
#define zMat3DZero(mat)   zMat3DCopy( ZMAT3DZERO, mat )
#define zMat3DIdent(mat)  zMat3DCopy( ZMAT3DIDENT, mat )

/*! \brief check if two 3x3 matrices are equal.
 *
 * zMat3DMatch() and zMat3DEqual() check if two 3x3 matrices \a mat1 and \a mat2 are equal.
 * They return a boolean value.
 *
 * zMat3DMatch() strictly compares the two matrices, while zMat3DEqual() checks if the error between
 * \a mat1 and \a mat2 are sufficiently small.
 * \return
 * zMat3DMatch() and zMat3DEqual() return the true value if \a mat1 and \a mat2 are equal, or false otherwise.
 */
#define _zMat3DMatch(mat1,mat2) ( _zVec3DMatch(&(mat1)->b.x,&(mat2)->b.x) && _zVec3DMatch(&(mat1)->b.y,&(mat2)->b.y) && _zVec3DMatch(&(mat1)->b.z,&(mat2)->b.z) )
__ZEO_EXPORT bool zMat3DMatch(const zMat3D *mat1, const zMat3D *mat2);
#define _zMat3DEqual(mat1,mat2) ( _zVec3DEqual(&(mat1)->b.x,&(mat2)->b.x) && _zVec3DEqual(&(mat1)->b.y,&(mat2)->b.y) && _zVec3DEqual(&(mat1)->b.z,&(mat2)->b.z) )
__ZEO_EXPORT bool zMat3DEqual(const zMat3D *mat1, const zMat3D *mat2);

/*! \brief check if a 3x3 matrix is the identity matrix. */
#define _zMat3DIsIdent(mat) _zMat3DEqual( mat, ZMAT3DIDENT )
__ZEO_EXPORT bool zMat3DIsIdent(const zMat3D *mat);

/*! \brief check if a 3x3 matrix is tiny.
 *
 * zMat3DIsTol() checks if the absolute values of all components of a 3x3 matrix \a mat are smaller than \a tol.
 *
 * zMat3DIsTiny() applies zTOL (defined in zm_misc.h) to the tolerance of zMat3DIsTol().
 * \return
 * zMat3DIsTol() and zMat3DIsTiny() return the true value if the absolute values of all components of \a mat
 * are smaller than \a tol and zTOL, respectively, or the false value, otherwise.
 * \notes
 * \a tol must be positive.
 * \sa
 * zIsTol, zIsTiny
 */
#define _zMat3DIsTol(mat,tol) ( _zVec3DIsTol( &(mat)->b.x, tol ) && _zVec3DIsTol( &(mat)->b.y, tol ) && _zVec3DIsTol( &(mat)->b.z, tol ) )
__ZEO_EXPORT bool zMat3DIsTol(const zMat3D *mat, double tol);
#define _zMat3DIsTiny(mat) _zMat3DIsTol( mat, zTOL )
#define zMat3DIsTiny(mat)  zMat3DIsTol( mat, zTOL )

/*! \brief abstract row/column vectors from a 3x3 matrix.
 *
 * zMat3DRow() abstracts the \a i th row from a 3x3 matrix \a mat and puts it into \a vec.
 *
 * zMat3DCol() abstracts the \a i th column from a 3x3 matrix \a mat and puts it into \a vec.
 * \return
 * zMat3DRow() and zMat3DCol() return a pointer \a vec.
 */
#define _zMat3DRow(mat,i,vec) _zVec3DCreate( (vec), (mat)->e[0][(i)], (mat)->e[1][(i)], (mat)->e[2][(i)] )
__ZEO_EXPORT zVec3D *zMat3DRow(const zMat3D *mat, int i, zVec3D *vec);
#define zMat3DCol(mat,i,vec)  zVec3DCopy( zMat3DVec(mat,i), vec )

/*! \brief transpose of a 3x3 matrix.
 *
 * zMat3DT() transposes a 3x3 matrix \a mat, and puts it into \a tmat.
 *
 * zMat3DTDRC() directly transposes  3x3 matrix \a mat.
 * \return
 * zMat3DT() returns a pointer \a tmat.
 * zMat3DTDRC() returns a pointer \a mat.
 * \notes
 * It is not allowed to let \a tmat point to the same address with \a mat.
 * When \a tmat is the same with \a mat, anything might happen.
 */
#define _zMat3DT(mat,tmat) do{\
  _zMat3DRow( mat, 0, &(tmat)->b.x );\
  _zMat3DRow( mat, 1, &(tmat)->b.y );\
  _zMat3DRow( mat, 2, &(tmat)->b.z );\
} while(0)
#define _zMat3DTDRC(mat) do{\
  double __tmp;\
  __tmp = (mat)->e[1][0]; (mat)->e[1][0] = (mat)->e[0][1]; (mat)->e[0][1] = __tmp;\
  __tmp = (mat)->e[2][0]; (mat)->e[2][0] = (mat)->e[0][2]; (mat)->e[0][2] = __tmp;\
  __tmp = (mat)->e[2][1]; (mat)->e[2][1] = (mat)->e[1][2]; (mat)->e[1][2] = __tmp;\
} while(0)
__ZEO_EXPORT zMat3D *zMat3DT(const zMat3D *mat, zMat3D *tmat);
__ZEO_EXPORT zMat3D *zMat3DTDRC(zMat3D *mat);

/*! \brief symmetrize a 3x3 matrix. */
#define _zMat3DSymmetrize(src,dest) do{\
  (dest)->c.xx = (src)->c.xx; \
  (dest)->c.yy = (src)->c.yy; \
  (dest)->c.zz = (src)->c.zz; \
  (dest)->c.xy = (dest)->c.yx = 0.5 * ( (src)->c.xy + (src)->c.yx ); \
  (dest)->c.yz = (dest)->c.zy = 0.5 * ( (src)->c.yz + (src)->c.zy ); \
  (dest)->c.zx = (dest)->c.xz = 0.5 * ( (src)->c.zx + (src)->c.xz ); \
} while(0)
__ZEO_EXPORT zMat3D *zMat3DSymmetrize(const zMat3D *src, zMat3D *dest);

#define zMat3DSymmetrizeDRC(mat) zMat3DSymmetrize( mat, mat )

#define _zMat3DIsSymmetric(mat) ( (mat)->c.xy == (mat)->c.yx && (mat)->c.yz == (mat)->c.zy && (mat)->c.zx == (mat)->c.xz )
/*! \brief check if a matrix is square and symmetric. */
__ZEO_EXPORT bool zMat3DIsSymmetric(const zMat3D *mat);

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
__ZEO_EXPORT zMat3D *zMat3DAdd(const zMat3D *m1, const zMat3D *m2, zMat3D *m);
#define _zMat3DSub(m1,m2,m) do{\
  _zVec3DSub( &(m1)->b.x, &(m2)->b.x, &(m)->b.x );\
  _zVec3DSub( &(m1)->b.y, &(m2)->b.y, &(m)->b.y );\
  _zVec3DSub( &(m1)->b.z, &(m2)->b.z, &(m)->b.z );\
} while(0)
__ZEO_EXPORT zMat3D *zMat3DSub(const zMat3D *m1, const zMat3D *m2, zMat3D *m);
#define _zMat3DRev(m,rm) do{\
  _zVec3DRev( &(m)->b.x, &(rm)->b.x );\
  _zVec3DRev( &(m)->b.y, &(rm)->b.y );\
  _zVec3DRev( &(m)->b.z, &(rm)->b.z );\
} while(0)
__ZEO_EXPORT zMat3D *zMat3DRev(const zMat3D *m, zMat3D *rm);
#define _zMat3DMul(m,k,mm) do{\
  _zVec3DMul( &(m)->b.x, k, &(mm)->b.x );\
  _zVec3DMul( &(m)->b.y, k, &(mm)->b.y );\
  _zVec3DMul( &(m)->b.z, k, &(mm)->b.z );\
} while(0)
__ZEO_EXPORT zMat3D *zMat3DMul(const zMat3D *m, double k, zMat3D *mm);
__ZEO_EXPORT zMat3D *zMat3DDiv(const zMat3D *m, double k, zMat3D *dm);
#define _zMat3DCat(m1,k,m2,m) do{\
  _zVec3DCat( &(m1)->b.x, k, &(m2)->b.x, &(m)->b.x );\
  _zVec3DCat( &(m1)->b.y, k, &(m2)->b.y, &(m)->b.y );\
  _zVec3DCat( &(m1)->b.z, k, &(m2)->b.z, &(m)->b.z );\
} while(0)
__ZEO_EXPORT zMat3D *zMat3DCat(const zMat3D *m1, double k, const zMat3D *m2, zMat3D *m);

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
__ZEO_EXPORT zMat3D *zMat3DDyad(zMat3D *m, const zVec3D *v1, const zVec3D *v2);

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
__ZEO_EXPORT zMat3D *zMat3DAddDyad(zMat3D *m, const zVec3D *v1, const zVec3D *v2);

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
__ZEO_EXPORT zMat3D *zMat3DSubDyad(zMat3D *m, const zVec3D *v1, const zVec3D *v2);

/*! \brief create a 3x3 matrix equivalent to outer product of 3D vectors.
 *
 * zVec3DOuterProd2Mat3D() computes the outer-product matrix of a 3D vector
 * \a v and puts it into \a m. Namely, \a m a is equivalent with \a v x a
 * with respect to an arbitrary 3D vector a. \a m becomes skew-symmetric.
 *
 * zVec3DTripleProd2Mat3D() computes the triple product matrix of a 3D vector
 * \a v1 and \a v2 and puts it into \a m. Namely, \a m a is equivalent with
 * \a v1 x ( \a v2 x a ) ) with respect to an arbitrary 3D vector a.
 *
 * zVec3DDoubleOuterProd2Mat3D() computes the triple product matrix of a 3D
 * vector \a v and puts it into \a m. Namely, the result is equivalent with
 * zVec3DOuterProd2Mat3D( \a v, \a v, \a m ).
 * \return
 * zVec3DOuterProd2Mat3D(), zVec3DTripleProd2Mat3D() and zVec3DDoubleOuterProd2Mat3D()
 * return a pointer \a m.
 */
#define _zVec3DOuterProd2Mat3D(vo,m) \
  _zMat3DCreate( m,\
    0.0,-(vo)->c.z, (vo)->c.y,\
    (vo)->c.z, 0.0,-(vo)->c.x,\
   -(vo)->c.y, (vo)->c.x, 0.0 )
__ZEO_EXPORT zMat3D *zVec3DOuterProd2Mat3D(const zVec3D *v, zMat3D *m);
#define _zVec3DTripleProd2Mat3D(v1,v2,m) \
  _zMat3DCreate( m,\
    -(v1)->c.y*(v2)->c.y-(v1)->c.z*(v2)->c.z, (v1)->c.y*(v2)->c.x, (v1)->c.z*(v2)->c.x,\
     (v1)->c.x*(v2)->c.y,-(v1)->c.z*(v2)->c.z-(v1)->c.x*(v2)->c.x, (v1)->c.z*(v2)->c.y,\
     (v1)->c.x*(v2)->c.z, (v1)->c.y*(v2)->c.z,-(v1)->c.x*(v2)->c.x-(v1)->c.y*(v2)->c.y )
__ZEO_EXPORT zMat3D *zVec3DTripleProd2Mat3D(const zVec3D *v1, const zVec3D *v2, zMat3D *m);
#define _zVec3DDoubleOuterProd2Mat3D(v,m) do{\
  double __xx, __yy, __zz;\
  __xx = (v)->c.x * (v)->c.x;\
  __yy = (v)->c.y * (v)->c.y;\
  __zz = (v)->c.z * (v)->c.z;\
  (m)->c.xx = -__yy-__zz;\
  (m)->c.yy = -__zz-__xx;\
  (m)->c.zz = -__xx-__yy;\
  (m)->c.xy = (m)->c.yx = (v)->c.x*(v)->c.y;\
  (m)->c.yz = (m)->c.zy = (v)->c.y*(v)->c.z;\
  (m)->c.zx = (m)->c.xz = (v)->c.z*(v)->c.x;\
} while(0)
__ZEO_EXPORT zMat3D *zVec3DDoubleOuterProd2Mat3D(const zVec3D *v, zMat3D *m);

/*! \brief concatenate a double outer-product matrix of a 3D vector to another matrix. */
#define _zMat3DCatVec3DDoubleOuterProd(m,k,v,dm) do{\
  double __kxx, __kyy, __kzz, __kxy, __kyz, __kzx;\
  __kxx = (k) * (v)->c.x * (v)->c.x;\
  __kyy = (k) * (v)->c.y * (v)->c.y;\
  __kzz = (k) * (v)->c.z * (v)->c.z;\
  __kxy = (k) * (v)->c.x * (v)->c.y;\
  __kyz = (k) * (v)->c.y * (v)->c.z;\
  __kzx = (k) * (v)->c.z * (v)->c.x;\
  (dm)->c.xx = (m)->c.xx - __kyy - __kzz;\
  (dm)->c.yy = (m)->c.yy - __kzz - __kxx;\
  (dm)->c.zz = (m)->c.zz - __kxx - __kyy;\
  (dm)->c.xy = (m)->c.xy + __kxy;\
  (dm)->c.yx = (m)->c.yx + __kxy;\
  (dm)->c.yz = (m)->c.yz + __kyz;\
  (dm)->c.zy = (m)->c.zy + __kyz;\
  (dm)->c.zx = (m)->c.zx + __kzx;\
  (dm)->c.xz = (m)->c.xz + __kzx;\
} while(0)
__ZEO_EXPORT zMat3D *zMat3DCatVec3DDoubleOuterProd(const zMat3D *m, double k, const zVec3D *v, zMat3D *dm);
#define _zMat3DCatVec3DDoubleOuterProdDRC(m,k,v) _zMat3DCatVec3DDoubleOuterProd(m,k,v,m)
#define zMat3DCatVec3DDoubleOuterProdDRC(m,k,v) zMat3DCatVec3DDoubleOuterProd(m,k,v,m)

/*! \brief multiply cross product of a 3D vector and a 3x3 matrix.
 *
 * zMulVec3DOuterProdMat3D() multiplies a 3x3 matrix \a m by the outer
 * product of a 3D vector \a ohm and puts it into \a mv. Namely,
 *   \a mv = \a ohm x \a m.
 *
 * zMulVec3DOuterProdMat3DDRC() directly multiplies a 3x3 matrix \a m
 * by the outer product of a 3D vector \a ohm.
 * \return
 * zMulVec3DOuterProdMat3D() returns a pointer \a mv.
 * zMulVec3DOuterProdMat3DDRC() returns a pointer \a m.
 */
__ZEO_EXPORT zMat3D *zMulVec3DOuterProdMat3D(const zVec3D *ohm, const zMat3D *m, zMat3D *mv);
#define zMulVec3DOuterProdMat3DDRC(o,m) zMulVec3DOuterProdMat3D( o, m, m )

/*! \brief calculate norm of a 3x3 matrix.
 *
 * zMat3DNorm() calculates a norm of a 3x3 matrix \a m.
 * zMat3DSqrNorm() calculates a squared norm of \a m.
 * \return
 * zMat3DNorm() returns a norm of \a m.
 * zMat3DSqrNorm() returns a squared norm of \a m.
 */
#define _zMat3DSqrNorm(m) ( _zVec3DSqrNorm(&(m)->b.x) + _zVec3DSqrNorm(&(m)->b.y) + _zVec3DSqrNorm(&(m)->b.z) )
__ZEO_EXPORT double zMat3DSqrNorm(const zMat3D *m);
#define _zMat3DNorm(m) sqrt( _zMat3DSqrNorm(m) )
#define zMat3DNorm(m) sqrt( zMat3DSqrNorm(m) )

/*! \brief check if a 3x3 matrix is orthonormal. */
__ZEO_EXPORT bool zMat3DIsOrthonormal(const zMat3D *m);
/*! \brief check if a 3x3 matrix is a right-hand system. */
__ZEO_EXPORT bool zMat3DIsRightHand(const zMat3D *m);

/*! \brief orthonormalize a 3x3 matrix. */
__ZEO_EXPORT zMat3D *zMat3DOrthonormalize(const zMat3D *src, zAxis axis1, zAxis axis2, zMat3D *dest);
#define zMat3DOrthonormalizeDRC(m,axis1,axis2) zMat3DOrthonormalize(m,axis1,axis2,m)

/* ********************************************************** */
/* adjugate & inverse of a 3x3 matrix
 * ********************************************************** */

/* adjugate matrix of a 3x3 matrix. */
__ZEO_EXPORT zMat3D *zMat3DAdj(const zMat3D *mat, zMat3D *adj_mat);

/*! \brief determinant of a 3x3 matrix.
 *
 * zMat3DDet() computes the determinant of an arbitrary 3x3 matrix \a mat.
 * \retval the determinant of \a mat
 */
#define _zMat3DDet(mat) ( ( (mat)->c.yy*(mat)->c.zz - (mat)->c.yz*(mat)->c.zy ) * (mat)->c.xx \
                        + ( (mat)->c.xz*(mat)->c.zy - (mat)->c.xy*(mat)->c.zz ) * (mat)->c.yx \
                        + ( (mat)->c.xy*(mat)->c.yz - (mat)->c.xz*(mat)->c.yy ) * (mat)->c.zx )
__ZEO_EXPORT double zMat3DDet(const zMat3D *mat);

/*! \brief inverse of a 3x3 matrix.
 *
 * zMat3DInv() computes the inverse matrix of an arbitrary 3x3 matrix \a mat, and puts it into \a inv_mat.
* It does not assume that \a mat is an orthonormal matrix.
 * \return
 * zMat3DInv() returns a pointer \a inv_mat, if it succeeds.
 * If \a mat is singular, it returns the null pointer.
 * \notes
 * \a inv_mat has to point to a different address from \a mat. If \a inv_mat is the same with \a mat,
 * anything might happen.
 */
__ZEO_EXPORT zMat3D *zMat3DInv(const zMat3D *mat, zMat3D *inv_mat);

/* ********************************************************** */
/* multiplication of a 3D vector by a 3x3 matrix
 * ********************************************************** */

/*! \brief multiply a 3x3 matrix and a 3D vector.
 *
 * zMulMat3DVec3D() multiplies a 3D vector \a v by a 3x3 matrix \a m and puts it into \a mv.
 *
 * zMulMat3DTVec3D() multiplies a 3D vector \a v by the transpose matrix of a 3x3 matrix \a m
 * and puts it into \a mv.
 *
 * zMulMat3DVec3DDRC() directly multiplies a 3D vector \a v by a 3x3 matrix \a m.
 *
 * zMulMat3DTVec3DDRC() directly multiplies a 3D vector v by the transpose of a 3x3 matrix \a m.
 * \return
 * zMulMat3DVec3D() and zMulMat3DTVec3D() return a pointer \a mv.
 * zMulMat3DVec3DDRC() and zMulMat3DTVec3DDRC() return a pointer \a v.
 */
#define _zMulMat3DVec3D(m,v,mv) do{\
  double __x, __y, __z;\
  __x = (m)->c.xx*(v)->c.x + (m)->c.yx*(v)->c.y + (m)->c.zx*(v)->c.z;\
  __y = (m)->c.xy*(v)->c.x + (m)->c.yy*(v)->c.y + (m)->c.zy*(v)->c.z;\
  __z = (m)->c.xz*(v)->c.x + (m)->c.yz*(v)->c.y + (m)->c.zz*(v)->c.z;\
  _zVec3DCreate( mv, __x, __y, __z );\
} while(0)
__ZEO_EXPORT zVec3D *zMulMat3DVec3D(const zMat3D *m, const zVec3D *v, zVec3D *mv);
#define _zMulMat3DTVec3D(m,v,mv) do{\
  double __x, __y, __z;\
  __x = _zVec3DInnerProd( &(m)->b.x, v );\
  __y = _zVec3DInnerProd( &(m)->b.y, v );\
  __z = _zVec3DInnerProd( &(m)->b.z, v );\
  _zVec3DCreate( mv, __x, __y, __z );\
} while(0)
__ZEO_EXPORT zVec3D *zMulMat3DTVec3D(const zMat3D *m, const zVec3D *v, zVec3D *mv);

/*! \brief directly multiply a 3D vector by a 3x3 matrix. */
#define _zMulMat3DVec3DDRC(m,v) _zMulMat3DVec3D(m,v,v)
__ZEO_EXPORT zVec3D *zMulMat3DVec3DDRC(const zMat3D *m, zVec3D *v);

/*! \brief directly multiply a 3D vector by transpose of a 3x3 matrix. */
#define _zMulMat3DTVec3DDRC(m,v) _zMulMat3DTVec3D(m,v,v)
__ZEO_EXPORT zVec3D *zMulMat3DTVec3DDRC(const zMat3D *m, zVec3D *v);

/*! \brief multiply a 3D vector by adjugate of a 3x3 matrix.
 *
 * zMulInvMat3DVec3D() multiplies a 3D vector \a vec by the adjugate of a 3x3 matrix \a mat,
 * and puts it into \a adj_vec.
 * \return
 * zMulInvMat3DVec3D() returns a pointer \a adj_vec.
 */
__ZEO_EXPORT zVec3D *zMulAdjMat3DVec3D(const zMat3D *mat, const zVec3D *vec, zVec3D *adj_vec);

/*! \brief multiply a 3D vector by inverse of a 3x3 matrix.
 *
 * zMulInvMat3DVec3D() multiplies a 3D vector \a vec by the inverse of a 3x3 matrix \a mat,
 * and puts it into \a inv_vec.
 * \return
 * zMulInvMat3DVec3D() returns a pointer \a inv_vec, if \a mat is regular. If \a mat is not regular,
 * it returns the null pointer.
 */
__ZEO_EXPORT zVec3D *zMulInvMat3DVec3D(const zMat3D *mat, const zVec3D *vec, zVec3D *inv_vec);

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
__ZEO_EXPORT double *zVec3DCatRatio(const zVec3D *v1, const zVec3D *v2, const zVec3D *v3, const zVec3D *v, double ratio[]);

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
 * \return
 * Each function returns a pointer to the result.
 */
__ZEO_EXPORT zMat3D *zMulMat3DMat3D(const zMat3D *m1, const zMat3D *m2, zMat3D *m);
__ZEO_EXPORT zMat3D *zMulMat3DTMat3D(const zMat3D *m1, const zMat3D *m2, zMat3D *m);
__ZEO_EXPORT zMat3D *zMulMat3DMat3DT(const zMat3D *m1, const zMat3D *m2, zMat3D *m);

#define zMulMat3DMat3DDRC(m1,m2)  zMulMat3DMat3D(m1,m2,m2)
#define zMulMat3DTMat3DDRC(m1,m2) zMulMat3DTMat3D(m1,m2,m2)
#define zMulMat3DMat3DTDRC(m1,m2) zMulMat3DMat3DT(m1,m2,m1)

/*! \brief multiply a 3x3 matrix by adjugate of another 3x3 matrix.
 *
 * zMulAdjMat3DMat3D() multiplies a 3x3 matrix \a m2 by the adjugate of the other 3x3 matrix \a m1
 * from the leftside, and puts it into \a m.
 * \return
 * zMulAdjMat3DMat3D() returns the pointer \a m.
 */
__ZEO_EXPORT zMat3D *zMulAdjMat3DMat3D(const zMat3D *m1, const zMat3D *m2, zMat3D *m);

/*! \brief multiply the inverse of a 3x3 matrix to another.
 *
 * zMulInvMat3DMat3D() multiplies a 3x3 matrix \a m2 by the inverse of the other 3x3 matrix \a m1
 * from the leftside, and puts it into \a m.
 * \return
 * zMulInvMat3DMat3D() returns the pointer \a m, if \a m1 is regular.
 * If \a m1 is not regular, it returns the null pointer.
 */
__ZEO_EXPORT zMat3D *zMulInvMat3DMat3D(const zMat3D *m1, const zMat3D *m2, zMat3D *m);

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
__ZEO_EXPORT zVec6D *zMulMat3DVec6D(const zMat3D *m, const zVec6D *v, zVec6D *mv);
__ZEO_EXPORT zVec6D *zMulMat3DTVec6D(const zMat3D *m, const zVec6D *v, zVec6D *mv);

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
__ZEO_EXPORT zMat3D *zMat3DRotRoll(const zMat3D *m, double angle, zMat3D *rm);
__ZEO_EXPORT zMat3D *zMat3DRotPitch(const zMat3D *m, double angle, zMat3D *rm);
__ZEO_EXPORT zMat3D *zMat3DRotYaw(const zMat3D *m, double angle, zMat3D *rm);
__ZEO_EXPORT zMat3D *zMat3DRotRollSC(const zMat3D *m, double s, double c, zMat3D *rm);
__ZEO_EXPORT zMat3D *zMat3DRotPitchSC(const zMat3D *m, double s, double c, zMat3D *rm);
__ZEO_EXPORT zMat3D *zMat3DRotYawSC(const zMat3D *m, double s, double c, zMat3D *rm);
__ZEO_EXPORT zMat3D *zMat3DRotRollDRC(zMat3D *m, double theta);
__ZEO_EXPORT zMat3D *zMat3DRotPitchDRC(zMat3D *m, double theta);
__ZEO_EXPORT zMat3D *zMat3DRotYawDRC(zMat3D *m, double theta);
__ZEO_EXPORT zMat3D *zMat3DRotRollSCDRC(zMat3D *m, double s, double c);
__ZEO_EXPORT zMat3D *zMat3DRotPitchSCDRC(zMat3D *m, double s, double c);
__ZEO_EXPORT zMat3D *zMat3DRotYawSCDRC(zMat3D *m, double s, double c);

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
__ZEO_EXPORT zMat3D *zMat3DFromZYX(zMat3D *m, double azim, double elev, double tilt);
__ZEO_EXPORT zMat3D *zMat3DFromZYXSC(zMat3D *m, double sa, double ca, double se, double ce, double st, double ct);
__ZEO_EXPORT zVec3D *zMat3DToZYX(const zMat3D *m, zVec3D *angle);
__ZEO_EXPORT zMat3D *zMat3DFromZYZ(zMat3D *m, double heading, double pitch, double bank);
__ZEO_EXPORT zMat3D *zMat3DFromZYZSC(zMat3D *m, double sh, double ch, double sp, double cp, double sb, double cb);
__ZEO_EXPORT zVec3D *zMat3DToZYZ(const zMat3D *m, zVec3D *angle);
__ZEO_EXPORT zMat3D *zMat3DFromAA(zMat3D *m, const zVec3D *aa);
__ZEO_EXPORT zVec3D *zMat3DToAA(const zMat3D *m, zVec3D *aa);

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
__ZEO_EXPORT zMat3D *zRotMat3D(const zMat3D *r, const zMat3D *m, zMat3D *rm);
__ZEO_EXPORT zMat3D *zRotMat3DInv(const zMat3D *r, const zMat3D *m, zMat3D *rm);

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
__ZEO_EXPORT zMat3D *zMat3DRot(const zMat3D *m, const zVec3D *aa, zMat3D *rm);

#define zMat3DRotDRC(m,aa) zMat3DRot( m, aa, m )

__ZEO_EXPORT zMat3D *zMat3DRotCat(const zMat3D *m, const zVec3D *omega, double dt, zMat3D *rm);

/*! \brief cascade an angle-axis vector to another.
 */
__ZEO_EXPORT zVec3D *zAACascade(const zVec3D *aa1, const zVec3D *aa2, zVec3D *aa);

/*! \brief error vector between two 3D attitude matrices.
 *
 * zMat3DError() calculates the error vector, namely, the equivalent
 * angle-axis vector from a 3D attitude matrix \a m2 to the other \a m1
 * (notice the order) and puts it into \a err.
 * i.e., R(\a err) \a m2 = \a m1.
 * \return
 * zMat3DError() returns a pointer \a err.
 */
__ZEO_EXPORT zVec3D *zMat3DError(const zMat3D *m1, const zMat3D *m2, zVec3D *err);

/*! \brief error between two angle-axis vectors.
 *
 * zAAError() calculates the error vector, namely, the angle-axis vector
 * from an attitude represented by \a a2 to another \a a1 (note the order).
 * The result is put into \a err.
 * \return
 * zAAError() returns a pointer \a err.
 */
__ZEO_EXPORT zVec3D *zAAError(const zVec3D *a1, const zVec3D *a2, zVec3D *err);

/* ********************************************************** */
/* differential kinematics
 * ********************************************************** */

__ZEO_EXPORT zVec3D *zMat3DDif(const zMat3D *m, const zMat3D *mnew, double dt, zVec3D *omega);

/* ********************************************************** */
/* eigensystem and singular value decomposition
 * ********************************************************** */

/*! \def component identifier corresponding to the minimum eigenvalue. */
#define _zMat3DEigMinID( val )  ( ( val[0] < val[1] ) ? ( val[0] < val[2] ? 0 : 2 ) : ( val[1] < val[2] ? 1 : 2 ) )

/*! \brief calculate the dominant eigenvalue.
 *
 * zMat3DEigPower() computes the largest eigenvalue and the corresponding eigenvector of a 3x3 matrix
 * \a m by power method. The eigenvector is stored in \a eigvec.
 * \a iter is the maximum number of iterations. If 0 is given for the argument, Z_MAX_ITER_NUM defined
 * in zm_misc.h is applied.
 * \return
 * zMat3DEigPower() returns the largest eigenvalue of \a m.
 * \sa
 * zMat3DEigPowerInv
 */
__ZEO_EXPORT double zMat3DEigPower(const zMat3D *m, zVec3D *eigvec, int iter);

/*! \brief calculate the minimal eigenvalue.
 *
 * zMat3DEigPowerInv() computes the smallest eigenvalue and the corresponding eigenvector of a 3x3 matrix
 * \a m by the inverse power method. The eigenvector is stored in \a eigvec.
 * \a iter is the maximum number of iterations. If 0 is given for the argument, Z_MAX_ITER_NUM defined
 * in zm_misc.h is applied.
 * \return
 * zMat3DEigPower() returns the smallest eigenvalue of \a m.
 * \sa
 * zMat3DEigPower
 */
__ZEO_EXPORT double zMat3DEigPowerInv(const zMat3D *m, zVec3D *eigvec, int iter);

/*! \brief eigensystem of a symmetric 3x3 matrix by Jacobi's method.
 *
 * zMat3DSymEig() calculates eigenvalues and eigenvectors of a symmetric 3x3 matrix \a m with Jacobi's
 * method. Each eigenvalue and eigenvector are stored in \a eigval and \a eigbase, respectively, in the
 * corresponding order.
 * \return
 * zMat3DSymEig() returns the false value if it does not converge within Z_MAX_ITER_NUM. Otherwise,
 * it returns the true value.
 * \notes
 * \a m must be symmetric. Otherwise, the correct result will not be expected.
 */
__ZEO_EXPORT bool zMat3DSymEig(const zMat3D *m, zVec3D *eigval, zMat3D *eigbase);

/*! \brief eigenvector of a 3x3 symmetric matrix corresponding to the minimum eigenavlue.
 *
 * zMat3DSymEigMin() finds the eigenvector of a 3x3 symmetric matrix \a m corresponding to the minimum
 * eigenvalue. The result is put into \a eig.
 * \return
 * zMat3DSymEigMin() returns a pointer \a eig.
 * \sa
 * zMat3DSymEig
 */
__ZEO_EXPORT zVec3D *zMat3DSymEigMin(const zMat3D *m, zVec3D *eig);

/*! \brief singular value decomposition of a 3x3 matrix.
 *
 * zMat3DSVD() conducts singular value decomposition on a 3x3 matrix \a m, namely, finds 3x3 matrices
 * \a u and \a v and a trio of scalar values \a s that satisfy
 * \a m = \a u diag{ \a s[0], \a s[1], \a s[2] } \a v ^T.
 * \return
 * zMat3DSVD() returns the rank, namely, the number of non-zero singular values of \a m.
 */
__ZEO_EXPORT int zMat3DSVD(const zMat3D *m, zMat3D *u, zVec3D *sv, zMat3D *v);

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/*! \brief read a 3x3 matrix from a ZTK format processor. */
__ZEO_EXPORT zMat3D *zMat3DFromZTK(zMat3D *m, ZTK *ztk);
/*! \brief add a 3x3 matrix to a ZTK format processor. */
__ZEO_EXPORT ZTK *zMat3DToZTK(const zMat3D *m, ZTK *ztk);

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
__ZEO_EXPORT zMat3D *zMat3DFScan(FILE *fp, zMat3D *m);
#define zMat3DScan(m) zMat3DFScan( stdin, (m) )
__ZEO_EXPORT void zMat3DFPrint(FILE *fp, const zMat3D *m);
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
__ZEO_EXPORT zVec3D *zAAFromZTK(zVec3D *aa, ZTK *ztk);

__END_DECLS

#ifdef __cplusplus
inline zVec3D &zMat3D::vec(zAxis axis){ return *zMat3DVec( this, axis ); }
inline zMat3D &zMat3D::create(double a11, double a12, double a13, double a21, double a22, double a23, double a31, double a32, double a33){
  _zMat3DCreate( this, a11, a12, a13, a21, a22, a23, a31, a32, a33 );
  return *this;
}
inline zMat3D &zMat3D::copy(const zMat3D &m){ zMat3DCopy( &m, this ); return *this; }
inline zMat3D &zMat3D::zero(){ zMat3DZero( this ); return *this; }
inline zMat3D &zMat3D::ident(){ zMat3DIdent( this ); return *this; }
inline bool zMat3D::isEqual(const zMat3D &m){ return _zMat3DEqual( this, &m ); }
inline bool zMat3D::isIdent(){ return _zMat3DIsIdent( this ); }
inline bool zMat3D::isTol(double tol){ return _zMat3DIsTol( this, tol ); }
inline bool zMat3D::isTiny(){ return _zMat3DIsTiny( this ); }
inline zVec3D &zMat3D::row(int i, zVec3D &v){ _zMat3DRow( this, i, &v ); return v; }
inline zVec3D &zMat3D::col(int i, zVec3D &v){ zMat3DCol( this, i, &v ); return v; }
inline zVec3D zMat3D::row(int i){ zVec3D v; _zMat3DRow( this, i, &v ); return v; }
inline zVec3D zMat3D::col(int i){ return *zMat3DVec( this, i ); }
inline zMat3D &zMat3D::transpose(zMat3D &m){ _zMat3DT( this, &m ); return m; }
inline zMat3D zMat3D::transpose(){ zMat3D m; _zMat3DT( this, &m ); return m; }
inline zMat3D &zMat3D::transposeDirect(){ _zMat3DTDRC( this ); return *this; }
inline zMat3D zMat3D::operator+(const zMat3D &m){ zMat3D ret; _zMat3DAdd( this, &m, &ret ); return ret; }
inline zMat3D zMat3D::operator-(const zMat3D &m){ zMat3D ret; _zMat3DSub( this, &m, &ret ); return ret; }
inline zMat3D zMat3D::operator-(){ zMat3D ret; _zMat3DRev( this, &ret ); return ret; }
inline zMat3D zMat3D::operator*(double k){ zMat3D ret; _zMat3DMul( this, k, &ret ); return ret; }
inline zMat3D zMat3D::operator/(double k){ zMat3D ret; zMat3DDiv( this, k, &ret ); return ret; }
inline zMat3D &zMat3D::operator+=(const zMat3D &m){ _zMat3DAddDRC( this, &m ); return *this; }
inline zMat3D &zMat3D::operator-=(const zMat3D &m){ _zMat3DSubDRC( this, &m ); return *this; }
inline zMat3D &zMat3D::operator*=(double k){ _zMat3DMulDRC( this, k ); return *this; }
inline zMat3D &zMat3D::operator/=(double k){ zMat3DDivDRC( this, k ); return *this; }
inline zMat3D &zMat3D::createDyad(const zVec3D &v1, const zVec3D &v2){ _zMat3DDyad( this, &v1, &v2 ); return *this; }
inline zMat3D &zMat3D::createOuterprod(const zVec3D &v){ _zVec3DOuterProd2Mat3D( &v, this ); return *this; }
inline zMat3D &zMat3D::createTripleprod(const zVec3D &v1, const zVec3D &v2){ _zVec3DTripleProd2Mat3D( &v1, &v2, this ); return *this; }
inline zMat3D &zMat3D::createDoubleOuterprod(const zVec3D &v){ _zVec3DDoubleOuterProd2Mat3D( &v, this ); return *this; }
inline zMat3D &zMat3D::orthonormalize(zAxis axis1, zAxis axis2){ return *zMat3DOrthonormalizeDRC( this, axis1, axis2 ); }
inline double zMat3D::squareNorm(){ return _zMat3DSqrNorm( this ); }
inline double zMat3D::norm(){ return _zMat3DNorm( this ); }
inline double zMat3D::det(){ return _zMat3DDet( this ); }
inline zMat3D &zMat3D::inv(zMat3D &m){ zMat3DInv( this, &m ); return m; }
inline zMat3D zMat3D::inv(){ zMat3D m; zMat3DInv( this, &m ); return m; }
inline zVec3D zMat3D::operator*(const zVec3D &v){ zVec3D mv; _zMulMat3DVec3D( this, &v, &mv ); return mv; }
inline zVec3D zMat3D::mulT(const zVec3D &v){ zVec3D mv; _zMulMat3DTVec3D( this, &v, &mv ); return mv; }
inline zVec3D zMat3D::mulInv(const zVec3D &v){ zVec3D mv; zMulInvMat3DVec3D( this, &v, &mv ); return mv; }
inline zMat3D zMat3D::operator*(const zMat3D &m){ zMat3D mm; zMulMat3DMat3D( this, &m, &mm ); return mm; }
inline zMat3D zMat3D::mulT(const zMat3D &m){ zMat3D mm; zMulMat3DTMat3D( this, &m, &mm ); return mm; }
inline zMat3D zMat3D::mulInv(const zMat3D &m){ zMat3D mm; zMulInvMat3DMat3D( this, &m, &mm ); return mm; }
inline zVec6D zMat3D::operator*(const zVec6D &v){ zVec6D mv; zMulMat3DVec6D( this, &v, &mv ); return mv; }
inline zVec6D zMat3D::mulT(const zVec6D &v){ zVec6D mv; zMulMat3DVec6D( this, &v, &mv ); return mv; }
inline zMat3D &zMat3D::rotRoll(double angle){ return *zMat3DRotRollDRC( this, angle ); }
inline zMat3D &zMat3D::rotPitch(double angle){ return *zMat3DRotPitchDRC( this, angle ); }
inline zMat3D &zMat3D::rotYaw(double angle){ return *zMat3DRotYawDRC( this, angle ); }
inline zMat3D &zMat3D::createFromZYX(double azim, double elev, double tilt){ return *zMat3DFromZYX( this, azim, elev, tilt ); }
inline zMat3D &zMat3D::createFromZYZ(double heading, double pitch, double bank){ return *zMat3DFromZYZ( this, heading, pitch, bank ); }
inline zMat3D &zMat3D::createFromAA(const zVec3D &aa){ return *zMat3DFromAA( this, &aa ); }
inline zMat3D &zMat3D::rot(const zMat3D &r){ return *zRotMat3DDRC( &r, this ); }
inline zMat3D &zMat3D::rotInv(const zMat3D &r){ return *zRotMat3DInvDRC( &r, this ); }
inline zMat3D &zMat3D::rot(const zVec3D &aa){ return *zMat3DRotDRC( this, &aa ); }

inline bool operator==(const zMat3D &m1, const zMat3D &m2){ return _zMat3DMatch( &m1, &m2 ); }
__ZEO_EXPORT std::ostream &operator<<(std::ostream &stream, zMat3D &mat);
#endif /* __cplusplus */

#endif /* __ZEO_MAT3D_H__ */
