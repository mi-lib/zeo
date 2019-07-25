/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_ecyl - 3D shapes: elliptic cylinder.
 */

#include <zeo/zeo_shape.h>

/* ********************************************************** */
/* CLASS: zECyl3D
 * 3D elliptic cylinder class
 * ********************************************************** */

static bool _zECyl3DFScan(FILE *fp, void *instance, char *buf, bool *success);

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
zECyl3D *zECyl3DAlloc(void)
{
  zECyl3D *ecyl;

  if( !( ecyl = zAlloc( zECyl3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
   return ecyl;
}

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
  zECyl3DCenter(dest,0)->e[axis] *= -1;
  zECyl3DCenter(dest,1)->e[axis] *= -1;
  zECyl3DRadVec(dest,0)->e[axis] *= -1;
  zECyl3DRadVec(dest,1)->e[axis] *= -1;
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
  zVec3D v, axis, vr;
  double rat[3], r0, r1, r02, r12, x2, y2, l;
  zPex pex;
  zComplex ans[4];
  register int i;

  zVec3DSub( p, zECyl3DCenter(cyl,0), &v );
  zECyl3DAxis( cyl, &axis );
  zVec3DCatRatio( zECyl3DRadVec(cyl,0), zECyl3DRadVec(cyl,1), &axis, &v, rat );
  x2 = zSqr( rat[0] );
  y2 = zSqr( rat[1] );
  r02 = zSqr( ( r0 = zECyl3DRadius( cyl, 0 ) ) );
  r12 = zSqr( ( r1 = zECyl3DRadius( cyl, 1 ) ) );

  if( x2/r02 + y2/r12 > 1 ){
    if( !( pex = zPexAlloc( 4 ) ) ){
      ZALLOCERROR();
      return HUGE_VAL;
    }
    zPexSetCoeff( pex, 4, 1 );
    zPexSetCoeff( pex, 3, 2 * ( r0 + r1 ) );
    zPexSetCoeff( pex, 2, r02 + 4*r0*r1 + r12 - x2 - y2 );
    zPexSetCoeff( pex, 1, 2 * ( r0 * ( r12 - y2 ) + r1 * ( r02 - x2 ) ) );
    zPexSetCoeff( pex, 0, r02*r12 - x2*r12 - y2*r02 );
    zPexBH( pex, ans, zTOL, 0 );
    zPexFree( pex );
    for( i=0; i<4; i++ )
      if( ( l = ans[i].re ) >= 0 ) break;
    if( i == 4 || !zIsTiny( ans[i].im ) ){
      ZRUNERROR( ZEO_ERR_FATAL );
      return HUGE_VAL;
    }
    rat[0] *= r0 / ( l + r0 );
    rat[1] *= r1 / ( l + r1 );
  }
  zVec3DMul( zECyl3DRadVec(cyl,0), rat[0], &vr );
  zVec3DCatDRC( &vr, rat[1], zECyl3DRadVec(cyl,1) );

  if( rat[2] < 0 ){
    zVec3DAdd( zECyl3DCenter(cyl,0), &vr, cp );
  } else
  if( rat[2] > 1 ){
    zVec3DAdd( zECyl3DCenter(cyl,1), &vr, cp );
  } else{
    zVec3DCat( zECyl3DCenter(cyl,0), rat[2], &axis, cp );
    zVec3DAddDRC( cp, &vr );
  }
  return zVec3DDist( cp, p );
}

/* distance from a point to a 3D elliptic cylinder. */
double zECyl3DPointDist(zECyl3D *cyl, zVec3D *p)
{
  zVec3D cp;

  return zECyl3DClosest( cyl, p, &cp );
}

/* check if a point is inside of an elliptic cylinder. */
bool zECyl3DPointIsInside(zECyl3D *cyl, zVec3D *p, bool rim)
{
  return zECyl3DPointDist( cyl, p ) < ( rim ? zTOL : 0 ) ? true : false;
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

/* inertia tensor of a 3D elliptic cylinder. */
zMat3D *zECyl3DInertia(zECyl3D *cyl, zMat3D *inertia)
{
  double vol, aa, bb, hh;
  zMat3D i, att;

  vol = zECyl3DVolume( cyl );
  /* aligned inertia tensor */
  aa = 0.25 * zSqr( zECyl3DRadius(cyl,0) ) * vol;
  bb = 0.25 * zSqr( zECyl3DRadius(cyl,1) ) * vol;
  zECyl3DAxis( cyl, &att.b.z );
  hh = zVec3DSqrNorm(&att.b.z) * vol / 12.0;
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

/* convert an elliptic cylinder to a polyhedron. */
zPH3D *zECyl3DToPH(zECyl3D *cyl, zPH3D *ph)
{
  zVec3D *vert, d, r;
  zTri3D *face;
  double s, c;
  register int i, j, n;

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

static void _zECyl3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zECyl3DCenter((zECyl3D*)obj,i) ); }
static void _zECyl3DRadiusFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zECyl3DRadius((zECyl3D*)obj,i) ); }
static void _zECyl3DRefFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zECyl3DRadVec((zECyl3D*)obj,0) ); }
static void _zECyl3DDivFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zECyl3DDiv((zECyl3D*)obj) ); }

static ZTKPrp __ztk_prp_shape_ecyl[] = {
  { "center", 2, _zECyl3DCenterFromZTK, _zECyl3DCenterFPrint },
  { "radius", 2, _zECyl3DRadiusFromZTK, _zECyl3DRadiusFPrint },
  { "ref", 1, _zECyl3DRefFromZTK, _zECyl3DRefFPrint },
  { "div", 1, _zECyl3DDivFromZTK, _zECyl3DDivFPrint },
};

/* register a definition of tag-and-keys for a 3D elliptic cylinder to a ZTK format processor. */
bool zECyl3DDefRegZTK(ZTK *ztk, char *tag)
{
  return ZTKDefRegPrp( ztk, tag, __ztk_prp_shape_ecyl );
}

/* read a 3D elliptic cylinder from a ZTK format processor. */
zECyl3D *zECyl3DFromZTK(zECyl3D *ecyl, ZTK *ztk)
{
  zECyl3DInit( ecyl );
  return ZTKEncodeKey( ecyl, NULL, ztk, __ztk_prp_shape_ecyl );
}

typedef struct{
  int ic; /* number of centers */
  int ir; /* number of radi */
  zVec3D c[2]; /* centers */
  double r[2]; /* radi */
  zVec3D ref; /* long radius vector */
  int div; /* number of division */
} _zECylParam;

/* (static)
 * scan a 3D elliptic cylinder (internal function). */
bool _zECyl3DFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  _zECylParam *prm;

  prm = instance;
  if( !strcmp( buf, "center" ) ){
    if( prm->ic > 1 )
      ZRUNWARN( ZEO_ERR_CENTER_MANY );
    else
      zVec3DFScan( fp, &prm->c[prm->ic++] );
  } else if( !strcmp( buf, "radius" ) ){
    if( prm->ir > 1 )
      ZRUNWARN( ZEO_ERR_RADIUS_MANY );
    else
      prm->r[prm->ir++] = zFDouble( fp );
  } else if( !strcmp( buf, "ref" ) ){
    zVec3DFScan( fp, &prm->ref );
  } else if( !strcmp( buf, "div" ) ){
    prm->div = zFInt( fp );
  } else
    return false;
  return true;
}

/* scan a 3D elliptic cylinder from a file. */
zECyl3D *zECyl3DFScan(FILE *fp, zECyl3D *cyl)
{
  _zECylParam prm;

  zECyl3DInit( cyl );
  zVec3DZero( &prm.c[0] );
  zVec3DZero( &prm.c[1] );
  zVec3DZero( &prm.ref );
  prm.r[0] = prm.r[1] = 0;
  prm.ic = prm.ir = prm.div = 0;
  zFieldFScan( fp, _zECyl3DFScan, &prm );
  return zECyl3DCreate( cyl, &prm.c[0], &prm.c[1], prm.r[0], prm.r[1], &prm.ref, prm.div );
}

/* print out a 3D elliptic cylinder to a file. */
void zECyl3DFPrint(FILE *fp, zECyl3D *ecyl)
{
  ZTKPrpKeyFPrint( fp, ecyl, __ztk_prp_shape_ecyl );
}

/* methods for abstraction */

static void *_zShape3DInitECyl(void* shape){
  return zECyl3DInit( shape ); }
static void *_zShape3DAllocECyl(void){
  return zECyl3DAlloc(); }
static void *_zShape3DCloneECyl(void *src){
  zECyl3D *cln;
  return ( cln = zECyl3DAlloc() ) ? zECyl3DCopy( src, cln ) : NULL; }
static void *_zShape3DMirrorECyl(void *src, zAxis axis){
  zECyl3D *mrr;
  return ( mrr = zECyl3DAlloc() ) ? zECyl3DMirror( src, mrr, axis ) : NULL; }
static void _zShape3DDestroyECyl(void *shape){}
static void *_zShape3DXformECyl(void *src, zFrame3D *f, void *dest){
  return zECyl3DXform( src, f, dest ); }
static void *_zShape3DXformInvECyl(void *src, zFrame3D *f, void *dest){
  return zECyl3DXformInv( src, f, dest ); }
static double _zShape3DClosestECyl(void *shape, zVec3D *p, zVec3D *cp){
  return zECyl3DClosest( shape, p, cp ); }
static double _zShape3DPointDistECyl(void *shape, zVec3D *p){
  return zECyl3DPointDist( shape, p ); }
static bool _zShape3DPointIsInsideECyl(void *shape, zVec3D *p, bool rim){
  return zECyl3DPointIsInside( shape, p, rim ); }
static double _zShape3DVolumeECyl(void *shape){
  return zECyl3DVolume( shape ); }
static zVec3D *_zShape3DBarycenterECyl(void *shape, zVec3D *c){
  return zECyl3DBarycenter( shape, c ); }
static zMat3D *_zShape3DInertiaECyl(void *shape, zMat3D *i){
  return zECyl3DInertia( shape, i ); }
static void _zShape3DBaryInertiaECyl(void *shape, zVec3D *c, zMat3D *i){
  zECyl3DBarycenter( shape, c );
  zECyl3DInertia( shape, i ); }
static zPH3D *_zShape3DToPHECyl(void *shape, zPH3D *ph){
  return zECyl3DToPH( shape, ph ); }
static void *_zShape3DParseZTKECyl(void *shape, ZTK *ztk){
  return zECyl3DFromZTK( shape, ztk ); }
static void *_zShape3DFScanECyl(FILE *fp, void *shape){
  return zECyl3DFScan( fp, shape ); }
static void _zShape3DFPrintECyl(FILE *fp, void *shape){
  return zECyl3DFPrint( fp, shape ); }

zShape3DCom zeo_shape_ecyl3d_com = {
  "ellipticcylinder",
  _zShape3DInitECyl,
  _zShape3DAllocECyl,
  _zShape3DCloneECyl,
  _zShape3DMirrorECyl,
  _zShape3DDestroyECyl,
  _zShape3DXformECyl,
  _zShape3DXformInvECyl,
  _zShape3DClosestECyl,
  _zShape3DPointDistECyl,
  _zShape3DPointIsInsideECyl,
  _zShape3DVolumeECyl,
  _zShape3DBarycenterECyl,
  _zShape3DInertiaECyl,
  _zShape3DBaryInertiaECyl,
  _zShape3DToPHECyl,
  _zShape3DParseZTKECyl,
  _zShape3DFScanECyl,
  _zShape3DFPrintECyl,
};

/* create a 3D shape as an elliptic cylinder. */
zShape3D *zShape3DCreateECyl(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div)
{
  zShape3DInit( shape );
  if( !( shape->body = zECyl3DAlloc() ) ) return NULL;
  zECyl3DCreate( zShape3DECyl(shape), c1, c2, r1, r2, ref, div );
  shape->com = &zeo_shape_ecyl3d_com;
  return shape;
}