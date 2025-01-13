/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_octree - collision checking: octree.
 */

#ifndef __ZEO_COL_OCTREE_H__
#define __ZEO_COL_OCTREE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* matrix-based octree */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zOctree3D ){
  struct _zOctree3D *octant[8];
  zAABox3D region;
  zVec3DList points;
};

#define zOctree3DSetRegion(octree,xmin,ymin,zmin,xmax,ymax,zmax) zAABox3DCreate( &(octree)->region, (xmin), (ymin), (zmin), (xmax), (ymax), (zmax) )

#define zOctree3DPointIsInside(octree,point) zAABox3DPointIsInside( &(octree)->region, point, zTOL )

#define zOctree3DIsLeaf(octree,xres,yres,zres) \
  ( zAABox3DDepth( &(octree)->region) < (xres) + zTOL && \
    zAABox3DWidth( &(octree)->region) < (yres) + zTOL && \
    zAABox3DHeight(&(octree)->region) < (zres) + zTOL )

/*! \brief initialize 3D octree. */
__ZEO_EXPORT zOctree3D *zOctree3DInit(zOctree3D *octree);

/*! \brief destroy 3D octree. */
__ZEO_EXPORT void zOctree3DDestroy(zOctree3D *octree);

/*! \brief add a 3D point to 3D octree. */
__ZEO_EXPORT bool zOctree3DAddPoint(zOctree3D *octree, zVec3D *v, double xres, double yres, double zres);

/*! \brief merge suboctants in 3D octree. */
__ZEO_EXPORT void zOctree3DMerge(zOctree3D *octree, double xres, double yres, double zres);

/*! \brief find an octant that contains a 3D point in 3D octree. */
__ZEO_EXPORT zOctree3D *zOctree3DFindOctant(zOctree3D *octree, zVec3D *point);

/*! \brief build 3D octree from pointcloud. */
__ZEO_EXPORT bool zVec3DDataOctree(zVec3DData *pointdata, double xres, double yres, double zres, zOctree3D *octree);

/* for debug */

/*! \brief print out a ZTK file that represents 3D octree. */
__ZEO_EXPORT void zOctree3DFPrintZTK(FILE *fp, zOctree3D *octree, zOpticalInfo *oi, const char *name);

__END_DECLS

#endif /* __ZEO_COL_OCTREE_H__ */
