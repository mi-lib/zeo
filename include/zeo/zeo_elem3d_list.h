/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_list - 3D shape element list.
 */

#ifndef __ZEO_ELEM3D_LIST_H__
#define __ZEO_ELEM3D_LIST_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief loop of 3D vertices.
 *//* ******************************************************* */

typedef zVec3DAddrListCell zLoop3DCell;
typedef zVec3DAddrList zLoop3D;

#define zLoop3DAdd(l,c)    zVec3DAddrListAdd( l, c )
#define zLoop3DCreate(l,a) zVec3DAddrListCreate( l, a )
#define zLoop3DDestroy(l)  zVec3DAddrListDestroy( l )

/* ********************************************************** */
/*! \brief list of 3D triangles.
 *//* ******************************************************* */

zListClass( zTri3DList, zTri3DListCell, zTri3D );

/*! add a 3D triangle to a list of 3D triangles.
 *
 * zTri3DListAdd() adds a copy of a 3D triangle \a t at the head
 * of a list of 3D triangles \a list.
 * \return
 * zTri3DListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT zTri3DListCell *zTri3DListAdd(zTri3DList *list, const zTri3D *t);

/*! destroy a list of 3D triangles.
 *
 * zTri3DListDestroy() destroys a list of 3D triangles \a list.
 * \return
 * zTri3DListDestroy() returns no value.
 */
#define zTri3DListDestroy(l) zListDestroy( zTri3DListCell, l )

/*! \brief align triangles to a direction referred by a vector.
 *
 * zTri3DListAlign() aligns directions of all triangles contained in a
 * list of triangles \a list to one referred by a vector \a ref. Namely,
 * if the direction of a triangle in \a list is opposite to \a ref, the
 * triangle is flipped.
 * \return
 * zTri3DListAlign() returns no value.
 */
__ZEO_EXPORT void zTri3DListAlign(zTri3DList *list, const zVec3D *ref);

/*! \brief copy triangles of a list to an array.
 *
 * zTri3DListCopyArray() copies triangles held by a list \a list to an
 * array \a t. \a n is the size of the array. If the size of \a list is
 * larger than \a n, the leftovers are omitted.
 * \return
 * zTri3DListCopyArray() returns no value.
 */
__ZEO_EXPORT void zTri3DListCopyArray(const zTri3DList *list, zTri3D t[], int n);

/* ********************************************************** *
 * trianglation of non-convex.
 * ********************************************************** */

/*! \brief triangulate a simple loop of vertices. */
__ZEO_EXPORT int zLoop3DTriangulate(const zLoop3D *loop, zTri3DList *tlist);

/*! \brief trianglate a looped series of vertices of a non-convex.
 *
 * zTriangulate() triangulates a looped series of vertices of a non-convex
 * given by an array of 3D vectors \a array. Namely, it divides the non-convex
 * into triangle pieces.
 * As the result, a triangle list \a tlist is newly created.
 * \return
 * zTriangulate() returns the number of the generated triangles.
 */
__ZEO_EXPORT int zTriangulate(const zVec3DArray *array, zTri3DList *tlist);

__END_DECLS

#endif /* __ZEO_ELEM3D_LIST_H__ */
