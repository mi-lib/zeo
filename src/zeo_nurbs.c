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
      zVec3DZero( zNURBS3DCP(nurbs,i,j) );
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

/* transform control points of a NURBS curve / surface. */
zNURBS3D *zNURBS3DXform(zNURBS3D *src, zFrame3D *f, zNURBS3D *dest)
{
  register int i, j;

  if( !zNURBS3DCopy( src, dest ) ) return NULL;
  for( i=0; i<zNURBS3DCPNum(dest,0); i++ )
    for( j=0; j<zNURBS3DCPNum(dest,1); j++ )
      zXform3D( f, zNURBS3DCP(src,i,j), zNURBS3DCP(dest,i,j) );
  return dest;
}

/* inversely transform control points of a NURBS curve / surface. */
zNURBS3D *zNURBS3DXformInv(zNURBS3D *src, zFrame3D *f, zNURBS3D *dest)
{
  register int i, j;

  if( !zNURBS3DCopy( src, dest ) ) return NULL;
  for( i=0; i<zNURBS3DCPNum(dest,0); i++ )
    for( j=0; j<zNURBS3DCPNum(dest,1); j++ )
      zXform3DInv( f, zNURBS3DCP(src,i,j), zNURBS3DCP(dest,i,j) );
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
  zVec3DZero( p );
  for( den=0, i=su-nurbs->dim[0]; i<=su; i++ ){
    bu = _zNURBS3DBasis(nurbs,0,u,i,nurbs->dim[0],su);
    zVec3DZero( &tmp );
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
  zVec3DZero( p );
  zVec3DZero( tu );
  zVec3DZero( tv );
  for( den=dubv=budv=0, i=su-nurbs->dim[0]; i<=su; i++ ){
    bu = _zNURBS3DBasis(nurbs,0,u,i,nurbs->dim[0],su);
    dbu = _zNURBS3DBasisDiff(nurbs,0,u,i,nurbs->dim[0],su);
    zVec3DZero( &pv_tmp );
    zVec3DZero( &dpv_tmp );
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
      zVec3DZero( n );
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

/* parse ZTK format */

static zVec _zNURBS3DKnotFromZTK(zNURBS3D *nurbs, int id, ZTK *ztk)
{
  register int i;

  if( !ZTKValRewind(ztk) ) return NULL;
  if( nurbs->knot[id] ){
    ZRUNWARN( ZEO_ERR_NURBS_KNOTALREADY );
    zVecFree( nurbs->knot[id] );
  }
  if( !( nurbs->knot[id] = zVecAlloc( ZTKInt(ztk) ) ) ) return NULL;
  for( i=0; i<zNURBS3DKnotNum(nurbs,id); i++ )
    zNURBS3DKnot(nurbs,id,i) = ZTKDouble(ztk);
  return nurbs->knot[id];
}

static void *_zNURBS3DDimFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zNURBS3D*)obj)->dim[0] = ZTKInt( ztk );
  ((zNURBS3D*)obj)->dim[1] = ZTKInt( ztk );
  return obj;
}
static void *_zNURBS3DUKnotFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return _zNURBS3DKnotFromZTK( obj, 0, ztk ) ? obj : NULL;
}
static void *_zNURBS3DVKnotFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return _zNURBS3DKnotFromZTK( obj, 1, ztk ) ? obj : NULL;
}
static void *_zNURBS3DSliceFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zNURBS3D*)obj)->ns[0] = ZTKInt( ztk );
  ((zNURBS3D*)obj)->ns[1] = ZTKInt( ztk );
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
    ZRUNERROR( ZEO_ERR_NURBS_INVCP );
    return NULL;
  }
  zNURBS3DSetWeight( ((zNURBS3D*)obj), j, k, ZTKDouble(ztk) );
  zVec3DFromZTK( zNURBS3DCP(((zNURBS3D*)obj),j,k), ztk );
  return obj;
}

static void _zNURBS3DDimFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d %d\n", ((zNURBS3D*)obj)->dim[0], ((zNURBS3D*)obj)->dim[1] );
}
static void _zNURBS3DUKnotFPrint(FILE *fp, int i, void *obj){
  if( zVecSizeNC(((zNURBS3D*)obj)->knot[0]) > 0 ) zVecFPrint( fp, ((zNURBS3D*)obj)->knot[0] );
}
static void _zNURBS3DVKnotFPrint(FILE *fp, int i, void *obj){
  if( zVecSizeNC(((zNURBS3D*)obj)->knot[1]) > 0 ) zVecFPrint( fp, ((zNURBS3D*)obj)->knot[1] );
}
static void _zNURBS3DSliceFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d %d\n", ((zNURBS3D*)obj)->ns[0], ((zNURBS3D*)obj)->ns[1] );
}
static void _zNURBS3DSizeFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d %d\n", zNURBS3DCPNum((zNURBS3D*)obj,0), zNURBS3DCPNum((zNURBS3D*)obj,1) );
}

static ZTKPrp __ztk_prp_nurbs[] = {
  { "dim", 1, _zNURBS3DDimFromZTK, _zNURBS3DDimFPrint },
  { "uknot", 1, _zNURBS3DUKnotFromZTK, _zNURBS3DUKnotFPrint },
  { "vknot", 1, _zNURBS3DVKnotFromZTK, _zNURBS3DVKnotFPrint },
  { "size", 1, _zNURBS3DSizeFromZTK, _zNURBS3DSizeFPrint },
  { "cp", -1, _zNURBS3DCPFromZTK, NULL },
  { "slice", 1, _zNURBS3DSliceFromZTK, _zNURBS3DSliceFPrint },
};

/* register a definition of tag-and-keys for a 3D NURBS to a ZTK format processor. */
bool zNURBS3DRegZTK(ZTK *ztk, char *tag)
{
  return ZTKDefRegPrp( ztk, tag, __ztk_prp_nurbs );
}

/* read a 3D NURBS from a ZTK format processor. */
zNURBS3D *zNURBS3DFromZTK(zNURBS3D *nurbs, ZTK *ztk)
{
  zNURBS3DInit( nurbs );
  return ZTKEvalKey( nurbs, NULL, ztk, __ztk_prp_nurbs );
}

/* print out a 3D NURBS to a file. */
void zNURBS3DFPrintZTK(FILE *fp, zNURBS3D *nurbs)
{
  register int i, j;

  ZTKPrpKeyFPrint( fp, nurbs, __ztk_prp_nurbs );
  for( i=0; i<zNURBS3DCPNum(nurbs,0); i++ )
    for( j=0; j<zNURBS3DCPNum(nurbs,1); j++ ){
      fprintf( fp, "cp: %d %d %.12g ", i, j, zNURBS3DWeight(nurbs,i,j) );
      zVec3DFPrint( fp, zNURBS3DCP(nurbs,i,j) );
    }
}
