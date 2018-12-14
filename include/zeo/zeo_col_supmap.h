/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_supmap - collision checking: support map.
 */

#ifndef __ZEO_COL_SUPMAP_H__
#define __ZEO_COL_SUPMAP_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* support map
 * ********************************************************** */

/* METHOD:
 * zSupportMapPL, zSupportMap
 * - support map.
 * [SYNOPSIS]
 * zVec3D *zSupportMap(zVec3D p[], int n, zVec3D *d, zVec3D *sp);
 * zVec3D *zSupportMapPL(zVec3DList *pl, zVec3D *d, zVec3D *sp);
 * [DESCRIPTION]
 * 'zSupportMap()' computes a support map of a set
 * of points 'p' along with a direction vector 'd'.
 * 'n' is the number of points of 'p'.
 * The support point mapped is put into 'sp'.
 * #
 * 'zSupportMapPL()' also computes a support map,
 * where the set of points is given by a list 'pl'.
 * [RETURN VALUE]
 * 'zSupportMap()' and 'zSupportMapPL()' return
 * a pointer 'sp'.
 */
__EXPORT zVec3D *zSupportMap(zVec3D p[], int n, zVec3D *d, zVec3D *sp);
__EXPORT zVec3D *zSupportMapPL(zVec3DList *pl, zVec3D *d, zVec3D *sp);

__END_DECLS

#endif /* __ZEO_COL_SUPMAP_H__ */
