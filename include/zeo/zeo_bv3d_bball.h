/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_bball - 3D bounding volume: bounding ball.
 */

#ifndef __ZEO_BV3D_BBALL_H__
#define __ZEO_BV3D_BBALL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* bounding ball / smallest enclosing ball
 * ********************************************************** */

/*! \brief If the number of points exceeds the this threshold, points inside of the convex hull are discarded from the list. */
#define ZEO_BBALL_PN_THRESHOLD 1000
/* NOTE: This should be optimized through a comparison of exection time of _zBBall() and zCH3D(). */

/*! \brief bounding ball of points.
 *
 * zBBall() computes bounding ball (or, smallest enclosing ball)
 * of a set of points \a p. \a num is the number of points.
 * The result is put int \a bb.
 *
 * zBBallPL() also computes bounding ball. For this function,
 * a set of points is given as a vector list \a p.
 *
 * The pointers to points on the sphere will be stored into the
 * array pointed by \a vp, unless \a vp is the null pointer.
 *
 * The algorithm is according to E. Welzl(1991).
 * \notes
 * For the robustness against numerical error, the radius of
 * \a bb has a margine of zTOL to the actual distance from its
 * center to the furthest point.
 * \return
 * zBBall() and zBBallPL() return the number of points on the
 * sphere of \a bb.
 */
__EXPORT int zBBall(zSphere3D *bb, zVec3D p[], int num, zVec3D **vp);
__EXPORT int zBBallPL(zSphere3D *bb, zVec3DList *p, zVec3D **vp);

__END_DECLS

#endif /* __ZEO_BV3D_BBALL_H__ */
