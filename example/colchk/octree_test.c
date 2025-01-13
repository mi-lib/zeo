#include <zeo/zeo_bv3d.h>
#include <zeo/zeo_vec3d_pcd.h>

static void output_opticalinfo(FILE *fp, zOpticalInfo *oi)
{
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  zOpticalInfoFPrintZTK( fp, oi );
}

static void output_point(zVec3D *point, double radius, zOpticalInfo *oi, const char *filename)
{
  FILE *fp;

  fp = fopen( filename, "w" );
  output_opticalinfo( fp, oi );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: point\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: %s\n", zName(oi) );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, point );
  fprintf( fp, "radius: %g\n", radius );
  fclose( fp );
}

static void output_box(zOctree3D *octant, zOpticalInfo *oi, const char *filename)
{
  FILE *fp;

  fp = fopen( filename, "w" );
  output_opticalinfo( fp, oi );
  _zOctree3DLeafFPrintZTK( fp, octant, oi, "basename" );
  fclose( fp );
}


#define RESOLUTION 0.005

int main(int argc, char *argv[])
{
  zOctree3D octree;
  zOctree3D *octant;
  zOpticalInfo oi;
  zVec3DData pointdata;
  zAABox3D aabb;
  zVec3D point;
  FILE *fp;

  if( argc < 2 ){
    ZRUNERROR( "PCD file not specified" );
    return EXIT_FAILURE;
  }
  zVec3DDataReadPCDFile( &pointdata, argv[1] );
  zVec3DDataAABB( &pointdata, &aabb, NULL );

  zOctree3DInit( &octree );
  zOctree3DSetRegion( &octree, zAABox3DXMin(&aabb), zAABox3DYMin(&aabb), zAABox3DZMin(&aabb), zAABox3DXMax(&aabb), zAABox3DYMax(&aabb), zAABox3DZMax(&aabb) );

  zVec3DDataOctree( &pointdata, RESOLUTION, RESOLUTION, RESOLUTION, &octree );

  if( ( fp = fopen( "octree.ztk", "w" ) ) ){
    zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 0.6, 1.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, "lightgreen" );
    output_opticalinfo( fp, &oi );
    zOctree3DFPrintZTK( fp, &octree, &oi, "octant" );
    zOpticalInfoDestroy( &oi );
    fclose( fp );
  }

  zVec3DCreate( &point,
    zRandF(zAABox3DXMin(&octree.region),zAABox3DXMax(&octree.region)),
    zRandF(zAABox3DYMin(&octree.region),zAABox3DYMax(&octree.region)),
    zRandF(zAABox3DZMin(&octree.region),zAABox3DZMax(&octree.region)) );
  zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, "red" );
  output_point( &point, RESOLUTION, &oi, "point.ztk" );
  zOpticalInfoDestroy( &oi );
  if( ( octant = zOctree3DFindOctant( &octree, &point ) ) ){
    zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, "magenta" );
    output_box( octant, &oi, "box.ztk" );
    zOpticalInfoDestroy( &oi );
  }

  zOctree3DMerge( &octree, 4*RESOLUTION, 4*RESOLUTION, 4*RESOLUTION );
  if( ( fp = fopen( "octree_merge.ztk", "w" ) ) ){
    zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 0.6, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.5, "yellow" );
    output_opticalinfo( fp, &oi );
    zOctree3DFPrintZTK( fp, &octree, &oi, "octant_merged" );
    zOpticalInfoDestroy( &oi );
    fclose( fp );
  }

  zOctree3DDestroy( &octree );
  zVec3DDataDestroy( &pointdata );
  return EXIT_SUCCESS;
}
