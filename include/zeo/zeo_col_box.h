/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_box - collision checking: axis-aligned box and oriented box.
 */

#ifndef __ZEO_COL_BOX_H__
#define __ZEO_COL_BOX_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* axis-aligned box */

/*! \brief check if a plane and an axis-aligned box intersect with each other.
 *
 * zColChkPlaneAABox3D() checks if a plane \a p and an axis-aligned
 * box \a box intersect with each other.
 */
__EXPORT bool zColChkPlaneAABox3D(zPlane3D *p, zAABox3D *box);

/*! \brief intersection of a plane and an axis-aligned box.
 *
 * zIntersectPlaneAABox3D() returns intersection points between a plane
 * \a p and an axis-aligned box \a box. The results are stored in
 * an array \a ip. The number of intersection points is returned.
 */
__EXPORT int zIntersectPlaneAABox3D(zPlane3D *p, zAABox3D *box, zVec3D ip[]);

/*! \brief check if a triangle intersects with an axis-aligned box.
 *
 * zColChkTriAABox3D() checks if a triangle \a t and an axis-aligned
 * box \a box intersect with each other.
 */
__EXPORT bool zColChkTriAABox3D(zTri3D *t, zAABox3D *box);

/*! \brief intersection of a triangle and an axis-aligned box.
 *
 * zIntersectTriAABox3D() returns intersection points between a
 * triangle \a t and an axis-aligned box \a box. The results are
 * stored in an array \a ip. The number of intersection points
 * is returned.
 */
__EXPORT int zIntersectTriAABox3D(zTri3D *t, zAABox3D *box, zVec3D ip[]);

/*! \brief check collision between two axis-aligned boxes.
 *
 * zColChkAABox3D() checks if two axis-aligned boxes \a b1
 * and \a b2 collide with each other.
 */
__EXPORT bool zColChkAABox3D(zAABox3D *b1, zAABox3D *b2);

/*! \brief get intersection of two axis-aligned boxes.
 *
 * zIntersectAABox3D() gets the intersection of two axis-aligned boxes
 * \a src1 and \a src2 and stores it into \a dst, which is also an
 * axis-aligned box.
 */
__EXPORT zAABox3D *zIntersectAABox3D(zAABox3D *dst, zAABox3D *src1, zAABox3D *src2);

/*! \brief get intersection of axis-aligned boxes of two polyhedra.
 *
 * zIntersectPH3DBox() gets the intersection of axis-aligned boxes
 * of two polyhedra \a ph1 and \a ph2 and stores it to \a box, which
 * is also an axis-aligned box.
 */
__EXPORT zAABox3D *zIntersectPH3DBox(zPH3D *ph1, zPH3D *ph2, zAABox3D *box);

/* box vs box */

/*! \brief check collision between two boxes.
 *
 * zColChkBox3D() checks if a box \a b1 and another
 * \a b2 collide with each other.
 * \retval true \a b1 and \a b2 collide with each other.
 * \retval false \a b1 and \a b2 do not collide.
 */
__EXPORT bool zColChkBox3D(zBox3D *b1, zBox3D *b2);

__END_DECLS

#endif /* __ZEO_COL_BOX_H__ */
