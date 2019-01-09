/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs3d - NURBS surface
 */

#include <zeo/zeo_nurbs3d.h>

static void _zNURBS3DKnotInit(zNURBS3D *nurbs, int i);
static int _zNURBS3DSeg(zNURBS3D *nurbs, int i, double t);

static double _zNURBS3DBasis(zNURBS3D *nurbs, int i, double t, int j, int r, int k);
static double _zNURBS3DBasisDiff(zNURBS3D *nurbs, int i, double t, int j, int r, int k);

/* set knots & assign control points & initialize weight uniformly. */
void _zNURBS3DKnotInit(zNURBS3D *nurbs, int i)
{
  register int j, k;

  for( j=0; j<nurbs->dim[i]/2+1; j++ )
    zNURBS3DSetKnot( nurbs, i, j, 0 );
  for( k=0; k<zNURBS3DCPNum(nurbs,i); k++, j++ )
    zNURBS3DSetKnot( nurbs, i, j, zNURBS3DKnot(nurbs,i,j-1) + 1 );
  for( ; j<zNURBS3DKnotNum(nurbs,i); j++ )
    zNURBS3DSetKnot( nurbs, i, j, zNURBS3DKnot(nurbs,i,j-1) );
}

/* allocate a NURBS curve / surface. */
bool zNURBS3DAlloc(zNURBS3D *nurbs, int size1, int size2, int dim1, int dim2)
{
  register int i, j;

  nurbs->dim[0] = dim1;
  nurbs->dim[1] = dim2;
  nurbs->knot[0] = zVecAlloc( size1+dim1+1 );
  nurbs->knot[1] = zVecAlloc( size2+dim2+1 );

  zArray2Alloc( &nurbs->cpnet, zNURBS3DCPCell, size1, size2 );
  if( !nurbs->knot[0] || !nurbs->knot[1] || zArray2Buf(&nurbs->cpnet) == NULL ){
    ZALLOCERROR();
    zNURBS3DDestroy( nurbs );
    return false;
  }
  _zNURBS3DKnotInit( nurbs, 0 );
  _zNURBS3DKnotInit( nurbs, 1 );
  for( i=0; i<size1; i++ )
    for( j=0; j<size2; j++ ){
      zNURBS3DSetWeight( nurbs, i, j, 1.0 );
      zVec3DClear( zNURBS3DCP(nurbs,i,j) );
    }
  return true;
}

/* destroy a NURBS curve / surface. */
void zNURBS3DDestroy(zNURBS3D *nurbs)
{
  nurbs->dim[0] = nurbs->dim[1] = 0;
  zVecFree( nurbs->knot[0] );
  zVecFree( nurbs->knot[1] );
  zArray2Free( &nurbs->cpnet );
}

/* normalize knot vectors of a NURBS curve / surface. */
void zNURBS3DKnotNormalize(zNURBS3D *nurbs)
{
  zVecShift( nurbs->knot[0], -zNURBS3DKnot0(nurbs,0) );
  zVecDivDRC( nurbs->knot[0], zNURBS3DKnotE(nurbs,0) );
  zVecShift( nurbs->knot[1], -zNURBS3DKnot0(nurbs,1) );
  zVecDivDRC( nurbs->knot[1], zNURBS3DKnotE(nurbs,1) );
}

/* find a knot segment that includes the given parameter. */
int _zNURBS3DSeg(zNURBS3D *nurbs, int i, double t)
{
  register int j, k, l;

  if( t < zNURBS3DKnot0(nurbs,i) ) return 0;
  if( t >= zNURBS3DKnotE(nurbs,i) ) return zNURBS3DKnotNum(nurbs,i)-1;
  for( j=0, k=zNURBS3DKnotNum(nurbs,i)-1; ; ){
    while( zNURBS3DKnot(nurbs,i,j+1) == zNURBS3DKnot(nurbs,i,j) && j < k ) j++;
    while( zNURBS3DKnot(nurbs,i,k-1) == zNURBS3DKnot(nurbs,i,k) && k > j ) k--;
    if( ( l = ( j + k ) / 2 ) == j ) break;
    if( zNURBS3DKnot(nurbs,i,l) <= t )
      j = l;
    else
      k = l;
  }
  return j;
}

/* basis function of NURBS. */
double _zNURBS3DBasis(zNURBS3D *nurbs, int i, double t, int j, int r, int k)
{
  double t1, tr1, b=0;

  if( r == 1 )
    return j == k ? 1 : 0;
  if( j > k - r + 1 ){
    t1  = zNURBS3DKnot(nurbs,i,j);
    tr1 = zNURBS3DKnot(nurbs,i,j+r-1);
    if( !zIsTiny( tr1 - t1 ) )
      b += ( t - t1 ) / ( tr1 - t1 ) * _zNURBS3DBasis(nurbs,i,t,j,r-1,k);
  }
  if( j <= k ){
    t1  = zNURBS3DKnot(nurbs,i,j+1);
    tr1 = zNURBS3DKnot(nurbs,i,j+r);
    if( !zIsTiny( tr1 - t1 ) )
      b += ( tr1 - t ) / ( tr1 - t1 ) * _zNURBS3DBasis(nurbs,i,t,j+1,r-1,k);
  }
  return b;
}

/* compute a vector on a NURBS curve / surface. */
zVec3D *zNURBS3DVec(zNURBS3D *nurbs, double u, double v, zVec3D *p)
{
  register int su, sv, eu, ev, i, j;
  double bu, bv, dv, den;
  zVec3D tmp;

  su = _zNURBS3DSeg( nurbs, 0, u );
  sv = _zNURBS3DSeg( nurbs, 1, v );
  eu = zMin( su+1, zNURBS3DCPNum(nurbs,0) );
  ev = zMin( sv+1, zNURBS3DCPNum(nurbs,1) );
  zVec3DClear( p );
  for( den=0, i=zMax(su-nurbs->dim[0],0); i<eu; i++ ){
    bu = _zNURBS3DBasis(nurbs,0,u,i,nurbs->dim[0]+1,su);
    zVec3DClear( &tmp );
    for( dv=0, j=zMax(sv-nurbs->dim[1],0); j<ev; j++ ){
      dv += bv = zNURBS3DWeight(nurbs,i,j) * _zNURBS3DBasis(nurbs,1,v,j,nurbs->dim[1]+1,sv);
      zVec3DCatDRC( &tmp, bv, zNURBS3DCP(nurbs,i,j) );
    }
    zVec3DCatDRC( p, bu, &tmp );
    den += bu * dv;
  }
  return zIsTiny(den) ? NULL : zVec3DDivDRC( p, den );
}

/* derivative of the basis function of NURBS. */
double _zNURBS3DBasisDiff(zNURBS3D *nurbs, int i, double t, int j, int r, int k)
{
  double dt, b = 0;

  if( j > k - r + 1 && !zIsTiny( ( dt = zNURBS3DKnot(nurbs,i,j+r-1) - zNURBS3DKnot(nurbs,i,j) ) ) )
    b += _zNURBS3DBasis(nurbs,i,t,j,r-1,k) / dt;
  if( j <= k && !zIsTiny( ( dt = zNURBS3DKnot(nurbs,i,j+r) - zNURBS3DKnot(nurbs,i,j+1) ) ) )
    b -= _zNURBS3DBasis(nurbs,i,t,j+1,r-1,k) / dt;
  return b * ( r - 1 );
}

/* compute a position and normal vectors on NURBS surface. */
zVec3D *zNURBS3DVecNorm(zNURBS3D *nurbs, double u, double v, zVec3D *p, zVec3D *n, zVec3D *t1, zVec3D *t2)
{
  register int su, sv, eu, ev, i, j;
  double bu, bv, dbu, dbv, dv, ddv, dubv, budv, den;
  zVec3D *tu, *tv, tu_tmp, tv_tmp, pv_tmp, dpv_tmp;

  tu = t1 ? t1 : &tu_tmp;
  tv = t2 ? t2 : &tv_tmp;
  su = _zNURBS3DSeg( nurbs, 0, u );
  sv = _zNURBS3DSeg( nurbs, 1, v );
  eu = zMin( su+1, zNURBS3DCPNum(nurbs,0) );
  ev = zMin( sv+1, zNURBS3DCPNum(nurbs,1) );
  zVec3DClear( p );
  zVec3DClear( tu );
  zVec3DClear( tv );
  for( den=dubv=budv=0, i=zMax(su-nurbs->dim[0],0); i<eu; i++ ){
    bu = _zNURBS3DBasis(nurbs,0,u,i,nurbs->dim[0]+1,su);
    dbu = _zNURBS3DBasisDiff(nurbs,0,u,i,nurbs->dim[0]+1,su);
    zVec3DClear( &pv_tmp );
    zVec3DClear( &dpv_tmp );
    for( dv=ddv=0, j=zMax(sv-nurbs->dim[1],0); j<ev; j++ ){
      dv += bv = zNURBS3DWeight(nurbs,i,j) * _zNURBS3DBasis(nurbs,1,v,j,nurbs->dim[1]+1,sv);
      ddv += dbv = zNURBS3DWeight(nurbs,i,j) * _zNURBS3DBasisDiff(nurbs,1,v,j,nurbs->dim[1]+1,sv);
      zVec3DCatDRC( &pv_tmp, bv, zNURBS3DCP(nurbs,i,j) );
      zVec3DCatDRC( &dpv_tmp, dbv, zNURBS3DCP(nurbs,i,j) );
    }
    zVec3DCatDRC( p, bu, &pv_tmp );
    zVec3DCatDRC( tu, dbu, &pv_tmp );
    zVec3DCatDRC( tv, bu, &dpv_tmp );
    dubv += dbu * dv;
    budv += bu * ddv;
    den += bu * dv;
  }
  if( zIsTiny(den) ) return NULL;
  zVec3DDivDRC( p, den );
  zVec3DCatDRC( tu, -dubv, p );
  zVec3DCatDRC( tv, -budv, p );
  if( n ){
    zVec3DOuterProd( tu, tv, n );
    if( zVec3DIsTiny( n ) )
      zVec3DClear( n );
    else
      zVec3DNormalizeDRC( n );
  }
  if( t1 && !zVec3DIsTiny( t1 ) ) zVec3DNormalizeDRC( t1 );
  if( t2 && !zVec3DIsTiny( t2 ) ) zVec3DNormalizeDRC( t2 );
  return p;
}

#define ZNURBS3D_VEC_NN_GAIN 0.1
/* nearest neighbor of a 3D point on a NURBS surface. */
double zNURBS3DVecNN(zNURBS3D *nurbs, zVec3D *p, zVec3D *nn, double *u, double *v)
{
  double utmp, vtmp, u_prev, v_prev, duu, duv, dvv, eu, ev, eval, eval_prev, gain;
  zVec3D du, dv, e;
  register int i;
  int iter = 0;

  if( !u ) u = &utmp;
  if( !v ) v = &vtmp;
  *u = u_prev = 0.5 * ( zNURBS3DKnot0(nurbs,0) + zNURBS3DKnotE(nurbs,0) );
  *v = v_prev = 0.5 * ( zNURBS3DKnot0(nurbs,1) + zNURBS3DKnotE(nurbs,1) );
  ZITERINIT( iter );
  for( eval_prev=HUGE_VAL, i=0; i<iter; i++ ){
    zNURBS3DVecNorm( nurbs, *u, *v, nn, NULL, &du, &dv );
    zVec3DSub( p, nn, &e );
    eval = zVec3DNorm(&e);
    duu = zVec3DInnerProd( &du, &du );
    duv = zVec3DInnerProd( &du, &dv );
    dvv = zVec3DInnerProd( &dv, &dv );
    eu = zVec3DInnerProd( &du, &e );
    ev = zVec3DInnerProd( &dv, &e );
    gain = ZNURBS3D_VEC_NN_GAIN / ( duu * dvv - duv * duv );
    *u += ( ( dvv + eval ) * eu - duv * ev ) * gain;
    *v += ( ( duu + eval ) * ev - duv * eu ) * gain;
    *u = zLimit( *u, zNURBS3DKnot0(nurbs,0)+zTOL, zNURBS3DKnotE(nurbs,0)-zTOL );
    *v = zLimit( *v, zNURBS3DKnot0(nurbs,1)+zTOL, zNURBS3DKnotE(nurbs,1)-zTOL );
    if( ( zIsTiny( *u - u_prev ) && zIsTiny( *v - v_prev ) ) || zIsTiny( eval - eval_prev ) ) break;
    u_prev = *u;
    v_prev = *v;
    eval_prev = eval;
  }
  return zVec3DDist( p, nn );
}

/* for debug */

/* output knots of a NURBS curve. */
void zNURBS3DKnotFWrite(FILE *fp, zNURBS3D *nurbs)
{
  register int i;

  fprintf( fp, "[" );
  for( i=0; i<zNURBS3DKnotNum(nurbs,0); i++ ){
    fprintf( fp, " %g", zNURBS3DKnot(nurbs,0,i) );
  }
  fprintf( fp, " ]\n" );
  fprintf( fp, "[" );
  for( i=0; i<zNURBS3DKnotNum(nurbs,1); i++ ){
    fprintf( fp, " %g", zNURBS3DKnot(nurbs,1,i) );
  }
  fprintf( fp, " ]\n" );
}

/* outpu control points of a NURBS curve. */
void zNURBS3DCPFWrite(FILE *fp, zNURBS3D *nurbs)
{
  register int i, j;

  for( i=0; i<zNURBS3DCPNum(nurbs,0); i++ )
    for( j=0; j<zNURBS3DCPNum(nurbs,1); j++ ){
      fprintf( fp, "[%03d,%03d] (%g) ", i, j, zNURBS3DWeight(nurbs,i,j) );
      zVec3DFWrite( fp, zNURBS3DCP(nurbs,i,j) );
    }
}
