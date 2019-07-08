#include <zeo/zeo_map.h>

int main(int argc, char *argv[])
{
  zMap map;

  if( argc <= 1 ) return 1;
  zMapScanFile( &map, argv[1] );
  zMapFPrint( stdout, &map );
  zMapDestroy( &map );
  return 0;
}
