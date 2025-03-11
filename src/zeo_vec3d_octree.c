/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_octree - octree for 3D pointcloud.
 */

#include <zeo/zeo_vec3d_data.h>

/* 3D octant */

#define _zVec3DOctantSetRegion(octant,xmin,ymin,zmin,xmax,ymax,zmax) zAABox3DCreate( &(octant)->region, (xmin), (ymin), (zmin), (xmax), (ymax), (zmax) )

#define _zVec3DOctantPointIsInside(octant,point) zAABox3DPointIsInside( &(octant)->region, point, zTOL )

#define _zVec3DOctantIsSmallest(octant,resolution) \
  ( zAABox3DDepth( &(octant)->region) < (resolution) + zTOL && \
    zAABox3DWidth( &(octant)->region) < (resolution) + zTOL && \
    zAABox3DHeight(&(octant)->region) < (resolution) + zTOL )

/* initialize a 3D octant. */
static zVec3DOctant *_zVec3DOctantInit(zVec3DOctant *octant)
{
  int i;

  for( i=0; i<8; i++ )
    octant->suboctant[i] = NULL;
  zAABox3DInit( &octant->region );
  zListInit( &octant->points );
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
  _zVec3DOctantInit( octant );
}

/* allocate a suboctant of a 3D octant. */
static zVec3DOctant *_zVec3DOctantAllocSuboctant(zVec3DOctant *octant, byte xb, byte yb, byte zb, zVec3D *mid)
{
  byte i;

  i = zb << 2 | yb << 1 | xb;
  if( !octant->suboctant[i] ){
    if( !( octant->suboctant[i] = zAlloc( zVec3DOctant, 1 ) ) ){
      ZALLOCERROR();
      return NULL;
    }
    _zVec3DOctantInit( octant->suboctant[i] );
    _zVec3DOctantSetRegion( octant->suboctant[i],
      xb ? mid->c.x : zAABox3DXMin(&octant->region),
      yb ? mid->c.y : zAABox3DYMin(&octant->region),
      zb ? mid->c.z : zAABox3DZMin(&octant->region),
      xb ? zAABox3DXMax(&octant->region) : mid->c.x,
      yb ? zAABox3DYMax(&octant->region) : mid->c.y,
      zb ? zAABox3DZMax(&octant->region) : mid->c.z );
  }
  return octant->suboctant[i];
}

/* add a 3D point to a 3D octant. */
static zVec3DOctant *_zVec3DOctantAddPoint(zVec3DOctant *octant, const zVec3D *point, double resolution)
{
  zVec3D mid;
  byte xb, yb, zb;
  zVec3DOctant *suboctant;

  if( _zVec3DOctantIsSmallest( octant, resolution ) )
    return zVec3DListAdd( &octant->points, point ) ? octant : NULL;
  zAABox3DCenter( &octant->region, &mid );
  xb = point->c.x > mid.c.x ? 1 : 0;
  yb = point->c.y > mid.c.y ? 1 : 0;
  zb = point->c.z > mid.c.z ? 1 : 0;
  if( !( suboctant = _zVec3DOctantAllocSuboctant( octant, xb, yb, zb, &mid ) ) ) return NULL;
  return _zVec3DOctantAddPoint( suboctant, point, resolution );
}

/* divide a 3D octant to suboctants. */
static bool _zVec3DOctantDivide(zVec3DOctant *octant, double resolution)
{
  zVec3DListCell *cp;
  zVec3D mid;
  byte xb, yb, zb, i;
  zVec3DOctant *suboctant;

  if( !octant ) return true;
  if( _zVec3DOctantIsSmallest( octant, resolution ) ) return true;
  zAABox3DCenter( &octant->region, &mid );
  while( !zListIsEmpty( &octant->points ) ){
    zListDeleteHead( &octant->points, &cp );
    xb = cp->data.c.x > mid.c.x ? 1 : 0;
    yb = cp->data.c.y > mid.c.y ? 1 : 0;
    zb = cp->data.c.z > mid.c.z ? 1 : 0;
    if( !( suboctant = _zVec3DOctantAllocSuboctant( octant, xb, yb, zb, &mid ) ) ) return false;
    zListInsertHead( &suboctant->points, cp );
  }
  for( i=0; i<8; i++ )
    if( !_zVec3DOctantDivide( octant->suboctant[i], resolution ) ) return false;
  return true;
}

/* merge suboctants into a 3D octant. */
static void _zVec3DOctantMerge(zVec3DOctant *octant, double resolution)
{
  int i;

  if( !octant ) return;
  for( i=0; i<8; i++ )
    _zVec3DOctantMerge( octant->suboctant[i], resolution );
  if( _zVec3DOctantIsSmallest( octant, resolution ) )
    for( i=0; i<8; i++ )
      if( octant->suboctant[i] ){
        zListAppend( &octant->points, &octant->suboctant[i]->points );
        _zVec3DOctantDestroy( octant->suboctant[i] );
        zFree( octant->suboctant[i] );
      }
}

/* find an octant that contains a 3D point in 3D octant. */
static const zVec3DOctant *_zVec3DOctantFindContainer(const zVec3DOctant *octant, const zVec3D *point)
{
  int i;
  const zVec3DOctant *suboctant;

  if( !octant ) return NULL;
  if( !_zVec3DOctantPointIsInside( octant, point ) ) return NULL;
  for( i=0; i<8; i++ )
    if( ( suboctant = _zVec3DOctantFindContainer( octant->suboctant[i], point ) ) ) return suboctant;
  return octant;
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
  if( !_zVec3DOctantPointIsInside( &octree->root, point ) ){
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
static zVec3DData *_zVec3DOctantVicinity(zVec3DOctant *octant, const zVec3D *point, double radius_sqr, zVec3DData *vicinity)
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
zVec3DData *zVec3DOctreeVicinity(zVec3DOctree *octree, const zVec3D *point, double radius, zVec3DData *vicinity)
{
  zVec3DDataInitAddrList( vicinity );
  return _zVec3DOctantVicinity( &octree->root, point, _zSqr(radius), vicinity );
}
