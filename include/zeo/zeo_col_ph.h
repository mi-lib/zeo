/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_ph - collision checking: polyhedra.
 */

#ifndef __ZEO_COL_PH_H__
#define __ZEO_COL_PH_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief check if two polyhedra are in collision with each other.
 *
 * zColChkPH3D() checks if the convex hulls of two polyhedra \a ph1 and \a ph2 are in collision
 * with each other within a margin \a margin (see zeo_col.h). Namely, \a ph1 and \a ph2 are regarded
 * as convex. If they intersect and \a margin is negative, the penetration depth is computed by the
 * expanding polytope algorithm in the same way with zGJKDepth(), which is heavier than zGJK(). The
 * pair of the closest points of them, or a point in the intersection of them if they intersect, is
 * put into \a p1 and \a p2 (see zGJK()).
 * The null pointer can be given for \a p1 and \a p2.
 * \return
 * zColChkPH3D() returns the true value if \a ph1 and \a ph2 are in collision with each other.
 * Otherwise, it returns the false value.
 */
__ZEO_EXPORT bool zColChkPH3D(zPH3D *ph1, zPH3D *ph2, double margin, zVec3D *p1, zVec3D *p2);

/*! \brief intersection of two convex polyhedra.
 *
 * zIntersectPH3D() computes the intersection of two convex polyhedra \a ph1 and \a ph2 based on
 * Muller-Preparata's algorithm:
 *   D. E. Muller and F. P. Preparata, Finding the Intersection of Two Convex Polyhedra, Theoretical
 *   Computer Science, 7(2):217-236, 1978.
 * The resultant polyhedron is put into \a phcol.
 * \a ph1 and \a ph2 have to be convex, and the normal vectors of their faces have to direct outward.
 * If not, anything might happen.
 * \return
 * zIntersectPH3D() returns a pointer \a phcol if it succeeds. If the polyhedra do not intersect, or the
 * intersection is degenerate, namely, the radius of the largest ball inscribed in it is not larger
 * than zTOL times the size of the polyhedra (the maximum distance of their vertices from their
 * centroid), the null pointer is returned.
 */
__ZEO_EXPORT zPH3D *zIntersectPH3D(zPH3D *ph1, zPH3D *ph2, zPH3D *phcol);

__END_DECLS

#endif /* __ZEO_COL_PH_H__ */
