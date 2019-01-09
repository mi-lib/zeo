/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec6d - 6D spatial vector.
 */

#include <zeo/zeo_vec6d.h>

/* ********************************************************** */
/* CLASS: zVec6D
 * 6D quasi vector class - union of linear/angular vector
 * ********************************************************** */

/* OBJECT:
 * zvec6Dzero, zvec6Dlinx, zvec6Dliny, zvec6Dlinz
 * zvec6Dangx, zvec6Dangy, zvec6Dangz
 * - 6D zero vector and unit vectors along (x,y,z) axis.
 */
const zVec6D zvec6Dzero = { { 0, 0, 0, 0, 0, 0 } };
const zVec6D zvec6Dlinx = { { 1, 0, 0, 0, 0, 0 } };
const zVec6D zvec6Dliny = { { 0, 1, 0, 0, 0, 0 } };
const zVec6D zvec6Dlinz = { { 0, 0, 1, 0, 0, 0 } };
const zVec6D zvec6Dangx = { { 0, 0, 0, 1, 0, 0 } };
const zVec6D zvec6Dangy = { { 0, 0, 0, 0, 1, 0 } };
const zVec6D zvec6Dangz = { { 0, 0, 0, 0, 0, 1 } };

/* zVec6DCreate
 * - create 6D vector.
 */
zVec6D *zVec6DCreate(zVec6D *v, double x, double y, double z, double xa, double ya, double za)
{
  _zVec6DCreate( v, x, y, z, xa, ya, za );
  return v;
}

/* zVec6DFromVec3D
 * - convert from a set of two 3D vectors to 6D vector.
 */
zVec6D *zVec6DFromVec3D(zVec6D *v, zVec3D *vl, zVec3D *va)
{
  _zVec6DFromVec3D( v, vl, va );
  return v;
}

/* zVec6DMatch
 * - check if the two 6D vectors match each other.
 */
bool zVec6DMatch(zVec6D *v1, zVec6D *v2)
{
  return _zVec6DMatch( v1, v2 );
}

/* zVec6DEqual
 * - check if the two 6D vectors are equal.
 */
bool zVec6DEqual(zVec6D *v1, zVec6D *v2)
{
  return _zVec6DEqual( v1, v2 );
}

/* zVec6DIsTol
 * - check if the 6D vector is tiny enough.
 */
bool zVec6DIsTol(zVec6D *v, double tol)
{
  return _zVec6DIsTol( v, tol );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* zVec6DAdd
 * - add two 6D vectors.
 */
zVec6D *zVec6DAdd(zVec6D *v1, zVec6D *v2, zVec6D *v)
{
  _zVec6DAdd( v1, v2, v );
  return v;
}

/* zVec6DSub
 * - subtract a 6D vector from another.
 */
zVec6D *zVec6DSub(zVec6D *v1, zVec6D *v2, zVec6D *v)
{
  _zVec6DSub( v1, v2, v );
  return v;
}

/* zVec6DRev
 * - reverse 6D vector.
 */
zVec6D *zVec6DRev(zVec6D *v, zVec6D *rv)
{
  _zVec6DRev( v, rv );
  return rv;
}

/* zVec6DMul
 * - multiply 6D vector by a scalar value.
 */
zVec6D *zVec6DMul(zVec6D *v, double k, zVec6D *mv)
{
  _zVec6DMul( v, k, mv );
  return mv;
}

/* zVec6DDiv
 * - divide 6D vector by a scalar value.
 */
zVec6D *zVec6DDiv(zVec6D *v, double k, zVec6D *dv)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zVec3DMul( zVec6DLin(v), k, zVec6DLin(dv) );
  _zVec3DMul( zVec6DAng(v), k, zVec6DAng(dv) );
  return dv;
}

/* zVec6DCat
 * - concatenate 6D vector.
 */
zVec6D *zVec6DCat(zVec6D *v1, double k, zVec6D *v2, zVec6D *v)
{
  _zVec6DCat( v1, k, v2, v );
  return v;
}

/* zVec6DInnerProd
 * - inner product of two 6D vectors.
 */
double zVec6DInnerProd(zVec6D *v1, zVec6D *v2)
{
  return _zVec6DInnerProd( v1, v2 );
}

/* zVec6DLinShift
 * - shift velocity type of 6D vector.
 */
zVec6D *zVec6DLinShift(zVec6D *src, zVec3D *pos, zVec6D *dest)
{
  zVec3D v;

  _zVec3DOuterProd( zVec6DAng(src), pos, &v );
  _zVec3DAdd( zVec6DLin(src), &v, zVec6DLin(dest) );
  zVec3DCopy( zVec6DAng(src), zVec6DAng(dest) );
  return dest;
}

/* zVec6DLinShiftDRC
 * - shift velocity type 6D vector directly.
 */
zVec6D *zVec6DLinShiftDRC(zVec6D *vec, zVec3D *pos)
{
  zVec3D v;

  _zVec3DOuterProd( zVec6DAng(vec), pos, &v );
  _zVec3DAddDRC( zVec6DLin(vec), &v );
  return vec;
}

/* zVec6DAngShift
 * - shift force type of 6D vector.
 */
zVec6D *zVec6DAngShift(zVec6D *src, zVec3D *pos, zVec6D *dest)
{
  zVec3D n;

  _zVec3DOuterProd( pos, zVec6DLin(src), &n );
  _zVec3DAdd( zVec6DAng(src), &n, zVec6DAng(dest) );
  zVec3DCopy( zVec6DLin(src), zVec6DLin(dest) );
  return dest;
}

/* zVec6DAngShiftDRC
 * - shift force type of 6D vector directly.
 */
zVec6D *zVec6DAngShiftDRC(zVec6D *vec, zVec3D *pos)
{
  zVec3D n;

  _zVec3DOuterProd( pos, zVec6DLin(vec), &n );
  _zVec3DAddDRC( zVec6DAng(vec), &n );
  return vec;
}

/* ********************************************************** */
/* differential kinematics
 * ********************************************************** */

/* zVec6DDif
 * - numerical differentiation of 6D vector.
 */
zVec6D *zVec6DDif(zVec6D *v, zVec6D *vnew, double dt, zVec6D *vel)
{
  _zVec6DSub( vnew, v, vel );
  return zVec6DDivDRC( vel, dt );
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* zVec6DFRead
 * - input of 6D vector from file.
 */
zVec6D *zVec6DFRead(FILE *fp, zVec6D *v)
{
  register int i;

  for( i=zX; i<=zZA; i++ )
    v->e[i] = zFDouble( fp );
  return v;
}

/* zVec6DDataFWrite
 * - output of 6D vector data to file.
 */
zVec6D *zVec6DDataFWrite(FILE *fp, zVec6D *v)
{
  if( !v ) return NULL;
  fprintf( fp, " %.10g %.10g %.10g %.10g %.10g %.10g",
    v->e[zX], v->e[zY], v->e[zZ], v->e[zXA], v->e[zYA], v->e[zZA] );
  return v;
}

/* zVec6DDataNLFWrite
 * - output of 6D vector data with the new line to file.
 */
zVec6D *zVec6DDataNLFWrite(FILE *fp, zVec6D *v)
{
  if( !zVec6DDataFWrite( fp, v ) ) return NULL;
  fprintf( fp, "\n" );
  return v;
}

/* zVec6DFWrite
 * - output of 6D vector to file.
 */
zVec6D *zVec6DFWrite(FILE *fp, zVec6D *v)
{
  if( !v ){
    fprintf( fp, "(null 6D vector)\n" );
    return NULL;
  }
  zVec3DFWrite( fp, zVec6DLin(v) );
  zVec3DFWrite( fp, zVec6DAng(v) );
  return v;
}
