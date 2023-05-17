/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_obb - 3D bounding volume: oriented bounding box.
 */

#ifndef __ZEO_BV3D_OBB_H__
#define __ZEO_BV3D_OBB_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief oriented bounding box of points.
 *
 * zOBB3D() computes oriented bounding box of a set of points \a p.
 * \a num is the number of points. The result is put into \a obb.
 *
 * \a obb is a combination of a box and the attitude matrix.
 * About zBox3D class, see zeo_prim_box.h.
 *
 * zOBB3DPL() also computes oriented bounding box of a set of
 * points, which is given by a list of pointers to points \a pl.
 * \return
 * zOBB3D() returns a pointer \a obb.
 */
__ZEO_EXPORT zBox3D *zOBB3D(zBox3D *obb, zVec3D p[], int n);
__ZEO_EXPORT zBox3D *zOBB3DPL(zBox3D *obb, zVec3DList *pl);

__END_DECLS

#endif /* __ZEO_BV3D_OBB_H__ */
