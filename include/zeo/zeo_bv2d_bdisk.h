/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_bdisk - 2D bounding volume: bounding disk.
 */

#ifndef __ZEO_BV2D_BDISK_H__
#define __ZEO_BV2D_BDISK_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief bounding disk of 2D points.
 *
 * zBDisk() computes the bounding disk (or, smallest enclosing disk)
 * of a set of 2D points \a p. \a num is the number of points.
 * The result is put int \a bd.
 *
 * zBDiskPL() also computes the bounding disk. For this function,
 * the set of points is given as a vector list \a p.
 *
 * The pointers to points on the outer rim circle will be stored
 * into the array pointed by \a vp, unless \a vp is the null pointer.
 *
 * The algorithm is according to E. Welzl(1991).
 * \return
 * zBDisk() and zBDiskPL() return the number of points on the
 * outer rim circle of \a bd.
 */
__EXPORT int zBDiskPL(zDisk2D *bd, zVec2DList *pl, zVec2D **vp);
__EXPORT int zBDisk(zDisk2D *bd, zVec2D p[], int num, zVec2D **vp);

__END_DECLS

#endif /* __ZEO_BV2D_BDISK_H__ */