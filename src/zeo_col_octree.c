/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_octree - collision checking: octree.
 */

#include <zeo/zeo_col.h>

/* 3D octant */

#define _zOctant3DSetRegion(octant,xmin,ymin,zmin,xmax,ymax,zmax) zAABox3DCreate( &(octant)->region, (xmin), (ymin), (zmin), (xmax), (ymax), (zmax) )

#define _zOctant3DPointIsInside(octant,point) zAABox3DPointIsInside( &(octant)->region, point, zTOL )

#define _zOctant3DIsSmallest(octant,resolution) \
  ( zAABox3DDepth( &(octant)->region) < (resolution) + zTOL && \
    zAABox3DWidth( &(octant)->region) < (resolution) + zTOL && \
    zAABox3DHeight(&(octant)->region) < (resolution) + zTOL )

/* initialize a 3D octant. */
static zOctant3D *_zOctant3DInit(zOctant3D *octant)
{
  int i;

  for( i=0; i<8; i++ )
    octant->suboctant[i] = NULL;
  zAABox3DInit( &octant->region );
  zListInit( &octant->points );
  return octant;
}

/* destroy a 3D octant. */
static void _zOctant3DDestroy(zOctant3D *octant)
{
  int i;

  for( i=0; i<8; i++ ){
    if( octant->suboctant[i] )
      _zOctant3DDestroy( octant->suboctant[i] );
    free( octant->suboctant[i] );
  }
  zVec3DListDestroy( &octant->points );
  _zOctant3DInit( octant );
}

/* allocate a suboctant of a 3D octant. */
static zOctant3D *_zOctant3DAllocSuboctant(zOctant3D *octant, byte xb, byte yb, byte zb, zVec3D *mid)
{
  byte i;

  i = zb << 2 | yb << 1 | xb;
  if( !octant->suboctant[i] ){
    if( !( octant->suboctant[i] = zAlloc( zOctant3D, 1 ) ) ){
      ZALLOCERROR();
      return NULL;
    }
    _zOctant3DInit( octant->suboctant[i] );
    _zOctant3DSetRegion( octant->suboctant[i],
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
static bool _zOctant3DAddPoint(zOctant3D *octant, zVec3D *v, double resolution)
{
  zVec3D mid;
  byte xb, yb, zb;
  zOctant3D *suboctant;

  if( _zOctant3DIsSmallest( octant, resolution ) ){
    return zVec3DListAdd( &octant->points, v ) ? true : false;
  }
  zAABox3DCenter( &octant->region, &mid );
  xb = v->c.x > mid.c.x ? 1 : 0;
  yb = v->c.y > mid.c.y ? 1 : 0;
  zb = v->c.z > mid.c.z ? 1 : 0;
  if( !( suboctant = _zOctant3DAllocSuboctant( octant, xb, yb, zb, &mid ) ) ) return false;
  return _zOctant3DAddPoint( suboctant, v, resolution );
}

/* divide a 3D octant to suboctants. */
static bool _zOctant3DDivide(zOctant3D *octant, double resolution)
{
  zVec3DListCell *cp;
  zVec3D mid;
  byte xb, yb, zb, i;
  zOctant3D *suboctant;

  if( !octant ) return true;
  if( _zOctant3DIsSmallest( octant, resolution ) ) return true;
  zAABox3DCenter( &octant->region, &mid );
  while( !zListIsEmpty( &octant->points ) ){
    zListDeleteHead( &octant->points, &cp );
    xb = cp->data.c.x > mid.c.x ? 1 : 0;
    yb = cp->data.c.y > mid.c.y ? 1 : 0;
    zb = cp->data.c.z > mid.c.z ? 1 : 0;
    if( !( suboctant = _zOctant3DAllocSuboctant( octant, xb, yb, zb, &mid ) ) ) return false;
    zListInsertHead( &suboctant->points, cp );
  }
  for( i=0; i<8; i++ ){
    if( !_zOctant3DDivide( octant->suboctant[i], resolution ) ) return false;
  }
  return true;
}

/* merge suboctants into a 3D octant. */
static void _zOctant3DMerge(zOctant3D *octant, double resolution)
{
  int i;

  if( !octant ) return;
  for( i=0; i<8; i++ )
    _zOctant3DMerge( octant->suboctant[i], resolution );
  if( _zOctant3DIsSmallest( octant, resolution ) )
    for( i=0; i<8; i++ )
      if( octant->suboctant[i] ){
        zListAppend( &octant->points, &octant->suboctant[i]->points );
        _zOctant3DDestroy( octant->suboctant[i] );
        zFree( octant->suboctant[i] );
      }
}

/* find an octant that contains a 3D point in 3D octant. */
static const zOctant3D *_zOctant3DFindContainer(const zOctant3D *octant, const zVec3D *point)
{
  int i;
  const zOctant3D *suboctant;

  if( !octant ) return NULL;
  if( !_zOctant3DPointIsInside( octant, point ) ) return NULL;
  for( i=0; i<8; i++ )
    if( ( suboctant = _zOctant3DFindContainer( octant->suboctant[i], point ) ) ) return suboctant;
  return octant;
}

/* octree */

/* initialize 3D octree. */
zOctree3D *zOctree3DInit(zOctree3D *octree, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, double resolution)
{
  _zOctant3DInit( &octree->root );
  _zOctant3DSetRegion( &octree->root, xmin, ymin, zmin, xmax, ymax, zmax );
  zOctree3DSetResolution( octree, resolution );
  return octree;
}

/* destroy 3D octree. */
void zOctree3DDestroy(zOctree3D *octree)
{
  _zOctant3DDestroy( &octree->root );
}

/* add a 3D point to 3D octree. */
bool zOctree3DAddPoint(zOctree3D *octree, zVec3D *v)
{
  if( !_zOctant3DPointIsInside( &octree->root, v ) ){
    ZRUNERROR( ZEO_ERR_OCTREE_POINT_OUTOFREGION );
    return false;
  }
  return _zOctant3DAddPoint( &octree->root, v, octree->resolution );
}

/* build 3D octree from pointcloud. */
bool zOctree3DEmbedPoints(zOctree3D *octree, zVec3DData *pointdata)
{
  zVec3D *v;

  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    if( !zOctree3DAddPoint( octree, v ) ) return false;
  }
  return true;
}

/* change resolution of 3D octree. */
bool zOctree3DChangeResolution(zOctree3D *octree, double resolution)
{
  if( resolution < octree->resolution ){
    zOctree3DSetResolution( octree, resolution );
    return _zOctant3DDivide( &octree->root, octree->resolution );
  }
  zOctree3DSetResolution( octree, resolution );
  _zOctant3DMerge( &octree->root, octree->resolution );
  return true;
}

/* find an octant that contains a 3D point in 3D octant. */
const zOctant3D *zOctree3DFindContainer(const zOctree3D *octree, const zVec3D *point)
{
  return _zOctant3DFindContainer( &octree->root, point );
}

/* find vicinity of a point in a 3D octant. */
static zVec3DData *_zOctant3DVicinity(zOctant3D *octant, const zSphere3D *boundary, zVec3DData *vicinity)
{
  int i;
  zVec3DListCell *cp;

  if( !zColChkSphereAABox3D( boundary, &octant->region ) ) return vicinity;
  if( !zListIsEmpty( &octant->points ) ){
    zListForEach( &octant->points, cp )
      if( zSphere3DPointIsInside( boundary, &cp->data, zTOL ) )
        if( !zVec3DDataAdd( vicinity, &cp->data ) ) return NULL;
    return vicinity;
  }
  for( i=0; i<8; i++ ){
    if( !octant->suboctant[i] ) continue;
    if( !_zOctant3DVicinity( octant->suboctant[i], boundary, vicinity ) ) return NULL;
  }
  return vicinity;
}

/* find vicinity of a point in 3D octree. */
zVec3DData *zOctree3DVicinity(zOctree3D *octree, const zVec3D *p, double radius, zVec3DData *vicinity)
{
  zSphere3D boundary;

  zVec3DDataInitAddrList( vicinity );
  zVec3DDataInitAddrList( vicinity );
  zSphere3DCreate( &boundary, p, radius, 0 );
  return _zOctant3DVicinity( &octree->root, &boundary, vicinity );
}

/* normal vector cloud of a 3D point cloud. */
zVec3DData *zVec3DDataNormalVec(zVec3DData *pointdata, double radius, zVec3DData *normaldata)
{
  zOctree3D octree;
  zAABox3D aabb;
  zVec3DData vicinity;
  zVec3D *v, normal, center, d;

  zVec3DDataInitList( normaldata );
  zVec3DDataAABB( pointdata, &aabb, NULL );
  zOctree3DInit( &octree, zAABox3DXMin(&aabb), zAABox3DYMin(&aabb), zAABox3DZMin(&aabb), zAABox3DXMax(&aabb), zAABox3DYMax(&aabb), zAABox3DZMax(&aabb), radius );
  zOctree3DEmbedPoints( &octree, pointdata );
  zVec3DDataBarycenter( pointdata, &center );
  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    zOctree3DVicinity( &octree, v, radius, &vicinity );
    if( zVec3DDataSize(&vicinity) <  5 ) ZRUNWARN( ZEO_WARN_POINTCLOUD_NORMAL_TOOFEWPOINTS );
    if( zVec3DDataSize(&vicinity) > 30 ) ZRUNWARN( ZEO_WARN_POINTCLOUD_NORMAL_TOOMANYPOINTS );
    zVec3DDataMeanNormal( &vicinity, v, &normal );
    zVec3DSub( v, &center, &d );
    if( zVec3DInnerProd( &normal, &d ) < 0 )
      zVec3DRevDRC( &normal ); /* flip the normal vector to be outward (not a strict way) */
    zVec3DDataAdd( normaldata, &normal );
    zVec3DDataDestroy( &vicinity );
  }
  zOctree3DDestroy( &octree );
  return normaldata;
}

/* for debug */

static void _zOctant3DLeafFPrintZTK(FILE *fp, const zOctant3D *octant, const zOpticalInfo *oi, const char *name)
{
  zBox3D box;

  zAABox3DToBox3D( &octant->region, &box );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: %s\n", name );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: %s\n", zName(oi) );
  zBox3DFPrintZTK( fp, &box );
}

static void _zOctant3DFPrintZTK(FILE *fp, const zOctant3D *octant, const zOpticalInfo *oi, const char *basename)
{
  char nodename[BUFSIZ];
  bool have_suboctant = false;
  int i;

  for( i=0; i<8; i++ ){
    if( octant->suboctant[i] ){
      have_suboctant = true;
      zStrCopy( nodename, basename, BUFSIZ );
      zStrAddChar( nodename, BUFSIZ, '0' + i );
      _zOctant3DFPrintZTK( fp, octant->suboctant[i], oi, nodename );
    }
  }
  if( !have_suboctant )
    _zOctant3DLeafFPrintZTK( fp, octant, oi, basename );
}

/* print out a ZTK file that represents 3D octree. */
void zOctree3DFPrintZTK(FILE *fp, const zOctree3D *octree, const zOpticalInfo *oi, const char *name)
{
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  zOpticalInfoFPrintZTK( fp, oi );
  _zOctant3DFPrintZTK( fp, &octree->root, oi, name );
}
