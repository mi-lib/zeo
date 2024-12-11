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
 * zVec2DDataBoundingDisk() computes the bounding disk (or, smallest enclosing disk)
 * of a set of 2D points \a data. The result is put int \a bd.
 * The extreme points on the outer rim circle will be stored into an array pointed
 * by \a vp, unless \a vp is the null pointer.
 *
 * The algorithm is according to E. Welzl(1991).
 * \return
 * zVec2DDataBoundingDisk() returns the number of extreme points on the outer rim
 * circle of \a bd.
 */
__ZEO_EXPORT int zVec2DDataBoundingDisk(zVec2DData *data, zDisk2D *bd, zVec2D **vp);

__END_DECLS

#endif /* __ZEO_BV2D_BOUNDINGDISK_H__ */
