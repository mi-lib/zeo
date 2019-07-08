/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_cone - primitive 3D shapes: cone.
 */

#include <zeo/zeo_prim.h>

/* ********************************************************** */
/* CLASS: zCone3D
 * 3D cone class
 * ********************************************************** */

static bool _zCone3DFScan(FILE *fp, void *instance, char *buf, bool *success);

/* create a 3D cone. */
zCone3D *zCone3DCreate(zCone3D *cone, zVec3D *c, zVec3D *v, double r, int div)
{
  zCone3DSetCenter( cone, c );
  zCone3DSetVert( cone, v );
  zCone3DSetRadius( cone, r );
  zCone3DSetDiv( cone, div == 0 ? ZEO_PRIM_DEFAULT_DIV : div );
  return cone;
}

/* initialize a 3D cone. */
zCone3D *zCone3DInit(zCone3D *cone)
{
  return zCone3DCreate( cone, ZVEC3DZERO, ZVEC3DZERO, 0, 0 );
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
  zVec3DOrthoSpace( &att.b.z, &att.b.x, &att.b.y );
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
    ZRUNERROR( ZEO_ERR_PRIM_ZEROC );
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
  zCone3DDiv((zCone3D*)obj) = zPrim3DDivFromZTK(ztk);
  return obj; }

static void _zCone3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCone3DCenter((zCone3D*)obj) ); }
static void _zCone3DVertFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCone3DVert((zCone3D*)obj) ); }
static void _zCone3DRadiusFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zCone3DRadius((zCone3D*)obj) ); }
static void _zCone3DDivFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zCone3DDiv((zCone3D*)obj) ); }

static ZTKPrp __ztk_prp_prim_cone[] = {
  { "center", 1, _zCone3DCenterFromZTK, _zCone3DCenterFPrint },
  { "vert", 1, _zCone3DVertFromZTK, _zCone3DVertFPrint },
  { "radius", 1, _zCone3DRadiusFromZTK, _zCone3DRadiusFPrint },
  { "div", 1, _zCone3DDivFromZTK, _zCone3DDivFPrint },
};

/* register a definition of tag-and-keys for a 3D cone to a ZTK format processor. */
bool zCone3DDefRegZTK(ZTK *ztk, char *tag)
{
  return ZTKDefRegPrp( ztk, tag, __ztk_prp_prim_cone );
}

/* read a 3D cone from a ZTK format processor. */
zCone3D *zCone3DFromZTK(zCone3D *cone, ZTK *ztk)
{
  zCone3DInit( cone );
  return ZTKEncodeKey( cone, NULL, ztk, __ztk_prp_prim_cone );
}

/* (static)
 * scan a 3D cone (internal function). */
bool _zCone3DFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  if( strcmp( buf, "center" ) == 0 ){
    zVec3DFScan( fp, zCone3DCenter( (zCone3D *)instance ) );
  } else if( strcmp( buf, "vert" ) == 0 ){
    zVec3DFScan( fp, zCone3DVert( (zCone3D *)instance ) );
  } else if( strcmp( buf, "radius" ) == 0 ){
    zCone3DSetRadius( (zCone3D *)instance, zFDouble( fp ) );
  } else if( strcmp( buf, "div" ) == 0 ){
    zCone3DSetDiv( (zCone3D *)instance, zFInt( fp ) );
  } else
    return false;
  return true;
}

/* scan a 3D cone from a file. */
zCone3D *zCone3DFScan(FILE *fp, zCone3D *cone)
{
  zCone3DInit( cone );
  zFieldFScan( fp, _zCone3DFScan, cone );
  return cone;
}

/* print out a 3D cone to a file. */
void zCone3DFPrint(FILE *fp, zCone3D *cone)
{
  ZTKPrpKeyFPrint( fp, cone, __ztk_prp_prim_cone );
}

/* methods for abstraction */

static void *_zPrim3DInitCone(void* prim){
  return zCone3DInit( prim ); }
static void *_zPrim3DCloneCone(void *src, void *dest){
  return zCone3DCopy( src, dest ); }
static void *_zPrim3DMirrorCone(void *src, void *dest, zAxis axis){
  return zCone3DMirror( src, dest, axis ); }
static void _zPrim3DDestroyCone(void *prim){}
static void *_zPrim3DXformCone(void *src, zFrame3D *f, void *dest){
  return zCone3DXform( src, f, dest ); }
static void *_zPrim3DXformInvCone(void *src, zFrame3D *f, void *dest){
  return zCone3DXformInv( src, f, dest ); }
static double _zPrim3DClosestCone(void *prim, zVec3D *p, zVec3D *cp){
  return zCone3DClosest( prim, p, cp ); }
static double _zPrim3DPointDistCone(void *prim, zVec3D *p){
  return zCone3DPointDist( prim, p ); }
static bool _zPrim3DPointIsInsideCone(void *prim, zVec3D *p, bool rim){
  return zCone3DPointIsInside( prim, p, rim ); }
static double _zPrim3DVolumeCone(void *prim){
  return zCone3DVolume( prim ); }
static zVec3D *_zPrim3DBarycenterCone(void *prim, zVec3D *c){
  return zCone3DBarycenter( prim, c ); }
static zMat3D *_zPrim3DInertiaCone(void *prim, zMat3D *i){
  return zCone3DInertia( prim, i ); }
static void _zPrim3DBaryInertiaCone(void *prim, zVec3D *c, zMat3D *i){
  zCone3DBarycenter( prim, c );
  zCone3DInertia( prim, i ); }
static zPH3D *_zPrim3DToPHCone(void *prim, zPH3D *ph){
  return zCone3DToPH( prim, ph ); }
static void *_zPrim3DParseZTKCone(void *prim, ZTK *ztk){
  return zCone3DFromZTK( prim, ztk ); }
static void *_zPrim3DFScanCone(FILE *fp, void *prim){
  return zCone3DFScan( fp, prim ); }
static void _zPrim3DFPrintCone(FILE *fp, void *prim){
  return zCone3DFPrint( fp, prim ); }

zPrimCom zprim_cone3d_com = {
  _zPrim3DInitCone,
  _zPrim3DCloneCone,
  _zPrim3DMirrorCone,
  _zPrim3DDestroyCone,
  _zPrim3DXformCone,
  _zPrim3DXformInvCone,
  _zPrim3DClosestCone,
  _zPrim3DPointDistCone,
  _zPrim3DPointIsInsideCone,
  _zPrim3DVolumeCone,
  _zPrim3DBarycenterCone,
  _zPrim3DInertiaCone,
  _zPrim3DBaryInertiaCone,
  _zPrim3DToPHCone,
  _zPrim3DParseZTKCone,
  _zPrim3DFScanCone,
  _zPrim3DFPrintCone,
};
