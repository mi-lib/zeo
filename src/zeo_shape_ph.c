/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_ph - 3D shapes: polyhedron (for class abstraction).
 */

#include <zeo/zeo_shape.h>

/* methods for abstraction of zPH3D */

static void *_zShape3DInitPH(void* shape){
  return zPH3DInit( shape ); }
static void *_zShape3DAllocPH(void){
  zPH3D *ph;
  if( !( ph = zAlloc( zPH3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  return ph;
}
static void *_zShape3DClonePH(void *src){
  zPH3D *cln;
  return ( cln = _zShape3DAllocPH() ) ? zPH3DClone( src, cln ) : NULL; }
static void *_zShape3DMirrorPH(void *src, zAxis axis){
  zPH3D *mrr;
  return ( mrr = _zShape3DAllocPH() ) ? zPH3DMirror( src, mrr, axis ) : NULL; }
static void _zShape3DDestroyPH(void *shape){
  zPH3DDestroy( shape ); }
static void *_zShape3DXformPH(void *src, zFrame3D *f, void *dest){
  return zPH3DXform( src, f, dest ); }
static void *_zShape3DXformInvPH(void *src, zFrame3D *f, void *dest){
  return zPH3DXformInv( src, f, dest ); }
static double _zShape3DClosestPH(void *shape, zVec3D *p, zVec3D *cp){
  return zPH3DClosest( shape, p, cp ); }
static double _zShape3DPointDistPH(void *shape, zVec3D *p){
  return zPH3DPointDist( shape, p ); }
static bool _zShape3DPointIsInsidePH(void *shape, zVec3D *p, bool rim){
  return zPH3DPointIsInside( shape, p, rim ); }
static double _zShape3DVolumePH(void *shape){
  return zPH3DVolume( shape ); }
static zVec3D *_zShape3DBarycenterPH(void *shape, zVec3D *c){
  return zPH3DBarycenter( shape, c ); }
static zMat3D *_zShape3DInertiaPH(void *shape, zMat3D *i){
  return zPH3DInertia( shape, i ); }
static void _zShape3DBaryInertiaPH(void *shape, zVec3D *c, zMat3D *i){
  zPH3DBaryInertia( shape, c, i ); }
static zPH3D *_zShape3DToPHPH(void *shape, zPH3D *ph){
  return zPH3DClone( shape, ph ); }
static void *_zShape3DParseZTKPH(void *shape, ZTK *ztk){
  return zPH3DFromZTK( shape, ztk ); }
static void *_zShape3DFScanPH(FILE *fp, void *shape){
  return zPH3DFScan( fp, shape ); }
static void _zShape3DFPrintPH(FILE *fp, void *shape){
  return zPH3DFPrint( fp, shape ); }

zShape3DCom zeo_shape_ph3d_com = {
  "polyhedron",
  _zShape3DInitPH,
  _zShape3DAllocPH,
  _zShape3DClonePH,
  _zShape3DMirrorPH,
  _zShape3DDestroyPH,
  _zShape3DXformPH,
  _zShape3DXformInvPH,
  _zShape3DClosestPH,
  _zShape3DPointDistPH,
  _zShape3DPointIsInsidePH,
  _zShape3DVolumePH,
  _zShape3DBarycenterPH,
  _zShape3DInertiaPH,
  _zShape3DBaryInertiaPH,
  _zShape3DToPHPH,
  _zShape3DParseZTKPH,
  _zShape3DFScanPH,
  _zShape3DFPrintPH,
};
