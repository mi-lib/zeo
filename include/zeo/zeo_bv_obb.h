/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv_obb - bounding volume: oriented bounding box.
 */

#ifndef __ZEO_BV_OBB_H__
#define __ZEO_BV_OBB_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief oriented bounding box of points.
 *
 * zOBB() computes oriented bounding box of a set of points \a p.
 * \a num is the number of points. The result is put into \a obb.
 *
 * \a obb is a combination of a box and the attitude matrix.
 * About zBox3D class, see zeo_prim_box.h.
 *
 * zOBBPL() also computes oriented bounding box of a set of
 * points, which is given by a list of pointers to points \a pl.
 * \return
 * zOBB() returns a pointer \a obb.
 */
__EXPORT zBox3D *zOBB(zBox3D *obb, zVec3D p[], int n);
__EXPORT zBox3D *zOBBPL(zBox3D *obb, zVec3DList *pl);

__END_DECLS

#endif /* __ZEO_BV_OBB_H__ */
