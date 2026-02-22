#include <zeo/zeo_multishape3d.h>

int main(int argc, char *argv[])
{
  zMultiShape3D ms;

  if( argc < 2 ) return EXIT_FAILURE;
  zMultiShape3DReadZTK( &ms, argv[1] );
  zMultiShape3DWriteZTK( &ms, "copy.ztk" );
  zMultiShape3DDestroy( &ms );
  return EXIT_SUCCESS;
}
