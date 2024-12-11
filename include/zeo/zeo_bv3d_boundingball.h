/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_boundingball - 3D bounding volume: bounding ball.
 */

#ifndef __ZEO_BV3D_BOUNDINGBALL_H__
#define __ZEO_BV3D_BOUNDINGBALL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief If the number of points exceeds the this threshold, points inside of the convex hull are discarded from the list. */
#define ZEO_BOUNDINGBALL_PN_THRESHOLD 1000
/* NOTE: This should be optimized through a comparison of exection time of zVec3DDataBoundingBall() and zVec3DDataConvexHull(). */

/*! \brief bounding ball of 3D points.
 *
 * zVec3DDataBoundingBall() computes the bounding ball (also known as smallest enclosing ball)
 * of a set of 3D points \a data. The result is put int \a bb.
 *
 * The pointers to extreme points on the sphere will be stored into the array pointed by \a vp,
 * unless it is the null pointer.
 *
 * The algorithm is according to E. Welzl(1991).
 * \return
 * zVec3DDataBoundingBall() returns the number of extreme points on the sphere \a bb.
 */
__ZEO_EXPORT int zVec3DDataBoundingBall(zVec3DData *data, zSphere3D *bb, zVec3D **vp);

__END_DECLS

#endif /* __ZEO_BV3D_BOUNDINGBALL_H__ */
