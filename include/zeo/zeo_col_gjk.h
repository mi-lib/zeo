/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_gjk - collision checking: Gilbert-Johnson-Keerthi's algorithm.
 */

#ifndef __ZEO_COL_GJK_H__
#define __ZEO_COL_GJK_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief Gilbert-Johnson-Keerthi algorithm.
 *
 * zGJK() finds a pair of the closest points of convex hulls of two sets
 * of points \a p1 and \a p2. \a n1 and \a n2 are the numbers of points of
 * \a p1 and \a p2, respectively. The pair of points found are stored in
 * \a c1 and \a c2.
 *
 * zGJKPL() also finds a pair of the closest points of convex hulls of
 * two lists of points \a pl1 and \a pl2.
 *
 * \return
 * zGJKPL() and zGJK() return the distance between the closest points.
 * If the convex hulls intersect, zero is returned as the distance, and
 * the center of intersection is put into \a c1 and \a c2.
 * \note
 * zGJK() does not work correctly when the given points have very large
 * component values such as 1.0e10, and zGJKPL() likewise.
 */
__EXPORT bool zGJK(zVec3D p1[], int n1, zVec3D p2[], int n2, zVec3D *c1, zVec3D *c2);
__EXPORT bool zGJKDepth(zVec3D p1[], int n1, zVec3D p2[], int n2, zVec3D *c1, zVec3D *c2);
__EXPORT bool zGJKPL(zVec3DList *pl1, zVec3DList *pl2, zVec3D *ca, zVec3D *cb);

__EXPORT bool zGJKPoint(zVec3D pl[], int n, zVec3D *p, zVec3D *c);

__END_DECLS

#endif /* __ZEO_COL_GJK_H__ */
