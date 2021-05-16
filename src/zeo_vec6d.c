/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec6d - 6D spatial vector.
 */

#include <zeo/zeo_vec6d.h>

/* ********************************************************** */
/* CLASS: zVec6D
 * 6D spatial vector class - set of linear/angular vectors
 * ********************************************************** */

/* 6D zero vector and unit vectors along (x,y,z) axes. */
const zVec6D zvec6Dzero = { { 0, 0, 0, 0, 0, 0 } };
const zVec6D zvec6Dlinx = { { 1, 0, 0, 0, 0, 0 } };
const zVec6D zvec6Dliny = { { 0, 1, 0, 0, 0, 0 } };
const zVec6D zvec6Dlinz = { { 0, 0, 1, 0, 0, 0 } };
const zVec6D zvec6Dangx = { { 0, 0, 0, 1, 0, 0 } };
const zVec6D zvec6Dangy = { { 0, 0, 0, 0, 1, 0 } };
const zVec6D zvec6Dangz = { { 0, 0, 0, 0, 0, 1 } };

/* create a 6D vector. */
zVec6D *zVec6DCreate(zVec6D *v, double x, double y, double z, double xa, double ya, double za)
{
  _zVec6DCreate( v, x, y, z, xa, ya, za );
  return v;
}

/* convert a set of two 3D vectors to a 6D vector. */
zVec6D *zVec6DFromVec3D(zVec6D *v, zVec3D *vl, zVec3D *va)
{
  _zVec6DFromVec3D( v, vl, va );
  return v;
}

/* check if two 6D vectors match each other. */
bool zVec6DMatch(zVec6D *v1, zVec6D *v2)
{
  return _zVec6DMatch( v1, v2 );
}

/* check if two 6D vectors are equal. */
bool zVec6DEqual(zVec6D *v1, zVec6D *v2)
{
  return _zVec6DEqual( v1, v2 );
}

/* check if a 6D vector is tiny. */
bool zVec6DIsTol(zVec6D *v, double tol)
{
  return _zVec6DIsTol( v, tol );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* add two 6D vectors. */
zVec6D *zVec6DAdd(zVec6D *v1, zVec6D *v2, zVec6D *v)
{
  _zVec6DAdd( v1, v2, v );
  return v;
}

/* subtract a 6D vector from another. */
zVec6D *zVec6DSub(zVec6D *v1, zVec6D *v2, zVec6D *v)
{
  _zVec6DSub( v1, v2, v );
  return v;
}

/* reverse a 6D vector. */
zVec6D *zVec6DRev(zVec6D *v, zVec6D *rv)
{
  _zVec6DRev( v, rv );
  return rv;
}

/* multiply a 6D vector by a scalar value. */
zVec6D *zVec6DMul(zVec6D *v, double k, zVec6D *mv)
{
  _zVec6DMul( v, k, mv );
  return mv;
}

/* divide a 6D vector by a scalar value. */
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

/* concatenate a 6D vector with another multiplied by a scalar value. */
zVec6D *zVec6DCat(zVec6D *v1, double k, zVec6D *v2, zVec6D *v)
{
  _zVec6DCat( v1, k, v2, v );
  return v;
}

/* inner product of two 6D vectors. */
double zVec6DInnerProd(zVec6D *v1, zVec6D *v2)
{
  return _zVec6DInnerProd( v1, v2 );
}

/* shift a velocity-type 6D vector. */
zVec6D *zVec6DLinShift(zVec6D *src, zVec3D *pos, zVec6D *dest)
{
  zVec3D v;

  _zVec3DOuterProd( zVec6DAng(src), pos, &v );
  _zVec3DAdd( zVec6DLin(src), &v, zVec6DLin(dest) );
  zVec3DCopy( zVec6DAng(src), zVec6DAng(dest) );
  return dest;
}

/* shift a velocity-type 6D vector directly. */
zVec6D *zVec6DLinShiftDRC(zVec6D *vec, zVec3D *pos)
{
  zVec3D v;

  _zVec3DOuterProd( zVec6DAng(vec), pos, &v );
  _zVec3DAddDRC( zVec6DLin(vec), &v );
  return vec;
}

/* shift a force-type 6D vector. */
zVec6D *zVec6DAngShift(zVec6D *src, zVec3D *pos, zVec6D *dest)
{
  zVec3D n;

  _zVec3DOuterProd( pos, zVec6DLin(src), &n );
  _zVec3DAdd( zVec6DAng(src), &n, zVec6DAng(dest) );
  zVec3DCopy( zVec6DLin(src), zVec6DLin(dest) );
  return dest;
}

/* shift a force-type 6D vector directly. */
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

/* numerical differentiation of a 6D vector. */
zVec6D *zVec6DDif(zVec6D *v, zVec6D *vnew, double dt, zVec6D *vel)
{
  _zVec6DSub( vnew, v, vel );
  return zVec6DDivDRC( vel, dt );
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 6D spatial vector from a ZTK format processor. */
zVec6D *zVec6DFromZTK(zVec6D *v, ZTK *ztk)
{
  v->e[0] = ZTKDouble(ztk);
  v->e[1] = ZTKDouble(ztk);
  v->e[2] = ZTKDouble(ztk);
  v->e[3] = ZTKDouble(ztk);
  v->e[4] = ZTKDouble(ztk);
  v->e[5] = ZTKDouble(ztk);
  return v;
}

/* scan a 6D vector from a file. */
zVec6D *zVec6DFScan(FILE *fp, zVec6D *v)
{
  register int i;

  for( i=zX; i<=zZA; i++ )
    zFDouble( fp, &v->e[i] );
  return v;
}

/* print a 6D vector data out to a file. */
zVec6D *zVec6DDataFPrint(FILE *fp, zVec6D *v)
{
  if( !v ) return NULL;
  fprintf( fp, " %.10g %.10g %.10g %.10g %.10g %.10g",
    v->e[zX], v->e[zY], v->e[zZ], v->e[zXA], v->e[zYA], v->e[zZA] );
  return v;
}

/* print a 6D vector data with the new line out to a file. */
zVec6D *zVec6DDataNLFPrint(FILE *fp, zVec6D *v)
{
  if( !zVec6DDataFPrint( fp, v ) ) return NULL;
  fprintf( fp, "\n" );
  return v;
}

/* print a 6D vector out to a file. */
zVec6D *zVec6DFPrint(FILE *fp, zVec6D *v)
{
  if( !v ){
    fprintf( fp, "(null 6D vector)\n" );
    return NULL;
  }
  zVec3DFPrint( fp, zVec6DLin(v) );
  zVec3DFPrint( fp, zVec6DAng(v) );
  return v;
}
