/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_ecyl - primitive 3D shapes: elliptic cylinder.
 */

#include <zeo/zeo_prim.h>

/* ********************************************************** */
/* CLASS: zECyl3D
 * 3D elliptic cylinder class
 * ********************************************************** */

static bool _zECyl3DFRead(FILE *fp, void *instance, char *buf, bool *success);

/* zECyl3DCreate
 * - create 3D elliptic cylinder.
 */
zECyl3D *zECyl3DCreate(zECyl3D *cyl, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div)
{
  zVec3D axis;

  zECyl3DSetCenter( cyl, 0, c1 );
  zECyl3DSetCenter( cyl, 1, c2 );
  zECyl3DSetRadius( cyl, 0, r1 );
  zECyl3DSetRadius( cyl, 1, r2 );
  zECyl3DAxis( cyl, &axis );
  if( ref && !zVec3DIsTiny(ref) ){
    zVec3DOrthogonalize( ref, &axis, zECyl3DRadVec(cyl,0) );
    zVec3DNormalizeDRC( zECyl3DRadVec(cyl,0) );
    zVec3DOuterProd( &axis, zECyl3DRadVec(cyl,0), zECyl3DRadVec(cyl,1) );
    zVec3DNormalizeDRC( zECyl3DRadVec(cyl,1) );
  } else{
    zVec3DOrthoSpace( &axis, zECyl3DRadVec(cyl,0), zECyl3DRadVec(cyl,1) );
  }
  zECyl3DSetDiv( cyl, div == 0 ? ZEO_PRIM_DEFAULT_DIV : div );
  return cyl;
}

/* zECyl3DInit
 * - initialize 3D elliptic cylinder.
 */
zECyl3D *zECyl3DInit(zECyl3D *cyl)
{
  zVec3DClear( zECyl3DCenter(cyl,0) );
  zVec3DClear( zECyl3DCenter(cyl,1) );
  zECyl3DSetRadius( cyl, 0, 0 );
  zECyl3DSetRadius( cyl, 1, 0 );
  zVec3DClear( zECyl3DRadVec(cyl,0) );
  zVec3DClear( zECyl3DRadVec(cyl,1) );
  zECyl3DSetDiv( cyl, ZEO_PRIM_DEFAULT_DIV );
  return cyl;
}

/* zECyl3DCopy
 * - copy 3D elliptic cylinder.
 */
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

/* zECyl3DMirror
 * - mirror 3D elliptic cylinder.
 */
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

/* zECyl3DXfer
 * - transform 3D elliptic cylinder.
 */
zECyl3D *zECyl3DXfer(zECyl3D *src, zFrame3D *f, zECyl3D *dest)
{
  zXfer3D( f, zECyl3DCenter(src,0), zECyl3DCenter(dest,0) );
  zXfer3D( f, zECyl3DCenter(src,1), zECyl3DCenter(dest,1) );
  zECyl3DSetRadius( dest, 0, zECyl3DRadius(src,0) );
  zECyl3DSetRadius( dest, 1, zECyl3DRadius(src,1) );
  zMulMat3DVec3D( zFrame3DAtt(f), zECyl3DRadVec(src,0), zECyl3DRadVec(dest,0) );
  zMulMat3DVec3D( zFrame3DAtt(f), zECyl3DRadVec(src,1), zECyl3DRadVec(dest,1) );
  zECyl3DSetDiv( dest, zECyl3DDiv(src) );
  return dest;
}

/* zECyl3DXferInv
 * - inversely transform 3D elliptic cylinder.
 */
zECyl3D *zECyl3DXferInv(zECyl3D *src, zFrame3D *f, zECyl3D *dest)
{
  zXfer3DInv( f, zECyl3DCenter(src,0), zECyl3DCenter(dest,0) );
  zXfer3DInv( f, zECyl3DCenter(src,1), zECyl3DCenter(dest,1) );
  zECyl3DSetRadius( dest, 0, zECyl3DRadius(src,0) );
  zECyl3DSetRadius( dest, 1, zECyl3DRadius(src,1) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zECyl3DRadVec(src,0), zECyl3DRadVec(dest,0) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zECyl3DRadVec(src,1), zECyl3DRadVec(dest,1) );
  zECyl3DSetDiv( dest, zECyl3DDiv(src) );
  return dest;
}

/* zECyl3DClosest
 * - the closest point to 3D elliptic cylinder.
 */
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

/* zECyl3DPointDist
 * - distance from a point to 3D elliptic cylinder.
 */
double zECyl3DPointDist(zECyl3D *cyl, zVec3D *p)
{
  zVec3D cp;

  return zECyl3DClosest( cyl, p, &cp );
}

/* zECyl3DPointIsInside
 * - check if a point is inside of an elliptic cylinder.
 */
bool zECyl3DPointIsInside(zECyl3D *cyl, zVec3D *p, bool rim)
{
  return zECyl3DPointDist( cyl, p ) < ( rim ? zTOL : 0 ) ? true : false;
}

/* zECyl3DHeight
 * - height of 3D elliptic cylinder.
 */
double zECyl3DHeight(zECyl3D *cyl)
{
  zVec3D axis;

  return zVec3DNorm( zECyl3DAxis( cyl, &axis ) );
}

/* zECyl3DVolume
 * - volume of 3D elliptic cylinder.
 */
double zECyl3DVolume(zECyl3D *cyl)
{
  return zPI * zECyl3DRadius(cyl,0) * zECyl3DRadius(cyl,1) * zECyl3DHeight(cyl);
}

/* zECyl3DBarycenter
 * - barycenter of 3D elliptic cylinder.
 */
zVec3D *zECyl3DBarycenter(zECyl3D *cyl, zVec3D *c)
{
  return zVec3DMid( zECyl3DCenter(cyl,0), zECyl3DCenter(cyl,1), c );
}

/* zECyl3DInertia
 * - inertia tensor of 3D elliptic cylinder.
 */
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

/* zECyl3DToPH
 * - convert elliptic cylinder to polyhedron.
 */
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
    ZRUNERROR( ZEO_ERR_PRIM_ZEROC );
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

typedef struct{
  int ic; /* number of centers */
  int ir; /* number of radi */
  zVec3D c[2]; /* centers */
  double r[2]; /* radi */
  zVec3D ref; /* long radius vector */
  int div; /* number of division */
} _zECylParam;

/* (static)
 * _zECyl3DFRead
 * - input of 3D elliptic cylinder.
 */
bool _zECyl3DFRead(FILE *fp, void *instance, char *buf, bool *success)
{
  _zECylParam *prm;

  prm = instance;
  if( !strcmp( buf, "center" ) ){
    if( prm->ic > 1 )
      ZRUNWARN( ZEO_ERR_CENTER_MANY );
    else
      zVec3DFRead( fp, &prm->c[prm->ic++] );
  } else if( !strcmp( buf, "radius" ) ){
    if( prm->ir > 1 )
      ZRUNWARN( ZEO_ERR_RADIUS_MANY );
    else
      prm->r[prm->ir++] = zFDouble( fp );
  } else if( !strcmp( buf, "ref" ) ){
    zVec3DFRead( fp, &prm->ref );
  } else if( !strcmp( buf, "div" ) ){
    prm->div = zFInt( fp );
  } else
    return false;
  return true;
}

/* zECyl3DFRead
 * - input of 3D elliptic cylinder.
 */
zECyl3D *zECyl3DFRead(FILE *fp, zECyl3D *cyl)
{
  _zECylParam prm;

  zECyl3DInit( cyl );
  zVec3DClear( &prm.c[0] );
  zVec3DClear( &prm.c[1] );
  zVec3DClear( &prm.ref );
  prm.r[0] = prm.r[1] = 0;
  prm.ic = prm.ir = prm.div = 0;
  zFieldFRead( fp, _zECyl3DFRead, &prm );
  return zECyl3DCreate( cyl, &prm.c[0], &prm.c[1], prm.r[0], prm.r[1], &prm.ref, prm.div );
}

/* zECyl3DFWrite
 * - output of 3D elliptic cylinder.
 */
void zECyl3DFWrite(FILE *fp, zECyl3D *cyl)
{
  fprintf( fp, "center: " );
  zVec3DFWrite( fp, zECyl3DCenter(cyl,0) );
  fprintf( fp, "center: " );
  zVec3DFWrite( fp, zECyl3DCenter(cyl,1) );
  fprintf( fp, "radius: %.10g\n", zECyl3DRadius(cyl,0) );
  fprintf( fp, "radius: %.10g\n", zECyl3DRadius(cyl,1) );
  fprintf( fp, "ref: " );
  zVec3DFWrite( fp, zECyl3DRadVec(cyl,0) );
  fprintf( fp, "div: %d\n", zECyl3DDiv(cyl) );
}

/* methods for abstraction */

static void *_zPrim3DInitECyl(void* prim){
  return zECyl3DInit( prim ); }
static void *_zPrim3DCloneECyl(void *src, void *dest){
  return zECyl3DCopy( src, dest ); }
static void *_zPrim3DMirrorECyl(void *src, void *dest, zAxis axis){
  return zECyl3DMirror( src, dest, axis ); }
static void _zPrim3DDestroyECyl(void *prim){}
static void *_zPrim3DXferECyl(void *src, zFrame3D *f, void *dest){
  return zECyl3DXfer( src, f, dest ); }
static void *_zPrim3DXferInvECyl(void *src, zFrame3D *f, void *dest){
  return zECyl3DXferInv( src, f, dest ); }
static double _zPrim3DClosestECyl(void *prim, zVec3D *p, zVec3D *cp){
  return zECyl3DClosest( prim, p, cp ); }
static double _zPrim3DPointDistECyl(void *prim, zVec3D *p){
  return zECyl3DPointDist( prim, p ); }
static bool _zPrim3DPointIsInsideECyl(void *prim, zVec3D *p, bool rim){
  return zECyl3DPointIsInside( prim, p, rim ); }
static double _zPrim3DVolumeECyl(void *prim){
  return zECyl3DVolume( prim ); }
static zVec3D *_zPrim3DBarycenterECyl(void *prim, zVec3D *c){
  return zECyl3DBarycenter( prim, c ); }
static zMat3D *_zPrim3DInertiaECyl(void *prim, zMat3D *i){
  return zECyl3DInertia( prim, i ); }
static void _zPrim3DBaryInertiaECyl(void *prim, zVec3D *c, zMat3D *i){
  zECyl3DBarycenter( prim, c );
  zECyl3DInertia( prim, i ); }
static zPH3D *_zPrim3DToPHECyl(void *prim, zPH3D *ph){
  return zECyl3DToPH( prim, ph ); }
static void *_zPrim3DFReadECyl(FILE *fp, void *prim){
  return zECyl3DFRead( fp, prim ); }
static void _zPrim3DFWriteECyl(FILE *fp, void *prim){
  return zECyl3DFWrite( fp, prim ); }

zPrimCom zprim_ecyl3d_com = {
  _zPrim3DInitECyl,
  _zPrim3DCloneECyl,
  _zPrim3DMirrorECyl,
  _zPrim3DDestroyECyl,
  _zPrim3DXferECyl,
  _zPrim3DXferInvECyl,
  _zPrim3DClosestECyl,
  _zPrim3DPointDistECyl,
  _zPrim3DPointIsInsideECyl,
  _zPrim3DVolumeECyl,
  _zPrim3DBarycenterECyl,
  _zPrim3DInertiaECyl,
  _zPrim3DBaryInertiaECyl,
  _zPrim3DToPHECyl,
  _zPrim3DFReadECyl,
  _zPrim3DFWriteECyl,
};
