/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_boundingdisk - 2D bounding volume: bounding disk.
 */

#ifndef __ZEO_BV2D_BOUNDINGDISK_H__
#define __ZEO_BV2D_BOUNDINGDISK_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief bounding disk of 2D points.
 *
 * zBoundingDisk2D() computes the bounding disk (or, smallest enclosing disk)
 * of an array of 2D points \a pa. The result is put int \a bd.
 *
 * zBoundingDisk2DPL() also computes the bounding disk. For this function,
 * the set of points is given as a vector list \a p.
 *
 * The pointers to points on the outer rim circle will be stored
 * into the array pointed by \a vp, unless \a vp is the null pointer.
 *
 * The algorithm is according to E. Welzl(1991).
 * \return
 * zBoundingDisk2D() and zBoundingDisk2DPL() return the number of points on the
 * outer rim circle of \a bd.
 */
__ZEO_EXPORT int zBoundingDisk2D(zDisk2D *bd, zVec2DArray *pa, zVec2D **vp);
__ZEO_EXPORT int zBoundingDisk2DPL(zDisk2D *bd, zVec2DList *pl, zVec2D **vp);

__END_DECLS

#endif /* __ZEO_BV2D_BOUNDINGDISK_H__ */
