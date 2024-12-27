/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_boundingball - 3D bounding volume: bounding ball.
 */

#ifndef __ZEO_BV3D_BOUNDINGBALL_H__
#define __ZEO_BV3D_BOUNDINGBALL_H__

/* NOTE: never include this header file in user programs. */

#include <zeo/zeo_bvxd_boundingball.h>

__BEGIN_DECLS

/*! \brief bounding ball of 3D points.
 *
 * zVec3DDataBoundingBall() and zVec3DDataBoundingBallRecursive() compute the bounding ball,
 * which is also known as smallest enclosing ball, of a set of 3D points \a data.
 * The ball in the 3D case means a sphere, and hence, the result is put into a sphere \a bb.
 *
 * The pointers to extreme points on the sphere will be stored into the array pointed by \a vp,
 * unless it is the null pointer.
 *
 * The original algorithm was proposed by E. Welzl (1991).
 * zVec3DDataBoundingBallRecursive() is a straightforward recursive implementation of the algorithm.
 * This is possibly violated due to stack-overflow.
 * On the other hand, zVec3DDataBoundingBall() is a non-recursive implementation. Though it is
 * also based on the original algorithm, it uses an internal action stack to avoid recursive
 * function calls.
 * The latter should be used in user programs if not for any particular reasons.
 * \return
 * zVec3DDataBoundingBall() returns the number of extreme points on the sphere \a bb, which is
 * up to four.
 * \sa
 * zVec2DDataBoundingBall, zVec2DDataBoundingBallRecursive
 */
__ZEO_EXPORT int zVec3DDataBoundingBallRecursive(zVec3DData *data, zSphere3D *bb, zVec3D **vp);
__ZEO_EXPORT int zVec3DDataBoundingBall(zVec3DData *pointdata, zSphere3D *bb, zVec3D **vp);

__END_DECLS

#endif /* __ZEO_BV3D_BOUNDINGBALL_H__ */
