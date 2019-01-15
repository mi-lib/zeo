/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs - NURBS curve / surface
 */

#include <zeo/zeo_nurbs.h>

static void _zNURBS3DKnotInit(zNURBS3D *nurbs, int i);
static int _zNURBS3DSeg(zNURBS3D *nurbs, int i, double t);

static double _zNURBS3DBasis(zNURBS3D *nurbs, int i, double t, int j, int r, int seg);
static double _zNURBS3DBasisDiff(zNURBS3D *nurbs, int i, double t, int j, int r, int seg);

/* set knots & assign control points & initialize weight uniformly. */
void _zNURBS3DKnotInit(zNURBS3D *nurbs, int i)
{
  register int j;

  for( j=0; j<=nurbs->dim[i]; j++ )
    zNURBS3DSetKnot( nurbs, i, j, 0 );
  for( ; j<=zNURBS3DCPNum(nurbs,i); j++ )
    zNURBS3DSetKnot( nurbs, i, j, zNURBS3DKnot(nurbs,i,j-1) + 1 );
  for( ; j<zNURBS3DKnotNum(nurbs,i); j++ )
    zNURBS3DSetKnot( nurbs, i, j, zNURBS3DKnot(nurbs,i,j-1) );
}

/* allocate a NURBS curve / surface. */
bool zNURBS3DAlloc(zNURBS3D *nurbs, int size1, int size2, int dim1, int dim2)
{
  register int i, j;

  if( size1 <= dim1 || size2 <= dim2 ){
    ZRUNERROR( ZEO_ERR_NURBS_INVDIM );
    return false;
  }
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
  zNURBS3DSetSliceNum( nurbs, ZEO_NURBS3D_DEFAULT_NS, ZEO_NURBS3D_DEFAULT_NS );
  return true;
}

/* initialize a NURBS curve / surface. */
zNURBS3D *zNURBS3DInit(zNURBS3D *nurbs)
{
  nurbs->dim[0] = nurbs->dim[1] = 0;
  zNURBS3DSetSliceNum( nurbs, 0, 0 );
  nurbs->knot[0] = nurbs->knot[1] = NULL;
  zArray2Init( &nurbs->cpnet );
  return nurbs;
}

/* destroy a NURBS curve / surface. */
void zNURBS3DDestroy(zNURBS3D *nurbs)
{
  zVecFree( nurbs->knot[0] );
  zVecFree( nurbs->knot[1] );
  zArray2Free( &nurbs->cpnet );
  zNURBS3DInit( nurbs );
}

/* copy a NURBS curve / surface. */
zNURBS3D *zNURBS3DCopy(zNURBS3D *src, zNURBS3D *dest)
{
  if( src->dim[0] != dest->dim[0] || src->dim[1] != dest->dim[1] ||
      !zVecSizeIsEqual( src->knot[0], dest->knot[0] ) ||
      !zVecSizeIsEqual( src->knot[1], dest->knot[1] ) ||
      zNURBS3DCPNum(src,0) != zNURBS3DCPNum(dest,0) ||
      zNURBS3DCPNum(src,1) != zNURBS3DCPNum(dest,1) ){
    ZRUNERROR( ZEO_ERR_NURBS_SIZMIS );
    return NULL;
  }
  zNURBS3DSetSliceNum( dest, src->ns[0], src->ns[1] );
  zVecCopyNC( src->knot[0], dest->knot[0] );
  zVecCopyNC( src->knot[1], dest->knot[1] );
  memcpy( zArray2Buf(&dest->cpnet), zArray2Buf(&src->cpnet), zNURBS3DCPNum(src,0)*zNURBS3DCPNum(src,1)*sizeof(zNURBS3DCPCell) );
  return dest;
}

/* clone a NURBS curve / surface. */
zNURBS3D *zNURBS3DClone(zNURBS3D *src, zNURBS3D *dest)
{
  if( !zNURBS3DAlloc( dest, zNURBS3DCPNum(src,0), zNURBS3DCPNum(src,1), src->dim[0], src->dim[1] ) )
    return NULL;
  return zNURBS3DCopy( src, dest );
}

/* mirror a NURBS curve / surface about specified axis. */
zNURBS3D *zNURBS3DMirror(zNURBS3D *src, zNURBS3D *dest, zAxis axis)
{
  register int i, j;

  if( !zNURBS3DCopy( src, dest ) ) return NULL;
  for( i=0; i<zNURBS3DCPNum(dest,0); i++ )
    for( j=0; j<zNURBS3DCPNum(dest,1); j++ )
      zNURBS3DCP(dest,i,j)->e[axis] *= -1;
  return dest;
}

/* transfer control points of a NURBS curve / surface. */
zNURBS3D *zNURBS3DXfer(zNURBS3D *src, zFrame3D *f, zNURBS3D *dest)
{
  register int i, j;

  if( !zNURBS3DCopy( src, dest ) ) return NULL;
  for( i=0; i<zNURBS3DCPNum(dest,0); i++ )
    for( j=0; j<zNURBS3DCPNum(dest,1); j++ )
      zXfer3D( f, zNURBS3DCP(src,i,j), zNURBS3DCP(dest,i,j) );
  return dest;
}

/* inversely transfer control points of a NURBS curve / surface. */
zNURBS3D *zNURBS3DXferInv(zNURBS3D *src, zFrame3D *f, zNURBS3D *dest)
{
  register int i, j;

  if( !zNURBS3DCopy( src, dest ) ) return NULL;
  for( i=0; i<zNURBS3DCPNum(dest,0); i++ )
    for( j=0; j<zNURBS3DCPNum(dest,1); j++ )
      zXfer3DInv( f, zNURBS3DCP(src,i,j), zNURBS3DCP(dest,i,j) );
  return dest;
}

/* normalize knot vectors of a NURBS curve / surface. */
void zNURBS3DKnotNormalize(zNURBS3D *nurbs)
{
  zVecShift( nurbs->knot[0], -zVecElemNC(nurbs->knot[0],0) );
  zVecDivDRC( nurbs->knot[0], zVecElemNC(nurbs->knot[0],zVecSizeNC(nurbs->knot[0])-1) );
  zVecShift( nurbs->knot[1], -zVecElemNC(nurbs->knot[1],0) );
  zVecDivDRC( nurbs->knot[1], zVecElemNC(nurbs->knot[1],zVecSizeNC(nurbs->knot[1])-1) );
}

/* find a knot segment that includes the given parameter. */
int _zNURBS3DSeg(zNURBS3D *nurbs, int i, double t)
{
  register int j, k, l;

  for( j=nurbs->dim[i], k=zNURBS3DCPNum(nurbs,i); ; ){
    while( zNURBS3DKnot(nurbs,i,j+1) == zNURBS3DKnot(nurbs,i,j) ) j++;
    while( zNURBS3DKnot(nurbs,i,k-1) == zNURBS3DKnot(nurbs,i,k) ) k--;
    if( k <= j + 1 ) break;
    l = ( j + k ) / 2;
    if( zNURBS3DKnot(nurbs,i,l) > t )
      k = l;
    else
      j = l;
  }
  return j;
}

/* basis function of NURBS. */
double _zNURBS3DBasis(zNURBS3D *nurbs, int i, double t, int j, int r, int seg)
{
  double t1, tr1, b = 0;

  if( r == 0 )
    return j == seg ? 1 : 0;
  if( j >= seg - r ){
    t1  = zNURBS3DKnot(nurbs,i,j);
    tr1 = zNURBS3DKnot(nurbs,i,j+r);
    if( tr1 != t1 )
      b += ( t - t1 ) / ( tr1 - t1 ) * _zNURBS3DBasis(nurbs,i,t,j,r-1,seg);
  }
  if( j <= seg ){
    t1  = zNURBS3DKnot(nurbs,i,j+1);
    tr1 = zNURBS3DKnot(nurbs,i,j+r+1);
    if( tr1 != t1 )
      b += ( tr1 - t ) / ( tr1 - t1 ) * _zNURBS3DBasis(nurbs,i,t,j+1,r-1,seg);
  }
  return b;
}

/* compute a vector on a NURBS curve / surface. */
zVec3D *zNURBS3DVec(zNURBS3D *nurbs, double u, double v, zVec3D *p)
{
  register int su, sv, i, j;
  double bu, bv, dv, den;
  zVec3D tmp;

  su = _zNURBS3DSeg( nurbs, 0, u );
  sv = _zNURBS3DSeg( nurbs, 1, v );
  zVec3DClear( p );
  for( den=0, i=su-nurbs->dim[0]; i<=su; i++ ){
    bu = _zNURBS3DBasis(nurbs,0,u,i,nurbs->dim[0],su);
    zVec3DClear( &tmp );
    for( dv=0, j=sv-nurbs->dim[1]; j<=sv; j++ ){
      dv += bv = zNURBS3DWeight(nurbs,i,j) * _zNURBS3DBasis(nurbs,1,v,j,nurbs->dim[1],sv);
      zVec3DCatDRC( &tmp, bv, zNURBS3DCP(nurbs,i,j) );
    }
    zVec3DCatDRC( p, bu, &tmp );
    den += bu * dv;
  }
  return zIsTiny(den) ? NULL : zVec3DDivDRC( p, den );
}

/* derivative of the basis function of NURBS. */
double _zNURBS3DBasisDiff(zNURBS3D *nurbs, int i, double t, int j, int r, int seg)
{
  double dt, b = 0;

  if( j >= seg - r && ( dt = zNURBS3DKnot(nurbs,i,j+r) - zNURBS3DKnot(nurbs,i,j) ) != 0 )
    b += _zNURBS3DBasis(nurbs,i,t,j,r-1,seg) / dt;
  if( j <= seg && ( dt = zNURBS3DKnot(nurbs,i,j+r+1) - zNURBS3DKnot(nurbs,i,j+1) ) != 0 )
    b -= _zNURBS3DBasis(nurbs,i,t,j+1,r-1,seg) / dt;
  return b * r;
}

/* compute a position and normal vectors on NURBS surface. */
zVec3D *zNURBS3DVecNorm(zNURBS3D *nurbs, double u, double v, zVec3D *p, zVec3D *n, zVec3D *t1, zVec3D *t2)
{
  register int su, sv, i, j;
  double bu, bv, dbu, dbv, dv, ddv, dubv, budv, den;
  zVec3D *tu, *tv, tu_tmp, tv_tmp, pv_tmp, dpv_tmp;

  tu = t1 ? t1 : &tu_tmp;
  tv = t2 ? t2 : &tv_tmp;
  su = _zNURBS3DSeg( nurbs, 0, u );
  sv = _zNURBS3DSeg( nurbs, 1, v );
  zVec3DClear( p );
  zVec3DClear( tu );
  zVec3DClear( tv );
  for( den=dubv=budv=0, i=su-nurbs->dim[0]; i<=su; i++ ){
    bu = _zNURBS3DBasis(nurbs,0,u,i,nurbs->dim[0],su);
    dbu = _zNURBS3DBasisDiff(nurbs,0,u,i,nurbs->dim[0],su);
    zVec3DClear( &pv_tmp );
    zVec3DClear( &dpv_tmp );
    for( dv=ddv=0, j=sv-nurbs->dim[1]; j<=sv; j++ ){
      dv += bv = zNURBS3DWeight(nurbs,i,j) * _zNURBS3DBasis(nurbs,1,v,j,nurbs->dim[1],sv);
      ddv += dbv = zNURBS3DWeight(nurbs,i,j) * _zNURBS3DBasisDiff(nurbs,1,v,j,nurbs->dim[1],sv);
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

#define ZNURBS3D_CLOSEST_GAIN 0.1
/* closest point of a 3D point on a NURBS surface. */
double zNURBS3DClosest(zNURBS3D *nurbs, zVec3D *p, zVec3D *nn, double *u, double *v)
{
  double utmp, vtmp, u_prev, v_prev, duu, duv, dvv, eu, ev, eval, eval_prev, gain;
  zVec3D du, dv, e;
  register int i;
  int iter = 0;

  if( !u ) u = &utmp;
  if( !v ) v = &vtmp;
  *u = u_prev = 0.5 * ( zNURBS3DKnotS(nurbs,0) + zNURBS3DKnotE(nurbs,0) );
  *v = v_prev = 0.5 * ( zNURBS3DKnotS(nurbs,1) + zNURBS3DKnotE(nurbs,1) );
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
    gain = ZNURBS3D_CLOSEST_GAIN / ( duu * dvv - duv * duv );
    *u += ( ( dvv + eval ) * eu - duv * ev ) * gain;
    *v += ( ( duu + eval ) * ev - duv * eu ) * gain;
    *u = zLimit( *u, zNURBS3DKnotS(nurbs,0), zNURBS3DKnotE(nurbs,0) );
    *v = zLimit( *v, zNURBS3DKnotS(nurbs,1), zNURBS3DKnotE(nurbs,1) );
    if( ( zIsTiny( *u - u_prev ) && zIsTiny( *v - v_prev ) ) || zIsTiny( eval - eval_prev ) ) break;
    u_prev = *u;
    v_prev = *v;
    eval_prev = eval;
  }
  return zVec3DDist( p, nn );
}

/* convert NURBS surface to a polyhedron. */
zPH3D *zNURBS3DToPH(zNURBS3D *nurbs, zPH3D *ph)
{
  zVec3D *vert, *vert2;
  zTri3D *face;
  register int i, j;
  double u, v;

  if( !zPH3DAlloc( ph,
      ( nurbs->ns[0] + 1 ) * ( nurbs->ns[1] + 1 ), nurbs->ns[0] * nurbs->ns[1] * 2 ) )
    return NULL;

  vert = zPH3DVertBuf( ph );
  for( i=0; i<=nurbs->ns[0]; i++ ){
    u = zNURBS3DKnotSlice( nurbs, 0, i );
    for( j=0; j<=nurbs->ns[1]; j++ ){
      v = zNURBS3DKnotSlice( nurbs, 1, j );
      if( zNURBS3DVec( nurbs, u, v, vert ) ) vert++;
    }
  }
  face = zPH3DFaceBuf( ph );
  vert = zPH3DVertBuf( ph );
  vert2 = vert + nurbs->ns[1] + 1;
  for( i=0; i<nurbs->ns[0]; i++, vert+=nurbs->ns[1]+1, vert2+=nurbs->ns[1]+1 )
    for( j=0; j<nurbs->ns[1]; j++ ){
      zTri3DCreate( face++, &vert[j], &vert2[j], &vert2[j+1] );
      zTri3DCreate( face++, &vert[j], &vert2[j+1], &vert[j+1] );
    }
  return ph;
}

static bool _zNURBS3DFRead(FILE *fp, void *instance, char *buf, bool *success);

/* read a 3D NURBS surface from file. */
bool _zNURBS3DFRead(FILE *fp, void *instance, char *buf, bool *success)
{
  zNURBS3D *nurbs;
  int size1, size2;
  int i, j;

  nurbs = instance;
  if( strcmp( buf, "dim" ) == 0 ){
    nurbs->dim[0] = zFInt( fp );
    nurbs->dim[1] = zFInt( fp );
  } else if( strcmp( buf, "uknot" ) == 0 ){
    if( nurbs->knot[0] ){
      ZRUNWARN( ZEO_ERR_NURBS_KNOTALREADY );
      zVecFree( nurbs->knot[0] );
    }
    if( !( nurbs->knot[0] = zVecFRead( fp ) ) ) return false;
  } else if( strcmp( buf, "vknot" ) == 0 ){
    if( nurbs->knot[1] ){
      ZRUNWARN( ZEO_ERR_NURBS_KNOTALREADY );
      zVecFree( nurbs->knot[1] );
    }
    if( !( nurbs->knot[1] = zVecFRead( fp ) ) ) return false;
  } else if( strcmp( buf, "slice" ) == 0 ){
    nurbs->ns[0] = zFInt( fp );
    nurbs->ns[1] = zFInt( fp );
  } else if( strcmp( buf, "size" ) == 0 ){
    if( zNURBS3DCPNum(nurbs,0) > 0 || zNURBS3DCPNum(nurbs,1) > 0 ){
      ZRUNWARN( ZEO_ERR_NURBS_CPALREADY );
      zArray2Free( &nurbs->cpnet );
    }
    size1 = zFInt( fp );
    size2 = zFInt( fp );
    zArray2Alloc( &nurbs->cpnet, zNURBS3DCPCell, size1, size2 );
    if( !zArray2Buf(&nurbs->cpnet) ){
      ZALLOCERROR();
      return false;
    }
  } else if( strcmp( buf, "cp" ) == 0 ){
    i = zFInt( fp );
    j = zFInt( fp );
    if( i < 0 || i > zNURBS3DCPNum(nurbs,0) || j < 0 || j > zNURBS3DCPNum(nurbs,1) ){
      ZRUNERROR( ZEO_ERR_NURBS_INVCP );
      return false;
    }
    zNURBS3DSetWeight( nurbs, i, j, zFDouble(fp) );
    zVec3DFRead( fp, zNURBS3DCP(nurbs,i,j) );
  } else
    return false;
  return true;
}

/* read a 3D NURBS surface from file. */
zNURBS3D *zNURBS3DFRead(FILE *fp, zNURBS3D *nurbs)
{
  zNURBS3DInit( nurbs );
  if( zFieldFRead( fp, _zNURBS3DFRead, nurbs ) ) return nurbs;
  zNURBS3DDestroy( nurbs );
  return NULL;
}

/* write a 3D NURBS surface to file. */
void zNURBS3DFWrite(FILE *fp, zNURBS3D *nurbs)
{
  register int i, j;

  fprintf( fp, "dim: %d %d\n", nurbs->dim[0], nurbs->dim[1] );
  if( zVecSizeNC(nurbs->knot[0]) > 0 ){
    fprintf( fp, "uknot: " );
    zVecFWrite( fp, nurbs->knot[0] );
  }
  if( zVecSizeNC(nurbs->knot[1]) > 0 ){
    fprintf( fp, "vknot: " );
    zVecFWrite( fp, nurbs->knot[1] );
  }
  fprintf( fp, "slice: %d %d\n", nurbs->ns[0], nurbs->ns[1] );
  fprintf( fp, "size: %d %d\n", zNURBS3DCPNum(nurbs,0), zNURBS3DCPNum(nurbs,1) );
  for( i=0; i<zNURBS3DCPNum(nurbs,0); i++ )
    for( j=0; j<zNURBS3DCPNum(nurbs,1); j++ ){
      fprintf( fp, "cp: %d %d %.12g ", i, j, zNURBS3DWeight(nurbs,i,j) );
      zVec3DFWrite( fp, zNURBS3DCP(nurbs,i,j) );
    }
}
