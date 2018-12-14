/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col - collision checking.
 */

#ifndef __ZEO_COL_H__
#define __ZEO_COL_H__

#include <zeo/zeo_bv.h>

__BEGIN_DECLS

/*! \brief collision and intersection between an edge and a plane.
 *
 * zColChkEdgePlane3D() sees if an edge \a e and a plane \a p
 * intersect with each other.
 *
 * zIntersectEdgePlane3D() calculates the all intersection
 * points between \a e and \a p, sets them into the array
 * \a ip and returns the number of them.
 * The following three cases can happen.
 *  1. 'e' and 'p' does not intersect. In this case, 'ip'
 *     is not treated, and the function returns 0.
 *  2. 'e' and 'p' intersect at one point. In this case,
 *     the point is set into the first component of 'ip',
 *     and the function returns 1.
 *  3. 'e' is perfectly on 'p'. In this case, though infinite
 *     number of intersection points exist, the function
 *     only sets the two endpoints of 'e' into the first
 *     and second components of 'ip'.
 * Consequently, the number of intersection points is at
 * most two, so that the size of 'ip' should be equal to
 * or more than two.
 * [RETURN VALUE]
 * zColChkEdgePlane3D() returns the true value if
 * 'e' and 'p' intersect with each other. Otherwise, the
 * false value is returned.
 *
 * zIntersectEdgePlane3D() returns the number of
 * intersection points, which can be 0, 1, or 2.
 */
__EXPORT bool zColChkEdgePlane3D(zEdge3D *e, zPlane3D *p);
__EXPORT int zIntersectEdgePlane3D(zEdge3D *e, zPlane3D *p, zVec3D ip[]);

/*! \brief collision and intersection of plane, edge and triangle.
 *
 * zColChkTriPlane3D() checks if 3D triangle 't' and
 * 3D plane 'p' intersect with each other.
 * zIntersectTriPlane3D() calculates all the intersection
 * points between 't' and 'p', sets them into the array
 * 'ip' and returns the number of them.
 * In the case that 't' is perfectly on 'p', though
 * infinite number of intersection points exist, the
 * function only sets the three vertices of 't' into
 * the first three components of 'ip'. Consequently,
 * the number of intersection points is at most three,
 * so that the size of 'ip' should be equal to or more
 * than three.
 *
 * zColChkEdgeTri3D() checks if 3D edge 'e' and
 * 3D triangle 't' intersect with each other.
 * zIntersectEdgeTri3D() calculates all the intersection
 * points between 'e' and 't' and sets them into 'ip'.
 * In the case that 'e' is included in 't', though infinite
 * number of intersection points exist, the function only
 * sets the two endpoints of 'e' into the first and
 * second components of 'ip'. Consequently, the number
 * of intersection points is at most two, so that the
 * size of 'ip' should be equal to or more than two.
 *
 * zColChkTri3D() checks if two 3D triangles 't1'
 * and 't2' intersect with each other.
 * zIntersectTri3D() calculates all the intersection
 * points between 't1' and 't2' and sets them into 'ip'.
 * In the case that one is included in the other (or the
 * two perfectly coincide with each other), though infinite
 * number of intersection points exist, the function only
 * sets the three vertices of the included into the first
 * three components of 'ip'. Consequently, the number
 * of intersection points is at most three, so that the
 * size of 'ip' should be equal to or more than three.
 * [RETURN VALUE]
 * zColChkTriPlane3D(), zColChkEdgeTri3D() and
 * zColChkTri3D() return the true value if the given two
 * intersect with each other. Otherwise, the false value
 * is returned.
 *
 * zIntersectTriPlane3D(), zIntersectEdgeTri3D() and
 * zIntersectTri3D() return the number of intersection
 * points.
 */
__EXPORT bool zColChkTriPlane3D(zTri3D *t, zPlane3D *p);
__EXPORT int zIntersectTriPlane3D(zTri3D *t, zPlane3D *p, zVec3D ip[]);
__EXPORT bool zColChkEdgeTri3D(zEdge3D *e, zTri3D *t);
__EXPORT int zIntersectEdgeTri3D(zEdge3D *e, zTri3D *t, zVec3D ip[]);
__EXPORT bool zColChkTri3D(zTri3D *t1, zTri3D *t2);
__EXPORT int zIntersectTri3D(zTri3D *t1, zTri3D *t2, zVec3D ip[]);

/*! \brief intersection between a loop of vertices and 3D plane.
 *
 * zIntersectVecListPlane3D() computes all intersection points
 * between a loop of vertices given by \a vl and a 3D plane \a p.
 * \a ip is an array of 3D vectors to store the intersection points.
 * \retval the number of intersection points.
 * \notes the size of \a ip has to be larger than the valued returned
 * by the function.
 */
__EXPORT int zIntersectVecListPlane3D(zVec3DList *vl, zPlane3D *p, zVec3D ip[]);

__END_DECLS

#include <zeo/zeo_col_box.h> /* axis-aligned box and oriented box */
#include <zeo/zeo_col_aabbtree.h> /* AABB tree */
#include <zeo/zeo_col_minkowski.h> /* Minkowski sum */
#include <zeo/zeo_col_gjk.h> /* GJK algorithm */
#include <zeo/zeo_col_ph.h>  /* polyhedra */

#endif /* __ZEO_COL_H__ */
