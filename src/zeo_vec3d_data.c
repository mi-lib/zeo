/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_data - abstract class of a set of 3D vectors.
 */

#include <zeo/zeo_vec3d.h>

/* methods for individual dataset representations */
ZEO_VECXD_DATA_DEF_STATIC_METHOD( 3D )

/* initialize 3D vector data as an array. */
ZEO_VECXD_DATA_INIT_ARRAY( 3D )

/* initialize 3D vector data as a list. */
ZEO_VECXD_DATA_INIT_LIST( 3D )

/* initialize 3D vector data as a list of addresses. */
ZEO_VECXD_DATA_INIT_ADDRLIST( 3D )

/* directly assign an array to 3D vector data. */
ZEO_VECXD_DATA_ASSIGN_ARRAY_DIRECT( 3D )

/* assign an array to 3D vector data. */
ZEO_VECXD_DATA_ASSIGN_ARRAY( 3D )

/* assign a list to 3D vector data. */
ZEO_VECXD_DATA_ASSIGN_LIST( 3D )

/* assign a list of addresses to 3D vector data. */
ZEO_VECXD_DATA_ASSIGN_ADDRLIST( 3D )

/* create a list of addresses of 3D vectors of data. */
ZEO_VECXD_DATA_TO_ADDRLIST( 3D )

/* a naive algorithm to find the nearest neighbor in a set of 3D vectors. */
ZEO_VECXD_DATA_NN( 3D )

/* support map of a set of 3D points with respect to a direction vector. */
ZEO_VECXD_DATA_SUPPORTMAP( 3D )

/* print out a set of 3D vectors. */
ZEO_VECXD_DATA_FPRINT( 3D )

/* print out values of a set of 3D vectors. */
ZEO_VECXD_DATA_VALUE_FPRINT( 3D )
