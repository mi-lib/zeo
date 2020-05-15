/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_texture - texture map
 */

#include <zeo/zeo_texture.h>

/* texture file reader */
bool (* __z_texture_read_file)(zTexture *, char *) = NULL;

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
zTri2D *zTextureSetFace(zTexture *texture, int i, zVec2D *v1, zVec2D *v2, zVec2D *v3)
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
  if( texture->buf ) free( texture->buf );
  zTextureInit( texture );
}

/* clone a texture */
zTexture *zTextureClone(zTexture *org, zTexture *cln)
{
  zTextureInit( cln );
  if( zNamePtr(org) && !zNameSet( cln, zNamePtr(org) ) ) goto FAILURE;
  cln->id = org->id;
  cln->filename = zStrClone( org->filename );
  zArrayAlloc( &cln->coord, zVec2D, zTextureCoordNum(org) );
  zArrayAlloc( &cln->face, zTri2D, zTextureFaceNum(org) );
  cln->buf = zAlloc( ubyte, org->width * org->height * 3 );
  if( !cln->filename || !cln->coord.buf || !cln->face.buf || !cln->buf ) goto FAILURE;
  cln->width = org->width;
  cln->height = org->height;
  return cln;

 FAILURE:
  zTextureDestroy( cln );
  return NULL;
}

/* parsing a ZTK format. */

static void *_zTextureNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameSet( (zTexture *)obj, ZTKVal(ztk) );
  return zNamePtr((zTexture *)obj) ? obj : NULL; }

static void *_zTextureFileFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  if( !( ((zTexture *)obj)->filename = zStrClone( ZTKVal(ztk) ) ) ) return NULL;
  zTextureReadFile( (zTexture *)obj, ZTKVal(ztk) );
  return obj; }

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

static void _zTextureNameFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zTexture*)obj) ); }

static void _zTextureFileFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", ((zTexture*)obj)->filename ); }

static ZTKPrp __ztk_prp_texture[] = {
  { "name", 1, _zTextureNameFromZTK, _zTextureNameFPrintZTK },
  { "file", 1, _zTextureFileFromZTK, _zTextureFileFPrintZTK },
  { "coord", -1, _zTextureCoordFromZTK, NULL },
  { "face", -1, _zTextureFaceFromZTK, NULL },
};

/* register a definition of tag-and-key for the texture to a ZTK format processor. */
bool zTextureRegZTK(ZTK *ztk)
{
  return ZTKDefRegPrp( ztk, ZTK_TAG_TEXTURE, __ztk_prp_texture );
}

/* encode a texture from a ZTK format processor. */
zTexture *zTextureFromZTK(zTexture *texture, ZTK *ztk)
{
  int num_coord, num_face;

  zTextureInit( texture );
  if( !ZTKKeyRewind( ztk ) ) return NULL;
  if( ( num_coord = ZTKCountKey( ztk, "coord" ) ) == 0 ){
    ZRUNWARN( ZEO_WARN_TEXTURE_EMPTY );
    return NULL;
  }
  num_face = ZTKCountKey( ztk, "face" );
  zArrayAlloc( &texture->coord, zVec2D, num_coord );
  zArrayAlloc( &texture->face, zTri2D, num_face );
  if( zTextureCoordNum(texture) != num_coord ||
      zTextureFaceNum(texture) != num_face ) return NULL;
  /* vertices & faces */
  return ZTKEvalKey( texture, NULL, ztk, __ztk_prp_texture );
}

/* print information of the texture parameter set out to a file. */
void zTextureFPrintZTK(FILE *fp, zTexture *texture)
{
  register int i;

  if( !texture ) return;
  ZTKPrpKeyFPrint( fp, texture, __ztk_prp_texture );
  for( i=0; i<zTextureCoordNum(texture); i++ ){
    fprintf( fp, "coord: %d ", i );
    zVec2DFPrint( fp, zTextureCoord(texture,i) );
  }
  for( i=0; i<zTextureFaceNum(texture); i++ ){
    fprintf( fp, "face: %d %d %d\n",
      (int)( zTextureFaceCoord(texture,i,0)-zTextureCoordBuf(texture) ),
      (int)( zTextureFaceCoord(texture,i,1)-zTextureCoordBuf(texture) ),
      (int)( zTextureFaceCoord(texture,i,2)-zTextureCoordBuf(texture) ) );
  }
  fprintf( fp, "\n" );
}
