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
#ifdef __cplusplus
const zMat3D zMat3D::zmat3Dzero  = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
const zMat3D zMat3D::zmat3Dident = { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } } };
#else
const zMat3D zmat3Dzero  = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
const zMat3D zmat3Dident = { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } } };
#endif /* __cplusplus */

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
bool zMat3DMatch(const zMat3D *m1, const zMat3D *m2)
{
  return _zMat3DMatch( m1, m2 );
}

/* check if two 3x3 matrices are equal. */
bool zMat3DEqual(const zMat3D *m1, const zMat3D *m2)
{
  return _zMat3DEqual( m1, m2 );
}

/* check if a 3x3 matrix is the identity matrix. */
bool zMat3DIsIdent(const zMat3D *m)
{
  return _zMat3DIsIdent( m );
}

/* check if a 3D matrix is tiny. */
bool zMat3DIsTol(const zMat3D *m, double tol)
{
  return _zMat3DIsTol( m, tol );
}

/* abstract row vector from a 3x3 matrix. */
zVec3D *zMat3DRow(const zMat3D *m, int i, zVec3D *v)
{
  _zMat3DRow( m, i, v );
  return v;
}

/* transpose a 3x3 matrix. */
zMat3D *zMat3DT(const zMat3D *m, zMat3D *tm)
{
  if( tm == m ){
    _zMat3DTDRC( tm );
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
zMat3D *zMat3DAdd(const zMat3D *m1, const zMat3D *m2, zMat3D *m)
{
  _zMat3DAdd( m1, m2, m );
  return m;
}

/* subtract a 3x3 matrix from another. */
zMat3D *zMat3DSub(const zMat3D *m1, const zMat3D *m2, zMat3D *m)
{
  _zMat3DSub( m1, m2, m );
  return m;
}

/* reverse a 3x3 matrix. */
zMat3D *zMat3DRev(const zMat3D *m, zMat3D *rm)
{
  _zMat3DRev( m, rm );
  return rm;
}

/* multiply a 3x3 matrix by a scalar. */
zMat3D *zMat3DMul(const zMat3D *m, double k, zMat3D *mm)
{
  _zMat3DMul( m, k, mm );
  return mm;
}

/* divide a 3x3 matrix by a scalar. */
zMat3D *zMat3DDiv(const zMat3D *m, double k, zMat3D *dm)
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
zMat3D *zMat3DCat(const zMat3D *m1, double k, const zMat3D *m2, zMat3D *m)
{
  _zMat3DCat( m1, k, m2, m );
  return m;
}

/* dyadic product of two 3D vectors. */
zMat3D *zMat3DDyad(zMat3D *m, const zVec3D *v1, const zVec3D *v2)
{
  _zMat3DDyad( m, v1, v2 );
  return m;
}

/* add dyadic product of two 3D vectors to a 3x3 matrix. */
zMat3D *zMat3DAddDyad(zMat3D *m, const zVec3D *v1, const zVec3D *v2)
{
  _zMat3DAddDyad( m, v1, v2 );
  return m;
}

/* subtract dyadic product of two 3D vectors from a 3x3 matrix. */
zMat3D *zMat3DSubDyad(zMat3D *m, const zVec3D *v1, const zVec3D *v2)
{
  _zMat3DSubDyad( m, v1, v2 );
  return m;
}

/* create an outer-product matrix of a 3D vector. */
zMat3D *zVec3DOuterProd2Mat3D(const zVec3D *v, zMat3D *m)
{
  _zVec3DOuterProd2Mat3D( v, m );
  return m;
}

/* create a triple-product matrix of a 3D vector. */
zMat3D *zVec3DTripleProd2Mat3D(const zVec3D *v1, const zVec3D *v2, zMat3D *m)
{
  _zVec3DTripleProd2Mat3D( v1, v2, m );
  return m;
}

/* create a double outer-product matrix of a 3D vector. */
zMat3D *zVec3DDoubleOuterProd2Mat3D(const zVec3D *v, zMat3D *m)
{
  _zVec3DDoubleOuterProd2Mat3D( v, m );
  return m;
}

/* concatenate a double outer-product matrix of a 3D vector to another matrix. */
zMat3D *zMat3DCatVec3DDoubleOuterProd(const zMat3D *m, double k, const zVec3D *v, zMat3D *dm)
{
  _zMat3DCatVec3DDoubleOuterProd( m, k, v, dm );
  return dm;
}

/* multiply cross product of vector and matrix. */
zMat3D *zMulVec3DOuterProdMat3D(const zVec3D *ohm, const zMat3D *m, zMat3D *mv)
{
  zVec3DOuterProd( ohm, &m->b.x, &mv->b.x );
  zVec3DOuterProd( ohm, &m->b.y, &mv->b.y );
  zVec3DOuterProd( ohm, &m->b.z, &mv->b.z );
  return mv;
}

/* calculate squared norm of a 3x3 matrix. */
double zMat3DSqrNorm(const zMat3D *m)
{
  return _zMat3DSqrNorm( m );
}

/* ********************************************************** */
/* inverse of a 3x3 matrix
 * ********************************************************** */

/* find indices of maximum and minimum components of a 3D vector. */
static double _zMat3DBalancingMinMaxIndex(const zVec3D *v, int *imax, int *imin)
{
  if( v->e[0] > v->e[1] ){
    *imax = 0; *imin = 1;
  } else{
    *imax = 1; *imin = 0;
  }
  if( v->e[*imax] < v->e[2] ){
    *imax = 2;
  } else
  if( v->e[*imin] > v->e[2] ){
    *imin = 2;
  }
  return v->e[*imax];
}

/* find a scaling vector for row-balancing of a 3x3 matrix. */
static double _zMat3DRowBalancingScale(const zMat3D *m, zVec3D *s)
{
  zVec3D v;
  int imax, imin;

  _zVec3DCreate( &v, fabs(m->e[0][0]), fabs(m->e[1][0]), fabs(m->e[2][0]) );
  s->e[0] = _zMat3DBalancingMinMaxIndex( &v, &imax, &imin );
  _zVec3DCreate( &v, fabs(m->e[0][1]), fabs(m->e[1][1]), fabs(m->e[2][1]) );
  s->e[1] = _zMat3DBalancingMinMaxIndex( &v, &imax, &imin );
  _zVec3DCreate( &v, fabs(m->e[0][2]), fabs(m->e[1][2]), fabs(m->e[2][2]) );
  s->e[2] = _zMat3DBalancingMinMaxIndex( &v, &imax, &imin );
  _zMat3DBalancingMinMaxIndex( s, &imax, &imin );
  return s->e[imin] / s->e[imax];
}

/* find a scaling vector for column-balancing of a 3x3 matrix. */
static double _zMat3DColBalancingScale(const zMat3D *m, zVec3D *s)
{
  zVec3D v;
  int imax, imin;

  _zVec3DCreate( &v, fabs(m->e[0][0]), fabs(m->e[0][1]), fabs(m->e[0][2]) );
  s->e[0] = _zMat3DBalancingMinMaxIndex( &v, &imax, &imin );
  _zVec3DCreate( &v, fabs(m->e[1][0]), fabs(m->e[1][1]), fabs(m->e[1][2]) );
  s->e[1] = _zMat3DBalancingMinMaxIndex( &v, &imax, &imin );
  _zVec3DCreate( &v, fabs(m->e[2][0]), fabs(m->e[2][1]), fabs(m->e[2][2]) );
  s->e[2] = _zMat3DBalancingMinMaxIndex( &v, &imax, &imin );
  _zMat3DBalancingMinMaxIndex( s, &imax, &imin );
  return s->e[imin] / s->e[imax];
}

/* scale a 3x3 matrix for row-balancing. */
#define _zMat3DRowBalancing(m,s,mb) _zMat3DCreate( mb,\
  (m)->e[0][0] / (s)->e[0], (m)->e[1][0] / (s)->e[0], (m)->e[2][0] / (s)->e[0],\
  (m)->e[0][1] / (s)->e[1], (m)->e[1][1] / (s)->e[1], (m)->e[2][1] / (s)->e[1],\
  (m)->e[0][2] / (s)->e[2], (m)->e[1][2] / (s)->e[2], (m)->e[2][2] / (s)->e[2] )

/* scale a 3x3 matrix for column-balancing. */
#define _zMat3DColBalancing(m,s,mb) _zMat3DCreate( mb,\
  (m)->e[0][0] / (s)->e[0], (m)->e[1][0] / (s)->e[1], (m)->e[2][0] / (s)->e[2],\
  (m)->e[0][1] / (s)->e[0], (m)->e[1][1] / (s)->e[1], (m)->e[2][1] / (s)->e[2],\
  (m)->e[0][2] / (s)->e[0], (m)->e[1][2] / (s)->e[1], (m)->e[2][2] / (s)->e[2] )

/* determinant of a 3x3 matrix. */
double zMat3DDet(const zMat3D *m)
{
  return _zMat3DDet( m );
}

/* misc for inverse of a 3x3 matrix. */
#define _zMat3DInvRow(m,im,i,j,k,idet) do{\
  (im)->e[(i)][(i)] = (idet) * ( (m)->e[(j)][(j)]*(m)->e[(k)][(k)] - (m)->e[(k)][(j)]*(m)->e[(j)][(k)] );\
  (im)->e[(j)][(i)] = (idet) * ( (m)->e[(k)][(i)]*(m)->e[(j)][(k)] - (m)->e[(j)][(i)]*(m)->e[(k)][(k)] );\
  (im)->e[(k)][(i)] = (idet) * ( (m)->e[(j)][(i)]*(m)->e[(k)][(j)] - (m)->e[(k)][(i)]*(m)->e[(j)][(j)] );\
} while(0)

/* inverse matrix of a 3x3 matrix. */
static zMat3D *_zMat3DInv(const zMat3D *m, zMat3D *im)
{
  double det, idet;

  det = _zMat3DDet( m );
  if( zIsTiny( det ) ){
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
    return NULL;
  }
  idet = 1.0 / det;
  _zMat3DInvRow( m, im, 0, 1, 2, idet );
  _zMat3DInvRow( m, im, 1, 2, 0, idet );
  _zMat3DInvRow( m, im, 2, 0, 1, idet );
  return im;
}

/* inverse matrix of a 3x3 matrix. */
zMat3D *zMat3DInv(const zMat3D *m, zMat3D *im)
{
  zMat3D mb_tmp, mb;
  zVec3D sr, sc;

  if( _zMat3DRowBalancingScale( m, &sr ) < _zMat3DColBalancingScale( m, &sc ) ){
    _zMat3DRowBalancing( m, &sr, &mb_tmp );
    _zMat3DColBalancingScale( &mb_tmp, &sc );
    _zMat3DColBalancing( &mb_tmp, &sc, &mb );
    _zMat3DInv( &mb, im );
    _zMat3DRowBalancing( im, &sc, im );
    _zMat3DColBalancing( im, &sr, im );
  } else{
    _zMat3DColBalancing( m, &sc, &mb_tmp );
    _zMat3DRowBalancingScale( &mb_tmp, &sr );
    _zMat3DRowBalancing( &mb_tmp, &sr, &mb );
    _zMat3DInv( &mb, im );
    _zMat3DColBalancing( im, &sr, im );
    _zMat3DRowBalancing( im, &sc, im );
  }
  return im;
}

/* ********************************************************** */
/* multiplication of a 3D vector by a 3x3 matrix
 * ********************************************************** */

/* multiply a 3D vector by a 3x3 matrix. */
zVec3D *zMulMat3DVec3D(const zMat3D *m, const zVec3D *v, zVec3D *mv)
{
  _zMulMat3DVec3D( m, v, mv );
  return mv;
}

/* multiply a 3D vector by transpose of a 3x3 matrix. */
zVec3D *zMulMat3DTVec3D(const zMat3D *m, const zVec3D *v, zVec3D *mv)
{
  _zMulMat3DTVec3D( m, v, mv );
  return mv;
}

/* directly multiply a 3D vector by a 3x3 matrix. */
zVec3D *zMulMat3DVec3DDRC(const zMat3D *m, zVec3D *v)
{
  _zMulMat3DVec3DDRC( m, v );
  return v;
}

/* directly multiply a 3D vector by transpose of a 3x3 matrix. */
zVec3D *zMulMat3DTVec3DDRC(const zMat3D *m, zVec3D *v)
{
  _zMulMat3DTVec3DDRC( m, v );
  return v;
}

/* misc for multiplication of a vector by real inverse of 3x3 matrix. */
static double _zMulInvMat3DVec3DRow(const zMat3D *m, const zVec3D *v, int i, int j, int k, double idet)
{
  return idet *
    ( ( m->e[j][j]*m->e[k][k] - m->e[k][j]*m->e[j][k] ) * v->e[i]
    + ( m->e[k][i]*m->e[j][k] - m->e[j][i]*m->e[k][k] ) * v->e[j]
    + ( m->e[j][i]*m->e[k][j] - m->e[k][i]*m->e[j][j] ) * v->e[k] );
}

#define _zMulInvMat3DVec3DInvDet(m,v,imv,idet) _zVec3DCreate( imv,\
  _zMulInvMat3DVec3DRow( m, v, 0, 1, 2, idet ),\
  _zMulInvMat3DVec3DRow( m, v, 1, 2, 0, idet ),\
  _zMulInvMat3DVec3DRow( m, v, 2, 0, 1, idet ) )

/* multiply a 3D vector by inverse of a 3x3 matrix. */
static zVec3D *_zMulInvMat3DVec3D(const zMat3D *m, const zVec3D *v, zVec3D *imv)
{
  double val;

  if( zIsTiny( ( val = zMat3DDet( m ) ) ) ){
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
    return NULL;
  }
  val = 1.0 / val;
  _zMulInvMat3DVec3DInvDet( m, v, imv, val );
  return imv;
}

/* multiply a 3D vector by inverse of a 3x3 matrix. */
zVec3D *zMulInvMat3DVec3D(const zMat3D *m, const zVec3D *v, zVec3D *imv)
{
  zMat3D mb_tmp, mb;
  zVec3D v_tmp, sr, sc;

  if( _zMat3DRowBalancingScale( m, &sr ) < _zMat3DColBalancingScale( m, &sc ) ){
    _zMat3DRowBalancing( m, &sr, &mb_tmp );
    _zMat3DColBalancingScale( &mb_tmp, &sc );
    _zMat3DColBalancing( &mb_tmp, &sc, &mb );
  } else{
    _zMat3DColBalancing( m, &sc, &mb_tmp );
    _zMat3DRowBalancingScale( &mb_tmp, &sr );
    _zMat3DRowBalancing( &mb_tmp, &sr, &mb );
  }
  _zVec3DDem( v, &sr, &v_tmp );
  _zMulInvMat3DVec3D( &mb, &v_tmp, imv );
  _zVec3DDem( imv, &sc, imv );
  return imv;
}

/* concatenate ratio of vector. */
double *zVec3DCatRatio(const zVec3D *v1, const zVec3D *v2, const zVec3D *v3, const zVec3D *v, double ratio[])
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
zMat3D *zMulMat3DMat3D(const zMat3D *m1, const zMat3D *m2, zMat3D *m)
{
  zMat3D tmp;

  _zMulMat3DVec3D( m1, &m2->b.x, &tmp.b.x );
  _zMulMat3DVec3D( m1, &m2->b.y, &tmp.b.y );
  _zMulMat3DVec3D( m1, &m2->b.z, &tmp.b.z );
  return zMat3DCopy( &tmp, m );
}

/* multiply a matrix by transpose matrix from leftside. */
zMat3D *zMulMat3DTMat3D(const zMat3D *m1, const zMat3D *m2, zMat3D *m)
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
zMat3D *zMulMat3DMat3DT(const zMat3D *m1, const zMat3D *m2, zMat3D *m)
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

/* multiply 3x3 matrix by inverse of another 3x3 matrix. */
static zMat3D *_zMulInvMat3DMat3D(const zMat3D *m1, const zMat3D *m2, zMat3D *m)
{
  double val;

  if( zIsTiny( ( val = zMat3DDet( m1 ) ) ) ){
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
    return NULL;
  }
  val = 1.0 / val;
  _zMulInvMat3DVec3DInvDet( m1, &m2->b.x, &m->b.x, val );
  _zMulInvMat3DVec3DInvDet( m1, &m2->b.y, &m->b.y, val );
  _zMulInvMat3DVec3DInvDet( m1, &m2->b.z, &m->b.z, val );
  return m;
}

/* multiply a 3x3 matrix by inverse of aother 3x3 matrix. */
zMat3D *zMulInvMat3DMat3D(const zMat3D *m1, const zMat3D *m2, zMat3D *m)
{
  zMat3D mb_tmp, mb, m2_tmp;
  zVec3D sr, sc;

  if( _zMat3DRowBalancingScale( m1, &sr ) < _zMat3DColBalancingScale( m1, &sc ) ){
    _zMat3DRowBalancing( m1, &sr, &mb_tmp );
    _zMat3DColBalancingScale( &mb_tmp, &sc );
    _zMat3DColBalancing( &mb_tmp, &sc, &mb );
  } else{
    _zMat3DColBalancing( m1, &sc, &mb_tmp );
    _zMat3DRowBalancingScale( &mb_tmp, &sr );
    _zMat3DRowBalancing( &mb_tmp, &sr, &mb );
  }
  _zMat3DRowBalancing( m2, &sr, &m2_tmp );
  _zMulInvMat3DMat3D( &mb, &m2_tmp, m );
  _zMat3DRowBalancing( m, &sc, m );
  return m;
}

/* ********************************************************** */
/* multiplication of a 6D spatial vector by a 3x3 matrix
 * ********************************************************** */

/* multiply a 6D vector by 3x3 matrix. */
zVec6D *zMulMat3DVec6D(const zMat3D *m, const zVec6D *v, zVec6D *mv)
{
  _zMulMat3DVec3D( m, zVec6DLin(v), zVec6DLin(mv) );
  _zMulMat3DVec3D( m, zVec6DAng(v), zVec6DAng(mv) );
  return mv;
}

/* multiply a 6D vector by transpose matrix. */
zVec6D *zMulMat3DTVec6D(const zMat3D *m, const zVec6D *v, zVec6D *mv)
{
  _zMulMat3DTVec3D( m, zVec6DLin(v), zVec6DLin(mv) );
  _zMulMat3DTVec3D( m, zVec6DAng(v), zVec6DAng(mv) );
  return mv;
}

/* ********************************************************** */
/* rotation
 * ********************************************************** */

/* rotate matrix along a base axis. */
static zMat3D *_zMat3DRotRPYSC(const zMat3D *m, int a0, int a1, int a2, double s, double c, zMat3D *rm)
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

/* rotate matrix directly along a base axis. */
static zMat3D *_zMat3DRotRPYSCDRC(zMat3D *m, int a0, int a1, int a2, double s, double c)
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

/* rotate matrix along x-axis. */
zMat3D *zMat3DRotRollSC(const zMat3D *m, double s, double c, zMat3D *rm){
  return _zMat3DRotRPYSC( m, 1, 2, 0, s, c, rm );
}
zMat3D *zMat3DRotRoll(const zMat3D *m, double theta, zMat3D *rm){
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

/* rotate matrix along y-axis. */
zMat3D *zMat3DRotPitchSC(const zMat3D *m, double s, double c, zMat3D *rm){
  return _zMat3DRotRPYSC( m, 2, 0, 1, s, c, rm );
}
zMat3D *zMat3DRotPitch(const zMat3D *m, double theta, zMat3D *rm){
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

/* rotate matrix along z-axis. */
zMat3D *zMat3DRotYawSC(const zMat3D *m, double s, double c, zMat3D *rm){
  return _zMat3DRotRPYSC( m, 0, 1, 2, s, c, rm );
}
zMat3D *zMat3DRotYaw(const zMat3D *m, double theta, zMat3D *rm){
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

/* create a 3D attitude matrix from z-y-x Eulerian angles. */
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

/* convert a 3D attitude matrix to a quasi 3D vector for z-y-x Eulerian angles. */
zVec3D *zMat3DToZYX(const zMat3D *m, zVec3D *angle)
{
  double azim, ca, sa;

  azim = atan2( m->c.xy, m->c.xx );
  _zSinCos( azim, &sa, &ca );
  angle->c.x = azim;
  angle->c.y = atan2(-m->c.xz, m->c.xx*ca+m->c.xy*sa );
  angle->c.z = atan2( m->c.zx*sa-m->c.zy*ca, -m->c.yx*sa+m->c.yy*ca );
  return angle;
}

/* create a 3D attitude matrix from z-y-z Eulerian angles. */
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

/* convert a 3D attitude matrix to a quasi 3D vector for z-y-z Eulerian angles. */
zVec3D *zMat3DToZYZ(const zMat3D *m, zVec3D *angle)
{
  double heading, sh, ch;

  heading = atan2( m->c.zy, m->c.zx );
  _zSinCos( heading, &sh, &ch );
  angle->c.x = heading;
  angle->c.y = atan2( m->c.zx*ch+m->c.zy*sh, m->c.zz );
  angle->c.z = atan2(-m->c.xx*sh+m->c.xy*ch,-m->c.yx*sh+m->c.yy*ch );
  return angle;
}

/* create a 3D attitude matrix from an angle-axis vector. */
zMat3D *zMat3DFromAA(zMat3D *m, const zVec3D *aa)
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

/* convert a 3D attitude matrix to an angle-axis vector. */
zVec3D *zMat3DToAA(const zMat3D *m, zVec3D *aa)
{
  int i;
  double l, a;
  zVec3D eigval;
  zMat3D eigbase;

  _zVec3DCreate( aa, m->c.yz-m->c.zy, m->c.zx-m->c.xz, m->c.xy-m->c.yx );
  l = _zVec3DNorm( aa );
  a = atan2( l, m->c.xx+m->c.yy+m->c.zz-1 );
  if( zIsTiny( l ) ){ /* singular case */
    zMat3DSymEig( m, &eigval, &eigbase );
    for( i=0; i<3; i++ )
      if( zIsTiny( eigval.e[i] - 1.0 ) )
        return zVec3DMul( &eigbase.v[i], a, aa );
  }
  return zVec3DMulDRC( aa, a/l );
}

/* rotational multiplication for 3D matrices. */
zMat3D *zRotMat3D(const zMat3D *r, const zMat3D *m, zMat3D *rm)
{
  zMulMat3DMat3D( r, m, rm );
  return zMulMat3DMat3DTDRC( rm, r );
}

/* inverse rotational multiplication for 3D matrices. */
zMat3D *zRotMat3DInv(const zMat3D *r, const zMat3D *m, zMat3D *rm)
{
  zMulMat3DMat3D( m, r, rm );
  return zMulMat3DTMat3DDRC( r, rm );
}

/* rotate a 3x3 matrix along an arbitrary axis. */
zMat3D *zMat3DRot(const zMat3D *m, const zVec3D *aa, zMat3D *rm)
{
  zMat3D ma;

  zMat3DFromAA( &ma, aa );
  return zMulMat3DMat3D( &ma, m, rm );
}

/* concatenate a 3D rotational vector to a 3x3 matrix. */
zMat3D *zMat3DRotCat(const zMat3D *m, const zVec3D *omega, double dt, zMat3D *rm)
{
  zVec3D aa;

  zVec3DMul( omega, dt, &aa );
  return zMat3DRot( m, &aa, rm );
}

/* cascade an angle-axis vector to another. */
zVec3D *zAACascade(const zVec3D *aa1, const zVec3D *aa2, zVec3D *aa)
{
  zMat3D m1, m2, m;

  zMat3DFromAA( &m1, aa1 );
  zMat3DFromAA( &m2, aa2 );
  zMulMat3DMat3D( &m2, &m1, &m );
  return zMat3DToAA( &m, aa );
}

/* error vector between two attitude matrices. */
zVec3D *zMat3DError(const zMat3D *m1, const zMat3D *m2, zVec3D *err)
{
  zMat3D em;

  zMulMat3DMat3DT( m1, m2, &em );
  return zMat3DToAA( &em, err );
}

/* error vector between two angle-axis vectors. */
zVec3D *zAAError(const zVec3D *a1, const zVec3D *a2, zVec3D *err)
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
zVec3D *zMat3DDif(const zMat3D *m, const zMat3D *mnew, double dt, zVec3D *omega)
{
  zMat3DError( mnew, m, omega );
  return zVec3DDivDRC( omega, dt );
}

/* ********************************************************** */
/* eigensystem
 * ********************************************************** */

/* calculate the dominant eigenvalue. */
double zMat3DEigPower(const zMat3D *m, zVec3D *eigvec, int iter)
{
  zVec3D ev, err;
  double eigval = 1.0;
  int i;

  zVec3DCreate( eigvec, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  ZITERINIT( iter );
  for( i=0; i<iter; i++ ){
    zVec3DNormalize( eigvec, &ev );
    zMulMat3DVec3D( m, &ev, eigvec );
    eigval = zVec3DInnerProd( &ev, eigvec );
    zVec3DCat( eigvec, -eigval, &ev, &err );
    if( zVec3DIsTiny( &err ) ) break;
  }
  zVec3DDivDRC( eigvec, eigval );
  return eigval;
}

/* calculate the minimal eigenvalue. */
double zMat3DEigPowerInv(const zMat3D *m, zVec3D *eigvec, int iter)
{
  zMat3D m_inv;
  double eigval = 0;

  if( zMat3DInv( m, &m_inv ) )
    eigval = zMat3DEigPower( &m_inv, eigvec, iter );
  else
    ZRUNERROR( ZEO_ERR_MAT_SINGULAR );
  return 1.0 / eigval;
}

/* transformation of a symmetric matrix by Jacobi's rotation. */
static void _zMat3DSymEigRot(zMat3D *m, zMat3D *r, int i, int j)
{
  int k;
  double v, t, ti, c, s;
  double tmp1, tmp2;

  if( zIsTiny( m->e[j][i] ) ) return;
  v = 0.5 * ( m->e[j][j] - m->e[i][i] ) / m->e[j][i];
  ti = sqrt( v * v + 1 );
  t = -v + ( v < 0 ? ti : -ti );
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
    /* update of symmetric matrix */
    if( k == i || k == j ) continue;
    tmp1 = m->e[k][i];
    tmp2 = m->e[k][j];
    m->e[k][i] = m->e[i][k] = c * tmp1 - s * tmp2;
    m->e[k][j] = m->e[j][k] = s * tmp1 + c * tmp2;
  }
}

/* eigensystem of a symmetric 3x3 matrix by Jacobi's method. */
bool zMat3DSymEig(const zMat3D *m, zVec3D *eigval, zMat3D *eigbase)
{
  int iter;
  int i, j, i1, j1, i2, j2;
  zMat3D l;
  bool ret = true;

  zMat3DCopy( m, &l );
  zMat3DIdent( eigbase );
  /* iterative elimination of non-diagonal components */
  i = 0; j = 1;
  for( iter=0; i<Z_MAX_ITER_NUM; iter++ ){
    _zMat3DSymEigRot( &l, eigbase, i, j );
    i1 = ( i + 1 ) % 3; j1 = ( j + 1 ) % 3;
    i2 = ( i + 2 ) % 3; j2 = ( j + 2 ) % 3;
    if( fabs( l.e[j1][i1] ) > fabs( l.e[j2][i2] ) ){
      i = i1; j = j1;
    } else{
      i = i2; j = j2;
    }
    if( zIsTiny( l.e[j][i] ) ) goto TERMINATE;
  }
  ZITERWARN( Z_MAX_ITER_NUM );
  ret = false;

 TERMINATE:
  for( i=0; i<3; i++ )
    eigval->e[i] = l.e[i][i];
  return ret;
}

/* eigenvector of a 3x3 symmetric matrix corresponding to the minimum eigenavlue. */
zVec3D *zMat3DSymEigMin(const zMat3D *m, zVec3D *eig)
{
  zVec3D eigval;
  zMat3D eigbase;

  zMat3DSymEig( m, &eigval, &eigbase );
  return zVec3DCopy( &eigbase.v[_zMat3DEigMinID( eigval.e )], eig );
}

/* singular value decomposition of a 3x3 matrix. */
int zMat3DSVD(const zMat3D *m, zMat3D *u, zVec3D *sv, zMat3D *v)
{
  zMat3D m2;
  int i, rank;

  zMulMat3DMat3DT( m, m, &m2 );
  zMat3DSymEig( &m2, sv, u );
  zMulMat3DTMat3D( m, u, v );
  for( rank=0, i=0; i<3; i++ ){
    if( zIsTiny( sv->e[i] ) )
      sv->e[i] = 0;
    else{
      zVec3DDivDRC( zMat3DVec(v,i), ( sv->e[i] = sqrt( sv->e[i] ) ) );
      rank++;
    }
  }
  return rank;
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 3x3 matrix from a ZTK format processor. */
zMat3D *zMat3DFromZTK(zMat3D *m, ZTK *ztk)
{
  char buf[BUFSIZ];
  int i, j;

  if( ZTKVal(ztk)[0] == '{' ){
    strncpy( buf, ZTKVal(ztk), BUFSIZ-1 );
    for( i=0; i<3; i++ )
      for( j=0; j<3; j++ )
        zSDouble( buf, &m->e[j][i] );
  } else{
    for( i=0; i<3; i++ )
      for( j=0; j<3; j++ )
        m->e[j][i] = ZTKDouble(ztk);
  }
  return m;
}

/* add a 3x3 matrix to a ZTK format processor. */
ZTK *zMat3DToZTK(const zMat3D *m, ZTK *ztk)
{
  char buf[BUFSIZ];

  sprintf( buf, "{\n %.10g, %.10g, %.10g\n %.10g, %.10g, %.10g\n %.10g, %.10g, %.10g\n}",
    m->c.xx, m->c.yx, m->c.zx,
    m->c.xy, m->c.yy, m->c.zy,
    m->c.xz, m->c.yz, m->c.zz );
  if( !ZTKAddVal( ztk, buf ) ) return NULL;
  return ztk;
}

/* scan a 3x3 matrix from a file. */
zMat3D *zMat3DFScan(FILE *fp, zMat3D *m)
{
  int i, j;

  for( i=0; i<3; i++ )
    for( j=0; j<3; j++ )
      zFDouble( fp, &m->e[j][i] );
  return m;
}

/* print a 3x3 matrix to a file. */
void zMat3DFPrint(FILE *fp, const zMat3D *m)
{
  if( !m ) return;
  fprintf( fp, "{\n" );
  fprintf( fp, " %.10g, %.10g, %.10g\n", m->c.xx, m->c.yx, m->c.zx );
  fprintf( fp, " %.10g, %.10g, %.10g\n", m->c.xy, m->c.yy, m->c.zy );
  fprintf( fp, " %.10g, %.10g, %.10g\n", m->c.xz, m->c.yz, m->c.zz );
  fprintf( fp, "}\n" );
}

/* read a rotation axis and angle from a ZTK format processor and make a 3x3 matrix. */
zVec3D *zAAFromZTK(zVec3D *aa, ZTK *ztk)
{
  zVec3D axis;
  double angle;

  zVec3DFromZTK( &axis, ztk );
  angle = zDeg2Rad( ZTKDouble( ztk ) );
  if( zVec3DNormalizeDRC( &axis ) > 0 )
    zVec3DMul( &axis, angle, aa );
  else
    zVec3DZero( aa );
  return aa;
}

#ifdef __cplusplus
std::ostream &operator<<(std::ostream &stream, zMat3D &mat){
  stream << "|~ " << mat.c.xx << ", " << mat.c.yx << ", " << mat.c.zx << " ~|" << std::endl;
  stream << "|  " << mat.c.xy << ", " << mat.c.yy << ", " << mat.c.zy << "  |" << std::endl;
  stream << "|_ " << mat.c.xz << ", " << mat.c.yz << ", " << mat.c.zz << " _|" << std::endl;
  return stream;
}
#endif /* __cplusplus */
