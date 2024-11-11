/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_capsule - 3D shapes: capsule.
 */

#include <zeo/zeo_shape3d.h>

/* ********************************************************** */
/* CLASS: zCapsule3D
 * 3D capsule class
 * ********************************************************** */

static void _zCapsule3DClosestPrep(zCapsule3D *capsule, zVec3D *p, zVec3D *axis, zVec3D *v, double *l, double *r, double *d)
{
  zCapsule3DAxis( capsule, axis );
  _zVec3DSub( p, zCapsule3DCenter(capsule,0), v );
  *l = zVec3DNormalizeDRC( axis );
  *r = zVec3DOuterProdNorm( v, axis );
  *d = zVec3DInnerProd( axis, v ) ;
}

/* the closest point to a 3D capsule. */
double zCapsule3DClosest(zCapsule3D *capsule, zVec3D *p, zVec3D *cp)
{
  zVec3D axis, v, *c, cr;
  double l, r, d;

  _zCapsule3DClosestPrep( capsule, p, &axis, &v, &l, &r, &d );
  if( d >= 0 && d <= l ){
    if( r <= zCapsule3DRadius(capsule) ){
      zVec3DCopy( p, cp );
      return 0;
    }
    zVec3DMul( &axis, d, &cr );
    zVec3DSubDRC( &v, &cr );
    zVec3DCat( p, -(r-zCapsule3DRadius(capsule))/zVec3DNorm(&v), &v, cp );
    return r - zCapsule3DRadius(capsule);
  }
  if( d > l ){
    c = zCapsule3DCenter(capsule,1);
  } else
    c = zCapsule3DCenter(capsule,0);
  zVec3DSub( p, c, &cr );
  if( ( r = zVec3DNorm( &cr ) ) <= zCapsule3DRadius(capsule) ){
    zVec3DCopy( p, cp );
    return 0;
  }
  zVec3DCat( c, zCapsule3DRadius(capsule)/r, &cr, cp );
  return r - zCapsule3DRadius(capsule);
}

/* distance from a point to a 3D capsule. */
double zCapsule3DDistFromPoint(zCapsule3D *capsule, zVec3D *p)
{
  zVec3D axis, v;
  double l, r, d;

  _zCapsule3DClosestPrep( capsule, p, &axis, &v, &l, &r, &d );
  if( d >= 0 && d <= l )
    return r <= zCapsule3DRadius(capsule) ? 0 : r - zCapsule3DRadius(capsule);
  if( d > l )
    return ( r = zVec3DDist( p, zCapsule3DCenter(capsule,1) ) ) < zCapsule3DRadius(capsule) ? 0 : zCapsule3DRadius(capsule) - r;
  return ( r = zVec3DDist( p, zCapsule3DCenter(capsule,0) ) ) < zCapsule3DRadius(capsule) ? 0 : zCapsule3DRadius(capsule) - r;
}

/* check if a point is inside of a capsule. */
bool zCapsule3DPointIsInside(zCapsule3D *capsule, zVec3D *p, double margin)
{
  return zCapsule3DDistFromPoint( capsule, p ) < margin ? true : false;
}

/* height of a 3D capsule. */
double zCapsule3DHeight(zCapsule3D *capsule)
{
  return zCyl3DHeight( capsule ) + zCapsule3DRadius( capsule ) * 2;
}

/* volume of a 3D capsule. */
double zCapsule3DVolume(zCapsule3D *capsule)
{
  return zCyl3DVolume( capsule ) + 4 * zPI * zCube( zCapsule3DRadius(capsule) ) / 3.0;
}

/* inertia tensor about barycenter of a 3D capsule from mass. */
zMat3D *zCapsule3DBaryInertiaMass(zCapsule3D *capsule, double mass, zMat3D *inertia)
{
  return zCapsule3DBaryInertia( capsule, mass / zCapsule3DVolume( capsule ), inertia );
}

/* inertia tensor about barycenter of a 3D capsule. */
zMat3D *zCapsule3DBaryInertia(zCapsule3D *capsule, double density, zMat3D *inertia)
{
  double r, r2, h, m, izz, ixx;
  zMat3D i, att;

  /* aligned inertia tensor */
  r2 = zSqr( ( r = zCapsule3DRadius(capsule) ) );
  zCapsule3DAxis( capsule, &att.b.z );
  h = zVec3DNorm(&att.b.z);
  m = density * zPI;
  izz = m * r2 * r2 * ( 8.0 * r / 15 + 0.5 * h );
  ixx = m * r2 * ( r2 * ( 8.0 * r / 15 + 0.75 * h ) + ( r + 0.25 * h ) * h * h / 3.0 );
  zMat3DCreate( &i,
    ixx, 0, 0,
    0, ixx, 0,
    0, 0, izz );
  /* rotate */
  zVec3DOrthoNormalSpace( &att.b.z, &att.b.x, &att.b.y );
  return zRotMat3D( &att, &i, inertia );
}

/* convert a capsule to a polyhedron. */
zPH3D *zCapsule3DToPH(zCapsule3D *capsule, zPH3D *ph)
{
  zVec3D *vert, d, dr, s, r, aa;
  zTri3D *face;
  int i, j, j2, n, nh, n1, n2, n3, n4;
  double lat;

  if( !zPH3DAlloc( ph, zSqr( zCapsule3DDiv(capsule) ) / 2 + 2, zSqr( zCapsule3DDiv(capsule) ) ) )
    return NULL;
  vert = zPH3DVertBuf( ph );
  face = zPH3DFaceBuf( ph );

  zCapsule3DAxis( capsule, &d );
  if( zVec3DIsTiny( &d ) ){
    ZRUNERROR( ZEO_ERR_SHAPE_ZEROC );
    return NULL;
  }
  zVec3DNormalizeDRC( &d );
  /* one radial vector */
  zVec3DOrthoNormal( &d, &s );
  zVec3DMulDRC( &s, zCapsule3DRadius(capsule) );
  zVec3DMul( &d, zCapsule3DRadius(capsule), &dr );
  /* -- vertices -- */
  /* north pole vertex */
  zVec3DAdd( zCapsule3DCenter(capsule,1), &dr, &vert[0] );
  /* north hemisphere vertices */
  nh = zCapsule3DDiv(capsule) / 4;
  for( n=0, i=1; i<=nh; i++ ){
    lat = zPI_2 * i / nh;
    zVec3DMul( &dr, cos(lat), &r );
    zVec3DCatDRC( &r, sin(lat), &s );
    for( j=0; j<zCapsule3DDiv(capsule); j++ ){
      zVec3DMul( &d, zPIx2 * j / zCapsule3DDiv(capsule), &aa );
      zVec3DRot( &r, &aa, &vert[++n] );
      zVec3DAddDRC( &vert[n], zCapsule3DCenter(capsule,1) );
    }
  }
  /* south hemisphere vertices */
  zVec3DRevDRC( &dr );
  for( i=nh; i>=1; i-- ){
    lat = zPI_2 * i / nh;
    zVec3DMul( &dr, cos(lat), &r );
    zVec3DCatDRC( &r, sin(lat), &s );
    for( j=0; j<zCapsule3DDiv(capsule); j++ ){
      zVec3DMul( &d, zPIx2 * j / zCapsule3DDiv(capsule), &aa );
      zVec3DRot( &r, &aa, &vert[++n] );
      zVec3DAddDRC( &vert[n], zCapsule3DCenter(capsule,0) );
    }
  }
  /* south pole vertex */
  zVec3DAdd( zCapsule3DCenter(capsule,0), &dr, &vert[++n] );
  /* -- faces -- */
  /* arctic faces */
  for( n=0, j=1, j2=zCapsule3DDiv(capsule); j<=zCapsule3DDiv(capsule); j++, j2=j-1 )
    zTri3DCreate( &face[n++], &vert[0], &vert[j2], &vert[j] );
  /* general faces */
  nh *= 2;
  for( i=1; i<nh; i++ ){
    for( j=1, j2=zCapsule3DDiv(capsule); j<=zCapsule3DDiv(capsule); j++, j2=j-1 ){
      n4 = ( n1 = zCapsule3DDiv(capsule)*i+j2 ) - zCapsule3DDiv(capsule);
      n3 = ( n2 = zCapsule3DDiv(capsule)*i+j  ) - zCapsule3DDiv(capsule);
      zTri3DCreate( &face[n++], &vert[n1], &vert[n3], &vert[n4] );
      zTri3DCreate( &face[n++], &vert[n1], &vert[n2], &vert[n3] );
    }
  }
  /* antarctic faces */
  for( j=zCapsule3DDiv(capsule)+1, j2=2; j>1; j--, j2=j+1 )
    zTri3DCreate( &face[n++], &vert[zPH3DVertNum(ph)-1], &vert[zPH3DVertNum(ph)-j], &vert[zPH3DVertNum(ph)-j2] );
  return ph;
}

/* parse ZTK format */

static void *_zCapsule3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zCapsule3DCenter((zCapsule3D*)obj,i), ztk );
  return obj;
}
static void *_zCapsule3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCapsule3DRadius((zCapsule3D*)obj) = ZTKDouble(ztk);
  return obj;
}
static void *_zCapsule3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCapsule3DDiv((zCapsule3D*)obj) = zShape3DDivFromZTK(ztk);
  return obj; }

static bool _zCapsule3DCenterFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCapsule3DCenter((zCapsule3D*)obj,i) );
  return true; }
static bool _zCapsule3DRadiusFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zCapsule3DRadius((zCapsule3D*)obj) );
  return true; }
static bool _zCapsule3DDivFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zCapsule3DDiv((zCapsule3D*)obj) );
  return true; }

static const ZTKPrp __ztk_prp_shape_capsule[] = {
  { ZTK_KEY_ZEO_SHAPE3D_CENTER, 2, _zCapsule3DCenterFromZTK, _zCapsule3DCenterFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_RADIUS, 1, _zCapsule3DRadiusFromZTK, _zCapsule3DRadiusFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_DIV,    1, _zCapsule3DDivFromZTK, _zCapsule3DDivFPrintZTK },
};

/* print a 3D capsule out to a file in a ZTK format. */
void zCapsule3DFPrintZTK(FILE *fp, zCapsule3D *capsule)
{
  ZTKPrpKeyFPrint( fp, capsule, __ztk_prp_shape_capsule );
}

/* methods for abstraction */

static void *_zShape3DCapsuleInit(void *body){
  return zCapsule3DInit( (zCapsule3D*)body ); }
static void *_zShape3DCapsuleAlloc(void){
  return zCapsule3DAlloc(); }
static void *_zShape3DCapsuleClone(void *src){
  zCapsule3D *cln;
  return ( cln = zCapsule3DAlloc() ) ? zCapsule3DCopy( (zCapsule3D*)src, cln ) : NULL; }
static void *_zShape3DCapsuleMirror(void *src, zAxis axis){
  zCapsule3D *mrr;
  return ( mrr = zCapsule3DAlloc() ) ? zCapsule3DMirror( (zCapsule3D*)src, mrr, axis ) : NULL; }
static void _zShape3DCapsuleDestroy(void *body){}
static void *_zShape3DCapsuleXform(void *src, zFrame3D *f, void *dest){
  return zCapsule3DXform( (zCapsule3D*)src, f, (zCapsule3D*)dest ); }
static void *_zShape3DCapsuleXformInv(void *src, zFrame3D *f, void *dest){
  return zCapsule3DXformInv( (zCapsule3D*)src, f, (zCapsule3D*)dest ); }
static double _zShape3DCapsuleClosest(void *body, zVec3D *p, zVec3D *cp){
  return zCapsule3DClosest( (zCapsule3D*)body, p, cp ); }
static double _zShape3DCapsuleDistFromPoint(void *body, zVec3D *p){
  return zCapsule3DDistFromPoint( (zCapsule3D*)body, p ); }
static bool _zShape3DCapsulePointIsInside(void *body, zVec3D *p, double margin){
  return zCapsule3DPointIsInside( (zCapsule3D*)body, p, margin ); }
static double _zShape3DCapsuleVolume(void *body){
  return zCapsule3DVolume( (zCapsule3D*)body ); }
static zVec3D *_zShape3DCapsuleBarycenter(void *body, zVec3D *c){
  return zCapsule3DBarycenter( (zCapsule3D*)body, c ); }
static zMat3D *_zShape3DCapsuleBaryInertiaMass(void *body, double mass, zMat3D *i){
  return zCapsule3DBaryInertiaMass( (zCapsule3D*)body, mass, i ); }
static zMat3D *_zShape3DCapsuleBaryInertia(void *body, double density, zMat3D *i){
  return zCapsule3DBaryInertia( (zCapsule3D*)body, density, i ); }
static zPH3D *_zShape3DCapsuleToPH(void *body, zPH3D *ph){
  return zCapsule3DToPH( (zCapsule3D*)body, ph ); }
static void *_zShape3DCapsuleParseZTK(void *body, ZTK *ztk){
  zCapsule3DInit( (zCapsule3D*)body );
  return ZTKEvalKey( body, NULL, ztk, __ztk_prp_shape_capsule ); }
static void _zShape3DCapsuleFPrintZTK(FILE *fp, void *body){
  zCapsule3DFPrintZTK( fp, (zCapsule3D*)body ); }

zShape3DCom zeo_shape3d_capsule_com = {
  "capsule",
  _zShape3DCapsuleInit,
  _zShape3DCapsuleAlloc,
  _zShape3DCapsuleClone,
  _zShape3DCapsuleMirror,
  _zShape3DCapsuleDestroy,
  _zShape3DCapsuleXform,
  _zShape3DCapsuleXformInv,
  _zShape3DCapsuleClosest,
  _zShape3DCapsuleDistFromPoint,
  _zShape3DCapsulePointIsInside,
  _zShape3DCapsuleVolume,
  _zShape3DCapsuleBarycenter,
  _zShape3DCapsuleBaryInertiaMass,
  _zShape3DCapsuleBaryInertia,
  _zShape3DCapsuleToPH,
  _zShape3DCapsuleParseZTK,
  _zShape3DCapsuleFPrintZTK,
};

/* create a 3D shape as a capsule. */
zShape3D *zShape3DCapsuleCreate(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zCapsule3DAlloc() ) ) return NULL;
  zCapsule3DCreate( zShape3DCapsule(shape), c1, c2, r, div );
  shape->com = &zeo_shape3d_capsule_com;
  return shape;
}
