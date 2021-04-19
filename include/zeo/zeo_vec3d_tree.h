/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_tree - 3D vector tree.
 */

#ifndef __ZEO_VEC3D_TREE_H__
#define __ZEO_VEC3D_TREE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \struct zVec3DTree
 * \brief 3D vector tree class
 *
 * zVec3DTree represents a binary tree composed from 3D vectors.
 * It is particularly utilized for the nearest neighbor search.
 * Initialize a tree by zVec3DTreeInit() and then incrementally
 * add 3D vectors by zVec3DTreeAdd().
 * The nearest neighbor to a vector in the tree is found by
 * zVec3DTreeNN().
 * The tree is freed by calling zVec3DTreeDestroy().
 *//* ******************************************************* */
typedef struct _zVec3DTree{
  int size;      /*!< size of tree */
  int id;        /*!< identifier of a tree node */
  zAxis split;   /*!< split axis index */
  zVec3D v;      /*!< spliting vertex */
  zVec3D vmin;   /*!< minimum corner of bounding box */
  zVec3D vmax;   /*!< maximum corner of bounding box */
  struct _zVec3DTree *s[2]; /*!< binary branches */
} zVec3DTree;

/*! \brief initialize a 3D vector tree.
 *
 * zVec3DTreeInit() initializes a 3D vector tree \a tree.
 * The bounding box is set at default for infinite volume.
 * \return a pointer \a tree is returned.
 */
__EXPORT zVec3DTree *zVec3DTreeInit(zVec3DTree *tree);

/*! \brief destroy a 3D vector tree.
 *
 * zVec3DTreeDestroy() destroys a 3D vector tree \a tree.
 * All nodes and leaves are freed.
 */
__EXPORT void zVec3DTreeDestroy(zVec3DTree *tree);

/*! \brief add a new 3D vector to a tree.
 *
 * zVec3DTreeAdd() adds a 3D vector \a v to a tree \a tree.
 * \a v is copied to a node of \a tree.
 */
__EXPORT zVec3DTree *zVec3DTreeAdd(zVec3DTree *tree, zVec3D *v);

/*! \brief add a new 3D vector to a tree with an identifier. */
__EXPORT zVec3DTree *zVec3DTreeAddID(zVec3DTree *tree, zVec3D *v, int id);

/*! \brief find the partition in which a 3D vector is contained.
 *
 * zVec3DTreePart() finds the partition in which a 3D vector \a v
 * is contained of a 3D vector tree \a tree.
 * This function is mainly for debug.
 * \return a pointer to the node found in the tree is returned.
 */
__EXPORT zVec3DTree *zVec3DTreePart(zVec3DTree *tree, zVec3D *v);

/*! \brief find the nearest neighbor to a 3D vector in a tree.
 *
 * zVec3DTreeNN() returns the nearest neighbor in a tree
 * \a tree to a given 3D vector \a v. The pointer to the
 * node found is stored into \a nn.
 * \return the distance from \a v to the nearest neighbor.
 */
__EXPORT double zVec3DTreeNN(zVec3DTree *tree, zVec3D *v, zVec3DTree **nn);

/*! \brief convert an array of 3D vectors to a 3D vector tree. */
__EXPORT zVec3DTree *zVec3DArray2Tree(zVec3DArray *array, zVec3DTree *tree);

/*! \brief convert a 3D vector tree to an array of 3D vectors. */
__EXPORT zVec3DArray *zVec3DTree2Array(zVec3DTree *tree, zVec3DArray *array);

/*! \brief convert a 3D vector tree to a 3D vector list. */
__EXPORT zVec3DList *zVec3DTree2List(zVec3DTree *tree, zVec3DList *list);

/*! \brief convert a 3D vector list to a 3D vector tree. */
__EXPORT zVec3DTree *zVec3DList2Tree(zVec3DList *list, zVec3DTree *tree);

/*! \brief print out a 3D vector tree (for debug). */
__EXPORT void zVec3DTreeFPrint(FILE *fp, zVec3DTree *tree);

__END_DECLS

#endif /* __ZEO_VEC3D_TREE_H__ */
