/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec2d_list - list of 2D vectors.
 */

#ifndef __ZEO_VEC2D_LIST_H__
#define __ZEO_VEC2D_LIST_H__

#include <zeo/zeo_vecxd_list.h>

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zVec2DList
 * list of 2D vectors
 * ********************************************************** */

ZEO_VECXD_LIST_DEF_STRUCT( 2D );

/*! \brief allocate a 2D vector list cell and copy a vector.
 *
 * zVec2DListNew() allocates a new 2D vector list cell and copy a 2D vector \a v to it.
 * \return
 * zVec2DListNew() returns a pointer to the newly allocated cell.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_NEW_PROTOTYPE( 2D );

/*! \brief add a 2D vector to a vector list.
 *
 * zVec2DListAdd() adds a copy of a 2D vector \a v at the head of a list of vectors \a list.
 * \return
 * zVec2DListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_ADD_PROTOTYPE( 2D );

/*! \brief append an array of 2D vectors to a list.
 *
 * zVec2DListAppendArray() appends an array of 2D vectors \a array to a list of 2D vectors \a list.
 * Vectors of \a array are copied to cells of \a list.
 * \return
 * zVec2DListAppendArray() returns \a list if succeeding. Otherwise, the null pointer is returned.
 * \sa zVec2DArray2List
 */
__ZEO_EXPORT ZEO_VECXD_LIST_APPEND_ARRAY_PROTOTYPE( 2D );

/*! \brief clone an array of 2D vectors.
 *
 * zVec2DListClone() clones an array of 2D vectors \a src and make another list \a dest.
 * \return
 * zVec2DListClone() returns a pointer \a dest if it succeeds.
 * Otherwise, the null pointer is returned.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_CLONE_PROTOTYPE( 2D );

/*! \brief converts an array of 2D vectors to a list.
 *
 * zVec2DArrayToList() converts an array of 2D vectors \a array to a list of 2D vectors \a list.
 * Vectors of \a array are copied to cells of \a list.
 * \return
 * zVec2DArrayToList() returns a pointer \a list.
 * \sa zVec2DArrayAppendList
 */
__ZEO_EXPORT ZEO_VECXD_LIST_ARRAY_TO_LIST_PROTOTYPE( 2D );

/*! \brief destroy a 2D vector list.
 *
 * zVec2DListDestroy() destroys a list of vectors \a list, freeing all cells.
 * \return
 * zVec2DListDestroy() returns no value.
 */
#define zVec2DListDestroy(list) zVecXDListDestroy( 2D, list )

/*! \brief find an identical 2D vector in a list.
 *
 * zVec2DListFind() finds a 2D vector in a list \a list that is identical with the given vector \a v.
 * \return
 * zVec2DListFind() returns a pointer to the 2D vector in \a list, if found. Otherwise, it returns
 * the null pointer.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_FIND_PROTOTYPE( 2D );

/*! \brief a quick sort routine for vector list class.
 *
 * zVec2DListQuickSort() is a quick sort routine for zVec2DList class.
 *
 * The cells of \a list will be sorted in ascending order according to the comparison function \a cmp.
 * (The factor a in \a list is put after another factor b when \a cmp(a,b,p) > 0, where p is for
 * programmer's utility, given by \a priv.)
 * \return
 * zVec2DListQuickSort() returns a pointer \a list.
 * \sa
 * zListQuickSortDef
 */
__ZEO_EXPORT ZEO_VECXD_LIST_QUICKSORT_PROTOTYPE( 2D );

/*! \brief print a list of 2D vectors.
 *
 * zVec2DListFPrint() prints a list of 2D vectors \a list out to the current position of a file \a fp
 * in the following format.
 * n
 *  ( x1, y1, z1 )
 *   ...
 *  ( xn, yn, zn )
 * zVec2DValuePrint() prints \a list out to the standard output.
 * \return
 * zVec2DListFPrint() and zVec2DListPrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_FPRINT_PROTOTYPE( 2D );
#define zVec2DListPrint(list) zVecXDListPrint( 2D, list )

/*! \brief print a list of 2D vectors in a plain form.
 *
 * zVec2DListValueFPrint() prints a list of 2D vectors \a list out to the current position of the file \a fp
 * in the following format.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec2DListValuePrint() prints \a list out to the standard output.
 * \return
 * zVec2DListValueFPrint() and zVec2DListValuePrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_VALUE_FPRINT_PROTOTYPE( 2D );
#define zVec2DListValuePrint(list) zVecXDListValuePrint( 2D, list )

/* ********************************************************** */
/* CLASS: zVec2DAddrList
 * list of addresses of 2D vectors
 * ********************************************************** */

ZEO_VECXD_ADDRLIST_DEF_STRUCT( 2D );

/*! \brief add a pointer to a 2D vector into a list of vectors.
 *
 * zVec2DAddrListAdd() adds a pointer to a 2D vector \a v at the head of a list of vectors \a list.
 * \return
 * zVec2DAddrListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_ADD_PROTOTYPE( 2D );

/*! \brief append an array of 2D vectors to a list of pointers to 2D vectors.
 *
 * zVec2DAddrListAppendArray() appends an array of 2D vectors \a array to a list of pointers to
 * 2D vectors \a list.
 * \return
 * zVec2DAddrListAppendArray() returns \a list if succeeding. Otherwise, the null pointer is returned.
 * \sa zVec2DArray2List
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_APPEND_ARRAY_PROTOTYPE( 2D );

/*! \brief create a list of pointers to 2D vectors from an array.
 *
 * zVec2DAddrListCreate() creates a list of pointers to 2D vectors \a list, each cell of which points
 * an element of an array of 2D vectors \a array.
 * \return
 * zVec2DAddrListCreate() returns a pointer \a list.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_CREATE_PROTOTYPE( 2D );

/*! \brief create a list of pointers to 2D vectors from a list of 2D vectors.
 *
 * zVec2DAddrListFromList() creates a list of pointers to 2D vectors \a addrlist, each cell of
 * which points a vector of the corresponding cell of \a list.
 * \return
 * zVec2DAddrListFromList() returns a pointer \a addrlist if it succeeds. Otherwise, the null
 * pointer is returned.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_FROM_LIST_PROTOTYPE( 2D );

/*! \brief clone a list of vectors.
 *
 * zVec2DAddrListClone() clones a list of pointers to 2D vectors \a src and make another list \a dest.
 * \return
 * zVec2DAddrListClone() returns a pointer \a dest if it succeeds.
 * Otherwise, the null pointer is returned.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_CLONE_PROTOTYPE( 2D );

/*! \brief destroy a list of 2D vectors.
 *
 * zVec2DAddrListDestroy() destroys a list of vectors \a list.
 * \return
 * zVec2DAddrListDestroy() returns no value.
 */
#define zVec2DAddrListDestroy(list) zVecXDAddrListDestroy( 2D, list )

/*! \brief a quick sort routine for vector list class.
 *
 * zVec2DAddrListQuickSort() is a quick sort routine for zVec2DAddrList class.
 *
 * The cells of \a list will be sorted in ascending order according to the comparison function \a cmp.
 * (The factor a in \a list is put after another factor b when \a cmp(a,b,p) > 0, where p is for
 * programmer's utility, given by \a priv.)
 * \return
 * zVec2DAddrListQuickSort() returns a pointer \a list.
 * \sa
 * zListQuickSortDef
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_QUICKSORT_PROTOTYPE( 2D );

/*! \brief print a list of pointers to 2D vectors.
 *
 * zVec2DAddrListFPrint() prints a list of pointers to 2D vectors \a list to the current position
 * of a file \a fp in the following format.
 * n
 *  ( x1, y1, z1 )
 *   ...
 *  ( xn, yn, zn )
 * zVec2DAddrListPrint() prints \a list out to the standard output.
 * \return
 * zVec2DAddrListFPrint() and zVec2DAddrListPrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_FPRINT_PROTOTYPE( 2D );
#define zVec2DAddrListPrint(list) zVecXDAddrListPrint( 2D, list )

/*! \brief print a list of pointers to 2D vectors in a plain form.
 *
 * zVec2DAddrListValueFPrint() prints a list of pointers to 2D vectors \a list out to the current
 * position of a file \a fp in the following format.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec2DAddrListValuePrint() prints \a list out to the standard output.
 * \return
 * zVec2DAddrListValueFPrint() and zVec2DAddrListValuePrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_VALUE_FPRINT_PROTOTYPE( 2D );
#define zVec2DAddrListValuePrint(list) zVecXDAddrListValuePrint( 2D, list )

__END_DECLS

#endif /* __ZEO_VEC2D_LIST_H__ */
