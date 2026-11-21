/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat2d - 2x2 matrix.
 */

#include <zeo/zeo_mat2d.h>

/* 2x2 zero matrix and identity matrix. */
#ifdef __cplusplus
const zMat2D zMat2D::zmat2Dzero { 0, 0, 0, 0 };
const zMat2D zMat2D::zmat2Dident{ 1, 0, 0, 1 };
#else
const zMat2D zmat2Dzero  = { { { 0, 0 }, { 0, 0 } } };
const zMat2D zmat2Dident = { { { 1, 0 }, { 0, 1 } } };
#endif /* __cplusplus */

/* create a 2x2 matrix. */
zMat2D *zMat2DCreate(zMat2D *m, double a11, double a12, double a21, double a22)
{
  _zMat2DCreate( m, a11, a12, a21, a22 );
  return m;
}

/* create a 2x2 rotation matrix. */
zMat2D *zMat2DCreateAngle(zMat2D *m, double angle)
{
  double s, c;

  _zSinCos( angle, &s, &c );
  return zMat2DCreate( m, c, -s, s, c );
}

/* copy a 2x2 matrix to the other. */
zMat2D *zMat2DCopy(const zMat2D *src, zMat2D *dest)
{
  _zMat2DCopy( src, dest );
  return dest;
}

/* check if two 2x2 matrices are strictly equal. */
bool zMat2DMatch(const zMat2D *m1, const zMat2D *m2)
{
  return _zMat2DMatch( m1, m2 );
}

/* check if two 2x2 matrices are equal. */
bool zMat2DEqual(const zMat2D *m1, const zMat2D *m2)
{
  return _zMat2DEqual( m1, m2 );
}

/* check if a 2x2 matrix is the identity matrix. */
bool zMat2DIsIdent(const zMat2D *m)
{
  return _zMat2DIsIdent( m );
}

/* abstract a row vector from a 2x2 matrix. */
zVec2D *zMat2DRow(const zMat2D *m, int i, zVec2D *v)
{
  _zMat2DRow( m, i, v );
  return v;
}

/* transpose a 2x2 matrix. */
zMat2D *zMat2DT(const zMat2D *m, zMat2D *tm)
{
  _zMat2DT( m, tm );
  return tm;
}

/* directly transpose a 2x2 matrix. */
zMat2D *zMat2DTDRC(zMat2D *m)
{
  _zMat2DTDRC( m );
  return m;
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* add two 2x2 matrices. */
zMat2D *zMat2DAdd(const zMat2D *m1, const zMat2D *m2, zMat2D *m)
{
  _zMat2DAdd( m1, m2, m );
  return m;
}

/* subtract a 2x2 matrix from another. */
zMat2D *zMat2DSub(const zMat2D *m1, const zMat2D *m2, zMat2D *m)
{
  _zMat2DSub( m1, m2, m );
  return m;
}

/* reverse a 2x2 matrix. */
zMat2D *zMat2DRev(const zMat2D *m, zMat2D *rm)
{
  _zMat2DRev( m, rm );
  return rm;
}

/* multiply a 2x2 matrix by a scalar. */
zMat2D *zMat2DMul(const zMat2D *m, double k, zMat2D *mm)
{
  _zMat2DMul( m, k, mm );
  return mm;
}

/* divide a 2x2 matrix by a scalar. */
zMat2D *zMat2DDiv(const zMat2D *m, double k, zMat2D *dm)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  _zMat2DDiv( m, k, dm );
  return dm;
}

/* concatenate a 2x2 matrix with another. */
zMat2D *zMat2DCat(const zMat2D *m1, double k, const zMat2D *m2, zMat2D *m)
{
  _zMat2DCat( m1, k, m2, m );
  return m;
}

/* dyadic product of two 2D vectors. */
zMat2D *zMat2DDyad(zMat2D *dyad, const zVec2D *v1, const zVec2D *v2)
{
  _zMat2DDyad( dyad, v1, v2 );
  return dyad;
}

/* calculate squared norm of a 2x2 matrix. */
double zMat2DSqrNorm(const zMat2D *m)
{
  return _zMat2DSqrNorm( m );
}

/* ********************************************************** */
/* inverse of a 2x2 matrix
 * ********************************************************** */

/* determinant of a 2x2 matrix. */
double zMat2DDet(const zMat2D *mat)
{
  return _zMat2DDet( mat );
}

/* adjoint matrix of a 3x3 matrix. */
zMat2D *zMat2DAdj(const zMat2D *mat, zMat2D *adj)
{
  _zMat2DAdj( mat, adj );
  return adj;
}

/* LU decomposition of a balanced 2x2 matrix with partial pivoting. */
typedef struct{
  zMat2D a;  /* L (unit lower triangular, below the diagonal) and U. a.v[i] is the i-th ROW. */
  zVec2D ir; /* reciprocals of the row scales */
  zVec2D ic; /* reciprocals of the column scales */
  zVec2D id; /* reciprocals of the diagonal components of U */
  bool s;    /* true if the rows are swapped */
} _zMat2DLU;

/* local macros for _zMat2DLUDecomp() and _zMat2DLUSolve(); see those for _zMat3DLUDecomp(). */
#define __zMat2DLURow(lu,i)        ( &(lu)->a.v[i] )
#define __zMat2DLUElem(lu,i,j)     (lu)->a.v[i].e[j]
#define __zMat2DLURowAbsMax(lu,i)  _zMax( fabs( __zMat2DLUElem(lu,i,0) ), fabs( __zMat2DLUElem(lu,i,1) ) )
#define __zMat2DLUColAbsMax(lu,j)  _zMax( fabs( __zMat2DLUElem(lu,0,j) ), fabs( __zMat2DLUElem(lu,1,j) ) )
#define __zMat2DLUSwapRow(lu) do{\
  zSwap( double, __zMat2DLUElem(lu,0,0), __zMat2DLUElem(lu,1,0) );\
  zSwap( double, __zMat2DLUElem(lu,0,1), __zMat2DLUElem(lu,1,1) );\
} while(0)
#define __zMat2DLUPivotIsZero(lu,k) ( !( fabs( __zMat2DLUElem(lu,k,k) ) > zTOL ) )
#define __zMat2DLUSetPivot(lu,k)   ( (lu)->id.e[k] = 1.0 / __zMat2DLUElem(lu,k,k) )

/* LU decomposition of a 2x2 matrix in the same way with _zMat3DLUDecomp(). */
static bool _zMat2DLUDecomp(const zMat2D *m, _zMat2DLU *lu)
{
  _zMat2DT( m, &lu->a );
  /* row balancing */
  if( !( ( lu->ir.c.x = __zMat2DLURowAbsMax(lu,0) ) > 0 ) ||
      !( ( lu->ir.c.y = __zMat2DLURowAbsMax(lu,1) ) > 0 ) ) return false; /* zero row */
  _zVec2DCreate( &lu->ir, 1.0 / lu->ir.c.x, 1.0 / lu->ir.c.y );
  _zVec2DMulDRC( __zMat2DLURow(lu,0), lu->ir.c.x );
  _zVec2DMulDRC( __zMat2DLURow(lu,1), lu->ir.c.y );
  /* column balancing */
  if( !( ( lu->ic.c.x = __zMat2DLUColAbsMax(lu,0) ) > 0 ) ||
      !( ( lu->ic.c.y = __zMat2DLUColAbsMax(lu,1) ) > 0 ) ) return false; /* zero column */
  _zVec2DCreate( &lu->ic, 1.0 / lu->ic.c.x, 1.0 / lu->ic.c.y );
  _zVec2DAmpDRC( __zMat2DLURow(lu,0), &lu->ic );
  _zVec2DAmpDRC( __zMat2DLURow(lu,1), &lu->ic );
  /* elimination of the first column */
  if( ( lu->s = ( fabs( __zMat2DLUElem(lu,1,0) ) > fabs( __zMat2DLUElem(lu,0,0) ) ) ) )
    __zMat2DLUSwapRow( lu );
  if( __zMat2DLUPivotIsZero( lu, 0 ) ) return false;
  __zMat2DLUSetPivot( lu, 0 );
  __zMat2DLUElem(lu,1,0) *= lu->id.e[0]; /* multiplier, stored as a component of L */
  __zMat2DLUElem(lu,1,1) -= __zMat2DLUElem(lu,1,0) * __zMat2DLUElem(lu,0,1);
  /* the last pivot */
  if( __zMat2DLUPivotIsZero( lu, 1 ) ) return false;
  __zMat2DLUSetPivot( lu, 1 );
  return true;
}

/* solve a 2x2 linear equation m x = b from the LU decomposition of m. x can be the same with b. */
static void _zMat2DLUSolve(const _zMat2DLU *lu, const zVec2D *b, zVec2D *x)
{
  zVec2D y;

  /* row balancing and pivoting */
  _zVec2DAmp( b, &lu->ir, &y );
  if( lu->s ) zSwap( double, y.c.x, y.c.y );
  /* forward and backward substitutions */
  y.c.y = ( y.c.y - __zMat2DLUElem(lu,1,0) * y.c.x ) * lu->id.c.y;
  y.c.x = ( y.c.x - __zMat2DLUElem(lu,0,1) * y.c.y ) * lu->id.c.x;
  /* column balancing */
  _zVec2DAmp( &y, &lu->ic, x );
}

#undef __zMat2DLURow
#undef __zMat2DLUElem
#undef __zMat2DLURowAbsMax
#undef __zMat2DLUColAbsMax
#undef __zMat2DLUSwapRow
#undef __zMat2DLUPivotIsZero
#undef __zMat2DLUSetPivot

/* ********************************************************** */
/* multiplication of a 2D vector by a 2x2 matrix
 * ********************************************************** */

/* multiply a 2D vector by a 2x2 matrix. */
zVec2D *zMulMat2DVec2D(const zMat2D *m, const zVec2D *v, zVec2D *mv)
{
  _zVec2DCreate( mv, m->c.xx*v->c.x+m->c.yx*v->c.y, m->c.xy*v->c.x+m->c.yy*v->c.y );
  return mv;
}

/* multiply a 2D vector by transpose of a 2x2 matrix. */
zVec2D *zMulMat2DTVec2D(const zMat2D *m, const zVec2D *v, zVec2D *mv)
{
  _zVec2DCreate( mv, m->c.xx*v->c.x+m->c.xy*v->c.y, m->c.yx*v->c.x+m->c.yy*v->c.y );
  return mv;
}

/* directly multiply a 2D vector by a 3x3 matrix. */
zVec2D *zMulMat2DVec2DDRC(const zMat2D *m, zVec2D *v)
{
  _zMulMat2DVec2DDRC( m, v );
  return v;
}

/* directly multiply a 2D vector by transpose of a 3x3 matrix. */
zVec2D *zMulMat2DTVec2DDRC(const zMat2D *m, zVec2D *v)
{
  _zMulMat2DTVec2DDRC( m, v );
  return v;
}

/* multiply a 2D vector by the adjoint of a 2x2 matrix. */
zVec2D *zMulAdjMat2DVec2D(const zMat2D *mat, const zVec2D *vec, zVec2D *adj_vec)
{
  _zMulAdjMat2DVec2D( mat, vec, adj_vec );
  return adj_vec;
}

/* multiply a 2D vector by the inverse of a 2x2 matrix. */
zVec2D *zMulInvMat2DVec2D(const zMat2D *mat, const zVec2D *vec, zVec2D *inv_vec)
{
  _zMat2DLU lu;

  if( !_zMat2DLUDecomp( mat, &lu ) ){
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
    return NULL;
  }
  _zMat2DLUSolve( &lu, vec, inv_vec );
  return inv_vec;
}

/* ********************************************************** */
/* multiplication of a 2x2 matrix by another 2x2 matrix
 * ********************************************************** */

/* multiply two 2D matrices. */
zMat2D *zMulMat2DMat2D(const zMat2D *m1, const zMat2D *m2, zMat2D *m)
{
  zMat2D tmp;

  _zMulMat2DVec2D( m1, &m2->b.x, &tmp.b.x );
  _zMulMat2DVec2D( m1, &m2->b.y, &tmp.b.y );
  return zMat2DCopy( &tmp, m );
}

/* multiply a matrix by transpose matrix from leftside. */
zMat2D *zMulMat2DTMat2D(const zMat2D *m1, const zMat2D *m2, zMat2D *m)
{
  zMat2D tmp;

  _zMulMat2DTVec2D( m1, &m2->b.x, &tmp.b.x );
  _zMulMat2DTVec2D( m1, &m2->b.y, &tmp.b.y );
  return zMat2DCopy( &tmp, m );
}

/* multiply a matrix by transpose matrix from rightside. */
#define __zMulMat2DMat2DTElem(m1,m2,i,j,m) \
  ( (m)->e[i][j] = (m1)->e[0][j]*(m2)->e[0][i] + (m1)->e[1][j]*(m2)->e[1][i] )
zMat2D *zMulMat2DMat2DT(const zMat2D *m1, const zMat2D *m2, zMat2D *m)
{
  zMat2D tmp;

  __zMulMat2DMat2DTElem( m1, m2, 0, 0, &tmp );
  __zMulMat2DMat2DTElem( m1, m2, 0, 1, &tmp );
  __zMulMat2DMat2DTElem( m1, m2, 1, 0, &tmp );
  __zMulMat2DMat2DTElem( m1, m2, 1, 1, &tmp );
  return zMat2DCopy( &tmp, m );
}

/* multiply a 2x2 matrix by the inverse of another matrix from leftside. */
zMat2D *zMulInvMat2DMat2D(const zMat2D *m1, const zMat2D *m2, zMat2D *m)
{
  _zMat2DLU lu;

  if( !_zMat2DLUDecomp( m1, &lu ) ){
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
    return NULL;
  }
  _zMat2DLUSolve( &lu, &m2->b.x, &m->b.x );
  _zMat2DLUSolve( &lu, &m2->b.y, &m->b.y );
  return m;
}

/* ********************************************************** */
/* rotation
 * ********************************************************** */

/* equivalent rotation angle of a 2x2 matrix. */
double zMat2DAngle(const zMat2D *mat)
{
  return _zMat2DAngle( mat );
}

/* rotate a 2x2 matrix with sine and cosine values. */
zMat2D *zMat2DRotSC(const zMat2D *m, double s, double c, zMat2D *rm)
{
  _zMat2DRotSC( m, s, c, rm );
  return rm;
}

/* rotate a 2x2 matrix. */
zMat2D *zMat2DRot(const zMat2D *m, double theta, zMat2D *rm)
{
  double s, c;
  zSinCos( theta, &s, &c );
  _zMat2DRotSC( m, s, c, rm );
  return rm;
}

/* error vector between two 2x2 attitude matrices. */
double zMat2DError(const zMat2D *m1, const zMat2D *m2)
{
  return _zMat2DError( m1, m2 );
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* scan a 2x2 matrix from a file. */
zMat2D *zMat2DFScan(FILE *fp, zMat2D *m)
{
  int i, j;

  for( i=0; i<2; i++ )
    for( j=0; j<2; j++ )
      zFDouble( fp, &m->e[j][i] );
  return m;
}

/* print a 2x2 matrix to a file. */
void zMat2DFPrint(FILE *fp, const zMat2D *m)
{
  if( !m ) return;
  fprintf( fp, "{\n" );
  fprintf( fp, " %.10g, %.10g\n", m->c.xx, m->c.yx );
  fprintf( fp, " %.10g, %.10g\n", m->c.xy, m->c.yy );
  fprintf( fp, "}\n" );
}
