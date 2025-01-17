/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_octree - octree for 3D pointcloud.
 */

#ifndef __ZEO_VEC3D_OCTREE_H__
#define __ZEO_VEC3D_OCTREE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* octant of matrix-based octree */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec3DOctant ){
  zVec3DOctant *suboctant[8];
  zAABox3D region;
  zVec3DList points;
};

/* matrix-based octree */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec3DOctree ){
  double resolution;
  zVec3DOctant root;
};

#define zVec3DOctreeSetResolution(octree,res) ( (octree)->resolution = (res) )

/*! \brief initialize 3D octree. */
__ZEO_EXPORT zVec3DOctree *zVec3DOctreeInit(zVec3DOctree *octree, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution);

/*! \brief destroy 3D octree. */
__ZEO_EXPORT void zVec3DOctreeDestroy(zVec3DOctree *octree);

/*! \brief add a 3D point to 3D octree. */
__ZEO_EXPORT bool zVec3DOctreeAddPoint(zVec3DOctree *octree, zVec3D *point);

/*! \brief embed pointcloud to 3D octree. */
__ZEO_EXPORT bool zVec3DOctreeEmbedPoints(zVec3DOctree *octree, zVec3DData *pointdata);

/*! \brief change resolution of 3D octree. */
__ZEO_EXPORT bool zVec3DOctreeChangeResolution(zVec3DOctree *octree, double resolution);

/*! \brief find an octant that contains a 3D point in 3D octree. */
__ZEO_EXPORT const zVec3DOctant *zVec3DOctreeFindContainer(const zVec3DOctree *octree, const zVec3D *point);

/*! \brief find vicinity of a point in 3D octree. */
__ZEO_EXPORT zVec3DData *zVec3DOctreeVicinity(zVec3DOctree *octree, const zVec3D *point, double radius, zVec3DData *vicinity);

__END_DECLS

#endif /* __ZEO_VEC3D_OCTREE_H__ */
