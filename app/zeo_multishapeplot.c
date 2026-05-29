/* output zMultiShape3D in gnuplot-friendly format. */

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
    zVec3DValueNLPrint( zShape3DFaceVert(s,i,0) );
    zVec3DValueNLPrint( zShape3DFaceVert(s,i,1) );
    printf( "\n" );
    zVec3DValueNLPrint( zShape3DFaceVert(s,i,2) );
    zVec3DValueNLPrint( zShape3DFaceVert(s,i,2) );
    printf( "\n\n" );
  }
}

void conv_multishape(char *filename)
{
  int i;
  zMultiShape3D ms;

  if( !zMultiShape3DReadZTK( &ms, filename ) )
    exit( EXIT_FAILURE );
  for( i=0; i<zMultiShape3DShapeNum(&ms); i++ )
    output_shape( zMultiShape3DShape(&ms,i) );
  zMultiShape3DDestroy( &ms );
}

int main(int argc, char *argv[])
{
  if( argc < 2 ) usage( argv[0] );
  conv_multishape( argv[1] );
  return 0;
}
