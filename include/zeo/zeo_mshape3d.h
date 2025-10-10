/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mshape3d - multiple 3D shape
 */

#ifndef __ZEO_MSHAPE3D_H__
#define __ZEO_MSHAPE3D_H__

#include <zeo/zeo_shape3d.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief multiple 3D shape class.
 * ********************************************************** */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zMShape3D ){
  zShape3DArray shape_array;
  zOpticalInfoArray optic_array;
  zTextureArray texture_array;
#ifdef __cplusplus
  zMShape3D();
  ~zMShape3D();
  int shapeNum();
  zShape3D *shape(int i);
  int opticNum();
  zOpticalInfo *optic(int i);
  int textureNum();
  zTexture *texture(int i);
  zMShape3D *init();
  void destroy();
  bool allocOpticArray(int num);
  bool allocTextureArray(int num);
  bool allocShapeArray(int num);
  zMShape3D *clone();
  const zVec3D *contigVert(const zVec3D *point, double *distance);
  const zVec3D *contigVert(const zVec3D &point, double *distance);
  double closest(const zVec3D *point, zVec3D *closestpoint);
  double closest(const zVec3D &point, zVec3D *closestpoint);
  zVec3D closest(const zVec3D *point);
  zVec3D closest(const zVec3D &point);
  bool pointIsInside(const zVec3D *point, double margin);
  bool pointIsInside(const zVec3D &point, double margin);
  zVec3DData *exportVertData(zVec3DData *data);
  zSphere3D *boudingball(zSphere3D *sphere);
  zSphere3D boudingball();
  zMShape3D *fromOctree(zVec3DOctree *octree, const zOpticalInfo *oi);
  zMShape3D *fromZTK(ZTK *ztk);
  void fprintZTK(FILE *fp);
  zMShape3D *readZTK(const char filename[]);
  bool writeZTK(const char filename[]);
#endif /* __cplusplus */
};

#define zMShape3DShapeArray(s)    ( &(s)->shape_array )
#define zMShape3DShapeNum(s)      zArraySize( zMShape3DShapeArray(s) )
#define zMShape3DShapeBuf(s)      zArrayBuf( zMShape3DShapeArray(s) )
#define zMShape3DShape(s,i)       zArrayElem( zMShape3DShapeArray(s), i )
#define zMShape3DSetShapeNum(s,n) ( zArraySize( zMShape3DShapeArray(s) ) = (n) )
#define zMShape3DSetShapeBuf(s,b) ( zArrayBuf( zMShape3DShapeArray(s) ) = (b) )

#define zMShape3DOpticArray(s)    ( &(s)->optic_array )
#define zMShape3DOpticNum(s)      zArraySize( zMShape3DOpticArray(s) )
#define zMShape3DOpticBuf(s)      zArrayBuf( zMShape3DOpticArray(s) )
#define zMShape3DOptic(s,i)       zArrayElem( zMShape3DOpticArray(s), i )
#define zMShape3DSetOpticNum(s,n) ( zArraySize( zMShape3DOpticArray(s) ) = (n) )
#define zMShape3DSetOpticBuf(s,b) ( zArrayBuf( zMShape3DOpticArray(s) ) = (b) )

#define zMShape3DTextureArray(s)    ( &(s)->texture_array )
#define zMShape3DTextureNum(s)      zArraySize( zMShape3DTextureArray(s) )
#define zMShape3DTextureBuf(s)      zArrayBuf( zMShape3DTextureArray(s) )
#define zMShape3DTexture(s,i)       zArrayElem( zMShape3DTextureArray(s) ,i )
#define zMShape3DSetTextureNum(s,n) ( zArraySize( zMShape3DTextureArray(s) ) = (n) )
#define zMShape3DSetTextureBuf(s,b) ( zArrayBuf( zMShape3DTextureArray(s) ) = (b) )

/*! \brief initialize and destroy multiple 3D shapes.
 *
 * zMShape3DInit() initializes multiple 3D shapes \a ms,
 * setting all arrays for the null pointer and all the
 * sizes of arrays for 0.
 *
 * zMShape3DDestroy() destroys the all inner arrays of \a ms.
 * \return
 * zMShape3DInit() returns a pointer to \a ms.
 *
 * zMShape3DDestroy() returns no value.
 */
__ZEO_EXPORT zMShape3D *zMShape3DInit(zMShape3D *ms);
__ZEO_EXPORT void zMShape3DDestroy(zMShape3D *ms);

/*! \brief allocate an array of sets of optical information of multiple 3D shapes */
#define zMShape3DAllocOpticArray(ms,num)   zArrayAlloc( zMShape3DOpticArray(ms), zOpticalInfo, num )
/*! \brief allocate an array of sets of texture of multiple 3D shapes */
#define zMShape3DAllocTextureArray(ms,num) zArrayAlloc( zMShape3DTextureArray(ms), zTexture, num )
/*! \brief allocate an array of shapes of multiple 3D shapes */
#define zMShape3DAllocShapeArray(ms,num)   zArrayAlloc( zMShape3DShapeArray(ms), zShape3D, num )

/*! \brief clone multiple 3D shapes.
 *
 * zMShape3DClone() creates a clone of multiple 3D shapes \a org.
 * \return
 * zMShape3DClone() returns a pointer to the newly allocated instance of multiple shapes, if it succeeds.
 * Otherwise, it returns the null pointer.
 */
__ZEO_EXPORT zMShape3D *zMShape3DClone(const zMShape3D *org);

/*! \brief check if a point is inside of multiple shapes.
 *
 * zMShape3DPointIsInside() checks if a point \a p is inside of the multiple shapes \a ms.
 * \a margin is a margin of the inside area outward from the boundary of \a ms.
 * \return
 * zMShape3DPointIsInside() returns the true value if \a p is inside of \a ms. Otherwise,
 * it returns the false value.
 * \sa
 * zShape3DPointIsInside, zPH3DPointIsInside
 */
__ZEO_EXPORT const zVec3D *zMShape3DContigVert(const zMShape3D *ms, const zVec3D *point, double *distance);
__ZEO_EXPORT double zMShape3DClosest(const zMShape3D *ms, const zVec3D *point, zVec3D *closestpoint);
__ZEO_EXPORT bool zMShape3DPointIsInside(const zMShape3D *ms, const zVec3D *point, double margin);

/*! \brief convert multiple shapes to polyhedra. */
__ZEO_EXPORT zMShape3D *zMShape3DToPH(zMShape3D *ms);

/*! \brief export a set of vertices of multiple 3D shapes. */
__ZEO_EXPORT zVec3DData *zMShape3DVertData(const zMShape3D *ms, zVec3DData *data);

/*! \brief generate the bounding ball of multiple 3D shapes. */
__ZEO_EXPORT zSphere3D *zMShape3DBoundingBall(const zMShape3D *ms, zSphere3D *boundingball);

/*! \brief convert a 3D octree to multiple 3D shapes. */
__ZEO_EXPORT zMShape3D *zMShape3DFromOctree(zMShape3D *ms, const zVec3DOctree *octree, const zOpticalInfo *oi);

/*! \brief read multiple 3D shapes from a ZTK format processor. */
__ZEO_EXPORT zMShape3D *zMShape3DFromZTK(zMShape3D *ms, ZTK *ztk);

/*! \brief print multiple 3D shapes to the current position of a file in ZTK format. */
__ZEO_EXPORT void zMShape3DFPrintZTK(FILE *fp, const zMShape3D *ms);

/*! \brief scan multiple 3D shapes from a ZTK format file. */
__ZEO_EXPORT zMShape3D *zMShape3DReadZTK(zMShape3D *ms, const char filename[]);

/*! \brief write multiple 3D shapes to a ZTK format file. */
__ZEO_EXPORT bool zMShape3DWriteZTK(const zMShape3D *ms, const char filename[]);

__END_DECLS

#ifdef __cplusplus
inline zMShape3D::zMShape3D(){ zMShape3DInit( this ); }
inline zMShape3D::~zMShape3D(){ zMShape3DDestroy( this ); }
inline int zMShape3D::shapeNum(){ return zMShape3DShapeNum( this ); }
inline zShape3D *zMShape3D::shape(int i){ return zMShape3DShape( this, i ); }
inline int zMShape3D::opticNum(){ return zMShape3DOpticNum( this ); }
inline zOpticalInfo *zMShape3D::optic(int i){ return zMShape3DOptic( this, i ); }
inline int zMShape3D::textureNum(){ return zMShape3DTextureNum( this ); }
inline zTexture *zMShape3D::texture(int i){ return zMShape3DTexture( this, i ); }
inline zMShape3D *zMShape3D::init(){ return zMShape3DInit( this ); }
inline void zMShape3D::destroy(){ zMShape3DDestroy( this ); }
inline bool zMShape3D::allocOpticArray(int num){ zMShape3DAllocOpticArray( this, num ); return zMShape3DOpticNum(this) == num; }
inline bool zMShape3D::allocTextureArray(int num){ zMShape3DAllocTextureArray( this, num ); return zMShape3DTextureNum(this) == num; }
inline bool zMShape3D::allocShapeArray(int num){ zMShape3DAllocShapeArray( this, num ); return zMShape3DShapeNum(this) == num; }
inline zMShape3D *zMShape3D::clone(){ return zMShape3DClone( this ); }
inline const zVec3D *zMShape3D::contigVert(const zVec3D *point, double *distance = NULL){ return zMShape3DContigVert( this, point, distance ); }
inline const zVec3D *zMShape3D::contigVert(const zVec3D &point, double *distance = NULL){ return zMShape3DContigVert( this, &point, distance ); }
inline double zMShape3D::closest(const zVec3D *point, zVec3D *closestpoint){ return zMShape3DClosest( this, point, closestpoint ); }
inline double zMShape3D::closest(const zVec3D &point, zVec3D *closestpoint){ return zMShape3DClosest( this, &point, closestpoint ); }
inline zVec3D zMShape3D::closest(const zVec3D *point){ zVec3D closestpoint; zMShape3DClosest( this, point, &closestpoint ); return closestpoint; }
inline zVec3D zMShape3D::closest(const zVec3D &point){ zVec3D closestpoint; zMShape3DClosest( this, &point, &closestpoint ); return closestpoint; }
inline bool zMShape3D::pointIsInside(const zVec3D *point, double margin = zTOL){ return zMShape3DPointIsInside( this, point, margin ); }
inline bool zMShape3D::pointIsInside(const zVec3D &point, double margin = zTOL){ return zMShape3DPointIsInside( this, &point, margin ); }
inline zVec3DData *zMShape3D::exportVertData(zVec3DData *data){ return zMShape3DVertData( this, data ); }
inline zSphere3D *zMShape3D::boudingball(zSphere3D *sphere){ return zMShape3DBoundingBall( this, sphere ); }
inline zSphere3D zMShape3D::boudingball(){ zSphere3D ball; zMShape3DBoundingBall( this, &ball ); return ball; }
inline zMShape3D *zMShape3D::fromOctree(zVec3DOctree *octree, const zOpticalInfo *oi = NULL){ return zMShape3DFromOctree( this, octree, oi ); }
inline zMShape3D *zMShape3D::fromZTK(ZTK *ztk){ return zMShape3DFromZTK( this, ztk ); }
inline void zMShape3D::fprintZTK(FILE *fp){ zMShape3DFPrintZTK( fp, this ); }
inline zMShape3D *zMShape3D::readZTK(const char filename[]){ return zMShape3DReadZTK( this, filename ); }
inline bool zMShape3D::writeZTK(const char filename[]){ return zMShape3DWriteZTK( this, filename ); }
#endif /* __cplusplus */

#endif /* __ZEO_MSHAPE3D_H__ */
