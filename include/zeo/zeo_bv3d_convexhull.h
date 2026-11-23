/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_convexhull - 3D bounding volume: 3D convex hull.
 */

#ifndef __ZEO_BV3D_CONVEXHULL_H__
#define __ZEO_BV3D_CONVEXHULL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief convex hull of 3D points.
 *
 * zVec3DDataConvexHull() computes the convex hull of a set of points \a data based on the
 * quickhull algorithm by C. Barber, D. Dobkin and H. Huhdanpaa (1996).
 * The result is put into \a ch.
 * The vertices of \a ch are points of \a data, and the faces are triangles. Coplanar triangles
 * can be contiguous. \a ch is the exact convex hull of \a data, where points on faces and edges
 * are not vertices. If all points of \a data are on a plane, \a ch is a planar polygon that
 * consists of triangles facing both sides of the plane.
 * The normal vectors of the faces are computed by zTri3DCalcNorm(). Note that the exact convex hull
 * of nearly coplanar points, e.g., points on a plane with rounding errors, can include thin
 * triangles, the normal vectors of which can be the zero vector.
 *
 * zVec3DDataConvexHullSimplify() computes the convex hull of \a data in the same way with
 * zVec3DDataConvexHull(), and then removes vertices close to the convex hull of the other vertices
 * within a tolerance \a tol, which avoids thin triangles from nearly coplanar points. Every point
 * of \a data is inside of \a ch or within \a tol from \a ch. If \a tol is not positive, it is
 * the same with zVec3DDataConvexHull().
 * \return
 * zVec3DDataConvexHull() and zVec3DDataConvexHullSimplify() return a pointer \a ch if they succeed
 * to compute the convex hull. If \a data is empty, includes NaN or all points are collinear, or if
 * they fail to allocate necessary memory, the null pointer is returned.
 */
__ZEO_EXPORT zPH3D *zVec3DDataConvexHull(zVec3DData *data, zPH3D *ch);
__ZEO_EXPORT zPH3D *zVec3DDataConvexHullSimplify(zVec3DData *data, zPH3D *ch, double tol);

__END_DECLS

#endif /* __ZEO_BV3D_CONVEXHULL_H__ */
