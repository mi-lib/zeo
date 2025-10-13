/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mshape3d - multiple 3D shape
 */

#include <zeo/zeo_mshape3d.h>
#include <zeo/zeo_bv3d.h>

/* ********************************************************** */
/* multiple 3D shape class
 * ********************************************************** */

/* initialize multiple shapes. */
zMShape3D *zMShape3DInit(zMShape3D *ms)
{
  zArrayInit( zMShape3DShapeArray(ms) );
  zArrayInit( zMShape3DOpticArray(ms) );
  zArrayInit( zMShape3DTextureArray(ms) );
  return ms;
}

/* destroy multiple shapes. */
void zMShape3DDestroy(zMShape3D *ms)
{
  int i;

  if( !ms ) return;
  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    zShape3DDestroy( zMShape3DShape(ms,i) );
  zArrayFree( zMShape3DShapeArray(ms) );
  for( i=0; i<zMShape3DOpticNum(ms); i++ )
    zOpticalInfoDestroy( zMShape3DOptic(ms,i) );
  zArrayFree( zMShape3DOpticArray(ms) );
  for( i=0; i<zMShape3DTextureNum(ms); i++ )
    zTextureDestroy( zMShape3DTexture(ms,i) );
  zArrayFree( zMShape3DTextureArray(ms) );
}

/* clone multiple shapes. */
zMShape3D *zMShape3DClone(const zMShape3D *org)
{
  zMShape3D *cln;
  int i;

  if( !( cln = zAlloc( zMShape3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  /* optical info. array */
  zMShape3DAllocOpticArray( cln, zMShape3DOpticNum(org) );
  if( zMShape3DOpticNum(cln) != zMShape3DOpticNum(org) ) return NULL;
  for( i=0; i<zMShape3DOpticNum(cln); i++ )
    if( !zOpticalInfoClone( zMShape3DOptic(org,i), zMShape3DOptic(cln,i) ) )
      return NULL;
  /* texture array */
  zMShape3DAllocTextureArray( cln, zMShape3DTextureNum(org) );
  if( zMShape3DTextureNum(cln) != zMShape3DTextureNum(org) ) return NULL;
  for( i=0; i<zMShape3DTextureNum(cln); i++ )
    if( !zTextureClone( zMShape3DTexture(org,i), zMShape3DTexture(cln,i) ) )
      return NULL;
  /* shape array */
  zMShape3DAllocShapeArray( cln, zMShape3DShapeNum(org) );
  if( zMShape3DShapeNum(cln) != zMShape3DShapeNum(org) ) return NULL;
  for( i=0; i<zMShape3DShapeNum(cln); i++ )
    if( !zShape3DClone( zMShape3DShape(org,i), zMShape3DShape(cln,i),
          zMShape3DOptic( cln, zShape3DOptic(zMShape3DShape(org,i)) - zMShape3DOpticBuf(org) ) ) )
      return NULL;
  return cln;
}

/* find contiguous vertex of multiple shapes to a point. */
const zVec3D *zMShape3DContigVert(const zMShape3D *ms, const zVec3D *point, double *distance)
{
  int i;
  const zVec3D *v, *nv;
  double _d, dist_min;

  if( !distance ) distance = &_d;
  v = zShape3DContigVert( zMShape3DShape(ms,0), point, distance );
  dist_min = *distance;
  for( i=1; i<zMShape3DShapeNum(ms); i++ ){
    nv = zShape3DContigVert( zMShape3DShape(ms,i), point, distance );
    if( *distance < dist_min ){
      v = nv;
      dist_min = *distance;
    }
  }
  *distance = dist_min;
  return v;
}

/* find the closest point on multiple shapes from a point. */
double zMShape3DClosest(const zMShape3D *ms, const zVec3D *point, zVec3D *closestpoint)
{
  int i;
  zVec3D cp;
  double dist, dist_min;

  dist_min = zShape3DClosest( zMShape3DShape(ms,0), point, closestpoint );
  for( i=1; i<zMShape3DShapeNum(ms); i++ )
    if( ( dist = zShape3DClosest( zMShape3DShape(ms,i), point, &cp ) ) < dist_min ){
      zVec3DCopy( &cp, closestpoint );
      dist_min = dist;
    }
  return dist_min;
}

/* check if a point is inside of multiple shapes. */
bool zMShape3DPointIsInside(const zMShape3D *ms, const zVec3D *point, double margin)
{
  int i;

  for( i=0; i<zMShape3DShapeNum(ms); i++ )
    if( zShape3DPointIsInside( zMShape3DShape(ms,i), point, margin ) )
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

/* export a set of vertices of multiple 3D shapes. */
zVec3DData *zMShape3DVertData(const zMShape3D *ms, zVec3DData *data)
{
  zShape3D *sp, s;
  int i, j;
  bool result = true;

  zVec3DDataInitList( data );
  for( i=0; i<zMShape3DShapeNum(ms); i++ ){
    sp = zMShape3DShape(ms,i);
    if( sp->com == &zeo_shape3d_ph_com ){
      for( j=0; j<zShape3DVertNum(sp); j++ ){
        if( !zVec3DDataAdd( data, zShape3DVert(sp,j) ) ) goto ZMSHAPE3DVERTDATA_ERROR;
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
      if( !result ) goto ZMSHAPE3DVERTDATA_ERROR;
    }
  }
  return data;

 ZMSHAPE3DVERTDATA_ERROR:
  zVec3DDataDestroy( data );
  return NULL;
}

/* generate the bounding ball of multiple 3D shapes. */
zSphere3D *zMShape3DBoundingBall(const zMShape3D *ms, zSphere3D *boundingball)
{
  zVec3DData data;

  if( zMShape3DVertData( ms, &data ) )
    zVec3DDataBoundingBall( &data, boundingball, NULL );
  else
    boundingball = NULL;
  zVec3DDataDestroy( &data );
  return boundingball;
}

/* convert a 3D octree to multiple 3D shapes. */
zMShape3D *zMShape3DFromOctree(zMShape3D *ms, const zVec3DOctree *octree, const zOpticalInfo *oi)
{
  zAABox3DList aabox_list;
  zAABox3DListCell *cp;
  int i, num;
  char box_name[BUFSIZ];

  zMShape3DInit( ms );
  zListInit( &aabox_list );
  if( ( num = zVec3DOctreeToAABox3DList( octree, &aabox_list ) ) == 0 ){
    ZRUNWARN( ZEO_WARN_MSHAPE_EMPTY_OCTREE );
    goto TERMINATE;
  }
  zMShape3DAllocShapeArray( ms, num );
  if( zMShape3DShapeNum(ms) != num ){
    ZALLOCERROR();
    goto FAILURE;
  }
  zMShape3DAllocOpticArray( ms, 1 );
  if( zMShape3DOpticNum(ms) != 1 ){
    ZALLOCERROR();
    goto FAILURE;
  }
  if( oi )
    zOpticalInfoClone( oi, zMShape3DOptic(ms,0) );
  else{
    zOpticalInfoInit( zMShape3DOptic(ms,0) );
    zNameSet( zMShape3DOptic(ms,0), "white" );
  }
  i = 0;
  zListForEach( &aabox_list, cp ){
    if( !zShape3DBoxCreateFromAABox( zMShape3DShape(ms,i), &cp->data ) ) goto FAILURE;
    sprintf( box_name, "box%d", i );
    zNameSet( zMShape3DShape(ms,i), box_name );
    zShape3DSetOptic( zMShape3DShape(ms,i), zMShape3DOptic(ms,0) );
    i++;
  }

 TERMINATE:
  zAABox3DListDestroy( &aabox_list );
  return ms;
 FAILURE:
  zMShape3DDestroy( ms );
  zAABox3DListDestroy( &aabox_list );
  return NULL;
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
    zMShape3DShapeArray((zMShape3D*)obj), zMShape3DOpticArray((zMShape3D*)obj), zMShape3DTextureArray((zMShape3D*)obj), ztk ) ? obj : NULL;
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
  zMShape3DAllocOpticArray( ms, num_optic );
  zMShape3DAllocTextureArray( ms, num_texture );
  zMShape3DAllocShapeArray( ms, num_shape );
  if( zMShape3DOpticNum(ms) != num_optic ||
      zMShape3DTextureNum(ms) != num_texture ||
      zMShape3DShapeNum(ms) != num_shape ) return NULL;
  if( zMShape3DShapeNum(ms) == 0 ){
    ZRUNWARN( ZEO_WARN_MSHAPE_EMPTY );
    return NULL;
  }
  _ZTKEvalTag( ms, NULL, ztk, __ztk_prp_mshape );
  return ms;
}

/* print multiple 3D shapes out to a file. */
void zMShape3DFPrintZTK(FILE *fp, const zMShape3D *ms)
{
  ZTKPrp *prp;
  size_t prpnum;

  prpnum = _ZTKPrpNum( __ztk_prp_mshape );
  if( !( prp = ZTKPrpDup( __ztk_prp_mshape, prpnum ) ) ){
    ZALLOCERROR();
    return;
  }
  ZTKPrpSetNum( prp, prpnum, ZTK_TAG_ZEO_OPTIC,   zMShape3DOpticNum(ms) );
  ZTKPrpSetNum( prp, prpnum, ZTK_TAG_ZEO_TEXTURE, zMShape3DTextureNum(ms) );
  ZTKPrpSetNum( prp, prpnum, ZTK_TAG_ZEO_SHAPE,   zMShape3DShapeNum(ms) );
  ZTKPrpTagFPrint( fp, (void *)ms, prp, prpnum );
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
bool zMShape3DWriteZTK(const zMShape3D *ms, const char filename[])
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
