/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_multishape3d - multiple 3D shapes.
 */

#ifndef __ZEO_MULTISHAPE3D_H__
#define __ZEO_MULTISHAPE3D_H__

#include <zeo/zeo_shape3d.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief multiple 3D shape class.
 * ********************************************************** */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zMultiShape3D ){
  zShape3DArray shape_array;
  zOpticalInfoArray optic_array;
  zTextureArray texture_array;
#ifdef __cplusplus
  zMultiShape3D();
  ~zMultiShape3D();
  int shapeNum();
  zShape3D *shape(int i);
  int opticNum();
  zOpticalInfo *optic(int i);
  int textureNum();
  zTexture *texture(int i);
  zMultiShape3D *init();
  void destroy();
  bool allocOpticArray(int num);
  bool allocTextureArray(int num);
  bool allocShapeArray(int num);
  zMultiShape3D *clone();
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
  zMultiShape3D *fromOctree(zVec3DOctree *octree, const zOpticalInfo *oi);
  zMultiShape3D *fromZTK(ZTK *ztk);
  void fprintZTK(FILE *fp);
  zMultiShape3D *readZTK(const char filename[]);
  bool writeZTK(const char filename[]);
#endif /* __cplusplus */
};

#define zMultiShape3DShapeArray(s)    ( &(s)->shape_array )
#define zMultiShape3DShapeNum(s)      zArraySize( zMultiShape3DShapeArray(s) )
#define zMultiShape3DShapeBuf(s)      zArrayBuf( zMultiShape3DShapeArray(s) )
#define zMultiShape3DShape(s,i)       zArrayElem( zMultiShape3DShapeArray(s), i )
#define zMultiShape3DSetShapeNum(s,n) ( zArraySize( zMultiShape3DShapeArray(s) ) = (n) )
#define zMultiShape3DSetShapeBuf(s,b) ( zArrayBuf( zMultiShape3DShapeArray(s) ) = (b) )

#define zMultiShape3DOpticArray(s)    ( &(s)->optic_array )
#define zMultiShape3DOpticNum(s)      zArraySize( zMultiShape3DOpticArray(s) )
#define zMultiShape3DOpticBuf(s)      zArrayBuf( zMultiShape3DOpticArray(s) )
#define zMultiShape3DOptic(s,i)       zArrayElem( zMultiShape3DOpticArray(s), i )
#define zMultiShape3DSetOpticNum(s,n) ( zArraySize( zMultiShape3DOpticArray(s) ) = (n) )
#define zMultiShape3DSetOpticBuf(s,b) ( zArrayBuf( zMultiShape3DOpticArray(s) ) = (b) )

#define zMultiShape3DTextureArray(s)    ( &(s)->texture_array )
#define zMultiShape3DTextureNum(s)      zArraySize( zMultiShape3DTextureArray(s) )
#define zMultiShape3DTextureBuf(s)      zArrayBuf( zMultiShape3DTextureArray(s) )
#define zMultiShape3DTexture(s,i)       zArrayElem( zMultiShape3DTextureArray(s) ,i )
#define zMultiShape3DSetTextureNum(s,n) ( zArraySize( zMultiShape3DTextureArray(s) ) = (n) )
#define zMultiShape3DSetTextureBuf(s,b) ( zArrayBuf( zMultiShape3DTextureArray(s) ) = (b) )

/*! \brief initialize and destroy multiple 3D shapes.
 *
 * zMultiShape3DInit() initializes multiple 3D shapes \a ms,
 * setting all arrays for the null pointer and all the
 * sizes of arrays for 0.
 *
 * zMultiShape3DDestroy() destroys the all inner arrays of \a ms.
 * \return
 * zMultiShape3DInit() returns a pointer to \a ms.
 *
 * zMultiShape3DDestroy() returns no value.
 */
__ZEO_EXPORT zMultiShape3D *zMultiShape3DInit(zMultiShape3D *ms);
__ZEO_EXPORT void zMultiShape3DDestroy(zMultiShape3D *ms);

/*! \brief allocate an array of sets of optical information of multiple 3D shapes */
#define zMultiShape3DAllocOpticArray(ms,num)   zArrayAlloc( zMultiShape3DOpticArray(ms), zOpticalInfo, num )
/*! \brief allocate an array of sets of texture of multiple 3D shapes */
#define zMultiShape3DAllocTextureArray(ms,num) zArrayAlloc( zMultiShape3DTextureArray(ms), zTexture, num )
/*! \brief allocate an array of shapes of multiple 3D shapes */
#define zMultiShape3DAllocShapeArray(ms,num)   zArrayAlloc( zMultiShape3DShapeArray(ms), zShape3D, num )

/*! \brief clone multiple 3D shapes.
 *
 * zMultiShape3DClone() creates a clone of multiple 3D shapes \a org.
 * \return
 * zMultiShape3DClone() returns a pointer to the newly allocated instance of multiple shapes, if it succeeds.
 * Otherwise, it returns the null pointer.
 */
__ZEO_EXPORT zMultiShape3D *zMultiShape3DClone(const zMultiShape3D *org);

/*! \brief check if a point is inside of multiple shapes.
 *
 * zMultiShape3DPointIsInside() checks if a point \a p is inside of the multiple shapes \a ms.
 * \a margin is a margin of the inside area outward from the boundary of \a ms.
 * \return
 * zMultiShape3DPointIsInside() returns the true value if \a p is inside of \a ms. Otherwise,
 * it returns the false value.
 * \sa
 * zShape3DPointIsInside, zPH3DPointIsInside
 */
__ZEO_EXPORT const zVec3D *zMultiShape3DContigVert(const zMultiShape3D *ms, const zVec3D *point, double *distance);
__ZEO_EXPORT double zMultiShape3DClosest(const zMultiShape3D *ms, const zVec3D *point, zVec3D *closestpoint);
__ZEO_EXPORT bool zMultiShape3DPointIsInside(const zMultiShape3D *ms, const zVec3D *point, double margin);

/*! \brief convert multiple shapes to polyhedra. */
__ZEO_EXPORT zMultiShape3D *zMultiShape3DToPH(zMultiShape3D *ms);

/*! \brief export a set of vertices of multiple 3D shapes. */
__ZEO_EXPORT zVec3DData *zMultiShape3DVertData(const zMultiShape3D *ms, zVec3DData *data);

/*! \brief generate the bounding ball of multiple 3D shapes. */
__ZEO_EXPORT zSphere3D *zMultiShape3DBoundingBall(const zMultiShape3D *ms, zSphere3D *boundingball);

/*! \brief convert a 3D octree to multiple 3D shapes. */
__ZEO_EXPORT zMultiShape3D *zMultiShape3DFromOctree(zMultiShape3D *ms, const zVec3DOctree *octree, const zOpticalInfo *oi);

/*! \brief read multiple 3D shapes from a ZTK format processor. */
__ZEO_EXPORT zMultiShape3D *zMultiShape3DFromZTK(zMultiShape3D *ms, ZTK *ztk);

/*! \brief print multiple 3D shapes to the current position of a file in ZTK format. */
__ZEO_EXPORT void zMultiShape3DFPrintZTK(FILE *fp, const zMultiShape3D *ms);

/*! \brief scan multiple 3D shapes from a ZTK format file. */
__ZEO_EXPORT zMultiShape3D *zMultiShape3DReadZTK(zMultiShape3D *ms, const char filename[]);

/*! \brief write multiple 3D shapes to a ZTK format file. */
__ZEO_EXPORT bool zMultiShape3DWriteZTK(const zMultiShape3D *ms, const char filename[]);

__END_DECLS

#ifdef __cplusplus
inline zMultiShape3D::zMultiShape3D(){ zMultiShape3DInit( this ); }
inline zMultiShape3D::~zMultiShape3D(){ zMultiShape3DDestroy( this ); }
inline int zMultiShape3D::shapeNum(){ return zMultiShape3DShapeNum( this ); }
inline zShape3D *zMultiShape3D::shape(int i){ return zMultiShape3DShape( this, i ); }
inline int zMultiShape3D::opticNum(){ return zMultiShape3DOpticNum( this ); }
inline zOpticalInfo *zMultiShape3D::optic(int i){ return zMultiShape3DOptic( this, i ); }
inline int zMultiShape3D::textureNum(){ return zMultiShape3DTextureNum( this ); }
inline zTexture *zMultiShape3D::texture(int i){ return zMultiShape3DTexture( this, i ); }
inline zMultiShape3D *zMultiShape3D::init(){ return zMultiShape3DInit( this ); }
inline void zMultiShape3D::destroy(){ zMultiShape3DDestroy( this ); }
inline bool zMultiShape3D::allocOpticArray(int num){ zMultiShape3DAllocOpticArray( this, num ); return zMultiShape3DOpticNum(this) == num; }
inline bool zMultiShape3D::allocTextureArray(int num){ zMultiShape3DAllocTextureArray( this, num ); return zMultiShape3DTextureNum(this) == num; }
inline bool zMultiShape3D::allocShapeArray(int num){ zMultiShape3DAllocShapeArray( this, num ); return zMultiShape3DShapeNum(this) == num; }
inline zMultiShape3D *zMultiShape3D::clone(){ return zMultiShape3DClone( this ); }
inline const zVec3D *zMultiShape3D::contigVert(const zVec3D *point, double *distance = NULL){ return zMultiShape3DContigVert( this, point, distance ); }
inline const zVec3D *zMultiShape3D::contigVert(const zVec3D &point, double *distance = NULL){ return zMultiShape3DContigVert( this, &point, distance ); }
inline double zMultiShape3D::closest(const zVec3D *point, zVec3D *closestpoint){ return zMultiShape3DClosest( this, point, closestpoint ); }
inline double zMultiShape3D::closest(const zVec3D &point, zVec3D *closestpoint){ return zMultiShape3DClosest( this, &point, closestpoint ); }
inline zVec3D zMultiShape3D::closest(const zVec3D *point){ zVec3D closestpoint; zMultiShape3DClosest( this, point, &closestpoint ); return closestpoint; }
inline zVec3D zMultiShape3D::closest(const zVec3D &point){ zVec3D closestpoint; zMultiShape3DClosest( this, &point, &closestpoint ); return closestpoint; }
inline bool zMultiShape3D::pointIsInside(const zVec3D *point, double margin = zTOL){ return zMultiShape3DPointIsInside( this, point, margin ); }
inline bool zMultiShape3D::pointIsInside(const zVec3D &point, double margin = zTOL){ return zMultiShape3DPointIsInside( this, &point, margin ); }
inline zVec3DData *zMultiShape3D::exportVertData(zVec3DData *data){ return zMultiShape3DVertData( this, data ); }
inline zSphere3D *zMultiShape3D::boudingball(zSphere3D *sphere){ return zMultiShape3DBoundingBall( this, sphere ); }
inline zSphere3D zMultiShape3D::boudingball(){ zSphere3D ball; zMultiShape3DBoundingBall( this, &ball ); return ball; }
inline zMultiShape3D *zMultiShape3D::fromOctree(zVec3DOctree *octree, const zOpticalInfo *oi = NULL){ return zMultiShape3DFromOctree( this, octree, oi ); }
inline zMultiShape3D *zMultiShape3D::fromZTK(ZTK *ztk){ return zMultiShape3DFromZTK( this, ztk ); }
inline void zMultiShape3D::fprintZTK(FILE *fp){ zMultiShape3DFPrintZTK( fp, this ); }
inline zMultiShape3D *zMultiShape3D::readZTK(const char filename[]){ return zMultiShape3DReadZTK( this, filename ); }
inline bool zMultiShape3D::writeZTK(const char filename[]){ return zMultiShape3DWriteZTK( this, filename ); }
#endif /* __cplusplus */

#endif /* __ZEO_MULTISHAPE3D_H__ */
