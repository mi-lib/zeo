/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_minkowski - collision checking: Minkowski sum
 */

#ifndef __ZEO_COL_MINKOWSKI_H__
#define __ZEO_COL_MINKOWSKI_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief Minkowski subtract.
 */
__EXPORT zPH3D *zPH3DMinkowskiSub(zPH3D *ph, zVec3D p1[], int n1, zVec3D p2[], int n2);

__END_DECLS

#endif /* __ZEO_COL_MINKOWSKI_H__ */
