/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat2d.h - 2x2 matrix.
 */

#include <zeo/zeo_mat2d.h>

/* ********************************************************** */
/* CLASS: zMat2D
 * 2x2 matrix class
 * ********************************************************** */

/* 2x2 zero matrix and identity matrix. */
#ifdef __cplusplus
const zMat2D zMat2D::zmat2Dzero  = { { { 0, 0 }, { 0, 0 } } };
const zMat2D zMat2D::zmat2Dident = { { { 1, 0 }, { 0, 1 } } };
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
  k = 1.0 / k;
  _zMat2DMul( m, k, dm );
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
double zMat2DDet(const zMat2D *m)
{
  return _zMat2DDet( m );
}

/* inverse matrix of a 3x3 matrix. */
zMat2D *zMat2DInv(const zMat2D *m, zMat2D *im)
{
  double det;

  if( zIsTiny( ( det = _zMat2DDet( m ) ) ) ){
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
    return NULL;
  }
  det = 1.0 / det;
  _zMat2DCreate( im, m->c.yy*det, -m->c.yx*det, -m->c.xy*det, m->c.xx*det );
  return im;
}

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

/* multiply a 2D vector by the inverse of a 2x2 matrix. */
zVec2D *zMulInvMat2DVec2D(const zMat2D *m, const zVec2D *v, zVec2D *mv)
{
  double det;

  if( zIsTiny( ( det = _zMat2DDet( m ) ) ) ){
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
    return NULL;
  }
  _zVec2DCreate( mv, (m->c.yy*v->c.x-m->c.yx*v->c.y)/det, (-m->c.xy*v->c.x+m->c.xx*v->c.y)/det );
  return mv;
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
  double det;

  if( zIsTiny( ( det = _zMat2DDet( m1 ) ) ) ){
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
    return NULL;
  }
  _zMat2DCreate( m,
    (m1->c.yy*m2->c.xx-m1->c.yx*m2->c.xy)/det, (m1->c.yy*m2->c.yx-m1->c.yx*m2->c.yy)/det,
   (-m1->c.xy*m2->c.xx+m1->c.xx*m2->c.xy)/det,(-m1->c.xy*m2->c.yx+m1->c.xx*m2->c.yy)/det );
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

#ifdef __cplusplus
std::ostream &operator<<(std::ostream &stream, zMat2D &mat){
  stream << "|~ " << mat.c.xx << ", " << mat.c.yx << " ~|" << std::endl;
  stream << "|_ " << mat.c.xy << ", " << mat.c.yy << " _|" << std::endl;
  return stream;
}
#endif /* __cplusplus */
