/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_nurbs - 3D shapes: NURBS (for class abstraction).
 */

#include <zeo/zeo_shape.h>

/* methods for abstraction of zNURBS3D */

static void *_zShape3DNURBSInit(void* shape){
  return zNURBS3DInit( shape ); }
static void *_zShape3DNURBSAlloc(void){
  zNURBS3D *nurbs;
  if( !( nurbs = zAlloc( zNURBS3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return nurbs;
}
static void *_zShape3DNURBSClone(void *src){
  zNURBS3D *cln;
  return ( cln = _zShape3DNURBSAlloc() ) ?  zNURBS3DClone( src, cln ) : NULL; }
static void *_zShape3DNURBSMirror(void *src, zAxis axis){
  zNURBS3D *mrr;
  return ( mrr = _zShape3DNURBSAlloc() ) ? zNURBS3DMirror( src, mrr, axis ) : NULL; }
static void _zShape3DNURBSDestroy(void *shape){
  zNURBS3DDestroy( shape ); }
static void *_zShape3DNURBSXform(void *src, zFrame3D *f, void *dest){
  return zNURBS3DXform( src, f, dest ); }
static void *_zShape3DNURBSXformInv(void *src, zFrame3D *f, void *dest){
  return zNURBS3DXformInv( src, f, dest ); }
static double _zShape3DNURBSClosest(void *shape, zVec3D *p, zVec3D *cp){
  return zNURBS3DClosest( shape, p, cp, NULL, NULL ); }
static double _zShape3DNURBSPointDist(void *shape, zVec3D *p){
  zVec3D nn;
  return zNURBS3DClosest( shape, p, &nn, NULL, NULL ); }

static bool _zShape3DNURBSPointIsInside(void *shape, zVec3D *p, bool rim){
  return false; }
static double _zShape3DNURBSVolume(void *shape){
  return 0; }
static zVec3D *_zShape3DNURBSBarycenter(void *shape, zVec3D *c){
  return NULL; }
static zMat3D *_zShape3DNURBSInertia(void *shape, zMat3D *i){
  return NULL; }
static void _zShape3DNURBSBaryInertia(void *shape, zVec3D *c, zMat3D *i){}

static zPH3D *_zShape3DNURBSToPH(void *shape, zPH3D *ph){
  return zNURBS3DToPH( shape, ph ); }
static void *_zShape3DNURBSParseZTK(void *shape, ZTK *ztk){
  return zNURBS3DFromZTK( shape, ztk ); }
static void _zShape3DNURBSFPrintZTK(FILE *fp, void *shape){
  return zNURBS3DFPrintZTK( fp, shape ); }

zShape3DCom zeo_shape3d_nurbs_com = {
  "nurbs",
  _zShape3DNURBSInit,
  _zShape3DNURBSAlloc,
  _zShape3DNURBSClone,
  _zShape3DNURBSMirror,
  _zShape3DNURBSDestroy,
  _zShape3DNURBSXform,
  _zShape3DNURBSXformInv,
  _zShape3DNURBSClosest,
  _zShape3DNURBSPointDist,
  _zShape3DNURBSPointIsInside,
  _zShape3DNURBSVolume,
  _zShape3DNURBSBarycenter,
  _zShape3DNURBSInertia,
  _zShape3DNURBSBaryInertia,
  _zShape3DNURBSToPH,
  _zShape3DNURBSParseZTK,
  _zShape3DNURBSFPrintZTK,
};

/* allocate memory for NURBS stored in a 3D shape. */
zShape3D *zShape3DNURBSAlloc(zShape3D *shape, int size1, int size2, int dim1, int dim2)
{
  zShape3DInit( shape );
  if( !( shape->body = _zShape3DNURBSAlloc() ) ) return NULL;
  zNURBS3DAlloc( zShape3DNURBS(shape), size1, size2, dim1, dim2 );
  shape->com = &zeo_shape3d_nurbs_com;
  return shape;
}
