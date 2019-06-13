/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_aabbtree - collision checking: AABB tree.
 */

#ifndef __ZEO_COL_AABBTREE_H__
#define __ZEO_COL_AABBTREE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief AABB (axis-aligned bounding box) tree class.
 *//* ******************************************************* */
typedef struct __zAABoxTree3D{
  zAABox3D box; /*!< a axis-aligned bounding box */
  struct __zAABoxTree3D *bp[2]; /*!< pointers to two branches */
} zAABoxTree3D;

/*! \brief initialize an AABB tree. */
__EXPORT void zAABoxTree3DInit(zAABoxTree3D *node);

/*! \brief destroy an AABB tree. */
__EXPORT void zAABoxTree3DDestroy(zAABoxTree3D *tree);

/*! \brief add an axis-aligned box to an AABB tree. */
__EXPORT bool zAABoxTree3DAdd(zAABoxTree3D *tree, zAABox3D *box);

/*! \brief print an AABB tree out to a file. */
__EXPORT void zAABoxTree3DFPrint(FILE *fp, zAABoxTree3D *tree);

__END_DECLS

#endif /* __ZEO_COL_AABBTREE_H__ */
