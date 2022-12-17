#include <zeo/zeo_mapnet.h>

int main(int argc, char *argv[])
{
  zMapNet mn;

  if( argc <= 1 ) return 1;
  zMapNetReadZTK( &mn, argv[1] );
  zMapNetFPrintZTK( stdout, &mn );
  zMapNetDestroy( &mn );
  return 0;
}
