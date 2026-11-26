/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_gjk - collision checking: Gilbert-Johnson-Keerthi's algorithm.
 */

#ifndef __ZEO_COL_GJK_H__
#define __ZEO_COL_GJK_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief Gilbert-Johnson-Keerthi algorithm for collision detection.
 *
 * zGJK() finds a pair of the closest points of convex hulls of two sets of 3D points \a data1 and
 * \a data2 based on a method proposed by Gilbert et al.:
 *   E. G. Gilbert, D. W. Johnson, and S. S. Keerthi, A Fast Procedure for Computing the Distance
 *   Between Complex Objects in Three-Dimensional Space, IEEE Journal of Robotics and Automation,
 *   4(2):193-203, 1988.
 * The pair of the closest points is stored in \a c1 and \a c2. The distance between them is computed
 * within the error of zTOL times the size of the problem, namely, the maximum norm of the points and
 * of their differences concerned in the computation.
 * \return
 * zGJK() returns the true value if the convex hulls intersect, namely, the distance between them is not
 * larger than the above error. In this case, a point in the intersection is put into \a c1 and \a c2.
 * Otherwise, it returns the false value.
 *
 * zGJKPoint() finds the closest point of the convex hull of a set of 3D points \a data to a 3D point
 * \a p in the same way with zGJK(). The closest point is stored in \a c. The distance between \a p and
 * \a c is computed within the same error with zGJK().
 * \return
 * zGJKPoint() returns the true value if \a p is inside of the convex hull, namely, the distance
 * between them is not larger than the above error. In this case, a point that coincides with \a p
 * within the error is put into \a c. Otherwise, it returns the false value.
 */
__ZEO_EXPORT bool zGJK(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2);
__ZEO_EXPORT bool zGJKPoint(zVec3DData *data, zVec3D *p, zVec3D *c);

/*! \brief penetration depth of two convex hulls.
 *
 * zGJKDepth() finds the penetration depth of the convex hulls of two sets of 3D points \a data1 and
 * \a data2 based on the expanding polytope algorithm proposed by van den Bergen:
 *   G. van den Bergen, Proximity Queries and Penetration Depth Computation on 3D Game Objects, Game
 *   Developers Conference, 2001.
 * If the convex hulls intersect, a pair of points on them, the difference of which is the shortest
 * translation to separate them, is stored in \a c1 and \a c2. Namely, the penetration depth is the
 * distance between \a c1 and \a c2, which is computed within the same error with zGJK(). Otherwise,
 * the pair of the closest points is stored in \a c1 and \a c2 as zGJK().
 * \return
 * zGJKDepth() returns the true value if the convex hulls intersect and the penetration depth is
 * successfully found. Otherwise, it returns the false value.
 */
__ZEO_EXPORT bool zGJKDepth(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2);

/*! \brief check if two convex hulls penetrate each other deeper than a given depth.
 *
 * _zGJKPenetrate() checks if the penetration depth of the convex hulls of two sets of 3D points
 * \a data1 and \a data2 is larger than a non-negative value \a depth. The pair of points found by
 * zGJK() is stored in \a c1 and \a c2.
 * \return
 * _zGJKPenetrate() returns the true value if the penetration depth is larger than \a depth.
 * Otherwise, it returns the false value.
 * \notes
 * This is for internal implementations of the library. No need to call this function in user's codes.
 */
__ZEO_EXPORT bool _zGJKPenetrate(zVec3DData *data1, zVec3DData *data2, double depth, zVec3D *c1, zVec3D *c2);

__END_DECLS

#endif /* __ZEO_COL_GJK_H__ */
