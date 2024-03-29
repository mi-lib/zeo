#include <zeo/zeo.h>

void zeo_stl2ztk_usage(char *arg)
{
  eprintf( "Usage: %s <STL file>\n", arg );
  eprintf( "zeo_stl2ztk reads an STL file and outputs it to the standard output in ZTK format\n" );
  exit( EXIT_SUCCESS );
}

int main(int argc, char *argv[])
{
  zShape3D shape;

  if( argc < 2 ) zeo_stl2ztk_usage( argv[0] );
  zShape3DInit( &shape );
  if( zShape3DReadFileSTL( &shape, argv[1] ) )
    zShape3DFPrintZTK( stdout, &shape );
  zShape3DDestroy( &shape );
  return EXIT_SUCCESS;
}
