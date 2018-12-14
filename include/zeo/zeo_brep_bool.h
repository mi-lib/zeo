/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_brep_bool - boolean operations of B-Rep.
 */

#ifndef __ZEO_BREP_BOOL_H__
#define __ZEO_BREP_BOOL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief merge a B-Rep to another (destructive).
 *
 * zBREPMerge() merges a B-Rep \a sub to another \a target.
 * This operation is destructive. After \a sub is merged to
 * \a target, it will be an empty set of boundaries.
 * On the other hand, \a target is directly modified as
 * the result of the operation.
 * \retval
 * zBREPMerge() returns a pointer \a target if succeeding to
 * create a new merged B-Rep. If it fails to allocate memory
 * for newly added edges, the null pointer is returned.
 */
__EXPORT zBREP *zBREPMerge(zBREP *target, zBREP *sub);

/* ********************************************************** */
/* CATEGOLY: intersection of polyhedra
 * ********************************************************** */

/*! \brief truncate a polyhedon.
 *
 * zPH3DTrunc() truncates a convex polyhedron \a src
 * by another polyhedron \a cut. The part of \a src which
 * is inside of \a cut will remain, and is put into \a dest.
 * \retval
 * zPH3DTrunc() returns a pointer \a dest if succeeding.
 * Otherwise, the null pointer is returned. The reason of
 * failure is the lack of memory.
 */
__EXPORT zPH3D *zPH3DTruncBREP(zPH3D *src, zPH3D *cut, zPH3D *dest);

/*! \brief intersect of polyhedra.
 *
 * zIntersectPH3DBREP() computes the intersection of two polyhedra
 * \a src1 and \a src2. It is assumed that both \a src1 and
 * \a src2 are convex, or at least, the intersecting part does
 * not contain non-convex shape. If not, anything might happen.
 * \retval
 * zIntersectPH3DBREP() returns a pointer \a dest if succeeding.
 * Otherwise, the null pointer is returned.
 */
__EXPORT zPH3D *zIntersectPH3DBREP(zPH3D *src1, zPH3D *src2, zPH3D *dest);

/*! \brief intersect of polyhedra.
 *
 * zIntersectPH3DBREPFast() computes the intersection of two
 * polyhedra \a src1 and \a src2. It is assumed that both \a src1
 * and \a src2 are convex, or at least, the intersecting part
 * does not contain non-convex shape. If not, anything might
 * happen.
 * It utilizes the intersection of AABBs of \a src1 and \a src2,
 * which is also an axis-aligned box so that it converts
 * vertices and faces only intersecting with the intersection
 * axis-aligned box.
 * \retval
 * zIntersectPH3DBREPFast() returns a pointer \a dest if
 * succeeding. Otherwise, the null pointer is returned.
 */
__EXPORT zPH3D *zIntersectPH3DBREPFast(zPH3D *src1, zPH3D *src2, zPH3D *dest);

__END_DECLS

#endif /* __ZEO_BREP_BOOL_H__ */
