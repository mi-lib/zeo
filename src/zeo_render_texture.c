/* zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_render_texture - rendering: texture map
 */

#include <zeo/zeo_render_texture.h>

/* texture file reader */
bool (* __z_texture_read_file)(zTexture *, const char *) = NULL;

/* allocate coordinates and faces of a texture data */
zTexture *zTextureAlloc(zTexture *texture, int nc, int nt)
{
  zArrayAlloc( &texture->coord, zVec2D, nc );
  zArrayAlloc( &texture->face, zTri2D, nt );
  if( !zArrayBuf(&texture->coord) || !zArrayBuf(&texture->face) ){
    zTextureDestroy( texture );
    return NULL;
  }
  return texture;
}

/* set coordinates in a texture image */
zVec2D *zTextureSetCoord(zTexture *texture, int i, double u, double v)
{
  if( !zArrayPosIsValid( &texture->coord, i ) ) return NULL;
  return zVec2DCreate( zTextureCoord(texture,i), u, v );
}

/* set a triangular face in a texure image */
zTri2D *zTextureSetFace(zTexture *texture, int i, const zVec2D *v1, const zVec2D *v2, const zVec2D *v3)
{
  if( !zArrayPosIsValid( &texture->face, i ) ) return NULL;
  return zTri2DCreate( zTextureFace(texture,i), v1, v2, v3 );
}

/* destroy a texture */
void zTextureDestroy(zTexture *texture)
{
  zNameFree( texture );
  if( texture->filename ) free( texture->filename );
  zArrayFree( &texture->coord );
  zArrayFree( &texture->face );
  zTextureInit( texture );
}

/* clone a texture */
zTexture *zTextureClone(const zTexture *org, zTexture *cln)
{
  zTextureInit( cln );
  if( zNamePtr(org) && !zNameSet( cln, zNamePtr(org) ) ) goto FAILURE;
  cln->id = org->id;
  cln->type = org->type;
  cln->filename = zStrClone( org->filename );
  zArrayAlloc( &cln->coord, zVec2D, zTextureCoordNum(org) );
  zArrayAlloc( &cln->face, zTri2D, zTextureFaceNum(org) );
  if( !cln->filename || !cln->coord.buf || !cln->face.buf ) goto FAILURE;
  return cln;

 FAILURE:
  zTextureDestroy( cln );
  return NULL;
}

/* bump mapping */

/* bump map file reader */
bool (* __z_texture_bump_read_file)(zTexture *, const char *) = NULL;

/* parsing a ZTK format. */

static void *_zTextureNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameSet( (zTexture *)obj, ZTKVal(ztk) );
  return zNamePtr((zTexture *)obj) ? obj : NULL;
}

static void *_zTextureFileFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  if( !( ((zTexture *)obj)->filename = zStrClone( ZTKVal(ztk) ) ) ) return NULL;
  return obj;
}

static void *_zTextureTypeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  if( strcmp( ZTKVal(ztk), "color" ) == 0 )
    ((zTexture *)obj)->type = ZTEXTURE_COLOR;
  else
  if( strcmp( ZTKVal(ztk), "bump" ) == 0 )
    ((zTexture *)obj)->type = ZTEXTURE_BUMP;
  else{
    ZRUNWARN( ZEO_WARN_TEXTURE_UNKNOWN_TYPE, ZTKVal(ztk) );
    ((zTexture *)obj)->type = ZTEXTURE_COLOR;
  }
  return obj;
}

static void *_zTextureDepthFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  if( !( ((zTexture *)obj)->depth = ZTKDouble(ztk) ) ) return NULL;
  return obj;
}

static void *_zTextureCoordFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  int ci;

  if( ( ci = ZTKInt(ztk) ) != i )
    ZRUNWARN( ZEO_WARN_TEXTURE_COORD_UNMATCH, ci );
  zVec2DFromZTK( zTextureCoord((zTexture*)obj,i), ztk );
  return obj;
}

static void *_zTextureFaceFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  int i0, i1, i2;

  if( ( i0 = ZTKInt(ztk) ) >= zTextureCoordNum((zTexture*)obj) ){
    ZRUNERROR( ZEO_ERR_TEXTURE_INVALID_COORD_ID, i0 );
    return NULL;
  }
  if( ( i1 = ZTKInt(ztk) ) >= zTextureCoordNum((zTexture*)obj) ){
    ZRUNERROR( ZEO_ERR_TEXTURE_INVALID_COORD_ID, i1 );
    return NULL;
  }
  if( ( i2 = ZTKInt(ztk) ) >= zTextureCoordNum((zTexture*)obj) ){
    ZRUNERROR( ZEO_ERR_TEXTURE_INVALID_COORD_ID, i2 );
    return NULL;
  }
  zTri2DCreate( zTextureFace((zTexture*)obj,i),
    zTextureCoord((zTexture*)obj,i0),
    zTextureCoord((zTexture*)obj,i1),
    zTextureCoord((zTexture*)obj,i2) );
  return obj;
}

static bool _zTextureNameFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zTexture*)obj) );
  return true;
}

static bool _zTextureFileFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", ((zTexture*)obj)->filename );
  return true;
}

static bool _zTextureTypeFPrintZTK(FILE *fp, int i, void *obj){
  switch( ((zTexture*)obj)->type ){
  case ZTEXTURE_BUMP:           fprintf( fp, "bump\n" );  break;
  case ZTEXTURE_COLOR: default: fprintf( fp, "color\n" ); break;
  }
  return true;
}

static bool _zTextureDepthFPrintZTK(FILE *fp, int i, void *obj){
  if( ((zTexture*)obj)->type != ZTEXTURE_BUMP ) return false;
  fprintf( fp, "%.10g\n", ((zTexture*)obj)->depth );
  return true;
}

static const ZTKPrp __ztk_prp_texture[] = {
  { ZTK_KEY_ZEO_TEXTURE_NAME,  1, _zTextureNameFromZTK, _zTextureNameFPrintZTK },
  { ZTK_KEY_ZEO_TEXTURE_FILE,  1, _zTextureFileFromZTK, _zTextureFileFPrintZTK },
  { ZTK_KEY_ZEO_TEXTURE_TYPE,  1, _zTextureTypeFromZTK, _zTextureTypeFPrintZTK },
  { ZTK_KEY_ZEO_TEXTURE_DEPTH, 1, _zTextureDepthFromZTK, _zTextureDepthFPrintZTK },
  { ZTK_KEY_ZEO_TEXTURE_COORD,-1, _zTextureCoordFromZTK, NULL },
  { ZTK_KEY_ZEO_TEXTURE_FACE, -1, _zTextureFaceFromZTK, NULL },
};

/* encode a texture from a ZTK format processor. */
zTexture *zTextureFromZTK(zTexture *texture, ZTK *ztk)
{
  int num_coord, num_face;

  zTextureInit( texture );
  if( !ZTKKeyRewind( ztk ) ) return NULL;
  if( ( num_coord = ZTKCountKey( ztk, ZTK_KEY_ZEO_TEXTURE_COORD ) ) == 0 ){
    ZRUNWARN( ZEO_WARN_TEXTURE_EMPTY );
    return NULL;
  }
  num_face = ZTKCountKey( ztk, ZTK_KEY_ZEO_TEXTURE_FACE );
  zArrayAlloc( &texture->coord, zVec2D, num_coord );
  zArrayAlloc( &texture->face, zTri2D, num_face );
  if( zTextureCoordNum(texture) != num_coord ||
      zTextureFaceNum(texture) != num_face ) return NULL;
  /* vertices & faces */
  if( !ZTKEvalKey( texture, NULL, ztk, __ztk_prp_texture ) ) return NULL;
  switch( texture->type ){
  case ZTEXTURE_COLOR:
    if( !zTextureReadFile( texture, texture->filename ) )
      ZRUNWARN( ZEO_WARN_TEXTURE_READFUNCTION_NOT_ASSIGNED );
    break;
  case ZTEXTURE_BUMP:
    if( !zTextureBumpReadFile( texture, texture->filename ) )
      ZRUNWARN( ZEO_WARN_TEXTURE_READFUNCTION_NOT_ASSIGNED );
    break;
  default: ;
  }
  return texture;
}

/* print information of the texture parameter set out to a file. */
void zTextureFPrintZTK(FILE *fp, const zTexture *texture)
{
  int i;

  if( !texture ) return;
  ZTKPrpKeyFPrint( fp, (void *)texture, __ztk_prp_texture );
  for( i=0; i<zTextureCoordNum(texture); i++ ){
    fprintf( fp, "%s: %d ", ZTK_KEY_ZEO_TEXTURE_COORD, i );
    zVec2DFPrint( fp, zTextureCoord(texture,i) );
  }
  for( i=0; i<zTextureFaceNum(texture); i++ ){
    fprintf( fp, "%s: %d %d %d\n", ZTK_KEY_ZEO_TEXTURE_FACE,
      (int)( zTextureFaceCoord(texture,i,0)-zTextureCoordBuf(texture) ),
      (int)( zTextureFaceCoord(texture,i,1)-zTextureCoordBuf(texture) ),
      (int)( zTextureFaceCoord(texture,i,2)-zTextureCoordBuf(texture) ) );
  }
  fprintf( fp, "\n" );
}
