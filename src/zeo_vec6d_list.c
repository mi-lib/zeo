/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_list - list of 6D vectors.
 */

#include <zeo/zeo_vec6d.h>

/* allocate a 6D vector list cell and copy a vector. */
ZEO_VECXD_LIST_NEW( 6D )

/* add a 6D vector to a vector list. */
ZEO_VECXD_LIST_ADD( 6D )

/* clone an array of 6D vectors. */
ZEO_VECXD_LIST_CLONE( 6D )

/* print a list of 6D vectors to a file. */
ZEO_VECXD_LIST_FPRINT( 6D )

/* print a list of 6D vectors in a plain form to a file. */
ZEO_VECXD_LIST_VALUE_FPRINT( 6D )
