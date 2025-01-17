/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_aabb - 3D bounding volume: axis-aligned bounding box.
 */

#ifndef __ZEO_BV3D_AABB_H__
#define __ZEO_BV3D_AABB_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief axis-aligned bounding box of 3D points.
 *
 * zVec3DDataAABB() computes the axis-aligned bounding box of a set of 3D points \a data.
 * The result is put into \a bb.
 *
 * The pointers to the extreme points (i.e. points on the faces of \a bb) will be stored into
 * the array pointed by \a vp, unless \a vp is the null pointer. The correspondency of the
 * pointers and faces is depicted as follows.
 *       /~~~~~/|0
 *      / 2 z|/ |
 *     |-----| 4|/x
 *    1|     |  /
 *     |  3  | /
 *  y _|_____|/
 *        5
 *
 * zVec3DDataAABBXform() computes the axis-aligned bouding box of a set of 3D points \a data
 * in a frame \a f. Namely, the actual positions of the points are obtained by transforming
 * individual points in \a data by \a f. The result is put into \a bb.
 * This function does not store addresses of the extreme points.
 * \return
 * zVec3DDataAABB() and zVec3DDataAABBXform() return a pointer \a bb.
 * \notes
 * Arrays pointed by \a vp must have more than six elements, unless it is the null pointer.
 * Since some of the extreme points are possibly at edges or corners of \a bb, some of the
 * elements of \a vp could point same points.
 */
__ZEO_EXPORT zAABox3D *zVec3DDataAABB(zVec3DData *data, zAABox3D *bb, zVec3D **vp);
__ZEO_EXPORT zAABox3D *zVec3DDataAABBXform(zVec3DData *data, zAABox3D *bb, zFrame3D *frame);

__END_DECLS

#endif /* __ZEO_BV3D_AABB_H__ */
