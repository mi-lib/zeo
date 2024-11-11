#include <zeo/zeo_mshape3d.h>

int main(int argc, char *argv[])
{
  zMShape3D ms;

  if( argc < 2 ) return EXIT_FAILURE;
  zMShape3DReadZTK( &ms, argv[1] );
  zMShape3DWriteZTK( &ms, "copy.ztk" );
  zMShape3DDestroy( &ms );
  return EXIT_SUCCESS;
}
