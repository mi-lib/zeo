/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_list - list of 3D vectors.
 */

#ifndef __ZEO_VEC3D_LIST_H__
#define __ZEO_VEC3D_LIST_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zVec3DList
 * list of 3D vectors
 * ********************************************************** */

zListClass( zVec3DList, zVec3DListCell, zVec3D );

/*! \brief allocate a 3D vector list cell and copy a vector.
 *
 * zVec3DListNew() allocates a new 3D vector list cell and copy a 3D vector \a v to it.
 * \return
 * zVec3DListNew() returns a pointer to the newly allocated cell.
 */
__ZEO_EXPORT zVec3DListCell *zVec3DListNew(const zVec3D *v);

/*! \brief add a 3D vector to a vector list.
 *
 * zVec3DListAdd() adds a copy of a 3D vector \a v at the head of a list of vectors \a list.
 * \return
 * zVec3DListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT zVec3DListCell *zVec3DListAdd(zVec3DList *list, const zVec3D *v);

/*! \brief append an array of 3D vectors to a list.
 *
 * zVec3DListAppendArray() appends an array of 3D vectors \a array to a list of 3D vectors \a list.
 * Vectors of \a array are copied to cells of \a list.
 * \return
 * zVec3DListAppendArray() returns \a list if succeeding. Otherwise, the null pointer is returned.
 * \sa zVec3DArray2List
 */
__ZEO_EXPORT zVec3DList *zVec3DListAppendArray(zVec3DList *list, const zVec3DArray *array);

/*! \brief clone an array of 3D vectors.
 *
 * zVec3DListClone() clones an array of 3D vectors \a src and make another list \a dest.
 * \return
 * zVec3DListClone() returns a pointer \a dest if it succeeds.
 * Otherwise, the null pointer is returned.
 */
__ZEO_EXPORT zVec3DList *zVec3DListClone(const zVec3DList *src, zVec3DList *dest);

/*! \brief converts an array of 3D vectors to a list.
 *
 * zVec3DArray2List() converts an array of 3D vectors \a array to a list of 3D vectors \a list.
 * Vectors of \a array are copied to cells of \a list.
 * \return
 * zVec3DArray2List() returns a pointer \a list.
 * \sa zVec3DArrayAppendList
 */
__ZEO_EXPORT zVec3DList *zVec3DArray2List(const zVec3DArray *array, zVec3DList *list);

/*! \brief destroy a 3D vector list.
 *
 * zVec3DListDestroy() destroys a list of vectors \a list, freeing all cells.
 * \return
 * zVec3DListDestroy() returns no value.
 */
#define zVec3DListDestroy(list) zListDestroy( zVec3DListCell, list )

/*! \brief find an identical 3D vector in a list.
 *
 * zVec3DListFind() finds a 3D vector in a list \a list that is identical with the given vector \a v.
 * \return
 * zVec3DListFind() returns a pointer to the 3D vector in \a list, if found. Otherwise, it returns
 * the null pointer.
 */
__ZEO_EXPORT zVec3DListCell *zVec3DListFind(const zVec3DList *list, const zVec3D *v);

/*! \brief a quick sort routine for vector list class.
 *
 * zVec3DListQuickSort() is a quick sort routine for zVec3DList class.
 *
 * The cells of \a list will be sorted in ascending order according to the comparison function \a cmp.
 * (The factor a in \a list is put after another factor b when \a cmp(a,b,p) > 0, where p is for
 * programmer's utility, given by \a priv.)
 * \return
 * zVec3DListQuickSort() returns a pointer \a list.
 * \sa
 * zListQuickSortDef
 */
__ZEO_EXPORT zVec3DList *zVec3DListQuickSort(zVec3DList *list, int (*cmp)(void*,void*,void*), void *priv);

/*! \brief print a list of 3D vectors.
 *
 * zVec3DListFPrint() prints a list of 3D vectors \a list out to the current position of a file \a fp
 * in the following format.
 * n
 *  ( x1, y1, z1 )
 *   ...
 *  ( xn, yn, zn )
 * zVec3DDataPrint() prints \a list out to the standard output.
 * \return
 * zVec3DListFPrint() and zVec3DListPrint() return no value.
 */
__ZEO_EXPORT void zVec3DListFPrint(FILE *fp, const zVec3DList *list);
#define zVec3DListPrint(l) zVec3DListFPrint( stdout, (l) )

/*! \brief print a list of 3D vectors in a plain form.
 *
 * zVec3DListDataFPrint() prints a list of 3D vectors \a list out to the current position of the file \a fp
 * in the following format.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec3DListDataPrint() prints \a list out to the standard output.
 * \return
 * zVec3DListDataFPrint() and zVec3DListDataPrint() return no value.
 */
__ZEO_EXPORT void zVec3DListDataFPrint(FILE *fp, const zVec3DList *list);
#define zVec3DListDataPrint(l) zVec3DListDataFPrint( stdout, (l) )

/* ********************************************************** */
/* CLASS: zVec3DAddrList
 * list of addresses of 3D vectors
 * ********************************************************** */

zListClass( zVec3DAddrList, zVec3DAddrListCell, zVec3D* );

/*! \brief add a pointer to a 3D vector into a list of vectors.
 *
 * zVec3DAddrListAdd() adds a pointer to a 3D vector \a v at the head of a list of vectors \a list.
 * \return
 * zVec3DAddrListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT zVec3DAddrListCell *zVec3DAddrListAdd(zVec3DAddrList *list, const zVec3D *v);

/*! \brief append an array of 3D vectors to a list of pointers to 3D vectors.
 *
 * zVec3DAddrListAppendArray() appends an array of 3D vectors \a array to a list of pointers to
 * 3D vectors \a list.
 * \return
 * zVec3DAddrListAppendArray() returns \a list if succeeding. Otherwise, the null pointer is returned.
 * \sa zVec3DArray2List
 */
__ZEO_EXPORT zVec3DAddrList *zVec3DAddrListAppendArray(zVec3DAddrList *list, const zVec3DArray *array);

/*! \brief create a list of pointers to 3D vectors from an array.
 *
 * zVec3DAddrListCreate() creates a list of pointers to 3D vectors \a list, each cell of which points
 * an element of an array of 3D vectors \a array.
 * \return
 * zVec3DAddrListCreate() returns a pointer \a list.
 */
__ZEO_EXPORT zVec3DAddrList *zVec3DAddrListCreate(zVec3DAddrList *list, const zVec3DArray *array);

/*! \brief clone a list of vectors.
 *
 * zVec3DAddrListClone() clones a list of pointers to 3D vectors \a src and make another list \a dest.
 * \return
 * zVec3DAddrListClone() returns a pointer \a dest if it succeeds.
 * Otherwise, the null pointer is returned.
 */
__ZEO_EXPORT zVec3DAddrList *zVec3DAddrListClone(const zVec3DAddrList *src, zVec3DAddrList *dest);

/*! \brief destroy a list of 3D vectors.
 *
 * zVec3DAddrListDestroy() destroys a list of vectors \a list.
 * \return
 * zVec3DAddrListDestroy() returns no value.
 */
#define zVec3DAddrListDestroy(l) zListDestroy( zVec3DAddrListCell, l )

/*! \brief a quick sort routine for vector list class.
 *
 * zVec3DAddrListQuickSort() is a quick sort routine for zVec3DAddrList class.
 *
 * The cells of \a list will be sorted in ascending order according to the comparison function \a cmp.
 * (The factor a in \a list is put after another factor b when \a cmp(a,b,p) > 0, where p is for
 * programmer's utility, given by \a priv.)
 * \return
 * zVec3DAddrListQuickSort() returns a pointer \a list.
 * \sa
 * zListQuickSortDef
 */
__ZEO_EXPORT zVec3DAddrList *zVec3DAddrListQuickSort(zVec3DAddrList *list, int (*cmp)(void*,void*,void*), void *priv);

/*! \brief print a list of pointers to 3D vectors.
 *
 * zVec3DAddrListFPrint() prints a list of pointers to 3D vectors \a list to the current position
 * of a file \a fp in the following format.
 * n
 *  ( x1, y1, z1 )
 *   ...
 *  ( xn, yn, zn )
 * zVec3DAddrListPrint() prints \a list out to the standard output.
 * \return
 * zVec3DAddrListFPrint() and zVec3DAddrListPrint() return no value.
 */
__ZEO_EXPORT void zVec3DAddrListFPrint(FILE *fp, const zVec3DAddrList *list);
#define zVec3DAddrListPrint(l) zVec3DAddrListFPrint( stdout, (l) )

/*! \brief print a list of pointers to 3D vectors in a plain form.
 *
 * zVec3DAddrListDataFPrint() prints a list of pointers to 3D vectors \a list out to the current
 * position of a file \a fp in the following format.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec3DAddrListDataPrint() prints \a list out to the standard output.
 * \return
 * zVec3DAddrListDataFPrint() and zVec3DAddrListDataPrint() return no value.
 */
__ZEO_EXPORT void zVec3DAddrListDataFPrint(FILE *fp, const zVec3DAddrList *list);
#define zVec3DAddrListDataPrint(l) zVec3DAddrListDataFPrint( stdout, (l) )

/* ********************************************************** */
/* point cloud utilities
 * ********************************************************** */

/*! \brief a naive algorithm to find the nearest neighbor in a list of 3D vectors.
 */
__ZEO_EXPORT double zVec3DListNN(const zVec3DList *list, const zVec3D *v, zVec3D **nn);

/*! \brief support map of a set of points with respect to a direction vector. */
__ZEO_EXPORT const zVec3D *zVec3DSupportMap(const zVec3D p[], int n, zVec3D *v);

/*! \brief support map of a list of points with respect to a direction vector. */
__ZEO_EXPORT const zVec3D *zVec3DListSupportMap(const zVec3DList *pl, zVec3D *v);

__END_DECLS

#endif /* __ZEO_VEC3D_LIST_H__ */
