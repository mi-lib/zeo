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
static void *_zShape3DPHClone(const void *src){
  zPH3D *cln;
  return ( cln = (zPH3D*)_zShape3DPHAlloc() ) ? zPH3DClone( (const zPH3D*)src, cln ) : NULL; }
static void *_zShape3DPHMirror(const void *src, zAxis axis){
  zPH3D *mrr;
  return ( mrr = (zPH3D*)_zShape3DPHAlloc() ) ? zPH3DMirror( (const zPH3D*)src, mrr, axis ) : NULL; }
static void _zShape3DPHDestroy(void *body){
  zPH3DDestroy( (zPH3D*)body ); }
static void *_zShape3DPHXform(const void *src, const zFrame3D *f, void *dest){
  return zPH3DXform( (const zPH3D*)src, f, (zPH3D*)dest ); }
static void *_zShape3DPHXformInv(const void *src, const zFrame3D *f, void *dest){
  return zPH3DXformInv( (const zPH3D*)src, f, (zPH3D*)dest ); }
static double _zShape3DPHClosest(const void *body, const zVec3D *p, zVec3D *cp){
  return zPH3DClosest( (const zPH3D*)body, p, cp ); }
static double _zShape3DPHDistFromPoint(const void *body, const zVec3D *p){
  return zPH3DDistFromPoint( (const zPH3D*)body, p ); }
static bool _zShape3DPHPointIsInside(const void *body, const zVec3D *p, double margin){
  return zPH3DPointIsInside( (const zPH3D*)body, p, margin ); }
static double _zShape3DPHVolume(const void *body){
  return zPH3DVolume( (const zPH3D*)body ); }
static zVec3D *_zShape3DPHBarycenter(const void *body, zVec3D *c){
  return zPH3DBarycenter( (const zPH3D*)body, c ); }
static zMat3D *_zShape3DPHBaryInertiaMass(const void *body, double mass, zMat3D *i){
  return zPH3DBaryInertiaMass( (const zPH3D*)body, mass, i ); }
static zMat3D *_zShape3DPHBaryInertia(const void *body, double density, zMat3D *i){
  return zPH3DBaryInertia( (const zPH3D*)body, density, i ); }
static zPH3D *_zShape3DPHToPH(const void *body, zPH3D *ph){
  return zPH3DClone( (zPH3D*)body, ph ); }
static void *_zShape3DPHParseZTK(void *body, ZTK *ztk){
  return zPH3DFromZTK( (zPH3D*)body, ztk ); }
static void _zShape3DPHFPrintZTK(FILE *fp, const void *body){
  return zPH3DFPrintZTK( fp, (const zPH3D*)body ); }

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
  _zShape3DPHDistFromPoint,
  _zShape3DPHPointIsInside,
  _zShape3DPHVolume,
  _zShape3DPHBarycenter,
  _zShape3DPHBaryInertiaMass,
  _zShape3DPHBaryInertia,
  _zShape3DPHToPH,
  _zShape3DPHParseZTK,
  _zShape3DPHFPrintZTK,
};
