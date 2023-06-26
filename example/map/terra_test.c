#ifndef __unix__
#include <unistd.h>
#endif /* __unix__ */
#include <zeo/zeo_map.h>

#define XMIN -5.0
#define XMAX  5.0
#define YMIN -5.0
#define YMAX  5.0
#define ZMIN -HUGE_VAL
#define ZMAX  HUGE_VAL

#define DX   0.5
#define DY   0.5

#define DIV_PATH 10

#define N   10000

#define TERRAIN_SRC_FILE  "terrain_src.dat"
#define TERRAIN_PATH_FILE "path"
#define TERRAIN_OUT_FILE  "terrain.ztk"
#define TERRAIN_DAT_FILE  "terrain.dat"
#define TERRAIN_VAR_FILE  "terrain_var.dat"

void terra_gen_sample(zVec3DList *pl)
{
  double x, y, z;
  zVec3D *p;
  register int i;
  FILE *fp;

  zListInit( pl );
  for( i=0; i<N; i++ ){
    x = zRandF( XMIN, XMAX );
    y = zRandF( YMIN, YMAX );
    z = 2*cos( zPI*x/(XMAX-XMIN) )*cos( zPI*y/(XMAX-XMIN) ) + 0.5*cos( 4*zPI*x/(XMAX-XMIN) )*cos( 4*zPI*y/(XMAX-XMIN) ) + zRandF(-1.0,1.0);
    p = zAlloc( zVec3D, 1 );
    zVec3DCreate( p, x, y, z );
    zVec3DListAdd( pl, p );
  }
  fp = fopen( TERRAIN_SRC_FILE, "w" );
  zVec3DListDataFPrint( fp, pl );
  fclose( fp );
}

void path_find_test(zTerra *terra)
{
  double x0, y0, x1, y1, x, y, z;
  int i;
  FILE *fp;

  x0 = zRandF( terra->xmin, 0.5 * ( terra->xmin + terra->xmax ) );
  y0 = zRandF( terra->ymin, 0.5 * ( terra->ymin + terra->ymax ) );
  x1 = zRandF( 0.5 * ( terra->xmin + terra->xmax ), terra->xmax );
  y1 = zRandF( 0.5 * ( terra->ymin + terra->ymax ), terra->ymax );
  fp = fopen( TERRAIN_PATH_FILE, "w" );
  for( i=0; i<=DIV_PATH; i++ ){
    x = x0 + ( x1 - x0 ) * i / DIV_PATH;
    y = y0 + ( y1 - y0 ) * i / DIV_PATH;
    z = zTerraZ( terra, x, y );
    fprintf( fp, "%.10g %.10g %.10g\n", x, y, z );
  }
  fclose( fp );
}

void terra_output(zTerra *terra, double scale)
{
  FILE *fp;
  zTerraCell *grid;
  register int i, j;

  fp = zOpenZTKFile( TERRAIN_OUT_FILE, "w" );
  fprintf( fp, "[%s]\n", ZTK_TAG_MAP );
  fprintf( fp, "name: test_terrain\n" );
  fprintf( fp, "type: terra\n" );
  zTerraFPrintZTK( fp, terra );
  fclose( fp );

  fp = fopen( TERRAIN_DAT_FILE, "w" );
  for( i=0; i<zTerraXSize(terra); i++ )
    for( j=0; j<zTerraYSize(terra); j++ ){
      grid = zTerraGridNC(terra,i,j);
      fprintf( fp, "%.10g %.10g %.10g\n", zTerraX(terra,i), zTerraY(terra,j), grid->z );
      fprintf( fp, "%.10g %.10g %.10g\n\n\n", zTerraX(terra,i)+scale*grid->norm.e[zX], zTerraY(terra,j)+scale*grid->norm.e[zY], grid->z+scale*grid->norm.e[zZ] );
    }
  fclose( fp );

  fp = fopen( TERRAIN_VAR_FILE, "w" );
  for( i=0; i<zTerraXSize(terra); i++ )
    for( j=0; j<zTerraYSize(terra); j++ ){
      grid = zTerraGridNC(terra,i,j);
      fprintf( fp, "%.10g %.10g %.10g\n", zTerraX(terra,i), zTerraY(terra,j), grid->z-0.5*grid->var );
      fprintf( fp, "%.10g %.10g %.10g\n\n\n", zTerraX(terra,i), zTerraY(terra,j), grid->z+0.5*grid->var );
    }
  fclose( fp );
}

void delete_file(void)
{
  unlink( TERRAIN_SRC_FILE );
  unlink( TERRAIN_PATH_FILE );
  unlink( TERRAIN_OUT_FILE );
  unlink( TERRAIN_DAT_FILE );
  unlink( TERRAIN_VAR_FILE );
  exit( 0 );
}

int main(int argc, char *argv[])
{
  zTerra terra;
  zVec3DList pl;

  if( argc > 1 && strcmp( argv[1], "clear" ) == 0 ) delete_file();
  zRandInit();
  terra_gen_sample( &pl );
  zTerraAllocRegion( &terra, XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX, DX, DY );
  zTerraIdent( &terra, &pl );
  zTerraCheckTravs( &terra );
  zTerraAdjustZRange( &terra );
  terra_output( &terra, 0.3 );
  path_find_test( &terra );
  zTerraDestroy( &terra );
  zVec3DListDestroy( &pl );
  return 0;
}
