/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_ph - 3D shapes: polyhedron (for class abstraction).
 */

#include <zeo/zeo_shape3d.h>

/* methods for abstraction of zPH3D */

static void *_zShape3DPHInit(void* shape){
  return zPH3DInit( shape ); }
static void *_zShape3DPHAlloc(void){
  zPH3D *ph;
  if( !( ph = zAlloc( zPH3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return ph;
}
static void *_zShape3DPHClone(void *src){
  zPH3D *cln;
  return ( cln = _zShape3DPHAlloc() ) ? zPH3DClone( src, cln ) : NULL; }
static void *_zShape3DPHMirror(void *src, zAxis axis){
  zPH3D *mrr;
  return ( mrr = _zShape3DPHAlloc() ) ? zPH3DMirror( src, mrr, axis ) : NULL; }
static void _zShape3DPHDestroy(void *shape){
  zPH3DDestroy( shape ); }
static void *_zShape3DPHXform(void *src, zFrame3D *f, void *dest){
  return zPH3DXform( src, f, dest ); }
static void *_zShape3DPHXformInv(void *src, zFrame3D *f, void *dest){
  return zPH3DXformInv( src, f, dest ); }
static double _zShape3DPHClosest(void *shape, zVec3D *p, zVec3D *cp){
  return zPH3DClosest( shape, p, cp ); }
static double _zShape3DPHPointDist(void *shape, zVec3D *p){
  return zPH3DPointDist( shape, p ); }
static bool _zShape3DPHPointIsInside(void *shape, zVec3D *p, bool rim){
  return zPH3DPointIsInside( shape, p, rim ); }
static double _zShape3DPHVolume(void *shape){
  return zPH3DVolume( shape ); }
static zVec3D *_zShape3DPHBarycenter(void *shape, zVec3D *c){
  return zPH3DBarycenter( shape, c ); }
static zMat3D *_zShape3DPHInertia(void *shape, zMat3D *i){
  return zPH3DInertia( shape, i ); }
static void _zShape3DPHBaryInertia(void *shape, zVec3D *c, zMat3D *i){
  zPH3DBaryInertia( shape, c, i ); }
static zPH3D *_zShape3DPHToPH(void *shape, zPH3D *ph){
  return zPH3DClone( shape, ph ); }
static void *_zShape3DPHParseZTK(void *shape, ZTK *ztk){
  return zPH3DFromZTK( shape, ztk ); }
static void _zShape3DPHFPrintZTK(FILE *fp, void *shape){
  return zPH3DFPrintZTK( fp, shape ); }

zShape3DCom zeo_shape3d_ph_com = {
  "polyhedron",
  _zShape3DPHInit,
  _zShape3DPHAlloc,
  _zShape3DPHClone,
  _zShape3DPHMirror,
  _zShape3DPHDestroy,
  _zShape3DPHXform,
  _zShape3DPHXformInv,
  _zShape3DPHClosest,
  _zShape3DPHPointDist,
  _zShape3DPHPointIsInside,
  _zShape3DPHVolume,
  _zShape3DPHBarycenter,
  _zShape3DPHInertia,
  _zShape3DPHBaryInertia,
  _zShape3DPHToPH,
  _zShape3DPHParseZTK,
  _zShape3DPHFPrintZTK,
};
