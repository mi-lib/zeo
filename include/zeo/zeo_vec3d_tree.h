/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_tree - 3D vector tree (kd-tree with k=3).
 */

#ifndef __ZEO_VEC3D_TREE_H__
#define __ZEO_VEC3D_TREE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \struct zVec3DTree
 * \brief 3D vector tree class
 *
 * zVec3DTree represents a binary tree composed from 3D vectors.
 * It is particularly utilized for the nearest neighbor search.
 * Initialize a tree by zVec3DTreeInit() and then incrementally add 3D vectors by zVec3DTreeAdd().
 * The nearest neighbor to a vector in the tree is found by zVec3DTreeNN().
 * The tree is freed by calling zVec3DTreeDestroy().
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec3DTreeData ){
  int id;          /*!< identifier of a tree node */
  zAxis split;     /*!< split axis index */
  zVec3D point;    /*!< spliting vertex */
  zAABox3D region; /*!< region to cover */
};

__ZEO_EXPORT zVec3DTreeData *zVec3DTreeDataInit(zVec3DTreeData *data);

zTreeClass( __ZEO_EXPORT, zVec3DTree, zVec3DTreeData );

/*! \brief add a new 3D point to a 3D vector tree.
 *
 * zVec3DTreeAddPoint() adds a 3D point \a point to a 3D vector tree \a tree.
 * \a point is copied to a node of \a tree.
 */
__ZEO_EXPORT zVec3DTree *zVec3DTreeAddPoint(zVec3DTree *tree, const zVec3D *point);

/*! \brief find the partition in which a 3D point is contained.
 *
 * zVec3DTreePart() finds a partition in which a 3D point \a point is contained of a 3D vector tree \a tree.
 * This function is mainly for debug.
 * \return a pointer to the node found in the tree is returned.
 */
__ZEO_EXPORT const zVec3DTree *zVec3DTreePart(const zVec3DTree *node, const zVec3D *point);

/*! \brief find the nearest neighbor to a 3D vector in a tree.
 *
 * zVec3DTreeNN() returns the nearest neighbor in a 3D vector tree \a tree to a given 3D vector \a v.
 * The pointer to the node found is stored into \a nn.
 * \return
 * zVec3DTreeNN() returns the distance from \a v to the nearest neighbor.
 */
__ZEO_EXPORT double zVec3DTreeNN(const zVec3DTree *tree, const zVec3D *point, zVec3DTree **nn);

/*! \brief find vicinity of a 3D point in a 3D vector tree.
 *
 * zVec3DTreeVicinity() finds vicinity of a 3D point \a point in a 3D vector tree \a tree.
 * \a radius defines the boundary of the vicinity, which is a sphere with the radius.
 * The result is stored in a list of addresses of 3D vectors \a vicinity.
 * \return
 * zVec3DTreeVicinity() returns the pointer \a vicinity, if it succeeds. If it fails to
 * allocate memory for the list to store the result, it returns the null pointer.
 */
__ZEO_EXPORT zVec3DData *zVec3DTreeVicinity(const zVec3DTree *tree, const zVec3D *point, double radius, zVec3DData *vicinity);

/*! \brief convert an array of 3D vectors to a 3D vector tree. */
__ZEO_EXPORT zVec3DTree *zVec3DArrayToTree(const zVec3DArray *array, zVec3DTree *tree);

/*! \brief convert a 3D vector tree to an array of 3D vectors. */
__ZEO_EXPORT zVec3DArray *zVec3DTreeToArray(const zVec3DTree *tree, zVec3DArray *array);

/*! \brief convert a 3D vector tree to a 3D vector list. */
__ZEO_EXPORT zVec3DList *zVec3DTreeToList(const zVec3DTree *tree, zVec3DList *list);

/*! \brief convert a 3D vector list to a 3D vector tree. */
__ZEO_EXPORT zVec3DTree *zVec3DListToTree(const zVec3DList *list, zVec3DTree *tree);

/*! \brief convert a set of 3D vectors to a 3D vector tree. */
__ZEO_EXPORT zVec3DTree *zVec3DTreeAddData(zVec3DTree *tree, zVec3DData *data);
/*! \brief convert a 3D vector tree to a set of 3D vectors. */
__ZEO_EXPORT zVec3DData *zVec3DTreeToData(const zVec3DTree *tree, zVec3DData *data);

/*! \brief print out a 3D vector tree (for debug). */
__ZEO_EXPORT void zVec3DTreeFPrint(FILE *fp, const zVec3DTree *tree);

/*! \brief print out values of a 3D vector tree. */
__ZEO_EXPORT void zVec3DTreeValueFPrint(FILE *fp, const zVec3DTree *tree);

__END_DECLS

#endif /* __ZEO_VEC3D_TREE_H__ */
