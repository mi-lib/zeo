/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_ecyl - 3D shapes: elliptic cylinder.
 */

#include <zeo/zeo_shape3d.h>

/* ********************************************************** */
/* CLASS: zECyl3D
 * 3D elliptic cylinder class
 * ********************************************************** */

/* create a 3D elliptic cylinder. */
zECyl3D *zECyl3DCreate(zECyl3D *cyl, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div)
{
  zECyl3DSetCenter( cyl, 0, c1 );
  zECyl3DSetCenter( cyl, 1, c2 );
  zECyl3DSetRadius( cyl, 0, r1 );
  zECyl3DSetRadius( cyl, 1, r2 );
  zECyl3DDefAxis( cyl, ref );
  zECyl3DSetDiv( cyl, div == 0 ? ZEO_SHAPE_DEFAULT_DIV : div );
  return cyl;
}

/* initialize a 3D elliptic cylinder. */
zECyl3D *zECyl3DInit(zECyl3D *cyl)
{
  zVec3DZero( zECyl3DCenter(cyl,0) );
  zVec3DZero( zECyl3DCenter(cyl,1) );
  zECyl3DSetRadius( cyl, 0, 0 );
  zECyl3DSetRadius( cyl, 1, 0 );
  zVec3DZero( zECyl3DRadVec(cyl,0) );
  zVec3DZero( zECyl3DRadVec(cyl,1) );
  zECyl3DSetDiv( cyl, ZEO_SHAPE_DEFAULT_DIV );
  return cyl;
}

/* allocate memory for a 3D elliptic cylinder. */
ZDEF_ALLOC_FUNCTION( zECyl3D )

/* copy a 3D elliptic cylinder to another. */
zECyl3D *zECyl3DCopy(zECyl3D *src, zECyl3D *dest)
{
  zVec3DCopy( zECyl3DCenter(src,0), zECyl3DCenter(dest,0) );
  zVec3DCopy( zECyl3DCenter(src,1), zECyl3DCenter(dest,1) );
  zECyl3DSetRadius( dest, 0, zECyl3DRadius(src,0) );
  zECyl3DSetRadius( dest, 1, zECyl3DRadius(src,1) );
  zVec3DCopy( zECyl3DRadVec(src,0), zECyl3DRadVec(dest,0) );
  zVec3DCopy( zECyl3DRadVec(src,1), zECyl3DRadVec(dest,1) );
  zECyl3DSetDiv( dest, zECyl3DDiv(src) );
  return dest;
}

/* mirror a 3D elliptic cylinder. */
zECyl3D *zECyl3DMirror(zECyl3D *src, zECyl3D *dest, zAxis axis)
{
  zECyl3DCopy( src, dest );
  zECyl3DCenter(dest,0)->e[(int)axis] *= -1;
  zECyl3DCenter(dest,1)->e[(int)axis] *= -1;
  zECyl3DRadVec(dest,0)->e[(int)axis] *= -1;
  zECyl3DRadVec(dest,1)->e[(int)axis] *= -1;
  zVec3DRevDRC( zECyl3DRadVec(dest,1) );
  return dest;
}

/* define orthonormal axes of the bottom face of a 3D elliptic cylinder. */
void zECyl3DDefAxis(zECyl3D *cyl, zVec3D *ref)
{
  zVec3D axis;

  zECyl3DAxis( cyl, &axis );
  if( ref && !zVec3DIsTiny(ref) ){
    zVec3DOrthogonalize( ref, &axis, zECyl3DRadVec(cyl,0) );
    zVec3DNormalizeDRC( zECyl3DRadVec(cyl,0) );
    zVec3DOuterProd( &axis, zECyl3DRadVec(cyl,0), zECyl3DRadVec(cyl,1) );
    zVec3DNormalizeDRC( zECyl3DRadVec(cyl,1) );
  } else{
    zVec3DOrthoSpace( &axis, zECyl3DRadVec(cyl,0), zECyl3DRadVec(cyl,1) );
  }
}

/* transform coordinates of a 3D elliptic cylinder. */
zECyl3D *zECyl3DXform(zECyl3D *src, zFrame3D *f, zECyl3D *dest)
{
  zXform3D( f, zECyl3DCenter(src,0), zECyl3DCenter(dest,0) );
  zXform3D( f, zECyl3DCenter(src,1), zECyl3DCenter(dest,1) );
  zECyl3DSetRadius( dest, 0, zECyl3DRadius(src,0) );
  zECyl3DSetRadius( dest, 1, zECyl3DRadius(src,1) );
  zMulMat3DVec3D( zFrame3DAtt(f), zECyl3DRadVec(src,0), zECyl3DRadVec(dest,0) );
  zMulMat3DVec3D( zFrame3DAtt(f), zECyl3DRadVec(src,1), zECyl3DRadVec(dest,1) );
  zECyl3DSetDiv( dest, zECyl3DDiv(src) );
  return dest;
}

/* inversely transform coordinates of a 3D elliptic cylinder. */
zECyl3D *zECyl3DXformInv(zECyl3D *src, zFrame3D *f, zECyl3D *dest)
{
  zXform3DInv( f, zECyl3DCenter(src,0), zECyl3DCenter(dest,0) );
  zXform3DInv( f, zECyl3DCenter(src,1), zECyl3DCenter(dest,1) );
  zECyl3DSetRadius( dest, 0, zECyl3DRadius(src,0) );
  zECyl3DSetRadius( dest, 1, zECyl3DRadius(src,1) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zECyl3DRadVec(src,0), zECyl3DRadVec(dest,0) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zECyl3DRadVec(src,1), zECyl3DRadVec(dest,1) );
  zECyl3DSetDiv( dest, zECyl3DDiv(src) );
  return dest;
}

/* the closest point to a 3D elliptic cylinder. */
double zECyl3DClosest(zECyl3D *cyl, zVec3D *p, zVec3D *cp)
{
  zEllips2D ellips2d;
  zVec3D v, axis, vs, vc;

  zVec3DSub( p, zECyl3DCenter(cyl,0), &v );
  zECyl3DAxis( cyl, &axis );
  zVec3DCatRatio( zECyl3DRadVec(cyl,0), zECyl3DRadVec(cyl,1), &axis, &v, vs.e );
  zEllips2DCreate( &ellips2d, ZVEC2DZERO, zECyl3DRadius(cyl,0), zECyl3DRadius(cyl,1) );
  zEllips2DClosest( &ellips2d, (zVec2D*)vs.e, (zVec2D*)vc.e );
  vc.e[zZ] = zLimit( vs.e[zZ], 0, 1 );
  zVec3DCat( zECyl3DCenter(cyl,0), vc.e[zZ], &axis, cp );
  zVec3DCatDRC( cp, vc.e[zX], zECyl3DRadVec(cyl,0) );
  zVec3DCatDRC( cp, vc.e[zY], zECyl3DRadVec(cyl,1) );
  return zVec3DDist( cp, p );
}

/* distance from a point to a 3D elliptic cylinder. */
double zECyl3DDistFromPoint(zECyl3D *cyl, zVec3D *p)
{
  zVec3D cp;

  return zECyl3DClosest( cyl, p, &cp );
}

/* check if a point is inside of an elliptic cylinder. */
bool zECyl3DPointIsInside(zECyl3D *ecyl, zVec3D *p, double margin)
{
  zEllips2D ellips2d;
  zVec3D v, axis, vs;
  double l;

  zVec3DSub( p, zECyl3DCenter(ecyl,0), &v );
  zECyl3DAxis( ecyl, &axis );
  l = zVec3DNormalizeDRC( &axis );
  zVec3DCatRatio( zECyl3DRadVec(ecyl,0), zECyl3DRadVec(ecyl,1), &axis, &v, vs.e );
  if( vs.c.z <= -margin || vs.c.z >= l + margin ) return false;
  zEllips2DCreate( &ellips2d, ZVEC2DZERO, zECyl3DRadius(ecyl,0), zECyl3DRadius(ecyl,1) );
  return zEllips2DPointIsInside( &ellips2d, (zVec2D*)vs.e, margin );
}

/* height of a 3D elliptic cylinder. */
double zECyl3DHeight(zECyl3D *cyl)
{
  zVec3D axis;

  return zVec3DNorm( zECyl3DAxis( cyl, &axis ) );
}

/* volume of a 3D elliptic cylinder. */
double zECyl3DVolume(zECyl3D *cyl)
{
  return zPI * zECyl3DRadius(cyl,0) * zECyl3DRadius(cyl,1) * zECyl3DHeight(cyl);
}

/* barycenter of a 3D elliptic cylinder. */
zVec3D *zECyl3DBarycenter(zECyl3D *cyl, zVec3D *c)
{
  return zVec3DMid( zECyl3DCenter(cyl,0), zECyl3DCenter(cyl,1), c );
}

/* inertia tensor about barycenter of a 3D elliptic cylinder from mass. */
zMat3D *zECyl3DBaryInertiaMass(zECyl3D *cyl, double mass, zMat3D *inertia)
{
  double aa, bb, hh;
  zMat3D i, att;

  /* aligned inertia tensor */
  aa = 0.25 * mass * zSqr( zECyl3DRadius(cyl,0) );
  bb = 0.25 * mass * zSqr( zECyl3DRadius(cyl,1) );
  zECyl3DAxis( cyl, &att.b.z );
  hh = zVec3DSqrNorm(&att.b.z) * mass / 12.0;
  zMat3DCreate( &i,
    bb+hh, 0, 0,
    0, aa+hh, 0,
    0, 0, aa+bb );
  /* rotate */
  zVec3DCopy( zECyl3DRadVec(cyl,0), &att.b.x );
  zVec3DCopy( zECyl3DRadVec(cyl,1), &att.b.y );
  zVec3DNormalizeDRC( &att.b.z );
  return zRotMat3D( &att, &i, inertia );
}

/* inertia tensor about barycenter of a 3D elliptic cylinder. */
zMat3D *zECyl3DBaryInertia(zECyl3D *cyl, double density, zMat3D *inertia)
{
  return zECyl3DBaryInertiaMass( cyl, density * zECyl3DVolume( cyl ), inertia );
}

/* convert an elliptic cylinder to a polyhedron. */
zPH3D *zECyl3DToPH(zECyl3D *cyl, zPH3D *ph)
{
  zVec3D *vert, d, r;
  zTri3D *face;
  double s, c;
  int i, j, n;

  if( !zPH3DAlloc( ph, zECyl3DDiv(cyl)*2, (zECyl3DDiv(cyl)-1)*4 ) )
    return NULL;
  vert = zPH3DVertBuf(ph);
  face = zPH3DFaceBuf(ph);

  zECyl3DAxis( cyl, &d );
  if( zVec3DNormalizeDRC( &d ) < 0 ){
    ZRUNERROR( ZEO_ERR_SHAPE_ZEROC );
    return NULL;
  }
  /* create vertices */
  for( i=0; i<zECyl3DDiv(cyl); i++ ){
    zSinCos( zPIx2 * i / zECyl3DDiv(cyl), &s, &c );
    zVec3DMul( zECyl3DRadVec(cyl,0), zECyl3DRadius(cyl,0)*c, &r );
    zVec3DCatDRC( &r, zECyl3DRadius(cyl,1)*s, zECyl3DRadVec(cyl,1) );
    /* vertices on the top rim */
    zVec3DAdd( zECyl3DCenter(cyl,0), &r, &vert[i] );
    /* vertices on the bottom rim */
    zVec3DAdd( zECyl3DCenter(cyl,1), &r, &vert[i+zECyl3DDiv(cyl)] );
  }
  /* -- faces -- */
  /* base 1 */
  for( n=0, i=2; i<zECyl3DDiv(cyl); i++ )
    zTri3DCreate( &face[n++], &vert[0], &vert[i], &vert[i-1] );
  /* side faces */
  for( i=0, j=zECyl3DDiv(cyl)-1; i<zECyl3DDiv(cyl); j=i++ ){
    zTri3DCreate( &face[n++], &vert[j], &vert[i], &vert[i+zECyl3DDiv(cyl)] );
    zTri3DCreate( &face[n++], &vert[j], &vert[i+zECyl3DDiv(cyl)], &vert[j+zECyl3DDiv(cyl)] );
  }
  /* base 2 */
  for( i=zPH3DVertNum(ph)-zECyl3DDiv(cyl); i<zPH3DVertNum(ph)-2; i++ )
    zTri3DCreate( &face[n++],
      &vert[zPH3DVertNum(ph)-1], &vert[i], &vert[i+1] );

  return ph;
}

/* parse ZTK format */

static void *_zECyl3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zECyl3DCenter((zECyl3D*)obj,i), ztk );
  return obj; }
static void *_zECyl3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zECyl3DRadius((zECyl3D*)obj,i) = ZTKDouble(ztk);
  return obj; }
static void *_zECyl3DRefFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3D ref;
  zVec3DFromZTK( &ref, ztk );
  zECyl3DDefAxis( (zECyl3D*)obj, &ref );
  return obj;
}
static void *_zECyl3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zECyl3DDiv((zECyl3D*)obj) = zShape3DDivFromZTK(ztk);
  return obj; }

static void _zECyl3DCenterFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zECyl3DCenter((zECyl3D*)obj,i) ); }
static void _zECyl3DRadiusFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zECyl3DRadius((zECyl3D*)obj,i) ); }
static void _zECyl3DRefFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zECyl3DRadVec((zECyl3D*)obj,0) ); }
static void _zECyl3DDivFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zECyl3DDiv((zECyl3D*)obj) ); }

static ZTKPrp __ztk_prp_shape_ecyl[] = {
  { "center", 2, _zECyl3DCenterFromZTK, _zECyl3DCenterFPrintZTK },
  { "radius", 2, _zECyl3DRadiusFromZTK, _zECyl3DRadiusFPrintZTK },
  { "ref", 1, _zECyl3DRefFromZTK, _zECyl3DRefFPrintZTK },
  { "div", 1, _zECyl3DDivFromZTK, _zECyl3DDivFPrintZTK },
};

/* print a 3D elliptic cylinder out to a file in a ZTK format. */
void zECyl3DFPrintZTK(FILE *fp, zECyl3D *ecyl)
{
  ZTKPrpKeyFPrint( fp, ecyl, __ztk_prp_shape_ecyl );
}

/* methods for abstraction */

static void *_zShape3DECylInit(void *body){
  return zECyl3DInit( (zECyl3D*)body ); }
static void *_zShape3DECylAlloc(void){
  return zECyl3DAlloc(); }
static void *_zShape3DECylClone(void *src){
  zECyl3D *cln;
  return ( cln = zECyl3DAlloc() ) ? zECyl3DCopy( (zECyl3D*)src, cln ) : NULL; }
static void *_zShape3DECylMirror(void *src, zAxis axis){
  zECyl3D *mrr;
  return ( mrr = zECyl3DAlloc() ) ? zECyl3DMirror( (zECyl3D*)src, mrr, axis ) : NULL; }
static void _zShape3DECylDestroy(void *body){}
static void *_zShape3DECylXform(void *src, zFrame3D *f, void *dest){
  return zECyl3DXform( (zECyl3D*)src, f, (zECyl3D*)dest ); }
static void *_zShape3DECylXformInv(void *src, zFrame3D *f, void *dest){
  return zECyl3DXformInv( (zECyl3D*)src, f, (zECyl3D*)dest ); }
static double _zShape3DECylClosest(void *body, zVec3D *p, zVec3D *cp){
  return zECyl3DClosest( (zECyl3D*)body, p, cp ); }
static double _zShape3DECylDistFromPoint(void *body, zVec3D *p){
  return zECyl3DDistFromPoint( (zECyl3D*)body, p ); }
static bool _zShape3DECylPointIsInside(void *body, zVec3D *p, double margin){
  return zECyl3DPointIsInside( (zECyl3D*)body, p, margin ); }
static double _zShape3DECylVolume(void *body){
  return zECyl3DVolume( (zECyl3D*)body ); }
static zVec3D *_zShape3DECylBarycenter(void *body, zVec3D *c){
  return zECyl3DBarycenter( (zECyl3D*)body, c ); }
static zMat3D *_zShape3DECylBaryInertiaMass(void *body, double mass, zMat3D *i){
  return zECyl3DBaryInertiaMass( (zECyl3D*)body, mass, i ); }
static zMat3D *_zShape3DECylBaryInertia(void *body, double density, zMat3D *i){
  return zECyl3DBaryInertia( (zECyl3D*)body, density, i ); }
static zPH3D *_zShape3DECylToPH(void *body, zPH3D *ph){
  return zECyl3DToPH( (zECyl3D*)body, ph ); }
static void *_zShape3DECylParseZTK(void *body, ZTK *ztk){
  zECyl3DInit( (zECyl3D*)body );
  return ZTKEvalKey( body, NULL, ztk, __ztk_prp_shape_ecyl ); }
static void _zShape3DECylFPrintZTK(FILE *fp, void *body){
  zECyl3DFPrintZTK( fp, (zECyl3D*)body ); }

zShape3DCom zeo_shape3d_ecyl_com = {
  "ellipticcylinder",
  _zShape3DECylInit,
  _zShape3DECylAlloc,
  _zShape3DECylClone,
  _zShape3DECylMirror,
  _zShape3DECylDestroy,
  _zShape3DECylXform,
  _zShape3DECylXformInv,
  _zShape3DECylClosest,
  _zShape3DECylDistFromPoint,
  _zShape3DECylPointIsInside,
  _zShape3DECylVolume,
  _zShape3DECylBarycenter,
  _zShape3DECylBaryInertiaMass,
  _zShape3DECylBaryInertia,
  _zShape3DECylToPH,
  _zShape3DECylParseZTK,
  _zShape3DECylFPrintZTK,
};

/* create a 3D shape as an elliptic cylinder. */
zShape3D *zShape3DECylCreate(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zECyl3DAlloc() ) ) return NULL;
  zECyl3DCreate( zShape3DECyl(shape), c1, c2, r1, r2, ref, div );
  shape->com = &zeo_shape3d_ecyl_com;
  return shape;
}
