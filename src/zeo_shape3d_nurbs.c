/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_nurbs - 3D shapes: NURBS (for class abstraction).
 */

#include <zeo/zeo_shape3d.h>

/* methods for abstraction of zNURBS3D */

static void *_zShape3DNURBSInit(void *body){
  return zNURBS3DInit( (zNURBS3D*)body ); }
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
  return ( cln = (zNURBS3D*)_zShape3DNURBSAlloc() ) ?  zNURBS3DClone( (zNURBS3D*)src, cln ) : NULL; }
static void *_zShape3DNURBSMirror(void *src, zAxis axis){
  zNURBS3D *mrr;
  return ( mrr = (zNURBS3D*)_zShape3DNURBSAlloc() ) ? zNURBS3DMirror( (zNURBS3D*)src, mrr, axis ) : NULL; }
static void _zShape3DNURBSDestroy(void *body){
  zNURBS3DDestroy( (zNURBS3D*)body ); }
static void *_zShape3DNURBSXform(void *src, zFrame3D *f, void *dest){
  return zNURBS3DXform( (zNURBS3D*)src, f, (zNURBS3D*)dest ); }
static void *_zShape3DNURBSXformInv(void *src, zFrame3D *f, void *dest){
  return zNURBS3DXformInv( (zNURBS3D*)src, f, (zNURBS3D*)dest ); }
static double _zShape3DNURBSClosest(void *body, zVec3D *p, zVec3D *cp){
  return zNURBS3DClosest( (zNURBS3D*)body, p, cp, NULL, NULL ); }
static double _zShape3DNURBSDistFromPoint(void *body, zVec3D *p){
  zVec3D nn;
  return zNURBS3DClosest( (zNURBS3D*)body, p, &nn, NULL, NULL ); }

/* dummy functions */
static bool _zShape3DNURBSPointIsInside(void *body, zVec3D *p, double margin){
  return false; }
static double _zShape3DNURBSVolume(void *body){
  return 0; }
static zVec3D *_zShape3DNURBSBarycenter(void *body, zVec3D *c){
  return NULL; }
static zMat3D *_zShape3DNURBSBaryInertiaMass(void *body, double mass, zMat3D *i){
  return NULL; }
static zMat3D *_zShape3DNURBSBaryInertia(void *body, double density, zMat3D *i){
  return NULL; }

static zPH3D *_zShape3DNURBSToPH(void *body, zPH3D *ph){
  return zNURBS3DToPH( (zNURBS3D*)body, ph ); }
static void *_zShape3DNURBSParseZTK(void *body, ZTK *ztk){
  return zNURBS3DFromZTK( (zNURBS3D*)body, ztk ); }
static void _zShape3DNURBSFPrintZTK(FILE *fp, void *body){
  return zNURBS3DFPrintZTK( fp, (zNURBS3D*)body ); }

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
  _zShape3DNURBSDistFromPoint,
  _zShape3DNURBSPointIsInside,
  _zShape3DNURBSVolume,
  _zShape3DNURBSBarycenter,
  _zShape3DNURBSBaryInertiaMass,
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
