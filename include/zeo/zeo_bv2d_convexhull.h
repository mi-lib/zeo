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

/*! \brief planar convex hull.
 *
 * zVec3DDataConvexHull2D() creates a planar convex hull of a set of 3D points \a data,
 * which are supposed to be on a common plane.
 * The convex hull is represented as a list of pointers to the vertices in \a data, which
 * is stored where \a chloop points.
 * The points are regarded as degenerate if all of them are within zTOL from one point,
 * or the ratio of the distances between the points and a line to the extent of the points
 * along the line is not larger than zTOL.
 * A planar convex hull as a polyhedron is computed by zVec3DDataConvexHull().
 * \return
 * zVec3DDataConvexHull2D() returns a pointer \a chloop, if it succeeds.
 * If the points are degenerate, or it fails to allocate memory for cells of the list, it
 * returns the null pointer.
 */
__ZEO_EXPORT zLoop3D *zVec3DDataConvexHull2D(zVec3DData *data, zLoop3D *chloop);

/*! \brief the closest point in a convex hull to a point.
 *
 * zConvexHull2DClosest() finds the closest point in a convex hull \a ch to
 * a point \a p. \a ch is represented as a list of vertices. The result
 * is stored where \a cp points.
 * If \a p is inside of \a ch, \a p is copied to \a cp.
 * If \a ch is a non-convex hull, this function does not work validly.
 * \return
 * zConvexHull2DClosest() returns the distance from \a p to \a ch.
 * If \a p is inside of \a ch, the value returned is zero.
 * \notes
 * If \a ch is a non-convex hull, anything might happen.
 */
__ZEO_EXPORT double zConvexHull2DClosest(zLoop3D *ch, zVec3D *p, zVec3D *cp);

__END_DECLS

#endif /* __ZEO_BV2D_CONVEXHULL_H__ */
