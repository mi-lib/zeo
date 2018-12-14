/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_box - primitive 3D shapes: box.
 */

#include <zeo/zeo_prim.h>

/* ********************************************************** */
/* CLASS: zBox3D
 * 3D box class
 * ********************************************************** */

static bool _zBox3DFRead(FILE *fp, void *instance, char *buf, bool *success);

/* zBox3DCreate
 * - create 3D box.
 */
zBox3D *zBox3DCreate(zBox3D *box, zVec3D *c, zVec3D *ax, zVec3D *ay, zVec3D *az, double d, double w, double h)
{
  zBox3DSetCenter( box, c );
  zBox3DSetAxis( box, 0, ax );
  zBox3DSetAxis( box, 1, ay );
  zBox3DSetAxis( box, 2, az );
  zBox3DSetDepth( box, fabs(d) );
  zBox3DSetWidth( box, fabs(w) );
  zBox3DSetHeight( box, fabs(h) );
  return box;
}

/* zBox3DInit
 * - initialize 3D box.
 */
zBox3D *zBox3DInit(zBox3D *box)
{
  return zBox3DCreateAlign( box, ZVEC3DZERO, 0, 0, 0 );
}

/* zBox3DCopy
 * - copy 3D box.
 */
zBox3D *zBox3DCopy(zBox3D *src, zBox3D *dest)
{
  return zBox3DCreate( dest, zBox3DCenter(src),
    zBox3DAxis(src,0), zBox3DAxis(src,1), zBox3DAxis(src,2),
    zBox3DDepth(src), zBox3DWidth(src), zBox3DHeight(src) );
}

/* zBox3DMirror
 * - mirror 3D box.
 */
zBox3D *zBox3DMirror(zBox3D *src, zBox3D *dest, zAxis axis)
{
  zBox3DCopy( src, dest );
  zBox3DCenter(dest)->e[axis] *= -1;
  zBox3DAxis(dest,0)->e[axis] *= -1;
  zBox3DAxis(dest,1)->e[axis] *= -1;
  zBox3DAxis(dest,2)->e[axis] *= -1;
  return dest;
}

/* zBox3DXfer
 * - transfer 3D box.
 */
zBox3D *zBox3DXfer(zBox3D *src, zFrame3D *f, zBox3D *dest)
{
  zXfer3D( f, zBox3DCenter(src), zBox3DCenter(dest) );
  zMulMatVec3D( zFrame3DAtt(f), zBox3DAxis(src,zX), zBox3DAxis(dest,zX) );
  zMulMatVec3D( zFrame3DAtt(f), zBox3DAxis(src,zY), zBox3DAxis(dest,zY) );
  zMulMatVec3D( zFrame3DAtt(f), zBox3DAxis(src,zZ), zBox3DAxis(dest,zZ) );
  zBox3DSetDepth( dest, zBox3DDepth(src) );
  zBox3DSetWidth( dest, zBox3DWidth(src) );
  zBox3DSetHeight( dest, zBox3DHeight(src) );
  return dest;
}

/* zBox3DXfer
 * - inversely transfer 3D box.
 */
zBox3D *zBox3DXferInv(zBox3D *src, zFrame3D *f, zBox3D *dest)
{
  zXfer3DInv( f, zBox3DCenter(src), zBox3DCenter(dest) );
  zMulMatTVec3D( zFrame3DAtt(f), zBox3DAxis(src,zX), zBox3DAxis(dest,zX) );
  zMulMatTVec3D( zFrame3DAtt(f), zBox3DAxis(src,zY), zBox3DAxis(dest,zY) );
  zMulMatTVec3D( zFrame3DAtt(f), zBox3DAxis(src,zZ), zBox3DAxis(dest,zZ) );
  zBox3DSetDepth( dest, zBox3DDepth(src) );
  zBox3DSetWidth( dest, zBox3DWidth(src) );
  zBox3DSetHeight( dest, zBox3DHeight(src) );
  return dest;
}

/* zBox3DClosest
 * - the closest point from 3D point to 3D box.
 */
double zBox3DClosest(zBox3D *box, zVec3D *p, zVec3D *cp)
{
  zVec3D _p;
  double min, max;
  register zDir d;

  zXfer3DInv( &box->f, p, &_p );
  for( d=zX; d<=zZ; d++ ){
    min =-0.5 * zBox3DDia(box,d);
    max = 0.5 * zBox3DDia(box,d);
    cp->e[d] = zLimit( _p.e[d], min, max );
  }
  zXfer3DDRC( &box->f, cp );
  return zVec3DDist( p, cp );
}

/* zBox3DPointDist
 * - distance from a point to 3D box.
 */
double zBox3DPointDist(zBox3D *box, zVec3D *p)
{
  zVec3D cp;
  return zBox3DClosest( box, p, &cp );
}

/* zBox3DPointIsInside
 * - check if a point is in a box.
 */
bool zBox3DPointIsInside(zBox3D *box, zVec3D *p, bool rim)
{
  register zDir d;
  zVec3D err;
  double l;

  zXfer3DInv( &box->f, p, &err );
  for( d=zX; d<=zZ; d++ ){
    l = 0.5 * zBox3DDia(box,d);
    if( rim ) l += zTOL;
    if( err.e[d] > l || err.e[d] < -l )
      return false;
  }
  return true;
}

/* zBox3DVolume
 * - volume of 3D box.
 */
double zBox3DVolume(zBox3D *box)
{
  return zBox3DDepth(box) * zBox3DWidth(box) * zBox3DHeight(box);
}

/* zBox3DInertia
 * - inertia of 3D box.
 */
zMat3D *zBox3DInertia(zBox3D *box, zMat3D *inertia)
{
  zMat3D i;
  double xx, yy, zz, c;

  c = zBox3DVolume( box ) / 12;
  xx = zSqr( zBox3DDepth(box) ) * c;
  yy = zSqr( zBox3DWidth(box) ) * c;
  zz = zSqr( zBox3DHeight(box) ) * c;
  zMat3DCreate( &i,
    yy+zz, 0, 0,
    0, zz+xx, 0,
    0, 0, xx+yy );
  zMulMatMat3DDRC( zFrame3DAtt(&box->f), &i );
  return zMulMatMatT3D( &i, zFrame3DAtt(&box->f), inertia );
}

/* zBox3DVert
 * - get vertex of a box.
 */
zVec3D *zBox3DVert(zBox3D *box, int i, zVec3D *v)
{
  _zVec3DCreate( v,
    ( (i&0x1)^(i>>1&0x1) ) ? -0.5*zBox3DDepth(box) : 0.5*zBox3DDepth(box),
    (  i&0x2             ) ? -0.5*zBox3DWidth(box) : 0.5*zBox3DWidth(box),
    (  i&0x4             ) ? -0.5*zBox3DHeight(box): 0.5*zBox3DHeight(box) );
  return zXfer3DDRC( &box->f, v );
}

#define __zBox3DToPH_tri(p,i,i1,i2,i3,i4) do{\
  zTri3DCreate( zPH3DFace(p,i),   zPH3DVert(p,i1), zPH3DVert(p,i2), zPH3DVert(p,i3) );\
  zTri3DCreate( zPH3DFace(p,i+1), zPH3DVert(p,i1), zPH3DVert(p,i3), zPH3DVert(p,i4) );\
} while(0)

/* zBox3DToPH
 * - convert box to polyhedron.
 */
zPH3D* zBox3DToPH(zBox3D *box, zPH3D *ph)
{
  register int i;

  if( !zPH3DAlloc( ph, 8, 12 ) ) return NULL;
  for( i=0; i<8; i++ )
    zBox3DVert( box, i, zPH3DVert(ph,i) );
  __zBox3DToPH_tri( ph, 0, 0, 1, 2, 3 );
  __zBox3DToPH_tri( ph, 2, 0, 4, 5, 1 );
  __zBox3DToPH_tri( ph, 4, 1, 5, 6, 2 );
  __zBox3DToPH_tri( ph, 6, 2, 6, 7, 3 );
  __zBox3DToPH_tri( ph, 8, 3, 7, 4, 0 );
  __zBox3DToPH_tri( ph,10, 7, 6, 5, 4 );
  return ph;
}

/* (static)
 * _zBox3DFRead
 * - input of 3D box.
 */
bool _zBox3DFRead(FILE *fp, void *instance, char *buf, bool *success)
{
  zVec3D a;

  if( strcmp( buf, "center" ) == 0 )
    zVec3DFRead( fp, zBox3DCenter( (zBox3D *)instance ) );
  else if( strcmp( buf, "ax" ) == 0 ){
    zVec3DFRead( fp, &a );
    zVec3DNormalize( &a, zBox3DAxis( (zBox3D *)instance, zX ) );
  } else if( strcmp( buf, "ay" ) == 0 ){
    zVec3DFRead( fp, &a );
    zVec3DNormalize( &a, zBox3DAxis( (zBox3D *)instance, zY ) );
  } else if( strcmp( buf, "az" ) == 0 ){
    zVec3DFRead( fp, &a );
    zVec3DNormalize( &a, zBox3DAxis( (zBox3D *)instance, zZ ) );
  } else if( strcmp( buf, "depth" ) == 0 )
    zBox3DSetDepth( (zBox3D *)instance, zFDouble( fp ) );
  else if( strcmp( buf, "width" ) == 0 )
    zBox3DSetWidth( (zBox3D *)instance, zFDouble( fp ) );
  else if( strcmp( buf, "height" ) == 0 )
    zBox3DSetHeight( (zBox3D *)instance, zFDouble( fp ) );
  else
    return false;
  return true;
}

/* zBox3DFRead
 * - input of 3D box.
 */
zBox3D *zBox3DFRead(FILE *fp, zBox3D *box)
{
  zBox3DInit( box );
  zFieldFRead( fp, _zBox3DFRead, box );
  return box;
}

/* zBox3DFWrite
 * - output a 3D box to a file.
 */
void zBox3DFWrite(FILE *fp, zBox3D *box)
{
  fprintf( fp, "center: " );
  zVec3DFWrite( fp, zBox3DCenter( box ) );
  fprintf( fp, "ax: " );
  zVec3DFWrite( fp, zBox3DAxis( box, zX ) );
  fprintf( fp, "ay: " );
  zVec3DFWrite( fp, zBox3DAxis( box, zY ) );
  fprintf( fp, "az: " );
  zVec3DFWrite( fp, zBox3DAxis( box, zZ ) );
  fprintf( fp, "depth: %.10g\n", zBox3DDepth( box ) );
  fprintf( fp, "width: %.10g\n", zBox3DWidth( box ) );
  fprintf( fp, "height: %.10g\n", zBox3DHeight( box ) );
}

/* zBox3DDataFWrite
 * - output a 3D box to a file in a format to be plotted.
 */
void zBox3DDataFWrite(FILE *fp, zBox3D *box)
{
  zVec3D v[8];
  register int i;

  for( i=0; i<8; i++ )
    zBox3DVert( box, i, &v[i] );
  zVec3DDataNLFWrite( fp, &v[0] );
  zVec3DDataNLFWrite( fp, &v[1] );
  zVec3DDataNLFWrite( fp, &v[2] );
  zVec3DDataNLFWrite( fp, &v[3] );
  zVec3DDataNLFWrite( fp, &v[0] );
  zVec3DDataNLFWrite( fp, &v[4] );
  zVec3DDataNLFWrite( fp, &v[5] );
  zVec3DDataNLFWrite( fp, &v[6] );
  zVec3DDataNLFWrite( fp, &v[7] );
  zVec3DDataNLFWrite( fp, &v[4] );
  fprintf( fp, "\n" );
  zVec3DDataNLFWrite( fp, &v[1] );
  zVec3DDataNLFWrite( fp, &v[5] );
  fprintf( fp, "\n" );
  zVec3DDataNLFWrite( fp, &v[2] );
  zVec3DDataNLFWrite( fp, &v[6] );
  fprintf( fp, "\n" );
  zVec3DDataNLFWrite( fp, &v[3] );
  zVec3DDataNLFWrite( fp, &v[7] );
  fprintf( fp, "\n\n" );
}

/* methods for abstraction */

static void *_zPrim3DInitBox(void* prim){
  return zBox3DInit( prim ); }
static void *_zPrim3DCloneBox(void *src, void *dest){
  return zBox3DCopy( src, dest ); }
static void *_zPrim3DMirrorBox(void *src, void *dest, zAxis axis){
  return zBox3DMirror( src, dest, axis ); }
static void _zPrim3DDestroyBox(void *prim){}
static void *_zPrim3DXferBox(void *src, zFrame3D *f, void *dest){
  return zBox3DXfer( src, f, dest ); }
static void *_zPrim3DXferInvBox(void *src, zFrame3D *f, void *dest){
  return zBox3DXferInv( src, f, dest ); }
static double _zPrim3DClosestBox(void *prim, zVec3D *p, zVec3D *cp){
  return zBox3DClosest( prim, p, cp ); }
static double _zPrim3DPointDistBox(void *prim, zVec3D *p){
  return zBox3DPointDist( prim, p ); }
static bool _zPrim3DPointIsInsideBox(void *prim, zVec3D *p, bool rim){
  return zBox3DPointIsInside( prim, p, rim ); }
static double _zPrim3DVolumeBox(void *prim){
  return zBox3DVolume( prim ); }
static zVec3D *_zPrim3DBarycenterBox(void *prim, zVec3D *c){
  zVec3DCopy( zBox3DCenter((zBox3D*)prim), c ); return c; }
static zMat3D *_zPrim3DInertiaBox(void *prim, zMat3D *i){
  return zBox3DInertia( prim, i ); }
static void _zPrim3DBaryInertiaBox(void *prim, zVec3D *c, zMat3D *i){
  zVec3DCopy( zBox3DCenter((zBox3D*)prim), c );
  zBox3DInertia( prim, i ); }
static zPH3D *_zPrim3DToPHBox(void *prim, zPH3D *ph){
  return zBox3DToPH( prim, ph ); }
static void *_zPrim3DFReadBox(FILE *fp, void *prim){
  return zBox3DFRead( fp, prim ); }
static void _zPrim3DFWriteBox(FILE *fp, void *prim){
  return zBox3DFWrite( fp, prim ); }

zPrimCom zprim_box3d_com = {
  _zPrim3DInitBox,
  _zPrim3DCloneBox,
  _zPrim3DMirrorBox,
  _zPrim3DDestroyBox,
  _zPrim3DXferBox,
  _zPrim3DXferInvBox,
  _zPrim3DClosestBox,
  _zPrim3DPointDistBox,
  _zPrim3DPointIsInsideBox,
  _zPrim3DVolumeBox,
  _zPrim3DBarycenterBox,
  _zPrim3DInertiaBox,
  _zPrim3DBaryInertiaBox,
  _zPrim3DToPHBox,
  _zPrim3DFReadBox,
  _zPrim3DFWriteBox,
};
