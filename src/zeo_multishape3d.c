/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_multishape3d - multiple 3D shapes.
 */

#include <zeo/zeo_multishape3d.h>
#include <zeo/zeo_bv3d.h>

/* ********************************************************** */
/* multiple 3D shape class
 * ********************************************************** */

/* initialize multiple shapes. */
zMultiShape3D *zMultiShape3DInit(zMultiShape3D *ms)
{
  zArrayInit( zMultiShape3DShapeArray(ms) );
  zArrayInit( zMultiShape3DOpticArray(ms) );
  zArrayInit( zMultiShape3DTextureArray(ms) );
  return ms;
}

/* destroy multiple shapes. */
void zMultiShape3DDestroy(zMultiShape3D *ms)
{
  int i;

  if( !ms ) return;
  for( i=0; i<zMultiShape3DShapeNum(ms); i++ )
    zShape3DDestroy( zMultiShape3DShape(ms,i) );
  zArrayFree( zMultiShape3DShapeArray(ms) );
  for( i=0; i<zMultiShape3DOpticNum(ms); i++ )
    zOpticalInfoDestroy( zMultiShape3DOptic(ms,i) );
  zArrayFree( zMultiShape3DOpticArray(ms) );
  for( i=0; i<zMultiShape3DTextureNum(ms); i++ )
    zTextureDestroy( zMultiShape3DTexture(ms,i) );
  zArrayFree( zMultiShape3DTextureArray(ms) );
}

/* clone multiple shapes. */
zMultiShape3D *zMultiShape3DClone(const zMultiShape3D *org)
{
  zMultiShape3D *cln;
  int i;

  if( !( cln = zAlloc( zMultiShape3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  /* optical info. array */
  zMultiShape3DAllocOpticArray( cln, zMultiShape3DOpticNum(org) );
  if( zMultiShape3DOpticNum(cln) != zMultiShape3DOpticNum(org) ) return NULL;
  for( i=0; i<zMultiShape3DOpticNum(cln); i++ )
    if( !zOpticalInfoClone( zMultiShape3DOptic(org,i), zMultiShape3DOptic(cln,i) ) )
      return NULL;
  /* texture array */
  zMultiShape3DAllocTextureArray( cln, zMultiShape3DTextureNum(org) );
  if( zMultiShape3DTextureNum(cln) != zMultiShape3DTextureNum(org) ) return NULL;
  for( i=0; i<zMultiShape3DTextureNum(cln); i++ )
    if( !zTextureClone( zMultiShape3DTexture(org,i), zMultiShape3DTexture(cln,i) ) )
      return NULL;
  /* shape array */
  zMultiShape3DAllocShapeArray( cln, zMultiShape3DShapeNum(org) );
  if( zMultiShape3DShapeNum(cln) != zMultiShape3DShapeNum(org) ) return NULL;
  for( i=0; i<zMultiShape3DShapeNum(cln); i++ )
    if( !zShape3DClone( zMultiShape3DShape(org,i), zMultiShape3DShape(cln,i),
          zMultiShape3DOptic( cln, zShape3DOptic(zMultiShape3DShape(org,i)) - zMultiShape3DOpticBuf(org) ) ) )
      return NULL;
  return cln;
}

/* find contiguous vertex of multiple shapes to a point. */
const zVec3D *zMultiShape3DContigVert(const zMultiShape3D *ms, const zVec3D *point, double *distance)
{
  int i;
  const zVec3D *v, *nv;
  double _d, dist_min;

  if( !distance ) distance = &_d;
  v = zShape3DContigVert( zMultiShape3DShape(ms,0), point, distance );
  dist_min = *distance;
  for( i=1; i<zMultiShape3DShapeNum(ms); i++ ){
    nv = zShape3DContigVert( zMultiShape3DShape(ms,i), point, distance );
    if( *distance < dist_min ){
      v = nv;
      dist_min = *distance;
    }
  }
  *distance = dist_min;
  return v;
}

/* find the closest point on multiple shapes from a point. */
double zMultiShape3DClosest(const zMultiShape3D *ms, const zVec3D *point, zVec3D *closestpoint)
{
  int i;
  zVec3D cp;
  double dist, dist_min;

  dist_min = zShape3DClosest( zMultiShape3DShape(ms,0), point, closestpoint );
  for( i=1; i<zMultiShape3DShapeNum(ms); i++ )
    if( ( dist = zShape3DClosest( zMultiShape3DShape(ms,i), point, &cp ) ) < dist_min ){
      zVec3DCopy( &cp, closestpoint );
      dist_min = dist;
    }
  return dist_min;
}

/* check if a point is inside of multiple shapes. */
bool zMultiShape3DPointIsInside(const zMultiShape3D *ms, const zVec3D *point, double margin)
{
  int i;

  for( i=0; i<zMultiShape3DShapeNum(ms); i++ )
    if( zShape3DPointIsInside( zMultiShape3DShape(ms,i), point, margin ) )
      return true;
  return false;
}

/* convert multiple shapes to polyhedra. */
zMultiShape3D *zMultiShape3DToPH(zMultiShape3D *ms)
{
  int i;

  for( i=0; i<zMultiShape3DShapeNum(ms); i++ )
    if( !zShape3DToPH( zMultiShape3DShape(ms,i) ) ) return NULL;
  return ms;
}

/* export a set of vertices of multiple 3D shapes. */
zVec3DData *zMultiShape3DVertData(const zMultiShape3D *ms, zVec3DData *data)
{
  zShape3D *sp, s;
  int i, j;
  bool result = true;

  zVec3DDataInitList( data );
  for( i=0; i<zMultiShape3DShapeNum(ms); i++ ){
    sp = zMultiShape3DShape(ms,i);
    if( sp->com == &zeo_shape3d_ph_com ){
      for( j=0; j<zShape3DVertNum(sp); j++ ){
        if( !zVec3DDataAdd( data, zShape3DVert(sp,j) ) ) goto ZEO_MULTISHAPE3DVERTDATA_ERROR;
      }
    } else{
      zShape3DClone( sp, &s, NULL );
      if( !zShape3DToPH( &s ) ){
        result = false;
      } else
      for( j=0; j<zShape3DVertNum(&s); j++ ){
        if( !zVec3DDataAdd( data, zShape3DVert(&s,j) ) ) result = false;
      }
      zShape3DDestroy( &s );
      if( !result ) goto ZEO_MULTISHAPE3DVERTDATA_ERROR;
    }
  }
  return data;

 ZEO_MULTISHAPE3DVERTDATA_ERROR:
  zVec3DDataDestroy( data );
  return NULL;
}

/* generate the bounding ball of multiple 3D shapes. */
zSphere3D *zMultiShape3DBoundingBall(const zMultiShape3D *ms, zSphere3D *boundingball)
{
  zVec3DData data;

  if( zMultiShape3DVertData( ms, &data ) )
    zVec3DDataBoundingBall( &data, boundingball, NULL );
  else
    boundingball = NULL;
  zVec3DDataDestroy( &data );
  return boundingball;
}

/* convert a 3D octree to multiple 3D shapes. */
zMultiShape3D *zMultiShape3DFromOctree(zMultiShape3D *ms, const zVec3DOctree *octree, const zOpticalInfo *oi)
{
  zAABox3DList aabox_list;
  zAABox3DListCell *cp;
  int i, num;
  char box_name[BUFSIZ];

  zMultiShape3DInit( ms );
  zListInit( &aabox_list );
  if( ( num = zVec3DOctreeToAABox3DList( octree, &aabox_list ) ) == 0 ){
    ZRUNWARN( ZEO_WARN_MULTISHAPE_EMPTY_OCTREE );
    goto TERMINATE;
  }
  zMultiShape3DAllocShapeArray( ms, num );
  if( zMultiShape3DShapeNum(ms) != num ){
    ZALLOCERROR();
    goto FAILURE;
  }
  zMultiShape3DAllocOpticArray( ms, 1 );
  if( zMultiShape3DOpticNum(ms) != 1 ){
    ZALLOCERROR();
    goto FAILURE;
  }
  if( oi )
    zOpticalInfoClone( oi, zMultiShape3DOptic(ms,0) );
  else{
    zOpticalInfoInit( zMultiShape3DOptic(ms,0) );
    zNameSet( zMultiShape3DOptic(ms,0), "white" );
  }
  i = 0;
  zListForEach( &aabox_list, cp ){
    if( !zShape3DBoxCreateFromAABox( zMultiShape3DShape(ms,i), &cp->data ) ) goto FAILURE;
    sprintf( box_name, "box%d", i );
    zNameSet( zMultiShape3DShape(ms,i), box_name );
    zShape3DSetOptic( zMultiShape3DShape(ms,i), zMultiShape3DOptic(ms,0) );
    i++;
  }

 TERMINATE:
  zAABox3DListDestroy( &aabox_list );
  return ms;
 FAILURE:
  zMultiShape3DDestroy( ms );
  zAABox3DListDestroy( &aabox_list );
  return NULL;
}

/* parse ZTK format */

static void *_zMultiShape3DOpticFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zOpticalInfoFromZTK( zMultiShape3DOptic((zMultiShape3D*)obj,i), ztk ) ? obj : NULL;
}
static void *_zMultiShape3DTextureFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zTextureFromZTK( zMultiShape3DTexture((zMultiShape3D*)obj,i), ztk ) ? obj : NULL;
}
static void *_zMultiShape3DShapeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zShape3DFromZTK( zMultiShape3DShape((zMultiShape3D*)obj,i),
    zMultiShape3DShapeArray((zMultiShape3D*)obj), zMultiShape3DOpticArray((zMultiShape3D*)obj), zMultiShape3DTextureArray((zMultiShape3D*)obj), ztk ) ? obj : NULL;
}

static bool _zMultiShape3DOpticFPrint(FILE *fp, int i, void *obj){
  zOpticalInfoFPrintZTK( fp, zMultiShape3DOptic( (zMultiShape3D*)obj, i ) );
  return true;
}
static bool _zMultiShape3DTextureFPrint(FILE *fp, int i, void *obj){
  zTextureFPrintZTK( fp, zMultiShape3DTexture( (zMultiShape3D*)obj, i ) );
  return true;
}
static bool _zMultiShape3DShapeFPrint(FILE *fp, int i, void *obj){
  zShape3DFPrintZTK( fp, zMultiShape3DShape( (zMultiShape3D*)obj, i ) );
  return true;
}

static const ZTKPrp __ztk_prp_multishape[] = {
  { ZTK_TAG_ZEO_OPTIC,   -1, _zMultiShape3DOpticFromZTK,   _zMultiShape3DOpticFPrint },
  { ZTK_TAG_ZEO_TEXTURE, -1, _zMultiShape3DTextureFromZTK, _zMultiShape3DTextureFPrint },
  { ZTK_TAG_ZEO_SHAPE,   -1, _zMultiShape3DShapeFromZTK,   _zMultiShape3DShapeFPrint },
};

/* read multiple 3D shapes from a ZTK format processor. */
zMultiShape3D *zMultiShape3DFromZTK(zMultiShape3D *ms, ZTK *ztk)
{
  int num_optic, num_texture, num_shape;

  zMultiShape3DInit( ms );
  num_optic = ZTKCountTag( ztk, ZTK_TAG_ZEO_OPTIC );
  num_texture = ZTKCountTag( ztk, ZTK_TAG_ZEO_TEXTURE );
  num_shape = ZTKCountTag( ztk, ZTK_TAG_ZEO_SHAPE );
  zMultiShape3DAllocOpticArray( ms, num_optic );
  zMultiShape3DAllocTextureArray( ms, num_texture );
  zMultiShape3DAllocShapeArray( ms, num_shape );
  if( zMultiShape3DOpticNum(ms) != num_optic ||
      zMultiShape3DTextureNum(ms) != num_texture ||
      zMultiShape3DShapeNum(ms) != num_shape ) return NULL;
  if( zMultiShape3DShapeNum(ms) == 0 ){
    ZRUNWARN( ZEO_WARN_MULTISHAPE_EMPTY );
    return NULL;
  }
  _ZTKEvalTag( ms, NULL, ztk, __ztk_prp_multishape );
  return ms;
}

/* print multiple 3D shapes out to a file. */
void zMultiShape3DFPrintZTK(FILE *fp, const zMultiShape3D *ms)
{
  ZTKPrp *prp;
  size_t prpnum;

  prpnum = _ZTKPrpNum( __ztk_prp_multishape );
  if( !( prp = ZTKPrpDup( __ztk_prp_multishape, prpnum ) ) ){
    ZALLOCERROR();
    return;
  }
  ZTKPrpSetNum( prp, prpnum, ZTK_TAG_ZEO_OPTIC,   zMultiShape3DOpticNum(ms) );
  ZTKPrpSetNum( prp, prpnum, ZTK_TAG_ZEO_TEXTURE, zMultiShape3DTextureNum(ms) );
  ZTKPrpSetNum( prp, prpnum, ZTK_TAG_ZEO_SHAPE,   zMultiShape3DShapeNum(ms) );
  ZTKPrpTagFPrint( fp, (void *)ms, prp, prpnum );
  free( prp );
}

/* read multiple 3D shapes from a ZTK format file. */
zMultiShape3D *zMultiShape3DReadZTK(zMultiShape3D *ms, const char filename[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  ZTKParse( &ztk, filename );
  ms = zMultiShape3DFromZTK( ms, &ztk );
  ZTKDestroy( &ztk );
  return ms;
}

/* write multiple 3D shapes to a ZTK format file. */
bool zMultiShape3DWriteZTK(const zMultiShape3D *ms, const char filename[])
{
  FILE *fp;

  if( !( fp = zOpenZTKFile( filename, "w" ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  zMultiShape3DFPrintZTK( fp, ms );
  fclose( fp );
  return true;
}
