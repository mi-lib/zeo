/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_cyl - 3D shapes: cylinder.
 */

#include <zeo/zeo_shape3d.h>

/* ********************************************************** */
/* 3D cylinder class
 * ********************************************************** */

/* create a 3D cylinder. */
zCyl3D *zCyl3DCreate(zCyl3D *cyl, const zVec3D *center1, const zVec3D *center2, double radius, int div)
{
  zCyl3DSetCenter( cyl, 0, center1 );
  zCyl3DSetCenter( cyl, 1, center2 );
  zCyl3DSetRadius( cyl, radius );
  zCyl3DSetDiv( cyl, div == 0 ? ZEO_SHAPE_DEFAULT_DIV : div );
  return cyl;
}

/* initialize a 3D cylinder. */
zCyl3D *zCyl3DInit(zCyl3D *cyl)
{
  return zCyl3DCreate( cyl, ZVEC3DZERO, ZVEC3DZERO, 0, 0 );
}

/* allocate memory for a 3D cylinder. */
ZDEF_ALLOC_FUNCTION( zCyl3D )

/* copy a 3D cylinder to another. */
zCyl3D *zCyl3DCopy(const zCyl3D *src, zCyl3D *dest)
{
  return zCyl3DCreate( dest,
    zCyl3DCenter(src,0), zCyl3DCenter(src,1),
    zCyl3DRadius(src), zCyl3DDiv(src) );
}

/* mirror a 3D cylinder. */
zCyl3D *zCyl3DMirror(const zCyl3D *src, zCyl3D *dest, zAxis axis)
{
  zCyl3DCopy( src, dest );
  zCyl3DCenter(dest,0)->e[(int)axis] *= -1;
  zCyl3DCenter(dest,1)->e[(int)axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D cylinder. */
zCyl3D *zCyl3DXform(const zCyl3D *src, const zFrame3D *frame, zCyl3D *dest)
{
  zXform3D( frame, zCyl3DCenter(src,0), zCyl3DCenter(dest,0) );
  zXform3D( frame, zCyl3DCenter(src,1), zCyl3DCenter(dest,1) );
  zCyl3DSetRadius( dest, zCyl3DRadius(src) );
  zCyl3DSetDiv( dest, zCyl3DDiv(src) );
  return dest;
}

/* inversely transform coordinates of a 3D cylinder. */
zCyl3D *zCyl3DXformInv(const zCyl3D *src, const zFrame3D *frame, zCyl3D *dest)
{
  zXform3DInv( frame, zCyl3DCenter(src,0), zCyl3DCenter(dest,0) );
  zXform3DInv( frame, zCyl3DCenter(src,1), zCyl3DCenter(dest,1) );
  zCyl3DSetRadius( dest, zCyl3DRadius(src) );
  zCyl3DSetDiv( dest, zCyl3DDiv(src) );
  return dest;
}

static void _zCyl3DClosestPrep(const zCyl3D *cyl, const zVec3D *point, zVec3D *axis, zVec3D *v, double *l, double *r, double *d)
{
  zCyl3DAxis( cyl, axis );
  _zVec3DSub( point, zCyl3DCenter(cyl,0), v );
  *l = zVec3DNormalizeDRC( axis );
  *r = zVec3DOuterProdNorm( v, axis );
  *d = zVec3DInnerProd( axis, v ) ;
}

/* the closest point to a 3D cylinder. */
double zCyl3DClosest(const zCyl3D *cyl, const zVec3D *point, zVec3D *closestpoint)
{
  zVec3D axis, v, cr;
  const zVec3D *c;
  double l, r, d;

  _zCyl3DClosestPrep( cyl, point, &axis, &v, &l, &r, &d );

  if( d >= 0 && d <= l ){
    if( r <= zCyl3DRadius(cyl) ){
      zVec3DCopy( point, closestpoint );
      return 0;
    }
    zVec3DMul( &axis, d, &cr );
    zVec3DSubDRC( &v, &cr );
    zVec3DCat( point, -(r-zCyl3DRadius(cyl))/zVec3DNorm(&v), &v, closestpoint );
    return r - zCyl3DRadius(cyl);
  }
  if( d > l ){
    d -= l;
    c = zCyl3DCenter(cyl,1);
  } else
    c = zCyl3DCenter(cyl,0);

  zVec3DCat( point, -d, &axis, closestpoint );
  if( r <= zCyl3DRadius(cyl) ) return -d;
  zVec3DSub( closestpoint, c, &cr );
  zVec3DCat( c, zCyl3DRadius(cyl)/r, &cr, closestpoint );
  return sqrt( zSqr( r - zCyl3DRadius(cyl) ) + zSqr(d) );
}

/* distance from a point to a 3D cylinder. */
double zCyl3DDistFromPoint(const zCyl3D *cyl, const zVec3D *point)
{
  zVec3D axis, v;
  double l, r, d;

  _zCyl3DClosestPrep( cyl, point, &axis, &v, &l, &r, &d );
  if( d >= 0 && d <= l )
    return r <= zCyl3DRadius(cyl) ? 0 : r - zCyl3DRadius(cyl);
  if( d > l ) d -= l;
  return r <= zCyl3DRadius(cyl) ?
    -d : sqrt( zSqr( r - zCyl3DRadius(cyl) ) + zSqr(d) );
}

/* check if a point is inside of a cylinder. */
bool zCyl3DPointIsInside(const zCyl3D *cyl, const zVec3D *point, double margin)
{
  zVec3D axis, v;
  double l, r, d;

  _zCyl3DClosestPrep( cyl, point, &axis, &v, &l, &r, &d );
  if( r - zCyl3DRadius(cyl) >= margin ) return false;
  return d > -margin && d < l + margin ? true : false;
}

/* height of a 3D cylinder. */
double zCyl3DHeight(const zCyl3D *cyl)
{
  zVec3D axis;

  return zVec3DNorm( zCyl3DAxis( cyl, &axis ) );
}

/* volume of a 3D cylinder. */
double zCyl3DVolume(const zCyl3D *cyl)
{
  return zPI * zSqr(zCyl3DRadius(cyl)) * zCyl3DHeight(cyl);
}

/* barycenter of a 3D cylinder. */
zVec3D *zCyl3DBarycenter(const zCyl3D *cyl, zVec3D *center)
{
  return zVec3DMid( zCyl3DCenter(cyl,0), zCyl3DCenter(cyl,1), center );
}

/* inertia tensor about barycenter of a 3D cylinder from mass. */
zMat3D *zCyl3DBaryInertiaMass(const zCyl3D *cyl, double mass, zMat3D *inertia)
{
  double rr, hh;
  zMat3D i, att;

  /* aligned inertia tensor */
  rr = 3 * mass * zSqr(zCyl3DRadius(cyl));
  zCyl3DAxis( cyl, &att.b.z );
  hh = mass * zVec3DSqrNorm(&att.b.z);
  zMat3DCreate( &i,
    (rr+hh)/12, 0, 0,
    0, (rr+hh)/12, 0,
    0, 0, rr/6 );
  /* rotate */
  zVec3DOrthonormalSpace( &att.b.z, &att.b.x, &att.b.y );
  return zRotMat3D( &att, &i, inertia );
}

/* inertia tensor about barycenter of a 3D cylinder. */
zMat3D *zCyl3DBaryInertia(const zCyl3D *cyl, double density, zMat3D *inertia)
{
  return zCyl3DBaryInertiaMass( cyl, density * zCyl3DVolume( cyl ), inertia );
}

/* convert a cylinder to a polyhedron. */
zPH3D *zCyl3DToPH(const zCyl3D *cyl, zPH3D *ph)
{
  zVec3D *vert, d, s, r, aa;
  zTri3D *face;
  int i, j, n;

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
  zVec3DOrthonormal( &d, &s );
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

static bool _zCyl3DCenterFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCyl3DCenter((zCyl3D*)obj,i) );
  return true; }
static bool _zCyl3DRadiusFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zCyl3DRadius((zCyl3D*)obj) );
  return true; }
static bool _zCyl3DDivFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zCyl3DDiv((zCyl3D*)obj) );
  return true; }

static const ZTKPrp __ztk_prp_shape_cyl[] = {
  { ZTK_KEY_ZEO_SHAPE3D_CENTER, 2, _zCyl3DCenterFromZTK, _zCyl3DCenterFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_RADIUS, 1, _zCyl3DRadiusFromZTK, _zCyl3DRadiusFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_DIV,    1, _zCyl3DDivFromZTK, _zCyl3DDivFPrintZTK },
};

/* print a 3D cylinder out to a file in a ZTK format. */
void zCyl3DFPrintZTK(FILE *fp, const zCyl3D *cyl)
{
  _ZTKPrpKeyFPrint( fp, (void*)cyl, __ztk_prp_shape_cyl );
}

/* methods for abstraction */

static void *_zShape3DCylInit(void *body){
  return zCyl3DInit( (zCyl3D*)body ); }
static void *_zShape3DCylAlloc(void){
  return zCyl3DAlloc(); }
static void *_zShape3DCylClone(void *src){
  zCyl3D *cln;
  return ( cln = zCyl3DAlloc() ) ? zCyl3DCopy( (zCyl3D*)src, cln ) : NULL; }
static void *_zShape3DCylMirror(void *src, zAxis axis){
  zCyl3D *mrr;
  return ( mrr = zCyl3DAlloc() ) ? zCyl3DMirror( (zCyl3D*)src, mrr, axis ) : NULL; }
static void _zShape3DCylDestroy(void *body){}
static void *_zShape3DCylXform(void *src, const zFrame3D *f, void *dest){
  return zCyl3DXform( (zCyl3D*)src, f, (zCyl3D*)dest ); }
static void *_zShape3DCylXformInv(void *src, const zFrame3D *f, void *dest){
  return zCyl3DXformInv( (zCyl3D*)src, f, (zCyl3D*)dest ); }
static double _zShape3DCylClosest(void *body, const zVec3D *p, zVec3D *cp){
  return zCyl3DClosest( (zCyl3D*)body, p, cp ); }
static double _zShape3DCylDistFromPoint(void *body, const zVec3D *p){
  return zCyl3DDistFromPoint( (zCyl3D*)body, p ); }
static bool _zShape3DCylPointIsInside(void *body, const zVec3D *p, double margin){
  return zCyl3DPointIsInside( (zCyl3D*)body, p, margin ); }
static double _zShape3DCylVolume(void *body){
  return zCyl3DVolume( (zCyl3D*)body ); }
static zVec3D *_zShape3DCylBarycenter(void *body, zVec3D *c){
  return zCyl3DBarycenter( (zCyl3D*)body, c ); }
static zMat3D *_zShape3DCylBaryInertiaMass(void *body, double mass, zMat3D *i){
  return zCyl3DBaryInertiaMass( (zCyl3D*)body, mass, i ); }
static zMat3D *_zShape3DCylBaryInertia(void *body, double density, zMat3D *i){
  return zCyl3DBaryInertia( (zCyl3D*)body, density, i ); }
static zPH3D *_zShape3DCylToPH(void *body, zPH3D *ph){
  return zCyl3DToPH( (zCyl3D*)body, ph ); }
static void *_zShape3DCylParseZTK(void *body, ZTK *ztk){
  zCyl3DInit( (zCyl3D*)body );
  return _ZTKEvalKey( body, NULL, ztk, __ztk_prp_shape_cyl ); }
static void _zShape3DCylFPrintZTK(FILE *fp, void *body){
  zCyl3DFPrintZTK( fp, (zCyl3D*)body ); }

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
  _zShape3DCylDistFromPoint,
  _zShape3DCylPointIsInside,
  _zShape3DCylVolume,
  _zShape3DCylBarycenter,
  _zShape3DCylBaryInertiaMass,
  _zShape3DCylBaryInertia,
  _zShape3DCylToPH,
  _zShape3DCylParseZTK,
  _zShape3DCylFPrintZTK,
};

/* create a 3D shape as a cylinder. */
zShape3D *zShape3DCylCreate(zShape3D *shape, const zVec3D *center1, const zVec3D *center2, double radius, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zCyl3DAlloc() ) ) return NULL;
  zCyl3DCreate( zShape3DCyl(shape), center1, center2, radius, div );
  shape->com = &zeo_shape3d_cyl_com;
  return shape;
}
