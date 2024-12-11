/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_list - list of 3D vectors.
 */

#ifndef __ZEO_VEC3D_LIST_H__
#define __ZEO_VEC3D_LIST_H__

#include <zeo/zeo_vecxd_list.h>

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zVec3DList
 * list of 3D vectors
 * ********************************************************** */

ZEO_VECXD_LIST_DEF_STRUCT( 3D );

/*! \brief allocate a 3D vector list cell and copy a vector.
 *
 * zVec3DListNew() allocates a new 3D vector list cell and copy a 3D vector \a v to it.
 * \return
 * zVec3DListNew() returns a pointer to the newly allocated cell.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_NEW_PROTOTYPE( 3D );

/*! \brief add a 3D vector to a vector list.
 *
 * zVec3DListAdd() adds a copy of a 3D vector \a v at the head of a list of vectors \a list.
 * \return
 * zVec3DListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_ADD_PROTOTYPE( 3D );

/*! \brief append an array of 3D vectors to a list.
 *
 * zVec3DListAppendArray() appends an array of 3D vectors \a array to a list of 3D vectors \a list.
 * Vectors of \a array are copied to cells of \a list.
 * \return
 * zVec3DListAppendArray() returns \a list if succeeding. Otherwise, the null pointer is returned.
 * \sa zVec3DArray2List
 */
__ZEO_EXPORT ZEO_VECXD_LIST_APPEND_ARRAY_PROTOTYPE( 3D );

/*! \brief clone an array of 3D vectors.
 *
 * zVec3DListClone() clones an array of 3D vectors \a src and make another list \a dest.
 * \return
 * zVec3DListClone() returns a pointer \a dest if it succeeds.
 * Otherwise, the null pointer is returned.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_CLONE_PROTOTYPE( 3D );

/*! \brief converts an array of 3D vectors to a list.
 *
 * zVec3DArrayToList() converts an array of 3D vectors \a array to a list of 3D vectors \a list.
 * Vectors of \a array are copied to cells of \a list.
 * \return
 * zVec3DArrayToList() returns a pointer \a list.
 * \sa zVec3DArrayAppendList
 */
__ZEO_EXPORT ZEO_VECXD_LIST_ARRAY_TO_LIST_PROTOTYPE( 3D );

/*! \brief destroy a 3D vector list.
 *
 * zVec3DListDestroy() destroys a list of vectors \a list, freeing all cells.
 * \return
 * zVec3DListDestroy() returns no value.
 */
#define zVec3DListDestroy(list) zVecXDListDestroy( 3D, list )

/*! \brief find an identical 3D vector in a list.
 *
 * zVec3DListFind() finds a 3D vector in a list \a list that is identical with the given vector \a v.
 * \return
 * zVec3DListFind() returns a pointer to the 3D vector in \a list, if found. Otherwise, it returns
 * the null pointer.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_FIND_PROTOTYPE( 3D );

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
__ZEO_EXPORT ZEO_VECXD_LIST_QUICKSORT_PROTOTYPE( 3D );

/*! \brief print a list of 3D vectors.
 *
 * zVec3DListFPrint() prints a list of 3D vectors \a list out to the current position of a file \a fp
 * in the following format.
 * n
 *  ( x1, y1, z1 )
 *   ...
 *  ( xn, yn, zn )
 * zVec3DValuePrint() prints \a list out to the standard output.
 * \return
 * zVec3DListFPrint() and zVec3DListPrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_FPRINT_PROTOTYPE( 3D );
#define zVec3DListPrint(list) zVecXDListPrint( 3D, list )

/*! \brief print a list of 3D vectors in a plain form.
 *
 * zVec3DListValueFPrint() prints a list of 3D vectors \a list out to the current position of the file \a fp
 * in the following format.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec3DListValuePrint() prints \a list out to the standard output.
 * \return
 * zVec3DListValueFPrint() and zVec3DListValuePrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_VALUE_FPRINT_PROTOTYPE( 3D );
#define zVec3DListValuePrint(list) zVecXDListValuePrint( 3D, list )

/* ********************************************************** */
/* CLASS: zVec3DAddrList
 * list of addresses of 3D vectors
 * ********************************************************** */

ZEO_VECXD_ADDRLIST_DEF_STRUCT( 3D );

/*! \brief add a pointer to a 3D vector into a list of vectors.
 *
 * zVec3DAddrListAdd() adds a pointer to a 3D vector \a v at the head of a list of vectors \a list.
 * \return
 * zVec3DAddrListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_ADD_PROTOTYPE( 3D );

/*! \brief append an array of 3D vectors to a list of pointers to 3D vectors.
 *
 * zVec3DAddrListAppendArray() appends an array of 3D vectors \a array to a list of pointers to
 * 3D vectors \a list.
 * \return
 * zVec3DAddrListAppendArray() returns \a list if succeeding. Otherwise, the null pointer is returned.
 * \sa zVec3DArray2List
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_APPEND_ARRAY_PROTOTYPE( 3D );

/*! \brief create a list of pointers to 3D vectors from an array.
 *
 * zVec3DAddrListCreate() creates a list of pointers to 3D vectors \a list, each cell of which points
 * an element of an array of 3D vectors \a array.
 * \return
 * zVec3DAddrListCreate() returns a pointer \a list.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_CREATE_PROTOTYPE( 3D );

/*! \brief create a list of pointers to 3D vectors from a list of 3D vectors.
 *
 * zVec3DAddrListFromList() creates a list of pointers to 3D vectors \a addrlist, each cell of
 * which points a vector of the corresponding cell of \a list.
 * \return
 * zVec3DAddrListFromList() returns a pointer \a addrlist if it succeeds. Otherwise, the null
 * pointer is returned.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_FROM_LIST_PROTOTYPE( 3D );

/*! \brief clone a list of vectors.
 *
 * zVec3DAddrListClone() clones a list of pointers to 3D vectors \a src and make another list \a dest.
 * \return
 * zVec3DAddrListClone() returns a pointer \a dest if it succeeds.
 * Otherwise, the null pointer is returned.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_CLONE_PROTOTYPE( 3D );

/*! \brief destroy a list of 3D vectors.
 *
 * zVec3DAddrListDestroy() destroys a list of vectors \a list.
 * \return
 * zVec3DAddrListDestroy() returns no value.
 */
#define zVec3DAddrListDestroy(list) zVecXDAddrListDestroy( 3D, list )

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
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_QUICKSORT_PROTOTYPE( 3D );

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
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_FPRINT_PROTOTYPE( 3D );
#define zVec3DAddrListPrint(list) zVecXDAddrListPrint( 3D, list )

/*! \brief print a list of pointers to 3D vectors in a plain form.
 *
 * zVec3DAddrListValueFPrint() prints a list of pointers to 3D vectors \a list out to the current
 * position of a file \a fp in the following format.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec3DAddrListValuePrint() prints \a list out to the standard output.
 * \return
 * zVec3DAddrListValueFPrint() and zVec3DAddrListValuePrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_ADDRLIST_VALUE_FPRINT_PROTOTYPE( 3D );
#define zVec3DAddrListValuePrint(list) zVecXDAddrListValuePrint( 3D, list )

__END_DECLS

#endif /* __ZEO_VEC3D_LIST_H__ */
