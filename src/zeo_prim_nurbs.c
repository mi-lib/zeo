/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_nurbs - primitive 3D shapes: NURBS (for class abstraction).
 */

#include <zeo/zeo_prim.h>

/* methods for abstraction of zNURBS3D */

static void *_zPrim3DInitNURBS(void* prim){
  return zNURBS3DInit( prim ); }
static void *_zPrim3DCloneNURBS(void *src, void *dest){
  return zNURBS3DClone( src, dest ); }
static void *_zPrim3DMirrorNURBS(void *src, void *dest, zAxis axis){
  return zNURBS3DMirror( src, dest, axis ); }
static void _zPrim3DDestroyNURBS(void *prim){
  zNURBS3DDestroy( prim ); }
static void *_zPrim3DXferNURBS(void *src, zFrame3D *f, void *dest){
  return zNURBS3DXfer( src, f, dest ); }
static void *_zPrim3DXferInvNURBS(void *src, zFrame3D *f, void *dest){
  return zNURBS3DXferInv( src, f, dest ); }
static double _zPrim3DClosestNURBS(void *prim, zVec3D *p, zVec3D *cp){
  return zNURBS3DClosest( prim, p, cp, NULL, NULL ); }
static double _zPrim3DPointDistNURBS(void *prim, zVec3D *p){
  zVec3D nn;
  return zNURBS3DClosest( prim, p, &nn, NULL, NULL ); }

static bool _zPrim3DPointIsInsideNURBS(void *prim, zVec3D *p, bool rim){
  return false; }
static double _zPrim3DVolumeNURBS(void *prim){
  return 0; }
static zVec3D *_zPrim3DBarycenterNURBS(void *prim, zVec3D *c){
  return NULL; }
static zMat3D *_zPrim3DInertiaNURBS(void *prim, zMat3D *i){
  return NULL; }
static void _zPrim3DBaryInertiaNURBS(void *prim, zVec3D *c, zMat3D *i){}

static zPH3D *_zPrim3DToPHNURBS(void *prim, zPH3D *ph){
  return zNURBS3DToPH( prim, ph ); }
static void *_zPrim3DFReadNURBS(FILE *fp, void *prim){
  return zNURBS3DFRead( fp, prim ); }
static void _zPrim3DFWriteNURBS(FILE *fp, void *prim){
  return zNURBS3DFWrite( fp, prim ); }

zPrimCom zprim_nurbs_com = {
  _zPrim3DInitNURBS,
  _zPrim3DCloneNURBS,
  _zPrim3DMirrorNURBS,
  _zPrim3DDestroyNURBS,
  _zPrim3DXferNURBS,
  _zPrim3DXferInvNURBS,
  _zPrim3DClosestNURBS,
  _zPrim3DPointDistNURBS,
  _zPrim3DPointIsInsideNURBS,
  _zPrim3DVolumeNURBS,
  _zPrim3DBarycenterNURBS,
  _zPrim3DInertiaNURBS,
  _zPrim3DBaryInertiaNURBS,
  _zPrim3DToPHNURBS,
  _zPrim3DFReadNURBS,
  _zPrim3DFWriteNURBS,
};
