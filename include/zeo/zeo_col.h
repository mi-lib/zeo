/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col - collision checking.
 */

#ifndef __ZEO_COL_H__
#define __ZEO_COL_H__

#include <zeo/zeo_bv3d.h>

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
 *  1. \a e and \a p does not intersect. In this case, \a ip
 *     is not treated, and the function returns 0.
 *  2. \a e and \a p intersect at one point. In this case,
 *     the point is set into the first component of \a ip,
 *     and the function returns 1.
 *  3. \a e is perfectly on \a p. In this case, though infinite
 *     number of intersection points exist, the function
 *     only sets the two endpoints of \a e into the first
 *     and second components of \a ip.
 * Consequently, the number of intersection points is at
 * most two, so that the size of \a ip has to be more than
 * or equal to two.
 * \return
 * zColChkEdgePlane3D() returns the true value if \a e and
 * \a p intersect with each other. Otherwise, the false value
 * is returned.
 *
 * zIntersectEdgePlane3D() returns the number of intersection
 * points, 0, 1, or 2.
 */
__EXPORT bool zColChkEdgePlane3D(zEdge3D *e, zPlane3D *p);
__EXPORT int zIntersectEdgePlane3D(zEdge3D *e, zPlane3D *p, zVec3D ip[]);

/*! \brief collision and intersection of plane, edge and triangle.
 *
 * zColChkTriPlane3D() checks if 3D triangle \a t and 3D
 * plane \a p intersect with each other.
 * zIntersectTriPlane3D() calculates all the intersection
 * points between \a t and \a p, sets them into the array
 * \a ip and returns the number of them.
 * In the case that \a t is perfectly on \a p, though
 * infinite number of intersection points exist, the
 * function only sets the three vertices of \a t into the
 * first three components of \a ip. Consequently, the number
 * of intersection points is at most three, so that the size
 * of \a ip has to be more than or equal to three.
 *
 * zColChkEdgeTri3D() checks if 3D edge \a e and 3D triangle
 * \a t intersect with each other.
 * zIntersectEdgeTri3D() calculates all the intersection
 * points between \a e and \a t and sets them into \a ip.
 * In the case that \a e is included in \a t, though infinite
 * number of intersection points exist, the function only
 * sets the two endpoints of \a e into the first and second
 * components of \a ip. Consequently, the number of
 * intersection points is at most two, so that the size of
 * \a ip has to be more than or equal to two.
 *
 * zColChkTri3D() checks if two 3D triangles \a t1 and \a t2
 * intersect with each other.
 * zIntersectTri3D() calculates all the intersection points
 * between \a t1 and \a t2 and sets them into \a ip.
 * In the case that one is included in the other (or the two
 * perfectly coincide with each other), though infinite
 * number of intersection points exist, the function only
 * sets the three vertices of the included into the first
 * three components of \a ip. Consequently, the number of
 * intersection points is at most three, so that the size of
 * \a ip has to be more than or equal to three.
 * \return
 * zColChkTriPlane3D(), zColChkEdgeTri3D() and zColChkTri3D()
 * return the true value if the given two elements intersect
 * with each other. Otherwise, the false value is returned.
 *
 * zIntersectTriPlane3D(), zIntersectEdgeTri3D() and
 * zIntersectTri3D() return the number of intersection points.
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

#include <zeo/zeo_col_box.h> /* axis-aligned bounding box (AABB) and oriented bounding box (OBB) */
#include <zeo/zeo_col_minkowski.h> /* Minkowski sum */
#include <zeo/zeo_col_gjk.h> /* Gilbert-Johnson-Keerthi algorithm */
#include <zeo/zeo_col_mpr.h> /* Minkowski Portal Refinement algorithm */
#include <zeo/zeo_col_ph.h>  /* polyhedra */

#endif /* __ZEO_COL_H__ */
