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
 * zConvexHull2D() creates a planar convex hull of a set of points \a vert,
 * which are placed on a plane. \a n is the number of points.
 * The convex hull is represented as a list of pointers to the
 * vertices in \a vert.
 *
 * zConvexHull2DPL() also computes a planar convex hull of the set of points
 * given as a vector list \a pl.
 *
 * For these functions, it is supposed that the vertices are placed
 * on a common plane.
 *
 * zConvexHull2D2PH3D(), zConvexHull2DPL2PH3D() create a planar convex hull as a
 * polyhedron \a ch with dual-face triangles. For each function,
 * the set of points is given as an array and a list of vertices,
 * respectively.
 * \return
 * zConvexHull2D() and zConvexHull2DPL() return a pointer \a list, if succeeding.
 * When it fails to allocate cells dynamically, the null pointer is returned.
 *
 * zConvexHull2D2PH3D() and zConvexHull2DPL2PH3D() return a pointer to \a ch created.
 * If they fail to create it, the null pointer is returned.
 */
__ZEO_EXPORT zLoop3D *zConvexHull2D(zLoop3D *ch, zVec3D vert[], int n);
__ZEO_EXPORT zLoop3D *zConvexHull2DPL(zLoop3D *ch, zVec3DList *pl);
__ZEO_EXPORT zPH3D *zConvexHull2D2PH3D(zPH3D *ch, zVec3D vert[], int n);
__ZEO_EXPORT zPH3D *zConvexHull2DPL2PH3D(zPH3D *ch, zVec3DList *pl);

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
