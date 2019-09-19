/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_cone - 3D shapes: cone.
 */

#include <zeo/zeo_shape.h>

/* ********************************************************** */
/* CLASS: zCone3D
 * 3D cone class
 * ********************************************************** */

/* create a 3D cone. */
zCone3D *zCone3DCreate(zCone3D *cone, zVec3D *c, zVec3D *v, double r, int div)
{
  zCone3DSetCenter( cone, c );
  zCone3DSetVert( cone, v );
  zCone3DSetRadius( cone, r );
  zCone3DSetDiv( cone, div == 0 ? ZEO_SHAPE_DEFAULT_DIV : div );
  return cone;
}

/* initialize a 3D cone. */
zCone3D *zCone3DInit(zCone3D *cone)
{
  return zCone3DCreate( cone, ZVEC3DZERO, ZVEC3DZERO, 0, 0 );
}

/* allocate memory for a 3D cone. */
zCone3D *zCone3DAlloc(void)
{
  zCone3D *cone;

  if( !( cone = zAlloc( zCone3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return cone;
}

/* copy a 3D cone to another. */
zCone3D *zCone3DCopy(zCone3D *src, zCone3D *dest)
{
  return zCone3DCreate( dest,
    zCone3DCenter(src), zCone3DVert(src), zCone3DRadius(src), zCone3DDiv(src) );
}

/* mirror a 3D cone. */
zCone3D *zCone3DMirror(zCone3D *src, zCone3D *dest, zAxis axis)
{
  zCone3DCopy( src, dest );
  zCone3DCenter(dest)->e[axis] *= -1;
  zCone3DVert(dest)->e[axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D cone. */
zCone3D *zCone3DXform(zCone3D *src, zFrame3D *f, zCone3D *dest)
{
  zXform3D( f, zCone3DCenter(src), zCone3DCenter(dest) );
  zXform3D( f, zCone3DVert(src), zCone3DVert(dest) );
  zCone3DSetRadius( dest, zCone3DRadius(src) );
  zCone3DSetDiv( dest, zCone3DDiv(src) );
  return dest;
}

/* inversely transform coordinates of a 3D cone. */
zCone3D *zCone3DXformInv(zCone3D *src, zFrame3D *f, zCone3D *dest)
{
  zXform3DInv( f, zCone3DCenter(src), zCone3DCenter(dest) );
  zXform3DInv( f, zCone3DVert(src), zCone3DVert(dest) );
  zCone3DSetRadius( dest, zCone3DRadius(src) );
  zCone3DSetDiv( dest, zCone3DDiv(src) );
  return dest;
}

/* the closest point to a 3D cone. */
double zCone3DClosest(zCone3D *cone, zVec3D *p, zVec3D *cp)
{
  zVec3D axis, v, s, g;
  double l, r, d;

  zCone3DAxis( cone, &axis );
  l = zVec3DNormalizeDRC( &axis );
  zVec3DSub( p, zCone3DCenter(cone), &v );
  d = zVec3DInnerProd( &axis, &v );
  zVec3DCat( &v, -d, &axis, &s );
  r = zVec3DNormalizeDRC( &s );
  if( d < 0 ){
    if( r < zCone3DRadius(cone) ){
      zVec3DCat( p, -d, &axis, cp );
      return -d;
    }
  } else{
    if( r < l*(d-l)/zCone3DRadius(cone) ){
      zVec3DCopy( zCone3DVert(cone), cp );
      return sqrt( zSqr(d-l) + r*r );
    }
    if( d/l + r/zCone3DRadius(cone) <= 1 ){
      zVec3DCopy( p, cp );
      return 0;
    }
    if( r <= l*d/zCone3DRadius(cone) + zCone3DRadius(cone) ){
      zVec3DMul( &s, zCone3DRadius(cone), &g );
      zVec3DCatDRC( &g, -l, &axis );
      zVec3DNormalizeDRC( &g );
      zVec3DSub( p, zCone3DVert(cone), &v );
      zVec3DCat( zCone3DVert(cone), zVec3DInnerProd(&v,&g), &g, cp );
      return zVec3DDist( p, cp );
    }
  }
  zVec3DCat( zCone3DCenter(cone), zCone3DRadius(cone), &s, cp );
  return sqrt( d*d + zSqr(r-zCone3DRadius(cone)) );
}

/* distance from a point to a 3D cone. */
double zCone3DPointDist(zCone3D *cone, zVec3D *p)
{
  zVec3D axis, v, s, g;
  double l, r, d;

  zCone3DAxis( cone, &axis );
  l = zVec3DNormalizeDRC( &axis );
  zVec3DSub( p, zCone3DCenter(cone), &v );
  d = zVec3DInnerProd( &axis, &v );
  zVec3DCat( &v, -d, &axis, &s );
  r = zVec3DNormalizeDRC( &s );
  if( d < 0 ){
    if( r < zCone3DRadius(cone) ) return -d;
  } else{
    if( r < l*(d-l)/zCone3DRadius(cone) )
      return sqrt( zSqr(d-l) + r*r );
    if( d/l + r/zCone3DRadius(cone) <= 1 )
      return 0;
    if( r <= l*d/zCone3DRadius(cone) + zCone3DRadius(cone) ){
      zVec3DMul( &s, zCone3DRadius(cone), &g );
      zVec3DCatDRC( &g, -l, &axis );
      zVec3DNormalizeDRC( &g );
      zVec3DSub( p, zCone3DVert(cone), &v );
      return zVec3DOuterProdNorm( &v, &g );
    }
  }
  return sqrt( d*d + zSqr(r-zCone3DRadius(cone)) );
}

/* check if a point is inside of a cone. */
bool zCone3DPointIsInside(zCone3D *cone, zVec3D *p, bool rim)
{
  zVec3D axis, v;
  double l, d, gl;

  zCone3DAxis( cone, &axis );
  zVec3DSub( p, zCone3DVert(cone), &v );
  l = zVec3DNormalizeDRC( &axis );
  d = zVec3DInnerProd( &axis, &v );
  gl = sqrt( zSqr(l) + zSqr(zCone3DRadius(cone)) );
  if( -d / zVec3DNorm(&v) < l / gl ) return false; /* To be debugged */
  return -d <= l + ( rim ? zTOL : 0 ) ? true : false;
}

/* height of a 3D cone. */
double zCone3DHeight(zCone3D *cone)
{
  zVec3D axis;

  return zVec3DNorm( zCone3DAxis( cone, &axis ) );
}

/* volume of a 3D cone. */
double zCone3DVolume(zCone3D *cone)
{
  return zPI * zSqr(zCone3DRadius(cone)) * zCone3DHeight(cone) / 3;
}

/* barycenter of a 3D cone. */
zVec3D *zCone3DBarycenter(zCone3D *cone, zVec3D *c)
{
  return zVec3DInterDiv( zCone3DCenter(cone), zCone3DVert(cone), 0.25, c );
}

/* inertia tensor of a 3D cone. */
zMat3D *zCone3DInertia(zCone3D *cone, zMat3D *inertia)
{
  double vol, rr, hh;
  zMat3D i, att;

  vol = zCone3DVolume( cone );
  /* aligned inertia tensor */
  rr = 3 * zSqr(zCone3DRadius(cone)) * vol;
  zCone3DAxis( cone, &att.b.z );
  hh = 0.75 * zVec3DSqrNorm(&att.b.z) * vol;
  zMat3DCreate( &i,
    0.05*(rr+hh), 0, 0,
    0, 0.05*(rr+hh), 0,
    0, 0, 0.1*rr );
  /* rotate */
  zVec3DOrthoNormalSpace( &att.b.z, &att.b.x, &att.b.y );
  return zRotMat3D( &att, &i, inertia );
}

/* convert a cone to a polyhedron. */
zPH3D *zCone3DToPH(zCone3D *cone, zPH3D *ph)
{
  zVec3D *vert, d, s, r, aa;
  zTri3D *face;
  register int i, j, n;

  if( !zPH3DAlloc( ph, zCone3DDiv(cone)+1, zCone3DDiv(cone)*2-2 ) )
    return NULL;
  vert = zPH3DVertBuf( ph );
  face = zPH3DFaceBuf( ph );

  zCone3DAxis( cone, &d );
  if( zVec3DIsTiny( &d ) ){
    ZRUNERROR( ZEO_ERR_SHAPE_ZEROC );
    return NULL;
  }
  zVec3DNormalizeDRC( &d );
  /* one radial vector */
  if( !zIsTiny( d.e[zX] ) && !zIsTiny( d.e[zY] ) )
    zVec3DCreate( &s, d.e[zY],-d.e[zX], 0 );
  else
    zVec3DCreate( &s, d.e[zY]-d.e[zZ], d.e[zZ]-d.e[zX], d.e[zX]-d.e[zY] );
  zVec3DNormalizeDRC( &s );
  zVec3DMulDRC( &s, zCone3DRadius(cone) );
  /* create vertices */
  zVec3DCopy( zCone3DVert(cone), &vert[0] );
  for( i=0; i<zCone3DDiv(cone); ){
    zVec3DMul( &d, zPIx2 * i / zCone3DDiv(cone), &aa );
    zVec3DRot( &s, &aa, &r );
    /* vertices on the bottom rim */
    zVec3DAdd( zCone3DCenter(cone), &r, &vert[++i] );
  }
  /* -- faces -- */
  /* side faces */
  for( n=0, i=1, j=zCone3DDiv(cone); i<=zCone3DDiv(cone); j=i++ )
    zTri3DCreate( &face[n++], &vert[0], &vert[j], &vert[i] );
  /* base */
  for( i=3; i<=zCone3DDiv(cone); i++ )
    zTri3DCreate( &face[n++], &vert[1], &vert[i], &vert[i-1] );

  return ph;
}

/* parse ZTK format */

static void *_zCone3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zCone3DCenter((zCone3D*)obj), ztk );
  return obj;
}
static void *_zCone3DVertFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zCone3DVert((zCone3D*)obj), ztk );
  return obj;
}
static void *_zCone3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCone3DRadius((zCone3D*)obj) = ZTKDouble(ztk);
  return obj;
}
static void *_zCone3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCone3DDiv((zCone3D*)obj) = zShape3DDivFromZTK(ztk);
  return obj; }

static void _zCone3DCenterFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCone3DCenter((zCone3D*)obj) ); }
static void _zCone3DVertFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCone3DVert((zCone3D*)obj) ); }
static void _zCone3DRadiusFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zCone3DRadius((zCone3D*)obj) ); }
static void _zCone3DDivFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zCone3DDiv((zCone3D*)obj) ); }

static ZTKPrp __ztk_prp_shape_cone[] = {
  { "center", 1, _zCone3DCenterFromZTK, _zCone3DCenterFPrintZTK },
  { "vert", 1, _zCone3DVertFromZTK, _zCone3DVertFPrintZTK },
  { "radius", 1, _zCone3DRadiusFromZTK, _zCone3DRadiusFPrintZTK },
  { "div", 1, _zCone3DDivFromZTK, _zCone3DDivFPrintZTK },
};

/* methods for abstraction */

static void *_zShape3DConeInit(void* shape){
  return zCone3DInit( shape ); }
static void *_zShape3DConeAlloc(void){
  return zCone3DAlloc(); }
static void *_zShape3DConeClone(void *src){
  zCone3D *cln;
  return ( cln = zCone3DAlloc() ) ? zCone3DCopy( src, cln ) : NULL; }
static void *_zShape3DConeMirror(void *src, zAxis axis){
  zCone3D *mrr;
  return ( mrr = zCone3DAlloc() ) ? zCone3DMirror( src, mrr, axis ) : NULL; }
static void _zShape3DConeDestroy(void *shape){}
static void *_zShape3DConeXform(void *src, zFrame3D *f, void *dest){
  return zCone3DXform( src, f, dest ); }
static void *_zShape3DConeXformInv(void *src, zFrame3D *f, void *dest){
  return zCone3DXformInv( src, f, dest ); }
static double _zShape3DConeClosest(void *shape, zVec3D *p, zVec3D *cp){
  return zCone3DClosest( shape, p, cp ); }
static double _zShape3DConePointDist(void *shape, zVec3D *p){
  return zCone3DPointDist( shape, p ); }
static bool _zShape3DConePointIsInside(void *shape, zVec3D *p, bool rim){
  return zCone3DPointIsInside( shape, p, rim ); }
static double _zShape3DConeVolume(void *shape){
  return zCone3DVolume( shape ); }
static zVec3D *_zShape3DConeBarycenter(void *shape, zVec3D *c){
  return zCone3DBarycenter( shape, c ); }
static zMat3D *_zShape3DConeInertia(void *shape, zMat3D *i){
  return zCone3DInertia( shape, i ); }
static void _zShape3DConeBaryInertia(void *shape, zVec3D *c, zMat3D *i){
  zCone3DBarycenter( shape, c );
  zCone3DInertia( shape, i ); }
static zPH3D *_zShape3DConeToPH(void *shape, zPH3D *ph){
  return zCone3DToPH( shape, ph ); }
static bool _zShape3DConeRegZTK(ZTK *ztk, char *tag){
  return ZTKDefRegPrp( ztk, tag, __ztk_prp_shape_cone ); }
static void *_zShape3DConeParseZTK(void *shape, ZTK *ztk){
  zCone3DInit( shape );
  return ZTKEvalKey( shape, NULL, ztk, __ztk_prp_shape_cone ); }
static void _zShape3DConeFPrintZTK(FILE *fp, void *shape){
  ZTKPrpKeyFPrint( fp, shape, __ztk_prp_shape_cone ); }

zShape3DCom zeo_shape3d_cone_com = {
  "cone",
  _zShape3DConeInit,
  _zShape3DConeAlloc,
  _zShape3DConeClone,
  _zShape3DConeMirror,
  _zShape3DConeDestroy,
  _zShape3DConeXform,
  _zShape3DConeXformInv,
  _zShape3DConeClosest,
  _zShape3DConePointDist,
  _zShape3DConePointIsInside,
  _zShape3DConeVolume,
  _zShape3DConeBarycenter,
  _zShape3DConeInertia,
  _zShape3DConeBaryInertia,
  _zShape3DConeToPH,
  _zShape3DConeRegZTK,
  _zShape3DConeParseZTK,
  _zShape3DConeFPrintZTK,
};

/* create a 3D shape as a cone. */
zShape3D *zShape3DConeCreate(zShape3D *shape, zVec3D *c, zVec3D *v, double r, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zCone3DAlloc() ) ) return NULL;
  zCone3DCreate( zShape3DCone(shape), c, v, r, div );
  shape->com = &zeo_shape3d_cone_com;
  return shape;
}
