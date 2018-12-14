/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs3d - NURBS surface
 * contributer: 2018 Naoki Wakisaka
 */
#include <zeo/zeo_nurbs3d.h>

#define ZNURBS3D_ITER_MAX 100

void zNURBS3DInit(zNURBS3D *ns)
{
  register int i;

  for( i=0; i<2; i++ ){
    ns->dim[i] = 0;
    ns->knot[i] = NULL;
    ns->cpsize[i] = 0;
  }
  zArrayInit(&ns->cparray);
}

bool zNURBS3DCreateGridXY(zNURBS3D *ns, int dim, int xnum, int ynum, double gwidth)
{
  register int i, j;

  if( xnum <= dim || ynum <= dim ){
    ZRUNERROR( ZM_ERR_NURBS_INVDIM );
    return false;
  }
  ns->dim[0] = dim;
  ns->dim[1] = dim;
  ns->cpsize[0] = xnum;
  ns->cpsize[1] = ynum;

  ns->knot[0] = zVecAlloc( xnum+dim+1 );
  ns->knot[1] = zVecAlloc( ynum+dim+1 );

  zArrayAlloc( &ns->cparray, zNURBS3DCPCell, xnum*ynum );
  if( !ns->knot[0] || !ns->knot[1] || zArrayNum(&ns->cparray) == 0 ){
    ZALLOCERROR();
    zNURBS3DDestroy( ns );
    return false;
  }

  /* set knots */
  zNURBS3DSetKnotUniformFitEnds( ns->knot[0], dim );
  zNURBS3DSetKnotUniformFitEnds( ns->knot[1], dim );
  /* set cp */
  for( i=0; i<xnum; i++ )
    for( j=0; j<ynum; j++ ){
      zVec3DCreate( zNURBS3DCP(ns,i,j), (i - (xnum-1)/2.0)*gwidth, (j - (ynum-1)/2.0)*gwidth, 0.0 );
      zNURBS3DWeight(ns,i,j) = 1.0;
    }
  return true;
}

void zNURBS3DDestroy(zNURBS3D *ns)
{
  zVecFreeAO( 2, ns->knot[0], ns->knot[1] );
  zArrayFree( &ns->cparray );
  zNURBS3DInit( ns );
}

/* ************************************************************************* */
zNURBS3D *zNURBS3DCloneKnot(zNURBS3D *org, zNURBS3D *cln)
{
  register int i;

  for( i=0; i<2; i++ ){
    cln->knot[i] = zVecClone( org->knot[i] );
    if( !cln->knot[i] ){
      ZALLOCERROR();
      return NULL;
    }
  }
  return cln;
}

zNURBS3D *zNURBS3DCloneCP(zNURBS3D *org, zNURBS3D *cln)
{
  zArrayAlloc( &cln->cparray, zNURBS3DCPCell, zArrayNum(&org->cparray) );
  if( zArrayNum(&cln->cparray) == 0 ){
    ZALLOCERROR();
    return NULL;
  }
  memcpy( zArrayBuf(&cln->cparray), zArrayBuf(&org->cparray), sizeof(zNURBS3DCPCell)*zArrayNum(&org->cparray) );
  return cln;
}

zNURBS3D *zNURBS3DClone(zNURBS3D *org, zNURBS3D *cln)
{
  register int i;

  for( i=0; i<2; i++ ){
    cln->dim[i]    = org->dim[i];
    cln->cpsize[i] = org->cpsize[i];
  }
  if( !zNURBS3DCloneKnot( org, cln ) || !zNURBS3DCloneCP( org, cln ) ){
    zNURBS3DDestroy( cln );
    return NULL;
  }
  return cln;
}

zNURBS3D *zNURBS3DCopyNC(zNURBS3D *src, zNURBS3D *dest)
{
  register int i;

  for( i=0; i<2; i++ ){
    dest->dim[i] = src->dim[i];
    dest->cpsize[i] = src->cpsize[i];
    zVecCopyNC( src->knot[i], dest->knot[i] );
  }
  memcpy( zArrayBuf(&dest->cparray), zArrayBuf(&src->cparray), sizeof(zNURBS3DCPCell)*zArrayNum(&src->cparray) );
  return dest;
}

zNURBS3D *zNURBS3DCopy(zNURBS3D *src, zNURBS3D *dest)
{
  return
    zVecSizeIsEqual( src->knot[0], dest->knot[0])       &&
    zVecSizeIsEqual( src->knot[1], dest->knot[1])       &&
    zArrayNum(&src->cparray) == zArrayNum(&dest->cparray) ?
    zNURBS3DCopyNC( src, dest ) : NULL;
}

/* ************************************************************************* */
void zNURBS3DSetKnotUniform(zVec knot)
{
  register int i;

  for( i=0; i<zVecSizeNC(knot); i++ )
    zVecElem(knot,i) = i;
}

void zNURBS3DSetKnotUniformFitEnds(zVec knot, int dim)
{
  register int i;
  int size;
  int n = 0;

  size = zVecSizeNC(knot);
  for( i=0; i<dim+1;    i++ ) zVecSetElem( knot, i, n );
  for(    ; i<size-dim; i++ ) zVecSetElem( knot, i, ++n );
  for(    ; i<size;     i++ ) zVecSetElem( knot, i, n );
}

void zNURBS3DKnotNormalize(zNURBS3D *ns)
{
  register int i;

  for( i=0; i<2; i++ ){
    zVecShift( ns->knot[i], -zNURBS3DKnot0(ns,i) );
    zVecDivDRC( ns->knot[i], zNURBS3DKnotE(ns,i) );
  }
}

/* ************************************************************************* */
/* TODO: merge _zNURBSSeg */
int zNURBS3DSeg(zVec knot, double t)
{
  register int i, j, k;

  /* NOTE: compparison of double type might be deprecated */
  if( t < zVecElem( knot, 0 ) ) return -1;
  if( t >= zVecElem( knot, zVecSizeNC(knot)-1 ) ) return -2;
  for( i=0, j=zVecSizeNC(knot)-1; ; ){
    while( zVecElem(knot,i+1) == zVecElem(knot,i) && i < j ) i++;
    while( zVecElem(knot,j-1) == zVecElem(knot,j) && j > i ) j--;
    if( ( k = ( i + j ) / 2 ) == i ) break;
    if( zVecElem(knot,k) <= t )
      i = k;
    else
      j = k;
  }
  return i;
}

/* TODO: merge _zNURBSBasis */
double zNURBS3DBasis(zVec knot, double t, int i, int r, int k)
{
  double t1, tr1, b=0;

  if( r == 1 )
    return i == k ? 1 : 0;
  if( i > k - r + 1 ){
    t1  = zVecElem(knot,i);
    tr1 = zVecElem(knot,i+r-1);
    if( tr1 != t1 )
      b += ( t - t1 ) / ( tr1 - t1 ) * zNURBS3DBasis( knot, t, i, r-1, k );
  }
  if( i <= k ){
    t1  = zVecElem(knot,i+1);
    tr1 = zVecElem(knot,i+r);
    if( tr1 != t1 )
      b += ( tr1 - t ) / ( tr1 - t1 ) * zNURBS3DBasis( knot, t, i+1, r-1, k);
  }
  return b;
}

zVec3D *zNURBS3DVec(zNURBS3D *ns, double t1, double t2, zVec3D *v)
{
  return  zNURBS3DVecBD( ns, t1, t2, v, NULL );
}

zVec3D *zNURBS3DVecBD(zNURBS3D *ns, double t1, double t2, zVec3D *v, zNURBS3DBasisData *bd)
{
  register int i, j;
  int s[2], emin[2], emax[2], cnt;
  double b, den, b1;
  zVec b2vec;

  s[0] = zNURBS3DSeg( ns->knot[0], t1 );
  s[1] = zNURBS3DSeg( ns->knot[1], t2 );
  /* vertex */
  if( s[0] < 0 && s[1] < 0 )
    return zVec3DCopy( zNURBS3DCP(ns,
                                  s[0]==-1 ? 0 : zNURBS3DCPSize(ns,0)-1,
                                  s[1]==-1 ? 0 : zNURBS3DCPSize(ns,1)-1), v );

  zVec3DClear( v );
  if( s[1] < 0 ){
    /* on edge */
    j = s[1]==-1 ? 0 : zNURBS3DCPSize(ns,1)-1;
    emax[0] = zMin( s[0]+1, zNURBS3DCPSize(ns,0) );
    emin[0] = zMax( s[0] - ns->dim[0], 0 );
    for( den=0, i=emin[0]; i<emax[0]; i++ ){
      b = zNURBS3DWeight(ns,i,j) * zNURBS3DBasis( ns->knot[0], t1, i, ns->dim[0]+1, s[0]);
      den += b;
      zVec3DCatDRC( v, b, zNURBS3DCP(ns,i,j) );
    }
  } else if( s[0] < 0 ){
    /* on edge */
    i = s[0]==-1 ? 0 : zNURBS3DCPSize(ns,0)-1;
    emax[1] = zMin( s[1]+1, zNURBS3DCPSize(ns,1) );
    emin[1] = zMax( s[1] - ns->dim[1], 0 );
    for( den=0, j=emin[1]; j<emax[1]; j++ ){
      b = zNURBS3DWeight(ns,i,j) * zNURBS3DBasis( ns->knot[1], t2, j, ns->dim[1]+1, s[1]);
      den += b;
      zVec3DCatDRC( v, b, zNURBS3DCP(ns,i,j) );
    }
  } else {
    /* on plane */
    emax[1] = zMin( s[1]+1, zNURBS3DCPSize(ns,1) );
    emin[1] = zMax( s[1] - ns->dim[1], 0 );
    b2vec = zVecAlloc( emax[1] - emin[1] );
    for( j=emin[1]; j<emax[1]; j++ )
      zVecElem(b2vec,j-emin[1]) = zNURBS3DBasis( ns->knot[1], t2, j, ns->dim[1]+1, s[1] );

    emax[0] = zMin( s[0]+1, zNURBS3DCPSize(ns,0) );
    emin[0] = zMax( s[0] - ns->dim[0], 0 );
    cnt = 0;
    for( den=0, i=emin[0]; i<emax[0]; i++ ){
      b1 = zNURBS3DBasis( ns->knot[0], t1, i, ns->dim[0]+1, s[0] );
      for( j=emin[1]; j<emax[1]; j++ ){
        b = zNURBS3DWeight(ns,i,j) * b1 * zVecElem(b2vec,j-emin[1]);
        den += b;
        zVec3DCatDRC( v, b, zNURBS3DCP(ns,i,j) );
        if( bd ){
          bd[cnt].cn[0] = i;
          bd[cnt].cn[1] = j;
          bd[cnt].w = b;
          cnt++;
        }
      }
    }
    zVecFree( b2vec );
  }

  return zIsTiny(den) ?
    zVec3DCopy( zNURBS3DCP(ns,0,0), v ) : zVec3DDivDRC( v, den );
}

/* ************************************************************************* */
/* TODO: merge _zNURBSBasisDiff */
static double zNURBS3DBasisDiff(zVec knot, int dim, double t, int i, int r, int k, int diff);
double zNURBS3DBasisDiff(zVec knot, int dim, double t, int i, int r, int k, int diff)
{
  double dt, b=0;

  if( diff == 0 )
    return zNURBS3DBasis( knot, t, i, r, k );
  if( diff > dim + 1 || diff < 0 ){
    ZRUNERROR( ZM_ERR_NURBS_INVODR );
    return NAN;
  }
  if( i > k - r + 1 ){
    if( !zIsTiny( ( dt = zVecElem(knot,i+r-1) - zVecElem(knot,i) ) ) )
      b += zNURBS3DBasisDiff( knot, dim, t, i, r-1, k, diff-1 ) / dt;
  }
  if( i <= k ){
    if( !zIsTiny( ( dt = zVecElem(knot,i+r) - zVecElem(knot,i+1) ) ) )
      b -= zNURBS3DBasisDiff( knot, dim, t, i+1, r-1, k, diff-1 ) / dt;
  }
  return b * ( r - 1 );
}

static double _zNURBS3DDenDiff(zNURBS3D *ns, double t1, double t2, int *s, int *emin, int *emax, int diff1, int diff2);
double _zNURBS3DDenDiff(zNURBS3D *ns, double t1, double t2, int *s, int *emin, int *emax, int diff1, int diff2)
{
  register int i, j;
  double den;

  for( den=0, i=emin[0]; i<emax[0]; i++ )
    for( j=emin[1]; j<emax[1]; j++ )
      den += zNURBS3DWeight(ns,i,j)
        * zNURBS3DBasisDiff( ns->knot[0], ns->dim[0], t1, i, ns->dim[0]+1, s[0], diff1 )
        * zNURBS3DBasisDiff( ns->knot[1], ns->dim[1], t2, j, ns->dim[1]+1, s[1], diff2 );
  return den;
}

zVec3D *zNURBS3DDiff(zNURBS3D *ns, double t1, double t2, zVec3D *v, int diff1, int diff2)
{
  register int i, j;
  int t[2], s[2], emin[2], emax[2];
  double den, b;
  zVec3D tmp, tmp2;

  if( diff1 == 0 && diff2 == 0 )
    return zNURBS3DVec( ns, t1, t2, v );
  if( diff1 > ns->dim[0] + 1 || diff2 > ns->dim[1] + 1 || diff1 < 0 || diff2 < 0 ){
    ZRUNERROR( ZM_ERR_NURBS_INVODR );
    return NULL;
  }

  _zVec3DClear( v );
  t[0]=t1; t[1]=t2;
  den = 0.0;
  for( i=0; i<2; i++ ){
    s[i] = zNURBS3DSeg( ns->knot[i], t[i] );
    if( s[i] == -1 )
      t[i] = zNURBS3DKnot0(ns,i);
    else if( s[i] == -2 )
      t[i] = zNURBS3DKnotE(ns,0);
    emin[i] = zMax( s[i] - ns->dim[i], 0 );
    emax[i] = zMin( s[i]+1, zNURBS3DCPSize(ns,i) );
  }

  for( i=emin[0]; i<emax[0]; i++ )
    for( j=emin[1]; j<emax[1]; j++ ){
      b = zNURBS3DWeight(ns,i,j)
        * zNURBS3DBasisDiff( ns->knot[0], ns->dim[0], t[0], i, ns->dim[0]+1, s[0], diff1 )
        * zNURBS3DBasisDiff( ns->knot[1], ns->dim[1], t[1], j, ns->dim[1]+1, s[1], diff2 );
      den += zNURBS3DWeight(ns,i,j)
        * zNURBS3DBasis( ns->knot[0], t[0], i, ns->dim[0]+1, s[0] )
        * zNURBS3DBasis( ns->knot[1], t[1], j, ns->dim[1]+1, s[1] );
      zVec3DCatDRC( v, b, zNURBS3DCP(ns,i,j) );
    }


  for( i=1; i<diff1+1; i++ ){
    if( !zNURBS3DDiff( ns, t[0], t[1], &tmp, diff1-i, diff2 ) ) break;
    zVec3DCatDRC( v, -zCombi(diff1,i)*_zNURBS3DDenDiff( ns, t[0], t[1], s, emin, emax, i, 0 ), &tmp );
  }
  for( j=1; j<diff2+1; j++ ){
    if( !zNURBS3DDiff( ns, t[0], t[1], &tmp, diff1, diff2-j ) ) break;
    zVec3DCatDRC( v, -zCombi(diff2,j)*_zNURBS3DDenDiff( ns, t[0], t[1], s, emin, emax, 0, j ), &tmp );
  }
  for( i=1; i<diff1+1; i++ ){
    zVec3DClear( &tmp2 );
    for( j=1; j<diff2+1; j++ ){
      if( !zNURBS3DDiff( ns, t[0], t[1], &tmp, diff1-i, diff2-j ) ) break;
      zVec3DCatDRC( &tmp2, -zCombi(diff2,j)*_zNURBS3DDenDiff( ns, t[0], t[1], s, emin, emax, i, j ), &tmp );
    }
    zVec3DCatDRC( v, zCombi(diff1,i), &tmp2 );
  }

  return zIsTiny(den) ?
    zVec3DCopy( zNURBS3DCP(ns,0,0), v ) : zVec3DDivDRC( v, den );
}

/* ************************************************************************* */
/* NOTE: the implementation can be simplified
 *       such as the original suzuki's formulation                             */
zVec3D *zNURBS3DNormVec(zNURBS3D *ns, double t1, double t2, zVec3D *v)
{
  zVec3D v1, v2;

  if( !zNURBS3DDiff( ns, t1, t2, &v1, 1, 0 ) ||
      !zNURBS3DDiff( ns, t1, t2, &v2, 0, 1 ) ) return NULL;
  _zVec3DOuterProd( &v1, &v2, v );
  zVec3DNormalizeDRC( v );
  return v;
}

/* ************************************************************************* */
/* NOTE: only the cases that the position on the NURBS surface is unique
 *       with respect to x and y factors.
 *       for example, XYGrid and control points move along with z axis only. */
double zNURBS3DVecOnXY(zNURBS3D *ns, double x, double y, zVec3D *v, double *t1, double *t2, double tol)
{
  return zNURBS3DVecOnXYBD( ns, x, y, v, t1, t2, NULL, tol );
}

double zNURBS3DVecOnXYBD(zNURBS3D *ns, double x, double y, zVec3D *v, double *t1, double *t2, zNURBS3DBasisData *bd, double tol)
{
  register int i;
  double tmin[2], tmax[2], err;

  tmin[0] = zVecElem(ns->knot[0],0);
  tmin[1] = zVecElem(ns->knot[1],0);
  tmax[0] = zVecElem(ns->knot[0],zVecSizeNC(ns->knot[0])-1);
  tmax[1] = zVecElem(ns->knot[1],zVecSizeNC(ns->knot[1])-1);

  tol = zSqr( tol );
  for( i=0; i<ZNURBS3D_ITER_MAX; i++ ){
    *t1 = (tmin[0] + tmax[0]) / 2.0;
    *t2 = (tmin[1] + tmax[1]) / 2.0;
    zNURBS3DVecBD( ns, *t1, *t2, v, bd );
    err = zSqr( v->e[zX] - x ) + zSqr( v->e[zY] - y );
    if( zIsTiny( tmin[0] - tmax[0] ) || zIsTiny( tmin[1] - tmax[1] ) || err < tol ) break;

    if( x < v->e[zX] )
      tmax[0] = *t1;
    else
      tmin[0] = *t1;

    if( y < v->e[zY] )
      tmax[1] = *t2;
    else
      tmin[1] = *t2;
  }

  return sqrt( err );
}

/* NOTE: only when zNURBS3DVecNNOnXY is valid, this function is valid.
 *       generally, NURBS parameters t1, t2 and line parameter k are found by Newton method.
 * line: v + k dir, if k < 0, return NULL?
 * intersection point: cv */
zVec3D* zIntersectLineNURBS3D(zNURBS3D *ns, zVec3D *v, zVec3D *dir, zVec3D *cv, double *t1, double *t2, double tol)
{
  const double GAIN = 5;
  register int i;
  double l;
  zVec3D vn, vs;
  bool plas = true;
  double pmcGain = 1.0 / ZNURBS3D_ITER_MAX;
  const double PMCGAINMAG = 0.5;

  zVec3DCopy( v, cv );
  zNURBS3DVecOnXY( ns, cv->e[zX], cv->e[zY], &vn, t1, t2, tol );
  for( i=0; i<ZNURBS3D_ITER_MAX; i++ ){
    _zVec3DSub( cv, &vn, &vs );
    l = _zVec3DNorm( &vs );
    if( l < tol ) return cv;
    if( cv->e[zZ] - vn.e[zZ] > 0 ){
      if( !plas ){
        plas = true;
        pmcGain *= PMCGAINMAG;
      }
      zVec3DCatDRC( cv, l*(pmcGain*GAIN*(ZNURBS3D_ITER_MAX-i)), dir );
    } else {
      if( plas ){
        plas = false;
        pmcGain *= PMCGAINMAG;
      }
      zVec3DCatDRC( cv, l*-(pmcGain*GAIN*(ZNURBS3D_ITER_MAX-i)), dir );
    }
    zNURBS3DVecOnXY( ns, cv->e[zX], cv->e[zY], &vn, t1, t2, tol );
  }
  return NULL;
}

static bool _zVec3DEqualTol(zVec3D *v1, zVec3D *v2, double tol)
{
  zVec3D d;

  _zVec3DSub( v1, v2, &d );
  return _zVec3DIsTol( &d, tol );
}

/* for debug */
/* NOTE: this function is mainly used in Terra library
 *       but the computation cost is high because the convergence slows and
 *       the solution might not be the nearest point of v when there are two or more intersection points.
 */
zVec3D* zIntersectLineNURBS3D_v2(zNURBS3D *ns, zVec3D *v, zVec3D *dir, zVec3D *cv, double *t1, double *t2, double tol)
{
  register int i;
  zVec3D vn, cvn, norm, pv;

  if( zIsTiny( zVec3DNormalize( dir, &norm ) ) ) return NULL;
  zVec3DCopy( v, cv );
  zVec3DCopy( v, &pv );
  for( i=0; i<ZNURBS3D_ITER_MAX; i++ ){
    zNURBS3DVecOnXY( ns, cv->e[zX], cv->e[zY], &vn, t1, t2, tol );
    if( _zVec3DEqualTol( cv, &vn, tol ) ) return cv;
    /* cv <- cv + (dir^T(vn-cv))dir */
    _zVec3DSub( &vn, cv, &cvn );
    zVec3DCatDRC( cv, _zVec3DInnerProd( &norm, &cvn ), &norm );
    if( _zVec3DEqualTol( cv, &pv, tol ) ) return cv;
    zVec3DCopy( cv, &pv );
  }
  return NULL;
}

/* binary search */
/* NOTE: the same problem of v2 happens.
 */
zVec3D* zIntersectLineNURBS3D_v3(zNURBS3D *ns, zVec3D *v, zVec3D *dir, zVec3D *cv, double *t1, double *t2, double tol)
{
  register int i;
  double tmin[2], tmax[2];
  zVec3D proj, norm;

  if( zIsTiny( zVec3DNormalize( dir, &norm ) ) ) return NULL;
  tmin[0] = zVecElem(ns->knot[0],0);
  tmin[1] = zVecElem(ns->knot[1],0);
  tmax[0] = zVecElem(ns->knot[0],zVecSizeNC(ns->knot[0])-1);
  tmax[1] = zVecElem(ns->knot[1],zVecSizeNC(ns->knot[1])-1);

  for( i=0; i<ZNURBS3D_ITER_MAX; i++ ){
    *t1 = (tmin[0] + tmax[0]) / 2.0;
    *t2 = (tmin[1] + tmax[1]) / 2.0;
    zNURBS3DVec( ns, *t1, *t2, cv );
    _zVec3DSub( cv, v, &proj );
    zVec3DCat( v, _zVec3DInnerProd( &norm, &proj ), &norm, &proj );
    _zVec3DSubDRC( &proj, cv );
    if( zIsTiny( tmin[0] - tmax[0] ) || zIsTiny( tmin[1] - tmax[1] ) || zVec3DNorm( &proj ) < tol )
      return cv;

    if( proj.e[zX] < 0 )
      tmax[0] = *t1;
    else
      tmin[0] = *t1;

    if( proj.e[zY] < 0 )
      tmax[1] = *t2;
    else
      tmin[1] = *t2;
  }

  return NULL;
}

/* ************************************************************************* */
/* io */
bool zNURBS3DFRead(FILE *fp, zNURBS3D *ns)
{
  register int i, j;

  zNURBS3DInit( ns );
  if( feof(fp) ) return false;
  zFSkipDelimiter( fp );
  for( i=0; i<2; i++ ){
    ns->cpsize[i] = zFInt( fp );
    ns->dim[i]    = zFInt( fp );
    if( !( ns->knot[i] = zVecAlloc( zNURBS3DCalcKnotNum( ns, i ) ) ) ){
      zNURBS3DInit( ns );
      return false;
    }
    for( j=0; j<zVecSizeNC(ns->knot[i]); j++ )
      zVecElem(ns->knot[i],j) = zFDouble( fp );
  }
  zArrayAlloc( &ns->cparray, zNURBS3DCPCell, zNURBS3DCalcCPNum( ns ) );
  for( i=0; i<zNURBS3DCPNum(ns); i++ ){
    for( j=0; j<3; j++ )
      zNURBS3DCPE(ns,i)->e[j] = zFDouble( fp );
    zNURBS3DWeightE(ns,i) = zFDouble( fp );
  }
  return true;
}

bool zNURBS3DReadFile(zNURBS3D *ns, char filename[])
{
  FILE *fp;
  bool ret;

  if( !( fp = zOpenFile( filename, ZNURBS3D_SUFFIX, "r" ) ) )
    return false;
  ret = zNURBS3DFRead( fp, ns );
  fclose( fp );
  return ret;
}



void zNURBS3DFWrite(FILE *fp, zNURBS3D *ns)
{
  register int i,j;

  for( i=0; i<2; i++ ){
    fprintf( fp, "( %d %d ) ( ", ns->cpsize[i], ns->dim[i] );
    for( j=0; j<zVecSizeNC(ns->knot[i]); j++ )
      fprintf( fp, "%.10g ", zVecElem(ns->knot[i],j) );
    fprintf( fp, ")\n" );
  }
  for( i=0; i<ns->cpsize[0]; i++ ){
    for( j=0; j<ns->cpsize[1]; j++ ){
      fprintf( fp, " (" );
      zVec3DDataFWrite( fp, zNURBS3DCP(ns,i,j) );
      fprintf( fp, " ,%.10g )", zNURBS3DWeight(ns,i,j) );
    }
    fprintf( fp, "\n" );
  }
}

bool zNURBS3DWriteFile(zNURBS3D *ns, char filename[])
{
  char fname[BUFSIZ];
  FILE *fp;

  zAddSuffix( filename, ZNURBS3D_SUFFIX, fname, BUFSIZ );
  if( !( fp = fopen( fname, "w" ) ) ){
    ZOPENERROR( fname );
    return false;
  }
  zNURBS3DFWrite( fp, ns );
  fclose( fp );
  return true;
}


/* ************************************************************************* */
/* sequence */
/* ************************************************************************* */
void zNURBS3DSeqInit(zNURBS3DSeq *nseq)
{
  zListInit( nseq );
  zListRoot(nseq)->data.dt = 0;
  zNURBS3DInit( &zListRoot(nseq)->data.ns );
}

void zNURBS3DSeqFree(zNURBS3DSeq *nseq)
{
  zNURBS3DSeqListCell *cp;

  while( !zListIsEmpty( nseq ) )
    if( ( cp = zNURBS3DSeqDequeue( nseq ) ) )
      zNURBS3DSeqListCellFree( cp );
}

zNURBS3DSeqListCell *zNURBS3DSeqEnqueue(zNURBS3DSeq *nseq, zNURBS3D *ns, double dt)
{
  zNURBS3DSeqListCell *cp;

  if( !ns ){
    ZRUNERROR( "null pointer assigned" );
    return NULL;
  }
  if( !( cp = zAlloc( zNURBS3DSeqListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  if( !( zNURBS3DClone( ns, &cp->data.ns ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cp->data.dt = dt;
  zQueueEnqueue( nseq, cp );
  return cp;
}

zNURBS3DSeqListCell *zNURBS3DSeqDequeue(zNURBS3DSeq *nseq)
{
  zNURBS3DSeqListCell *cp;

  if( zListIsEmpty( nseq ) ){
    ZRUNWARN( ZM_WARN_SEQ_EMPTY );
    return NULL;
  }
  zQueueDequeue( nseq, &cp );
  return cp;
}

/* io */
zNURBS3DSeq *zNURBS3DSeqFRead(FILE *fp, zNURBS3DSeq *nseq)
{
  zNURBS3DSeqListCell *cp;

  zNURBS3DSeqInit( nseq );
  while( !feof(fp) ){
    if( !zFSkipDelimiter( fp ) ) break;
    if( !( cp = zAlloc( zNURBS3DSeqListCell, 1 ) ) ){
      ZALLOCERROR();
      break;
    }
    cp->data.dt = zFDouble(fp);
    if( !zNURBS3DFRead( fp, &cp->data.ns ) ){
      ZALLOCERROR();
      free( cp );
      break;
    }

    zQueueEnqueue( nseq, cp );
  }
  return nseq;
}

zNURBS3DSeq *zNURBS3DSeqReadFile(zNURBS3DSeq *nseq, char filename[])
{
  FILE *fp;
  zNURBS3DSeq *tmp;

  if( !( fp = zOpenFile( filename, ZNURBS3DSEQ_SUFFIX, "r" ) ) )
    return NULL;
  tmp = zNURBS3DSeqFRead( fp, nseq );
  fclose( fp );
  return tmp;
}

void zNURBS3DSeqFWrite(FILE *fp, zNURBS3DSeq *nseq)
{
  zNURBS3DSeqListCell *cp;

  zListForEachRew( nseq, cp ){
    fprintf( fp, "%.10g ", cp->data.dt );
    zNURBS3DFWrite( fp, &cp->data.ns );
  }
}

void zNURBS3DSeqWriteFile(zNURBS3DSeq *nseq, char filename[])
{
  char fname[BUFSIZ];
  FILE *fp;

  zAddSuffix( filename, ZNURBS3DSEQ_SUFFIX, fname, BUFSIZ );
  if( !( fp = fopen( fname, "w" ) ) ){
    ZOPENERROR( fname );
    return;
  }
  zNURBS3DSeqFWrite( fp, nseq );
  fclose( fp );
  return;
}

/* ************************************************************************* */
/* binary io */
#define ZNURBS3DSEQB_COMP_DIMKNOTNC  0x1 /* dim and knot are not changed */
#define ZNURBS3DSEQB_COMP_CPXYWNC    0x2 /* x, y and weight of cp are not changed */
#define ZNURBS3DSEQB_COMP_CPDIFFONLY 0x4 /* write only difference of cp */

/* byte swap */
void zNURBS3DSeqByteSwapKnotCP(zNURBS3DSeq *nseq){
  zNURBS3DSeqListCell *cp;
  register int i, j;
  int dsize;
  char *buf;

  dsize = sizeof(double);
  buf = zAlloc( char, dsize );
  zListForEachRew( nseq, cp ){
    zByteSwapAllBufDRC( &cp->data.dt, dsize, buf );
    for( i=0; i<2; i++ )
      for( j=0; j<zVecSizeNC(cp->data.ns.knot[i]); j++ )
        zByteSwapAllBufDRC( &zVecElem(cp->data.ns.knot[i],j), dsize, buf );
    for( i=0; i<zNURBS3DCPNum(&cp->data.ns); i++ )
      for( j=0; j<3; j++ )
        zByteSwapAllBufDRC( &zNURBS3DCPE(&cp->data.ns,i)->e[j], dsize, buf );
  }

  zFree( buf );
}

/* ************************************************************************* */
/* read */
static zHeader *_zNURBS3DSeqBFReadHeader(FILE *fp, zHeader *h);
static zNURBS3D *_zNURBS3DSeqBFReadKnot(FILE *fp, zNURBS3D *ns, zHeader *h);
static zNURBS3D *_zNURBS3DSeqBFReadCP(FILE *fp, zNURBS3D *ns, int i, int comp);
static zNURBS3DSeqListCell *_zNURBS3DSeqBFRead(FILE *fp, zNURBS3DSeqListCell *cp, zNURBS3DSeqListCell *pcp, zHeader *header, int comp);

zHeader *_zNURBS3DSeqBFReadHeader(FILE *fp, zHeader *h)
{
  zFileHeaderFReadB( fp, h );
  switch( h->state ){
  case ZHEADER_BROKEN:
    ZRUNERROR( "Header is BROKEN !! Can't read !!" );
    return NULL;
  case ZHEADER_NOTHING:
    ZRUNWARN( "Header Nothing !" );
    break;
  case ZHEADER_CORRECT:
    if( strcmp( h->suffix, ZNURBS3DSEQB_SUFFIX ) !=0 ){
      ZRUNERROR( "This File is NOT zNURBS3DSeqB File. Can't read !!" );
      return NULL;
    }
    if( zFileHeaderDVer(h) != ZNURBS3DSEQB_VERSION ){
      ZRUNERROR( "zSeqB File is another version. Can't read !!" );
      return NULL;
    }
    if( !zFileHeaderEndianIsEqual( h ) ){
      ZRUNERROR( "Cant read this endian !!" );
      return NULL;
    }
  }
  return h;
}

zNURBS3D *_zNURBS3DSeqBFReadKnot(FILE *fp, zNURBS3D *ns, zHeader *h)
{
  register int i;

  for( i=0; i<2; i++ ){
    if( fread( &ns->cpsize[i], sizeof(int), 1, fp ) != 1 ||
        fread( &ns->dim[i],    sizeof(int), 1, fp ) != 1 )
      return NULL;
    if( zFileHeaderEndianIsAnother( h ) ){
      zByteSwapDRC( int, ns->cpsize[i] );
      zByteSwapDRC( int, ns->dim[i] );
    }
    if( !( ns->knot[i] = zVecAlloc( zNURBS3DCalcKnotNum( ns, i ) ) ) ||
        fread(zVecBuf(ns->knot[i]), sizeof(double), zVecSizeNC(ns->knot[i]), fp ) != zVecSizeNC(ns->knot[i]) ){
      zNURBS3DDestroy( ns );
      return NULL;
    }
  }
  return ns;
}

zNURBS3D *_zNURBS3DSeqBFReadCP(FILE *fp, zNURBS3D *ns, int i, int comp)
{
  if( comp & ZNURBS3DSEQB_COMP_CPXYWNC ){
    if( fread( &zNURBS3DCPE(ns,i)->e[zZ], sizeof(double), 1, fp ) != 1 )
      return NULL;
  } else {
    if( fread( &zNURBS3DCPE(ns,i)->e[zX], sizeof(double), 3, fp ) != 3 ||
        fread( &zNURBS3DWeightE(ns,i), sizeof(double), 1, fp ) != 1 )
      return NULL;
  }
  return ns;
}

zNURBS3DSeqListCell *_zNURBS3DSeqBFRead(FILE *fp, zNURBS3DSeqListCell *cp, zNURBS3DSeqListCell *pcp, zHeader *header, int comp)
{
  zNURBS3D *ns, *pns;
  register int i;
  int num, id;

  ns  = &cp->data.ns;
  pns = &pcp->data.ns;
  zNURBS3DInit( ns );
  if( fread( &cp->data.dt, sizeof(cp->data.dt), 1, fp ) != 1 )
    return NULL;

  /* dim and knot */
  if( comp & ZNURBS3DSEQB_COMP_DIMKNOTNC ){
    for( i=0; i<2; i++ ){
      ns->dim[i] = pns->dim[i];
      ns->cpsize[i] = pns->cpsize[i];
    }
     zNURBS3DCloneKnot( pns, ns );
  } else if( !_zNURBS3DSeqBFReadKnot( fp, ns, header ) )
    return NULL;

  /* control points */
  /* alloc */
  if( comp & ZNURBS3DSEQB_COMP_CPXYWNC || comp & ZNURBS3DSEQB_COMP_CPDIFFONLY )
    zNURBS3DCloneCP( pns, ns );
  else
    zArrayAlloc( &ns->cparray, zNURBS3DCPCell, zNURBS3DCalcCPNum( ns ) );

  /* read */
  if( !(comp & ZNURBS3DSEQB_COMP_CPDIFFONLY) ){
    for( i=0; i<zNURBS3DCPNum(ns); i++ )
      if( !_zNURBS3DSeqBFReadCP( fp, ns, i, comp ) )
        return NULL;
  } else {
    if( fread( &num, sizeof(int), 1, fp ) != 1 || num < 0 || num > zNURBS3DCPNum(ns) )
      return NULL;
    for( ; num>0; num-- ){
      if( fread( &id, sizeof(int), 1, fp ) != 1 || !_zNURBS3DSeqBFReadCP( fp, ns, id, comp ) )
        return NULL;
    }
  }

  return cp;
}

zNURBS3DSeq *zNURBS3DSeqBFReadHeader(FILE *fp, zNURBS3DSeq *nseq, zHeader *header)
{
  zNURBS3DSeqListCell *cp, *pcp = NULL;
  int comp;

  if( !_zNURBS3DSeqBFReadHeader( fp, header ) )
    return NULL;

  zNURBS3DSeqInit( nseq );
  while( !feof(fp) ){
    if( !( cp = zAlloc( zNURBS3DSeqListCell, 1 ) ) ){
      ZALLOCERROR();
      break;
    }
    if( fread( &comp, sizeof(int), 1, fp ) != 1 ){
      if( !feof(fp) ) ZRUNERROR( "zNURBS3DSeqB File is Broken !!" );
      zNURBS3DSeqListCellFree( cp );
      break;
    }
    if( !_zNURBS3DSeqBFRead( fp, cp, pcp, header, comp ) ){
      ZRUNERROR( "zNURBS3DSeqB File is Broken !!" );
      zNURBS3DSeqListCellFree( cp );
      break;
    }

    zQueueEnqueue( nseq, cp );
    pcp = cp;
  }

  if( zFileHeaderEndianIsAnother( header ) )
    zNURBS3DSeqByteSwapKnotCP( nseq );
  return nseq;
}

zNURBS3DSeq *zNURBS3DSeqBFRead(FILE *fp, zNURBS3DSeq *nseq)
{
  zHeader h;
  return zNURBS3DSeqBFReadHeader( fp, nseq, &h );
}

zNURBS3DSeq *zNURBS3DSeqBReadFileHeader(zNURBS3DSeq *nseq, char filename[], zHeader *header)
{
  FILE *fp;
  zNURBS3DSeq *ret;

  if( !( fp = zOpenFile( filename, ZNURBS3DSEQB_SUFFIX, "rb" ) ) )
    return NULL;
  ret = zNURBS3DSeqBFReadHeader( fp, nseq, header );
  fclose( fp );
  return ret;
}

zNURBS3DSeq *zNURBS3DSeqBReadFile(zNURBS3DSeq *nseq, char filename[])
{
  zHeader h;
  return zNURBS3DSeqBReadFileHeader( nseq, filename, &h );
}

/* ************************************************************************* */
/* write */
static int _zNURBS3DSeqBFWriteCompType(zNURBS3D *ns, zNURBS3D *pns, zIndex idx, int *comp);
static void _zNURBS3DSeqBCellDataFWriteCP(FILE *fp, zNURBS3D *ns, int i, int comp);

zHeader* zNURBS3DSeqBGetHeaderDefault(zHeader *h)
{
  zFileHeaderInit( h );
  zFileHeaderSetSuffix( h, ZNURBS3DSEQB_SUFFIX );
  zFileHeaderDVer(h) = ZNURBS3DSEQB_VERSION;
  return h;
}

int _zNURBS3DSeqBFWriteCompType(zNURBS3D *ns, zNURBS3D *pns, zIndex idx, int *comp)
{
  register int i;
  bool xywchanged;
  int cnt, wsize;

  *comp = 0;
  if( !pns ) return 0;
  if( ns->cpsize[0] == pns->cpsize[0] && ns->dim[0] == pns->dim[0] && zVecIsEqual( ns->knot[0], pns->knot[0] ) &&
      ns->cpsize[1] == pns->cpsize[1] && ns->dim[1] == pns->dim[1] && zVecIsEqual( ns->knot[1], pns->knot[1] ) ){
    *comp |= ZNURBS3DSEQB_COMP_DIMKNOTNC;
  }

  cnt = 0;
  xywchanged = false;
  for( i=0; i<zNURBS3DCPNum(ns); i++ ){
    if( !zIsTiny( zNURBS3DCPE(ns,i)->e[zX] - zNURBS3DCPE(pns,i)->e[zX] ) ||
        !zIsTiny( zNURBS3DCPE(ns,i)->e[zY] - zNURBS3DCPE(pns,i)->e[zY] ) ||
        !zIsTiny( zNURBS3DWeightE(ns,i) - zNURBS3DWeightE(pns,i) ) ){
      xywchanged = true;
      zIndexElem(idx,cnt) = i;
      cnt++;
    } else if( !zIsTiny( zNURBS3DCPE(ns,i)->e[zZ] - zNURBS3DCPE(pns,i)->e[zZ] ) ){
      zIndexElem(idx,cnt) = i;
      cnt++;
    }
  }
  if( !xywchanged ) *comp |= ZNURBS3DSEQB_COMP_CPXYWNC;

  wsize = xywchanged ? 4 : 1; /* write xyzw -> 4 or z -> 1 */
  if( ( cnt * ( sizeof(double)*wsize + sizeof(int) ) + sizeof(int) ) < ( zNURBS3DCPNum(ns) * sizeof(double)*wsize ) )
    *comp |= ZNURBS3DSEQB_COMP_CPDIFFONLY;

  return cnt;
}

void _zNURBS3DSeqBCellDataFWriteCP(FILE *fp, zNURBS3D *ns, int i, int comp)
{
  if( comp & ZNURBS3DSEQB_COMP_CPXYWNC )
    fwrite( &zNURBS3DCPE(ns,i)->e[zZ], sizeof(double), 1, fp );
  else {
    fwrite( &zNURBS3DCPE(ns,i)->e[zX], sizeof(double), 3, fp );
    fwrite( &zNURBS3DWeightE(ns,i), sizeof(double), 1, fp );
  }
}


bool zNURBS3DSeqBCellDataFWrite(FILE *fp, zNURBS3D *ns, double dt, zNURBS3D *pns)
{
  zIndex idx;
  int comp, cnt;
  register int i;

  if( !( idx = zIndexCreate( zNURBS3DCPNum(ns) ) ) )
    return false;
  zIndexClear( idx );

  cnt = _zNURBS3DSeqBFWriteCompType( ns, pns, idx, &comp );
  fwrite( &comp, sizeof(int), 1, fp );
  fwrite( &dt, sizeof(double), 1, fp );

  if( !(comp & ZNURBS3DSEQB_COMP_DIMKNOTNC) )
    for( i=0; i<2; i++ ){
      fwrite( &ns->cpsize[i], sizeof(int), 1, fp );
      fwrite( &ns->dim[i], sizeof(int), 1, fp );
      fwrite( zVecBuf(ns->knot[i]), sizeof(double), zVecSizeNC(ns->knot[i]), fp );
    }

  if( !(comp & ZNURBS3DSEQB_COMP_CPDIFFONLY) )
    for( i=0; i<zNURBS3DCPNum(ns); i++ )
      _zNURBS3DSeqBCellDataFWriteCP( fp, ns, i, comp );
  else {
    fwrite( &cnt, sizeof(int), 1, fp );
    for( i=0 ; i<cnt; i++ ){
      fwrite( &zIndexElem(idx,i), sizeof(int), 1, fp );
      _zNURBS3DSeqBCellDataFWriteCP( fp, ns, zIndexElem(idx,i), comp );
    }
  }

  zIndexFree( idx );
  return true;
}

bool zNURBS3DSeqBDataFWrite(FILE *fp, zNURBS3DSeq *nseq)
{
  zNURBS3DSeqListCell *cp;
  zNURBS3D *pns = NULL;

  zListForEachRew( nseq, cp ){
    if( !zNURBS3DSeqBCellDataFWrite( fp, &cp->data.ns, cp->data.dt, pns ) )
      return false;
    pns = &cp->data.ns;
  }
  return true;
}

bool zNURBS3DSeqBFWriteHeader(FILE *fp, zNURBS3DSeq *nseq, zHeader *header)
{
  zFileHeaderFWriteB( fp, header );
  return zNURBS3DSeqBDataFWrite( fp, nseq );
}

bool zNURBS3DSeqBFWrite(FILE *fp, zNURBS3DSeq *nseq)
{
  zHeader h;

  zNURBS3DSeqBGetHeaderDefault( &h );
  return zNURBS3DSeqBFWriteHeader( fp, nseq, &h );
}

bool zNURBS3DSeqBWriteFileHeader(zNURBS3DSeq *nseq, char filename[], zHeader *header)
{
  char fname[BUFSIZ];
  FILE *fp;
  bool ret;

  zAddSuffix( filename, ZNURBS3DSEQB_SUFFIX, fname, BUFSIZ );
  if( !( fp = fopen( fname, "wb" ) ) ){
    ZOPENERROR( fname );
    return false;
  }
  ret = zNURBS3DSeqBFWriteHeader( fp, nseq, header );
  fclose( fp );
  return ret;
}

bool zNURBS3DSeqBWriteFile(zNURBS3DSeq *nseq, char filename[])
{
  zHeader h;

  zNURBS3DSeqBGetHeaderDefault( &h );
  return zNURBS3DSeqBWriteFileHeader( nseq, filename, &h );
}
