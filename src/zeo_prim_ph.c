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
static void *_zPrim3DXferPH(void *src, zFrame3D *f, void *dest){
  return zPH3DXfer( src, f, dest ); }
static void *_zPrim3DXferInvPH(void *src, zFrame3D *f, void *dest){
  return zPH3DXferInv( src, f, dest ); }
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
static void *_zPrim3DFReadPH(FILE *fp, void *prim){
  return zPH3DFRead( fp, prim ); }
static void _zPrim3DFWritePH(FILE *fp, void *prim){
  return zPH3DFWrite( fp, prim ); }

zPrimCom zprim_ph3d_com = {
  _zPrim3DInitPH,
  _zPrim3DClonePH,
  _zPrim3DMirrorPH,
  _zPrim3DDestroyPH,
  _zPrim3DXferPH,
  _zPrim3DXferInvPH,
  _zPrim3DClosestPH,
  _zPrim3DPointDistPH,
  _zPrim3DPointIsInsidePH,
  _zPrim3DVolumePH,
  _zPrim3DBarycenterPH,
  _zPrim3DInertiaPH,
  _zPrim3DBaryInertiaPH,
  _zPrim3DToPHPH,
  _zPrim3DFReadPH,
  _zPrim3DFWritePH,
};

/* dummy methods for abstraction */

static void *_zPrim3DInitNone(void* prim){ return NULL; }
static void *_zPrim3DCloneNone(void *src, void *dest){ return NULL; }
static void *_zPrim3DMirrorNone(void *src, void *dest, zAxis axis){ return NULL; }
static void _zPrim3DDestroyNone(void *prim){}
static void *_zPrim3DXferNone(void *src, zFrame3D *f, void *dest){ return NULL; }
static void *_zPrim3DXferInvNone(void *src, zFrame3D *f, void *dest){ return NULL; }
static double _zPrim3DClosestNone(void *prim, zVec3D *p, zVec3D *cp){ return HUGE_VAL; }
static double _zPrim3DPointDistNone(void *prim, zVec3D *p){ return HUGE_VAL; }
static bool _zPrim3DPointIsInsideNone(void *prim, zVec3D *p, bool rim){ return false; }
static double _zPrim3DVolumeNone(void *prim){ return 0; }
static zVec3D *_zPrim3DBarycenterNone(void *prim, zVec3D *c){ return NULL; }
static zMat3D *_zPrim3DInertiaNone(void *prim, zMat3D *i){ return NULL; }
static void _zPrim3DBaryInertiaNone(void *prim, zVec3D *c, zMat3D *i){}
static zPH3D *_zPrim3DToPHNone(void *prim, zPH3D *ph){ return NULL; }
static void *_zPrim3DFReadNone(FILE *fp, void *prim){ return NULL; }
static void _zPrim3DFWriteNone(FILE *fp, void *prim){}

zPrimCom zprim_none_com = {
  _zPrim3DInitNone,
  _zPrim3DCloneNone,
  _zPrim3DMirrorNone,
  _zPrim3DDestroyNone,
  _zPrim3DXferNone,
  _zPrim3DXferInvNone,
  _zPrim3DClosestNone,
  _zPrim3DPointDistNone,
  _zPrim3DPointIsInsideNone,
  _zPrim3DVolumeNone,
  _zPrim3DBarycenterNone,
  _zPrim3DInertiaNone,
  _zPrim3DBaryInertiaNone,
  _zPrim3DToPHNone,
  _zPrim3DFReadNone,
  _zPrim3DFWriteNone,
};
