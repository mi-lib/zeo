/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_ph - 3D shapes: polyhedron (for class abstraction).
 */

#include <zeo/zeo_shape3d.h>

/* methods for abstraction of zPH3D */

static void *_zShape3DPHInit(void *body){
  return zPH3DInit( body ); }
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
static void _zShape3DPHDestroy(void *body){
  zPH3DDestroy( body ); }
static void *_zShape3DPHXform(void *src, zFrame3D *f, void *dest){
  return zPH3DXform( src, f, dest ); }
static void *_zShape3DPHXformInv(void *src, zFrame3D *f, void *dest){
  return zPH3DXformInv( src, f, dest ); }
static double _zShape3DPHClosest(void *body, zVec3D *p, zVec3D *cp){
  return zPH3DClosest( body, p, cp ); }
static double _zShape3DPHPointDist(void *body, zVec3D *p){
  return zPH3DPointDist( body, p ); }
static bool _zShape3DPHPointIsInside(void *body, zVec3D *p, bool rim){
  return zPH3DPointIsInside( body, p, rim ); }
static double _zShape3DPHVolume(void *body){
  return zPH3DVolume( body ); }
static zVec3D *_zShape3DPHBarycenter(void *body, zVec3D *c){
  return zPH3DBarycenter( body, c ); }
static zMat3D *_zShape3DPHInertia(void *body, double density, zMat3D *i){
  zVec3D c;
  zPH3DBaryInertia( body, density, &c, i );
  return i; }
static zMat3D *_zShape3DPHInertiaMass(void *body, double mass, zMat3D *i){
  return _zShape3DPHInertia( body, mass / _zShape3DPHVolume(body), i ); }
static void _zShape3DPHBaryInertia(void *body, double density, zVec3D *c, zMat3D *i){
  zPH3DBaryInertia( body, density, c, i ); }
static zPH3D *_zShape3DPHToPH(void *body, zPH3D *ph){
  return zPH3DClone( body, ph ); }
static void *_zShape3DPHParseZTK(void *body, ZTK *ztk){
  return zPH3DFromZTK( body, ztk ); }
static void _zShape3DPHFPrintZTK(FILE *fp, void *body){
  return zPH3DFPrintZTK( fp, body ); }

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
  _zShape3DPHInertiaMass,
  _zShape3DPHInertia,
  _zShape3DPHBaryInertia,
  _zShape3DPHToPH,
  _zShape3DPHParseZTK,
  _zShape3DPHFPrintZTK,
};
