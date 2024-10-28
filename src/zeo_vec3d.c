/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d - 3D vector.
 */

#include <zeo/zeo_vec3d.h>

/* ********************************************************** */
/* CLASS: zVec3D
 * 3D vector class
 * ********************************************************** */

/* 3D zero vector and unit vectors along (x,y,z) axis. */
#ifdef __cplusplus
const zVec3D zVec3D::zvec3Dzero = { { 0, 0, 0 } };
const zVec3D zVec3D::zvec3Dx = { { 1, 0, 0 } };
const zVec3D zVec3D::zvec3Dy = { { 0, 1, 0 } };
const zVec3D zVec3D::zvec3Dz = { { 0, 0, 1 } };
#else
const zVec3D zvec3Dzero = { { 0, 0, 0 } };
const zVec3D zvec3Dx    = { { 1, 0, 0 } };
const zVec3D zvec3Dy    = { { 0, 1, 0 } };
const zVec3D zvec3Dz    = { { 0, 0, 1 } };
#endif /* __cplusplus */

/* create a 3D vector. */
zVec3D *zVec3DCreate(zVec3D *v, double x, double y, double z)
{
  _zVec3DCreate( v, x, y, z );
  return v;
}

/* create a 3D vector by the set of value for a polar expression. */
zVec3D *zVec3DCreatePolar(zVec3D *v, double r, double theta, double phi)
{
  double rs;

  rs = r * sin( theta );
  _zVec3DCreate( v, rs*cos(phi), rs*sin(phi), r*cos(theta) );
  return v;
}

/* check if two 3D vectors match are exactly the same. */
bool zVec3DMatch(zVec3D *v1, zVec3D *v2)
{
  return _zVec3DMatch( v1, v2 );
}

/* check if two 3D vectors are equal. */
bool zVec3DEqual(zVec3D *v1, zVec3D *v2)
{
  return _zVec3DEqual( v1, v2 );
}

/* check if a 3D vector is small. */
bool zVec3DIsTol(zVec3D *v, double tol)
{
  return _zVec3DIsTol( v, tol );
}

/* check if a 3D vector includes NaN or Inf components. */
bool zVec3DIsNan(zVec3D *v)
{
  return zIsNan( v->c.x ) || zIsInf( v->c.x ) ||
         zIsNan( v->c.y ) || zIsInf( v->c.y ) ||
         zIsNan( v->c.z ) || zIsInf( v->c.z );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* add two 3D vectors. */
zVec3D *zVec3DAdd(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  _zVec3DAdd( v1, v2, v );
  return v;
}

/* subtract a 3D vector from another. */
zVec3D *zVec3DSub(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  _zVec3DSub( v1, v2, v );
  return v;
}

/* reverse a 3D vector. */
zVec3D *zVec3DRev(zVec3D *v, zVec3D *rv)
{
  _zVec3DRev( v, rv );
  return rv;
}

/* multiply a 3D vector by a scalar. */
zVec3D *zVec3DMul(zVec3D *v, double k, zVec3D *mv)
{
  _zVec3DMul( v, k, mv );
  return mv;
}

/* divide a 3D vector by a scalar. */
zVec3D *zVec3DDiv(zVec3D *v, double k, zVec3D *dv)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zVec3DMul( v, k, dv );
  return dv;
}

/* amplify a 3D vector by another. */
zVec3D *zVec3DAmp(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  _zVec3DAmp( v1, v2, v );
  return v;
}

/* demagnify a 3D vector by another. */
zVec3D *zVec3DDem(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  _zVec3DDem( v1, v2, v );
  return v;
}

/* concatenate a 3D vector with another. */
zVec3D *zVec3DCat(zVec3D *v1, double k, zVec3D *v2, zVec3D *v)
{
  _zVec3DCat( v1, k, v2, v );
  return v;
}

/* directly add a 3D vector to another. */
zVec3D *zVec3DAddDRC(zVec3D *v1, zVec3D *v2)
{
  _zVec3DAddDRC( v1, v2 );
  return v1;
}

/* directly subtract a 3D vector from another. */
zVec3D *zVec3DSubDRC(zVec3D *v1, zVec3D *v2)
{
  _zVec3DSubDRC( v1, v2 );
  return v1;
}

/* directly reverse a 3D vector. */
zVec3D *zVec3DRevDRC(zVec3D *v)
{
  _zVec3DRevDRC( v );
  return v;
}

/* directly multiply a 3D vector by a scalar value. */
zVec3D *zVec3DMulDRC(zVec3D *v, double k)
{
  _zVec3DMulDRC( v, k );
  return v;
}

/* directly divide a 3D vector by a scalar value. */
zVec3D *zVec3DDivDRC(zVec3D *v, double k)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zVec3DMulDRC( v, k );
  return v;
}

/* directly amplify a 3D vector by another. */
zVec3D *zVec3DAmpDRC(zVec3D *v1, zVec3D *v2)
{
  _zVec3DAmpDRC( v1, v2 );
  return v1;
}

/* directly demagnify a 3D vector by another. */
zVec3D *zVec3DDemDRC(zVec3D *v1, zVec3D *v2)
{
  _zVec3DDemDRC( v1, v2 );
  return v1;
}

/* directly concatenate a 3D vector with another. */
zVec3D *zVec3DCatDRC(zVec3D *v1, double k, zVec3D *v2)
{
  _zVec3DCatDRC( v1, k, v2 );
  return v1;
}

/* inner product of two 3D vectors. */
double zVec3DInnerProd(zVec3D *v1, zVec3D *v2)
{
  return _zVec3DInnerProd( v1, v2 );
}

/* outer product of two 3D vectors. */
zVec3D *zVec3DOuterProd(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  zVec3D v3; /* a temporary vector is necessary because v1 or v2 can be the same with v */

  _zVec3DOuterProd( v1, v2, &v3 );
  return zVec3DCopy( &v3, v );
}

/* norm of outer product of two 3D vectors. */
double zVec3DOuterProdNorm(zVec3D *v1, zVec3D *v2)
{
  zVec3D v;

  _zVec3DOuterProd( v1, v2, &v );
  return _zVec3DNorm( &v );
}

/* scalar triple product of three 3D vectors. */
double zVec3DGrassmannProd(zVec3D *v1, zVec3D *v2, zVec3D *v3)
{
  zVec3D v;

  _zVec3DOuterProd( v2, v3, &v );
  return _zVec3DInnerProd( v1, &v );
}

/* vector triple product of three 3D vectors. */
zVec3D *zVec3DTripleProd(zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v)
{
  _zVec3DTripleProd( v1, v2, v3, v );
  return v;
}

/* squared norm of a 3D vector. */
double zVec3DSqrNorm(zVec3D *v)
{
  return _zVec3DSqrNorm( v );
}

/* squared weighted norm of a 3D vector. */
double zVec3DWSqrNorm(zVec3D *v, zVec3D *w)
{
  return _zVec3DWSqrNorm( v, w );
}

/* distance between two positions. */
double zVec3DSqrDist(zVec3D *v1, zVec3D *v2)
{
  zVec3D dv;

  _zVec3DSub( v1, v2, &dv );
  return _zVec3DSqrNorm( &dv );
}

#define __zVec3DNormalize(v,nv) do{\
  double __l, __k;\
  __k = 1.0 / ( __l = zVec3DNorm(v) );\
  _zVec3DMul( v, __k, nv );\
  return __l;\
} while(0)

/* normalize a 3D vector without checking vector size. */
double zVec3DNormalizeNC(zVec3D *v, zVec3D *nv)
{
  __zVec3DNormalize( v, nv );
}

/* normalize a 3D vector. */
double zVec3DNormalize(zVec3D *v, zVec3D *nv)
{
  if( zVec3DIsTiny( v ) ){
    ZRUNWARN( ZEO_ERR_ZERONORM );
    return 0;
  }
  __zVec3DNormalize( v, nv );
}

/* ********************************************************** */
/* geometry
 * ********************************************************** */

/* interior division of two points. */
zVec3D *zVec3DInterDiv(zVec3D *v1, zVec3D *v2, double ratio, zVec3D *v)
{
  _zVec3DInterDiv( v1, v2, ratio, v );
  return v;
}

/* middle point of two points. */
zVec3D *zVec3DMid(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  _zVec3DMid( v1, v2, v );
  return v;
}

/* angle between two vectors. */
double zVec3DAngle(zVec3D *v1, zVec3D *v2, zVec3D *n)
{
  double c, s;
  zVec3D d;

  c = _zVec3DInnerProd( v1, v2 );
  _zVec3DOuterProd( v1, v2, &d );
  s = _zVec3DNorm( &d );
  if( n && _zVec3DInnerProd( &d, n ) < 0 ) s = -s;
  return atan2( s, c );
}

/* angle-axis error of two vectors. */
zVec3D *zVec3DAAError(zVec3D *v1, zVec3D *v2, zVec3D *aa)
{
  double c, s;
  zVec3D d;

  _zVec3DOuterProd( v1, v2, &d );
  if( zVec3DIsTiny( &d ) ){
    _zVec3DZero( aa );
  } else{
    c = _zVec3DInnerProd( v1, v2 );
    s = _zVec3DNorm( &d );
    _zVec3DMul( &d, atan2( s, c ) / s, aa );
  }
  return aa;
}

/* project a 3D vector onto another. */
zVec3D *zVec3DProj(zVec3D *v, zVec3D *n, zVec3D *pv)
{
  double l;

  if( _zVec3DIsTiny( n ) ){
    ZRUNWARN( ZEO_ERR_ZERONORM );
    return NULL;
  }
  l = _zVec3DInnerProd( n, v ) / _zVec3DSqrNorm( n );
  _zVec3DMul( n, l, pv );
  return pv;
}

/* create an orthonormal 3D vector. */
zVec3D *zVec3DOrthoNormal(zVec3D *v, zVec3D *ov)
{
  if( !zIsTiny( v->c.x ) || !zIsTiny( v->c.y ) )
    _zVec3DCreate( ov, v->c.y, -v->c.x, 0 );
  else
    _zVec3DCreate( ov, v->c.y - v->c.z, v->c.z - v->c.x, v->c.x - v->c.y );
  if( zVec3DIsTiny( ov ) ){
    ZRUNWARN( ZEO_ERR_ZERONORM );
    return NULL;
  }
  zVec3DNormalizeDRC( ov );
  return ov;
}

/* orthogonalize a 3D vector. */
zVec3D *zVec3DOrthogonalize(zVec3D *v, zVec3D *n, zVec3D *ov)
{
  double l;

  if( _zVec3DIsTiny( n ) ){
    ZRUNWARN( ZEO_ERR_ZERONORM );
    return NULL;
  }
  l = -_zVec3DInnerProd( n, v ) / _zVec3DSqrNorm( n );
  _zVec3DCat( v, l, n, ov );
  return ov;
}

/* create the orthogonal space of a 3D vector. */
bool zVec3DOrthoSpace(zVec3D *v, zVec3D *sv1, zVec3D *sv2)
{
  if( !zVec3DOrthoNormal( v, sv1 ) ) return false;
  _zVec3DOuterProd( v, sv1, sv2 );
  return true;
}

/* create the orthonormal space of a 3D vector. */
bool zVec3DOrthoNormalSpace(zVec3D *v, zVec3D *sv1, zVec3D *sv2)
{
  if( !zVec3DOrthoSpace( v, sv1, sv2 ) ) return false;
  zVec3DNormalizeDRC( v );
  zVec3DNormalizeDRC( sv1 );
  zVec3DNormalizeDRC( sv2 );
  return true;
}

/* rotate a 3D vector along an axis. */
zVec3D *zVec3DRot(zVec3D *v, zVec3D *aa, zVec3D *rv)
{
  zVec3D v0, v1, v2, n;
  double angle, s, c;

  if( _zVec3DIsTiny( aa ) ) return zVec3DCopy( v, rv );
  angle = zVec3DNormalize( aa, &n );
  zVec3DProj( v, &n, &v0 );
  _zVec3DSub( v, &v0, &v1 );
  _zVec3DOuterProd( &n, v, &v2 );
  _zSinCos( angle, &s, &c );
  _zVec3DCat( &v0, c, &v1, rv );
  _zVec3DCatDRC( rv, s, &v2 );
  return rv;
}

/* ********************************************************** */
/* differential kinematics
 * ********************************************************** */

/* numerical differentiation of a 3D vector. */
zVec3D *zVec3DDif(zVec3D *v, zVec3D *vnew, double dt, zVec3D *vel)
{
  _zVec3DSub( vnew, v, vel );
  return zVec3DDivDRC( vel, dt );
}

/* convert z-y-x Eulerian angle differential to angular velocity. */
zVec3D *zZYXVelToAngVel(zVec3D *zyxvel, zVec3D *zyx, zVec3D *angvel)
{
  double sa, ca, sb, cb;

  _zSinCos( zyx->c.x, &sa, &ca );
  _zSinCos( zyx->c.y, &sb, &cb );
  return zZYXVelToAngVelSC( zyxvel, sa, ca, sb, cb, angvel );
}

/* convert z-y-x Eulerian angle differential to angular velocity from sine/cosine sets. */
zVec3D *zZYXVelToAngVelSC(zVec3D *zyxvel, double sa, double ca, double sb, double cb, zVec3D *angvel)
{
  _zVec3DCreate( angvel,-sa*zyxvel->c.y + ca*cb*zyxvel->c.z,
                         ca*zyxvel->c.y + sa*cb*zyxvel->c.z,
                            zyxvel->c.x -    sb*zyxvel->c.z );
  return angvel;
}

/* convert angular velocity to z-y-x Eulerian angle differential. */
zVec3D *zAngVelToZYXVel(zVec3D *angvel, zVec3D *zyx, zVec3D *zyxvel)
{
  double sa, ca, sb, cb;

  _zSinCos( zyx->c.x, &sa, &ca );
  _zSinCos( zyx->c.y, &sb, &cb );
  return zAngVelToZYXVelSC( angvel, sa, ca, sb, cb, zyxvel );
}

/* convert angular velocity to z-y-x Eulerian angle differential from sine/cosine sets. */
zVec3D *zAngVelToZYXVelSC(zVec3D *angvel, double sa, double ca, double sb, double cb, zVec3D *zyxvel)
{
  double w;

  /* at the singular point, 'zyxvel' remains the same value. */
  if( zIsTiny(cb) ) return zyxvel;
  w = ( ca*angvel->c.x + sa*angvel->c.y ) / cb;
  _zVec3DCreate( zyxvel, w*sb + angvel->c.z,
                        -sa*angvel->c.x + ca*angvel->c.y,
                         w );
  return zyxvel;
}

/* convert z-y-z Eulerian angle differential to angular velocity. */
zVec3D *zZYZVelToAngVel(zVec3D *zyzvel, zVec3D *zyz, zVec3D *angvel)
{
  double sa, ca, sb, cb;

  _zSinCos( zyz->c.x, &sa, &ca );
  _zSinCos( zyz->c.y, &sb, &cb );
  return zZYZVelToAngVelSC( zyzvel, sa, ca, sb, cb, angvel );
}

/* convert z-y-z Eulerian angle differential to angular velocity from sine/cosine sets. */
zVec3D *zZYZVelToAngVelSC(zVec3D *zyzvel, double sa, double ca, double sb, double cb, zVec3D *angvel)
{
  _zVec3DCreate( angvel,-sa*zyzvel->c.y + ca*sb*zyzvel->c.z,
                         ca*zyzvel->c.y + sa*sb*zyzvel->c.z,
                            zyzvel->c.x +    cb*zyzvel->c.z );
  return angvel;
}

/* convert angular velocity to z-y-z Eulerian angle differential. */
zVec3D *zAngVelToZYZVel(zVec3D *angvel, zVec3D *zyz, zVec3D *zyzvel)
{
  double sa, ca, sb, cb;

  _zSinCos( zyz->c.x, &sa, &ca );
  _zSinCos( zyz->c.y, &sb, &cb );
  return zAngVelToZYZVelSC( angvel, sa, ca, sb, cb, zyzvel );
}

/* convert angular velocity to z-y-z Eulerian angle differential from sine/cosine sets. */
zVec3D *zAngVelToZYZVelSC(zVec3D *angvel, double sa, double ca, double sb, double cb, zVec3D *zyzvel)
{
  double w;

  /* at the singular point, 'zyzvel' remains the same value. */
  if( zIsTiny(sb) ) return zyzvel;
  w = ( ca*angvel->c.x + sa*angvel->c.y ) / sb;
  _zVec3DCreate( zyzvel, angvel->c.z - w*cb,
                        -sa*angvel->c.x + ca*angvel->c.y,
                         w );
  return zyzvel;
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 3D vector from a ZTK format processor. */
zVec3D *zVec3DFromZTK(zVec3D *v, ZTK *ztk)
{
  v->e[0] = ZTKDouble(ztk);
  v->e[1] = ZTKDouble(ztk);
  v->e[2] = ZTKDouble(ztk);
  return v;
}

/* add a 3D vector to a ZTK format processor. */
ZTK *zVec3DToZTK(zVec3D *v, ZTK *ztk)
{
  if( !ZTKAddDouble( ztk, v->e[0] ) ) return NULL;
  if( !ZTKAddDouble( ztk, v->e[1] ) ) return NULL;
  if( !ZTKAddDouble( ztk, v->e[2] ) ) return NULL;
  return ztk;
}

/* scan a 3D vector from a file. */
zVec3D *zVec3DFScan(FILE *fp, zVec3D *v)
{
  zFDouble( fp, &v->c.x );
  zFDouble( fp, &v->c.y );
  zFDouble( fp, &v->c.z );
  return v;
}

/* print a 3D vector out to a file. */
zVec3D *zVec3DDataFPrint(FILE *fp, zVec3D *v)
{
  if( !v ) return NULL;
  fprintf( fp, " %.10g, %.10g, %.10g", v->c.x, v->c.y, v->c.z );
  return v;
}

/* print a 3D vector out to a file with the new line. */
zVec3D *zVec3DDataNLFPrint(FILE *fp, zVec3D *v)
{
  if( !zVec3DDataFPrint( fp, v ) ) return NULL;
  fprintf( fp, "\n" );
  return v;
}

/* print a 3D vector out to a file. */
zVec3D *zVec3DFPrint(FILE *fp, zVec3D *v)
{
  fprintf( fp, "(" );
  if( !v )
    fprintf( fp, "null 3D vector" );
  else
    zVec3DDataFPrint( fp, v );
  fprintf( fp, ")\n" );
  return v;
}
