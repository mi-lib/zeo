#include <zeo/zeo_col.h>

void gjk_usage(char *arg)
{
  eprintf( "Usage: %s <vlist1.dat> <vlist2.dat>\n", arg );
  eprintf( "vlist1.dat\tlist of vertices 1\n" );
  eprintf( "vlist2.dat\tlist of vertices 2\n" );
  exit( EXIT_SUCCESS );
}

void gjk_read_vset(char filename[], zVec3DList *list)
{
  FILE *fp;
  zVec3D v;

  if( !( fp = fopen( filename, "r" ) ) ){
    ZOPENERROR( filename );
    exit( EXIT_FAILURE );
  }
  zListInit( list );
  do{
    zVec3DFScan( fp, &v );
    zVec3DListAdd( list, &v );
  } while( !feof( fp ) );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zVec3DList vl1, vl2;
  zVec3D c1, c2;

  if( argc < 2 ) gjk_usage( argv[0] );

  gjk_read_vset( argv[1], &vl1 );
  gjk_read_vset( argv[2], &vl2 );

  zGJKPL( &vl1, &vl2, &c1, &c2 );
  zVec3DValueNLPrint( &c1 );
  zVec3DValueNLPrint( &c2 );

  zVec3DListDestroy( &vl1 );
  zVec3DListDestroy( &vl2 );
  return 0;
}
