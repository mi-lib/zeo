#include <zeo/zeo_col.h>
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

static void output_box(zOctant3D *octant, zOpticalInfo *oi, const char *filename)
{
  FILE *fp;
  zBox3D box;

  fp = fopen( filename, "w" );
  output_opticalinfo( fp, oi );
  zAABox3DToBox3D( &octant->region, &box );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: container\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: %s\n", zName(oi) );
  zBox3DFPrintZTK( fp, &box );
  fclose( fp );
}


#define RESOLUTION 0.01

int main(int argc, char *argv[])
{
  zOctree3D octree;
  zOctant3D *octant;
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

  zOctree3DInit( &octree, zAABox3DXMin(&aabb), zAABox3DYMin(&aabb), zAABox3DZMin(&aabb), zAABox3DXMax(&aabb), zAABox3DYMax(&aabb), zAABox3DZMax(&aabb), RESOLUTION );
  zOctree3DEmbedPoints( &octree, &pointdata );

  if( ( fp = fopen( "octree.ztk", "w" ) ) ){
    zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 0.6, 1.0, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.8, "lightgreen" );
    output_opticalinfo( fp, &oi );
    zOctree3DFPrintZTK( fp, &octree, &oi, "octant" );
    zOpticalInfoDestroy( &oi );
    fclose( fp );
  }

  zVec3DCreate( &point,
    zRandF(zAABox3DXMin(&octree.root.region),zAABox3DXMax(&octree.root.region)),
    zRandF(zAABox3DYMin(&octree.root.region),zAABox3DYMax(&octree.root.region)),
    zRandF(zAABox3DZMin(&octree.root.region),zAABox3DZMax(&octree.root.region)) );
  zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, "red" );
  output_point( &point, RESOLUTION, &oi, "point.ztk" );
  zOpticalInfoDestroy( &oi );
  if( ( octant = zOctree3DFindContainer( &octree, &point ) ) ){
    zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.6, "magenta" );
    output_box( octant, &oi, "box.ztk" );
    zOpticalInfoDestroy( &oi );
  }

  zOctree3DChangeResolution( &octree, RESOLUTION/2 );
  if( ( fp = fopen( "octree_divided.ztk", "w" ) ) ){
    zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 0.4, 0.4, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, "blue" );
    output_opticalinfo( fp, &oi );
    zOctree3DFPrintZTK( fp, &octree, &oi, "octant_divided" );
    zOpticalInfoDestroy( &oi );
    fclose( fp );
  }

  zOctree3DChangeResolution( &octree, RESOLUTION*2 );
  if( ( fp = fopen( "octree_merged.ztk", "w" ) ) ){
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
