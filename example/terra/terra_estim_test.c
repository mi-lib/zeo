#include <zeo/zeo_pointcloud.h>
#include <zeo/zeo_terra.h>

#define XMIN 0
#define XMAX 6.4
#define YMIN 0
#define YMAX 4.8
#define ZMIN 0
#define ZMAX 1.0

#define DX   0.1
#define DY   0.1

#define DIV 10

void height_estim_test(zTerra *terra)
{
  double x0, y0, x1, y1, x, y, z;
  int i;
  FILE *fp;

  zRandInit();
  x0 = zRandF( terra->xmin, 0.5 * ( terra->xmin + terra->xmax ) );
  y0 = zRandF( terra->ymin, 0.5 * ( terra->ymin + terra->ymax ) );
  x1 = zRandF( 0.5 * ( terra->xmin + terra->xmax ), terra->xmax );
  y1 = zRandF( 0.5 * ( terra->ymin + terra->ymax ), terra->ymax );
  fp = fopen( "path", "w" );
  for( i=0; i<=DIV; i++ ){
    x = x0 + ( x1 - x0 ) * i / DIV;
    y = y0 + ( y1 - y0 ) * i / DIV;
    z = zTerraZ( terra, x, y );
    fprintf( fp, "%.10g %.10g %.10g\n", x, y, z );
  }
  fclose( fp );
}

void terra_output(zTerra *terra)
{
  FILE *fp;

  fp = fopen( "test.ztr", "w" );
  zTerraFWrite( fp, terra );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zTerra terra;
  zVec3DList pl;

  if( !zVec3DListReadPCDFile( &pl, argc > 1 ? argv[1] : "terra_test.pcd" ) ){
    ZRUNERROR( "cannot read point cloud file" );
    return 1;
  }
  zTerraAllocRegion( &terra, XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX, DX, DY );
  zTerraIdent( &terra, &pl );
  zTerraCheckTravs( &terra );
  terra_output( &terra );
  zTerraDataFWrite( stdout, &terra );
  height_estim_test( &terra );
  zTerraFree( &terra );
  zVec3DListDestroy( &pl );
  return 0;
}
