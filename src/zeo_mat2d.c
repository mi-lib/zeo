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

/* create a 2x2 matrix. */
zMat2D *zMat2DCreate(zMat2D *m, double a11, double a12, double a21, double a22)
{
  _zMat2DCreate( m, a11, a12, a21, a22 );
  return m;
}

/* copy a 2x2 matrix to the other. */
zMat2D *zMat2DCopy(zMat2D *src, zMat2D *dest)
{
  _zMat2DCopy( src, dest );
  return dest;
}

/* abstract row vector from a 2x2 matrix. */
void zMat2DRow(zMat2D *m, zVec2D *r1, zVec2D *r2)
{
  if( r1 ) _zVec2DCreate( r1, m->c.xx, m->c.yx );
  if( r2 ) _zVec2DCreate( r2, m->c.xy, m->c.yy );
}

/* abstract column vector from a 2x2 matrix. */
void zMat2DCol(zMat2D *m, zVec2D *c1, zVec2D *c2)
{
  if( c1 ) _zVec2DCopy( &m->b.x, c1 );
  if( c2 ) _zVec2DCopy( &m->b.y, c2 );
}

/* transpose a 2x2 matrix. */
zMat2D *zMat2DT(zMat2D *m, zMat2D *tm)
{
  _zMat2DT( m, tm );
  return tm;
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* add two 2x2 matrices. */
zMat2D *zMat2DAdd(zMat2D *m1, zMat2D *m2, zMat2D *m)
{
  _zMat2DAdd( m1, m2, m );
  return m;
}

/* subtract a 2x2 matrix from another. */
zMat2D *zMat2DSub(zMat2D *m1, zMat2D *m2, zMat2D *m)
{
  _zMat2DSub( m1, m2, m );
  return m;
}

/* reverse a 2x2 matrix. */
zMat2D *zMat2DRev(zMat2D *m, zMat2D *rm)
{
  _zMat2DRev( m, rm );
  return rm;
}

/* multiply a 2x2 matrix by a scalar. */
zMat2D *zMat2DMul(zMat2D *m, double k, zMat2D *mm)
{
  _zMat2DMul( m, k, mm );
  return mm;
}

/* divide a 2x2 matrix by a scalar. */
zMat2D *zMat2DDiv(zMat2D *m, double k, zMat2D *dm)
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
zMat2D *zMat2DCat(zMat2D *m1, double k, zMat2D *m2, zMat2D *m)
{
  _zMat2DCat( m1, k, m2, m );
  return m;
}

/* dyadic product of two 2D vectors. */
zMat2D *zMat2DDyad(zMat2D *dyad, zVec2D *v1, zVec2D *v2)
{
  _zMat2DDyad( dyad, v1, v2 );
  return dyad;
}

/* ********************************************************** */
/* inverse of a 2x2 matrix
 * ********************************************************** */

/* determinant of a 2x2 matrix. */
double zMat2DDet(zMat2D *m)
{
  return _zMat2DDet( m );
}

/* inverse matrix of a 3x3 matrix. */
zMat2D *zMat2DInv(zMat2D *m, zMat2D *im)
{
  double det;

  if( zIsTiny( ( det = _zMat2DDet( m ) ) ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
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
zVec2D *zMulMat2DVec2D(zMat2D *m, zVec2D *v, zVec2D *mv)
{
  _zVec2DCreate( mv, m->c.xx*v->c.x+m->c.yx*v->c.y, m->c.xy*v->c.x+m->c.yy*v->c.y );
  return mv;
}

/* multiply a 2D vector by transpose of a 2x2 matrix. */
zVec2D *zMulMat2DTVec2D(zMat2D *m, zVec2D *v, zVec2D *mv)
{
  _zVec2DCreate( mv, m->c.xx*v->c.x+m->c.xy*v->c.y, m->c.yx*v->c.x+m->c.yy*v->c.y );
  return mv;
}

/* multiply a 2D vector by the inverse of a 2x2 matrix. */
zVec2D *zMulInvMat2DVec2D(zMat2D *m, zVec2D *v, zVec2D *mv)
{
  double det;

  if( zIsTiny( ( det = _zMat2DDet( m ) ) ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
    return NULL;
  }
  _zVec2DCreate( mv, (m->c.yy*v->c.x-m->c.yx*v->c.y)/det, (-m->c.xy*v->c.x+m->c.xx*v->c.y)/det );
  return mv;
}

/* ********************************************************** */
/* multiplication of a 2x2 matrix by another 2x2 matrix
 * ********************************************************** */

/* multiply two 2x2 matrices. */
zMat2D *zMulMat2DMat2D(zMat2D *m1, zMat2D *m2, zMat2D *m)
{
  _zMulMat2DMat2D( m1, m2, m );
  return m;
}

/* multiply a 2x2 matrix by transpose of another matrix from leftside. */
zMat2D *zMulMat2DTMat2D(zMat2D *m1, zMat2D *m2, zMat2D *m)
{
  _zMulMat2DTMat2D( m1, m2, m );
  return m;
}

/* multiply a 2x2 matrix by transpose of another from rightside. */
zMat2D *zMulMat2DMat2DT(zMat2D *m1, zMat2D *m2, zMat2D *m)
{
  _zMulMat2DMat2DT( m1, m2, m );
  return m;
}

/* multiply a 2x2 matrix by the inverse of another matrix from leftside. */
zMat2D *zMulInvMat2DMat2D(zMat2D *m1, zMat2D *m2, zMat2D *m)
{
  double det;

  if( zIsTiny( ( det = _zMat2DDet( m1 ) ) ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
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

/* rotate a 2x2 matrix with sine and cosine values. */
zMat2D *zMat2DRotSC(zMat2D *m, double s, double c, zMat2D *rm)
{
  _zMat2DRotSC( m, s, c, rm );
  return rm;
}

/* rotate a 2x2 matrix. */
zMat2D *zMat2DRot(zMat2D *m, double theta, zMat2D *rm)
{
  double s, c;
  zSinCos( theta, &s, &c );
  _zMat2DRotSC( m, s, c, rm );
  return rm;
}

/* error vector between two 2x2 attitude matrices. */
double zMat2DError(zMat2D *m1, zMat2D *m2)
{
  return _zMat2DError( m1, m2 );
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* scan a 2x2 matrix from a file. */
zMat2D *zMat2DFScan(FILE *fp, zMat2D *m)
{
  register int i, j;

  for( i=0; i<2; i++ )
    for( j=0; j<2; j++ )
      m->e[j][i] = zFDouble( fp );
  return m;
}

/* print a 2x2 matrix to a file. */
void zMat2DFPrint(FILE *fp, zMat2D *m)
{
  if( !m ) return;
  fprintf( fp, "{\n" );
  fprintf( fp, " %.10g, %.10g\n", m->c.xx, m->c.yx );
  fprintf( fp, " %.10g, %.10g\n", m->c.xy, m->c.yy );
  fprintf( fp, "}\n" );
}
