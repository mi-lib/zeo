/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec_tree - 3D vector tree.
 */

#ifndef __ZEO_VEC3D_TREE_H__
#define __ZEO_VEC3D_TREE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \struct zVecTree3D
 * \brief 3D vector tree class
 *
 * zVecTree3D represents a binary tree composed from 3D vectors.
 * It is particularly utilized for the nearest neighbor search.
 * Initialize a tree by zVecTree3DInit() and then incrementally
 * add 3D vectors by zVecTree3DAdd().
 * The nearest neighbor to a vector in the tree is found by
 * zVecTree3DNN().
 * The tree is freed by calling zVecTree3DDestroy().
 *//* ******************************************************* */
typedef struct _zVecTree3D{
  zAxis split;   /*!< split axis index */
  zVec3D v;      /*!< spliting vertex */
  zVec3D vmin;   /*!< minimum corner of bounding box */
  zVec3D vmax;   /*!< maximum corner of bounding box */
  struct _zVecTree3D *s[2]; /*!< binary branches */
} zVecTree3D;

/*! \brief initialize a 3D vector tree.
 *
 * zVecTree3DInit() initializes a 3D vector tree \a tree.
 * The bounding box is set at default for infinite volume.
 * \return a pointer \a tree is returned.
 */
__EXPORT zVecTree3D *zVecTree3DInit(zVecTree3D *tree);

/*! \brief destroy a 3D vector tree.
 *
 * zVecTree3DDestroy() destroys a 3D vector tree \a tree.
 * All nodes and leaves are freed.
 */
__EXPORT void zVecTree3DDestroy(zVecTree3D *tree);

/*! \brief add a new 3D vector to a tree.
 *
 * zVecTree3DAdd() adds a newly given 3D vector \a v to a tree
 * \a tree.
 */
__EXPORT zVecTree3D *zVecTree3DAdd(zVecTree3D *tree, zVec3D *v);

/*! \brief find the partition in which a 3D vector is contained.
 *
 * zVecTree3DPart() finds the partition in which a 3D vector \a v
 * is contained of a 3D vector tree \a tree.
 * This function is mainly for debug.
 * \return a pointer to the node found in the tree is returned.
 */
__EXPORT zVecTree3D *zVecTree3DPart(zVecTree3D *tree, zVec3D *v);

/*! \brief find the nearest neighbor to a 3D vector in a tree.
 *
 * zVecTree3DNN() returns the nearest neighbor in a tree
 * \a tree to a given 3D vector \a v. The pointer to the
 * node found is stored into \a nn.
 * \return the distance from \a v to the nearest neighbor.
 */
__EXPORT double zVecTree3DNN(zVecTree3D *tree, zVec3D *v, zVecTree3D **nn);

__END_DECLS

#endif /* __ZEO_VEC3D_TREE_H__ */
