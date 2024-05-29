/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_convexhull - 3D bounding volume: 3D convex hull.
 */

#ifndef __ZEO_BV3D_CONVEXHULL_H__
#define __ZEO_BV3D_CONVEXHULL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* 3D convex hull
 * ********************************************************** */

/*! \brief convex hull of points.
 *
 * zConvexHull3D() computes convex hull of a set of points \a p. \a num
 * is the number of points. The result is put into \a ch.
 *
 * zConvexHull3DPL() also computes convex hull. For this function, a set
 * of points is given as a vector list \a pl.
 *
 * The algorithm is according to quickhull by C. Barber,
 * D. Dobkin and H. Huhdanpaa(1996).
 * \notes
 * In the cource of computation, \a pl is partially destroyed
 * but not freed by zConvexHull3DPL().
 * \return
 * zConvexHull3DPL() and zConvexHull3D() return a pointer \a ch if succeeding
 * to compute the convex hull. If failing to allocate working
 * memory necessitated in computation, the null pointer is
 * returned.
 */
__ZEO_EXPORT zPH3D *zConvexHull3D(zPH3D *ch, zVec3D p[], int num);
__ZEO_EXPORT zPH3D *zConvexHull3DPL(zPH3D *ch, zVec3DList *pl);

__END_DECLS

#endif /* __ZEO_BV3D_CONVEXHULL_H__ */
