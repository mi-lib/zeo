/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_list_tree - conversion from/to list to/from tree of 3D vectors.
 */

#ifndef __ZEO_VEC3D_LIST_TREE_H__
#define __ZEO_VEC3D_LIST_TREE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief convert a 3D vector tree to a 3D vector list. */
__EXPORT zVec3DList *zVec3DTree2List(zVec3DTree *tree, zVec3DList *list);

/*! \brief convert a 3D vector list to a 3D vector tree. */
__EXPORT zVec3DTree *zVec3DList2Tree(zVec3DList *list, zVec3DTree *tree);

__END_DECLS

#endif /* __ZEO_VEC3D_LIST_TREE_H__ */
