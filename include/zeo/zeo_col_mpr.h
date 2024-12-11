/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_mpr - collision checking: Minkowski Portal Refinement algorithm.
 */

#ifndef __ZEO_COL_MPR_H__
#define __ZEO_COL_MPR_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief Minkowski Portal Refinement algorithm for collision detection.
 *
 * zMPR() checks if two convex objects are in collision. Each object is represented as the
 * convex hull of a set of 3D points \a data1 and \a data2.
 *
 * zMPRDepth() checks if the two convex objects are in collision, and if yes, computes the
 * penetration depth of the objects. Each object is represented as the convex hull of a set of
 * 3D points \a data1 and \a data2.
 * The penetration depth, the point of collision, and the penetration direction are stored
 * in \a depth, \a pos and \a dir, respectively.
 * \return
 * zMPR() and zMPRDepth() return the true value if the two objects are in collisiond.
 * Otherwise, the false value is returned.
 */
__ZEO_EXPORT bool zMPR(zVec3DData *data1, zVec3DData *data2);
__ZEO_EXPORT bool zMPRDepth(zVec3DData *data1, zVec3DData *data2, double *depth, zVec3D *pos, zVec3D *dir);

__END_DECLS

#endif /* __ZEO_COL_MPR_H__ */
