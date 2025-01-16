/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_octree - collision checking: octree.
 */

#ifndef __ZEO_COL_OCTREE_H__
#define __ZEO_COL_OCTREE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* octant of matrix-based octree */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zOctant3D ){
  zOctant3D *suboctant[8];
  zAABox3D region;
  zVec3DList points;
};

/* matrix-based octree */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zOctree3D ){
  double resolution;
  zOctant3D root;
};

#define zOctree3DSetResolution(octree,res) ( (octree)->resolution = (res) )

/*! \brief initialize 3D octree. */
__ZEO_EXPORT zOctree3D *zOctree3DInit(zOctree3D *octree, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution);

/*! \brief destroy 3D octree. */
__ZEO_EXPORT void zOctree3DDestroy(zOctree3D *octree);

/*! \brief add a 3D point to 3D octree. */
__ZEO_EXPORT bool zOctree3DAddPoint(zOctree3D *octree, zVec3D *v);

/*! \brief embed pointcloud to 3D octree. */
__ZEO_EXPORT bool zOctree3DEmbedPoints(zOctree3D *octree, zVec3DData *pointdata);

/*! \brief change resolution of 3D octree. */
__ZEO_EXPORT bool zOctree3DChangeResolution(zOctree3D *octree, double resolution);

/*! \brief find an octant that contains a 3D point in 3D octree. */
__ZEO_EXPORT const zOctant3D *zOctree3DFindContainer(const zOctree3D *octree, const zVec3D *point);

/*! \brief find vicinity of a point in 3D octree. */
__ZEO_EXPORT zVec3DData *zOctree3DVicinity(zOctree3D *octree, const zVec3D *p, double radius, zVec3DData *vicinity);

/*! \brief normal vector cloud of a 3D point cloud. */
__ZEO_EXPORT zVec3DData *zVec3DDataNormalVec(zVec3DData *pointdata, double radius, zVec3DData *normaldata);

/* for debug */

/*! \brief print out a ZTK file that represents 3D octree. */
__ZEO_EXPORT void zOctree3DFPrintZTK(FILE *fp, const zOctree3D *octree, const zOpticalInfo *oi, const char *name);

__END_DECLS

#endif /* __ZEO_COL_OCTREE_H__ */
