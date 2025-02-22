/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec2d_list - list of 2D vectors.
 */

#include <zeo/zeo_vec2d.h>

/* ********************************************************** */
/* CLASS: zVec2DList
 * list of 2D vectors
 * ********************************************************** */

/* allocate a 2D vector list cell and copy a vector. */
ZEO_VECXD_LIST_NEW( 2D )

/* add a 2D vector to a vector list. */
ZEO_VECXD_LIST_ADD( 2D )

/* append an array of 2D vectors to a list. */
ZEO_VECXD_LIST_APPEND_ARRAY( 2D )

/* clone an array of 2D vectors. */
ZEO_VECXD_LIST_CLONE( 2D )

/* converts an array of 2D vectors to a list. */
ZEO_VECXD_LIST_ARRAY_TO_LIST( 2D )

/* find an identical 2D vector in a list. */
ZEO_VECXD_LIST_FIND( 2D )

/* quick sort of a list of 2D vectors. */
ZEO_VECXD_LIST_QUICKSORT( 2D )

static int _zVec2DListQuickSortDefaultCmp(void *a, void *b, void *dummy)
{
  zVec2D *v1, *v2;

  v1 = &((zVec2DListCell*)a)->data;
  v2 = &((zVec2DListCell*)b)->data;
  if( v1->e[zX] > v2->e[zX] ) return 1;
  if( v1->e[zX] < v2->e[zX] ) return -1;
  if( v1->e[zY] > v2->e[zY] ) return 1;
  if( v1->e[zY] < v2->e[zY] ) return -1;
  return 0;
}
ZEO_VECXD_LIST_QUICKSORT_DEFAULT( 2D )

/* print a list of 2D vectors to a file. */
ZEO_VECXD_LIST_FPRINT( 2D )

/* print a list of 2D vectors in a plain form to a file. */
ZEO_VECXD_LIST_VALUE_FPRINT( 2D )

/* ********************************************************** */
/* CLASS: zVec2DAddrList
 * list of pointers to 2D vectors
 * ********************************************************** */

/* add a pointer to a 2D vector into a list of pointers to 2D vectors. */
ZEO_VECXD_ADDRLIST_ADD( 2D )

/* append an array of 2D vectors to a list of pointers to 2D vectors. */
ZEO_VECXD_ADDRLIST_APPEND_ARRAY( 2D )

/* create a list of pointers to 2D vectors from an array of 2D vectors. */
ZEO_VECXD_ADDRLIST_CREATE( 2D )

/* create a list of pointers to 2D vectors from a list of 2D vectors. */
ZEO_VECXD_ADDRLIST_FROM_LIST( 2D )

/* clone a list of pointers to 2D vectors. */
ZEO_VECXD_ADDRLIST_CLONE( 2D )

/* quick sort of a list of pointers to 2D vectors. */
ZEO_VECXD_ADDRLIST_QUICKSORT( 2D )

/* print a list of pointers to 2D vectors to a file. */
ZEO_VECXD_ADDRLIST_FPRINT( 2D )

/* print a list of pointers to 2D vectors in a plain form to a file. */
ZEO_VECXD_ADDRLIST_VALUE_FPRINT( 2D )
