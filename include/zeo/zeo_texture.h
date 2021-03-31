/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_texture - texture map
 */

#ifndef __ZEO_TEXTURE_H__
#define __ZEO_TEXTURE_H__

#include <zeo/zeo_tri2d.h>

__BEGIN_DECLS

enum{
  ZTEXTURE_COLOR,
  ZTEXTURE_BUMP,
};

/*! \struct a texture class */
typedef struct{
  Z_NAMED_CLASS
  uint id;           /*!< \brief identifier (for visualization) */
  uint id_bump;      /*!< \brief identifier for bump mapping */
  ubyte type;        /*!< \brief type of mapping (color mapping/bump mapping) */
  char *filename;    /*!< \brief name of the image source file */
  zVec2DArray coord; /*!< \brief array of coordinates */
  zTri2DArray face;  /*!< \brief array of triangular faces */
  int width;         /*!< \brief width of the texture image */
  int height;        /*!< \brief height of the texture image */
  double depth;      /*!< \brief depth of bumps (for bump mapping only) */
  ubyte *buf;        /*!< \brief buffer for texture data */
  ubyte *lbuf[6];    /*!< \brief buffer for normalized light map */
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
  (tex)->id = 0;\
  (tex)->id_bump = 0;\
  (tex)->type = ZTEXTURE_COLOR;\
  (tex)->filename = NULL;\
  zArrayInit( &(tex)->coord );\
  zArrayInit( &(tex)->face );\
  (tex)->width = (tex)->height = 0;\
  (tex)->depth = 1.0;\
  (tex)->buf = NULL;\
  (tex)->lbuf[0] = (tex)->lbuf[1] = (tex)->lbuf[2] = (tex)->lbuf[3] = (tex)->lbuf[4] = (tex)->lbuf[5] = NULL;\
} while(0)

/*! \brief texture file reader */
extern bool (* __z_texture_read_file)(zTexture *, char *);

/*! \brief set a texture file reader */
#define zTextureSetReadFunc(f)       ( __z_texture_read_file = (f) )

/*! \brief check if the texture file reader is assigned */
#define zTextureReadFuncIsAssigned() ( __z_texture_read_file ? true : false )

/*! \brief read a texture image file */
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

/* bump mapping */

/*! \brief bump map file reader */
extern bool (* __z_texture_bump_read_file)(zTexture *, char *);

/*! \brief set a bump map texture file reader */
#define zTextureSetBumpReadFunc(f)   ( __z_texture_bump_read_file = (f) )

/*! \brief check if the texture file reader is assigned */
#define zTextureBumpReadFuncIsAssigned() ( __z_texture_bump_read_file ? true : false )

/*! \brief read a bump map image file */
#define zTextureBumpReadFile(t,f) ( __z_texture_bump_read_file ? __z_texture_bump_read_file( (t), (f) ) : false )

/*! \brief allocate workspace for bump mapping */
__EXPORT bool zTextureBumpAlloc(zTexture *bump, int width, int height);

/* tag to identify a texture */
#define ZTK_TAG_TEXTURE "texture"

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
