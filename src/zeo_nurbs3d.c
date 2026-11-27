/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs3d - NURBS curve / surface.
 */

#include <zeo/zeo_nurbs3d.h>

/* allocate a NURBS curve / surface. */
bool zNURBS3DAlloc(zNURBS3D *nurbs, int size1, int size2, int order1, int order2)
{
  int i, j;

  if( size1 <= order1 || size2 <= order2 ){
    ZRUNERROR( ZEO_ERR_NURBS_INVALID_ORDER );
    return false;
  }
  if( !zBSplineParamAlloc( &nurbs->param[0], order1, size1, 0 ) ||
      !zBSplineParamAlloc( &nurbs->param[1], order2, size2, 0 ) ){
    zBSplineParamFree( &nurbs->param[0] );
    zBSplineParamFree( &nurbs->param[1] );
    return false;
  }
  zArray2Alloc( &nurbs->cpnet, zNURBS3DCPCell, size1, size2 );
  if( !zArray2Buf(&nurbs->cpnet) ){
    ZALLOCERROR();
    zNURBS3DDestroy( nurbs );
    return false;
  }
  zBSplineParamKnotInit( &nurbs->param[0] );
  zBSplineParamKnotInit( &nurbs->param[1] );
  for( i=0; i<size1; i++ )
    for( j=0; j<size2; j++ ){
      zNURBS3DSetWeight( nurbs, i, j, 1.0 );
      zVec3DZero( zNURBS3DCP(nurbs,i,j) );
    }
  return true;
}

/* initialize a NURBS curve / surface. */
zNURBS3D *zNURBS3DInit(zNURBS3D *nurbs)
{
  zBSplineParamInit( &nurbs->param[0] );
  zBSplineParamInit( &nurbs->param[1] );
  zArray2Init( &nurbs->cpnet );
  return nurbs;
}

/* destroy a NURBS curve / surface. */
void zNURBS3DDestroy(zNURBS3D *nurbs)
{
  zBSplineParamFree( &nurbs->param[0] );
  zBSplineParamFree( &nurbs->param[1] );
  zArray2Free( &nurbs->cpnet );
  zNURBS3DInit( nurbs );
}

/* copy a NURBS curve / surface. */
zNURBS3D *zNURBS3DCopy(const zNURBS3D *src, zNURBS3D *dest)
{
  if( !zBSplineParamCopy( &src->param[0], &dest->param[0] ) ||
      !zBSplineParamCopy( &src->param[1], &dest->param[1] ) ){
    return NULL;
  }
  if( zNURBS3DCPNum(src,0) != zNURBS3DCPNum(dest,0) ||
      zNURBS3DCPNum(src,1) != zNURBS3DCPNum(dest,1) ){
    ZRUNERROR( ZEO_ERR_NURBS_SIZEMISMATCH );
    return NULL;
  }
  memcpy( (void *)zArray2Buf(&dest->cpnet), (void *)zArray2Buf(&src->cpnet), zNURBS3DCPNum(src,0)*zNURBS3DCPNum(src,1)*sizeof(zNURBS3DCPCell) );
  return dest;
}

/* clone a NURBS curve / surface. */
zNURBS3D *zNURBS3DClone(const zNURBS3D *src, zNURBS3D *dest)
{
  if( !zNURBS3DAlloc( dest, zNURBS3DCPNum(src,0), zNURBS3DCPNum(src,1), zNURBS3DOrder(src,0), zNURBS3DOrder(src,1) ) )
    return NULL;
  return zNURBS3DCopy( src, dest );
}

/* mirror a NURBS curve / surface about specified axis. */
zNURBS3D *zNURBS3DMirror(const zNURBS3D *src, zNURBS3D *dest, zAxis axis)
{
  int i, j;

  if( !zNURBS3DCopy( src, dest ) ) return NULL;
  for( i=0; i<zNURBS3DCPNum(dest,0); i++ )
    for( j=0; j<zNURBS3DCPNum(dest,1); j++ )
      zNURBS3DCP(dest,i,j)->e[(int)axis] *= -1;
  return dest;
}

/* transform control points of a NURBS curve / surface. */
zNURBS3D *zNURBS3DXform(const zNURBS3D *src, const zFrame3D *f, zNURBS3D *dest)
{
  int i, j;

  if( !zNURBS3DCopy( src, dest ) ) return NULL;
  for( i=0; i<zNURBS3DCPNum(dest,0); i++ )
    for( j=0; j<zNURBS3DCPNum(dest,1); j++ )
      zXform3D( f, zNURBS3DCP(src,i,j), zNURBS3DCP(dest,i,j) );
  return dest;
}

/* inversely transform control points of a NURBS curve / surface. */
zNURBS3D *zNURBS3DXformInv(const zNURBS3D *src, const zFrame3D *f, zNURBS3D *dest)
{
  int i, j;

  if( !zNURBS3DCopy( src, dest ) ) return NULL;
  for( i=0; i<zNURBS3DCPNum(dest,0); i++ )
    for( j=0; j<zNURBS3DCPNum(dest,1); j++ )
      zXform3DInv( f, zNURBS3DCP(src,i,j), zNURBS3DCP(dest,i,j) );
  return dest;
}

/* compute a vector on a NURBS curve / surface. */
zVec3D *zNURBS3DVec(const zNURBS3D *nurbs, double u, double v, zVec3D *p)
{
  int su, sv, i, j;
  double bu, bv, dv, den;
  zVec3D tmp;

  su = zBSplineParamSeg( &nurbs->param[0], u );
  sv = zBSplineParamSeg( &nurbs->param[1], v );
  zVec3DZero( p );
  for( den=0, i=su-zNURBS3DOrder(nurbs,0); i<=su; i++ ){
    bu = zBSplineParamBasis( &nurbs->param[0], u, i, zNURBS3DOrder(nurbs,0), su );
    zVec3DZero( &tmp );
    for( dv=0, j=sv-zNURBS3DOrder(nurbs,1); j<=sv; j++ ){
      dv += bv = zNURBS3DWeight(nurbs,i,j) * zBSplineParamBasis( &nurbs->param[1], v, j, zNURBS3DOrder(nurbs,1), sv );
      zVec3DCatDRC( &tmp, bv, zNURBS3DCP(nurbs,i,j) );
    }
    zVec3DCatDRC( p, bu, &tmp );
    den += bu * dv;
  }
  return zIsTiny(den) ? NULL : zVec3DDivDRC( p, den );
}

/* a 3D vector on a NURBS surface and the derivatives of it with respect to the parameters. */
static zVec3D *_zNURBS3DVecDiff(const zNURBS3D *nurbs, double u, double v, zVec3D *p, zVec3D *tu, zVec3D *tv)
{
  int su, sv, i, j;
  double bu, bv, dbu, dbv, dv, ddv, dubv, budv, den;
  zVec3D pv_tmp, dpv_tmp;

  su = zBSplineParamSeg( &nurbs->param[0], u );
  sv = zBSplineParamSeg( &nurbs->param[1], v );
  zVec3DZero( p );
  zVec3DZero( tu );
  zVec3DZero( tv );
  for( den=dubv=budv=0, i=su-zNURBS3DOrder(nurbs,0); i<=su; i++ ){
    bu = zBSplineParamBasis( &nurbs->param[0], u, i, zNURBS3DOrder(nurbs,0), su );
    dbu = zBSplineParamBasisDiff( &nurbs->param[0], u, i, zNURBS3DOrder(nurbs,0), su, 1 );
    zVec3DZero( &pv_tmp );
    zVec3DZero( &dpv_tmp );
    for( dv=ddv=0, j=sv-zNURBS3DOrder(nurbs,1); j<=sv; j++ ){
      dv += bv = zNURBS3DWeight(nurbs,i,j) * zBSplineParamBasis( &nurbs->param[1], v, j, zNURBS3DOrder(nurbs,1), sv );
      ddv += dbv = zNURBS3DWeight(nurbs,i,j) * zBSplineParamBasisDiff( &nurbs->param[1], v, j, zNURBS3DOrder(nurbs,1), sv, 1 );
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
  zVec3DDivDRC( tu, den );
  zVec3DDivDRC( tv, den );
  return p;
}

/* a 3D vector on a NURBS surface and the normal and tangential vectors at it. */
zVec3D *zNURBS3DVecTSpace(const zNURBS3D *nurbs, double u, double v, zVec3D *p, zVec3D *n, zVec3D *t1, zVec3D *t2)
{
  zVec3D *tu, *tv, tu_tmp, tv_tmp;

  tu = t1 ? t1 : &tu_tmp;
  tv = t2 ? t2 : &tv_tmp;
  if( !_zNURBS3DVecDiff( nurbs, u, v, p, tu, tv ) ) return NULL;
  if( n ){
    zVec3DOuterProd( tu, tv, n );
    if( zVec3DIsTiny( n ) )
      zVec3DZero( n );
    else
      zVec3DNormalizeDRC( n, 0 );
  }
  if( t1 && !zVec3DIsTiny( t1 ) ) zVec3DNormalizeDRC( t1, 0 );
  if( t2 && !zVec3DIsTiny( t2 ) ) zVec3DNormalizeDRC( t2, 0 );
  return p;
}

/* number of samples of a parameter around a pole. */
#define ZNURBS3D_NORM_POLE_SAMPLE 16
/* number of samples of a parameter per control point to find the initial guess of the closest point. */
#define ZNURBS3D_CLOSEST_SAMPLE 2
/* maximum number of iterations to find the closest point. */
#define ZNURBS3D_CLOSEST_MAX_ITER 100

/* number of initial guesses of the closest point. */
#define ZNURBS3D_CLOSEST_INIT_NUM 4

/* the initial guesses of the parameters of the closest point of a 3D point on a NURBS surface, which
 * are the closest ones among sampled points on the surface, in order to avoid local minima. Only the
 * closest one is taken in each patch divided by the knots, so that the initial guesses are on
 * different patches, e.g., different faces of a box. Samples at the same position as one of the
 * initial guesses, e.g., a pole shared by patches, are skipped. It returns the number of the initial
 * guesses. */
static int _zNURBS3DClosestInit(const zNURBS3D *nurbs, const zVec3D *p, double u[], double v[])
{
  double us, vs, d, dmin[ZNURBS3D_CLOSEST_INIT_NUM];
  int i, j, k, l, n = 0, nu, nv, patch, pid[ZNURBS3D_CLOSEST_INIT_NUM];
  zVec3D q, qi[ZNURBS3D_CLOSEST_INIT_NUM];

  nu = ZNURBS3D_CLOSEST_SAMPLE * zNURBS3DCPNum(nurbs,0);
  nv = ZNURBS3D_CLOSEST_SAMPLE * zNURBS3DCPNum(nurbs,1);
  for( i=0; i<=nu; i++ ){
    us = zNURBS3DKnotS(nurbs,0) + ( zNURBS3DKnotE(nurbs,0) - zNURBS3DKnotS(nurbs,0) ) * i / nu;
    for( j=0; j<=nv; j++ ){
      vs = zNURBS3DKnotS(nurbs,1) + ( zNURBS3DKnotE(nurbs,1) - zNURBS3DKnotS(nurbs,1) ) * j / nv;
      if( !zNURBS3DVec( nurbs, us, vs, &q ) ) continue;
      d = zVec3DSqrDist( p, &q );
      patch = zBSplineParamSeg( &nurbs->param[0], us ) * zNURBS3DKnotNum(nurbs,1) + zBSplineParamSeg( &nurbs->param[1], vs );
      for( l=0; l<n; l++ ) /* a sample at the same position */
        if( zVec3DEqual( &q, &qi[l] ) ) break;
      if( l < n ) continue;
      for( l=0; l<n; l++ ) /* a sample in the same patch */
        if( pid[l] == patch ) break;
      if( l < n ){
        if( d >= dmin[l] ) continue;
        for( k=l; k<n-1; k++ ){ /* remove the old one */
          dmin[k] = dmin[k+1]; u[k] = u[k+1]; v[k] = v[k+1]; pid[k] = pid[k+1]; qi[k] = qi[k+1];
        }
        n--;
      }
      /* insert into the sorted list */
      for( k=n; k>0 && d<dmin[k-1]; k-- )
        if( k < ZNURBS3D_CLOSEST_INIT_NUM ){
          dmin[k] = dmin[k-1]; u[k] = u[k-1]; v[k] = v[k-1]; pid[k] = pid[k-1]; qi[k] = qi[k-1];
        }
      if( k < ZNURBS3D_CLOSEST_INIT_NUM ){
        dmin[k] = d; u[k] = us; v[k] = vs; pid[k] = patch; qi[k] = q;
        if( n < ZNURBS3D_CLOSEST_INIT_NUM ) n++;
      }
    }
  }
  return n;
}

/* the Hessian matrix of the half of the squared distance |p - s(u,v)|^2 with respect to the parameters,
 * where e = p - s, and su and sv are the derivatives of s. The second derivatives of s are computed by
 * the central differences of the first derivatives. */
static void _zNURBS3DClosestHess(const zNURBS3D *nurbs, double u, double v, const zVec3D *e, const zVec3D *su, const zVec3D *sv, double h[])
{
  double hu, hv, u1, u2, v1, v2;
  zVec3D s, su1, sv1, su2, sv2, d;

  hu = 1.0e-6 * ( zNURBS3DKnotE(nurbs,0) - zNURBS3DKnotS(nurbs,0) );
  hv = 1.0e-6 * ( zNURBS3DKnotE(nurbs,1) - zNURBS3DKnotS(nurbs,1) );
  h[0] = zVec3DSqrNorm( su );
  h[1] = zVec3DInnerProd( su, sv );
  h[2] = zVec3DSqrNorm( sv );
  u1 = _zMax( u - hu, zNURBS3DKnotS(nurbs,0) ); u2 = _zMin( u + hu, zNURBS3DKnotE(nurbs,0) );
  v1 = _zMax( v - hv, zNURBS3DKnotS(nurbs,1) ); v2 = _zMin( v + hv, zNURBS3DKnotE(nurbs,1) );
  if( _zNURBS3DVecDiff( nurbs, u1, v, &s, &su1, &sv1 ) && _zNURBS3DVecDiff( nurbs, u2, v, &s, &su2, &sv2 ) ){
    zVec3DSub( &su2, &su1, &d ); /* s_uu */
    h[0] -= zVec3DInnerProd( e, &d ) / ( u2 - u1 );
    zVec3DSub( &sv2, &sv1, &d ); /* s_uv */
    h[1] -= 0.5 * zVec3DInnerProd( e, &d ) / ( u2 - u1 );
  }
  if( _zNURBS3DVecDiff( nurbs, u, v1, &s, &su1, &sv1 ) && _zNURBS3DVecDiff( nurbs, u, v2, &s, &su2, &sv2 ) ){
    zVec3DSub( &sv2, &sv1, &d ); /* s_vv */
    h[2] -= zVec3DInnerProd( e, &d ) / ( v2 - v1 );
    zVec3DSub( &su2, &su1, &d ); /* s_uv */
    h[1] -= 0.5 * zVec3DInnerProd( e, &d ) / ( v2 - v1 );
  }
}

/* the descent of the closest point of a 3D point on a NURBS surface from an initial guess.
 * The squared distance f(u,v) = |p - s(u,v)|^2 is minimized by the Newton method with the
 * Levenberg-Marquardt damping from the initial guess, where s(u,v) is the point on the surface.
 * The step d of the parameters is the solution of ( H + lambda m I ) d = J^T ( p - s ), where
 * J = [ s_u s_v ], H is the Hessian matrix of f/2, and m is the mean of the diagonal elements of J^T J,
 * so that the degenerate case where s_u or s_v vanishes at a pole is also treated. The damping
 * factor lambda is increased until H + lambda m I is positive definite and the step decreases f.
 * At a pole, where the derivative with respect to a parameter vanishes, the iteration cannot choose
 * the direction to leave the pole. In this case, the parameter is sampled over the whole domain at the
 * other parameter slightly moved from the pole, and the closest one is taken.
 * The parameters are clamped in the domain. A step is accepted only if it decreases f.
 * Since the derivatives are discontinuous on knot lines, along which the surface may have creases,
 * the iteration can stall or zigzag on a knot line. Hence, steps of either of the parameters, which
 * move the point along the knot line, are also tried, and the best one is taken. Then, if no step
 * decreases f, steps in the positive and negative directions
 * of the parameters with decreasing sizes, since the descent direction computed from the derivatives
 * on one side of a knot line may be wrong on the other side. */
static double _zNURBS3DClosestDescent(const zNURBS3D *nurbs, const zVec3D *p, double *u, double *v)
{
  double un = 0, vn = 0, uk, vk, fk, a, b, c, gu, gv, m, det = 0, du, dv, f, fn, lambda, lambda0[3] = { 1.0e-3, 1.0e-3, 1.0e-3 }, h, uw, vw, hess[3];
  zVec3D s, su, sv, e, sn;
  int iter, k, i;
  bool accepted;

  uw = zNURBS3DKnotE(nurbs,0) - zNURBS3DKnotS(nurbs,0);
  vw = zNURBS3DKnotE(nurbs,1) - zNURBS3DKnotS(nurbs,1);
  if( !_zNURBS3DVecDiff( nurbs, *u, *v, &s, &su, &sv ) ) return HUGE_VAL;
  zVec3DSub( p, &s, &e );
  f = zVec3DSqrNorm( &e );
  for( iter=0; iter<ZNURBS3D_CLOSEST_MAX_ITER; iter++ ){
    _zNURBS3DClosestHess( nurbs, *u, *v, &e, &su, &sv, hess );
    a = hess[0];
    b = hess[1];
    c = hess[2];
    gu = zVec3DInnerProd( &su, &e );
    gv = zVec3DInnerProd( &sv, &e );
    if( ( m = 0.5 * ( zVec3DSqrNorm( &su ) + zVec3DSqrNorm( &sv ) ) ) == 0 ) break;
    /* the best of the three kinds of steps: k=0: both, k=1: u only, k=2: v only */
    for( accepted=false, fn=f, k=0; k<3; k++ )
      for( lambda=lambda0[k]; lambda<1.0e10; lambda*=10 ){
        if( ( k != 2 && a + lambda*m <= 0 ) || ( k != 1 && c + lambda*m <= 0 ) ||
            ( k == 0 && ( det = ( a + lambda*m ) * ( c + lambda*m ) - b*b ) <= 0 ) ) continue; /* not positive definite */
        du = k == 2 ? 0 : k == 1 ? gu / ( a + lambda*m ) : ( ( c + lambda*m ) * gu - b * gv ) / det;
        dv = k == 1 ? 0 : k == 2 ? gv / ( c + lambda*m ) : ( ( a + lambda*m ) * gv - b * gu ) / det;
        uk = zLimit( *u + du, zNURBS3DKnotS(nurbs,0), zNURBS3DKnotE(nurbs,0) );
        vk = zLimit( *v + dv, zNURBS3DKnotS(nurbs,1), zNURBS3DKnotE(nurbs,1) );
        if( !zNURBS3DVec( nurbs, uk, vk, &sn ) ) continue;
        if( ( fk = zVec3DSqrDist( p, &sn ) ) < f ){
          lambda0[k] = _zMax( lambda * 0.1, 1.0e-10 ); /* the damping factor of each kind of steps */
          if( fk < fn ){
            accepted = true;
            un = uk; vn = vk; fn = fk;
          }
          break;
        }
      }
    if( !accepted ) /* leaving a pole */
      for( k=0; k<2 && !accepted; k++ ){
        if( zVec3DNorm( k == 0 ? &su : &sv ) > zTOL * zVec3DNorm( k == 0 ? &sv : &su ) ) continue;
        for( h=1.0e-6; h<1 && !accepted; h*=10 )
          for( i=0; i<ZNURBS3D_NORM_POLE_SAMPLE; i++ ){
            un = k == 0 ? zNURBS3DKnotS(nurbs,0) + uw * ( i + 0.5 ) / ZNURBS3D_NORM_POLE_SAMPLE :
              *u + h * ( 0.5 * ( zNURBS3DKnotS(nurbs,0) + zNURBS3DKnotE(nurbs,0) ) - *u );
            vn = k == 1 ? zNURBS3DKnotS(nurbs,1) + vw * ( i + 0.5 ) / ZNURBS3D_NORM_POLE_SAMPLE :
              *v + h * ( 0.5 * ( zNURBS3DKnotS(nurbs,1) + zNURBS3DKnotE(nurbs,1) ) - *v );
            if( zNURBS3DVec( nurbs, un, vn, &sn ) && ( fn = zVec3DSqrDist( p, &sn ) ) < f ){
              accepted = true;
              break;
            }
          }
      }
    if( !accepted ){ /* pattern search */
      for( h=1.0e-2; h>zTOL && !accepted; h*=0.1 )
        for( k=0; k<4; k++ ){
          un = zLimit( *u + ( k == 0 ? h : k == 1 ? -h : 0 ) * uw, zNURBS3DKnotS(nurbs,0), zNURBS3DKnotE(nurbs,0) );
          vn = zLimit( *v + ( k == 2 ? h : k == 3 ? -h : 0 ) * vw, zNURBS3DKnotS(nurbs,1), zNURBS3DKnotE(nurbs,1) );
          if( zNURBS3DVec( nurbs, un, vn, &sn ) && ( fn = zVec3DSqrDist( p, &sn ) ) < f ){
            accepted = true;
            break;
          }
        }
    }
    if( !accepted ) break; /* no step decreases the distance */
    *u = un;
    *v = vn;
    f = fn;
    if( !_zNURBS3DVecDiff( nurbs, *u, *v, &s, &su, &sv ) ) break;
    zVec3DSub( p, &s, &e );
    /* converged if the error vector is perpendicular to the both tangent vectors */
    if( zSqr( zVec3DInnerProd( &su, &e ) ) <= zTOL * zTOL * zVec3DSqrNorm( &su ) * f &&
        zSqr( zVec3DInnerProd( &sv, &e ) ) <= zTOL * zTOL * zVec3DSqrNorm( &sv ) * f ) break;
  }
  return f;
}

/* closest point of a 3D point on a NURBS surface, which is the best of those found by the descent
 * from the initial guesses. */
double zNURBS3DClosest(const zNURBS3D *nurbs, const zVec3D *p, zVec3D *nn, double *u, double *v)
{
  double utmp, vtmp, ui[ZNURBS3D_CLOSEST_INIT_NUM], vi[ZNURBS3D_CLOSEST_INIT_NUM], f, fmin = HUGE_VAL;
  int i, n;

  if( !u ) u = &utmp;
  if( !v ) v = &vtmp;
  *u = 0.5 * ( zNURBS3DKnotS(nurbs,0) + zNURBS3DKnotE(nurbs,0) );
  *v = 0.5 * ( zNURBS3DKnotS(nurbs,1) + zNURBS3DKnotE(nurbs,1) );
  n = _zNURBS3DClosestInit( nurbs, p, ui, vi );
  for( i=0; i<n; i++ )
    if( ( f = _zNURBS3DClosestDescent( nurbs, p, &ui[i], &vi[i] ) ) < fmin ){
      fmin = f;
      *u = ui[i];
      *v = vi[i];
    }
  if( !zNURBS3DVec( nurbs, *u, *v, nn ) ) return HUGE_VAL;
  return zVec3DDist( p, nn );
}

/* the normal vector of a NURBS surface around given parameters. It is the sum of the normal vectors
 * at four parameters slightly apart from the given ones, which coincides with the normal vector on a
 * smooth part, and bisects the normal vectors of the both sides on a crease along a knot line.
 * At a pole, where the derivative of the position with respect to a parameter vanishes, normal
 * vectors are defined in all directions around it. In this case, the sum of the normal vectors at
 * the parameters slightly moved from the pole along the other parameter, which are sampled over the
 * whole domain of the former parameter, is taken. */
static zVec3D *_zNURBS3DNorm(const zNURBS3D *nurbs, double u, double v, zVec3D *n)
{
  double ws[2], wc[2], d[2], q[2];
  zVec3D p, ni, t[2];
  int i, k;

  for( k=0; k<2; k++ ){
    ws[k] = zNURBS3DKnotE(nurbs,k) - zNURBS3DKnotS(nurbs,k);
    wc[k] = 0.5 * ( zNURBS3DKnotS(nurbs,k) + zNURBS3DKnotE(nurbs,k) );
    d[k] = 1.0e-8 * ws[k];
  }
  zVec3DZero( n );
  if( !_zNURBS3DVecDiff( nurbs, u, v, &p, &t[0], &t[1] ) ) return n;
  for( k=0; k<2; k++ ) /* a pole where the derivative with respect to the k-th parameter vanishes */
    if( zVec3DNorm( &t[k] ) <= zTOL * zVec3DNorm( &t[1-k] ) ){
      q[1-k] = ( k == 0 ? v : u ) + 1.0e-6 * ( wc[1-k] - ( k == 0 ? v : u ) );
      for( i=0; i<ZNURBS3D_NORM_POLE_SAMPLE; i++ ){
        q[k] = zNURBS3DKnotS(nurbs,k) + ws[k] * ( i + 0.5 ) / ZNURBS3D_NORM_POLE_SAMPLE;
        if( zNURBS3DVecNorm( nurbs, q[0], q[1], &p, &ni ) ) zVec3DAddDRC( n, &ni );
      }
      return n;
    }
  for( i=0; i<4; i++ )
    if( zNURBS3DVecNorm( nurbs,
          zLimit( u + ( i & 1 ? d[0] : -d[0] ), zNURBS3DKnotS(nurbs,0), zNURBS3DKnotE(nurbs,0) ),
          zLimit( v + ( i & 2 ? d[1] : -d[1] ), zNURBS3DKnotS(nurbs,1), zNURBS3DKnotE(nurbs,1) ), &p, &ni ) )
      zVec3DAddDRC( n, &ni );
  return n;
}

/* distance from a 3D point to a NURBS surface. */
double zNURBS3DDistFromPoint(const zNURBS3D *nurbs, const zVec3D *p)
{
  zVec3D cp;
  return zNURBS3DClosest( nurbs, p, &cp, NULL, NULL );
}

/* signed distance from a 3D point to a NURBS surface. */
double zNURBS3DSignedDistFromPoint(const zNURBS3D *nurbs, const zVec3D *p)
{
  zVec3D cp, n, e;
  double u, v, d;

  d = zNURBS3DClosest( nurbs, p, &cp, &u, &v );
  _zNURBS3DNorm( nurbs, u, v, &n );
  zVec3DSub( p, &cp, &e );
  return zVec3DInnerProd( &e, &n ) < 0 ? -d : d;
}

/* check if a 3D point is inside of a NURBS surface. */
bool zNURBS3DPointIsInside(const zNURBS3D *nurbs, const zVec3D *p, double margin)
{
  return zNURBS3DSignedDistFromPoint( nurbs, p ) < margin;
}

/* convert NURBS surface to a polyhedron. */
zPH3D *zNURBS3DToPH(const zNURBS3D *nurbs, zPH3D *ph)
{
  zVec3D *vert, *vert2;
  zTri3D *face;
  int i, j;
  double u, v;

  if( !zPH3DAlloc( ph,
      ( zNURBS3DSlice(nurbs,0)+1 ) * ( zNURBS3DSlice(nurbs,1)+1 ), zNURBS3DSlice(nurbs,0) * zNURBS3DSlice(nurbs,1)*2 ) )
    return NULL;

  vert = zPH3DVertBuf( ph );
  for( i=0; i<=zNURBS3DSlice(nurbs,0); i++ ){
    u = zNURBS3DKnotSlice( nurbs, 0, i );
    for( j=0; j<=zNURBS3DSlice(nurbs,1); j++ ){
      v = zNURBS3DKnotSlice( nurbs, 1, j );
      if( zNURBS3DVec( nurbs, u, v, vert ) ) vert++;
    }
  }
  face = zPH3DFaceBuf( ph );
  vert = zPH3DVertBuf( ph );
  vert2 = vert + zNURBS3DSlice(nurbs,1) + 1;
  for( i=0; i<zNURBS3DSlice(nurbs,0); i++, vert+=zNURBS3DSlice(nurbs,1)+1, vert2+=zNURBS3DSlice(nurbs,1)+1 )
    for( j=0; j<zNURBS3DSlice(nurbs,1); j++ ){
      zTri3DCreate( face++, &vert[j], &vert2[j], &vert2[j+1] );
      zTri3DCreate( face++, &vert[j], &vert2[j+1], &vert[j+1] );
    }
  return ph;
}

/* parse ZTK format */

static zVec _zNURBS3DKnotFromZTK(zNURBS3D *nurbs, int id, ZTK *ztk)
{
  if( nurbs->param[id].knot ){
    ZRUNWARN( ZEO_ERR_NURBS_KNOTALREADY );
    zVecFree( nurbs->param[id].knot );
  }
  return ( nurbs->param[id].knot = zVecFromZTK( ztk ) );
}

static void *_zNURBS3DUKnotFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return _zNURBS3DKnotFromZTK( (zNURBS3D *)obj, 0, ztk ) ? obj : NULL;
}
static void *_zNURBS3DVKnotFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return _zNURBS3DKnotFromZTK( (zNURBS3D *)obj, 1, ztk ) ? obj : NULL;
}
static void *_zNURBS3DSliceFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNURBS3DSlice((zNURBS3D*)obj,0) = ZTKInt( ztk );
  zNURBS3DSlice((zNURBS3D*)obj,1) = ZTKInt( ztk );
  return obj;
}
static void *_zNURBS3DSizeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  int size1, size2;
  if( zNURBS3DCPNum((zNURBS3D*)obj,0) > 0 || zNURBS3DCPNum((zNURBS3D*)obj,1) > 0 ){
    ZRUNWARN( ZEO_ERR_NURBS_CPALREADY );
    zArray2Free( &((zNURBS3D*)obj)->cpnet );
  }
  size1 = ZTKInt( ztk );
  size2 = ZTKInt( ztk );
  zArray2Alloc( &((zNURBS3D*)obj)->cpnet, zNURBS3DCPCell, size1, size2 );
  return zArray2RowSize(&((zNURBS3D*)obj)->cpnet) == size1 &&
         zArray2ColSize(&((zNURBS3D*)obj)->cpnet) == size2 ? obj : NULL;
}
static void *_zNURBS3DCPFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  int j, k;
  j = ZTKInt( ztk );
  k = ZTKInt( ztk );
  if( !zArray2PosIsValid( &((zNURBS3D*)obj)->cpnet, j, k ) ){
    ZRUNERROR( ZEO_ERR_NURBS_INVALID_CP );
    return NULL;
  }
  zNURBS3DSetWeight( ((zNURBS3D*)obj), j, k, ZTKDouble(ztk) );
  zVec3DFromZTK( zNURBS3DCP(((zNURBS3D*)obj),j,k), ztk );
  return obj;
}

static bool _zNURBS3DUKnotFPrint(FILE *fp, int i, const void *obj){
  if( zVecSizeNC(((zNURBS3D*)obj)->param[0].knot) <= 0 ) return false;
  zVecFPrint( fp, ((zNURBS3D*)obj)->param[0].knot );
  return true;
}
static bool _zNURBS3DVKnotFPrint(FILE *fp, int i, const void *obj){
  if( zVecSizeNC(((zNURBS3D*)obj)->param[1].knot) <= 0 ) return false;
  zVecFPrint( fp, ((zNURBS3D*)obj)->param[1].knot );
  return true;
}
static bool _zNURBS3DSliceFPrint(FILE *fp, int i, const void *obj){
  fprintf( fp, "%d %d\n", zNURBS3DSlice((zNURBS3D*)obj,0), zNURBS3DSlice((zNURBS3D*)obj,1) );
  return true;
}
static bool _zNURBS3DSizeFPrint(FILE *fp, int i, const void *obj){
  fprintf( fp, "%d %d\n", zNURBS3DCPNum((zNURBS3D*)obj,0), zNURBS3DCPNum((zNURBS3D*)obj,1) );
  return true;
}

static const ZTKPrp __ztk_prp_nurbs[] = {
  { ZTK_KEY_ZEO_NURBS_UKNOT, 1, _zNURBS3DUKnotFromZTK, _zNURBS3DUKnotFPrint },
  { ZTK_KEY_ZEO_NURBS_VKNOT, 1, _zNURBS3DVKnotFromZTK, _zNURBS3DVKnotFPrint },
  { ZTK_KEY_ZEO_NURBS_SIZE,  1, _zNURBS3DSizeFromZTK, _zNURBS3DSizeFPrint },
  { ZTK_KEY_ZEO_NURBS_CP,   -1, _zNURBS3DCPFromZTK, NULL },
  { ZTK_KEY_ZEO_NURBS_SLICE, 1, _zNURBS3DSliceFromZTK, _zNURBS3DSliceFPrint },
};

/* read a 3D NURBS from a ZTK format processor. */
zNURBS3D *zNURBS3DFromZTK(zNURBS3D *nurbs, ZTK *ztk)
{
  zNURBS3DInit( nurbs );
  if( !_ZTKEvalKey( nurbs, NULL, ztk, __ztk_prp_nurbs ) ) return NULL;
  if( ( zNURBS3DOrder(nurbs,0) = zNURBS3DKnotNum(nurbs,0) - zNURBS3DCPNum(nurbs,0) - 1 ) <= 0 ||
      ( zNURBS3DOrder(nurbs,1) = zNURBS3DKnotNum(nurbs,1) - zNURBS3DCPNum(nurbs,1) - 1 ) <= 0 ){
    ZRUNERROR( ZEO_ERR_NURBS_INVALID_ORDER );
    return NULL;
  }
  return nurbs;
}

/* print out a 3D NURBS to a file. */
void zNURBS3DFPrintZTK(FILE *fp, const zNURBS3D *nurbs)
{
  int i, j;

  _ZTKPrpKeyFPrint( fp, (void*)nurbs, __ztk_prp_nurbs );
  for( i=0; i<zNURBS3DCPNum(nurbs,0); i++ )
    for( j=0; j<zNURBS3DCPNum(nurbs,1); j++ ){
      fprintf( fp, "%s: %d %d %.12g ", ZTK_KEY_ZEO_NURBS_CP, i, j, zNURBS3DWeight(nurbs,i,j) );
      zVec3DFPrint( fp, zNURBS3DCP(nurbs,i,j) );
    }
}
