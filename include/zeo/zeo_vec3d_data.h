/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_data - abstract class of a set of 3D vectors.
 */

#ifndef __ZEO_VEC3D_DATA_H__
#define __ZEO_VEC3D_DATA_H__

#include <zeo/zeo_ep.h>
#include <zeo/zeo_elem3d.h>
#include <zeo/zeo_vecxd_data.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zVec3DData
 * a set of 3D vectors
 * ********************************************************** */
ZEO_VECXD_DATA_DEF_STRUCT( 3D );

#define zVec3DDataDestroy(data)  zVecXDDataDestroy(data)
#define zVec3DDataSize(data)     zVecXDDataSize(data)
#define zVec3DDataIsEmpty(data)  zVecXDDataIsEmpty(data)
#define zVec3DDataAdd(data,v)    zVecXDDataAdd(data,v)
#define zVec3DDataRewind(data)   zVecXDDataRewind(data)
#define zVec3DDataFetch(data)    zVecXDDataFetch(data)

/*! \brief initialize 3D vector data as an array. */
__ZEO_EXPORT ZEO_VECXD_DATA_INIT_ARRAY_PROTOTYPE( 3D );

/*! \brief initialize 3D vector data as a list. */
__ZEO_EXPORT ZEO_VECXD_DATA_INIT_LIST_PROTOTYPE( 3D );

/*! \brief initialize 3D vector data as a list of addresses. */
__ZEO_EXPORT ZEO_VECXD_DATA_INIT_ADDRLIST_PROTOTYPE( 3D );

/*! \brief directly assign an array to 3D vector data. */
__ZEO_EXPORT ZEO_VECXD_DATA_ASSIGN_ARRAY_DIRECT_PROTOTYPE( 3D );

/*! \brief assign an array to 3D vector data. */
__ZEO_EXPORT ZEO_VECXD_DATA_ASSIGN_ARRAY_PROTOTYPE( 3D );

/*! \brief assign a list to 3D vector data. */
__ZEO_EXPORT ZEO_VECXD_DATA_ASSIGN_LIST_PROTOTYPE( 3D );

/*! \brief assign a list of addresses to 3D vector data. */
__ZEO_EXPORT ZEO_VECXD_DATA_ASSIGN_ADDRLIST_PROTOTYPE( 3D );

/*! \brief create a list of addresses of 3D vectors of data. */
__ZEO_EXPORT ZEO_VECXD_DATA_TO_ADDRLIST_PROTOTYPE( 3D );

/*! \brief find an identical vector in a set of 3D vectors. */
__ZEO_EXPORT ZEO_VECXD_DATA_FIND_PROTOTYPE( 3D );

/*! \brief transform points in a set of 3D vectors. */
__ZEO_EXPORT ZEO_VECXD_DATA_XFORM_PROTOTYPE( 3D );

/*! \brief a naive algorithm to find the nearest neighbor in a set of 3D vectors. */
__ZEO_EXPORT ZEO_VECXD_DATA_NN_PROTOTYPE( 3D );

/*! \brief a naive algorithm to find vicinity of a point in a set of 3D vectors. */
__ZEO_EXPORT ZEO_VECXD_DATA_VICINITY_PROTOTYPE( 3D );

/*! \brief support map of a set of 3D points with respect to a direction vector. */
__ZEO_EXPORT ZEO_VECXD_DATA_SUPPORTMAP_PROTOTYPE( 3D );

/*! \brief print out a set of 3D vectors. */
__ZEO_EXPORT ZEO_VECXD_DATA_FPRINT_PROTOTYPE( 3D );
#define zVec3DDataPrint(data) zVecXDDataPrint( 3D, data )

/*! \brief print out values of a set of 3D vectors. */
__ZEO_EXPORT ZEO_VECXD_DATA_VALUE_FPRINT_PROTOTYPE( 3D );
#define zVec3DDataValuePrint(data) zVecXDDataValuePrint( 3D, data )

__END_DECLS

#include <zeo/zeo_vec3d_tree.h>   /* kd-tree with k=3 */
#include <zeo/zeo_vec3d_octree.h> /* octree */

#include <zeo/zeo_vec3d_pca.h>
#include <zeo/zeo_vec3d_pcd.h>

#endif /* __ZEO_VEC3D_DATA_H__ */
