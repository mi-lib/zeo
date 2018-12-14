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

/* OBJECT:
 * zvec3Dzero, zvec3Dx, zvec3Dy, zvec3Dz
 * - 3D zero vector and unit vectors along (x,y,z) axis.
 */
const zVec3D zvec3Dzero = { { 0, 0, 0 } };
const zVec3D zvec3Dx    = { { 1, 0, 0 } };
const zVec3D zvec3Dy    = { { 0, 1, 0 } };
const zVec3D zvec3Dz    = { { 0, 0, 1 } };

/* zVec3DCreate
 * - create a 3D vector.
 */
zVec3D *zVec3DCreate(zVec3D *v, double x, double y, double z)
{
  _zVec3DCreate( v, x, y, z );
  return v;
}

/* zVec3DCreatePolar
 * - create a 3D vector by the set of value for a polar
 *   expression.
 */
zVec3D *zVec3DCreatePolar(zVec3D *v, double r, double theta, double phi)
{
  double rs;

  rs = r * sin( theta );
  _zVec3DCreate( v, rs*cos(phi), rs*sin(phi), r*cos(theta) );
  return v;
}

/* zVec3DMatch
 * - check if two 3D vectors match each other.
 */
bool zVec3DMatch(zVec3D *v1, zVec3D *v2)
{
  return _zVec3DMatch( v1, v2 );
}

/* zVec3DEqual
 * - check if two 3D vectors are equal.
 */
bool zVec3DEqual(zVec3D *v1, zVec3D *v2)
{
  return _zVec3DEqual( v1, v2 );
}

/* zVec3DIsTol
 * - check if a 3D vector is tiny enough.
 */
bool zVec3DIsTol(zVec3D *v, double tol)
{
  return _zVec3DIsTol( v, tol );
}

/* zVec3DIsNan
 * - check if a 3D vector includes NaN or Inf components.
 */
bool zVec3DIsNan(zVec3D *v)
{
  return zIsNan( v->c.x ) || zIsInf( v->c.x ) ||
         zIsNan( v->c.y ) || zIsInf( v->c.y ) ||
         zIsNan( v->c.z ) || zIsInf( v->c.z );
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* zVec3DAdd
 * - add two 3D vectors.
 */
zVec3D *zVec3DAdd(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  _zVec3DAdd( v1, v2, v );
  return v;
}

/* zVec3DSub
 * - subtract a 3D vector from another.
 */
zVec3D *zVec3DSub(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  _zVec3DSub( v1, v2, v );
  return v;
}

/* zVec3DRev
 * - reverse a 3D vector.
 */
zVec3D *zVec3DRev(zVec3D *v, zVec3D *rv)
{
  _zVec3DRev( v, rv );
  return rv;
}

/* zVec3DMul
 * - multiply a 3D vector by a scalar.
 */
zVec3D *zVec3DMul(zVec3D *v, double k, zVec3D *mv)
{
  _zVec3DMul( v, k, mv );
  return mv;
}

/* zVec3DDiv
 * - divide a 3D vector by a scalar.
 */
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

/* zVec3DAmp
 * - amplify a 3D vector by another.
 */
zVec3D *zVec3DAmp(zVec3D *v, zVec3D *a, zVec3D *av)
{
  _zVec3DAmp( v, a, av );
  return av;
}

/* zVec3DCat
 * - concatenate 3D vectors.
 */
zVec3D *zVec3DCat(zVec3D *v1, double k, zVec3D *v2, zVec3D *v)
{
  _zVec3DCat( v1, k, v2, v );
  return v;
}

/* zVec3DAddDRC
 * - directly add a 3D vector to another.
 */
zVec3D *zVec3DAddDRC(zVec3D *v1, zVec3D *v2)
{
  _zVec3DAddDRC( v1, v2 );
  return v1;
}

/* zVec3DSubDRC
 * - directly subtract a 3D vector from another.
 */
zVec3D *zVec3DSubDRC(zVec3D *v1, zVec3D *v2)
{
  _zVec3DSubDRC( v1, v2 );
  return v1;
}

/* zVec3DRevDRC
 * - directly reverse a 3D vector.
 */
zVec3D *zVec3DRevDRC(zVec3D *v)
{
  _zVec3DRevDRC( v );
  return v;
}

/* zVec3DMulDRC
 * - directly multiply a 3D vector by a scalar value.
 */
zVec3D *zVec3DMulDRC(zVec3D *v, double k)
{
  _zVec3DMulDRC( v, k );
  return v;
}

/* zVec3DDivDRC
 * - directly divide a 3D vector by a scalar value.
 */
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

/* zVec3DAmpDRC
 * - directly amplify a 3D vector by another.
 */
zVec3D *zVec3DAmpDRC(zVec3D *v1, zVec3D *v2)
{
  _zVec3DAmpDRC( v1, v2 );
  return v1;
}

/* zVec3DCatDRC
 * -directly concatenate a 3D vector multiplied by a scalar to another.
 */
zVec3D *zVec3DCatDRC(zVec3D *v1, double k, zVec3D *v2)
{
  _zVec3DCatDRC( v1, k, v2 );
  return v1;
}

/* zVec3DInnerProd
 * - inner product of two 3D vectors.
 */
double zVec3DInnerProd(zVec3D *v1, zVec3D *v2)
{
  return _zVec3DInnerProd( v1, v2 );
}

/* zVec3DOuterProd
 * - outer product of two 3D vectors.
 */
zVec3D *zVec3DOuterProd(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  zVec3D v3; /* a temporary vector is necessary because v1 or v2 can be the same with v */

  _zVec3DOuterProd( v1, v2, &v3 );
  return zVec3DCopy( &v3, v );
}

/* zVec3DOuterProdNorm
 * - norm of outer product of two 3D vectors.
 */
double zVec3DOuterProdNorm(zVec3D *v1, zVec3D *v2)
{
  zVec3D v;

  _zVec3DOuterProd( v1, v2, &v );
  return _zVec3DNorm( &v );
}

/* zVec3DGrassmannProd
 * - scalar triple product of three 3D vectors.
 */
double zVec3DGrassmannProd(zVec3D *v1, zVec3D *v2, zVec3D *v3)
{
  zVec3D v;

  _zVec3DOuterProd( v2, v3, &v );
  return _zVec3DInnerProd( v1, &v );
}

/* zVec3DTripleProd
 * - vector triple product of three 3D vectors.
 */
zVec3D *zVec3DTripleProd(zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v)
{
  _zVec3DTripleProd( v1, v2, v3, v );
  return v;
}

/* zVec3DSqrNorm
 * - squared norm of a 3D vector.
 */
double zVec3DSqrNorm(zVec3D *v)
{
  return _zVec3DSqrNorm( v );
}

/* zVec3DWSqrNorm
 * - squared weighted norm of a 3D vector.
 */
double zVec3DWSqrNorm(zVec3D *v, zVec3D *w)
{
  return _zVec3DWSqrNorm( v, w );
}

/* zVec3DSqrDist
 * - distance between two positions.
 */
double zVec3DSqrDist(zVec3D *v1, zVec3D *v2)
{
  zVec3D dv;

  _zVec3DSub( v1, v2, &dv );
  return _zVec3DSqrNorm( &dv );
}

/* zVec3DNormalizeNC
 * - normalize a 3D vector without checking vector size.
 */
double zVec3DNormalizeNC(zVec3D *v, zVec3D *nv)
{
  double l, k;

  k = 1.0 / ( l = zVec3DNorm(v) );
  _zVec3DMul( v, k, nv );
  return l;
}

/* zVec3DNormalize
 * - normalize a 3D vector.
 */
double zVec3DNormalize(zVec3D *v, zVec3D *nv)
{
  double l, k;

  if( zVec3DIsTiny( v ) ){
    ZRUNWARN( ZEO_ERR_ZERONORM );
    return -1;
  }
  k = 1.0 / ( l = zVec3DNorm(v) );
  _zVec3DMul( v, k, nv );
  return l;
}

/* ********************************************************** */
/* geometry
 * ********************************************************** */

/* zVec3DInterDiv
 * - interior division of two the positions.
 */
zVec3D *zVec3DInterDiv(zVec3D *v1, zVec3D *v2, double ratio, zVec3D *v)
{
  _zVec3DInterDiv( v1, v2, ratio, v );
  return v;
}

/* zVec3DMid
 * - middle point of the two positions.
 */
zVec3D *zVec3DMid(zVec3D *v1, zVec3D *v2, zVec3D *v)
{
  _zVec3DMid( v1, v2, v );
  return v;
}

/* zVec3DAngle
 * - angle between the two vectors.
 */
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

/* zVec3DProj
 * - projection of a vector.
 */
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

/* zVec3DOrthogonalize
 * - orthogonalize a vector.
 */
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

/* zVec3DOrthoSpace
 * - create the orthogonal space of a vector.
 */
bool zVec3DOrthoSpace(zVec3D *v, zVec3D *sv1, zVec3D *sv2)
{
  double l;

  zVec3DCopy( v, sv1 );
  if( v->c.y != 0 || v->c.z != 0 )
    sv1->c.x += 1.0;
  else
    sv1->c.y = 1.0;
  if( _zVec3DIsTiny( v ) ){
    ZRUNWARN( ZEO_ERR_ZERONORM );
    return false;
  }
  l = -_zVec3DInnerProd( v, sv1 ) / _zVec3DSqrNorm( v );
  _zVec3DCatDRC( sv1, l, v );
  _zVec3DOuterProd( v, sv1, sv2 );
  return true;
}

/* zVec3DRot
 * - rotate a 3D vector along an arbitrary axis.
 */
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

/* zVec3DDif
 * - numerical differentiation of 3D vector.
 */
zVec3D *zVec3DDif(zVec3D *v, zVec3D *vnew, double dt, zVec3D *vel)
{
  _zVec3DSub( vnew, v, vel );
  return zVec3DDivDRC( vel, dt );
}

/* zVec3DZYXVel2AngVel
 * - convert z-y-x Eulerian angle differential to angular velocity.
 */
zVec3D *zVec3DZYXVel2AngVel(zVec3D *zyxvel, zVec3D *zyx, zVec3D *angvel)
{
  double sa, ca, sb, cb;

  _zSinCos( zyx->c.x, &sa, &ca );
  _zSinCos( zyx->c.y, &sb, &cb );
  return zVec3DZYXVel2AngVelSC( zyxvel, sa, ca, sb, cb, angvel );
}

/* zVec3DZYXVel2AngVelSC
 * - convert z-y-x Eulerian angle differential to angular velocity,
 *   directly accepting sine/cosine sets.
 */
zVec3D *zVec3DZYXVel2AngVelSC(zVec3D *zyxvel, double sa, double ca, double sb, double cb, zVec3D *angvel)
{
  _zVec3DCreate( angvel,-sa*zyxvel->c.y + ca*cb*zyxvel->c.z,
                         ca*zyxvel->c.y + sa*cb*zyxvel->c.z,
                            zyxvel->c.x -    sb*zyxvel->c.z );
  return angvel;
}

/* zVec3DAngVel2ZYXVel
 * - convert angular velocity to z-y-x Eulerian angle differential.
 */
zVec3D *zVec3DAngVel2ZYXVel(zVec3D *angvel, zVec3D *zyx, zVec3D *zyxvel)
{
  double sa, ca, sb, cb;

  _zSinCos( zyx->c.x, &sa, &ca );
  _zSinCos( zyx->c.y, &sb, &cb );
  return zVec3DAngVel2ZYXVelSC( angvel, sa, ca, sb, cb, zyxvel );
}

/* zVec3DAngVel2ZYXVelSC
 * - convert angular velocity to z-y-x Eulerian angle differential.
 *   directly accepting sine/cosine sets.
 */
zVec3D *zVec3DAngVel2ZYXVelSC(zVec3D *angvel, double sa, double ca, double sb, double cb, zVec3D *zyxvel)
{
  /* at the singular point, 'zyxvel' remains the same value. */
  if( zIsTiny(cb) ) return zyxvel;
  _zVec3DCreate( zyxvel, ca/cb*angvel->c.x+sa/cb*angvel->c.y,
                         sb*zyxvel->c.z+angvel->c.z,
                        -sa*angvel->c.x+ca*angvel->c.y );
  return zyxvel;
}

/* zVec3DZYZVel2AngVel
 * - convert z-y-z Eulerian angle differential to angular velocity.
 */
zVec3D *zVec3DZYZVel2AngVel(zVec3D *zyzvel, zVec3D *zyz, zVec3D *angvel)
{
  double sa, ca, sb, cb;

  _zSinCos( zyz->c.x, &sa, &ca );
  _zSinCos( zyz->c.y, &sb, &cb );
  return zVec3DZYZVel2AngVelSC( zyzvel, sa, ca, sb, cb, angvel );
}

/* zVec3DZYZVel2AngVelSC
 * - convert z-y-z Eulerian angle differential to angular velocity.
 *   directly accepting sine/cosine sets.
 */
zVec3D *zVec3DZYZVel2AngVelSC(zVec3D *zyzvel, double sa, double ca, double sb, double cb, zVec3D *angvel)
{
  _zVec3DCreate( angvel,-sa*zyzvel->c.y + ca*sb*zyzvel->c.z,
                         ca*zyzvel->c.y + sa*sb*zyzvel->c.z,
                            zyzvel->c.x +    cb*zyzvel->c.z );
  return angvel;
}

/* zVec3DAngVel2ZYZVel
 * - convert angular velocity to z-y-z Eulerian angle differential.
 */
zVec3D *zVec3DAngVel2ZYZVel(zVec3D *angvel, zVec3D *zyz, zVec3D *zyzvel)
{
  double sa, ca, sb, cb;

  _zSinCos( zyz->c.x, &sa, &ca );
  _zSinCos( zyz->c.y, &sb, &cb );
  return zVec3DAngVel2ZYZVelSC( angvel, sa, ca, sb, cb, zyzvel );
}

/* zVec3DAngVel2ZYZVelSC
 * - convert angular velocity to z-y-z Eulerian angle differential.
 *   directly accepting sine/cosine sets.
 */
zVec3D *zVec3DAngVel2ZYZVelSC(zVec3D *angvel, double sa, double ca, double sb, double cb, zVec3D *zyzvel)
{
  /* at the singular point, 'zyzvel' remains the same value. */
  if( zIsTiny(sb) ) return zyzvel;
  _zVec3DCreate( zyzvel, ca/sb*angvel->c.x+sa/sb*angvel->c.y,
                        -cb*zyzvel->c.z+angvel->c.z,
                        -sa*angvel->c.x+ca*angvel->c.y );
  return zyzvel;
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* zVec3DFRead
 * - input a 3D vector from file.
 */
zVec3D *zVec3DFRead(FILE *fp, zVec3D *v)
{
  v->c.x = zFDouble( fp );
  v->c.y = zFDouble( fp );
  v->c.z = zFDouble( fp );
  return v;
}

/* zVec3DDataFWrite
 * - output a 3D vector to file.
 */
zVec3D *zVec3DDataFWrite(FILE *fp, zVec3D *v)
{
  if( !v ) return NULL;
  fprintf( fp, " %.10g %.10g %.10g", v->c.x, v->c.y, v->c.z );
  return v;
}

/* zVec3DDataNLFWrite
 * - output a 3D vector to file with the new line.
 */
zVec3D *zVec3DDataNLFWrite(FILE *fp, zVec3D *v)
{
  if( !zVec3DDataFWrite( fp, v ) ) return NULL;
  fprintf( fp, "\n" );
  return v;
}

/* zVec3DFWrite
 * - output of 3D vector to file.
 */
zVec3D *zVec3DFWrite(FILE *fp, zVec3D *v)
{
  fprintf( fp, "(" );
  if( !v )
    fprintf( fp, "null 3D vector" );
  else
    zVec3DDataFWrite( fp, v );
  fprintf( fp, ")\n" );
  return v;
}

/* METHOD:
 * zVec3DFWriteXML - xml output.
 * ... yet testing.
 */
void zVec3DFWriteXML(FILE *fp, zVec3D *v)
{
  fprintf( fp, "\"" );
  zVec3DDataFWrite( fp, v );
  fprintf( fp, "\"" );
}
