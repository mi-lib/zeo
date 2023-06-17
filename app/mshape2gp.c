#include <zeo/zeo.h>

void usage(char *arg)
{
  eprintf( "Usage: %s <.ztk file>\n", arg );
  exit( 0 );
}

void output_shape(zShape3D *s)
{
  int i;

  for( i=0; i<zShape3DFaceNum(s); i++ ){
    zVec3DDataPrint( zShape3DFaceVert(s,i,0) );
    zVec3DDataPrint( zShape3DFaceVert(s,i,1) );
    printf( "\n" );
    zVec3DDataPrint( zShape3DFaceVert(s,i,2) );
    zVec3DDataPrint( zShape3DFaceVert(s,i,2) );
    printf( "\n\n" );
  }
}

void conv_mshape(char *filename)
{
  int i;
  zMShape3D ms;

  if( !zMShape3DReadZTK( &ms, filename ) )
    exit( EXIT_FAILURE );
  for( i=0; i<zMShape3DShapeNum(&ms); i++ )
    output_shape( zMShape3DShape(&ms,i) );
  zMShape3DDestroy( &ms );
}

int main(int argc, char *argv[])
{
  if( argc < 2 ) usage( argv[0] );
  conv_mshape( argv[1] );
  return 0;
}
