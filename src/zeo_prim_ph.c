/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_ph - primitive 3D shapes: polyhedron (for class abstraction).
 */

#include <zeo/zeo_prim.h>

/* methods for abstraction of zPH3D */

static void *_zPrim3DInitPH(void* prim){
  return zPH3DInit( prim ); }
static void *_zPrim3DClonePH(void *src, void *dest){
  return zPH3DClone( src, dest ); }
static void *_zPrim3DMirrorPH(void *src, void *dest, zAxis axis){
  return zPH3DMirror( src, dest, axis ); }
static void _zPrim3DDestroyPH(void *prim){
  zPH3DDestroy( prim ); }
static void *_zPrim3DXformPH(void *src, zFrame3D *f, void *dest){
  return zPH3DXform( src, f, dest ); }
static void *_zPrim3DXformInvPH(void *src, zFrame3D *f, void *dest){
  return zPH3DXformInv( src, f, dest ); }
static double _zPrim3DClosestPH(void *prim, zVec3D *p, zVec3D *cp){
  return zPH3DClosest( prim, p, cp ); }
static double _zPrim3DPointDistPH(void *prim, zVec3D *p){
  return zPH3DPointDist( prim, p ); }
static bool _zPrim3DPointIsInsidePH(void *prim, zVec3D *p, bool rim){
  return zPH3DPointIsInside( prim, p, rim ); }
static double _zPrim3DVolumePH(void *prim){
  return zPH3DVolume( prim ); }
static zVec3D *_zPrim3DBarycenterPH(void *prim, zVec3D *c){
  return zPH3DBarycenter( prim, c ); }
static zMat3D *_zPrim3DInertiaPH(void *prim, zMat3D *i){
  return zPH3DInertia( prim, i ); }
static void _zPrim3DBaryInertiaPH(void *prim, zVec3D *c, zMat3D *i){
  zPH3DBaryInertia( prim, c, i ); }
static zPH3D *_zPrim3DToPHPH(void *prim, zPH3D *ph){
  return zPH3DClone( prim, ph ); }
static void *_zPrim3DParseZTKPH(void *prim, ZTK *ztk){
  return zPH3DFromZTK( prim, ztk ); }
static void *_zPrim3DFScanPH(FILE *fp, void *prim){
  return zPH3DFScan( fp, prim ); }
static void _zPrim3DFPrintPH(FILE *fp, void *prim){
  return zPH3DFPrint( fp, prim ); }

zPrimCom zprim_ph3d_com = {
  _zPrim3DInitPH,
  _zPrim3DClonePH,
  _zPrim3DMirrorPH,
  _zPrim3DDestroyPH,
  _zPrim3DXformPH,
  _zPrim3DXformInvPH,
  _zPrim3DClosestPH,
  _zPrim3DPointDistPH,
  _zPrim3DPointIsInsidePH,
  _zPrim3DVolumePH,
  _zPrim3DBarycenterPH,
  _zPrim3DInertiaPH,
  _zPrim3DBaryInertiaPH,
  _zPrim3DToPHPH,
  _zPrim3DParseZTKPH,
  _zPrim3DFScanPH,
  _zPrim3DFPrintPH,
};
