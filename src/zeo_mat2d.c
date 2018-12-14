/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat2d.h - 2D matrix.
 */

#include <zeo/zeo_mat2d.h>

/* ********************************************************** */
/* CLASS: zMat2D
 * 2D matrix class
 * ********************************************************** */

/* OBJECT:
 * zmat2Dzero, zmat2Dident
 * - 2D zero matrix and identity matrix.
 */
const zMat2D zmat2Dzero  = { 0, 0, 0, 0 };
const zMat2D zmat2Dident = { 1, 0, 0, 1 };

/* zMat2DCreate
 * - create 2D matrix.
 */
double *zMat2DCreate(zMat2D m, double a11, double a12, double a21, double a22)
{
  m[0] = a11; m[1] = a12; m[2] = a21; m[3] = a22;
  return m;
}

/* zMat2DCopy
 * - copy a 2D matrix to the other.
 */
double *zMat2DCopy(zMat2D src, zMat2D dest)
{
  return (double *)memcpy( dest, src, sizeof(zMat2D) );
}

/* zMat2DT
 * - transpose a 2D matrix.
 */
double *zMat2DT(zMat2D m, zMat2D tm)
{
  return zMat2DCreate( tm, m[0], m[2], m[1], m[3] );
}

/* zMat2DRow
 * - abstract row vector from 2D matrix.
 */
void zMat2DRow(zMat2D m, zVec2D r1, zVec2D r2)
{
  if( r1 ) zVec2DCreate( r1, m[0], m[1] );
  if( r2 ) zVec2DCreate( r2, m[2], m[3] );
}

/* zMat2DCol
 * - abstract column vector from 2D matrix.
 */
void zMat2DCol(zMat2D m, zVec2D c1, zVec2D c2)
{
  if( c1 ) zVec2DCreate( c1, m[0], m[2] );
  if( c2 ) zVec2DCreate( c2, m[1], m[3] );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* zMat2DAdd
 * - add two 2D matrices.
 */
double *zMat2DAdd(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m, m1[0]+m2[0], m1[1]+m2[1], m1[2]+m2[2], m1[3]+m2[3] );
}

/* zMat2DSub
 * - subtract a 2D matrix from another.
 */
double *zMat2DSub(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m, m1[0]-m2[0], m1[1]-m2[1], m1[2]-m2[2], m1[3]-m2[3] );
}

/* zMat2DRev
 * - reverse 2D matrix.
 */
double *zMat2DRev(zMat2D m, zMat2D rm)
{
  return zMat2DCreate( rm, -m[0], -m[1], -m[2], -m[3] );
}

/* zMat2DMul
 * - multiply 2D matrix by a scalar.
 */
double *zMat2DMul(zMat2D m, double k, zMat2D mm)
{
  return zMat2DCreate( mm, m[0]*k, m[1]*k, m[2]*k, m[3]*k );
}

/* zMat2DDiv
 * - divide 2D matrix by a scalar.
 */
double *zMat2DDiv(zMat2D m, double k, zMat2D dm)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  return zMat2DMul( m, 1.0/k, dm );
}

/* zMat2DCat
 * - concatenate 2D matrix.
 */
double *zMat2DCat(zMat2D m1, double k, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m, m1[0]+m2[0]*k, m1[1]+m2[1]*k, m1[2]+m2[2]*k, m1[3]+m2[3]*k );
}

/* zMat2DDyad
 * - dyadic product.
 */
double *zMat2DDyad(zVec2D v1, zVec2D v2, zMat2D dyad)
{
  return zMat2DCreate( dyad, v1[0]*v2[0], v1[0]*v2[1], v1[1]*v2[0], v1[1]*v2[1] );
}

/* ********************************************************** */
/* inverse of a 2x2 matrix
 * ********************************************************** */

/* zMat2DDet
 * - determinant of 2x2 matrix.
 */
double zMat2DDet(zMat2D m)
{
  return m[0]*m[3] - m[1]*m[2];
}

/* zMat2DInv
 * - inverse matrix of 3x3 matrix.
 */
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

/* zMulMatVec2D
 * - multiply a 2D vector by 2D matrix.
 */
double *zMulMatVec2D(zMat2D m, zVec2D v, zVec2D mv)
{
  return zVec2DCreate( mv, m[0]*v[0]+m[1]*v[1], m[2]*v[0]+m[3]*v[1] );
}

/* zMulMatTVec2D
 * - multiply a 2D vector by a transpose matrix.
 */
double *zMulMatTVec2D(zMat2D m, zVec2D v, zVec2D mv)
{
  return zVec2DCreate( mv, m[0]*v[0]+m[2]*v[1], m[1]*v[0]+m[3]*v[1] );
}

/* zMulInvMatVec2D
 * - multiply a 2D vector by the inverse of a 2x2 matrix.
 */
double *zMulInvMatVec2D(zMat2D m, zVec2D v, zVec2D mv)
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

/* zMulMatMat2D
 * - multiply two 2D matrices.
 */
double *zMulMatMat2D(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m,
    m1[0]*m2[0]+m1[1]*m2[2], m1[0]*m2[1]+m1[1]*m2[3],
    m1[2]*m2[0]+m1[3]*m2[2], m1[2]*m2[1]+m1[3]*m2[3] );
}

/* zMulMatTMat2D
 * - multiply a matrix by transpose matrix from leftside.
 */
double *zMulMatTMat2D(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m,
    m1[0]*m2[0]+m1[2]*m2[2], m1[0]*m2[1]+m1[2]*m2[3],
    m1[1]*m2[0]+m1[3]*m2[2], m1[1]*m2[1]+m1[3]*m2[3] );
}

/* zMulMatMatT2D
 * - multiply a matrix by transpose matrix from rightside.
 */
double *zMulMatMatT2D(zMat2D m1, zMat2D m2, zMat2D m)
{
  return zMat2DCreate( m,
    m1[0]*m2[0]+m1[1]*m2[1], m1[0]*m2[2]+m1[1]*m2[3],
    m1[2]*m2[0]+m1[3]*m2[1], m1[2]*m2[2]+m1[3]*m2[3] );
}

/* zMulInvMatMat2D
 * - multiply a matrix by the inverse of another matrix from leftside.
 */
double *zMulInvMatMat2D(zMat2D m1, zMat2D m2, zMat2D m)
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

/* zMat2DRotSC, zMat2DRot
 * - rotate matrix along x-axis.
 */
double *zMat2DRotSC(zMat2D m, double s, double c, zMat2D rm){
  return zMat2DCreate( rm,
    c*m[0]-s*m[2], c*m[1]-s*m[3], s*m[0]+c*m[2], s*m[1]+c*m[3] );
}
double *zMat2DRot(zMat2D m, double theta, zMat2D rm){
  double s, c;
  zSinCos( theta, &s, &c );
  return zMat2DRotSC( m, s, c, rm );
}

/* zMat2DError
 * - error vector between two attitude matrices.
 */
double zMat2DError(zMat2D m1, zMat2D m2)
{
  return atan2( m2[0]*m1[2]+m2[1]*m1[3]-m2[2]*m1[0]-m2[3]*m1[1], m2[0]*m1[0]+m2[1]*m1[1]+m2[2]*m1[2]+m2[3]*m1[3] );
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* zMat2DFRead
 * - input of 2D matrix from file.
 */
double *zMat2DFRead(FILE *fp, zMat2D m)
{
  register int i;

  for( i=0; i<4; i++ )
    m[i] = zFDouble( fp );
  return m;
}

/* zMat2DFWrite
 * - output of 2D matrix to file.
 */
void zMat2DFWrite(FILE *fp, zMat2D m)
{
  if( !m )
    fprintf( fp, "(null 2D matrix)\n" );
  else{
    fprintf( fp, "{\n" );
    fprintf( fp, " %.10g, %.10g\n", m[0], m[1] );
    fprintf( fp, " %.10g, %.10g\n", m[2], m[3] );
    fprintf( fp, "}\n" );
  }
}
