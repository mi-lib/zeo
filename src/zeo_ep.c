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

/* create Euler parameter. */
zEP *zEPCreate(zEP *ep, double w, double x, double y, double z)
{
  ep->e[0] = w;
  ep->e[1] = x;
  ep->e[2] = y;
  ep->e[3] = z;
  return zEPNormalize( ep );
}

/* create Euler parameter from rotation angle and axis vector. */
zEP *zEPCreateAA(zEP *ep, double theta, zVec3D *axis)
{
  double sh, ch;

  if( zVec3DIsTiny( axis ) ){
    ep->ex.w = 1.0;
    zVec3DZero( &ep->ex.v );
  } else{
    zSinCos( 0.5*theta, &sh, &ch );
    ep->ex.w = ch;
    zVec3DMul( axis, sh/zVec3DNorm(axis), &ep->ex.v );
  }
  return ep;
}

/* check if Euler parameter is for identity conversion. */
bool zEPIsIdent(zEP *ep)
{
  return zIsTiny( ep->ex.w - 1.0 ) && zVec3DIsTiny( &ep->ex.v );
}

/* convert Euler parameter to angle-axis vector. */
zVec3D *zEP2AA(zEP *ep, zVec3D *aa)
{
  double theta, sh;

  sh = zVec3DNorm( &ep->ex.v );
  theta = 2 * atan2( sh, ep->ex.w );
  return zIsTiny(sh) ? zVec3DZero(aa) : zVec3DMul( &ep->ex.v, theta/sh, aa );
}

/* convert angle-axis vector to Euler parameter. */
zEP *zAA2EP(zVec3D *aa, zEP *ep)
{
  return zEPCreateAA( ep, zVec3DNorm(aa), aa );
}

/* convert Euler parameter to attitude matrix. */
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

/* convert attitude matrix to Euler parameter. */
zEP *zMat3DToEP(zMat3D *m, zEP *ep)
{
  double k, s[4];
  int i, imax;

  s[0] = m->c.xx + m->c.yy + m->c.zz + 1.0;
  s[1] = m->c.xx - m->c.yy - m->c.zz + 1.0;
  s[2] =-m->c.xx + m->c.yy - m->c.zz + 1.0;
  s[3] =-m->c.xx - m->c.yy + m->c.zz + 1.0;
  for( imax=0, i=1; i<4; i++ )
    if( s[i] > s[imax] ) imax = i;
  if( s[imax] <= 0 ){
    ZRUNERROR( ZEO_ERR_MAT_NOTSO3 );
    return NULL;
  }
  k = 0.25 / ( ep->e[imax] = 0.5 * sqrt( s[imax] ) );
  switch( imax ){
  case 0:
    ep->e[1] = k * ( m->e[1][2] - m->e[2][1] );
    ep->e[2] = k * ( m->e[2][0] - m->e[0][2] );
    ep->e[3] = k * ( m->e[0][1] - m->e[1][0] );
    break;
  case 1:
    ep->e[2] = k * ( m->e[1][0] + m->e[0][1] );
    ep->e[3] = k * ( m->e[0][2] + m->e[2][0] );
    ep->e[0] = k * ( m->e[1][2] - m->e[2][1] );
    break;
  case 2:
    ep->e[3] = k * ( m->e[2][1] + m->e[1][2] );
    ep->e[0] = k * ( m->e[2][0] - m->e[0][2] );
    ep->e[1] = k * ( m->e[1][0] + m->e[0][1] );
    break;
  case 3:
    ep->e[0] = k * ( m->e[0][1] - m->e[1][0] );
    ep->e[1] = k * ( m->e[0][2] + m->e[2][0] );
    ep->e[2] = k * ( m->e[2][1] + m->e[1][2] );
    break;
  }
  return ep;
}

/* rotate vector by Euler parameter. */
zVec3D *zEPRotVec3D(zEP *ep, zVec3D *v, zVec3D *rv)
{
  _zVec3DOuterProd( &ep->ex.v, v, rv );
  zVec3DMulDRC( rv, 2*ep->ex.w );
  zVec3DCatDRC( rv, zSqr(ep->ex.w)-_zVec3DSqrNorm(&ep->ex.v), v );
  zVec3DCatDRC( rv, 2*_zVec3DInnerProd(&ep->ex.v,v), &ep->ex.v );
  return rv;
}

/* convert from Euler parameter derivative to angular velocity. */
zVec3D *zEPVel2AngVel(zEP *epvel, zEP *ep, zVec3D *angvel)
{
  _zVec3DOuterProd( &ep->ex.v, &epvel->ex.v, angvel );
  _zVec3DCatDRC( angvel, ep->ex.w, &epvel->ex.v );
  _zVec3DCatDRC( angvel,-epvel->ex.w, &ep->ex.v );
  return zVec3DMulDRC( angvel, 2 );
}

/* convert from angular velocity to Euler parameter derivative. */
zEP *zAngVel2EPVel(zVec3D *angvel, zEP *ep, zEP *epvel)
{
  epvel->ex.w = -0.5*_zVec3DInnerProd(&ep->ex.v,angvel);
  _zVec3DOuterProd( angvel, &ep->ex.v, &epvel->ex.v );
  _zVec3DCatDRC( &epvel->ex.v, ep->ex.w, angvel );
  _zVec3DMulDRC( &epvel->ex.v, 0.5 );
  return epvel;
}

/* add Euler parameters. */
zEP *zEPAdd(zEP *ep1, zEP *ep2, zEP *ep)
{
  ep->ex.w = ep1->ex.w + ep2->ex.w;
  _zVec3DAdd( &ep1->ex.v, &ep2->ex.v, &ep->ex.v );
  return ep;
}

/* subtract an Euler parameter from another. */
zEP *zEPSub(zEP *ep1, zEP *ep2, zEP *ep)
{
  ep->ex.w = ep1->ex.w - ep2->ex.w;
  _zVec3DSub( &ep1->ex.v, &ep2->ex.v, &ep->ex.v );
  return ep;
}

/* reverse an Euler parameter. */
zEP *zEPRev(zEP *ep1, zEP *ep)
{
  ep->ex.w = -ep1->ex.w;
  _zVec3DRev( &ep1->ex.v, &ep->ex.v );
  return ep;
}

/* multiply an Euler parameter by a scalar value. */
zEP *zEPMul(zEP *ep1, double k, zEP *ep)
{
  ep->ex.w = k * ep1->ex.w;
  _zVec3DMul( &ep1->ex.v, k, &ep->ex.v );
  return ep;
}

/* concatenate an Euler parameter multiplied by a scalar value. */
zEP *zEPCat(zEP *ep1, double k, zEP *ep2, zEP *ep)
{
  ep->ex.w = ep1->ex.w + k * ep2->ex.w;
  _zVec3DCat( &ep1->ex.v, k, &ep2->ex.v, &ep->ex.v );
  return zEPNormalize( ep );
}

/* numerically differentiate Euler parameters. */
zEP *zEPDif(zEP *ep1, zEP *ep2, double dt, zEP *ep_vel)
{
  if( zIsTiny(dt) ){
    ZRUNERROR( ZEO_ERR_ZERODIV );
    return NULL;
  }
  zEPSub( ep2, ep1, ep_vel );
  return zEPMulDRC( ep_vel, 1.0/dt );
}

/* inner product of Euler parameters. */
double zEPInnerProd(zEP *ep1, zEP *ep2)
{
  return ep1->ex.w*ep2->ex.w + _zVec3DInnerProd( &ep1->ex.v, &ep2->ex.v );
}

/* norm of Euler parameter. */
double zEPNorm(zEP *ep)
{
  return sqrt( zEPInnerProd( ep, ep ) );
}

/* normalize Euler parameter. */
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

/* cascade a Euler parameter to another. */
zEP *zEPCascade(zEP *ep1, zEP *ep2, zEP *ep)
{
  zEP tmp;

  tmp.ex.w = ep1->ex.w * ep2->ex.w - _zVec3DInnerProd(&ep1->ex.v,&ep2->ex.v);
  _zVec3DOuterProd( &ep2->ex.v, &ep1->ex.v, &tmp.ex.v );
  _zVec3DCatDRC( &tmp.ex.v, ep1->ex.w, &ep2->ex.v );
  _zVec3DCatDRC( &tmp.ex.v, ep2->ex.w, &ep1->ex.v );
  zEPCopy( &tmp, ep );
  return ep;
}

/* interior division of Euler parameter for SLERP. */
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

/* interior division of two attitude matrices for SLERP. */
zMat3D *zMat3DInterDiv(zMat3D *m1, zMat3D *m2, double t, zMat3D *m)
{
  zEP ep1, ep2, ep;

  zMat3DToEP( m1, &ep1 );
  zMat3DToEP( m2, &ep2 );
  zEPInterDiv( &ep1, &ep2, t, &ep );
  return zMat3DFromEP( m, &ep );
}

/* print Euler parameter out to a file. */
void zEPFPrint(FILE *fp, zEP *ep)
{
  fprintf( fp, "%f ", ep->ex.w );
  zVec3DFPrint( fp, &ep->ex.v );
}
