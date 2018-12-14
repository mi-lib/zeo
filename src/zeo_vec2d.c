/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec2d - 2D vector.
 */

#include <zeo/zeo_vec2d.h>

/* ********************************************************** */
/* CLASS: zVec2D
 * 2D vector class
 * ********************************************************** */

/* OBJECT:
 * zvec2Dzero, zvec2Dx, zvec2Dy
 * - 2D zero vector and unit vectors along (x,y) axis.
 */
const zVec2D zvec2Dzero = { 0, 0 };
const zVec2D zvec2Dx    = { 1, 0 };
const zVec2D zvec2Dy    = { 0, 1 };

/* zVec2DCreate
 * - create a 2D vector.
 */
double *zVec2DCreate(zVec2D v, double x, double y)
{
  v[zX] = x;
  v[zY] = y;
  return v;
}

/* zVec2DCopy
 * - copy a 2D vector.
 */
double *zVec2DCopy(zVec2D src, zVec2D dest)
{
  dest[zX] = src[zX];
  dest[zY] = src[zY];
  return dest;
}

/* zVec2DCreatePolar
 * - create a 2D vector by the set of value for a polar
 *   expression.
 */
double *zVec2DCreatePolar(zVec2D v, double r, double theta)
{
  return zVec2DCreate( v, r*cos(theta), r*sin(theta) );
}

/* zVec2DEqual
 * - check if the two 2D vectors are equal.
 */
bool zVec2DEqual(zVec2D v1, zVec2D v2)
{
  return v1[zX] == v2[zX] && v1[zY] == v2[zY];
}

/* zVec2DIsTol
 * - check if the 2D vector is tiny enough.
 */
bool zVec2DIsTol(zVec2D v, double tol)
{
  return zIsTol( v[zX], tol ) && zIsTol( v[zY], tol );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* zVec2DAdd
 * - add two 2D vectors.
 */
double *zVec2DAdd(zVec2D v1, zVec2D v2, zVec2D v)
{
  return zVec2DCreate( v, v1[zX] + v2[zX], v1[zY] + v2[zY] );
}

/* zVec2DSub
 * - subtract a 2D vector from another.
 */
double *zVec2DSub(zVec2D v1, zVec2D v2, zVec2D v)
{
  return zVec2DCreate( v, v1[zX] - v2[zX], v1[zY] - v2[zY] );
}

/* zVec2DRev
 * - reverse a 2D vector.
 */
double *zVec2DRev(zVec2D v, zVec2D rv)
{
  return zVec2DCreate( rv, -v[zX], -v[zY] );
}

/* zVec2DMul
 * - multiply a 2D vector by a scalar.
 */
double *zVec2DMul(zVec2D v, double k, zVec2D mv)
{
  return zVec2DCreate( mv, v[zX]*k, v[zY]*k );
}

/* zVec2DDiv
 * - divide a 2D vector by a scalar.
 */
double *zVec2DDiv(zVec2D v, double k, zVec2D dv)
{
  if( k == 0 ){
    ZRUNERROR( ZEO_ERR_ZERODIV );
    return NULL;
  }
  return zVec2DCreate( dv, v[zX]/k, v[zY]/k );
}

/* zVec2DCat
 * - concatenate 2D vector.
 */
double *zVec2DCat(zVec2D v1, double k, zVec2D v2, zVec2D v)
{
  return zVec2DCreate( v, v1[zX]+v2[zX]*k, v1[zY]+v2[zY]*k );
}

/* zVec2DSqrNorm
 * - squared norm of 2D vector.
 */
double zVec2DSqrNorm(zVec2D v)
{
  return zVec2DInnerProd( v, v );
}

/* zVec2DSqrDist
 * - distance between 2 positions.
 */
double zVec2DSqrDist(zVec2D v1, zVec2D v2)
{
  zVec2D v;

  return zVec2DSqrNorm( zVec2DSub( v1, v2, v ) );
}

/* zVec2DNormalize
 * - normalize a 2D vector.
 */
double *zVec2DNormalize(zVec2D v, zVec2D nv)
{
  if( zVec2DIsTiny( v ) ){
    ZRUNERROR( ZEO_ERR_ZERONORM );
    return NULL;
  }
  return zVec2DDiv( v, zVec2DNorm(v), nv );
}

/* zVec2DInnerProd
 * - inner product of two 2D vectors.
 */
double zVec2DInnerProd(zVec2D v1, zVec2D v2)
{
  return v1[zX]*v2[zX] + v1[zY]*v2[zY];
}

/* zVec2DOuterProd
 * - outer product of two 2D vectors.
 */
double zVec2DOuterProd(zVec2D v1, zVec2D v2)
{
  return v1[zX]*v2[zY] - v1[zY]*v2[zX];
}

/* ********************************************************** */
/* geometry
 * ********************************************************** */

/* zVec2DInterDiv
 * - interior division of the two positions.
 */
double *zVec2DInterDiv(zVec2D v1, zVec2D v2, double ratio, zVec2D v)
{
  zVec2DSub( v2, v1, v );
  return zVec2DCat( v1, ratio, v, v );
}

/* zVec2DMid
 * - middle point of the two positions.
 */
double *zVec2DMid(zVec2D v1, zVec2D v2, zVec2D v)
{
  return zVec2DInterDiv( v1, v2, 0.5, v );
}

/* zVec2DAngle
 * - angle between the two vectors.
 */
double zVec2DAngle(zVec2D v1, zVec2D v2)
{
  return atan2( zVec2DOuterProd(v1,v2), zVec2DInnerProd(v1,v2) );
}

/* zVec2DProject
 * - projection of a vector.
 */
double *zVec2DProject(zVec2D v, zVec2D n, zVec2D pv)
{
  zVec2D d;

  if( !zVec2DNormalize( n, d ) ) return NULL;
  return zVec2DMul( d, zVec2DInnerProd(d,v), pv );
}

/* zVec2DRot
 * - rotate a 2D vector.
 */
double *zVec2DRot(zVec2D v, double angle, zVec2D rv)
{
  double s, c;

  zSinCos( angle, &s, &c );
  return zVec2DCreate( rv, c*v[zX]-s*v[zY], s*v[zX]+c*v[zY] );
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* zVec2DFRead
 * - input a 2D vector from file.
 */
double *zVec2DFRead(FILE *fp, zVec2D v)
{
  v[zX] = zFDouble( fp );
  v[zY] = zFDouble( fp );
  return v;
}

/* zVec2DDataFWrite
 * - output a 2D vector data to file.
 */
void zVec2DDataFWrite(FILE *fp, zVec2D v)
{
  if( !v ) return;
  fprintf( fp, " %.10g %.10g", v[zX], v[zY] );
}

/* zVec2DDataNLFWrite
 * - output a 2D vector data with the new line to file.
 */
void zVec2DDataNLFWrite(FILE *fp, zVec2D v)
{
  if( !v ) return;
  zVec2DDataFWrite( fp, v );
  fprintf( fp, "\n" );
}

/* zVec2DFWrite
 * - output a 2D vector to file.
 */
void zVec2DFWrite(FILE *fp, zVec2D v)
{
  fprintf( fp, "(" );
  if( !v )
    fprintf( fp, "null 2D vector\n" );
  else
    fprintf( fp, " %.10g, %.10g ", v[zX], v[zY] );
  fprintf( fp, ")\n" );
}
