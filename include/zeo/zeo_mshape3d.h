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
/* CLASS: zMShape3D
 * multiple 3D shape class
 * ********************************************************** */

typedef struct{
  zShape3DArray shape;
  zOpticalInfoArray optic;
  zTextureArray texture;
} zMShape3D;

#define zMShape3DShapeNum(s)      zArraySize(&(s)->shape)
#define zMShape3DShapeBuf(s)      zArrayBuf(&(s)->shape)
#define zMShape3DShape(s,i)       zArrayElemNC(&(s)->shape,i)
#define zMShape3DSetShapeNum(s,n) ( zArraySize(&(s)->shape) = (n) )
#define zMShape3DSetShapeBuf(s,b) ( zArrayBuf(&(s)->shape) = (b) )

#define zMShape3DOpticNum(s)      zArraySize(&(s)->optic)
#define zMShape3DOpticBuf(s)      zArrayBuf(&(s)->optic)
#define zMShape3DOptic(s,i)       zArrayElemNC(&(s)->optic,i)
#define zMShape3DSetOpticNum(s,n) ( zArraySize(&(s)->optic) = (n) )
#define zMShape3DSetOpticBuf(s,b) ( zArrayBuf(&(s)->optic) = (b) )

#define zMShape3DTextureNum(s)      zArraySize(&(s)->texture)
#define zMShape3DTextureBuf(s)      zArrayBuf(&(s)->texture)
#define zMShape3DTexture(s,i)       zArrayElemNC(&(s)->texture,i)
#define zMShape3DSetTextureNum(s,n) ( zArraySize(&(s)->texture) = (n) )
#define zMShape3DSetTextureBuf(s,b) ( zArrayBuf(&(s)->texture) = (b) )

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
__ZEO_EXPORT zMShape3D *zMShape3DClone(zMShape3D *org);

/*! \brief check if a point is inside of multiple shapes.
 *
 * zMShape3DPointIsInside() checks if a point \a p is inside of the
 * multiple shapes \a ms. \a margin is a margin of the inside area
 * outward from the boundary of \a ms.
 * \return
 * zMShape3DPointIsInside() returns the true value if \a p is inside
 * of \a ms, or the false value otherwise.
 * \sa
 * zShape3DPointIsInside, zPH3DPointIsInside
 */
__ZEO_EXPORT zVec3D *zMShape3DContigVert(zMShape3D *ms, zVec3D *p, double *d);
__ZEO_EXPORT double zMShape3DClosest(zMShape3D *ms, zVec3D *p, zVec3D *cp);
__ZEO_EXPORT bool zMShape3DPointIsInside(zMShape3D *ms, zVec3D *p, double margin);

/*! \brief convert multiple shapes to polyhedra. */
__ZEO_EXPORT zMShape3D *zMShape3DToPH(zMShape3D *ms);

/*! \brief make a list of vertices of multiple 3D shapes. */
__ZEO_EXPORT zVec3DList *zMShape3DVertList(zMShape3D *ms, zVec3DList *vl);

/*! \brief generate the bounding ball of multiple 3D shapes. */
__ZEO_EXPORT zSphere3D *zMShape3DBBall(zMShape3D *ms, zSphere3D *bb);

/*! \brief read multiple 3D shapes from a ZTK format processor. */
__ZEO_EXPORT zMShape3D *zMShape3DFromZTK(zMShape3D *ms, ZTK *ztk);

/*! \brief print multiple 3D shapes to the current position of a file in ZTK format. */
__ZEO_EXPORT void zMShape3DFPrintZTK(FILE *fp, zMShape3D *ms);

/*! \brief scan multiple 3D shapes from a ZTK format file. */
__ZEO_EXPORT zMShape3D *zMShape3DReadZTK(zMShape3D *ms, const char filename[]);

/*! \brief write multiple 3D shapes to a ZTK format file. */
__ZEO_EXPORT bool zMShape3DWriteZTK(zMShape3D *ms, const char filename[]);

__END_DECLS

#endif /* __ZEO_MSHAPE3D_H__ */
