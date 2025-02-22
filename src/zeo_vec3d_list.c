/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_list - list of 3D vectors.
 */

#include <zeo/zeo_vec3d.h>

/* ********************************************************** */
/* CLASS: zVec3DList
 * list of 3D vectors
 * ********************************************************** */

/* allocate a 3D vector list cell and copy a vector. */
ZEO_VECXD_LIST_NEW( 3D )

/* add a 3D vector to a vector list. */
ZEO_VECXD_LIST_ADD( 3D )

/* append an array of 3D vectors to a list. */
ZEO_VECXD_LIST_APPEND_ARRAY( 3D )

/* clone an array of 3D vectors. */
ZEO_VECXD_LIST_CLONE( 3D )

/* converts an array of 3D vectors to a list. */
ZEO_VECXD_LIST_ARRAY_TO_LIST( 3D )

/* find an identical 3D vector in a list. */
ZEO_VECXD_LIST_FIND( 3D )

/* quick sort of a list of 3D vectors. */
ZEO_VECXD_LIST_QUICKSORT( 3D )

static int _zVec3DListQuickSortDefaultCmp(void *a, void *b, void *dummy)
{
  zVec3D *v1, *v2;

  v1 = &((zVec3DListCell*)a)->data;
  v2 = &((zVec3DListCell*)b)->data;
  if( v1->e[zX] > v2->e[zX] ) return 1;
  if( v1->e[zX] < v2->e[zX] ) return -1;
  if( v1->e[zY] > v2->e[zY] ) return 1;
  if( v1->e[zY] < v2->e[zY] ) return -1;
  if( v1->e[zZ] > v2->e[zZ] ) return 1;
  if( v1->e[zZ] < v2->e[zZ] ) return -1;
  return 0;
}
ZEO_VECXD_LIST_QUICKSORT_DEFAULT( 3D )

/* print a list of 3D vectors to a file. */
ZEO_VECXD_LIST_FPRINT( 3D )

/* print a list of 3D vectors in a plain form to a file. */
ZEO_VECXD_LIST_VALUE_FPRINT( 3D )

/* ********************************************************** */
/* CLASS: zVec3DAddrList
 * list of pointers to 3D vectors
 * ********************************************************** */

/* add a pointer to a 3D vector into a list of pointers to 3D vectors. */
ZEO_VECXD_ADDRLIST_ADD( 3D )

/* append an array of 3D vectors to a list of pointers to 3D vectors. */
ZEO_VECXD_ADDRLIST_APPEND_ARRAY( 3D )

/* create a list of pointers to 3D vectors from an array of 3D vectors. */
ZEO_VECXD_ADDRLIST_CREATE( 3D )

/* create a list of pointers to 3D vectors from a list of 3D vectors. */
ZEO_VECXD_ADDRLIST_FROM_LIST( 3D )

/* clone a list of pointers to 3D vectors. */
ZEO_VECXD_ADDRLIST_CLONE( 3D )

/* quick sort of a list of pointers to 3D vectors. */
ZEO_VECXD_ADDRLIST_QUICKSORT( 3D )

/* print a list of pointers to 3D vectors to a file. */
ZEO_VECXD_ADDRLIST_FPRINT( 3D )

/* print a list of pointers to 3D vectors in a plain form to a file. */
ZEO_VECXD_ADDRLIST_VALUE_FPRINT( 3D )
