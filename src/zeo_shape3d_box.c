/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_box - 3D shapes: box.
 */

#include <zeo/zeo_shape3d.h>

/* create a 3D box. */
zBox3D *zBox3DCreate(zBox3D *box, const zVec3D *c, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double d, double w, double h)
{
  zVec3D ux, uy, uz;

  zBox3DSetCenter( box, c );
  zVec3DNormalize( ax, &ux ); zBox3DSetAxis( box, 0, &ux );
  zVec3DNormalize( ay, &uy ); zBox3DSetAxis( box, 1, &uy );
  zVec3DNormalize( az, &uz ); zBox3DSetAxis( box, 2, &uz );
  zBox3DSetDepth( box, fabs(d) );
  zBox3DSetWidth( box, fabs(w) );
  zBox3DSetHeight( box, fabs(h) );
  return box;
}

/* initialize a 3D box. */
zBox3D *zBox3DInit(zBox3D *box)
{
  return zBox3DCreateAlign( box, ZVEC3DZERO, 0, 0, 0 );
}

/* allocate memory for a 3D box. */
ZDEF_ALLOC_FUNCTION( zBox3D )

/* copy a 3D box to another. */
zBox3D *zBox3DCopy(const zBox3D *src, zBox3D *dest)
{
  return zBox3DCreate( dest, zBox3DCenter(src),
    zBox3DAxis(src,0), zBox3DAxis(src,1), zBox3DAxis(src,2),
    zBox3DDepth(src), zBox3DWidth(src), zBox3DHeight(src) );
}

/* mirror a 3D box along an axis. */
zBox3D *zBox3DMirror(const zBox3D *src, zBox3D *dest, zAxis axis)
{
  zBox3DCopy( src, dest );
  zBox3DCenter(dest)->e[(int)axis] *= -1;
  zBox3DAxis(dest,0)->e[(int)axis] *= -1;
  zBox3DAxis(dest,1)->e[(int)axis] *= -1;
  zBox3DAxis(dest,2)->e[(int)axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D box. */
zBox3D *zBox3DXform(const zBox3D *src, const zFrame3D *f, zBox3D *dest)
{
  zXform3D( f, zBox3DCenter(src), zBox3DCenter(dest) );
  zMulMat3DVec3D( zFrame3DAtt(f), zBox3DAxis(src,zX), zBox3DAxis(dest,zX) );
  zMulMat3DVec3D( zFrame3DAtt(f), zBox3DAxis(src,zY), zBox3DAxis(dest,zY) );
  zMulMat3DVec3D( zFrame3DAtt(f), zBox3DAxis(src,zZ), zBox3DAxis(dest,zZ) );
  zBox3DSetDepth( dest, zBox3DDepth(src) );
  zBox3DSetWidth( dest, zBox3DWidth(src) );
  zBox3DSetHeight( dest, zBox3DHeight(src) );
  return dest;
}

/* inversely transform coordinates of a 3D box. */
zBox3D *zBox3DXformInv(const zBox3D *src, const zFrame3D *f, zBox3D *dest)
{
  zXform3DInv( f, zBox3DCenter(src), zBox3DCenter(dest) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zBox3DAxis(src,zX), zBox3DAxis(dest,zX) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zBox3DAxis(src,zY), zBox3DAxis(dest,zY) );
  zMulMat3DTVec3D( zFrame3DAtt(f), zBox3DAxis(src,zZ), zBox3DAxis(dest,zZ) );
  zBox3DSetDepth( dest, zBox3DDepth(src) );
  zBox3DSetWidth( dest, zBox3DWidth(src) );
  zBox3DSetHeight( dest, zBox3DHeight(src) );
  return dest;
}

/* the closest point from a 3D point to a 3D box. */
double zBox3DClosest(const zBox3D *box, const zVec3D *p, zVec3D *cp)
{
  zVec3D _p;
  double min, max;
  zDir d;

  zXform3DInv( &box->f, p, &_p );
  for( d=zX; d<=zZ; d++ ){
    min =-0.5 * zBox3DDia(box,(int)d);
    max = 0.5 * zBox3DDia(box,(int)d);
    cp->e[(int)d] = _zLimit( _p.e[(int)d], min, max );
  }
  zXform3DDRC( &box->f, cp );
  return zVec3DDist( p, cp );
}

/* distance from a point to a 3D box. */
double zBox3DDistFromPoint(const zBox3D *box, const zVec3D *p)
{
  zVec3D cp;
  return zBox3DClosest( box, p, &cp );
}

/* check if a point is inside of a box. */
bool zBox3DPointIsInside(const zBox3D *box, const zVec3D *p, double margin)
{
  zDir dir;
  zVec3D pl;
  double l;

  zXform3DInv( &box->f, p, &pl );
  for( dir=zX; dir<=zZ; dir++ ){
    l = 0.5 * zBox3DDia(box,(int)dir) + margin;
    if( pl.e[(int)dir] >= l || pl.e[(int)dir] <= -l ) return false;
  }
  return true;
}

/* volume of a 3D box. */
double zBox3DVolume(const zBox3D *box)
{
  return zBox3DDepth(box) * zBox3DWidth(box) * zBox3DHeight(box);
}

/* inertia tensor about barycenter of a 3D box from mass. */
zMat3D *zBox3DBaryInertiaMass(const zBox3D *box, double mass, zMat3D *inertia)
{
  zMat3D i;
  double xx, yy, zz, c;

  c = mass / 12;
  xx = zSqr( zBox3DDepth(box) ) * c;
  yy = zSqr( zBox3DWidth(box) ) * c;
  zz = zSqr( zBox3DHeight(box) ) * c;
  zMat3DCreate( &i,
    yy+zz, 0, 0,
    0, zz+xx, 0,
    0, 0, xx+yy );
  return zRotMat3D( zFrame3DAtt(&box->f), &i, inertia );
}

/* inertia tensor about barycenter of a 3D box. */
zMat3D *zBox3DBaryInertia(const zBox3D *box, double density, zMat3D *inertia)
{
  return zBox3DBaryInertiaMass( box, density * zBox3DVolume( box ), inertia );
}

/* get vertex of a box. */
zVec3D *zBox3DVert(const zBox3D *box, int i, zVec3D *v)
{
  _zVec3DCreate( v,
    ( (i&0x1)^(i>>1&0x1) ) ? -0.5*zBox3DDepth(box) : 0.5*zBox3DDepth(box),
    (  i&0x2             ) ? -0.5*zBox3DWidth(box) : 0.5*zBox3DWidth(box),
    (  i&0x4             ) ? -0.5*zBox3DHeight(box): 0.5*zBox3DHeight(box) );
  return zXform3DDRC( &box->f, v );
}

#define __zBox3DToPH_tri(p,i,i1,i2,i3,i4) do{\
  zTri3DCreate( zPH3DFace(p,i),   zPH3DVert(p,i1), zPH3DVert(p,i2), zPH3DVert(p,i3) );\
  zTri3DCreate( zPH3DFace(p,i+1), zPH3DVert(p,i1), zPH3DVert(p,i3), zPH3DVert(p,i4) );\
} while(0)

/* convert a 3D axis-aligned box to a general box. */
zBox3D *zAABox3DToBox3D(const zAABox3D *aab, zBox3D *box)
{
  zVec3DMid( &aab->max, &aab->min, zBox3DCenter(box) );
  zVec3DSub( &aab->max, &aab->min, &box->dia );
  zBox3DSetAxis( box, 0, ZVEC3DX );
  zBox3DSetAxis( box, 1, ZVEC3DY );
  zBox3DSetAxis( box, 2, ZVEC3DZ );
  return box;
}

/* enlarge a 3D axis-aligned box. */
static zAABox3D *_zAABox3DEnlarge(zAABox3D *aabox, zVec3D *v)
{
  if( zAABox3DXMin(aabox) > v->c.x ) zAABox3DXMin(aabox) = v->c.x;
  if( zAABox3DXMax(aabox) < v->c.x ) zAABox3DXMax(aabox) = v->c.x;
  if( zAABox3DYMin(aabox) > v->c.y ) zAABox3DYMin(aabox) = v->c.y;
  if( zAABox3DYMax(aabox) < v->c.y ) zAABox3DYMax(aabox) = v->c.y;
  if( zAABox3DZMin(aabox) > v->c.z ) zAABox3DZMin(aabox) = v->c.z;
  if( zAABox3DZMax(aabox) < v->c.z ) zAABox3DZMax(aabox) = v->c.z;
  return aabox;
}

/* compute a 3D axis-aligned bounding box of a 3D box. */
zAABox3D *zBox3DToAABox3D(const zBox3D *box, zAABox3D *aabox)
{
  zVec3D v;
  int i;

  zVec3DCreate( &aabox->min, HUGE_VAL, HUGE_VAL, HUGE_VAL );
  zVec3DCreate( &aabox->max,-HUGE_VAL,-HUGE_VAL,-HUGE_VAL );
  for( i=0; i<8; i++ ){
    zBox3DVert( box, i, &v );
    _zAABox3DEnlarge( aabox, &v );
  }
  return aabox;
}

/* convert a box to a polyhedron. */
zPH3D* zBox3DToPH(const zBox3D *box, zPH3D *ph)
{
  int i;

  if( !zPH3DAlloc( ph, 8, 12 ) ) return NULL;
  for( i=0; i<8; i++ )
    zBox3DVert( box, i, zPH3DVert(ph,i) );
  __zBox3DToPH_tri( ph, 0, 0, 1, 2, 3 );
  __zBox3DToPH_tri( ph, 2, 0, 4, 5, 1 );
  __zBox3DToPH_tri( ph, 4, 1, 5, 6, 2 );
  __zBox3DToPH_tri( ph, 6, 2, 6, 7, 3 );
  __zBox3DToPH_tri( ph, 8, 0, 3, 7, 4 );
  __zBox3DToPH_tri( ph,10, 7, 6, 5, 4 );
  return ph;
}

/* parse ZTK format */

static zVec3D *_zBox3DAxisFromZTK(zBox3D *box, int i0, int i1, int i2, ZTK *ztk)
{
  if( ZTKValCmp( ztk, "auto" ) )
    zVec3DOuterProd( zBox3DAxis(box,i1), zBox3DAxis(box,i2), zBox3DAxis(box,i0) );
  else
    zVec3DFromZTK( zBox3DAxis(box,i0), ztk );
  zVec3DNormalizeDRC( zBox3DAxis(box,i0) );
  return zBox3DAxis(box,i0);
}

static void *_zBox3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zVec3DFromZTK( zBox3DCenter((zBox3D*)obj), ztk ) ? obj : NULL; }
static void *_zBox3DAxisXFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  _zBox3DAxisFromZTK( (zBox3D*)obj, 0, 1, 2, ztk );
  return obj; }
static void *_zBox3DAxisYFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  _zBox3DAxisFromZTK( (zBox3D*)obj, 1, 2, 0, ztk );
  return obj; }
static void *_zBox3DAxisZFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  _zBox3DAxisFromZTK( (zBox3D*)obj, 2, 0, 1, ztk );
  return obj; }
static void *_zBox3DDepthFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DDepth((zBox3D*)obj) = ZTKDouble(ztk);
  return obj; }
static void *_zBox3DWidthFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DWidth((zBox3D*)obj) = ZTKDouble(ztk);
  return obj; }
static void *_zBox3DHeightFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DHeight((zBox3D*)obj) = ZTKDouble(ztk);
  return obj; }

static bool _zBox3DCenterFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zBox3DCenter((zBox3D*)obj) );
  return true; }
static bool _zBox3DAxisXFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zX) );
  return true; }
static bool _zBox3DAxisYFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zY) );
  return true; }
static bool _zBox3DAxisZFPrintZTK(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zZ) );
  return true; }
static bool _zBox3DDepthFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zBox3DDepth((zBox3D*)obj) );
  return true; }
static bool _zBox3DWidthFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zBox3DWidth((zBox3D*)obj) );
  return true; }
static bool _zBox3DHeightFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zBox3DHeight((zBox3D*)obj) );
  return true; }

static const ZTKPrp __ztk_prp_shape_box[] = {
  { ZTK_KEY_ZEO_SHAPE3D_CENTER, 1, _zBox3DCenterFromZTK, _zBox3DCenterFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_AX,     1, _zBox3DAxisXFromZTK, _zBox3DAxisXFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_AY,     1, _zBox3DAxisYFromZTK, _zBox3DAxisYFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_AZ,     1, _zBox3DAxisZFromZTK, _zBox3DAxisZFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_DEPTH,  1, _zBox3DDepthFromZTK, _zBox3DDepthFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_WIDTH,  1, _zBox3DWidthFromZTK, _zBox3DWidthFPrintZTK },
  { ZTK_KEY_ZEO_SHAPE3D_HEIGHT, 1, _zBox3DHeightFromZTK, _zBox3DHeightFPrintZTK },
};

/* print a 3D box out to a file in a ZTK format. */
void zBox3DFPrintZTK(FILE *fp, const zBox3D *box)
{
  ZTKPrpKeyFPrint( fp, (void *)box, __ztk_prp_shape_box );
}

/* print a 3D box out to a file in a plottable format. */
void zBox3DValueFPrint(FILE *fp, const zBox3D *box)
{
  zVec3D v[8];
  int i;

  for( i=0; i<8; i++ )
    zBox3DVert( box, i, &v[i] );
  zVec3DValueNLFPrint( fp, &v[0] );
  zVec3DValueNLFPrint( fp, &v[1] );
  zVec3DValueNLFPrint( fp, &v[2] );
  zVec3DValueNLFPrint( fp, &v[3] );
  zVec3DValueNLFPrint( fp, &v[0] );
  zVec3DValueNLFPrint( fp, &v[4] );
  zVec3DValueNLFPrint( fp, &v[5] );
  zVec3DValueNLFPrint( fp, &v[6] );
  zVec3DValueNLFPrint( fp, &v[7] );
  zVec3DValueNLFPrint( fp, &v[4] );
  fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[1] );
  zVec3DValueNLFPrint( fp, &v[5] );
  fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[2] );
  zVec3DValueNLFPrint( fp, &v[6] );
  fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[3] );
  zVec3DValueNLFPrint( fp, &v[7] );
  fprintf( fp, "\n\n" );
}

/* methods for abstraction */

static void *_zShape3DBoxInit(void *body){
  return zBox3DInit( (zBox3D*)body ); }
static void *_zShape3DBoxAlloc(void){
  return zBox3DAlloc(); }
static void *_zShape3DBoxClone(void *src){
  zBox3D *cln;
  return ( cln = (zBox3D*)zBox3DAlloc() ) ? zBox3DCopy( (zBox3D*)src, cln ) : NULL; }
static void *_zShape3DBoxMirror(void *src, zAxis axis){
  zBox3D *mrr;
  return ( mrr = zBox3DAlloc() ) ? zBox3DMirror( (zBox3D*)src, mrr, axis ) : NULL; }
static void _zShape3DBoxDestroy(void *body){}
static void *_zShape3DBoxXform(void *src, const zFrame3D *f, void *dest){
  return zBox3DXform( (zBox3D*)src, f, (zBox3D*)dest ); }
static void *_zShape3DBoxXformInv(void *src, const zFrame3D *f, void *dest){
  return zBox3DXformInv( (zBox3D*)src, f, (zBox3D*)dest ); }
static double _zShape3DBoxClosest(void *body, const zVec3D *p, zVec3D *cp){
  return zBox3DClosest( (zBox3D*)body, p, cp ); }
static double _zShape3DBoxDistFromPoint(void *body, const zVec3D *p){
  return zBox3DDistFromPoint( (zBox3D*)body, p ); }
static bool _zShape3DBoxPointIsInside(void *body, const zVec3D *p, double margin){
  return zBox3DPointIsInside( (zBox3D*)body, p, margin ); }
static double _zShape3DBoxVolume(void *body){
  return zBox3DVolume( (zBox3D*)body ); }
static zVec3D *_zShape3DBoxBarycenter(void *body, zVec3D *c){
  zVec3DCopy( zBox3DCenter((zBox3D*)body), c ); return c; }
static zMat3D *_zShape3DBoxBaryInertiaMass(void *body, double mass, zMat3D *i){
  return zBox3DBaryInertiaMass( (zBox3D*)body, mass, i ); }
static zMat3D *_zShape3DBoxBaryInertia(void *body, double density, zMat3D *i){
  return zBox3DBaryInertia( (zBox3D*)body, density, i ); }
static zPH3D *_zShape3DBoxToPH(void *body, zPH3D *ph){
  return zBox3DToPH( (zBox3D*)body, ph ); }
static void *_zShape3DBoxParseZTK(void *body, ZTK *ztk){
  zBox3DInit( (zBox3D*)body );
  return ZTKEvalKey( body, NULL, ztk, __ztk_prp_shape_box ); }
static void _zShape3DBoxFPrintZTK(FILE *fp, void *body){
  zBox3DFPrintZTK( fp, (zBox3D*)body ); }

zShape3DCom zeo_shape3d_box_com = {
  "box",
  _zShape3DBoxInit,
  _zShape3DBoxAlloc,
  _zShape3DBoxClone,
  _zShape3DBoxMirror,
  _zShape3DBoxDestroy,
  _zShape3DBoxXform,
  _zShape3DBoxXformInv,
  _zShape3DBoxClosest,
  _zShape3DBoxDistFromPoint,
  _zShape3DBoxPointIsInside,
  _zShape3DBoxVolume,
  _zShape3DBoxBarycenter,
  _zShape3DBoxBaryInertiaMass,
  _zShape3DBoxBaryInertia,
  _zShape3DBoxToPH,
  _zShape3DBoxParseZTK,
  _zShape3DBoxFPrintZTK,
};

/* create a 3D shape as a box. */
zShape3D *zShape3DBoxCreate(zShape3D *shape, const zVec3D *c, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double d, double w, double h)
{
  zShape3DInit( shape );
  if( !( shape->body = zBox3DAlloc() ) ) return NULL;
  zBox3DCreate( zShape3DBox(shape), c, ax, ay, az, d, w, h );
  shape->com = &zeo_shape3d_box_com;
  return shape;
}

/* create a 3D shape as an axis-aligned box. */
zShape3D *zShape3DBoxCreateAlign(zShape3D *shape, const zVec3D *c, double d, double w, double h)
{
  zShape3DInit( shape );
  if( !( shape->body = zBox3DAlloc() ) ) return NULL;
  zBox3DCreateAlign( zShape3DBox(shape), c, d, w, h );
  shape->com = &zeo_shape3d_box_com;
  return shape;
}
