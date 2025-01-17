/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_aabb - 2D bounding volume: axis-aligned bounding box.
 */

#ifndef __ZEO_BV2D_AABB_H__
#define __ZEO_BV2D_AABB_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief axis-aligned bounding box of 2D points.
 *
 * zVec2DDataAABB() computes the axis-aligned bounding box of a set of 2D points \a data.
 * The result is put into \a bb.
 * The pointers to the extreme points (i.e. points on the faces of \a bb) will be stored into
 * the array pointed by \a vp, unless \a vp is the null pointer. The correspondency of
 * pointers and faces is depicted as follows.
 *  y|_____
 *   |  1  |
 *  2|     |0
 *  _|_____|_ x
 *      3
 * \return
 * zVec2DDataAABB() returns a pointer \a bb.
 * \notes
 * Arrays pointed by \a vp must have more than four elements, unless it is the null pointer.
 * Since some of the extreme points are possibly at edges or corners of \a bb, some of the
 * elements in \a vp could point same points.
 */
__ZEO_EXPORT zAABox2D *zVec2DDataAABB(zVec2DData *data, zAABox2D *bb, zVec2D **vp);

__END_DECLS

#endif /* __ZEO_BV2D_AABB_H__ */
