/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_obb - 3D bounding volume: oriented bounding box.
 */

#ifndef __ZEO_BV3D_OBB_H__
#define __ZEO_BV3D_OBB_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief oriented bounding box of 3D points.
 *
 * zVec3DDataOBB() computes an oriented bounding box of a set of points \a data.
 * The result is put into \a obb.
 * \return
 * zVec3DDataOBB() returns a pointer \a obb.
 */
__ZEO_EXPORT zBox3D *zVec3DDataOBB(zVec3DData *data, zBox3D *obb);

__END_DECLS

#endif /* __ZEO_BV3D_OBB_H__ */
