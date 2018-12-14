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

static bool _zCone3DFRead(FILE *fp, void *instance, char *buf, bool *success);

/* zCone3DCreate
 * - create 3D cone.
 */
zCone3D *zCone3DCreate(zCone3D *cone, zVec3D *c, zVec3D *v, double r, int div)
{
  zCone3DSetCenter( cone, c );
  zCone3DSetVert( cone, v );
  zCone3DSetRadius( cone, r );
  zCone3DSetDiv( cone, div == 0 ? ZEO_PRIM_DEFAULT_DIV : div );
  return cone;
}

/* zCone3DInit
 * - initialize 3D cone.
 */
zCone3D *zCone3DInit(zCone3D *cone)
{
  return zCone3DCreate( cone, ZVEC3DZERO, ZVEC3DZERO, 0, 0 );
}

/* zCone3DCopy
 * - copy 3D cone.
 */
zCone3D *zCone3DCopy(zCone3D *src, zCone3D *dest)
{
  return zCone3DCreate( dest,
    zCone3DCenter(src), zCone3DVert(src), zCone3DRadius(src), zCone3DDiv(src) );
}

/* zCone3DMirror
 * - mirror 3D cone.
 */
zCone3D *zCone3DMirror(zCone3D *src, zCone3D *dest, zAxis axis)
{
  zCone3DCopy( src, dest );
  zCone3DCenter(dest)->e[axis] *= -1;
  zCone3DVert(dest)->e[axis] *= -1;
  return dest;
}

/* zCone3DXfer
 * - transform 3D cone.
 */
zCone3D *zCone3DXfer(zCone3D *src, zFrame3D *f, zCone3D *dest)
{
  zXfer3D( f, zCone3DCenter(src), zCone3DCenter(dest) );
  zXfer3D( f, zCone3DVert(src), zCone3DVert(dest) );
  zCone3DSetRadius( dest, zCone3DRadius(src) );
  zCone3DSetDiv( dest, zCone3DDiv(src) );
  return dest;
}

/* zCone3DXferInv
 * - inversely transform 3D cone.
 */
zCone3D *zCone3DXferInv(zCone3D *src, zFrame3D *f, zCone3D *dest)
{
  zXfer3DInv( f, zCone3DCenter(src), zCone3DCenter(dest) );
  zXfer3DInv( f, zCone3DVert(src), zCone3DVert(dest) );
  zCone3DSetRadius( dest, zCone3DRadius(src) );
  zCone3DSetDiv( dest, zCone3DDiv(src) );
  return dest;
}

/* zCone3DClosest
 * - the closest point to 3D cone.
 */
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

/* zCone3DPointDist
 * - distance from a point to 3D cone.
 */
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

/* zCone3DPointIsInside
 * - check if a point is inside of a cone.
 */
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

/* zCone3DHeight
 * - height of 3D cone.
 */
double zCone3DHeight(zCone3D *cone)
{
  zVec3D axis;

  return zVec3DNorm( zCone3DAxis( cone, &axis ) );
}

/* zCone3DVolume
 * - volume of 3D cone.
 */
double zCone3DVolume(zCone3D *cone)
{
  return zPI * zSqr(zCone3DRadius(cone)) * zCone3DHeight(cone) / 3;
}

/* zCone3DBarycenter
 * - barycenter of 3D cone.
 */
zVec3D *zCone3DBarycenter(zCone3D *cone, zVec3D *c)
{
  return zVec3DInterDiv( zCone3DCenter(cone), zCone3DVert(cone), 0.25, c );
}

/* zCone3DInertia
 * - inertia tensor of 3D cone.
 */
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
  zMulMatMat3DDRC( &att, &i );
  return zMulMatMatT3D( &i, &att, inertia );
}

/* zCone3DToPH
 * - convert cone to polyhedron.
 */
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

/* (static)
 * _zCone3DFRead
 * - input of 3D cone.
 */
bool _zCone3DFRead(FILE *fp, void *instance, char *buf, bool *success)
{
  if( strcmp( buf, "center" ) == 0 ){
    zVec3DFRead( fp, zCone3DCenter( (zCone3D *)instance ) );
  } else if( strcmp( buf, "vert" ) == 0 ){
    zVec3DFRead( fp, zCone3DVert( (zCone3D *)instance ) );
  } else if( strcmp( buf, "radius" ) == 0 ){
    zCone3DSetRadius( (zCone3D *)instance, zFDouble( fp ) );
  } else if( strcmp( buf, "div" ) == 0 ){
    zCone3DSetDiv( (zCone3D *)instance, zFInt( fp ) );
  } else
    return false;
  return true;
}

/* zCone3DFRead
 * - input of 3D cone.
 */
zCone3D *zCone3DFRead(FILE *fp, zCone3D *cone)
{
  zCone3DInit( cone );
  zFieldFRead( fp, _zCone3DFRead, cone );
  return cone;
}

/* zCone3DFWrite
 * - output of 3D cone.
 */
void zCone3DFWrite(FILE *fp, zCone3D *cone)
{
  fprintf( fp, "center: " );
  zVec3DFWrite( fp, zCone3DCenter(cone) );
  fprintf( fp, "vert: " );
  zVec3DFWrite( fp, zCone3DVert(cone) );
  fprintf( fp, "radius: %.10g\n", zCone3DRadius(cone) );
  fprintf( fp, "div: %d\n", zCone3DDiv(cone) );
}

/* methods for abstraction */

static void *_zPrim3DInitCone(void* prim){
  return zCone3DInit( prim ); }
static void *_zPrim3DCloneCone(void *src, void *dest){
  return zCone3DCopy( src, dest ); }
static void *_zPrim3DMirrorCone(void *src, void *dest, zAxis axis){
  return zCone3DMirror( src, dest, axis ); }
static void _zPrim3DDestroyCone(void *prim){}
static void *_zPrim3DXferCone(void *src, zFrame3D *f, void *dest){
  return zCone3DXfer( src, f, dest ); }
static void *_zPrim3DXferInvCone(void *src, zFrame3D *f, void *dest){
  return zCone3DXferInv( src, f, dest ); }
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
static void *_zPrim3DFReadCone(FILE *fp, void *prim){
  return zCone3DFRead( fp, prim ); }
static void _zPrim3DFWriteCone(FILE *fp, void *prim){
  return zCone3DFWrite( fp, prim ); }

zPrimCom zprim_cone3d_com = {
  _zPrim3DInitCone,
  _zPrim3DCloneCone,
  _zPrim3DMirrorCone,
  _zPrim3DDestroyCone,
  _zPrim3DXferCone,
  _zPrim3DXferInvCone,
  _zPrim3DClosestCone,
  _zPrim3DPointDistCone,
  _zPrim3DPointIsInsideCone,
  _zPrim3DVolumeCone,
  _zPrim3DBarycenterCone,
  _zPrim3DInertiaCone,
  _zPrim3DBaryInertiaCone,
  _zPrim3DToPHCone,
  _zPrim3DFReadCone,
  _zPrim3DFWriteCone,
};
