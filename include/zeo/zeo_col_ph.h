/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_ph - collision checking: polyhedra.
 */

#ifndef __ZEO_COL_PH_H__
#define __ZEO_COL_PH_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief check if two polyhedra intersect with each other.
 *
 * zColChkPH3D() checks if two polyhedra \a ph1 and \a ph2
 * intersect with each other.
 */
__EXPORT bool zColChkPH3D(zPH3D *ph1, zPH3D *ph2, zVec3D *p1, zVec3D *p2);

/*! \brief intersect of two polyhedra by Muller-Preparata's algorithm.
 *
 * zIntersectPH3D() computes the intersection of two 3D convex
 * polyhedra \a ph1 and \a ph2 based on Muller-Preparats's algorithm.
 * \a phcol is the resultant polyhedral intersection.
 * \a ph1 and \a ph2 have to be convices. If not, anything might
 * happen.
 *
 * \return a pointer \a phcol.
 */
__EXPORT zPH3D *zIntersectPH3D(zPH3D *ph1, zPH3D *ph2, zPH3D *phcol);

/*! \brief intersect of two polyhedra by Muller-Preparata's algorithm
 * with a focusing-acceleration.
 *
 * zPH3DIntersectMPFast() computes the intersection of two 3D
 * convex polyhedra \a ph1 and \a ph2 based on Muller-Preparats's
 * algorithm with a focusing-acceleration technique. It utilizes
 * the intersection of AABBs of \a ph1 and \a ph2, which is also
 * an axis-aligned box so that it converts vertices and faces only
 * intersecting with the intersection axis-aligned box.
 * \a phcol is the resultant polyhedral intersection.
 * \a ph1 and \a ph2 have to be convices. If not, anything might
 * happen.
 *
 * \return a pointer \a phcol.
 */
__EXPORT zPH3D *zIntersectPH3DFast(zPH3D *ph1, zPH3D *ph2, zPH3D *phcol);

__END_DECLS

#endif /* __ZEO_COL_PH_H__ */
