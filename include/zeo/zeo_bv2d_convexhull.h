/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_convexhull - 2D bounding volume: planar convex hull.
 */

#ifndef __ZEO_BV2D_CONVEXHULL_H__
#define __ZEO_BV2D_CONVEXHULL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* planar convex hull
 * ********************************************************** */

/*! \brief planar convex hull.
 *
 * zVec3DDataConvexHull2D() creates a planar convex hull of a set of 3D points \a data,
 * which are supposed to be on a common plane.
 * The convex hull is represented as a list of pointers to the vertices in \a data, which
 * is stored where \a chloop points.
 *
 * zVec3DConvexHull2DPH3D() creates a planar convex hull of a set of 3D points \a data as
 * a polyhedron \a ch with dual-face triangles.
 * \return
 * zVec3DDataConvexHull2D() returns a pointer \a chloop, if it succeeds.
 * If it fails to allocate memory for cells of the list, it returns the null pointer.
 *
 * zVec3DConvexHull2DPH3D() returns a pointer \a ch, if it succeeds.
 * If it fails to create it, the null pointer is returned.
 */
__ZEO_EXPORT zLoop3D *zVec3DDataConvexHull2D(zVec3DData *data, zLoop3D *chloop);
__ZEO_EXPORT zPH3D *zVec3DDataConvexHull2DPH3D(zVec3DData *data, zPH3D *ch);

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
