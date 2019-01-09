/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat3d - 3x3 matrix.
 */

#include <zeo/zeo_mat3d.h>

/* ********************************************************** */
/* CLASS: zMat3D
 * 3x3 matrix class
 * ********************************************************** */

/* 3x3 zero matrix and identity matrix. */
const zMat3D zmat3Dzero  = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
const zMat3D zmat3Dident = { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } } };

/* create a 3x3 matrix. */
zMat3D *zMat3DCreate(zMat3D *m,
  double a11, double a12, double a13,
  double a21, double a22, double a23,
  double a31, double a32, double a33)
{
  _zMat3DCreate( m, a11, a12, a13, a21, a22, a23, a31, a32, a33 );
  return m;
}

/* check if two 3x3 matrices are strictly equal. */
bool zMat3DMatch(zMat3D *m1, zMat3D *m2)
{
  return _zMat3DMatch( m1, m2 );
}

/* check if two 3x3 matrices are equal. */
bool zMat3DEqual(zMat3D *m1, zMat3D *m2)
{
  return _zMat3DEqual( m1, m2 );
}

/* abstract row vector from a 3x3 matrix. */
zVec3D *zMat3DRow(zMat3D *m, int i, zVec3D *v)
{
  _zMat3DRow( m, i, v );
  return v;
}

/* transpose a 3x3 matrix. */
zMat3D *zMat3DT(zMat3D *m, zMat3D *tm)
{
  if( tm == m ){
    _zMat3DTDRC( m );
  } else{
    _zMat3DT( m, tm );
  }
  return tm;
}

/* directly transpose a 3x3 matrix. */
zMat3D *zMat3DTDRC(zMat3D *m)
{
  _zMat3DTDRC( m );
  return m;
}

/* ********************************************************** */
/* arithmetics
 * ********************************************************** */

/* add two 3D matrices. */
zMat3D *zMat3DAdd(zMat3D *m1, zMat3D *m2, zMat3D *m)
{
  _zMat3DAdd( m1, m2, m );
  return m;
}

/* subtract a 3x3 matrix from another. */
zMat3D *zMat3DSub(zMat3D *m1, zMat3D *m2, zMat3D *m)
{
  _zMat3DSub( m1, m2, m );
  return m;
}

/* reverse a 3x3 matrix. */
zMat3D *zMat3DRev(zMat3D *m, zMat3D *rm)
{
  _zMat3DRev( m, rm );
  return rm;
}

/* multiply a 3x3 matrix by a scalar. */
zMat3D *zMat3DMul(zMat3D *m, double k, zMat3D *mm)
{
  _zMat3DMul( m, k, mm );
  return mm;
}

/* divide a 3x3 matrix by a scalar. */
zMat3D *zMat3DDiv(zMat3D *m, double k, zMat3D *dm)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zMat3DMul( m, k, dm );
  return dm;
}

/* concatenate a 3x3 matrix with another. */
zMat3D *zMat3DCat(zMat3D *m1, double k, zMat3D *m2, zMat3D *m)
{
  _zMat3DCat( m1, k, m2, m );
  return m;
}

/* dyadic product of two 3D vectors. */
zMat3D *zMat3DDyad(zMat3D *m, zVec3D *v1, zVec3D *v2)
{
  _zMat3DDyad( m, v1, v2 );
  return m;
}

/* add dyadic product of two 3D vectors to a 3x3 matrix. */
zMat3D *zMat3DAddDyad(zMat3D *m, zVec3D *v1, zVec3D *v2)
{
  _zMat3DAddDyad( m, v1, v2 );
  return m;
}

/* subtract dyadic product of two 3D vectors from a 3x3 matrix. */
zMat3D *zMat3DSubDyad(zMat3D *m, zVec3D *v1, zVec3D *v2)
{
  _zMat3DSubDyad( m, v1, v2 );
  return m;
}

/* create a skew-symmetric outer-product matrix. */
zMat3D *zVec3DOuterProd2Mat3D(zVec3D *v, zMat3D *m)
{
  _zVec3DOuterProd2Mat3D( v, m );
  return m;
}

/* create a twice-outer-product matrix. */
zMat3D *zVec3DTripleProd2Mat3D(zVec3D *v1, zVec3D *v2, zMat3D *m)
{
  _zVec3DTripleProd2Mat3D( v1, v2, m );
  return m;
}

/* multiply cross product of vector and matrix. */
zMat3D *zMulVec3DOPMat3D(zVec3D *ohm, zMat3D *m, zMat3D *mv)
{
  zVec3DOuterProd( ohm, &m->b.x, &mv->b.x );
  zVec3DOuterProd( ohm, &m->b.y, &mv->b.y );
  zVec3DOuterProd( ohm, &m->b.z, &mv->b.z );
  return mv;
}

/* ********************************************************** */
/* inverse of a 3x3 matrix
 * ********************************************************** */

/* determinant of 3x3 matrix. */
double zMat3DDet(zMat3D *m)
{
  return _zMat3DDet( m );
}

/* misc for real inverse of 3x3 matrix. */
#define _zMat3DInvRow(m,im,i,j,k,idet) do{\
  (im)->e[(i)][(i)] = (idet) * ( (m)->e[(j)][(j)]*(m)->e[(k)][(k)] - (m)->e[(k)][(j)]*(m)->e[(j)][(k)] );\
  (im)->e[(j)][(i)] = (idet) * ( (m)->e[(k)][(i)]*(m)->e[(j)][(k)] - (m)->e[(j)][(i)]*(m)->e[(k)][(k)] );\
  (im)->e[(k)][(i)] = (idet) * ( (m)->e[(j)][(i)]*(m)->e[(k)][(j)] - (m)->e[(k)][(i)]*(m)->e[(j)][(j)] );\
} while(0)

/* inverse matrix of 3x3 matrix. */
zMat3D *zMat3DInv(zMat3D *m, zMat3D *im)
{
  double det, idet;

  det = _zMat3DDet( m );
  if( zIsTiny( det ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
    return NULL;
  }
  idet = 1.0 / det;
  _zMat3DInvRow( m, im, 0, 1, 2, idet );
  _zMat3DInvRow( m, im, 1, 2, 0, idet );
  _zMat3DInvRow( m, im, 2, 0, 1, idet );
  return im;
}

/* ********************************************************** */
/* multiplication of a 3D vector by a 3x3 matrix
 * ********************************************************** */

static double _zMulInvMat3DVec3DRow(zMat3D *m, zVec3D *v, int i, int j, int k, double idet);
static zVec3D *_zMulInvMat3DVec3D(zMat3D *m, zVec3D *v, zVec3D *miv, double idet);

/* multiply a 3D vector by a 3x3 matrix. */
zVec3D *zMulMat3DVec3D(zMat3D *m, zVec3D *v, zVec3D *mv)
{
  _zMulMat3DVec3D( m, v, mv );
  return mv;
}

/* multiply a 3D vector by transpose of a 3x3 matrix. */
zVec3D *zMulMat3DTVec3D(zMat3D *m, zVec3D *v, zVec3D *mv)
{
  _zMulMat3DTVec3D( m, v, mv );
  return mv;
}

/* directly multiply a 3D vector by a 3x3 matrix. */
zVec3D *zMulMat3DVec3DDRC(zMat3D *m, zVec3D *v)
{
  _zMulMat3DVec3DDRC( m, v );
  return v;
}

/* directly multiply a 3D vector by transpose of a 3x3 matrix. */
zVec3D *zMulMat3DTVec3DDRC(zMat3D *m, zVec3D *v)
{
  _zMulMat3DTVec3DDRC( m, v );
  return v;
}

/* misc for multiplication of a vector by real inverse of 3x3 matrix. */
double _zMulInvMat3DVec3DRow(zMat3D *m, zVec3D *v, int i, int j, int k, double idet)
{
  return idet *
    ( ( m->e[j][j]*m->e[k][k] - m->e[k][j]*m->e[j][k] ) * v->e[i]
    + ( m->e[k][i]*m->e[j][k] - m->e[j][i]*m->e[k][k] ) * v->e[j]
    + ( m->e[j][i]*m->e[k][j] - m->e[k][i]*m->e[j][j] ) * v->e[k] );
}

/* multiply a 3D vector by inverse matrix of 3x3 matrix. */
zVec3D *_zMulInvMat3DVec3D(zMat3D *m, zVec3D *v, zVec3D *miv, double idet)
{
  _zVec3DCreate( miv,
    _zMulInvMat3DVec3DRow( m, v, 0, 1, 2, idet ),
    _zMulInvMat3DVec3DRow( m, v, 1, 2, 0, idet ),
    _zMulInvMat3DVec3DRow( m, v, 2, 0, 1, idet ) );
  return miv;
}

/* multiply a 3D vector by inverse of a 3x3 matrix. */
zVec3D *zMulInvMat3DVec3D(zMat3D *m, zVec3D *v, zVec3D *miv)
{
  double det;

  if( zIsTiny( ( det = zMat3DDet( m ) ) ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
    return NULL;
  }
  return _zMulInvMat3DVec3D( m, v, miv, 1.0 / det );
}

/* concatenate ratio of vector. */
double *zVec3DCatRatio(zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v, double ratio[])
{
  zMat3D m, im;

  zVec3DCopy( v1, &m.b.x );
  zVec3DCopy( v2, &m.b.y );
  zVec3DCopy( v3, &m.b.z );
  zMat3DInv( &m, &im );
  zMulMat3DVec3D( &im, v, (zVec3D*)ratio );
  return ratio;
}

/* ********************************************************** */
/* multiplication of a 3x3 matrix by another 3x3 matrix
 * ********************************************************** */

/* multiply two 3D matrices. */
zMat3D *zMulMat3DMat3D(zMat3D *m1, zMat3D *m2, zMat3D *m)
{
  zMat3D tmp;

  _zMulMat3DVec3D( m1, &m2->b.x, &tmp.b.x );
  _zMulMat3DVec3D( m1, &m2->b.y, &tmp.b.y );
  _zMulMat3DVec3D( m1, &m2->b.z, &tmp.b.z );
  return zMat3DCopy( &tmp, m );
}

/* multiply a matrix by transpose matrix from leftside. */
zMat3D *zMulMat3DTMat3D(zMat3D *m1, zMat3D *m2, zMat3D *m)
{
  zMat3D tmp;

  _zMulMat3DTVec3D( m1, &m2->b.x, &tmp.b.x );
  _zMulMat3DTVec3D( m1, &m2->b.y, &tmp.b.y );
  _zMulMat3DTVec3D( m1, &m2->b.z, &tmp.b.z );
  return zMat3DCopy( &tmp, m );
}

/* multiply a matrix by transpose matrix from rightside. */
#define __zMulMat3DMat3DTElem(m1,m2,i,j,m) \
  ( (m)->e[i][j] = (m1)->e[0][j]*(m2)->e[0][i] + (m1)->e[1][j]*(m2)->e[1][i] + (m1)->e[2][j]*(m2)->e[2][i] )
zMat3D *zMulMat3DMat3DT(zMat3D *m1, zMat3D *m2, zMat3D *m)
{
  zMat3D tmp;

  __zMulMat3DMat3DTElem( m1, m2, 0, 0, &tmp );
  __zMulMat3DMat3DTElem( m1, m2, 0, 1, &tmp );
  __zMulMat3DMat3DTElem( m1, m2, 0, 2, &tmp );
  __zMulMat3DMat3DTElem( m1, m2, 1, 0, &tmp );
  __zMulMat3DMat3DTElem( m1, m2, 1, 1, &tmp );
  __zMulMat3DMat3DTElem( m1, m2, 1, 2, &tmp );
  __zMulMat3DMat3DTElem( m1, m2, 2, 0, &tmp );
  __zMulMat3DMat3DTElem( m1, m2, 2, 1, &tmp );
  __zMulMat3DMat3DTElem( m1, m2, 2, 2, &tmp );
  return zMat3DCopy( &tmp, m );
}

/* multiply 3x3 matrix by inverse matrix of another 3x3 matrix. */
zMat3D *zMulInvMat3DMat3D(zMat3D *m1, zMat3D *m2, zMat3D *m)
{
  double det, idet;

  if( zIsTiny( ( det = zMat3DDet( m1 ) ) ) ){
    ZRUNERROR( ZEO_ERR_SINGULARMAT );
    return NULL;
  }
  idet = 1.0 /det;
  _zMulInvMat3DVec3D( m1, &m2->b.x, &m->b.x, idet );
  _zMulInvMat3DVec3D( m1, &m2->b.y, &m->b.y, idet );
  _zMulInvMat3DVec3D( m1, &m2->b.z, &m->b.z, idet );
  return m;
}

/* ********************************************************** */
/* multiplication of a 6D spatial vector by a 3x3 matrix
 * ********************************************************** */

/* multiply a 6D vector by 3x3 matrix. */
zVec6D *zMulMat3DVec6D(zMat3D *m, zVec6D *v, zVec6D *mv)
{
  _zMulMat3DVec3D( m, zVec6DLin(v), zVec6DLin(mv) );
  _zMulMat3DVec3D( m, zVec6DAng(v), zVec6DAng(mv) );
  return mv;
}

/* multiply a 6D vector by transpose matrix. */
zVec6D *zMulMat3DTVec6D(zMat3D *m, zVec6D *v, zVec6D *mv)
{
  _zMulMat3DTVec3D( m, zVec6DLin(v), zVec6DLin(mv) );
  _zMulMat3DTVec3D( m, zVec6DAng(v), zVec6DAng(mv) );
  return mv;
}

/* ********************************************************** */
/* rotation
 * ********************************************************** */

/* (static)
 * _zMat3DRotRPYSC
 * - rotate matrix along a base axis.
 */
static zMat3D *_zMat3DRotRPYSC(zMat3D *m, int a0, int a1, int a2, double s, double c, zMat3D *rm);
zMat3D *_zMat3DRotRPYSC(zMat3D *m, int a0, int a1, int a2, double s, double c, zMat3D *rm)
{
  rm->e[a0][a0] = m->e[a0][a0]*c - m->e[a0][a1]*s;
  rm->e[a1][a0] = m->e[a1][a0]*c - m->e[a1][a1]*s;
  rm->e[a2][a0] = m->e[a2][a0]*c - m->e[a2][a1]*s;
  rm->e[a0][a1] = m->e[a0][a0]*s + m->e[a0][a1]*c;
  rm->e[a1][a1] = m->e[a1][a0]*s + m->e[a1][a1]*c;
  rm->e[a2][a1] = m->e[a2][a0]*s + m->e[a2][a1]*c;
  rm->e[a0][a2] = m->e[a0][a2];
  rm->e[a1][a2] = m->e[a1][a2];
  rm->e[a2][a2] = m->e[a2][a2];
  return rm;
}

/* (static)
 * _zMat3DRotRPYSCDRC
 * - rotate matrix directly along a base axis.
 */
static zMat3D *_zMat3DRotRPYSCDRC(zMat3D *m, int a0, int a1, int a2, double s, double c);
zMat3D *_zMat3DRotRPYSCDRC(zMat3D *m, int a0, int a1, int a2, double s, double c)
{
  double r00, r01, r02, r10, r11, r12;

  r00 = m->e[a0][a0];
  r01 = m->e[a1][a0];
  r02 = m->e[a2][a0];
  r10 = m->e[a0][a1];
  r11 = m->e[a1][a1];
  r12 = m->e[a2][a1];
  m->e[a0][a0] = r00*c-r10*s;
  m->e[a1][a0] = r01*c-r11*s;
  m->e[a2][a0] = r02*c-r12*s;
  m->e[a0][a1] = r00*s+r10*c;
  m->e[a1][a1] = r01*s+r11*c;
  m->e[a2][a1] = r02*s+r12*c;
  return m;
}

/* zMat3DRotRollSC, zMat3DRotRoll, zMat3DRotRollSCDRC, zMat3DRotRollDRC
 * - rotate matrix along x-axis.
 */
zMat3D *zMat3DRotRollSC(zMat3D *m, double s, double c, zMat3D *rm){
  return _zMat3DRotRPYSC( m, 1, 2, 0, s, c, rm );
}
zMat3D *zMat3DRotRoll(zMat3D *m, double theta, zMat3D *rm){
  double s, c;
  _zSinCos( theta, &s, &c );
  return zMat3DRotRollSC( m, s, c, rm );
}
zMat3D *zMat3DRotRollSCDRC(zMat3D *m, double s, double c){
  return _zMat3DRotRPYSCDRC( m, 1, 2, 0, s, c );
}
zMat3D *zMat3DRotRollDRC(zMat3D *m, double theta){
  double s, c;
  _zSinCos( theta, &s, &c );
  return zMat3DRotRollSCDRC( m, s, c );
}

/* zMat3DRotPitchSC, zMat3DRotPitch, zMat3DRotPitchSCDRC, zMat3DRotPitchDRC
 * - rotate matrix along y-axis.
 */
zMat3D *zMat3DRotPitchSC(zMat3D *m, double s, double c, zMat3D *rm){
  return _zMat3DRotRPYSC( m, 2, 0, 1, s, c, rm );
}
zMat3D *zMat3DRotPitch(zMat3D *m, double theta, zMat3D *rm){
  double s, c;
  _zSinCos( theta, &s, &c );
  return zMat3DRotPitchSC( m, s, c, rm );
}
zMat3D *zMat3DRotPitchSCDRC(zMat3D *m, double s, double c){
  return _zMat3DRotRPYSCDRC( m, 2, 0, 1, s, c );
}
zMat3D *zMat3DRotPitchDRC(zMat3D *m, double theta){
  double s, c;
  _zSinCos( theta, &s, &c );
  return zMat3DRotPitchSCDRC( m, s, c );
}

/* zMat3DRotYawSC, zMat3DRotYaw, zMat3DRotYawSCDRC, zMat3DRotYawDRC
 * - rotate matrix along z-axis.
 */
zMat3D *zMat3DRotYawSC(zMat3D *m, double s, double c, zMat3D *rm){
  return _zMat3DRotRPYSC( m, 0, 1, 2, s, c, rm );
}
zMat3D *zMat3DRotYaw(zMat3D *m, double theta, zMat3D *rm){
  double s, c;
  _zSinCos( theta, &s, &c );
  return zMat3DRotYawSC( m, s, c, rm );
}
zMat3D *zMat3DRotYawSCDRC(zMat3D *m, double s, double c){
  return _zMat3DRotRPYSCDRC( m, 0, 1, 2, s, c );
}
zMat3D *zMat3DRotYawDRC(zMat3D *m, double theta){
  double s, c;
  _zSinCos( theta, &s, &c );
  return zMat3DRotYawSCDRC( m, s, c );
}

/* zMat3DFromZYXSC, zMat3DFromZYX
 * - 3D attitude matrix expressed by z-y-x Eulerian angle.
 */
zMat3D *zMat3DFromZYXSC(zMat3D *m, double sa, double ca, double se, double ce, double st, double ct)
{
  _zMat3DCreate( m,
    ce*ca, st*se*ca-ct*sa, ct*se*ca+st*sa,
    ce*sa, st*se*sa+ct*ca, ct*se*sa-st*ca,
   -se,    st*ce,          ct*ce );
  return m;
}
zMat3D *zMat3DFromZYX(zMat3D *m, double azim, double elev, double tilt)
{
  double sa, ca, se, ce, st, ct;

  _zSinCos( azim, &sa, &ca );
  _zSinCos( elev, &se, &ce );
  _zSinCos( tilt, &st, &ct );
  return zMat3DFromZYXSC( m, sa, ca, se, ce, st, ct );
}

/* zMat3DToZYX
 * - quasi 3D vector for the expression of 3D attitude
 *   as z-y-x Eulerian angle.
 */
zVec3D *zMat3DToZYX(zMat3D *m, zVec3D *angle)
{
  double azim, ca, sa;

  azim = atan2( m->c.xy, m->c.xx );
  _zSinCos( azim, &sa, &ca );
  angle->c.x = azim;
  angle->c.y = atan2(-m->c.xz, m->c.xx*ca+m->c.xy*sa );
  angle->c.z = atan2( m->c.zx*sa-m->c.zy*ca, -m->c.yx*sa+m->c.yy*ca );
  return angle;
}

/* zMat3DFromZYZSC, zMat3DFromZYZ
 * - 3D attitude matrix expressed by z-y-z Eulerian angle.
 */
zMat3D *zMat3DFromZYZSC(zMat3D *m, double sh, double ch, double sp, double cp, double sb, double cb)
{
  _zMat3DCreate( m,
    ch*cp*cb-sh*sb,-ch*cp*sb-sh*cb, ch*sp,
    sh*cp*cb+ch*sb,-sh*cp*sb+ch*cb, sh*sp,
      -sp*cb,          sp*sb,          cp );
  return m;
}
zMat3D *zMat3DFromZYZ(zMat3D *m, double heading, double pitch, double bank)
{
  double sh, ch, sp, cp, sb, cb;

  _zSinCos( heading, &sh, &ch );
  _zSinCos( pitch, &sp, &cp );
  _zSinCos( bank, &sb, &cb );
  return zMat3DFromZYZSC( m, sh, ch, sp, cp, sb, cb );
}

/* zMat3DToZYZ
 * - quasi 3D vector for the expression of 3D attitude
 *   as z-y-z Eulerian angle.
 */
zVec3D *zMat3DToZYZ(zMat3D *m, zVec3D *angle)
{
  double heading, sh, ch;

  heading = atan2( m->c.zy, m->c.zx );
  _zSinCos( heading, &sh, &ch );
  angle->c.x = heading;
  angle->c.y = atan2( m->c.zx*ch+m->c.zy*sh, m->c.zz );
  angle->c.z = atan2(-m->c.xx*sh+m->c.xy*ch,-m->c.yx*sh+m->c.yy*ch );
  return angle;
}

/* zMat3DFromAA
 * - 3D attitude matrix expressed by angle-axis vector.
 */
zMat3D *zMat3DFromAA(zMat3D *m, zVec3D *aa)
{
  double l, c, s, r1, r2, r3;
  zVec3D n, nl;

  if( _zVec3DIsTiny( aa ) ) return zMat3DIdent( m );
  l = _zVec3DNorm( aa );
  _zSinCos( l, &s, &c );
  zVec3DMul( aa, 1.0/l, &n );
  zVec3DMul( &n, 1-c, &nl );
  _zVec3DTripleProd2Mat3D( &n, &nl, m );
  r1 = s * n.c.x;
  r2 = s * n.c.y;
  r3 = s * n.c.z;
  m->e[0][0] += 1.0;
  m->e[1][0] -= r3;
  m->e[2][0] += r2;
  m->e[0][1] += r3;
  m->e[1][1] += 1.0;
  m->e[2][1] -= r1;
  m->e[0][2] -= r2;
  m->e[1][2] += r1;
  m->e[2][2] += 1.0;
  return m;
}

/* zMat3DToAA
 * - convert 3x3 matrix to equivalent angle-axis vector.
 */
zVec3D *zMat3DToAA(zMat3D *m, zVec3D *aa)
{
  register int i;
  double l, a;
  double eval[3];
  zVec3D evec[3];

  _zVec3DCreate( aa, m->c.yz-m->c.zy, m->c.zx-m->c.xz, m->c.xy-m->c.yx );
  l = _zVec3DNorm( aa );
  a = atan2( l, m->c.xx+m->c.yy+m->c.zz-1 );
  if( zIsTiny( l ) ){ /* singular case */
    zMat3DSymEig( m, eval, evec );
    for( i=0; i<3; i++ )
      if( zIsTiny( eval[i] - 1.0 ) )
        return zVec3DMul( &evec[i], a, aa );
  }
  return zVec3DMulDRC( aa, a/l );
}

/* rotational multiplication for 3D matrices. */
zMat3D *zRotMat3D(zMat3D *r, zMat3D *m, zMat3D *rm)
{
  zMulMat3DMat3D( r, m, rm );
  return zMulMat3DMat3DTDRC( rm, r );
}

/* inverse rotational multiplication for 3D matrices. */
zMat3D *zRotMat3DInv(zMat3D *r, zMat3D *m, zMat3D *rm)
{
  zMulMat3DMat3D( m, r, rm );
  return zMulMat3DTMat3DDRC( r, rm );
}

/* rotate a 3x3 matrix along an arbitrary axis. */
zMat3D *zMat3DRot(zMat3D *m, zVec3D *aa, zMat3D *rm)
{
  zMat3D ma;

  zMat3DFromAA( &ma, aa );
  return zMulMat3DMat3D( &ma, m, rm );
}

/* concatenate a 3D rotational vector to a 3x3 matrix. */
zMat3D *zMat3DRotCat(zMat3D *m, zVec3D *omega, double dt, zMat3D *rm)
{
  zVec3D aa;

  zVec3DMul( omega, dt, &aa );
  return zMat3DRot( m, &aa, rm );
}

/* cascade an angle-axis vector to another. */
zVec3D *zAACascade(zVec3D *aa1, zVec3D *aa2, zVec3D *aa)
{
  zMat3D m1, m2, m;

  zMat3DFromAA( &m1, aa1 );
  zMat3DFromAA( &m2, aa2 );
  zMulMat3DMat3D( &m2, &m1, &m );
  return zMat3DToAA( &m, aa );
}

/* error vector between two attitude matrices. */
zVec3D *zMat3DError(zMat3D *m1, zMat3D *m2, zVec3D *err)
{
  zMat3D em;

  zMulMat3DMat3DT( m1, m2, &em );
  return zMat3DToAA( &em, err );
}

/* error vector between two angle-axis vectors. */
zVec3D *zAAError(zVec3D *a1, zVec3D *a2, zVec3D *err)
{
  zMat3D m1, m2;

  zMat3DFromAA( &m1, a1 );
  zMat3DFromAA( &m2, a2 );
  return zMat3DError( &m1, &m2, err );
}

/* ********************************************************** */
/* differential kinematics
 * ********************************************************** */

/* numerical differentiation of 3x3 matrix. */
zVec3D *zMat3DDif(zMat3D *m, zMat3D *mnew, double dt, zVec3D *omega)
{
  zMat3DError( mnew, m, omega );
  return zVec3DDivDRC( omega, dt );
}

/* ********************************************************** */
/* eigensystem
 * ********************************************************** */

/* transformation of a symmetric matrix by Jacobi's rotation. */
static bool _zMat3DSymEigRot(zMat3D *m, zMat3D *r, int i, int j);
bool _zMat3DSymEigRot(zMat3D *m, zMat3D *r, int i, int j)
{
  register int k;
  double v, t, ti, c, s;
  double tmp1, tmp2;

  if( zIsTiny( m->e[j][i] ) ) return true;
  v = 0.5 * ( m->e[j][j] - m->e[i][i] ) / m->e[j][i];
  ti = sqrt( v * v + 1 ); /* sqrt */
  t = -v + ( v < 0 ? -ti : ti );
  s = t * ( c = 1 / sqrt( t*t + 1 ) );
  tmp1 = t * m->e[j][i];
  m->e[j][i] = m->e[i][j] = 0;
  m->e[i][i] -= tmp1;
  m->e[j][j] += tmp1;
  for( k=0; k<3; k++ ){
    /* update of transformation matrix */
    tmp1 = r->e[i][k];
    tmp2 = r->e[j][k];
    r->e[i][k] = c * tmp1 - s * tmp2;
    r->e[j][k] = s * tmp1 + c * tmp2;
    /* update of eigenmatrix */
    if( k == i || k == j ) continue;
    tmp1 = m->e[k][i];
    tmp2 = m->e[k][j];
    m->e[k][i] = m->e[i][k] = c * ( tmp1 - t * tmp2 );
    m->e[k][j] = m->e[j][k] = c * ( tmp2 + t * tmp1 );
  }
  return false;
}

/* eigensystem of a symmetric 3x3 matrix by Jacobi's method. */
void zMat3DSymEig(zMat3D *m, double eval[], zVec3D evec[])
{
  int n = 0;
  zMat3D l, r;
  bool ok;

  zMat3DCopy( m, &l );
  zMat3DIdent( &r );
  /* iterative elimination of non-diagonal components */
  do{
    ok = true;
    if( !_zMat3DSymEigRot( &l, &r, 1, 0 ) ) ok = false;
    if( !_zMat3DSymEigRot( &l, &r, 2, 0 ) ) ok = false;
    if( !_zMat3DSymEigRot( &l, &r, 2, 1 ) ) ok = false;
    if( n++ > Z_MAX_ITER_NUM ){
      ZITERWARN( Z_MAX_ITER_NUM );
      break;
    }
  } while( !ok );
  for( n=0; n<3; n++ ){
    eval[n] = l.e[n][n];
    zVec3DCopy( &r.v[n], &evec[n] );
  }
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* input a 3x3 matrix from file. */
zMat3D *zMat3DFRead(FILE *fp, zMat3D *m)
{
  register int i, j;

  for( i=0; i<3; i++ )
    for( j=0; j<3; j++ )
      m->e[j][i] = zFDouble( fp );
  return m;
}

/* output a 3x3 matrix to file. */
void zMat3DFWrite(FILE *fp, zMat3D *m)
{
  if( !m )
    fprintf( fp, "(null 3x3 matrix)\n" );
  else{
    fprintf( fp, "{\n" );
    fprintf( fp, " %.10g, %.10g, %.10g\n", m->c.xx, m->c.yx, m->c.zx );
    fprintf( fp, " %.10g, %.10g, %.10g\n", m->c.xy, m->c.yy, m->c.zy );
    fprintf( fp, " %.10g, %.10g, %.10g\n", m->c.xz, m->c.yz, m->c.zz );
    fprintf( fp, "}\n" );
  }
}
