#include <zeo/zeo_delaunay2d.h>

void delaunay2d_usage(char *arg)
{
  eprintf( "Usage: %s <list.dat>\n", arg );
  eprintf( "list.dat\tlist of points\n" );
  exit( EXIT_SUCCESS );
}

void delaunay2d_read_vset(char filename[], zVec2DList *pl)
{
  FILE *fp;
  zVec2D v;
  double x, y;
  char buf[BUFSIZ];

  if( !( fp = fopen( filename, "r" ) ) ){
    ZOPENERROR( filename );
    exit( EXIT_FAILURE );
  }
  zListInit( pl );
  do{
    if( !zFToken( fp, buf, BUFSIZ ) ) break;
    x = atof( buf );
    if( !zFToken( fp, buf, BUFSIZ ) ) break;
    y = atof( buf );
    zVec2DCreate( &v, x, y );
    zVec2DListAdd( pl, &v );
  } while( !feof( fp ) );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zVec2DList pl;
  zDTTri2DList tl;

  if( argc < 2 ) delaunay2d_usage( argv[0] );
  delaunay2d_read_vset( argv[1], &pl );
  zDelaunay2D( &pl, &tl );
  zDTTri2DListFPrint( stdout, &tl );
  zVec2DListDestroy( &pl );
  zListDestroy( zDTTri2DListCell, &tl );
  return 0;
}
