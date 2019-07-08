/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim - primitive 3D shapes(box, sphere, ellipsoid, cylinder, cone)
 */

#include <zeo/zeo_prim.h>

/* read the number of division for smooth primitives from a ZTK format processor. */
int zPrim3DDivFromZTK(ZTK *ztk)
{
  int val;
  return ( val = ZTKInt(ztk) ) > 0 ? val : ZEO_PRIM_DEFAULT_DIV;
}

/* dummy methods for abstraction */

static void *_zPrim3DInitNone(void* prim){ return NULL; }
static void *_zPrim3DCloneNone(void *src, void *dest){ return NULL; }
static void *_zPrim3DMirrorNone(void *src, void *dest, zAxis axis){ return NULL; }
static void _zPrim3DDestroyNone(void *prim){}
static void *_zPrim3DXformNone(void *src, zFrame3D *f, void *dest){ return NULL; }
static void *_zPrim3DXformInvNone(void *src, zFrame3D *f, void *dest){ return NULL; }
static double _zPrim3DClosestNone(void *prim, zVec3D *p, zVec3D *cp){ return HUGE_VAL; }
static double _zPrim3DPointDistNone(void *prim, zVec3D *p){ return HUGE_VAL; }
static bool _zPrim3DPointIsInsideNone(void *prim, zVec3D *p, bool rim){ return false; }
static double _zPrim3DVolumeNone(void *prim){ return 0; }
static zVec3D *_zPrim3DBarycenterNone(void *prim, zVec3D *c){ return NULL; }
static zMat3D *_zPrim3DInertiaNone(void *prim, zMat3D *i){ return NULL; }
static void _zPrim3DBaryInertiaNone(void *prim, zVec3D *c, zMat3D *i){}
static zPH3D *_zPrim3DToPHNone(void *prim, zPH3D *ph){ return NULL; }
static void *_zPrim3DParseZTKNone(void *prim, ZTK *ztk){ return NULL; }
static void *_zPrim3DFScanNone(FILE *fp, void *prim){ return NULL; }
static void _zPrim3DFPrintNone(FILE *fp, void *prim){}

zPrimCom zprim_none_com = {
  _zPrim3DInitNone,
  _zPrim3DCloneNone,
  _zPrim3DMirrorNone,
  _zPrim3DDestroyNone,
  _zPrim3DXformNone,
  _zPrim3DXformInvNone,
  _zPrim3DClosestNone,
  _zPrim3DPointDistNone,
  _zPrim3DPointIsInsideNone,
  _zPrim3DVolumeNone,
  _zPrim3DBarycenterNone,
  _zPrim3DInertiaNone,
  _zPrim3DBaryInertiaNone,
  _zPrim3DToPHNone,
  _zPrim3DParseZTKNone,
  _zPrim3DFScanNone,
  _zPrim3DFPrintNone,
};
