/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv_ch2 - bounding volume: planar convex hull.
 */

#ifndef __ZEO_BV_CH2_H__
#define __ZEO_BV_CH2_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* planar (2D) convex hull
 * ********************************************************** */

/*! \brief planar convex hull.
 *
 * zCH2D() creates a planar convex hull of a set of points \a vert,
 * which are placed on a plane. \a n is the number of points.
 * The convex hull is represented as a list of pointers to the
 * vertices in \a vert.
 *
 * zCH3DPL() also computes a planar convex hull of the set of points
 * given as a vector list \a pl.
 *
 * For these functions, it is supposed that the vertices are placed
 * on a common plane.
 *
 * zCH2D2PH3D(), zCH2DPL2PH3D() create a planar convex hull as a
 * polyhedron \a ch with dual-face triangles. For each function,
 * the set of points is given as an array and a list of vertices,
 * respectively.
 * \return
 * zCH2D() and zCH2DPL() return a pointer \a list, if succeeding.
 * When it fails to allocate cells dynamically, the null pointer is returned.
 *
 * zCH2D2PH3D() and zCH2DPL2PH3D() return a pointer to \a ch created.
 * If they fail to create it, the null pointer is returned.
 */
__EXPORT zLoop3D *zCH2D(zLoop3D *ch, zVec3D vert[], int n);
__EXPORT zLoop3D *zCH2DPL(zLoop3D *ch, zVec3DList *pl);
__EXPORT zPH3D *zCH2D2PH3D(zPH3D *ch, zVec3D vert[], int n);
__EXPORT zPH3D *zCH2DPL2PH3D(zPH3D *ch, zVec3DList *pl);

/*! \brief the closest point in a convex hull to a point.
 *
 * zCH2DClosest() finds the closest point in a convex hull \a ch to
 * a point \a p. \a ch is represented as a list of vertices. The result
 * is stored where \a cp points.
 * If \a p is inside of \a ch, \a p is copied to \a cp.
 * If \a ch is a non-convex hull, this function does not work validly.
 * \return
 * zCH2DClosest() returns the distance from \a p to \a ch.
 * If \a p is inside of \a ch, the value returned is zero.
 * \notes
 * If \a ch is a non-convex hull, anything might happen.
 */
__EXPORT double zCH2DClosest(zLoop3D *ch, zVec3D *p, zVec3D *cp);

__END_DECLS

#endif /* __ZEO_BV_CH2_H__ */
