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
  zVec3DOctant *suboctant[8]; /*!< \brief 8 suboctants */
  zAABox3D region;            /*!< \brief region to cover */
  zVec3D center;              /*!< \brief center of region */
  zVec3DList points;          /*!< \brief list of points */
  /*! \cond */
  zVec3D _norm;               /* unit normal vector */
  zMat3D _ncov;               /* variance-covariance matrix multiplied by the number of points */
  /*! \endcond */
};

/*! \brief check if a 3D point is inside of an octant. */
#define zVec3DOctantPointIsInside(octant,point) zAABox3DPointIsInside( &(octant)->region, point, zTOL )

/*! \brief check if an octant is with the smallest resolution. */
#define zVec3DOctantIsSmallest(octant,resolution) \
  ( zAABox3DDepth( &(octant)->region) < (resolution) + zTOL && \
    zAABox3DWidth( &(octant)->region) < (resolution) + zTOL && \
    zAABox3DHeight(&(octant)->region) < (resolution) + zTOL )

/* matrix-based octree */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec3DOctree ){
  double resolution; /*!< \brief spatial resolution */
  zVec3DOctant root; /*!< \brief root node of octree */
#ifdef __cplusplus
  zVec3DOctree();
  ~zVec3DOctree();
  void setResolution(double _resolution);
  zVec3DOctree *init(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution);
  zVec3DOctree *init(zAABox3D *region, double resolution);
  void destroy();
  zVec3DOctant *addPoint(zVec3D *point);
  zVec3DOctant *addPoint(zVec3D &point);
  zVec3DOctree *addPoints(zVec3DData *pointdata);
  void updateNormal();
  zVec3DOctree *create(zVec3DData *pointdata, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution);
  bool changeResolution(double _resolution);
  zVec3DOctree *unifyOctant();
  const zVec3DOctant *findContainer(const zVec3D *point);
  const zVec3DOctant *findContainer(const zVec3D &point);
  double nearestNeighbor(const zVec3D *point, zVec3D **nn);
  double nearestNeighbor(const zVec3D &point, zVec3D **nn);
  const zVec3D *nearestNeighbor(const zVec3D *point);
  const zVec3D *nearestNeighbor(const zVec3D &point);
  zVec3DData *vicinity(const zVec3D *point, double radius, zVec3DData *vicinity);
  zVec3DData *vicinity(const zVec3D &point, double radius, zVec3DData *vicinity);
  int toAABox3DList(zAABox3DList *list);
#endif /* __cplusplus */
};

#define zVec3DOctreeSetResolution(octree,res) ( (octree)->resolution = (res) )

/*! \brief initialize 3D octree. */
__ZEO_EXPORT zVec3DOctree *zVec3DOctreeInit(zVec3DOctree *octree, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution);
/*! \brief initialize 3D octree as a cubed region with resolution multiplied by power of two. */
__ZEO_EXPORT zVec3DOctree *zVec3DOctreeInitAuto(zVec3DOctree *octree, zAABox3D *region, double resolution);

/*! \brief destroy 3D octree. */
__ZEO_EXPORT void zVec3DOctreeDestroy(zVec3DOctree *octree);

/*! \brief add a 3D point to 3D octree. */
__ZEO_EXPORT zVec3DOctant *zVec3DOctreeAddPoint(zVec3DOctree *octree, zVec3D *point);

/*! \brief add a set of 3D vectors to a 3D octree. */
__ZEO_EXPORT zVec3DOctree *zVec3DOctreeAddData(zVec3DOctree *octree, zVec3DData *pointdata);

/*! \brief update normal vector of a 3D octree. */
__ZEO_EXPORT void zVec3DOctreeUpdateNormal(zVec3DOctree *octree);

/*! \brief convert a set of 3D vectors to a 3D octree. */
__ZEO_EXPORT zVec3DOctree *zVec3DDataToOctree(zVec3DData *pointdata, zVec3DOctree *octree, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution);

/*! \brief change resolution of 3D octree. */
__ZEO_EXPORT bool zVec3DOctreeChangeResolution(zVec3DOctree *octree, double resolution);

/*! \brief unify octants whose suboctants are all non-empty. */
__ZEO_EXPORT zVec3DOctree *zVec3DOctreeUnifyOctant(zVec3DOctree *octree);

/*! \brief find an octant that contains a 3D point in 3D octree. */
__ZEO_EXPORT const zVec3DOctant *zVec3DOctreeFindContainer(const zVec3DOctree *octree, const zVec3D *point);

/*! \brief find nearest neighbor of a 3D point in a 3D octree. */
__ZEO_EXPORT double zVec3DOctreeNN(const zVec3DOctree *octree, const zVec3D *point, zVec3D **nn);

/*! \brief find vicinity of a point in 3D octree. */
__ZEO_EXPORT zVec3DData *zVec3DOctreeVicinity(const zVec3DOctree *octree, const zVec3D *point, double radius, zVec3DData *vicinity);

/*! \brief convert a 3D octree to a list of axis-aligned boxes. */
__ZEO_EXPORT int zVec3DOctreeToAABox3DList(const zVec3DOctree *octree, zAABox3DList *list);

__END_DECLS

#ifdef __cplusplus
inline zVec3DOctree::zVec3DOctree(){ zVec3DOctreeInit( this, 0, 0, 0, 0, 0, 0, 1 ); }
inline zVec3DOctree::~zVec3DOctree(){ zVec3DOctreeDestroy( this ); }
inline void zVec3DOctree::setResolution(double _resolution){ zVec3DOctreeSetResolution( this, _resolution ); }
inline zVec3DOctree *zVec3DOctree::init(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double _resolution){ return zVec3DOctreeInit( this, xmin, ymin, zmin, xmax, ymax, zmax, _resolution ); }
inline zVec3DOctree *zVec3DOctree::init(zAABox3D *region, double _resolution){ return zVec3DOctreeInitAuto( this, region, _resolution ); }
inline void zVec3DOctree::destroy(){ zVec3DOctreeDestroy( this ); }
inline zVec3DOctant *zVec3DOctree::addPoint(zVec3D *point){ return zVec3DOctreeAddPoint( this, point ); }
inline zVec3DOctant *zVec3DOctree::addPoint(zVec3D &point){ return zVec3DOctreeAddPoint( this, &point ); }
inline zVec3DOctree *zVec3DOctree::addPoints(zVec3DData *pointdata){ return zVec3DOctreeAddData( this, pointdata ); }
inline void zVec3DOctree::updateNormal(){ zVec3DOctreeUpdateNormal( this ); }
inline zVec3DOctree *zVec3DOctree::create(zVec3DData *pointdata, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double _resolution){ return zVec3DDataToOctree( pointdata, this, xmin, ymin, zmin, xmax, ymax, zmax, _resolution ); }
inline bool zVec3DOctree::changeResolution(double _resolution){ return zVec3DOctreeChangeResolution( this, _resolution ); }
inline zVec3DOctree *zVec3DOctree::unifyOctant(){ return zVec3DOctreeUnifyOctant( this ); }
inline const zVec3DOctant *zVec3DOctree::findContainer(const zVec3D *point){ return zVec3DOctreeFindContainer( this, point ); }
inline const zVec3DOctant *zVec3DOctree::findContainer(const zVec3D &point){ return zVec3DOctreeFindContainer( this, &point ); }
inline double zVec3DOctree::nearestNeighbor(const zVec3D *point, zVec3D **nn){ return zVec3DOctreeNN( this, point, nn ); }
inline double zVec3DOctree::nearestNeighbor(const zVec3D &point, zVec3D **nn){ return zVec3DOctreeNN( this, &point, nn ); }
inline const zVec3D *zVec3DOctree::nearestNeighbor(const zVec3D *point){ zVec3D *nn; zVec3DOctreeNN( this, point, &nn ); return nn; }
inline const zVec3D *zVec3DOctree::nearestNeighbor(const zVec3D &point){ zVec3D *nn; zVec3DOctreeNN( this, &point, &nn ); return nn; }
inline zVec3DData *zVec3DOctree::vicinity(const zVec3D *point, double radius, zVec3DData *vicinity){ return zVec3DOctreeVicinity( this, point, radius, vicinity ); }
inline zVec3DData *zVec3DOctree::vicinity(const zVec3D &point, double radius, zVec3DData *vicinity){ return zVec3DOctreeVicinity( this, &point, radius, vicinity ); }
inline int zVec3DOctree::toAABox3DList(zAABox3DList *list){ return zVec3DOctreeToAABox3DList( this, list ); }
#endif /* __cplusplus */

#endif /* __ZEO_VEC3D_OCTREE_H__ */
