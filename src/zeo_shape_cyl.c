/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_cyl - 3D shapes: cylinder.
 */

#include <zeo/zeo_shape.h>

/* ********************************************************** */
/* CLASS: zCyl3D
 * 3D cylinder class
 * ********************************************************** */

/* create a 3D cylinder. */
zCyl3D *zCyl3DCreate(zCyl3D *cyl, zVec3D *c1, zVec3D *c2, double r, int div)
{
  zCyl3DSetCenter( cyl, 0, c1 );
  zCyl3DSetCenter( cyl, 1, c2 );
  zCyl3DSetRadius( cyl, r );
  zCyl3DSetDiv( cyl, div == 0 ? ZEO_SHAPE_DEFAULT_DIV : div );
  return cyl;
}

/* initialize a 3D cylinder. */
zCyl3D *zCyl3DInit(zCyl3D *cyl)
{
  return zCyl3DCreate( cyl, ZVEC3DZERO, ZVEC3DZERO, 0, 0 );
}

/* allocate memory for a 3D cylinder. */
zCyl3D *zCyl3DAlloc(void)
{
  zCyl3D *cyl;

  if( !( cyl = zAlloc( zCyl3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return cyl;
}

/* copy a 3D cylinder to another. */
zCyl3D *zCyl3DCopy(zCyl3D *src, zCyl3D *dest)
{
  return zCyl3DCreate( dest,
    zCyl3DCenter(src,0), zCyl3DCenter(src,1),
    zCyl3DRadius(src), zCyl3DDiv(src) );
}

/* mirror a 3D cylinder. */
zCyl3D *zCyl3DMirror(zCyl3D *src, zCyl3D *dest, zAxis axis)
{
  zCyl3DCopy( src, dest );
  zCyl3DCenter(dest,0)->e[axis] *= -1;
  zCyl3DCenter(dest,1)->e[axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D cylinder. */
zCyl3D *zCyl3DXform(zCyl3D *src, zFrame3D *f, zCyl3D *dest)
{
  zXform3D( f, zCyl3DCenter(src,0), zCyl3DCenter(dest,0) );
  zXform3D( f, zCyl3DCenter(src,1), zCyl3DCenter(dest,1) );
  zCyl3DSetRadius( dest, zCyl3DRadius(src) );
  zCyl3DSetDiv( dest, zCyl3DDiv(src) );
  return dest;
}

/* inversely transform coordinates of a 3D cylinder. */
zCyl3D *zCyl3DXformInv(zCyl3D *src, zFrame3D *f, zCyl3D *dest)
{
  zXform3DInv( f, zCyl3DCenter(src,0), zCyl3DCenter(dest,0) );
  zXform3DInv( f, zCyl3DCenter(src,1), zCyl3DCenter(dest,1) );
  zCyl3DSetRadius( dest, zCyl3DRadius(src) );
  zCyl3DSetDiv( dest, zCyl3DDiv(src) );
  return dest;
}

static void _zCyl3DClosestPrep(zCyl3D *cyl, zVec3D *p, zVec3D *axis, zVec3D *v, double *l, double *r, double *d);
void _zCyl3DClosestPrep(zCyl3D *cyl, zVec3D *p, zVec3D *axis, zVec3D *v, double *l, double *r, double *d)
{
  zCyl3DAxis( cyl, axis );
  zVec3DSub( p, zCyl3DCenter(cyl,0), v );
  *l = zVec3DNormalizeDRC( axis );
  *r = zVec3DOuterProdNorm( v, axis );
  *d = zVec3DInnerProd( axis, v ) ;
}

/* the closest point to a 3D cylinder. */
double zCyl3DClosest(zCyl3D *cyl, zVec3D *p, zVec3D *cp)
{
  zVec3D axis, v, *c, cr;
  double l, r, d;

  _zCyl3DClosestPrep( cyl, p, &axis, &v, &l, &r, &d );

  if( d >= 0 && d <= l ){
    if( r <= zCyl3DRadius(cyl) ){
      zVec3DCopy( p, cp );
      return 0;
    }
    zVec3DMul( &axis, d, &cr );
    zVec3DSubDRC( &v, &cr );
    zVec3DCat( p, -(r-zCyl3DRadius(cyl))/zVec3DNorm(&v), &v, cp );
    return r - zCyl3DRadius(cyl);
  }
  if( d > l ){
    d -= l;
    c = zCyl3DCenter(cyl,1);
  } else
    c = zCyl3DCenter(cyl,0);

  zVec3DCat( p, -d, &axis, cp );
  if( r <= zCyl3DRadius(cyl) ) return -d;
  zVec3DSub( cp, c, &cr );
  zVec3DCat( c, zCyl3DRadius(cyl)/r, &cr, cp );
  return sqrt( zSqr( r - zCyl3DRadius(cyl) ) + zSqr(d) );
}

/* distance from a point to a 3D cylinder. */
double zCyl3DPointDist(zCyl3D *cyl, zVec3D *p)
{
  zVec3D axis, v;
  double l, r, d;

  _zCyl3DClosestPrep( cyl, p, &axis, &v, &l, &r, &d );
  if( d >= 0 && d <= l )
    return r <= zCyl3DRadius(cyl) ? 0 : r - zCyl3DRadius(cyl);
  if( d > l ) d -= l;
  return r <= zCyl3DRadius(cyl) ?
    -d : sqrt( zSqr( r - zCyl3DRadius(cyl) ) + zSqr(d) );
}

/* check if a point is inside of a cylinder. */
bool zCyl3DPointIsInside(zCyl3D *cyl, zVec3D *p, bool rim)
{
  zVec3D axis, v;
  double l, r, d;

  _zCyl3DClosestPrep( cyl, p, &axis, &v, &l, &r, &d );
  if( r - zCyl3DRadius(cyl) > ( rim ? zTOL : 0 ) ) return false;
  return d >= ( rim ? -zTOL : 0 ) && d <= ( rim ? l+zTOL : l ) ? true : false;
}

/* height of a 3D cylinder. */
double zCyl3DHeight(zCyl3D *cyl)
{
  zVec3D axis;

  return zVec3DNorm( zCyl3DAxis( cyl, &axis ) );
}

/* volume of a 3D cylinder. */
double zCyl3DVolume(zCyl3D *cyl)
{
  return zPI * zSqr(zCyl3DRadius(cyl)) * zCyl3DHeight(cyl);
}

/* barycenter of a 3D cylinder. */
zVec3D *zCyl3DBarycenter(zCyl3D *cyl, zVec3D *c)
{
  return zVec3DMid( zCyl3DCenter(cyl,0), zCyl3DCenter(cyl,1), c );
}

/* inertia tensor of a 3D cylinder. */
zMat3D *zCyl3DInertia(zCyl3D *cyl, zMat3D *inertia)
{
  double vol, rr, hh;
  zMat3D i, att;

  vol = zCyl3DVolume( cyl );
  /* aligned inertia tensor */
  rr = 3 * zSqr(zCyl3DRadius(cyl)) * vol;
  zCyl3DAxis( cyl, &att.b.z );
  hh = zVec3DSqrNorm(&att.b.z) * vol;
  zMat3DCreate( &i,
    (rr+hh)/12, 0, 0,
    0, (rr+hh)/12, 0,
    0, 0, rr/6 );
  /* rotate */
  zVec3DOrthoNormalSpace( &att.b.z, &att.b.x, &att.b.y );
  return zRotMat3D( &att, &i, inertia );
}

/* convert a cylinder to a polyhedron. */
zPH3D *zCyl3DToPH(zCyl3D *cyl, zPH3D *ph)
{
  zVec3D *vert, d, s, r, aa;
  zTri3D *face;
  register int i, j, n;

  if( !zPH3DAlloc( ph, zCyl3DDiv(cyl)*2, (zCyl3DDiv(cyl)-1)*4 ) )
    return NULL;
  vert = zPH3DVertBuf( ph );
  face = zPH3DFaceBuf( ph );

  zCyl3DAxis( cyl, &d );
  if( zVec3DIsTiny( &d ) ){
    ZRUNERROR( ZEO_ERR_SHAPE_ZEROC );
    return NULL;
  }
  zVec3DNormalizeDRC( &d );
  /* one radial vector */
  if( !zIsTiny( d.c.x ) && !zIsTiny( d.c.y ) )
    _zVec3DCreate( &s, d.c.y,-d.c.x, 0 );
  else
    _zVec3DCreate( &s, d.c.y-d.c.z, d.c.z-d.c.x, d.c.x-d.c.y );
  zVec3DNormalizeDRC( &s );
  zVec3DMulDRC( &s, zCyl3DRadius(cyl) );
  /* create vertices */
  for( i=0; i<zCyl3DDiv(cyl); i++ ){
    zVec3DMul( &d, zPIx2 * i / zCyl3DDiv(cyl), &aa );
    zVec3DRot( &s, &aa, &r );
    /* vertices on the top rim */
    zVec3DAdd( zCyl3DCenter(cyl,0), &r, &vert[i] );
    /* vertices on the bottom rim */
    zVec3DAdd( zCyl3DCenter(cyl,1), &r, &vert[i+zCyl3DDiv(cyl)] );
  }
  /* -- faces -- */
  /* base 1 */
  for( n=0, i=2; i<zCyl3DDiv(cyl); i++ )
    zTri3DCreate( &face[n++], &vert[0], &vert[i], &vert[i-1] );
  /* side faces */
  for( i=0, j=zCyl3DDiv(cyl)-1; i<zCyl3DDiv(cyl); j=i++ ){
    zTri3DCreate( &face[n++], &vert[j], &vert[i], &vert[i+zCyl3DDiv(cyl)] );
    zTri3DCreate( &face[n++], &vert[j], &vert[i+zCyl3DDiv(cyl)], &vert[j+zCyl3DDiv(cyl)] );
  }
  /* base 2 */
  for( i=zPH3DVertNum(ph)-zCyl3DDiv(cyl); i<zPH3DVertNum(ph)-2; i++ )
    zTri3DCreate( &face[n++],
      &vert[zPH3DVertNum(ph)-1], &vert[i], &vert[i+1] );

  return ph;
}

/* parse ZTK format */

static void *_zCyl3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zCyl3DCenter((zCyl3D*)obj,i), ztk );
  return obj;
}
static void *_zCyl3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCyl3DRadius((zCyl3D*)obj) = ZTKDouble(ztk);
  return obj;
}
static void *_zCyl3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCyl3DDiv((zCyl3D*)obj) = zShape3DDivFromZTK(ztk);
  return obj; }

static void _zCyl3DCenterFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCyl3DCenter((zCyl3D*)obj,i) ); }
static void _zCyl3DRadiusFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zCyl3DRadius((zCyl3D*)obj) ); }
static void _zCyl3DDivFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zCyl3DDiv((zCyl3D*)obj) ); }

static ZTKPrp __ztk_prp_shape_cyl[] = {
  { "center", 2, _zCyl3DCenterFromZTK, _zCyl3DCenterFPrintZTK },
  { "radius", 1, _zCyl3DRadiusFromZTK, _zCyl3DRadiusFPrintZTK },
  { "div", 1, _zCyl3DDivFromZTK, _zCyl3DDivFPrintZTK },
};

/* methods for abstraction */

static void *_zShape3DCylInit(void* shape){
  return zCyl3DInit( shape ); }
static void *_zShape3DCylAlloc(void){
  return zCyl3DAlloc(); }
static void *_zShape3DCylClone(void *src){
  zCyl3D *cln;
  return ( cln = zCyl3DAlloc() ) ? zCyl3DCopy( src, cln ) : NULL; }
static void *_zShape3DCylMirror(void *src, zAxis axis){
  zCyl3D *mrr;
  return ( mrr = zCyl3DAlloc() ) ? zCyl3DMirror( src, mrr, axis ) : NULL; }
static void _zShape3DCylDestroy(void *shape){}
static void *_zShape3DCylXform(void *src, zFrame3D *f, void *dest){
  return zCyl3DXform( src, f, dest ); }
static void *_zShape3DCylXformInv(void *src, zFrame3D *f, void *dest){
  return zCyl3DXformInv( src, f, dest ); }
static double _zShape3DCylClosest(void *shape, zVec3D *p, zVec3D *cp){
  return zCyl3DClosest( shape, p, cp ); }
static double _zShape3DCylPointDist(void *shape, zVec3D *p){
  return zCyl3DPointDist( shape, p ); }
static bool _zShape3DCylPointIsInside(void *shape, zVec3D *p, bool rim){
  return zCyl3DPointIsInside( shape, p, rim ); }
static double _zShape3DCylVolume(void *shape){
  return zCyl3DVolume( shape ); }
static zVec3D *_zShape3DCylBarycenter(void *shape, zVec3D *c){
  return zCyl3DBarycenter( shape, c ); }
static zMat3D *_zShape3DCylInertia(void *shape, zMat3D *i){
  return zCyl3DInertia( shape, i ); }
static void _zShape3DCylBaryInertia(void *shape, zVec3D *c, zMat3D *i){
  zCyl3DBarycenter( shape, c );
  zCyl3DInertia( shape, i ); }
static zPH3D *_zShape3DCylToPH(void *shape, zPH3D *ph){
  return zCyl3DToPH( shape, ph ); }
static void *_zShape3DCylParseZTK(void *shape, ZTK *ztk){
  zCyl3DInit( shape );
  return ZTKEvalKey( shape, NULL, ztk, __ztk_prp_shape_cyl ); }
static void _zShape3DCylFPrintZTK(FILE *fp, void *shape){
  ZTKPrpKeyFPrint( fp, shape, __ztk_prp_shape_cyl ); }

zShape3DCom zeo_shape3d_cyl_com = {
  "cylinder",
  _zShape3DCylInit,
  _zShape3DCylAlloc,
  _zShape3DCylClone,
  _zShape3DCylMirror,
  _zShape3DCylDestroy,
  _zShape3DCylXform,
  _zShape3DCylXformInv,
  _zShape3DCylClosest,
  _zShape3DCylPointDist,
  _zShape3DCylPointIsInside,
  _zShape3DCylVolume,
  _zShape3DCylBarycenter,
  _zShape3DCylInertia,
  _zShape3DCylBaryInertia,
  _zShape3DCylToPH,
  _zShape3DCylParseZTK,
  _zShape3DCylFPrintZTK,
};

/* create a 3D shape as a cylinder. */
zShape3D *zShape3DCylCreate(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zCyl3DAlloc() ) ) return NULL;
  zCyl3DCreate( zShape3DCyl(shape), c1, c2, r, div );
  shape->com = &zeo_shape3d_cyl_com;
  return shape;
}
