#include <zeo/zeo_col.h>

void gjk_usage(char *arg)
{
  eprintf( "Usage: %s <vlist1.dat> <vlist2.dat>\n", arg );
  eprintf( "vlist1.dat\tlist of vertices 1\n" );
  eprintf( "vlist2.dat\tlist of vertices 2\n" );
  exit( EXIT_SUCCESS );
}

void gjk_read_vert_set(char filename[], zVec3DData *data)
{
  FILE *fp;
  zVec3D v;

  if( !( fp = fopen( filename, "r" ) ) ){
    ZOPENERROR( filename );
    exit( EXIT_FAILURE );
  }
  zVec3DDataInitList( data );
  do{
    zVec3DFScan( fp, &v );
    if( !zVec3DDataAdd( data, &v ) ) break;
  } while( !feof( fp ) );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zVec3DData data1, data2;
  zVec3D c1, c2;

  if( argc < 2 ) gjk_usage( argv[0] );

  gjk_read_vert_set( argv[1], &data1 );
  gjk_read_vert_set( argv[2], &data2 );

  zGJK( &data1, &data2, &c1, &c2 );
  zVec3DValueNLPrint( &c1 );
  zVec3DValueNLPrint( &c2 );

  zVec3DDataDestroy( &data1 );
  zVec3DDataDestroy( &data2 );
  return 0;
}
