/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ep - Euler parameter (unit quaternion) class.
 */

#include <zeo/zeo_ep.h>

/* ********************************************************** */
/* CLASS: zEP
 * Euler parameter class
 * ********************************************************** */

/* zEPCreate
 * - create Euler parameter.
 */
zEP *zEPCreate(zEP *ep, double w, double x, double y, double z)
{
  ep->e[0] = w;
  ep->e[1] = x;
  ep->e[2] = y;
  ep->e[3] = z;
  return zEPNormalize( ep );
}

/* zEPCreateAA
 * - create Euler parameter from rotation angle and axis vector.
 */
zEP *zEPCreateAA(zEP *ep, double theta, zVec3D *axis)
{
  double sh, ch;

  if( zVec3DIsTiny( axis ) ){
    ep->ex.w = 1.0;
    zVec3DClear( &ep->ex.v );
  } else{
    zSinCos( 0.5*theta, &sh, &ch );
    ep->ex.w = ch;
    zVec3DMul( axis, sh/zVec3DNorm(axis), &ep->ex.v );
  }
  return ep;
}

/* zEPIsIdent
 * - check if Euler parameter is identity.
 */
bool zEPIsIdent(zEP *ep)
{
  return zIsTiny( ep->ex.w - 1.0 ) && zVec3DIsTiny( &ep->ex.v );
}

/* zEP2AA
 * - convert Euler parameter to angle-axis vector.
 */
zVec3D *zEP2AA(zEP *ep, zVec3D *aa)
{
  double theta, sh;

  sh = zVec3DNorm( &ep->ex.v );
  theta = 2 * atan2( sh, ep->ex.w );
  return zIsTiny(sh) ? zVec3DClear(aa) : zVec3DMul( &ep->ex.v, theta/sh, aa );
}

/* zAA2EP
 * - convert angle-axis vector to Euler parameter.
 */
zEP *zAA2EP(zVec3D *aa, zEP *ep)
{
  return zEPCreateAA( ep, zVec3DNorm(aa), aa );
}

/* zMat3DFromEP
 * - convert Euler parameter to attitude matrix.
 */
zMat3D *zMat3DFromEP(zMat3D *m, zEP* ep)
{
  double e00, e11, e22, e33, e12, e23, e31, e01, e02, e03;

  e00 = zSqr(ep->e[0]);
  e11 = zSqr(ep->e[1]);
  e22 = zSqr(ep->e[2]);
  e33 = zSqr(ep->e[3]);
  e12 = ep->e[1] * ep->e[2];
  e23 = ep->e[2] * ep->e[3];
  e31 = ep->e[3] * ep->e[1];
  e01 = ep->e[0] * ep->e[1];
  e02 = ep->e[0] * ep->e[2];
  e03 = ep->e[0] * ep->e[3];
  return zMat3DCreate( m,
    e00+e11-e22-e33,     2*(e12-e03),     2*(e31+e02),
        2*(e12+e03), e00-e11+e22-e33,     2*(e23-e01),
        2*(e31-e02),     2*(e23+e01), e00-e11-e22+e33 );
}

/* zMat3DToEP
 * - convert attitude matrix to Euler parameter.
 */
zEP *zMat3DToEP(zMat3D *m, zEP *ep)
{
  double s;
  register int i, j, k, l;

  if( !zIsTiny( ( s = m->c.xx+m->c.yy+m->c.zz+1 ) ) ){
    ep->ex.w = 0.5 * sqrt( s );
    s = 0.25 / ep->ex.w;
    zVec3DCreate( &ep->ex.v, ( m->c.yz - m->c.zy ) * s, ( m->c.zx - m->c.xz ) * s, ( m->c.xy - m->c.yz ) * s );
  } else{
    ep->ex.w = 0;
    for( i=0; i<3; i++ ){
      j = i == 2 ? 0 : i+1;
      k = i == 0 ? 3 : i;
      l = k - 1;
      if( ( s = m->e[i][i] + m->e[j][j] ) < 0 ){
        s = 0.25 / ( ep->e[k] = sqrt( -0.5*s ) );
        ep->e[i+1] = ( m->e[i][l] + m->e[l][i] ) * s;
        ep->e[j+1] = ( m->e[l][j] + m->e[j][l] ) * s;
        goto RET;
      }
    }
  }
 RET:
  return ep;
}

/* zEPRotVec
 * - rotate vector by Euler parameter.
 */
zVec3D *zEPRotVec(zEP *ep, zVec3D *v, zVec3D *rv)
{
  _zVec3DOuterProd( &ep->ex.v, v, rv );
  zVec3DMulDRC( rv, 2*ep->ex.w );
  zVec3DCatDRC( rv, zSqr(ep->ex.w)-_zVec3DSqrNorm(&ep->ex.v), v );
  zVec3DCatDRC( rv, 2*_zVec3DInnerProd(&ep->ex.v,v), &ep->ex.v );
  return rv;
}

/* zEPVel2AngVel
 * - convert from Euler parameter derivative to angular velocity.
 */
zVec3D *zEPVel2AngVel(zEP *epvel, zEP *ep, zVec3D *angvel)
{
  _zVec3DOuterProd( &ep->ex.v, &epvel->ex.v, angvel );
  _zVec3DCatDRC( angvel, ep->ex.w, &epvel->ex.v );
  _zVec3DCatDRC( angvel,-epvel->ex.w, &ep->ex.v );
  return zVec3DMulDRC( angvel, 2 );
}

/* zAngVel2EPVel
 * - convert from angular velocity to Euler parameter derivative.
 */
zEP *zAngVel2EPVel(zVec3D *angvel, zEP *ep, zEP *epvel)
{
  epvel->ex.w = -0.5*_zVec3DInnerProd(&ep->ex.v,angvel);
  _zVec3DOuterProd( angvel, &ep->ex.v, &epvel->ex.v );
  _zVec3DCatDRC( &epvel->ex.v, ep->ex.w, angvel );
  _zVec3DMulDRC( &epvel->ex.v, 0.5 );
  return epvel;
}

/* zEPSub
 * - subtract Euler parameter.
 */
zEP *zEPSub(zEP *ep1, zEP *ep2, zEP *ep)
{
  ep->ex.w = ep1->ex.w - ep2->ex.w;
  _zVec3DSub( &ep1->ex.v, &ep2->ex.v, &ep->ex.v );
  return ep;
}

/* zEPRev
 * - reverse Euler parameter.
 */
zEP *zEPRev(zEP *ep1, zEP *ep)
{
  ep->ex.w = -ep1->ex.w;
  _zVec3DRev( &ep1->ex.v, &ep->ex.v );
  return ep;
}

/* zEPMul
 * - multiply Euler parameter by a scalar value.
 */
zEP *zEPMul(zEP *ep1, double k, zEP *ep)
{
  ep->ex.w = k * ep1->ex.w;
  _zVec3DMul( &ep1->ex.v, k, &ep->ex.v );
  return ep;
}

/* zEPCat
 * - concatenate Euler parameter multiplied by a scalar value.
 */
zEP *zEPCat(zEP *ep1, double k, zEP *ep2, zEP *ep)
{
  ep->ex.w = ep1->ex.w + k * ep2->ex.w;
  _zVec3DCat( &ep1->ex.v, k, &ep2->ex.v, &ep->ex.v );
  return zEPNormalize( ep );
}

/* zEPDif
 * - numerically differentiate Euler parameters.
 */
zEP *zEPDif(zEP *ep1, zEP *ep2, double dt, zEP *ep_vel)
{
  if( zIsTiny(dt) ){
    ZRUNERROR( ZEO_ERR_ZERODIV );
    return NULL;
  }
  zEPSub( ep2, ep1, ep_vel );
  return zEPMulDRC( ep_vel, 1.0/dt );
}

/* zEPInnerProd
 * - inner product of Euler parameters.
 */
double zEPInnerProd(zEP *ep1, zEP *ep2)
{
  return ep1->ex.w*ep2->ex.w + _zVec3DInnerProd( &ep1->ex.v, &ep2->ex.v );
}

/* zEPNorm
 * - norm of Euler parameter.
 */
double zEPNorm(zEP *ep)
{
  return sqrt( zEPInnerProd( ep, ep ) );
}

/* zEPNormalize
 * - normalize Euler parameter.
 */
zEP *zEPNormalize(zEP *ep)
{
  double norm;

  if( zIsTiny( ( norm = zEPNorm( ep ) ) ) ){
    ZRUNERROR( ZEO_ERR_ZERODIV );
    return NULL;
  }
  ep->ex.w /= norm;
  zVec3DDivDRC( &ep->ex.v, norm );
  return ep;
}

/* zEPCascade
 * - cascade a Euler parameter to another.
 */
zEP *zEPCascade(zEP *e1, zEP *e2, zEP *e)
{
  zEP tmp;

  tmp.ex.w = e1->ex.w * e2->ex.w - _zVec3DInnerProd(&e1->ex.v,&e2->ex.v);
  _zVec3DOuterProd( &e2->ex.v, &e1->ex.v, &tmp.ex.v );
  _zVec3DCatDRC( &tmp.ex.v, e1->ex.w, &e2->ex.v );
  _zVec3DCatDRC( &tmp.ex.v, e2->ex.w, &e1->ex.v );
  zEPCopy( &tmp, e );
  return e;
}

/* zEPInterDiv
 * - interior division of Euler parameter for SLERP.
 */
zEP *zEPInterDiv(zEP *ep1, zEP *ep2, double t, zEP *ep)
{
  double th, cth, sth, th2;

  if( ( cth = zEPInnerProd( ep1, ep2 ) ) < 0 ){
    zEPRevDRC( ep2 );
    cth = -cth;
  }
  if( cth > 1.0 ) cth = 1.0;
  th = acos( cth );
  sth = sin( th );
  if( zIsTiny( sth ) ){
    zEPMul( ep1, 1 - t, ep );
    zEPCatDRC( ep, t, ep2 );
  } else{
    th2 = th * t;
    zEPMul( ep1, sin( th - th2 ), ep );
    zEPCatDRC( ep, sin( th2 ), ep2 );
  }
  return zEPNormalize( ep );
}

/* zMat3DInterDiv
 * - interior division of two attitude matrices for SLERP.
 */
zMat3D *zMat3DInterDiv(zMat3D *m1, zMat3D *m2, double t, zMat3D *m)
{
  zEP ep1, ep2, ep;

  zMat3DToEP( m1, &ep1 );
  zMat3DToEP( m2, &ep2 );
  zEPInterDiv( &ep1, &ep2, t, &ep );
  return zMat3DFromEP( m, &ep );
}

/* zEPFWrite
 * - output of Euler parameter to file.
 */
void zEPFWrite(FILE *fp, zEP *ep)
{
  fprintf( fp, "%f ", ep->ex.w );
  zVec3DFWrite( fp, &ep->ex.v );
}
