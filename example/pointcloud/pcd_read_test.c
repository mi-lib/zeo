#include <zeo/zeo_pointcloud.h>

int main(int argc, char *argv[])
{
  zVec3DList pc;

  zVec3DListReadPCDFile( &pc, argc > 1 ? argv[1] : "sample" );
  zVec3DListDataPrint( &pc );
  zVec3DListDestroy( &pc );
  return 0;
}
