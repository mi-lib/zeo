/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec6d_list - list of 6D vectors.
 */

#ifndef __ZEO_VEC6D_LIST_H__
#define __ZEO_VEC6D_LIST_H__

#include <zeo/zeo_vecxd_list.h>

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \struct zVec6DList
 * \brief list of 6D vectors
 */
ZEO_VECXD_LIST_DEF_STRUCT( 6D );

/*! \brief allocate a 6D vector list cell and copy a vector.
 *
 * zVec6DListNew() allocates a new 6D vector list cell and copy a 6D vector \a v to it.
 * \return
 * zVec6DListNew() returns a pointer to the newly allocated cell.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_NEW_PROTOTYPE( 6D );

/*! \brief add a 6D vector to a vector list.
 *
 * zVec6DListAdd() adds a copy of a 6D vector \a v at the head of a list of vectors \a list.
 * \return
 * zVec6DListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_ADD_PROTOTYPE( 6D );

/*! \brief clone an array of 6D vectors.
 *
 * zVec6DListClone() clones an array of 6D vectors \a src and make another list \a dest.
 * \return
 * zVec6DListClone() returns a pointer \a dest if it succeeds.
 * Otherwise, the null pointer is returned.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_CLONE_PROTOTYPE( 6D );

/*! \brief destroy a 6D vector list.
 *
 * zVec6DListDestroy() destroys a list of vectors \a list, freeing all cells.
 * \return
 * zVec6DListDestroy() returns no value.
 */
#define zVec6DListDestroy(list) zVecXDListDestroy( 6D, list )

/*! \brief print a list of 6D vectors.
 *
 * zVec6DListFPrint() prints a list of 6D vectors \a list out to the current position of a file \a fp
 * in the following format.
 * n
 *  ( x1, y1, z1 )
 *   ...
 *  ( xn, yn, zn )
 * zVec6DValuePrint() prints \a list out to the standard output.
 * \return
 * zVec6DListFPrint() and zVec6DListPrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_FPRINT_PROTOTYPE( 6D );
#define zVec6DListPrint(list) zVecXDListPrint( 6D, list )

/*! \brief print a list of 6D vectors in a plain form.
 *
 * zVec6DListValueFPrint() prints a list of 6D vectors \a list out to the current position of the file \a fp
 * in the following format.
 *  x1, y1, z1
 *   ...
 *  xn, yn, zn
 * zVec6DListValuePrint() prints \a list out to the standard output.
 * \return
 * zVec6DListValueFPrint() and zVec6DListValuePrint() return no value.
 */
__ZEO_EXPORT ZEO_VECXD_LIST_VALUE_FPRINT_PROTOTYPE( 6D );
#define zVec6DListValuePrint(list) zVecXDListValuePrint( 6D, list )

__END_DECLS

#endif /* __ZEO_VEC6D_LIST_H__ */
