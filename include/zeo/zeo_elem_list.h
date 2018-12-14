/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem_list - 3D shape element list.
 */

#ifndef __ZEO_ELEM_LIST_H__
#define __ZEO_ELEM_LIST_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zLoop3D
 * loop of 3D vertices
 * ********************************************************** */

typedef zVec3DAddr zLoop3DCell;
typedef zVec3DAddrList zLoop3D;

#define zLoop3DInsert(l,c)       zVec3DAddrListInsert( l, c )
#define zLoop3DFromArray(l,v,n)  zVec3DAddrListFromArray( l, v, n )
#define zLoop3DDestroy(l)        zVec3DAddrListDestroy( l )

/* ********************************************************** */
/* CLASS: zTri3DList
 * 3D triangle list
 * ********************************************************** */

zListClass( zTri3DList, zTri3DListCell, zTri3D );

/*! insert a 3D triangle to a list of 3D triangles.
 *
 * zTri3DListInsert() inserts a new 3D triangle list cell \a t at
 * the head of a triangle list \a list.
 * Each cell inserted will have a copy of \a t.
 * \return
 * zTri3DListInsert() returns a pointer to the newly inserted cell.
 */
__EXPORT zTri3DListCell *zTri3DListInsert(zTri3DList *list, zTri3D *t);

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
__EXPORT void zTri3DListAlign(zTri3DList *list, zVec3D *ref);

/*! \brief copy triangles of a list to an array.
 *
 * zTri3DListCopyArray() copies triangles held by a list \a list to an
 * array \a t. \a n is the size of the array. If the size of \a list is
 * larger than \a n, the leftovers are omitted.
 * \return
 * zTri3DListCopyArray() returns no value.
 */
__EXPORT void zTri3DListCopyArray(zTri3DList *list, zTri3D t[], int n);

__END_DECLS

#endif /* __ZEO_ELEM_LIST_H__ */
