/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat6d - 6x6 matrix.
 * This class was originally defined by N. Wakisaka in 2011.
 */

#ifndef __ZEO_MAT6D_H__
#define __ZEO_MAT6D_H__

#include <zeo/zeo_mat3d.h>

__BEGIN_DECLS

/*! \struct zMat6D
 * \brief 6x6 matrix, which has 4 3x3 matrices within.
 */
typedef union{
  zMat3D m[4];    /*!< \brief four 3x3 matrices */
  zMat3D e[2][2]; /*!< \brief 2x2 3x3 matrices */
  struct{
    zMat3D ll, la, al, aa;
  } c;            /*!< \brief 2x2 block matrices */
} zMat6D;

#if 0
#define zMat6DElem(m,i,j)  zMat3DElem(&(m)->e[(i)/3][(j)/3], (i)%3, (j)%3 )
#endif
#define zMat6DMat3D(m,i,j) ( &(m)->e[(i)][(j)] )

/*! \brief create a 6x6 matrix. */
#define _zMat6DCreate(m,m1,m2,m3,m4) do{\
  zMat3DCopy( m1, &(m)->c.ll );\
  zMat3DCopy( m2, &(m)->c.la );\
  zMat3DCopy( m3, &(m)->c.al );\
  zMat3DCopy( m4, &(m)->c.aa );\
} while(0)
__EXPORT zMat6D *zMat6DCreate(zMat6D *m, zMat3D *m1, zMat3D *m2, zMat3D *m3, zMat3D *m4);
/*! \brief copy a 6x6 matrix to another */
#define zMat6DCopy(s,d) zCopy( zMat6D, s, d )
/*! \brief put a sub 3x3 matrix into a 6x6 matrix. */
__EXPORT zMat6D *zMat6DPutMat3D(zMat6D *m6d, int i, int j, zMat3D *m3d);
/*! \brief clear a 6x6 matrix to be zero. */
#define _zMat6DClear(m) do{\
  zMat3DClear( &m->c.ll );\
  zMat3DClear( &m->c.la );\
  zMat3DClear( &m->c.al );\
  zMat3DClear( &m->c.aa );\
} while(0)
__EXPORT zMat6D *zMat6DClear(zMat6D *m);

/*! \brief transpose of a 6x6 matrix. */
#define _zMat6DT(m,tm) do{\
  _zMat3DT( &(m)->c.ll, &(tm)->c.ll );\
  _zMat3DT( &(m)->c.la, &(tm)->c.al );\
  _zMat3DT( &(m)->c.al, &(tm)->c.la );\
  _zMat3DT( &(m)->c.aa, &(tm)->c.aa );\
} while(0)
__EXPORT zMat6D *zMat6DT(zMat6D *m, zMat6D *mout);

/*! \brief abstract row vector from a 6D matrix.
 */
zVec6D *zMat6DRow(zMat6D *m, int i, zVec6D *v);
/*! \brief abstract column vector from a 6D matrix.
 */
zVec6D *zMat6DCol(zMat6D *m, int i, zVec6D *v);

/*! \brief add two 6x6 matrices.
 * \a m = \a m1 + \a m2
 */
#define _zMat6DAdd(m1,m2,m) do{\
  _zMat3DAdd( &(m1)->c.ll, &(m2)->c.ll, &(m)->c.ll );\
  _zMat3DAdd( &(m1)->c.la, &(m2)->c.la, &(m)->c.la );\
  _zMat3DAdd( &(m1)->c.al, &(m2)->c.al, &(m)->c.al );\
  _zMat3DAdd( &(m1)->c.aa, &(m2)->c.aa, &(m)->c.aa );\
} while(0)
__EXPORT zMat6D *zMat6DAdd(zMat6D *m1, zMat6D *m2, zMat6D *m);

/*! \brief subtract a 6x6 matrix from another.
 * \a m = \a m1 - \a m2
 */
#define _zMat6DSub(m1,m2,m) do{\
  _zMat3DSub( &(m1)->c.ll, &(m2)->c.ll, &(m)->c.ll );\
  _zMat3DSub( &(m1)->c.la, &(m2)->c.la, &(m)->c.la );\
  _zMat3DSub( &(m1)->c.al, &(m2)->c.al, &(m)->c.al );\
  _zMat3DSub( &(m1)->c.aa, &(m2)->c.aa, &(m)->c.aa );\
} while(0)
__EXPORT zMat6D *zMat6DSub(zMat6D *m1, zMat6D *m2, zMat6D *m);

/*! \brief multiply a 6x6 matrix by a scalar value.
 * \a m = \a k \a m
 */
#define _zMat6DMul(m,k,mm) do{\
  _zMat3DMul( &(m)->c.ll, k, &(mm)->c.ll );\
  _zMat3DMul( &(m)->c.la, k, &(mm)->c.la );\
  _zMat3DMul( &(m)->c.al, k, &(mm)->c.al );\
  _zMat3DMul( &(m)->c.aa, k, &(mm)->c.aa );\
} while(0)
__EXPORT zMat6D *zMat6DMul(zMat6D *m, double k, zMat6D *mm);

/*! \brief divide a 6x6 matrix by a scalar value.
 * \a m = 1/\a k \a m
 */
__EXPORT zMat6D *zMat6DDiv(zMat6D *m, double k, zMat6D *dm);

#define _zMat6DAddDRC(m1,m2) do{\
  _zMat3DAddDRC( &(m1)->c.ll, &(m2)->c.ll );\
  _zMat3DAddDRC( &(m1)->c.la, &(m2)->c.la );\
  _zMat3DAddDRC( &(m1)->c.al, &(m2)->c.al );\
  _zMat3DAddDRC( &(m1)->c.aa, &(m2)->c.aa );\
} while(0)
__EXPORT zMat6D *zMat6DAddDRC(zMat6D *m1, zMat6D *m2);
#define _zMat6DSubDRC(m1,m2) do{\
  _zMat3DSubDRC( &(m1)->c.ll, &(m2)->c.ll );\
  _zMat3DSubDRC( &(m1)->c.la, &(m2)->c.la );\
  _zMat3DSubDRC( &(m1)->c.al, &(m2)->c.al );\
  _zMat3DSubDRC( &(m1)->c.aa, &(m2)->c.aa );\
} while(0)
__EXPORT zMat6D *zMat6DSubDRC(zMat6D *m1, zMat6D *m2);
#define _zMat6DMulDRC(m,k) do{\
  _zMat3DMulDRC( &(m)->c.ll, k );\
  _zMat3DMulDRC( &(m)->c.la, k );\
  _zMat3DMulDRC( &(m)->c.al, k );\
  _zMat3DMulDRC( &(m)->c.aa, k );\
} while(0)
__EXPORT zMat6D *zMat6DMulDRC(zMat6D *m, double k);
__EXPORT zMat6D *zMat6DDivDRC(zMat6D *m, double k);

/*! \brief multiply a 6x1 vector by a 6x6 matrix from the left side.
 * \a vout = \a m \a vin
 */
__EXPORT zVec6D *zMulMat6DVec6D(zMat6D *m, zVec6D *vin, zVec6D *vout);

/*! \brief multiply a 6x1 vector by the transpose of a 6x6 matrix from the left side.
 * \a vout = \a m^T \a vin
 */
__EXPORT zVec6D *zMulMat6DTVec6D(zMat6D *m, zVec6D *vin, zVec6D *vout);

/*! \brief multiply a 6x6 matrix by another.
 * \a m = \a m1 \a m2
 */
__EXPORT zMat6D *zMulMatMat6D(zMat6D *m1, zMat6D *m2, zMat6D *m);

/*! \brief multiply a 6x6 matrix by the transpose of another from the left side.
 * \a m = \a m1^T \a m2
 */
__EXPORT zMat6D *zMulMatTMat6D(zMat6D *m1, zMat6D *m2, zMat6D *m);

/*! \brief multiply a 6x6 matrix by the transpose of another from the right side.
 * \a m = \a m1 \a m2^T
 */
__EXPORT zMat6D *zMulMatMatT6D(zMat6D *m1, zMat6D *m2, zMat6D *m);

/*! \brief dyadic product of two 6x1 vectors. */
#define _zMat6DDyad(m,v1,v2) do{\
  _zMat3DDyad( &(m)->c.ll, zVec6DLin(v1), zVec6DLin(v2) );\
  _zMat3DDyad( &(m)->c.la, zVec6DLin(v1), zVec6DAng(v2) );\
  _zMat3DDyad( &(m)->c.al, zVec6DAng(v1), zVec6DLin(v2) );\
  _zMat3DDyad( &(m)->c.aa, zVec6DAng(v1), zVec6DAng(v2) );\
} while(0)
__EXPORT zMat6D *zMat6DDyad(zMat6D *m, zVec6D *v1, zVec6D *v2);

/*! \brief output a 6x6 matrix to the current position of a file. */
__EXPORT void zMat6DFWrite(FILE *fp, zMat6D *m);
#define zMat6DWrite(m) zMat6DFWrite( stdout, (m) )

__END_DECLS

#endif /* __ZEO_MAT6D_H__ */
