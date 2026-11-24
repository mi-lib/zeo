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
 */
__ZEO_EXPORT bool zGJK(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2);
__ZEO_EXPORT bool zGJKPoint(zVec3DData *data, zVec3D *p, zVec3D *c);

__ZEO_EXPORT bool zGJKDepth(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2);

__END_DECLS

#endif /* __ZEO_COL_GJK_H__ */
