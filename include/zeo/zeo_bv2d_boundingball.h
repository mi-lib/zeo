/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_boundingball - 2D bounding volume: bounding ball.
 */

#ifndef __ZEO_BV2D_BOUNDINGBALL_H__
#define __ZEO_BV2D_BOUNDINGBALL_H__

/* NOTE: never include this header file in user programs. */

#include <zeo/zeo_bvxd_boundingball.h>

__BEGIN_DECLS

/*! \brief bounding ball of 2D points.
 *
 * zVec2DDataBoundingBall() and zVec2DDataBoundingBallRecursive() compute the bounding ball,
 * which is also known as smallest enclosing ball, of a set of 2D points \a data.
 * The ball in the 2D case means a disk, and hence, the result is put into a disk \a bb.
 *
 * The pointers to extreme points on the disk will be stored into the array pointed by \a vp,
 * unless it is the null pointer.
 *
 * The original algorithm was proposed by E. Welzl (1991).
 * zVec2DDataBoundingBallRecursive() is a straightforward recursive implementation of the algorithm.
 * This is possibly violated due to stack-overflow.
 * On the other hand, zVec2DDataBoundingBall() is a non-recursive implementation. Though it is
 * also based on the original algorithm, it uses an internal action stack to avoid recursive
 * function calls.
 * The latter should be used in user programs if not for any particular reasons.
 * \return
 * zVec2DDataBoundingBall() returns the number of extreme points on the disk \a bb, which is
 * up to three.
 * \sa
 * zVec3DDataBoundingBall, zVec3DDataBoundingBallRecursive
 */
__ZEO_EXPORT int zVec2DDataBoundingBallRecursive(zVec2DData *data, zDisk2D *bb, zVec2D **vp);
__ZEO_EXPORT int zVec2DDataBoundingBall(zVec2DData *data, zDisk2D *bb, zVec2D **vp);

__END_DECLS

#endif /* __ZEO_BV2D_BOUNDINGBALL_H__ */
