#include <zeo/zeo_shape3d.h>
#include <zeo/zeo_bv3d.h>

static void output_opticalinfo(FILE *fp, const zOpticalInfo *oi)
{
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  zOpticalInfoFPrintZTK( fp, oi );
}

static void output_octant_leaf(FILE *fp, const zVec3DOctant *octant, const zOpticalInfo *oi, const char *name)
{
  zBox3D box;

  zAABox3DToBox3D( &octant->region, &box );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: %s\n", name );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: %s\n", zName(oi) );
  zBox3DFPrintZTK( fp, &box );
}

static void output_octant(FILE *fp, const zVec3DOctant *octant, const zOpticalInfo *oi, const char *basename)
{
  char nodename[BUFSIZ];
  bool have_suboctant = false;
  int i;

  for( i=0; i<8; i++ ){
    if( octant->suboctant[i] ){
      have_suboctant = true;
      zStrCopy( nodename, basename, BUFSIZ );
      zStrAddChar( nodename, BUFSIZ, '0' + i );
      output_octant( fp, octant->suboctant[i], oi, nodename );
    }
  }
  if( !have_suboctant )
    output_octant_leaf( fp, octant, oi, basename );
}

static void output_octree(FILE *fp, const zVec3DOctree *octree, const zOpticalInfo *oi, const char *name)
{
  output_opticalinfo( fp, oi );
  output_octant( fp, &octree->root, oi, name );
}

static void output_point(const zVec3D *point, double radius, const zOpticalInfo *oi, const char *filename)
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

static void output_box(const zVec3DOctant *octant, const zOpticalInfo *oi, const char *filename)
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

#define RESOLUTION 0.02

int main(int argc, char *argv[])
{
  zVec3DOctree octree;
  const zVec3DOctant *octant;
  zOpticalInfo oi_green;
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
  zVec3DOctreeInitAuto( &octree, &aabb, RESOLUTION );
  zVec3DOctreeAddData( &octree, &pointdata );

  zOpticalInfoCreate( &oi_green, 0.5, 0.5, 0.5, 0.3, 1.0, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, "green" );
  if( ( fp = fopen( "octree04.ztk", "w" ) ) ){
    output_opticalinfo( fp, &oi_green );
    output_octree( fp, &octree, &oi_green, "octant" );
    fclose( fp );
  }
  zVec3DOctreeChangeResolution( &octree, RESOLUTION/4 );
  if( ( fp = fopen( "octree16.ztk", "w" ) ) ){
    output_opticalinfo( fp, &oi_green );
    output_octree( fp, &octree, &oi_green, "octant_fine" );
    fclose( fp );
  }
  zVec3DOctreeChangeResolution( &octree, RESOLUTION/8 );
  if( ( fp = fopen( "octree32.ztk", "w" ) ) ){
    output_opticalinfo( fp, &oi_green );
    output_octree( fp, &octree, &oi_green, "octant_merged" );
    fclose( fp );
  }
  zVec3DOctreeChangeResolution( &octree, RESOLUTION/2 );
  if( ( fp = fopen( "octree08.ztk", "w" ) ) ){
    output_opticalinfo( fp, &oi_green );
    output_octree( fp, &octree, &oi_green, "octant_divided" );
    fclose( fp );
  }
  zOpticalInfoDestroy( &oi_green );

  zVec3DCreate( &point,
    zRandF(zAABox3DXMin(&octree.root.region),zAABox3DXMax(&octree.root.region)),
    zRandF(zAABox3DYMin(&octree.root.region),zAABox3DYMax(&octree.root.region)),
    zRandF(zAABox3DZMin(&octree.root.region),zAABox3DZMax(&octree.root.region)) );
  zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, "red" );
  output_point( &point, RESOLUTION, &oi, "point.ztk" );
  zOpticalInfoDestroy( &oi );
  if( ( octant = zVec3DOctreeFindContainer( &octree, &point ) ) ){
    zOpticalInfoCreate( &oi, 0.5, 0.5, 0.5, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, "magenta" );
    output_box( octant, &oi, "box.ztk" );
    zOpticalInfoDestroy( &oi );
  }

  zVec3DOctreeDestroy( &octree );
  zVec3DDataDestroy( &pointdata );
  return EXIT_SUCCESS;
}
