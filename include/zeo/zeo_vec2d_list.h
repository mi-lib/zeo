/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec2d_list - list of 2D vectors.
 */

#ifndef __ZEO_VEC2D_LIST_H__
#define __ZEO_VEC2D_LIST_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zVec2DList
 * list of 2D vectors
 * ********************************************************** */

zListClass( zVec2DList, zVec2DListCell, zVec2D* );

/*! \brief find an identical 2D vector in a list.
 *
 * zVec2DListFind() finds a 2D vector in a list \a list that is
 * identical with the given vector \a v.
 * \return
 * zVec2DListFind() returns a pointer to the 2D vector in \a list,
 * if found. Otherwise, it returns the null pointer.
 */
__ZEO_EXPORT zVec2DListCell *zVec2DListFind(zVec2DList *list, zVec2D *v);

/*! \brief add a 2D vector to a vector list.
 *
 * zVec2DListAdd() adds a copy of a 2D vector \a v at the head of
 * a list of vectors \a list.
 * \return
 * zVec2DListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT zVec2DListCell *zVec2DListAdd(zVec2DList *list, zVec2D *v);

/*! \brief destroy a list of 2D vectors. */
__ZEO_EXPORT void zVec2DListDestroy(zVec2DList *list);

/* ********************************************************** */
/* CLASS: zVec2DAddrList
 * list of pointers to 2D vectors
 * ********************************************************** */

typedef zVec2DList zVec2DAddrList;
typedef zVec2DListCell zVec2DAddr;

/*! \brief add a pointer to a 2D vector into a list of pointers to 2D vectors.
 *
 * zVec2DAddrListAdd() adds a pointer to a 2D vector \a v at the
 * head of a list of vectors \a list.
 * \return
 * zVec2DAddrListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT zVec2DAddr *zVec2DAddrListAdd(zVec2DAddrList *list, zVec2D *v);

/*! \brief create a list of pointers to 2D vectors from an array of 2D vectors.
 *
 * zVec2DAddrListCreate() creates a list of pointers to 2D vectors
 * \a list, each cell of which points an element of an array of 2D
 * vectors \a va.
 * \return
 * zVec2DAddrListCreate() returns a pointer \a list.
 */
__ZEO_EXPORT zVec2DAddrList *zVec2DAddrListCreate(zVec2DAddrList *list, zVec2DArray *va);

/*! \brief destroy a list of 2D vectors.
 *
 * zVec2DAddrListDestroy() destroys a list of vectors \a list.
 * \return
 * zVec2DAddrListDestroy() returns no value.
 */
#define zVec2DAddrListDestroy(l) zListDestroy( zVec2DAddr, l )

__END_DECLS

#endif /* __ZEO_VEC2D_LIST_H__ */
