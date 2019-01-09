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
double *zMat2DCreate(zMat2D m, double a11, double a12, double a21, double a22)
{
  m[0] = a11; m[1] = a12; m[2] = a21; m[3] = a22;
  return m;
}

/* copy a 2x2 matrix to the other. */
double *zMat2DCopy(zMat2D src, zMat2D dest)
{
  return (double *)memcpy( dest, src, sizeof(zMat2D) );
}

/* transpose a 2x2 matrix. */
double *zMat2DT(zMat2D m, zMat2D tm)
{
  return zMat2DCreate( tm, m[0], m[2], m[1], m[3] );
}

/* abstract row vector from a 2x2 matrix. */
void zMat2DRow(zMat2D m, zVec2D r1, zVec2D r2)
{
  if( r1 ) zVec2DCreate( r1, m[0], m[1] );
  if( r2 ) zVec2DCreate( r2, m[2], m[3] );
}

/* abstract column vector from a 2x2 matrix. */
void zMat2DCol(zMat2D m, zVec2D c1, zVec2D c2)
{
  if( c1 ) zVec2DCreate( c1, m[0], m[2] );
  if( c2 ) zVec2DCreate( c2, m[1], m[3] );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* add two 2x2 matrices. */
double *zMat2DAdd(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m, m1[0]+m2[0], m1[1]+m2[1], m1[2]+m2[2], m1[3]+m2[3] );
}

/* subtract a 2x2 matrix from another. */
double *zMat2DSub(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m, m1[0]-m2[0], m1[1]-m2[1], m1[2]-m2[2], m1[3]-m2[3] );
}

/* reverse a 2x2 matrix. */
double *zMat2DRev(zMat2D m, zMat2D rm)
{
  return zMat2DCreate( rm, -m[0], -m[1], -m[2], -m[3] );
}

/* multiply a 2x2 matrix by a scalar. */
double *zMat2DMul(zMat2D m, double k, zMat2D mm)
{
  return zMat2DCreate( mm, m[0]*k, m[1]*k, m[2]*k, m[3]*k );
}

/* divide a 2x2 matrix by a scalar. */
double *zMat2DDiv(zMat2D m, double k, zMat2D dm)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  return zMat2DMul( m, 1.0/k, dm );
}

/* concatenate a 2x2 matrix with another. */
double *zMat2DCat(zMat2D m1, double k, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m, m1[0]+m2[0]*k, m1[1]+m2[1]*k, m1[2]+m2[2]*k, m1[3]+m2[3]*k );
}

/* dyadic product of two 2D vectors. */
double *zMat2DDyad(zMat2D dyad, zVec2D v1, zVec2D v2)
{
  return zMat2DCreate( dyad, v1[0]*v2[0], v1[0]*v2[1], v1[1]*v2[0], v1[1]*v2[1] );
}

/* ********************************************************** */
/* inverse of a 2x2 matrix
 * ********************************************************** */

/* determinant of a 2x2 matrix. */
double zMat2DDet(zMat2D m)
{
  return m[0]*m[3] - m[1]*m[2];
}

/* inverse matrix of a 3x3 matrix. */
double *zMat2DInv(zMat2D m, zMat2D im)
{
  double det;

  if( zIsTiny( ( det = m[0]*m[3] - m[1]*m[2] ) ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
    return NULL;
  }
  det = 1.0 / det;
  return zMat2DCreate( im, m[3]*det, -m[1]*det, -m[2]*det, m[0]*det );
}

/* ********************************************************** */
/* multiplication of a 2D vector by a 2x2 matrix
 * ********************************************************** */

/* multiply a 2D vector by a 2x2 matrix. */
double *zMulMat2DVec2D(zMat2D m, zVec2D v, zVec2D mv)
{
  return zVec2DCreate( mv, m[0]*v[0]+m[1]*v[1], m[2]*v[0]+m[3]*v[1] );
}

/* multiply a 2D vector by transpose of a 2x2 matrix. */
double *zMulMat2DTVec2D(zMat2D m, zVec2D v, zVec2D mv)
{
  return zVec2DCreate( mv, m[0]*v[0]+m[2]*v[1], m[1]*v[0]+m[3]*v[1] );
}

/* zMulInvMat2DVec2D
 * - multiply a 2D vector by the inverse of a 2x2 matrix.
 */
double *zMulInvMat2DVec2D(zMat2D m, zVec2D v, zVec2D mv)
{
  double delta;

  if( zIsTiny( ( delta = zMat2DDet( m ) ) ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
    return NULL;
  }
  return zVec2DCreate( mv,
    (m[3]*v[0]-m[1]*v[1])/delta, (-m[2]*v[0]+m[0]*v[1])/delta );
}

/* ********************************************************** */
/* multiplication of a 2x2 matrix by another 2x2 matrix
 * ********************************************************** */

/* multiply two 2x2 matrices. */
double *zMulMat2DMat2D(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m,
    m1[0]*m2[0]+m1[1]*m2[2], m1[0]*m2[1]+m1[1]*m2[3],
    m1[2]*m2[0]+m1[3]*m2[2], m1[2]*m2[1]+m1[3]*m2[3] );
}

/* multiply a 2x2 matrix by transpose of another matrix from leftside. */
double *zMulMat2DTMat2D(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m,
    m1[0]*m2[0]+m1[2]*m2[2], m1[0]*m2[1]+m1[2]*m2[3],
    m1[1]*m2[0]+m1[3]*m2[2], m1[1]*m2[1]+m1[3]*m2[3] );
}

/* multiply a 2x2 matrix by transpose of another from rightside. */
double *zMulMat2DMat2DT(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m,
    m1[0]*m2[0]+m1[1]*m2[1], m1[0]*m2[2]+m1[1]*m2[3],
    m1[2]*m2[0]+m1[3]*m2[1], m1[2]*m2[2]+m1[3]*m2[3] );
}

/* multiply a 2x2 matrix by the inverse of another matrix from leftside. */
double *zMulInvMat2DMat2D(zMat2D m1, zMat2D m2, zMat2D m)
{
  double delta;

  if( zIsTiny( ( delta = zMat2DDet( m1 ) ) ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
    return NULL;
  }
  return zMat2DCreate( m,
    (m1[3]*m2[0]-m1[1]*m2[2])/delta, (m1[3]*m2[1]-m1[1]*m2[3])/delta,
   (-m1[2]*m2[0]+m1[0]*m2[2])/delta,(-m1[2]*m2[1]+m1[0]*m2[3])/delta );
}

/* ********************************************************** */
/* rotation
 * ********************************************************** */

/* rotate a 2x2 matrix about z-axis. */
double *zMat2DRotSC(zMat2D m, double s, double c, zMat2D rm){
  return zMat2DCreate( rm,
    c*m[0]-s*m[2], c*m[1]-s*m[3], s*m[0]+c*m[2], s*m[1]+c*m[3] );
}
double *zMat2DRot(zMat2D m, double theta, zMat2D rm){
  double s, c;
  zSinCos( theta, &s, &c );
  return zMat2DRotSC( m, s, c, rm );
}

/* error vector between two 2x2 attitude matrices. */
double zMat2DError(zMat2D m1, zMat2D m2)
{
  return atan2( m2[0]*m1[2]+m2[1]*m1[3]-m2[2]*m1[0]-m2[3]*m1[1], m2[0]*m1[0]+m2[1]*m1[1]+m2[2]*m1[2]+m2[3]*m1[3] );
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* input a 2x2 matrix from file. */
double *zMat2DFRead(FILE *fp, zMat2D m)
{
  register int i;

  for( i=0; i<4; i++ )
    m[i] = zFDouble( fp );
  return m;
}

/* output a 2x2 matrix to file. */
void zMat2DFWrite(FILE *fp, zMat2D m)
{
  if( !m )
    fprintf( fp, "(null 2x2 matrix)\n" );
  else{
    fprintf( fp, "{\n" );
    fprintf( fp, " %.10g, %.10g\n", m[0], m[1] );
    fprintf( fp, " %.10g, %.10g\n", m[2], m[3] );
    fprintf( fp, "}\n" );
  }
}
