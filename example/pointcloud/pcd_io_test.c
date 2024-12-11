#include <zeo/zeo_pointcloud.h>

int main(int argc, char *argv[])
{
  zVec3DData data;
  char sample[] = "sample";
  char output[] = "output";
  char ascii[] = "ascii";

  zVec3DDataInitList( &data );
  zVec3DDataReadPCDFile( &data, argc > 1 ? argv[1] : sample );
  zVec3DDataWritePCDFile( &data, argc > 2 ? argv[2] : output, argc > 3 ? argv[3] : ascii );
  zVec3DDataDestroy( &data );
  return 0;
}
