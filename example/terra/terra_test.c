#include <zeo/zeo_terra.h>

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
    zVec3DListInsert( pl, p );
  }
  fp = fopen( "terrain_sample.dat", "w" );
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
  fp = fopen( "path", "w" );
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

  fp = fopen( "terrain.ztr", "w" );
  zTerraFPrint( fp, terra );
  fclose( fp );

  fp = fopen( "terrain.dat", "w" );
  for( i=0; i<terra->_nx; i++ )
    for( j=0; j<terra->_ny; j++ ){
      grid = zTerraGridNC(terra,i,j);
      fprintf( fp, "%.10g %.10g %.10g\n", zTerraX(terra,i), zTerraY(terra,j), grid->z );
      fprintf( fp, "%.10g %.10g %.10g\n\n\n", zTerraX(terra,i)+scale*grid->norm.e[zX], zTerraY(terra,j)+scale*grid->norm.e[zY], grid->z+scale*grid->norm.e[zZ] );
    }
  fclose( fp );

  fp = fopen( "terrain_var.dat", "w" );
  for( i=0; i<terra->_nx; i++ )
    for( j=0; j<terra->_ny; j++ ){
      grid = zTerraGridNC(terra,i,j);
      fprintf( fp, "%.10g %.10g %.10g\n", zTerraX(terra,i), zTerraY(terra,j), grid->z-0.5*grid->var );
      fprintf( fp, "%.10g %.10g %.10g\n\n\n", zTerraX(terra,i), zTerraY(terra,j), grid->z+0.5*grid->var );
    }
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zTerra terra;
  zVec3DList pl;

  zRandInit();
  terra_gen_sample( &pl );
  zTerraAllocRegion( &terra, XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX, DX, DY );
  zTerraIdent( &terra, &pl );
  zTerraCheckTravs( &terra );
  terra_output( &terra, 0.3 );
  path_find_test( &terra );
  zTerraFree( &terra );
  zVec3DListDestroy( &pl );
  return 0;
}
