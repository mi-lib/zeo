/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_cyl - primitive 3D shapes: cylinder.
 */

#include <zeo/zeo_prim.h>

/* ********************************************************** */
/* CLASS: zCyl3D
 * 3D cylinder class
 * ********************************************************** */

static bool _zCyl3DFRead(FILE *fp, void *instance, char *buf, bool *success);

/* zCyl3DCreate
 * - create 3D cylinder.
 */
zCyl3D *zCyl3DCreate(zCyl3D *cyl, zVec3D *c1, zVec3D *c2, double r, int div)
{
  zCyl3DSetCenter( cyl, 0, c1 );
  zCyl3DSetCenter( cyl, 1, c2 );
  zCyl3DSetRadius( cyl, r );
  zCyl3DSetDiv( cyl, div == 0 ? ZEO_PRIM_DEFAULT_DIV : div );
  return cyl;
}

/* zCyl3DInit
 * - initialize 3D cylinder.
 */
zCyl3D *zCyl3DInit(zCyl3D *cyl)
{
  return zCyl3DCreate( cyl, ZVEC3DZERO, ZVEC3DZERO, 0, 0 );
}

/* zCyl3DCopy
 * - copy 3D cylinder.
 */
zCyl3D *zCyl3DCopy(zCyl3D *src, zCyl3D *dest)
{
  return zCyl3DCreate( dest,
    zCyl3DCenter(src,0), zCyl3DCenter(src,1),
    zCyl3DRadius(src), zCyl3DDiv(src) );
}

/* zCyl3DMirror
 * - mirror 3D cylinder.
 */
zCyl3D *zCyl3DMirror(zCyl3D *src, zCyl3D *dest, zAxis axis)
{
  zCyl3DCopy( src, dest );
  zCyl3DCenter(dest,0)->e[axis] *= -1;
  zCyl3DCenter(dest,1)->e[axis] *= -1;
  return dest;
}

/* zCyl3DXfer
 * - transform 3D cylinder.
 */
zCyl3D *zCyl3DXfer(zCyl3D *src, zFrame3D *f, zCyl3D *dest)
{
  zXfer3D( f, zCyl3DCenter(src,0), zCyl3DCenter(dest,0) );
  zXfer3D( f, zCyl3DCenter(src,1), zCyl3DCenter(dest,1) );
  zCyl3DSetRadius( dest, zCyl3DRadius(src) );
  zCyl3DSetDiv( dest, zCyl3DDiv(src) );
  return dest;
}

/* zCyl3DXferInv
 * - inversely transform 3D cylinder.
 */
zCyl3D *zCyl3DXferInv(zCyl3D *src, zFrame3D *f, zCyl3D *dest)
{
  zXfer3DInv( f, zCyl3DCenter(src,0), zCyl3DCenter(dest,0) );
  zXfer3DInv( f, zCyl3DCenter(src,1), zCyl3DCenter(dest,1) );
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

/* zCyl3DClosest
 * - the closest point to 3D cylinder.
 */
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

/* zCyl3DPointDist
 * - distance from a point to 3D cylinder.
 */
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

/* zCyl3DPointIsInside
 * - check if a point is inside of a cylinder.
 */
bool zCyl3DPointIsInside(zCyl3D *cyl, zVec3D *p, bool rim)
{
  zVec3D axis, v;
  double l, r, d;

  _zCyl3DClosestPrep( cyl, p, &axis, &v, &l, &r, &d );
  if( r - zCyl3DRadius(cyl) > ( rim ? zTOL : 0 ) ) return false;
  return d >= ( rim ? -zTOL : 0 ) && d <= ( rim ? l+zTOL : l ) ? true : false;
}

/* zCyl3DHeight
 * - height of 3D cylinder.
 */
double zCyl3DHeight(zCyl3D *cyl)
{
  zVec3D axis;

  return zVec3DNorm( zCyl3DAxis( cyl, &axis ) );
}

/* zCyl3DVolume
 * - volume of 3D cylinder.
 */
double zCyl3DVolume(zCyl3D *cyl)
{
  return zPI * zSqr(zCyl3DRadius(cyl)) * zCyl3DHeight(cyl);
}

/* zCyl3DBarycenter
 * - barycenter of 3D cylinder.
 */
zVec3D *zCyl3DBarycenter(zCyl3D *cyl, zVec3D *c)
{
  return zVec3DMid( zCyl3DCenter(cyl,0), zCyl3DCenter(cyl,1), c );
}

/* zCyl3DInertia
 * - inertia tensor of 3D cylinder.
 */
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
  zVec3DOrthoSpace( &att.b.z, &att.b.x, &att.b.y );
  zMulMatMat3DDRC( &att, &i );
  return zMulMatMatT3D( &i, &att, inertia );
}

/* zCyl3DToPH
 * - convert cylinder to polyhedron.
 */
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
    ZRUNERROR( ZEO_ERR_PRIM_ZEROC );
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

typedef struct{
  zVec3D c[2];
  int ic;
  double r;
  int div;
} _zCyl3DParam;

/* (static)
 * _zCyl3DFRead
 * - input of 3D cylinder.
 */
bool _zCyl3DFRead(FILE *fp, void *instance, char *buf, bool *success)
{
  _zCyl3DParam *prm;

  prm = instance;
  if( strcmp( buf, "center" ) == 0 ){
    if( prm->ic > 1 )
      ZRUNWARN( ZEO_ERR_CENTER_MANY );
    else
      zVec3DFRead( fp, &prm->c[prm->ic++] );
  } else if( strcmp( buf, "radius" ) == 0 ){
    prm->r = zFDouble( fp );
  } else if( strcmp( buf, "div" ) == 0 ){
    prm->div = zFInt( fp );
  } else
    return false;
  return true;
}

/* zCyl3DFRead
 * - input of 3D cylinder.
 */
zCyl3D *zCyl3DFRead(FILE *fp, zCyl3D *cyl)
{
  _zCyl3DParam prm;

  zCyl3DInit( cyl );
  zVec3DClear( &prm.c[0] );
  zVec3DClear( &prm.c[1] );
  prm.ic = 0;
  prm.r = 0;
  prm.div = 0;
  zFieldFRead( fp, _zCyl3DFRead, &prm );
  return zCyl3DCreate( cyl, &prm.c[0], &prm.c[1], prm.r, prm.div );
}

/* zCyl3DFWrite
 * - output of 3D cylinder.
 */
void zCyl3DFWrite(FILE *fp, zCyl3D *cyl)
{
  fprintf( fp, "center: " );
  zVec3DFWrite( fp, zCyl3DCenter(cyl,0) );
  fprintf( fp, "center: " );
  zVec3DFWrite( fp, zCyl3DCenter(cyl,1) );
  fprintf( fp, "radius: %.10g\n", zCyl3DRadius(cyl) );
  fprintf( fp, "div: %d\n", zCyl3DDiv(cyl) );
}

/* methods for abstraction */

static void *_zPrim3DInitCyl(void* prim){
  return zCyl3DInit( prim ); }
static void *_zPrim3DCloneCyl(void *src, void *dest){
  return zCyl3DCopy( src, dest ); }
static void *_zPrim3DMirrorCyl(void *src, void *dest, zAxis axis){
  return zCyl3DMirror( src, dest, axis ); }
static void _zPrim3DDestroyCyl(void *prim){}
static void *_zPrim3DXferCyl(void *src, zFrame3D *f, void *dest){
  return zCyl3DXfer( src, f, dest ); }
static void *_zPrim3DXferInvCyl(void *src, zFrame3D *f, void *dest){
  return zCyl3DXferInv( src, f, dest ); }
static double _zPrim3DClosestCyl(void *prim, zVec3D *p, zVec3D *cp){
  return zCyl3DClosest( prim, p, cp ); }
static double _zPrim3DPointDistCyl(void *prim, zVec3D *p){
  return zCyl3DPointDist( prim, p ); }
static bool _zPrim3DPointIsInsideCyl(void *prim, zVec3D *p, bool rim){
  return zCyl3DPointIsInside( prim, p, rim ); }
static double _zPrim3DVolumeCyl(void *prim){
  return zCyl3DVolume( prim ); }
static zVec3D *_zPrim3DBarycenterCyl(void *prim, zVec3D *c){
  return zCyl3DBarycenter( prim, c ); }
static zMat3D *_zPrim3DInertiaCyl(void *prim, zMat3D *i){
  return zCyl3DInertia( prim, i ); }
static void _zPrim3DBaryInertiaCyl(void *prim, zVec3D *c, zMat3D *i){
  zCyl3DBarycenter( prim, c );
  zCyl3DInertia( prim, i ); }
static zPH3D *_zPrim3DToPHCyl(void *prim, zPH3D *ph){
  return zCyl3DToPH( prim, ph ); }
static void *_zPrim3DFReadCyl(FILE *fp, void *prim){
  return zCyl3DFRead( fp, prim ); }
static void _zPrim3DFWriteCyl(FILE *fp, void *prim){
  return zCyl3DFWrite( fp, prim ); }

zPrimCom zprim_cyl3d_com = {
  _zPrim3DInitCyl,
  _zPrim3DCloneCyl,
  _zPrim3DMirrorCyl,
  _zPrim3DDestroyCyl,
  _zPrim3DXferCyl,
  _zPrim3DXferInvCyl,
  _zPrim3DClosestCyl,
  _zPrim3DPointDistCyl,
  _zPrim3DPointIsInsideCyl,
  _zPrim3DVolumeCyl,
  _zPrim3DBarycenterCyl,
  _zPrim3DInertiaCyl,
  _zPrim3DBaryInertiaCyl,
  _zPrim3DToPHCyl,
  _zPrim3DFReadCyl,
  _zPrim3DFWriteCyl,
};
