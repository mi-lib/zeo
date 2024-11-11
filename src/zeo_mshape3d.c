/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mshape3d - multiple 3D shape
 */

#include <zeo/zeo_mshape3d.h>
#include <zeo/zeo_bv3d.h>

/* ********************************************************** */
/* CLASS: zMShape3D
 * multiple 3D shape class
 * ********************************************************** */

/* initialize multiple shapes. */
zMShape3D *zMShape3DInit(zMShape3D *ms)
{
  zArrayInit( &ms->shape );
  zArrayInit( &ms->optic );
  zArrayInit( &ms->texture );
  return ms;
}

/* destroy multiple shapes. */
void zMShape3DDestroy(zMShape3D *ms)
{
  int i;

  if( !ms ) return;
  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    zShape3DDestroy( zMShape3DShape(ms,i) );
  zArrayFree( &ms->shape );
  for( i=0; i<zMShape3DOpticNum(ms); i++ )
    zOpticalInfoDestroy( zMShape3DOptic(ms,i) );
  zArrayFree( &ms->optic );
  for( i=0; i<zMShape3DTextureNum(ms); i++ )
    zTextureDestroy( zMShape3DTexture(ms,i) );
  zArrayFree( &ms->texture );
}

/* clone multiple shapes. */
zMShape3D *zMShape3DClone(zMShape3D *org)
{
  zMShape3D *cln;
  int i;

  if( !( cln = zAlloc( zMShape3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  /* optical info. array */
  zArrayAlloc( &cln->optic, zOpticalInfo, zMShape3DOpticNum(org) );
  if( zMShape3DOpticNum(cln) != zMShape3DOpticNum(org) ) return NULL;
  for( i=0; i<zMShape3DOpticNum(cln); i++ )
    if( !zOpticalInfoClone( zMShape3DOptic(org,i), zMShape3DOptic(cln,i) ) )
      return NULL;
  /* texture array */
  zArrayAlloc( &cln->texture, zTexture, zMShape3DTextureNum(org) );
  if( zMShape3DTextureNum(cln) != zMShape3DTextureNum(org) ) return NULL;
  for( i=0; i<zMShape3DTextureNum(cln); i++ )
    if( !zTextureClone( zMShape3DTexture(org,i), zMShape3DTexture(cln,i) ) )
      return NULL;
  /* shape array */
  zArrayAlloc( &cln->shape, zShape3D, zMShape3DShapeNum(org) );
  if( zMShape3DShapeNum(cln) != zMShape3DShapeNum(org) ) return NULL;
  for( i=0; i<zMShape3DShapeNum(cln); i++ )
    if( !zShape3DClone( zMShape3DShape(org,i), zMShape3DShape(cln,i),
          zMShape3DOptic( cln, zShape3DOptic(zMShape3DShape(org,i)) - zMShape3DOpticBuf(org) ) ) )
      return NULL;
  return cln;
}

/* find contiguous vertex of multiple shapes to a point. */
zVec3D *zMShape3DContigVert(zMShape3D *ms, zVec3D *p, double *d)
{
  int i;
  zVec3D *v, *nv;
  double _d, dmin;

  if( !d ) d = &_d;
  v = zShape3DContigVert( zMShape3DShape(ms,0), p, d );
  dmin = *d;
  for( i=1; i<zMShape3DShapeNum(ms); i++ ){
    nv = zShape3DContigVert( zMShape3DShape(ms,i), p, d );
    if( *d < dmin ){
      v = nv;
      dmin = *d;
    }
  }
  *d = dmin;
  return v;
}

/* find the closest point on multiple shapes from a point. */
double zMShape3DClosest(zMShape3D *ms, zVec3D *p, zVec3D *cp)
{
  int i;
  zVec3D ncp;
  double d, dmin;

  dmin = zShape3DClosest( zMShape3DShape(ms,0), p, cp );
  for( i=1; i<zMShape3DShapeNum(ms); i++ )
    if( ( d = zShape3DClosest( zMShape3DShape(ms,i), p, &ncp ) ) < dmin ){
      zVec3DCopy( &ncp, cp );
      dmin = d;
    }
  return dmin;
}

/* check if a point is inside of multiple shapes. */
bool zMShape3DPointIsInside(zMShape3D *ms, zVec3D *p, double margin)
{
  int i;

  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    if( zShape3DPointIsInside( zMShape3DShape(ms,i), p, margin ) )
      return true;
  return false;
}

/* convert multiple shapes to polyhedra. */
zMShape3D *zMShape3DToPH(zMShape3D *ms)
{
  int i;

  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    if( !zShape3DToPH( zMShape3DShape(ms,i) ) ) return NULL;
  return ms;
}

/* make a list of vertices of multiple 3D shapes. */
zVec3DList *zMShape3DVertList(zMShape3D *ms, zVec3DList *vl)
{
  zShape3D *sp, s;
  int i, j;

  zListInit( vl );
  for( i=0; i<zMShape3DShapeNum(ms); i++ ){
    sp = zMShape3DShape(ms,i);
    if( sp->com == &zeo_shape3d_ph_com ){
      for( j=0; j<zShape3DVertNum(sp); j++ ){
        if( !zVec3DListAdd( vl, zShape3DVert(sp,j) ) ) return NULL;
      }
    } else{
      zShape3DClone( sp, &s, NULL );
      if( !zShape3DToPH( &s ) ) return NULL;
      if( !zVec3DListAppendArray( vl, &zShape3DPH(&s)->vert ) ) vl = NULL;
      zShape3DDestroy( &s );
      if( !vl ) return NULL;
    }
  }
  return vl;
}

/* generate the bounding ball of multiple 3D shapes. */
zSphere3D *zMShape3DBoundingBall(zMShape3D *ms, zSphere3D *bb)
{
  zVec3DList pl;

  if( zMShape3DVertList( ms, &pl ) )
    zBoundingBall3DPL( bb, &pl, NULL );
  else
    bb = NULL;
  zVec3DListDestroy( &pl );
  return bb;
}

/* parse ZTK format */

static void *_zMShape3DOpticFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zOpticalInfoFromZTK( zMShape3DOptic((zMShape3D*)obj,i), ztk ) ? obj : NULL;
}
static void *_zMShape3DTextureFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zTextureFromZTK( zMShape3DTexture((zMShape3D*)obj,i), ztk ) ? obj : NULL;
}
static void *_zMShape3DShapeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zShape3DFromZTK( zMShape3DShape((zMShape3D*)obj,i),
    &((zMShape3D*)obj)->shape, &((zMShape3D*)obj)->optic, &((zMShape3D*)obj)->texture, ztk ) ? obj : NULL;
}

static bool _zMShape3DOpticFPrint(FILE *fp, int i, void *obj){
  zOpticalInfoFPrintZTK( fp, zMShape3DOptic( (zMShape3D*)obj, i ) );
  return true;
}
static bool _zMShape3DTextureFPrint(FILE *fp, int i, void *obj){
  zTextureFPrintZTK( fp, zMShape3DTexture( (zMShape3D*)obj, i ) );
  return true;
}
static bool _zMShape3DShapeFPrint(FILE *fp, int i, void *obj){
  zShape3DFPrintZTK( fp, zMShape3DShape( (zMShape3D*)obj, i ) );
  return true;
}

static const ZTKPrp __ztk_prp_mshape[] = {
  { ZTK_TAG_ZEO_OPTIC,   -1, _zMShape3DOpticFromZTK,   _zMShape3DOpticFPrint },
  { ZTK_TAG_ZEO_TEXTURE, -1, _zMShape3DTextureFromZTK, _zMShape3DTextureFPrint },
  { ZTK_TAG_ZEO_SHAPE,   -1, _zMShape3DShapeFromZTK,   _zMShape3DShapeFPrint },
};

/* read multiple 3D shapes from a ZTK format processor. */
zMShape3D *zMShape3DFromZTK(zMShape3D *ms, ZTK *ztk)
{
  int num_optic, num_texture, num_shape;

  zMShape3DInit( ms );
  num_optic = ZTKCountTag( ztk, ZTK_TAG_ZEO_OPTIC );
  num_texture = ZTKCountTag( ztk, ZTK_TAG_ZEO_TEXTURE );
  num_shape = ZTKCountTag( ztk, ZTK_TAG_ZEO_SHAPE );
  zArrayAlloc( &ms->optic, zOpticalInfo, num_optic );
  zArrayAlloc( &ms->texture, zTexture, num_texture );
  zArrayAlloc( &ms->shape, zShape3D, num_shape );
  if( zMShape3DOpticNum(ms) != num_optic ||
      zMShape3DTextureNum(ms) != num_texture ||
      zMShape3DShapeNum(ms) != num_shape ) return NULL;
  if( zMShape3DShapeNum(ms) == 0 ){
    ZRUNWARN( ZEO_WARN_MSHAPE_EMPTY );
    return NULL;
  }
  ZTKEvalTag( ms, NULL, ztk, __ztk_prp_mshape );
  return ms;
}

/* print multiple 3D shapes out to a file. */
void zMShape3DFPrintZTK(FILE *fp, zMShape3D *ms)
{
  ZTKPrp *prp;
  size_t size;

  if( !( prp = ZTKPrpDup( __ztk_prp_mshape ) ) ){
    ZALLOCERROR();
    return;
  }
  size = sizeof(__ztk_prp_mshape) / sizeof(ZTKPrp);
  _ZTKPrpSetNum( prp, size, ZTK_TAG_ZEO_OPTIC,   zMShape3DOpticNum(ms) );
  _ZTKPrpSetNum( prp, size, ZTK_TAG_ZEO_TEXTURE, zMShape3DTextureNum(ms) );
  _ZTKPrpSetNum( prp, size, ZTK_TAG_ZEO_SHAPE,   zMShape3DShapeNum(ms) );
  _ZTKPrpTagFPrint( fp, ms, prp, size );
  free( prp );
}

/* read multiple 3D shapes from a ZTK format file. */
zMShape3D *zMShape3DReadZTK(zMShape3D *ms, const char filename[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  ZTKParse( &ztk, filename );
  ms = zMShape3DFromZTK( ms, &ztk );
  ZTKDestroy( &ztk );
  return ms;
}

/* write multiple 3D shapes to a ZTK format file. */
bool zMShape3DWriteZTK(zMShape3D *ms, const char filename[])
{
  FILE *fp;

  if( !( fp = zOpenZTKFile( filename, "w" ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  zMShape3DFPrintZTK( fp, ms );
  fclose( fp );
  return true;
}
