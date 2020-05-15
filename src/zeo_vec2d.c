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

/* create a 2D vector. */
zVec2D *zVec2DCreate(zVec2D *v, double x, double y)
{
  _zVec2DCreate( v, x, y );
  return v;
}

/* copy a 2D vector. */
zVec2D *zVec2DCopy(zVec2D *src, zVec2D *dest)
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
bool zVec2DMatch(zVec2D *v1, zVec2D *v2)
{
  return _zVec2DMatch( v1, v2 );
}

/* check if two 2D vectors are equal. */
bool zVec2DEqual(zVec2D *v1, zVec2D *v2)
{
  return _zVec2DEqual( v1, v2 );
}

/* check if the 2D vector is negligibly small. */
bool zVec2DIsTol(zVec2D *v, double tol)
{
  return _zVec2DIsTol( v, tol );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* add two 2D vectors. */
zVec2D *zVec2DAdd(zVec2D *v1, zVec2D *v2, zVec2D *v)
{
  _zVec2DAdd( v1, v2, v );
  return v;
}

/* subtract a 2D vector from another. */
zVec2D *zVec2DSub(zVec2D *v1, zVec2D *v2, zVec2D *v)
{
  _zVec2DSub( v1, v2, v );
  return v;
}

/* reverse a 2D vector. */
zVec2D *zVec2DRev(zVec2D *v, zVec2D *rv)
{
  _zVec2DRev( v, rv );
  return rv;
}

/* multiply a 2D vector by a scalar. */
zVec2D *zVec2DMul(zVec2D *v, double k, zVec2D *mv)
{
  _zVec2DMul( v, k, mv );
  return mv;
}

/* divide a 2D vector by a scalar. */
zVec2D *zVec2DDiv(zVec2D *v, double k, zVec2D *dv)
{
  if( k == 0 ){
    ZRUNERROR( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zVec2DMul( v, k, dv );
  return dv;
}

/* concatenate a 2D vector with another. */
zVec2D *zVec2DCat(zVec2D *v1, double k, zVec2D *v2, zVec2D *v)
{
  _zVec2DCat( v1, k, v2, v );
  return v;
}

/* inner product of two 2D vectors. */
double zVec2DInnerProd(zVec2D *v1, zVec2D *v2)
{
  return _zVec2DInnerProd( v1, v2 );
}

/* outer product of two 2D vectors. */
double zVec2DOuterProd(zVec2D *v1, zVec2D *v2)
{
  return _zVec2DOuterProd( v1, v2 );
}

/* squared norm of a 2D vector. */
double zVec2DSqrNorm(zVec2D *v)
{
  return _zVec2DInnerProd( v, v );
}

/* distance between 2 points. */
double zVec2DSqrDist(zVec2D *v1, zVec2D *v2)
{
  zVec2D v;

  return zVec2DSqrNorm( zVec2DSub( v1, v2, &v ) );
}

/* normalize a 2D vector. */
zVec2D *zVec2DNormalize(zVec2D *v, zVec2D *nv)
{
  if( zVec2DIsTiny( v ) ){
    ZRUNERROR( ZEO_ERR_ZERONORM );
    return NULL;
  }
  return zVec2DDiv( v, zVec2DNorm(v), nv );
}

/* ********************************************************** */
/* geometry
 * ********************************************************** */

/* interior division of two points. */
zVec2D *zVec2DInterDiv(zVec2D *v1, zVec2D *v2, double ratio, zVec2D *v)
{
  _zVec2DSub( v2, v1, v );
  _zVec2DCat( v1, ratio, v, v );
  return v;
}

/* middle point of two points. */
zVec2D *zVec2DMid(zVec2D *v1, zVec2D *v2, zVec2D *v)
{
  return zVec2DInterDiv( v1, v2, 0.5, v );
}

/* angle between two vectors. */
double zVec2DAngle(zVec2D *v1, zVec2D *v2)
{
  return atan2( _zVec2DOuterProd(v1,v2), _zVec2DInnerProd(v1,v2) );
}

/* project a 2D vector. */
zVec2D *zVec2DProj(zVec2D *v, zVec2D *n, zVec2D *pv)
{
  zVec2D d;

  if( !zVec2DNormalize( n, &d ) ) return NULL;
  _zVec2DMul( &d, _zVec2DInnerProd(&d,v), pv );
  return pv;
}

/* rotate a 2D vector. */
zVec2D *zVec2DRot(zVec2D *v, double angle, zVec2D *rv)
{
  double s, c;

  zSinCos( angle, &s, &c );
  _zVec2DCreate( rv, c*v->c.x-s*v->c.y, s*v->c.x+c*v->c.y );
  return rv;
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
  v->c.x = zFDouble( fp );
  v->c.y = zFDouble( fp );
  return v;
}

/* print a 2D vector data to a file. */
void zVec2DDataFPrint(FILE *fp, zVec2D *v)
{
  if( !v ) return;
  fprintf( fp, " %.10g, %.10g", v->c.x, v->c.y );
}

/* print a 2D vector data with the new line to a file. */
void zVec2DDataNLFPrint(FILE *fp, zVec2D *v)
{
  if( !v ) return;
  zVec2DDataFPrint( fp, v );
  fprintf( fp, "\n" );
}

/* print a 2D vector to a file. */
void zVec2DFPrint(FILE *fp, zVec2D *v)
{
  fprintf( fp, "(" );
  if( !v )
    fprintf( fp, "null 2D vector\n" );
  else
    fprintf( fp, " %.10g, %.10g ", v->c.x, v->c.y );
  fprintf( fp, ")\n" );
}
