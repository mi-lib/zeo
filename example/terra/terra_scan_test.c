#include <zeo/zeo_terra.h>

int main(int argc, char *argv[])
{
  zTerra terra;
  FILE *fp;

  fp = fopen( "terrain.ztr", "r" );
  zTerraFScan( fp, &terra );
  fclose( fp );
  zTerraDataFPrint( stdout, &terra );
  zTerraFree( &terra );
  return 0;
}
