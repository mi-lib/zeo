/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_texture - texture map
 */

#ifndef __ZEO_TEXTURE_H__
#define __ZEO_TEXTURE_H__

#include <zeo/zeo_tri2d.h>

__BEGIN_DECLS

/*! \struct a texture class */
typedef struct{
  Z_NAMED_CLASS
  uint id;           /*!< \brief identifier (for visualization) */
  char *filename;    /*!< \brief name of the image source file */
  zVec2DArray coord; /*!< \brief array of coordinates */
  zTri2DArray face;  /*!< \brief array of triangular faces */
  int width;         /*!< \brief width of the texture image */
  int height;        /*!< \brief height of the texture image */
  ubyte *buf;        /*!< \brief buffer for RGB data */
} zTexture;

#define zTextureCoordNum(t)      zArraySize(&(t)->coord)
#define zTextureCoordBuf(t)      zArrayBuf(&(t)->coord)
#define zTextureCoord(t,i)       zArrayElemNC(&(t)->coord,i)
#define zTextureFaceNum(t)       zArraySize(&(t)->face)
#define zTextureFace(t,i)        zArrayElemNC(&(t)->face,i)
#define zTextureFaceCoord(t,i,j) zTri2DVert( zTextureFace(t,i), j )

/*! \brief initialize a texture */
#define zTextureInit(tex) do{\
  zNameSetPtr( tex, NULL );\
  (tex)->id = -1; \
  (tex)->filename = NULL;\
  zArrayInit( &(tex)->coord );\
  zArrayInit( &(tex)->face );\
  (tex)->width = (tex)->height = 0;\
  (tex)->buf = NULL;\
} while(0)

/*! \brief texture file reader */
extern bool (* __z_texture_read_file)(zTexture *, char *);

/*! \brief set a texture file reader */
#define zTextureSetReadFunc(f) ( __z_texture_read_file = (f) )

/*! \brief read an texture image file */
#define zTextureReadFile(t,f) ( __z_texture_read_file ? __z_texture_read_file( (t), (f) ) : false )

/*! \brief allocate coordinates and faces of a texture data */
__EXPORT zTexture *zTextureAlloc(zTexture *texture, int nc, int nt);

/*! \brief set coordinates in a texture image */
__EXPORT zVec2D *zTextureSetCoord(zTexture *texture, int i, double u, double v);

/*! \brief set a triangular face in a texure image */
__EXPORT zTri2D *zTextureSetFace(zTexture *texture, int i, zVec2D *v1, zVec2D *v2, zVec2D *v3);

/*! \brief destroy a texture */
__EXPORT void zTextureDestroy(zTexture *texture);

/*! \brief clone a texture */
__EXPORT zTexture *zTextureClone(zTexture *org, zTexture *cln);

/* tag to identify a texture */
#define ZTK_TAG_TEXTURE "texture"

/* register a definition of tag-and-key for the texture to a ZTK format processor. */
__EXPORT bool zTextureRegZTK(ZTK *ztk);

/* decode a texture from a ZTK format processor. */
__EXPORT zTexture *zTextureFromZTK(zTexture *texture, ZTK *ztk);

/* print out a texture to the current stream of a file. */
__EXPORT void zTextureFPrintZTK(FILE *fp, zTexture *texture);

/*! \struct zTextureArray
 * \brief array class of a set of textures.
 */
zArrayClass( zTextureArray, zTexture );

__END_DECLS

#endif /* __ZEO_TEXTURE_H__ */
