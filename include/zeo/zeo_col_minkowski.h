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
__ZEO_EXPORT zPH3D *zVec3DDataMinkowskiSub(zVec3DData *data1, zVec3DData *data2, zPH3D *ph);

__END_DECLS

#endif /* __ZEO_COL_MINKOWSKI_H__ */
