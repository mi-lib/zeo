/* zeo_terra
 * - terrain map generator
 *  1. generate leven terrain map
 *     input: xmin:xmax, ymin:ymax, zmin:zmax, dx, dy
 *     output: terrain map
 *  2. estimate terrain map from PCD
 *     input: PCD file, xmin:xmax, ymin:ymax, zmin:zmax, dx, dy
 *     output: terrain map
 *  3. resize terrain map (yet to be implemented)
 *     input: terrain map, xmin:xmax, ymin:ymax, zmin:zmax, dx, dy
 *     output: terrain map
 *  4. convert terrain map to triangle mesh (yet to be implemented)
 *     input: terrain map
 *     output: shape
 */

#include <zeo/zeo_mshape.h>
#include <zeo/zeo_terra.h>

enum{
  TERRA_OUTFILE = 0,
  TERRA_INFILE,
  TERRA_XRANGE,
  TERRA_YRANGE,
  TERRA_ZRANGE,
  TERRA_RESOL,
  TERRA_PCD,
  TERRA_VERBOSE,
  TERRA_HELP,
  TERRA_INVALID
};
zOption opt[] = {
  { "o", "output", "<file name>", "output .ztr file", (char *)"terra.ztr", false },
  { "i", "input", "<file name>", "input .ztr file", NULL, false },
  { "x", "xrange", "<xmin>:<xmax>", "range in x-axis", (char *)"-1:1", false },
  { "y", "yrange", "<ymin>:<ymax>", "range in y-axis", (char *)"-1:1", false },
  { "z", "zrange", "<zmin>:<zmax>", "range in z-axis", (char *)"0:10", false },
  { "r", "resolution", "<dx>:<dy>", "horizontal resolution", (char *)"0.1:0.1", false },
  { "p", "pcd", "<file name>", "PCD (point cloud data) file", NULL, false },
  { "v", "verbose", NULL, "make this program verbose", NULL, false },
  { "h", "help", NULL, "show this message", NULL, false },
  { NULL, NULL, NULL, NULL, NULL, false },
};

void terraUsage(void)
{
  eprintf( "Usage: terra [options]\n" );
  eprintf( "<options>\n" );
  zOptionHelp( opt );
  exit( EXIT_SUCCESS );
}

bool terraCommandArgs(int argc, char *argv[], FILE **fp)
{
  zStrList arglist;
  bool ret = true;

  zOptionRead( opt, argv, &arglist );
  if( opt[TERRA_HELP].flag ) terraUsage();
  if( !zListIsEmpty(&arglist) ){
    opt[TERRA_OUTFILE].flag = true;
    opt[TERRA_OUTFILE].arg  = zListTail(&arglist)->data;
  }
  if( !( *fp = fopen( opt[TERRA_OUTFILE].arg, "w" ) ) ){
    ZOPENERROR( opt[TERRA_OUTFILE].arg );
    ret = false;
  }
  zStrListDestroy( &arglist, false );
  return ret;
}

zVec3D *grid;
int nx, ny;

void terraSetGrid(int i, int j, double x, double y, double z)
{
  if( i < 0 || i > nx || j < 0 || j > ny ) return;
  zVec3DCreate( &grid[(nx+1)*j+i], x, y, z );
}

bool terraCreateGrid(double x0, double y0, double x1, double y1)
{
  register int i, j;

  if( !( grid = zRealloc( grid, zVec3D, (nx+1)*(ny+1) ) ) )
    return false;
  if( x0 > x1 ) zSwap( double, x0, x1 );
  if( y0 > y1 ) zSwap( double, y0, y1 );

  for( i=0; i<=nx; i++ )
    for( j=0; j<=ny; j++ )
      terraSetGrid( i, j, x0+(x1-x0)*i/nx, y0+(y1-y0)*j/ny, 0 );
  return true;
}

void terraOutputOpticOne(FILE *fp, char *hex, char *name)
{
  zRGB rgb;
  zOpticalInfo opt;

  zRGBDec( &rgb, hex );
  zOpticalInfoCreateSimple( &opt, rgb.r, rgb.g, rgb.b, name );
  zOpticalInfoSetAlpha( &opt, 0.8 );
  fprintf( fp, "[optic]\n" );
  zOpticalInfoFWrite( fp, &opt );
}

void terraOutputOptic(FILE *fp, char *hex1, char *hex2)
{
  terraOutputOpticOne( fp, hex1, "color0" );
  terraOutputOpticOne( fp, hex2, "color1" );
}

void terraOutputShapeOne(FILE *fp, int id)
{
  register int i, j, n;
  int i1, i2, i3, i4;

  fprintf( fp, "\n[shape]\n" );
  fprintf( fp, "name: mesh%d\n", id );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: color%d\n", id );
  n = ( nx + 1 ) * ( ny + 1 );
  for( i=0; i<n; i++ ){
    fprintf( fp, "vert %d: ", i );
    zVec3DFWrite( fp, &grid[i] );
  }
  for( i=0; i<nx; i++ )
    for( j=0; j<ny; j++ ){
      if( ( i % 2 + j % 2 ) % 2 == id ) continue;
      i1 = ( nx + 1 ) * j + i;
      i2 = ( nx + 1 ) * j + i + 1;
      i3 = ( nx + 1 ) * ( j + 1 ) + i;
      i4 = ( nx + 1 ) * ( j + 1 ) + i + 1;
      fprintf( fp, "face %d %d %d\n", i1, i2, i3 );
      fprintf( fp, "face %d %d %d\n", i2, i4, i3 );
    }
}

void terraOutputShape(FILE *fp)
{
  terraOutputShapeOne( fp, 0 );
  terraOutputShapeOne( fp, 1 );
}

int main(int argc, char *argv[])
{
  FILE *fp;

  if( !terraCommandArgs( argc, argv+1, &fp ) ) return 1;

  nx = atoi(opt[TERRA_NX].arg);
  ny = atoi(opt[TERRA_NY].arg);
  terraCreateGrid( atof(opt[TERRA_X0].arg), atof(opt[TERRA_Y0].arg), atof(opt[TERRA_X1].arg), atof(opt[TERRA_Y1].arg) );

  terraOutputOptic( fp, opt[TERRA_COLOR1].arg, opt[TERRA_COLOR2].arg );
  terraOutputShape( fp );
  fclose( fp );

  zFree( grid );
  return 0;
}
