/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_ph - 3D shapes: polyhedron (for class abstraction).
 */

#include <zeo/zeo_shape3d.h>

/* methods for abstraction of zPH3D */

static void *_zShape3DPHInit(void *body){
  return zPH3DInit( (zPH3D*)body ); }
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
  return ( cln = (zPH3D*)_zShape3DPHAlloc() ) ? zPH3DClone( (zPH3D*)src, cln ) : NULL; }
static void *_zShape3DPHMirror(void *src, zAxis axis){
  zPH3D *mrr;
  return ( mrr = (zPH3D*)_zShape3DPHAlloc() ) ? zPH3DMirror( (zPH3D*)src, mrr, axis ) : NULL; }
static void _zShape3DPHDestroy(void *body){
  zPH3DDestroy( (zPH3D*)body ); }
static void *_zShape3DPHXform(void *src, zFrame3D *f, void *dest){
  return zPH3DXform( (zPH3D*)src, f, (zPH3D*)dest ); }
static void *_zShape3DPHXformInv(void *src, zFrame3D *f, void *dest){
  return zPH3DXformInv( (zPH3D*)src, f, (zPH3D*)dest ); }
static double _zShape3DPHClosest(void *body, zVec3D *p, zVec3D *cp){
  return zPH3DClosest( (zPH3D*)body, p, cp ); }
static double _zShape3DPHPointDist(void *body, zVec3D *p){
  return zPH3DPointDist( (zPH3D*)body, p ); }
static bool _zShape3DPHPointIsInside(void *body, zVec3D *p, bool rim){
  return zPH3DPointIsInside( (zPH3D*)body, p, rim ); }
static double _zShape3DPHVolume(void *body){
  return zPH3DVolume( (zPH3D*)body ); }
static zVec3D *_zShape3DPHBarycenter(void *body, zVec3D *c){
  return zPH3DBarycenter( (zPH3D*)body, c ); }
static zMat3D *_zShape3DPHBaryInertiaMass(void *body, double mass, zMat3D *i){
  return zPH3DBaryInertiaMass( (zPH3D*)body, mass, i ); }
static zMat3D *_zShape3DPHBaryInertia(void *body, double density, zMat3D *i){
  return zPH3DBaryInertia( (zPH3D*)body, density, i ); }
static zPH3D *_zShape3DPHToPH(void *body, zPH3D *ph){
  return zPH3DClone( (zPH3D*)body, ph ); }
static void *_zShape3DPHParseZTK(void *body, ZTK *ztk){
  return zPH3DFromZTK( (zPH3D*)body, ztk ); }
static void _zShape3DPHFPrintZTK(FILE *fp, void *body){
  return zPH3DFPrintZTK( fp, (zPH3D*)body ); }

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
  _zShape3DPHBaryInertiaMass,
  _zShape3DPHBaryInertia,
  _zShape3DPHToPH,
  _zShape3DPHParseZTK,
  _zShape3DPHFPrintZTK,
};
