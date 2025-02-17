/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_render_texture - rendering: texture map
 */

#ifndef __ZEO_RENDER_TEXTURE_H__
#define __ZEO_RENDER_TEXTURE_H__

#include <zeda/zeda_misc.h>
#include <zeo/zeo_elem2d.h>

__BEGIN_DECLS

enum{
  ZTEXTURE_COLOR,
  ZTEXTURE_BUMP,
};

/*! \struct a texture class */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zTexture ){
  Z_NAMED_CLASS;
  uint id;           /*!< \brief identifier (for visualization) */
  uint id_bump;      /*!< \brief identifier for bump mapping */
  ubyte type;        /*!< \brief type of mapping (color mapping/bump mapping) */
  char *filename;    /*!< \brief name of the image source file */
  zVec2DArray coord; /*!< \brief array of coordinates */
  zTri2DArray face;  /*!< \brief array of triangular faces */
  int width;         /*!< \brief width of the texture image */
  int height;        /*!< \brief height of the texture image */
  double depth;      /*!< \brief depth of bumps (for bump mapping only) */
};

#define zTextureCoordNum(tex)      zArraySize(&(tex)->coord)
#define zTextureCoordBuf(tex)      zArrayBuf(&(tex)->coord)
#define zTextureCoord(tex,i)       zArrayElemNC(&(tex)->coord,i)
#define zTextureFaceNum(tex)       zArraySize(&(tex)->face)
#define zTextureFace(tex,i)        zArrayElemNC(&(tex)->face,i)
#define zTextureFaceCoord(tex,i,j) zTri2DVert( zTextureFace(tex,i), j )

/*! \brief initialize a texture */
#define zTextureInit(tex) do{\
  zNameSetPtr( tex, NULL );\
  (tex)->id = 0;\
  (tex)->id_bump = 0;\
  (tex)->type = ZTEXTURE_COLOR;\
  (tex)->filename = NULL;\
  zArrayInit( &(tex)->coord );\
  zArrayInit( &(tex)->face );\
  (tex)->width = (tex)->height = 0;\
  (tex)->depth = 1.0;\
} while(0)

/*! \brief texture file reader */
__ZEO_EXPORT bool (* __z_texture_read_file)(zTexture *, const char *);

/*! \brief set a texture file reader */
#define zTextureSetReadFunc(f)       ( __z_texture_read_file = (f) )

/*! \brief check if the texture file reader is assigned */
#define zTextureReadFuncIsAssigned() ( __z_texture_read_file ? true : false )

/*! \brief read a texture image file */
#define zTextureReadFile(t,f) ( __z_texture_read_file ? __z_texture_read_file( (t), (f) ) : false )

/*! \brief allocate coordinates and faces of a texture data */
__ZEO_EXPORT zTexture *zTextureAlloc(zTexture *texture, int nc, int nt);

/*! \brief set coordinates in a texture image */
__ZEO_EXPORT zVec2D *zTextureSetCoord(zTexture *texture, int i, double u, double v);

/*! \brief set a triangular face in a texure image */
__ZEO_EXPORT zTri2D *zTextureSetFace(zTexture *texture, int i, const zVec2D *v1, const zVec2D *v2, const zVec2D *v3);

/*! \brief destroy a texture */
__ZEO_EXPORT void zTextureDestroy(zTexture *texture);

/*! \brief clone a texture */
__ZEO_EXPORT zTexture *zTextureClone(const zTexture *org, zTexture *cln);

/* bump mapping */

/*! \brief bump map file reader */
__ZEO_EXPORT bool (* __z_texture_bump_read_file)(zTexture *, const char *);

/*! \brief set a bump map texture file reader */
#define zTextureSetBumpReadFunc(f)   ( __z_texture_bump_read_file = (f) )

/*! \brief check if the texture file reader is assigned */
#define zTextureBumpReadFuncIsAssigned() ( __z_texture_bump_read_file ? true : false )

/*! \brief read a bump map image file */
#define zTextureBumpReadFile(t,f) ( __z_texture_bump_read_file ? __z_texture_bump_read_file( (t), (f) ) : false )

/* tag to identify a texture */
#define ZTK_TAG_ZEO_TEXTURE       "zeo::texture"

#define ZTK_KEY_ZEO_TEXTURE_NAME  "name"
#define ZTK_KEY_ZEO_TEXTURE_FILE  "file"
#define ZTK_KEY_ZEO_TEXTURE_TYPE  "type"
#define ZTK_KEY_ZEO_TEXTURE_DEPTH "depth"
#define ZTK_KEY_ZEO_TEXTURE_COORD "coord"
#define ZTK_KEY_ZEO_TEXTURE_FACE  "face"

/* decode a texture from a ZTK format processor. */
__ZEO_EXPORT zTexture *zTextureFromZTK(zTexture *texture, ZTK *ztk);

/* print out a texture to the current stream of a file. */
__ZEO_EXPORT void zTextureFPrintZTK(FILE *fp, const zTexture *texture);

/*! \struct zTextureArray
 * \brief array class of a set of textures.
 */
zArrayClass( zTextureArray, zTexture );

__END_DECLS

#endif /* __ZEO_RENDER_TEXTURE_H__ */
