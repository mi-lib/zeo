/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_bball - 3D bounding volume: bounding ball.
 */

#ifndef __ZEO_BV3D_BBALL_H__
#define __ZEO_BV3D_BBALL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief If the number of points exceeds the this threshold, points inside of the convex hull are discarded from the list. */
#define ZEO_BBALL_PN_THRESHOLD 1000
/* NOTE: This should be optimized through a comparison of exection time of _zBBall() and zCH3D(). */

/*! \brief bounding ball of 3D points.
 *
 * zBBall3D() computes the bounding ball (also known as smallest
 * enclosing ball) of an array of 3D points \a pa.
 * The result is put int \a bb.
 *
 * zBBall3DPL() also computes the bounding ball. For this function,
 * the set of 3D points is given as a vector list \a p.
 *
 * The pointers to points on the sphere will be stored into the
 * array pointed by \a vp, unless \a vp is the null pointer.
 *
 * The algorithm is according to E. Welzl(1991).
 * \return
 * zBBall3D() and zBBall3DPL() return the number of points on the
 * sphere of \a bb.
 */
__ZEO_EXPORT int zBBall3D(zSphere3D *bb, zVec3DArray *pa, zVec3D **vp);
__ZEO_EXPORT int zBBall3DPL(zSphere3D *bb, zVec3DList *pl, zVec3D **vp);

__END_DECLS

#endif /* __ZEO_BV3D_BBALL_H__ */
