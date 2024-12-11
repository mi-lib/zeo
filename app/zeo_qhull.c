/* qhull: 3D convex hull computation based on quickhull algorithm. */

#include <zeo/zeo_mshape3d.h>
#include <zeo/zeo_bv3d.h>

void usage(char *arg)
{
  eprintf( "Usage: %s <vector list file>\n", arg );
  eprintf( "       %s <.ztk file>\n", arg );
  eprintf( "<vector list file>\n" );
  eprintf( " x1, y1, z1\n" );
  eprintf( " x2, y2, z2\n" );
  eprintf( " ...\n" );
  exit( EXIT_SUCCESS );
}

bool read_vert_list(char filename[], zVec3DData *data)
{
  FILE *fp;
  zVec3D v;
  bool retval = true;

  if( !( fp = fopen( filename, "r" ) ) ){
    ZOPENERROR( filename );
    exit( EXIT_FAILURE );
  }
  zVec3DDataInitList( data );
  do{
    zVec3DFScan( fp, &v );
    if( !zVec3DDataAdd( data, &v ) ){
      retval = false;
      break;
    }
  } while( !feof( fp ) );
  fclose( fp );
  return retval;
}

void output(zPH3D *ch)
{
  /* for visualization */
  printf( "[zeo::optic]\n" );
  printf( "name: cyan\n" );
  printf( "ambient: 0.7 1.0 1.0\n" );
  printf( "diffuse: 0.7 1.0 1.0\n" );
  printf( "specular: 0.0 0.0 0.0\n" );
  printf( "alpha: 0.4\n" );
  printf( "esr: 0.0\n\n" );
  printf( "[zeo::shape]\n" );
  printf( "name: ch\n" );
  printf( "type: polyhedron\n" );
  printf( "optic: cyan\n" );
  zPH3DFPrintZTK( stdout, ch );
}

int main(int argc, char *argv[])
{
  zMShape3D ms;
  zVec3DData data;
  zPH3D ch;
  char *sfx;

  if( argc < 2 ) usage( argv[0] );
  sfx = zGetSuffix( argv[1] );
  if( sfx && strcmp( sfx, ZEDA_ZTK_SUFFIX ) == 0 ){
    if( !zMShape3DReadZTK( &ms, argv[1] ) ) return 1;
    if( !zMShape3DVertData( &ms, &data ) ) goto TERMINATE;
    zMShape3DDestroy( &ms );
  } else{
    if( !read_vert_list( argv[1], &data ) ) goto TERMINATE;
  }
  zVec3DDataConvexHull( &data, &ch );
  output( &ch );
  zVec3DDataDestroy( &data );
 TERMINATE:
  zPH3DDestroy( &ch );
  return 0;
}
