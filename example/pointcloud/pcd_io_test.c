#include <zeo/zeo_pointcloud.h>

int main(int argc, char *argv[])
{
  zVec3DList pc;

  char sample[] = "sample";
  zVec3DListReadPCDFile( &pc, argc > 1 ? argv[1] : sample );
  char output[] = "output";
  char ascii[] = "ascii";
  zVec3DListWritePCDFile( &pc, argc > 2 ? argv[2] : output, argc > 3 ? argv[3] : ascii );
  zVec3DListDestroy( &pc );
  return 0;
}
