/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_octree - octree for 3D pointcloud.
 */

#include <zeo/zeo_vec3d_data.h>

/* 3D octant */

static void _zVec3DOctantSetRegion(zVec3DOctant *octant, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
{
  zAABox3DCreate( &octant->region, xmin, ymin, zmin, xmax, ymax, zmax );
  zAABox3DCenter( &octant->region, &octant->center );
}

/* initialize a 3D octant. */
static zVec3DOctant *_zVec3DOctantInit(zVec3DOctant *octant)
{
  int i;

  for( i=0; i<8; i++ )
    octant->suboctant[i] = NULL;
  zAABox3DInit( &octant->region );
  zVec3DZero( &octant->center );
  zListInit( &octant->points );

  zVec3DZero( &octant->_norm );
  zMat3DZero( &octant->_ncov );
  return octant;
}

/* destroy a 3D octant. */
static void _zVec3DOctantDestroy(zVec3DOctant *octant)
{
  int i;

  for( i=0; i<8; i++ ){
    if( octant->suboctant[i] )
      _zVec3DOctantDestroy( octant->suboctant[i] );
    free( octant->suboctant[i] );
  }
  zVec3DListDestroy( &octant->points );
}

/* allocate a suboctant of a 3D octant. */
static zVec3DOctant *_zVec3DOctantAllocSuboctant(zVec3DOctant *octant, const zVec3D *point)
{
  byte xb, yb, zb, i;

  i = ( xb = point->c.x > octant->center.c.x ? 0x1 : 0 ) |
      ( yb = point->c.y > octant->center.c.y ? 0x2 : 0 ) |
      ( zb = point->c.z > octant->center.c.z ? 0x4 : 0 );
  if( !octant->suboctant[i] ){
    if( !( octant->suboctant[i] = zAlloc( zVec3DOctant, 1 ) ) ){
      ZALLOCERROR();
      return NULL;
    }
    _zVec3DOctantInit( octant->suboctant[i] );
    _zVec3DOctantSetRegion( octant->suboctant[i],
      xb ? octant->center.c.x : zAABox3DXMin(&octant->region),
      yb ? octant->center.c.y : zAABox3DYMin(&octant->region),
      zb ? octant->center.c.z : zAABox3DZMin(&octant->region),
      xb ? zAABox3DXMax(&octant->region) : octant->center.c.x,
      yb ? zAABox3DYMax(&octant->region) : octant->center.c.y,
      zb ? zAABox3DZMax(&octant->region) : octant->center.c.z );
    zVec3DCopy( &octant->_norm, &octant->suboctant[i]->_norm );
  }
  return octant->suboctant[i];
}

/* estimate normal vector of a 3D octant. */
static void _zVec3DOctantEstimateNormal(zVec3DOctant *octant)
{
  zVec3D norm_tmp;

  zMat3DSymEigMin( &octant->_ncov, &norm_tmp );
  _zVec3DInnerProd( &norm_tmp, &octant->_norm ) >= 0 ?
    zVec3DCopy( &norm_tmp, &octant->_norm ) : zVec3DRev( &norm_tmp, &octant->_norm );
}

/* add a 3D point to a 3D octant. */
static zVec3DOctant *_zVec3DOctantAddPoint(zVec3DOctant *octant, const zVec3D *point, double resolution)
{
  zVec3DOctant *suboctant;

  if( zVec3DOctantIsSmallest( octant, resolution ) )
    return zVec3DListAdd( &octant->points, point ) ? octant : NULL;
  if( !( suboctant = _zVec3DOctantAllocSuboctant( octant, point ) ) ) return NULL;
  return _zVec3DOctantAddPoint( suboctant, point, resolution );
}

/* update normal vector of a 3D octant. */
static void _zVec3DOctantUpdateNormal(zVec3DOctant *octant)
{
  zVec3DListCell *cp;
  zVec3D p;
  int i;

  if( !zListIsEmpty( &octant->points ) ){
    zMat3DZero( &octant->_ncov );
    zListForEach( &octant->points, cp ){
      zVec3DSub( &cp->data, &octant->center, &p );
      zMat3DAddDyad( &octant->_ncov, &p, &p );
    }
    _zVec3DOctantEstimateNormal( octant );
  }
  for( i=0; i<8; i++ )
    if( octant->suboctant[i] )
      _zVec3DOctantUpdateNormal( octant->suboctant[i] );
}

/* divide a 3D octant to suboctants. */
static bool _zVec3DOctantDivide(zVec3DOctant *octant, double resolution)
{
  zVec3DListCell *cp;
  zVec3D p;
  byte i;
  zVec3DOctant *suboctant;

  if( !octant ) return true;
  if( zVec3DOctantIsSmallest( octant, resolution ) ){
    zListForEach( &octant->points, cp ){
      _zVec3DSub( &cp->data, &octant->center, &p );
      _zMat3DAddDyad( &octant->_ncov, &p, &p );
    }
    _zVec3DOctantEstimateNormal( octant );
    return true;
  }
  while( !zListIsEmpty( &octant->points ) ){
    zListDeleteHead( &octant->points, &cp );
    if( !( suboctant = _zVec3DOctantAllocSuboctant( octant, &cp->data ) ) ) return false;
    zListInsertHead( &suboctant->points, cp );
  }
  for( i=0; i<8; i++ )
    if( !_zVec3DOctantDivide( octant->suboctant[i], resolution ) ) return false;
  return true;
}

/* merge a suboctant into the parent octant. */
static void _zVec3DOctantMergeSuboctant(zVec3DOctant *octant, int i)
{
  zListAppend( &octant->points, &octant->suboctant[i]->points );
  _zMat3DAddDRC( &octant->_ncov, &octant->suboctant[i]->_ncov );
  _zVec3DAddDRC( &octant->_norm, &octant->suboctant[i]->_norm );
  _zVec3DOctantDestroy( octant->suboctant[i] );
  zFree( octant->suboctant[i] );
}

/* merge suboctants into the parent octant. */
static void _zVec3DOctantMerge(zVec3DOctant *octant, double resolution)
{
  int i;

  if( !octant ) return;
  for( i=0; i<8; i++ )
    _zVec3DOctantMerge( octant->suboctant[i], resolution );
  if( zVec3DOctantIsSmallest( octant, resolution ) ){
    for( i=0; i<8; i++ )
      if( octant->suboctant[i] )
        _zVec3DOctantMergeSuboctant( octant, i );
    if( !zListIsEmpty( &octant->points ) )
      _zVec3DOctantEstimateNormal( octant );
  }
}

/* unify suboctants of an octant if all of them are non-empty. */
static bool _zVec3DOctantUnify(zVec3DOctant *octant)
{
  uint8_t exist_suboctant = 0;
  int i;

  for( i=0; i<8; i++ ){
    exist_suboctant <<= 1;
    if( octant->suboctant[i] )
      exist_suboctant |= _zVec3DOctantUnify( octant->suboctant[i] ) ? 0x1 : 0;
  }
  if( exist_suboctant == 0 ) return true; /* leaf octant can always be merged. */
  if( exist_suboctant != 0xff ) return false; /* at least one of the suboctants is empty. */
  for( i=0; i<8; i++ ) /* suboctants are unified only if all of them are non-empty. */
    _zVec3DOctantMergeSuboctant( octant, i );
  return true;
}

/* find an octant that contains a 3D point in 3D octant. */
static const zVec3DOctant *_zVec3DOctantFindContainer(const zVec3DOctant *octant, const zVec3D *point)
{
  int i;
  const zVec3DOctant *suboctant;

  if( !octant ) return NULL;
  if( !zVec3DOctantPointIsInside( octant, point ) ) return NULL;
  for( i=0; i<8; i++ )
    if( ( suboctant = _zVec3DOctantFindContainer( octant->suboctant[i], point ) ) ) return suboctant;
  return octant;
}

/* convert an octant to a list of axis-aligned boxes. */
static int _zVec3DOctantToAABox3DList(const zVec3DOctant *octant, zAABox3DList *list)
{
  bool have_suboctant = false;
  int i, ret = 0;

  for( i=0; i<8; i++ ){
    if( octant->suboctant[i] ){
      have_suboctant = true;
      ret += _zVec3DOctantToAABox3DList( octant->suboctant[i], list );
    }
  }
  if( have_suboctant ) return ret;
  return zAABox3DListAdd( list, &octant->region ) ? 1 : 0;
}

/* octree */

/* initialize 3D octree. */
zVec3DOctree *zVec3DOctreeInit(zVec3DOctree *octree, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution)
{
  _zVec3DOctantInit( &octree->root );
  _zVec3DOctantSetRegion( &octree->root, xmin, ymin, zmin, xmax, ymax, zmax );
  zVec3DOctreeSetResolution( octree, resolution );
  return octree;
}

/* initialize 3D octree as a cubed region with resolution multiplied by power of two. */
zVec3DOctree *zVec3DOctreeInitAuto(zVec3DOctree *octree, zAABox3D *region, double resolution)
{
  double h;
  zVec3D center;

  h = resolution * zMax( pow( 2, zMax3(
    ceil( zLog( 2, zAABox3DDepth(region) / resolution ) ),
    ceil( zLog( 2, zAABox3DWidth(region) / resolution ) ),
    ceil( zLog( 2, zAABox3DHeight(region) / resolution ) ) ) - 1 ), 1 );
  zAABox3DCenter( region, &center );
  return zVec3DOctreeInit( octree,
    center.c.x - h, center.c.y - h, center.c.z - h,
    center.c.x + h, center.c.y + h, center.c.z + h, resolution );
}

/* destroy 3D octree. */
void zVec3DOctreeDestroy(zVec3DOctree *octree)
{
  _zVec3DOctantDestroy( &octree->root );
}

/* add a 3D point to 3D octree. */
zVec3DOctant *zVec3DOctreeAddPoint(zVec3DOctree *octree, zVec3D *point)
{
  if( !zVec3DOctantPointIsInside( &octree->root, point ) ){
    ZRUNERROR( ZEO_ERR_OCTREE_POINT_OUTOFREGION );
    return NULL;
  }
  return _zVec3DOctantAddPoint( &octree->root, point, octree->resolution );
}

/* add a set of 3D vectors to a 3D octree. */
zVec3DOctree *zVec3DOctreeAddData(zVec3DOctree *octree, zVec3DData *pointdata)
{
  zVec3D *v;

  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    if( !zVec3DOctreeAddPoint( octree, v ) ) return NULL;
  }
  return octree;
}

/* update normal vector of a 3D octree. */
void zVec3DOctreeUpdateNormal(zVec3DOctree *octree)
{
  _zVec3DOctantUpdateNormal( &octree->root );
}

/* convert a set of 3D vectors to a 3D octree. */
zVec3DOctree *zVec3DDataToOctree(zVec3DData *pointdata, zVec3DOctree *octree, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution)
{
  zVec3DOctreeInit( octree, xmin, ymin, zmin, xmax, ymax, zmax, resolution );
  return zVec3DOctreeAddData( octree, pointdata );
}

/* change resolution of 3D octree. */
bool zVec3DOctreeChangeResolution(zVec3DOctree *octree, double resolution)
{
  if( resolution < octree->resolution ){
    zVec3DOctreeSetResolution( octree, resolution );
    return _zVec3DOctantDivide( &octree->root, octree->resolution );
  }
  zVec3DOctreeSetResolution( octree, resolution );
  _zVec3DOctantMerge( &octree->root, octree->resolution );
  return true;
}

/* unify octants whose suboctants are all non-empty. */
zVec3DOctree *zVec3DOctreeUnifyOctant(zVec3DOctree *octree)
{
  _zVec3DOctantUnify( &octree->root );
  return octree;
}

/* find an octant that contains a 3D point in 3D octant. */
const zVec3DOctant *zVec3DOctreeFindContainer(const zVec3DOctree *octree, const zVec3D *point)
{
  return _zVec3DOctantFindContainer( &octree->root, point );
}

/* find nearest neighbor of a 3D point in a 3D leaf octant. */
static double _zVec3DOctantLeafNN(const zVec3DOctant *octant, const zVec3D *point, zVec3D **nn, double *dmin)
{
  zVec3DListCell *cp;
  double d_sqr;

  zListForEach( &octant->points, cp ){
    if( ( d_sqr = zVec3DSqrDist( &cp->data, point ) ) < _zSqr(*dmin) ){
      *nn = &cp->data;
      *dmin = sqrt( d_sqr );
    }
  }
  return *dmin;
}

/* find nearest neighbor of a 3D point in a 3D octant. */
static double _zVec3DOctantNN(const zVec3DOctant *octant, const zVec3D *point, zVec3D **nn, double *dmin)
{
  int i;

  if( !zListIsEmpty( &octant->points ) )
    return _zVec3DOctantLeafNN( octant, point, nn, dmin );
  for( i=0; i<8; i++ ){
    if( !octant->suboctant[i] ) continue;
    if( zAABox3DSqrDistFromPoint( &octant->suboctant[i]->region, point ) > _zSqr(*dmin) ) continue;
    _zVec3DOctantNN( octant->suboctant[i], point, nn, dmin );
  }
  return *dmin;
}

/* find nearest neighbor of a 3D point in a 3D octree. */
double zVec3DOctreeNN(const zVec3DOctree *octree, const zVec3D *point, zVec3D **nn)
{
  double dmin = HUGE_VAL;
  const zVec3DOctant *container;

  if( ( container = zVec3DOctreeFindContainer( octree, point ) ) )
    _zVec3DOctantLeafNN( container, point, nn, &dmin );
  return _zVec3DOctantNN( &octree->root, point, nn, &dmin );
}

/* find vicinity of a point in a 3D octant. */
static zVec3DData *_zVec3DOctantVicinity(const zVec3DOctant *octant, const zVec3D *point, double radius_sqr, zVec3DData *vicinity)
{
  int i;
  zVec3DListCell *cp;

  if( zAABox3DSqrDistFromPoint( &octant->region, point ) >= radius_sqr ) return vicinity;
  if( !zListIsEmpty( &octant->points ) ){
    zListForEach( &octant->points, cp )
      if( zVec3DSqrDist( &cp->data, point ) < radius_sqr )
        if( !zVec3DDataAdd( vicinity, &cp->data ) ) return NULL;
    return vicinity;
  }
  for( i=0; i<8; i++ ){
    if( !octant->suboctant[i] ) continue;
    if( !_zVec3DOctantVicinity( octant->suboctant[i], point, radius_sqr, vicinity ) ) return NULL;
  }
  return vicinity;
}

/* find vicinity of a point in 3D octree. */
zVec3DData *zVec3DOctreeVicinity(const zVec3DOctree *octree, const zVec3D *point, double radius, zVec3DData *vicinity)
{
  zVec3DDataInitAddrList( vicinity );
  return _zVec3DOctantVicinity( &octree->root, point, _zSqr(radius), vicinity );
}

/* convert a 3D octree to a list of axis-aligned boxes. */
int zVec3DOctreeToAABox3DList(const zVec3DOctree *octree, zAABox3DList *list)
{
  int num;
  num = _zVec3DOctantToAABox3DList( &octree->root, list );
  return num == zListSize(list) ? num : 0;
}
