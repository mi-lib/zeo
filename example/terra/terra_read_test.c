#include <zeo/zeo_terra.h>

int main(int argc, char *argv[])
{
  zTerra terra;
  FILE *fp;

  fp = fopen( "test.ztr", "r" );
  zTerraFRead( fp, &terra );
  fclose( fp );
  zTerraDataFWrite( stdout, &terra );
  zTerraFree( &terra );
  return 0;
}
