/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_mpr - collision checking: Minkowski Portal Refinement algorithm.
 */

#ifndef __ZEO_COL_MPR_H__
#define __ZEO_COL_MPR_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief Minkowski Portal Refinement algorithm.
 *
 * zMPR() checks if two convex objects are in collision.
 * Each object is represented as the convex hull of a set of
 * points \a p1, and \a p2. \a n1 and \a n2 are the numbers
 * of points of each set, respectively.
 *
 * zMPRDepth() checks if two convex objects are incollision,
 * and if yes, computes the penetration depth of the objects.
 * Each object is represented as the convex hull of a set of
 * points \a p1, and \a p2. \a n1 and \a n2 are the numbers
 * of points of each set, respectively.
 * The penetration depth, the point of collision and penetration
 * direction are stored in \a depth, \a pos and \a dir,
 * respectively.
 * \return
 * zMPR() and zMPRDepth() returns the true value if the two
 * specified objects are in collisiond. Otherwise, the false
 * value is returned.
 */
__EXPORT bool zMPR(zVec3D p1[], int n1, zVec3D p2[], int n2);
__EXPORT bool zMPRDepth(zVec3D p1[], int n1, zVec3D p2[], int n2, double *depth, zVec3D *pos, zVec3D *dir);

__END_DECLS

#endif /* __ZEO_COL_MPR_H__ */
