/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_octree - collision checking: octree.
 */

#include <zeo/zeo_col.h>

/* initialize 3D octree. */
zOctree3D *zOctree3DInit(zOctree3D *octree)
{
  int i;

  for( i=0; i<8; i++ )
    octree->octant[i] = NULL;
  zAABox3DInit( &octree->region );
  zListInit( &octree->points );
  return octree;
}

/* destroy 3D octree. */
void zOctree3DDestroy(zOctree3D *octree)
{
  int i;

  for( i=0; i<8; i++ ){
    if( octree->octant[i] )
      zOctree3DDestroy( octree->octant[i] );
    free( octree->octant[i] );
  }
  zVec3DListDestroy( &octree->points );
  zOctree3DInit( octree );
}

/* add a 3D point to 3D octree. */
bool zOctree3DAddPoint(zOctree3D *octree, zVec3D *v, double xres, double yres, double zres)
{
  zVec3D mid;
  byte xb, yb, zb, i;

  if( !zOctree3DPointIsInside( octree, v ) ){
    ZRUNERROR( "point out of region" );
    return false;
  }
  if( zOctree3DIsLeaf( octree, xres, yres, zres ) ){
    return zVec3DListAdd( &octree->points, v ) ? true : false;
  }
  zAABox3DCenter( &octree->region, &mid );
  xb = v->c.x > mid.c.x ? 1 : 0;
  yb = v->c.y > mid.c.y ? 1 : 0;
  zb = v->c.z > mid.c.z ? 1 : 0;
  i = zb << 2 | yb << 1 | xb;
  if( !octree->octant[i] ){
    if( !( octree->octant[i] = zAlloc( zOctree3D, 1 ) ) ){
      ZALLOCERROR();
      return false;
    }
    zOctree3DInit( octree->octant[i] );
    zOctree3DSetRegion( octree->octant[i],
      xb ? mid.c.x : zAABox3DXMin(&octree->region),
      yb ? mid.c.y : zAABox3DYMin(&octree->region),
      zb ? mid.c.z : zAABox3DZMin(&octree->region),
      xb ? zAABox3DXMax(&octree->region) : mid.c.x,
      yb ? zAABox3DYMax(&octree->region) : mid.c.y,
      zb ? zAABox3DZMax(&octree->region) : mid.c.z );
  }
  return zOctree3DAddPoint( octree->octant[i], v, xres, yres, zres );
}

/* merge suboctants in 3D octree. */
void zOctree3DMerge(zOctree3D *octree, double xres, double yres, double zres)
{
  int i;

  if( !octree ) return;
  for( i=0; i<8; i++ )
    zOctree3DMerge( octree->octant[i], xres, yres, zres );
  if( zOctree3DIsLeaf( octree, xres, yres, zres ) )
    for( i=0; i<8; i++ )
      if( octree->octant[i] ){
        zListAppend( &octree->points, &octree->octant[i]->points );
        zOctree3DDestroy( octree->octant[i] );
        zFree( octree->octant[i] );
      }
}

/* find an octant that contains a 3D point in 3D octree. */
zOctree3D *zOctree3DFindOctant(zOctree3D *octree, zVec3D *point)
{
  int i;
  zOctree3D *octant;

  if( !octree ) return NULL;
  if( !zOctree3DPointIsInside( octree, point ) ) return NULL;
  for( i=0; i<8; i++ )
    if( ( octant = zOctree3DFindOctant( octree->octant[i], point ) ) ) return octant;
  return octree;
}

/* build 3D octree from pointcloud. */
bool zVec3DDataOctree(zVec3DData *pointdata, double xres, double yres, double zres, zOctree3D *octree)
{
  zVec3D *v;

  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    if( !zOctree3DAddPoint( octree, v, xres, yres, zres ) ) return false;
  }
  return true;
}

/* for debug */

static void _zOctree3DLeafFPrintZTK(FILE *fp, zOctree3D *octree, zOpticalInfo *oi, const char *name)
{
  zBox3D box;

  zAABox3DToBox3D( &octree->region, &box );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: %s\n", name );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: %s\n", zName(oi) );
  zBox3DFPrintZTK( fp, &box );
}

static void _zOctree3DFPrintZTK(FILE *fp, zOctree3D *octree, zOpticalInfo *oi, const char *basename)
{
  char nodename[BUFSIZ];
  bool have_octant = false;
  int i;

  for( i=0; i<8; i++ ){
    if( octree->octant[i] ){
      have_octant = true;
      zStrCopy( nodename, basename, BUFSIZ );
      zStrAddChar( nodename, BUFSIZ, '0' + i );
      _zOctree3DFPrintZTK( fp, octree->octant[i], oi, nodename );
    }
  }
  if( !have_octant )
    _zOctree3DLeafFPrintZTK( fp, octree, oi, basename );
}

/* print out a ZTK file that represents 3D octree. */
void zOctree3DFPrintZTK(FILE *fp, zOctree3D *octree, zOpticalInfo *oi, const char *name)
{
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  zOpticalInfoFPrintZTK( fp, oi );
  _zOctree3DFPrintZTK( fp, octree, oi, name );
}
