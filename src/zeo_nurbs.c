/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs - NURBS curve / surface
 */

#include <zeo/zeo_nurbs.h>

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
  memcpy( zArray2Buf(&dest->cpnet), zArray2Buf(&src->cpnet), zNURBS3DCPNum(src,0)*zNURBS3DCPNum(src,1)*sizeof(zNURBS3DCPCell) );
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

/* compute a position and normal vectors on NURBS surface. */
zVec3D *zNURBS3DVecTSpace(const zNURBS3D *nurbs, double u, double v, zVec3D *p, zVec3D *n, zVec3D *t1, zVec3D *t2)
{
  int su, sv, i, j;
  double bu, bv, dbu, dbv, dv, ddv, dubv, budv, den;
  zVec3D *tu, *tv, tu_tmp, tv_tmp, pv_tmp, dpv_tmp;

  tu = t1 ? t1 : &tu_tmp;
  tv = t2 ? t2 : &tv_tmp;
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
double zNURBS3DClosest(const zNURBS3D *nurbs, const zVec3D *p, zVec3D *nn, double *u, double *v)
{
  double utmp, vtmp, u_prev, v_prev, duu, duv, dvv, eu, ev, eval, eval_prev, gain;
  zVec3D du, dv, e;
  int i, iter = 0;

  if( !u ) u = &utmp;
  if( !v ) v = &vtmp;
  *u = u_prev = 0.5 * ( zNURBS3DKnotS(nurbs,0) + zNURBS3DKnotE(nurbs,0) );
  *v = v_prev = 0.5 * ( zNURBS3DKnotS(nurbs,1) + zNURBS3DKnotE(nurbs,1) );
  ZITERINIT( iter );
  for( eval_prev=HUGE_VAL, i=0; i<iter; i++ ){
    zNURBS3DVecTan( nurbs, *u, *v, nn, &du, &dv );
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
  int i;

  if( !ZTKValRewind(ztk) ) return NULL;
  if( nurbs->param[id].knot ){
    ZRUNWARN( ZEO_ERR_NURBS_KNOTALREADY );
    zVecFree( nurbs->param[id].knot );
  }
  if( !( nurbs->param[id].knot = zVecAlloc( ZTKInt(ztk) ) ) ) return NULL;
  for( i=0; i<zNURBS3DKnotNum(nurbs,id); i++ )
    zNURBS3DKnot(nurbs,id,i) = ZTKDouble(ztk);
  return nurbs->param[id].knot;
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

static bool _zNURBS3DUKnotFPrint(FILE *fp, int i, void *obj){
  if( zVecSizeNC(((zNURBS3D*)obj)->param[0].knot) <= 0 ) return false;
  zVecFPrint( fp, ((zNURBS3D*)obj)->param[0].knot );
  return true;
}
static bool _zNURBS3DVKnotFPrint(FILE *fp, int i, void *obj){
  if( zVecSizeNC(((zNURBS3D*)obj)->param[1].knot) <= 0 ) return false;
  zVecFPrint( fp, ((zNURBS3D*)obj)->param[1].knot );
  return true;
}
static bool _zNURBS3DSliceFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d %d\n", zNURBS3DSlice((zNURBS3D*)obj,0), zNURBS3DSlice((zNURBS3D*)obj,1) );
  return true;
}
static bool _zNURBS3DSizeFPrint(FILE *fp, int i, void *obj){
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
  if( !ZTKEvalKey( nurbs, NULL, ztk, __ztk_prp_nurbs ) ) return NULL;
  zNURBS3DOrder(nurbs,0) = zNURBS3DKnotNum(nurbs,0) - zNURBS3DCPNum(nurbs,0) - 1;
  zNURBS3DOrder(nurbs,1) = zNURBS3DKnotNum(nurbs,1) - zNURBS3DCPNum(nurbs,1) - 1;
  return nurbs;
}

/* print out a 3D NURBS to a file. */
void zNURBS3DFPrintZTK(FILE *fp, const zNURBS3D *nurbs)
{
  int i, j;

  ZTKPrpKeyFPrint( fp, (void*)nurbs, __ztk_prp_nurbs );
  for( i=0; i<zNURBS3DCPNum(nurbs,0); i++ )
    for( j=0; j<zNURBS3DCPNum(nurbs,1); j++ ){
      fprintf( fp, "%s: %d %d %.12g ", ZTK_KEY_ZEO_NURBS_CP, i, j, zNURBS3DWeight(nurbs,i,j) );
      zVec3DFPrint( fp, zNURBS3DCP(nurbs,i,j) );
    }
}
