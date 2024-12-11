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
 * zGJK() finds a pair of the closest points of convex hulls of two sets of 3D points \a data1 and \a data2.
 * The pair of the closest points is stored in \a c1 and \a c2.
 * \return
 * zGJK() returns the distance between the closest points \a c1 and \a c2.
 * If the convex hulls intersect, zero is returned as the distance, and the center of intersection
 * is put into \a c1 and \a c2.
 * \note
 * zGJK() does not work correctly when the points have very large component values such as 1.0e10.
 */
__ZEO_EXPORT bool zGJK(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2);
__ZEO_EXPORT bool zGJKDepth(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2);
__ZEO_EXPORT bool zGJKPoint(zVec3DData *data, zVec3D *p, zVec3D *c);

__END_DECLS

#endif /* __ZEO_COL_GJK_H__ */
