/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_convexhull - 2D bounding volume: planar convex hull.
 */

#ifndef __ZEO_BV2D_CONVEXHULL_H__
#define __ZEO_BV2D_CONVEXHULL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief convex hull of 2D points.
 *
 * zVec2DDataConvexHull() computes the convex hull of a set of 2D points \a data by the monotone
 * chain algorithm proposed by Andrew:
 *   A. M. Andrew, Another Efficient Algorithm for Convex Hulls in Two Dimensions, Information
 *   Processing Letters, 9(5):216-219, 1979.
 * The result is put into \a ch.
 * The vertices of \a ch are points of \a data in counterclockwise order, where points on edges are
 * not vertices. The faces of \a ch are triangles that fan out from the first vertex, namely, the
 * i-th face consists of the 0-th, the (i+1)-th and the (i+2)-th vertices.
 * The convex hull is computed exactly in spite of rounding errors.
 * \return
 * zVec2DDataConvexHull() returns a pointer \a ch if it succeeds to compute the convex hull.
 * If \a data is empty, includes NaN or all points are collinear, or if it fails to allocate
 * necessary memory, the null pointer is returned.
 */
__ZEO_EXPORT zPG2D *zVec2DDataConvexHull(zVec2DData *data, zPG2D *ch);

__END_DECLS

#endif /* __ZEO_BV2D_CONVEXHULL_H__ */
