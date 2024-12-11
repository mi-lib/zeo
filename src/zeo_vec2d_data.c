/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec2d_data - abstract class of a set of 2D vectors.
 */

#include <zeo/zeo_vec2d.h>

/* methods for individual dataset representations */
ZEO_VECXD_DATA_DEF_STATIC_METHOD( 2D )

/* initialize 2D vector data as an array. */
ZEO_VECXD_DATA_INIT_ARRAY( 2D )

/* initialize 2D vector data as a list. */
ZEO_VECXD_DATA_INIT_LIST( 2D )

/* initialize 2D vector data as a list of addresses. */
ZEO_VECXD_DATA_INIT_ADDRLIST( 2D )

/* directly assign an array to 2D vector data. */
ZEO_VECXD_DATA_ASSIGN_ARRAY_DIRECT( 2D )

/* assign an array to 2D vector data. */
ZEO_VECXD_DATA_ASSIGN_ARRAY( 2D )

/* assign a list to 2D vector data. */
ZEO_VECXD_DATA_ASSIGN_LIST( 2D )

/* assign a list of addresses to 2D vector data. */
ZEO_VECXD_DATA_ASSIGN_ADDRLIST( 2D )

/* create a list of addresses of 2D vectors of data. */
ZEO_VECXD_DATA_TO_ADDRLIST( 2D )

/* a naive algorithm to find the nearest neighbor in a set of 2D vectors. */
ZEO_VECXD_DATA_NN( 2D )

/* support map of a set of 2D points with respect to a direction vector. */
ZEO_VECXD_DATA_SUPPORTMAP( 2D )

/* print out a set of 2D vectors. */
ZEO_VECXD_DATA_FPRINT( 2D )

/* print out values of a set of 2D vectors. */
ZEO_VECXD_DATA_VALUE_FPRINT( 2D )
