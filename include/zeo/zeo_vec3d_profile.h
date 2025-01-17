/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_profile - profiling of 3D pointcloud.
 */

#ifndef __ZEO_VEC3D_PROFILE_H__
#define __ZEO_VEC3D_PROFILE_H__

#include <zeo/zeo_bv3d.h>

__BEGIN_DECLS

#define ZEO_VEC3DDATA_NORMALVEC_NUM_MIN  5
#define ZEO_VEC3DDATA_NORMALVEC_NUM_MAX 30

/*! \brief normal vector cloud of a 3D point cloud. */
__ZEO_EXPORT zVec3DData *zVec3DDataNormalVec_Tree(zVec3DData *pointdata, double radius, zVec3DData *normaldata);
__ZEO_EXPORT zVec3DData *zVec3DDataNormalVec_Octree(zVec3DData *pointdata, double radius, zVec3DData *normaldata);
__ZEO_EXPORT zVec3DData *(* zVec3DDataNormalVec)(zVec3DData*, double, zVec3DData*);

__END_DECLS

#endif /* __ZEO_VEC3D_PROFILE_H__ */
