#include <zeo/zeo_mshape.h>

int main(int argc, char *argv[])
{
  zMShape3D ms;

  if( argc <= 1 ) return 1;
  if( zMShape3DScanFile( &ms, argv[1] ) )
    zMShape3DFPrint( stdout, &ms );
  zMShape3DDestroy( &ms );
  return 0;
}
