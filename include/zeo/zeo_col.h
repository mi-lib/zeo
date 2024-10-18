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
 * zColChkEdgePlane3D() checks if an edge \a edge and a plane \a plane intersect with each other.
 *
 * zIntersectEdgePlane3D() calculates the all intersection points between \a edge and \a plane,
 * sets them into the array \a ip and returns the number of them.
 * The following three cases can happen.
 *  1. \a edge and \a plane does not intersect. In this case, \a ip is not treated, and the function
 *     returns 0.
 *  2. \a edge and \a plane intersect at one point. In this case, the point is set into the first
 *     component of \a ip, and the function returns 1.
 *  3. \a edge is perfectly on \a plane. In this case, though infinite number of intersection points
 *     exist, the function only sets the two endpoints of \a edge into the first and second
 *     components of \a ip.
 * Consequently, the number of intersection points is at most two, so that the size of \a ip
 * has to be more than or equal to two.
 * \return
 * zColChkEdgePlane3D() returns the true value if \a edge and \a plane intersect with each other.
 * Otherwise, the false value is returned.
 *
 * zIntersectEdgePlane3D() returns the number of intersection points, 0, 1, or 2.
 */
__ZEO_EXPORT bool zColChkEdgePlane3D(zEdge3D *edge, zPlane3D *plane);
__ZEO_EXPORT int zIntersectEdgePlane3D(zEdge3D *edge, zPlane3D *plane, zVec3D ip[]);

/*! \brief intersection between two 3D planes.
 *
 * zIntersectPlane3D() finds the intersection line of a pair of 3D planes \a plane1 and \a plane2.
 * The result is put into \a line if it exists.
 * \return
 * zIntersectPlane3D() basically returns the number of the intersection lines. In ordinary cases,
 * the line is uniquely determined, and thus, 1 is returned.
 * If the two planes are identical, the intersection is not uniquely determined. In this case,
 * -1 is returned.
 * If the two planes are not identical but parallel, the intersection does not exist. In this case,
 * 0 is returned.
 * In the above two exceptional cases, nothing happens on \a line.
 */
__ZEO_EXPORT int zIntersectPlane3D(zPlane3D *plane1, zPlane3D *plane2, zLine3D *line);

/*! \brief collision and intersection of plane, edge and triangle.
 *
 * zColChkTriPlane3D() checks if 3D triangle \a tri and 3D plane \a plane intersect with each other.
 * zIntersectTriPlane3D() calculates all the intersection points between \a tri and \a plane, sets
 * them into the array \a ip and returns the number of them.
 * In the case that \a tri is perfectly on \a plane, though infinite number of intersection points
 * exist, the function only sets the three vertices of \a tri into the first three components of
 * \a ip. Consequently, the number of intersection points is at most three, so that the size
 * of \a ip has to be more than or equal to three.
 *
 * zColChkEdgeTri3D() checks if 3D edge \a edge and 3D triangle \a tri intersect with each other.
 * zIntersectEdgeTri3D() calculates all the intersection points between \a edge and \a tri and sets
 * them into \a ip.
 * In the case that \a edge is included in \a tri, though infinite number of intersection points
 * exist, the function only sets the two endpoints of \a edge into the first and second components
 * of \a ip. Consequently, the number of intersection points is at most two, so that the size of
 * \a ip has to be more than or equal to two.
 *
 * zColChkTri3D() checks if two 3D triangles \a tri1 and \a tri2 intersect with each other.
 * zIntersectTri3D() calculates all the intersection points between \a tri1 and \a tri2 and sets
 * them into \a ip.
 * In the case that one is included in the other (or the two perfectly coincide with each other),
 * though infinite number of intersection points exist, the function only sets the three vertices
 * of the included into the first three components of \a ip. Consequently, the number of
 * intersection points is at most three, so that the size of \a ip has to be more than or equal
 * to three.
 * \return
 * zColChkTriPlane3D(), zColChkEdgeTri3D() and zColChkTri3D() return the true value if the given
 * two elements intersect with each other. Otherwise, the false value is returned.
 *
 * zIntersectTriPlane3D(), zIntersectEdgeTri3D() and zIntersectTri3D() return the number of
 * intersection points.
 */
__ZEO_EXPORT bool zColChkTriPlane3D(zTri3D *tri, zPlane3D *plane);
__ZEO_EXPORT int zIntersectTriPlane3D(zTri3D *tri, zPlane3D *plane, zVec3D ip[]);
__ZEO_EXPORT bool zColChkEdgeTri3D(zEdge3D *edge, zTri3D *tri);
__ZEO_EXPORT int zIntersectEdgeTri3D(zEdge3D *edge, zTri3D *tri, zVec3D ip[]);
__ZEO_EXPORT bool zColChkTri3D(zTri3D *tri1, zTri3D *tri2);
__ZEO_EXPORT int zIntersectTri3D(zTri3D *tri1, zTri3D *tri2, zVec3D ip[]);

/*! \brief intersection between a loop of vertices and 3D plane.
 *
 * zIntersectVecListPlane3D() computes all intersection points between a loop of vertices given
 * by \a vl and a 3D plane \a plane.
 * \a ip is an array of 3D vectors to store the intersection points.
 * \retval the number of intersection points.
 * \notes the size of \a ip has to be larger than the valued returned by the function.
 */
__ZEO_EXPORT int zIntersectVecListPlane3D(zVec3DList *vl, zPlane3D *plane, zVec3D ip[]);

__END_DECLS

#include <zeo/zeo_col_box.h> /* axis-aligned bounding box (AABB) and oriented bounding box (OBB) */
#include <zeo/zeo_col_minkowski.h> /* Minkowski sum */
#include <zeo/zeo_col_gjk.h> /* Gilbert-Johnson-Keerthi algorithm */
#include <zeo/zeo_col_mpr.h> /* Minkowski Portal Refinement algorithm */
#include <zeo/zeo_col_ph.h>  /* polyhedra */

#endif /* __ZEO_COL_H__ */
