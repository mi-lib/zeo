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

/* 3D zero vector and unit vectors along (x,y,z) axis. */
#ifdef __cplusplus
const zVec2D zVec2D::zvec2Dzero = { { 0, 0 } };
const zVec2D zVec2D::zvec2Dx    = { { 1, 0 } };
const zVec2D zVec2D::zvec2Dy    = { { 0, 1 } };
#else
const zVec2D zvec2Dzero = { { 0, 0 } };
const zVec2D zvec2Dx    = { { 1, 0 } };
const zVec2D zvec2Dy    = { { 0, 1 } };
#endif

/* create a 2D vector. */
zVec2D *zVec2DCreate(zVec2D *v, double x, double y)
{
  _zVec2DCreate( v, x, y );
  return v;
}

/* copy a 2D vector. */
zVec2D *zVec2DCopy(const zVec2D *src, zVec2D *dest)
{
  _zVec2DCopy( src, dest );
  return dest;
}

/* create a 2D vector by the set of value for a polar expression. */
zVec2D *zVec2DCreatePolar(zVec2D *v, double r, double theta)
{
  _zVec2DCreatePolar( v, r, theta );
  return v;
}

/* check if two 2D vectors are exactly the same. */
bool zVec2DMatch(const zVec2D *v1, const zVec2D *v2)
{
  return _zVec2DMatch( v1, v2 );
}

/* check if two 2D vectors are equal. */
bool zVec2DEqual(const zVec2D *v1, const zVec2D *v2)
{
  return _zVec2DEqual( v1, v2 );
}

/* check if the 2D vector is negligibly small. */
bool zVec2DIsTol(const zVec2D *v, double tol)
{
  return _zVec2DIsTol( v, tol );
}

/* check if a 2D vector includes NaN or Inf components. */
bool zVec2DIsNan(const zVec2D *v)
{
  return zIsNan( v->c.x ) || zIsInf( v->c.x ) ||
         zIsNan( v->c.y ) || zIsInf( v->c.y );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* add two 2D vectors. */
zVec2D *zVec2DAdd(const zVec2D *v1, const zVec2D *v2, zVec2D *v)
{
  _zVec2DAdd( v1, v2, v );
  return v;
}

/* subtract a 2D vector from another. */
zVec2D *zVec2DSub(const zVec2D *v1, const zVec2D *v2, zVec2D *v)
{
  _zVec2DSub( v1, v2, v );
  return v;
}

/* reverse a 2D vector. */
zVec2D *zVec2DRev(const zVec2D *v, zVec2D *rv)
{
  _zVec2DRev( v, rv );
  return rv;
}

/* multiply a 2D vector by a scalar. */
zVec2D *zVec2DMul(const zVec2D *v, double k, zVec2D *mv)
{
  _zVec2DMul( v, k, mv );
  return mv;
}

/* divide a 2D vector by a scalar. */
zVec2D *zVec2DDiv(const zVec2D *v, double k, zVec2D *dv)
{
  if( k == 0 ){
    ZRUNERROR( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zVec2DMul( v, k, dv );
  return dv;
}

/* amplify a 2D vector by another. */
zVec2D *zVec2DAmp(const zVec2D *v1, const zVec2D *v2, zVec2D *v)
{
  _zVec2DAmp( v1, v2, v );
  return v;
}

/* demagnify a 2D vector by another. */
zVec2D *zVec2DDem(const zVec2D *v1, const zVec2D *v2, zVec2D *v)
{
  _zVec2DDem( v1, v2, v );
  return v;
}

/* concatenate a 2D vector with another. */
zVec2D *zVec2DCat(const zVec2D *v1, double k, const zVec2D *v2, zVec2D *v)
{
  _zVec2DCat( v1, k, v2, v );
  return v;
}

/* directly add a 2D vector to another. */
zVec2D *zVec2DAddDRC(zVec2D *v1, const zVec2D *v2)
{
  _zVec2DAddDRC( v1, v2 );
  return v1;
}

/* directly subtract a 2D vector from another. */
zVec2D *zVec2DSubDRC(zVec2D *v1, const zVec2D *v2)
{
  _zVec2DSubDRC( v1, v2 );
  return v1;
}

/* directly reverse a 2D vector. */
zVec2D *zVec2DRevDRC(zVec2D *v)
{
  _zVec2DRevDRC( v );
  return v;
}

/* directly multiply a 2D vector by a scalar value. */
zVec2D *zVec2DMulDRC(zVec2D *v, double k)
{
  _zVec2DMulDRC( v, k );
  return v;
}

/* directly divide a 2D vector by a scalar value. */
zVec2D *zVec2DDivDRC(zVec2D *v, double k)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zVec2DMulDRC( v, k );
  return v;
}

/* directly amplify a 2D vector by another. */
zVec2D *zVec2DAmpDRC(zVec2D *v1, const zVec2D *v2)
{
  _zVec2DAmpDRC( v1, v2 );
  return v1;
}

/* directly demagnify a 2D vector by another. */
zVec2D *zVec2DDemDRC(zVec2D *v1, const zVec2D *v2)
{
  _zVec2DDemDRC( v1, v2 );
  return v1;
}

/* directly concatenate a 2D vector with another. */
zVec2D *zVec2DCatDRC(zVec2D *v1, double k, const zVec2D *v2)
{
  _zVec2DCatDRC( v1, k, v2 );
  return v1;
}

/* inner product of two 2D vectors. */
double zVec2DInnerProd(const zVec2D *v1, const zVec2D *v2)
{
  return _zVec2DInnerProd( v1, v2 );
}

/* outer product of two 2D vectors. */
double zVec2DOuterProd(const zVec2D *v1, const zVec2D *v2)
{
  return _zVec2DOuterProd( v1, v2 );
}

/* squared norm of a 2D vector. */
double zVec2DSqrNorm(const zVec2D *v)
{
  return _zVec2DInnerProd( v, v );
}

/* distance between 2 points. */
double zVec2DSqrDist(const zVec2D *v1, const zVec2D *v2)
{
  zVec2D v;

  return zVec2DSqrNorm( zVec2DSub( v1, v2, &v ) );
}

/* normalize a 2D vector without checking vector size. */
double zVec2DNormalizeNC(const zVec2D *v, zVec2D *nv)
{
  double l, k;

  k = 1.0 / ( l = zVec2DNorm(v) );
  _zVec2DMul( v, k, nv );
  return l;
}

/* normalize a 2D vector. */
double zVec2DNormalize(const zVec2D *v, zVec2D *nv)
{
  double l, k;

  if( zVec2DIsTiny( v ) ){
    ZRUNWARN( ZEO_ERR_VEC_ZERONORM );
    return -1;
  }
  k = 1.0 / ( l = zVec2DNorm(v) );
  _zVec2DMul( v, k, nv );
  return l;
}

/* ********************************************************** */
/* geometry
 * ********************************************************** */

/* interior division of two points. */
zVec2D *zVec2DInterDiv(const zVec2D *v1, const zVec2D *v2, double ratio, zVec2D *v)
{
  _zVec2DSub( v2, v1, v );
  _zVec2DCat( v1, ratio, v, v );
  return v;
}

/* middle point of two points. */
zVec2D *zVec2DMid(const zVec2D *v1, const zVec2D *v2, zVec2D *v)
{
  return zVec2DInterDiv( v1, v2, 0.5, v );
}

/* angle between two vectors. */
double zVec2DAngle(const zVec2D *v1, const zVec2D *v2)
{
  return atan2( _zVec2DOuterProd(v1,v2), _zVec2DInnerProd(v1,v2) );
}

/* project a 2D vector. */
zVec2D *zVec2DProj(const zVec2D *v, const zVec2D *n, zVec2D *pv)
{
  zVec2D d;

  if( zVec2DNormalize( n, &d ) < 0 ) return NULL;
  _zVec2DMul( &d, _zVec2DInnerProd(&d,v), pv );
  return pv;
}

/* rotate a 2D vector. */
zVec2D *zVec2DRot(const zVec2D *v, const double angle, zVec2D *rv)
{
  double s, c;

  zSinCos( angle, &s, &c );
  _zVec2DCreate( rv, c*v->c.x-s*v->c.y, s*v->c.x+c*v->c.y );
  return rv;
}

/* rotate a 2D vector at 90 degree clockwize. */
zVec2D *zVec2DRot90CW(const zVec2D *src, zVec2D *dest)
{
  _zVec2DRot90CW( src, dest );
  return dest;
}

/* rotate a 2D vector at 90 degree counterclockwize. */
zVec2D *zVec2DRot90CCW(const zVec2D *src, zVec2D *dest)
{
  _zVec2DRot90CCW( src, dest );
  return dest;
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 2D vector from a ZTK format processor. */
zVec2D *zVec2DFromZTK(zVec2D *v, ZTK *ztk)
{
  v->c.x = ZTKDouble(ztk);
  v->c.y = ZTKDouble(ztk);
  return v;
}

/* scan a 2D vector from a file. */
zVec2D *zVec2DFScan(FILE *fp, zVec2D *v)
{
  zFDouble( fp, &v->c.x );
  zFDouble( fp, &v->c.y );
  return v;
}

/* print a 2D vector data to a file. */
void zVec2DDataFPrint(FILE *fp, const zVec2D *v)
{
  if( !v ) return;
  fprintf( fp, " %.10g, %.10g", v->c.x, v->c.y );
}

/* print a 2D vector data with the new line to a file. */
void zVec2DDataNLFPrint(FILE *fp, const zVec2D *v)
{
  if( !v ) return;
  zVec2DDataFPrint( fp, v );
  fprintf( fp, "\n" );
}

/* print a 2D vector to a file. */
void zVec2DFPrint(FILE *fp, const zVec2D *v)
{
  fprintf( fp, "(" );
  if( !v )
    fprintf( fp, "null 2D vector\n" );
  else
    fprintf( fp, " %.10g, %.10g ", v->c.x, v->c.y );
  fprintf( fp, ")\n" );
}
