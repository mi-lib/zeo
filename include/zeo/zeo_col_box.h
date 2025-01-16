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

/*! \brief check if a 3D plane and a 3D axis-aligned box are in collision with each other.
 *
 * zColChkPlaneAABox3D() checks if a 3D plane \a p and a 3D axis-aligned box \a box are in
 * collision with each other.
 * \retval true if \a p and \a box are in collision.
 * \retval false if \a p and \a box are not in collision.
 */
__ZEO_EXPORT bool zColChkPlaneAABox3D(const zPlane3D *p, const zAABox3D *box);

/*! \brief intersection of a plane and an axis-aligned box.
 *
 * zIntersectPlaneAABox3D() finds intersection points between a 3D plane \a p and a 3D
 * axis-aligned box \a box. The results are stored in an array \a ip.
 * \return
 * zIntersectPlaneAABox3D() returns the number of intersection points.
 */
__ZEO_EXPORT int zIntersectPlaneAABox3D(const zPlane3D *p, const zAABox3D *box, zVec3D ip[]);

/*! \brief check if a 3D triangle are in collision with a 3D axis-aligned box.
 *
 * zColChkTriAABox3D() checks if a 3D triangle \a t and a 3D axis-aligned box \a box are in
 * collision with each other.
 * \retval true if \a t and \a box are in collision with each other.
 * \retval false if \a t and \a box are not in collision with each other.
 */
__ZEO_EXPORT bool zColChkTriAABox3D(const zTri3D *t, const zAABox3D *box);

/*! \brief intersection of a 3D triangle and a 3D axis-aligned box.
 *
 * zIntersectTriAABox3D() finds intersection points between a 3D triangle \a t and a 3D
 * axis-aligned box \a box. The results are stored in an array \a ip.
 * \return
 * zIntersectTriAABox3D() returns the number of intersection points.
 */
__ZEO_EXPORT int zIntersectTriAABox3D(const zTri3D *t, const zAABox3D *box, zVec3D ip[]);

/*! \brief check collision between two 3D axis-aligned boxes.
 *
 * zColChkAABox3D() checks if two 3D axis-aligned boxes \a b1 and \a b2 are in collision with each other.
 */
__ZEO_EXPORT bool zColChkAABox3D(const zAABox3D *b1, const zAABox3D *b2);

/*! \brief intersection of two 3D axis-aligned boxes.
 *
 * zIntersectAABox3D() finds the intersection of two 3D axis-aligned boxes \a src1 and \a src2, and
 * stores it into \a dst, which is another 3D axis-aligned box.
 */
__ZEO_EXPORT zAABox3D *zIntersectAABox3D(zAABox3D *dst, const zAABox3D *src1, const zAABox3D *src2);

/*! \brief intersection of 3D axis-aligned boxes of two polyhedra.
 *
 * zIntersectPH3DBox() finds the intersection of 3D axis-aligned boxes of two polyhedra \a ph1 and
 * \a ph2, and stores it to \a box, which is another axis-aligned box.
 */
__ZEO_EXPORT zAABox3D *zIntersectPH3DBox(const zPH3D *ph1, const zPH3D *ph2, zAABox3D *box);

/* box vs box */

/*! \brief check collision between two 3D boxes.
 *
 * zColChkBox3D() checks if a 3D box \a b1 and another \a b2 are in collision with each other.
 * \retval true \a b1 and \a b2 are in collision with each other.
 * \retval false \a b1 and \a b2 are not in collision.
 */
__ZEO_EXPORT bool zColChkBox3D(const zBox3D *b1, const zBox3D *b2);

/* box vs sphere */

/*! \brief check collision between a 3D sphere and a 3D axis-aligned box.
 *
 * zColChkSphereAABox3D() checks if a 3D sphere \a sphere and a 3D axis-aligned box \a box
 * are in collision with each other.
 * \retval true if \a sphere and \a box are in collision with each other.
 * \retval false if \a sphere and \a box are not in collision.
 */
__ZEO_EXPORT bool zColChkSphereAABox3D(const zSphere3D *sphere, const zAABox3D *box);

/*! \brief check collision between a 3D sphere and a 3D box.
 *
 * zColChkSphereBox3D() checks if a 3D sphere \a sphere and a 3D box \a box are in collision
 * with each other.
 * \retval true if \a sphere and \a box are in collision with each other.
 * \retval false if \a sphere and \a box are not in collision.
 */
__ZEO_EXPORT bool zColChkSphereBox3D(const zSphere3D *sphere, const zBox3D *box);

__END_DECLS

#endif /* __ZEO_COL_BOX_H__ */
