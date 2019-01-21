/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec_list - list of 3D vectors.
 */

#ifndef __ZEO_VEC3D_LIST_H__
#define __ZEO_VEC3D_LIST_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zVec3DList
 * list of 3D vectors
 * ********************************************************** */

zListClass( zVec3DList, zVec3DListCell, zVec3D* );

/*! \brief insert a 3D vector to a vector list.
 *
 * zVec3DListInsert() inserts a 3D vector \a v at the head of a list of
 * vectors \a list. Each inserted cell will have a copy of \a v.
 * \return
 * zVec3DListInsert() returns a pointer to the newly inserted cell.
 */
__EXPORT zVec3DListCell *zVec3DListInsert(zVec3DList *list, zVec3D *v);

/*! \brief create a list of vectors from an array of 3D vectors.
 *
 * zVec3DListFromArray() creates a list of vectors \a list from an array
 * of 3D vectors \a varr. \a num is the number of vectors in \a varr.
 * Each cell inserted will have a copy of each vector of \a varr.
 * \return
 * zVec3DListFromArray() returns a pointer \a list.
 */
__EXPORT zVec3DList *zVec3DListFromArray(zVec3DList *list, zVec3D varr[], int num);

/*! \brief destroy a 3D vector list.
 *
 * zVec3DListDestroy() destroys a list of vectors \a list, freeing all cells.
 * \return
 * zVec3DListDestroy() returns no value.
 */
__EXPORT void zVec3DListDestroy(zVec3DList *list);

/*! \brief a quick sort routine for vector list class.
 *
 * zVec3DListQuickSort() is a quick sort routine for zVec3DList class.
 *
 * The cells of \a list will be sorted in ascending order according to
 * the comparison function \a cmp.
 * (The factor a in \a list is put after another factor b when
 * \a cmp(a,b,p) > 0, where p is for programmer's utility, given
 * by \a priv.)
 * \return
 * zVec3DListQuickSort() returns a pointer \a list.
 * \sa
 * zListQuickSortDef
 */
__EXPORT zVec3DList *zVec3DListQuickSort(zVec3DList *list, int (*cmp)(void*,void*,void*), void *priv);

/*! \brief output a list of 3D vectors.
 *
 * zVec3DListFWrite() writes a list of 3D vectors \a list to the
 * current position of a file \a fp in the following style.
 * n
 *  ( x1, y1, z1 )
 *   ...
 *  ( xn, yn, zn )
 * zVec3DDataWrite() outputs \a list to the standard output.
 * \return
 * zVec3DListFWrite() and zVec3DListWrite() return no value.
 */
__EXPORT void zVec3DListFWrite(FILE *fp, zVec3DList *list);
#define zVec3DListWrite(l) zVec3DListFWrite( stdout, (l) )

/*! \brief output a list of 3D vectors in a plain form.
 *
 * zVec3DListDataFWrite() writes a list of 3D vectors \a list to the
 * current position of the file \a fp in the following style.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec3DListDataWrite() outputs \a list to the standard output.
 * \return
 * zVec3DListDataFWrite() and zVec3DListDataWrite() return no value.
 */
__EXPORT void zVec3DListDataFWrite(FILE *fp, zVec3DList *list);
#define zVec3DListDataWrite(l) zVec3DListDataFWrite( stdout, (l) )

/* ********************************************************** */
/* CLASS: zVec3DAddrList
 * list of addresses of 3D vectors
 * ********************************************************** */

typedef zVec3DList zVec3DAddrList;
typedef zVec3DListCell zVec3DAddr;

/*! \brief insert a pointer to a 3D vector into a list of vectors.
 *
 * zVec3DAddrListInsert() inserts a pointer to a 3D vector \a v at the
 * head of a list of vectors \a list. Each cell inserted will have a
 * pointer to \a v.
 * \return
 * zVec3DAddrListInsert() returns a pointer to the newly inserted cell.
 */
__EXPORT zVec3DAddr *zVec3DAddrListInsert(zVec3DAddrList *list, zVec3D *v);

/*! \brief create a list of vectors from an array of 3D vectors.
 *
 * zVec3DAddrListFromArray() creates a list of vectors \a list from an
 * array of 3D vectors \a varr. \a num is the number of vectors in
 * \a varr. Each cell inserted will have a pointer to each vector of
 * \a varr.
 * \return
 * zVec3DAddrListFromArray() returns a pointer \a list.
 */
__EXPORT zVec3DAddrList *zVec3DAddrListFromArray(zVec3DAddrList *list, zVec3D varr[], int num);

/*! \brief destroy a list of 3D vectors.
 *
 * zVec3DAddrListDestroy() destroys a list of vectors \a list.
 * \return
 * zVec3DAddrListDestroy() returns no value.
 */
#define zVec3DAddrListDestroy(l) zListDestroy( zVec3DAddr, l )

/*! \brief a quick sort routine for vector list class.
 *
 * zVec3DAddrListQuickSort() is a quick sort routine for zVec3DAddrList class.
 *
 * The cells of \a list will be sorted in ascending order according to
 * the comparison function \a cmp.
 * (The factor a in \a list is put after another factor b when
 * \a cmp(a,b,p) > 0, where p is for programmer's utility, given by \a priv.)
 * \return
 * zVec3DAddrListQuickSort() returns a pointer \a list.
 * \sa
 * zListQuickSortDef
 */
__EXPORT zVec3DAddrList *zVec3DAddrListQuickSort(zVec3DAddrList *list, int (*cmp)(void*,void*,void*), void *priv);

/*! \brief output a list of pointers to 3D vectors.
 *
 * zVec3DAddrListFWrite() writes a list of pointers to 3D vectors \a list
 * to the current position of a file \a fp in the following style.
 * n
 *  ( x1, y1, z1 )
 *   ...
 *  ( xn, yn, zn )
 * zVec3DAddrListWrite() outputs \a list to the standard output.
 * \return
 * zVec3DAddrListFWrite() and zVec3DAddrListWrite() return no value.
 */
__EXPORT void zVec3DAddrListFWrite(FILE *fp, zVec3DAddrList *list);
#define zVec3DAddrListWrite(l) zVec3DAddrListFWrite( stdout, (l) )

/*! \brief output a list of pointers to 3D vectors in a plain form.
 *
 * zVec3DAddrListDataFWrite() writes a list of pointers to 3D vectors
 * \a list to the current position of the file \a fp in the following
 * style.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec3DAddrListDataWrite() outputs \a list to the standard output.
 * \return
 * zVec3DAddrListDataFWrite() and zVec3DAddrListDataWrite() return no value.
 */
__EXPORT void zVec3DAddrListDataFWrite(FILE *fp, zVec3DAddrList *list);
#define zVec3DAddrListDataWrite(l) zVec3DAddrListDataFWrite( stdout, (l) )

/* ********************************************************** */
/* point cloud utilities
 * ********************************************************** */

/*! \brief a naive algorithm to find the nearest neighbor in a list of 3D vectors.
 */
__EXPORT zVec3D *zVec3DListNN(zVec3DList *list, zVec3D *v, double *dmin);

/*! \brief support map of a set of points with respect to a direction vector. */
__EXPORT zVec3D *zVec3DSupportMap(zVec3D p[], int n, zVec3D *v);

/*! \brief support map of a list of points with respect to a direction vector. */
__EXPORT zVec3D *zVec3DListSupportMap(zVec3DList *pl, zVec3D *v);

__END_DECLS

#endif /* __ZEO_VEC3D_LIST_H__ */
