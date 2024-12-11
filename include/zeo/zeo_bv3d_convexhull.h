/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_convexhull - 3D bounding volume: 3D convex hull.
 */

#ifndef __ZEO_BV3D_CONVEXHULL_H__
#define __ZEO_BV3D_CONVEXHULL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* 3D convex hull
 * ********************************************************** */

/*! \brief convex hull of 3D points.
 *
 * zVec3DDataConvexHull() computes the convex hull of a set of points \a data based on the
 * quickhull algorithm by C. Barber, D. Dobkin and H. Huhdanpaa (1996).
 * The result is put into \a ch.
 * \return
 * zVec3DDataConvexHull() returns a pointer \a ch if it succeeds to compute the convex hull.
 * If it fails to allocate necessary memory for computation, the null pointer is returned.
 */
__ZEO_EXPORT zPH3D *zVec3DDataConvexHull(zVec3DData *data, zPH3D *ch);

__END_DECLS

#endif /* __ZEO_BV3D_CONVEXHULL_H__ */
