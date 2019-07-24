/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_nurbs - 3D shapes: NURBS (for class abstraction).
 */

#include <zeo/zeo_shape.h>

/* methods for abstraction of zNURBS3D */

static void *_zShape3DInitNURBS(void* shape){
  return zNURBS3DInit( shape ); }
static void *_zShape3DAllocNURBS(void){
  zNURBS3D *nurbs;
  if( !( nurbs = zAlloc( zNURBS3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return nurbs;
}
static void *_zShape3DCloneNURBS(void *src){
  zNURBS3D *cln;
  return ( cln = _zShape3DAllocNURBS() ) ?  zNURBS3DClone( src, cln ) : NULL; }
static void *_zShape3DMirrorNURBS(void *src, zAxis axis){
  zNURBS3D *mrr;
  return ( mrr = _zShape3DAllocNURBS() ) ? zNURBS3DMirror( src, mrr, axis ) : NULL; }
static void _zShape3DDestroyNURBS(void *shape){
  zNURBS3DDestroy( shape ); }
static void *_zShape3DXformNURBS(void *src, zFrame3D *f, void *dest){
  return zNURBS3DXform( src, f, dest ); }
static void *_zShape3DXformInvNURBS(void *src, zFrame3D *f, void *dest){
  return zNURBS3DXformInv( src, f, dest ); }
static double _zShape3DClosestNURBS(void *shape, zVec3D *p, zVec3D *cp){
  return zNURBS3DClosest( shape, p, cp, NULL, NULL ); }
static double _zShape3DPointDistNURBS(void *shape, zVec3D *p){
  zVec3D nn;
  return zNURBS3DClosest( shape, p, &nn, NULL, NULL ); }

static bool _zShape3DPointIsInsideNURBS(void *shape, zVec3D *p, bool rim){
  return false; }
static double _zShape3DVolumeNURBS(void *shape){
  return 0; }
static zVec3D *_zShape3DBarycenterNURBS(void *shape, zVec3D *c){
  return NULL; }
static zMat3D *_zShape3DInertiaNURBS(void *shape, zMat3D *i){
  return NULL; }
static void _zShape3DBaryInertiaNURBS(void *shape, zVec3D *c, zMat3D *i){}

static zPH3D *_zShape3DToPHNURBS(void *shape, zPH3D *ph){
  return zNURBS3DToPH( shape, ph ); }
static void *_zShape3DParseZTKNURBS(void *shape, ZTK *ztk){
  return zNURBS3DFromZTK( shape, ztk ); }
static void *_zShape3DFScanNURBS(FILE *fp, void *shape){
  return zNURBS3DFScan( fp, shape ); }
static void _zShape3DFPrintNURBS(FILE *fp, void *shape){
  return zNURBS3DFPrint( fp, shape ); }

zShape3DCom zeo_shape_nurbs_com = {
  "nurbs",
  _zShape3DInitNURBS,
  _zShape3DAllocNURBS,
  _zShape3DCloneNURBS,
  _zShape3DMirrorNURBS,
  _zShape3DDestroyNURBS,
  _zShape3DXformNURBS,
  _zShape3DXformInvNURBS,
  _zShape3DClosestNURBS,
  _zShape3DPointDistNURBS,
  _zShape3DPointIsInsideNURBS,
  _zShape3DVolumeNURBS,
  _zShape3DBarycenterNURBS,
  _zShape3DInertiaNURBS,
  _zShape3DBaryInertiaNURBS,
  _zShape3DToPHNURBS,
  _zShape3DParseZTKNURBS,
  _zShape3DFScanNURBS,
  _zShape3DFPrintNURBS,
};

/* allocate memory for NURBS stored in a 3D shape. */
zShape3D *zShape3DAllocNURBS(zShape3D *shape, int size1, int size2, int dim1, int dim2)
{
  zShape3DInit( shape );
  if( !( shape->body = _zShape3DAllocNURBS() ) ) return NULL;
  zNURBS3DAlloc( zShape3DNURBS(shape), size1, size2, dim1, dim2 );
  shape->com = &zeo_shape_nurbs_com;
  return shape;
}
