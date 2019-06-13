#include <zeo/zeo.h>

#define MODEL "../model/cube.ztk"

int main(void)
{
  zPH3D ph;
  char buf[BUFSIZ];
  FILE *fp;

  if( !( fp = fopen( MODEL, "rt" ) ) ){
    ZOPENERROR( MODEL );
    return 1;
  }
  if( !fgets( buf, BUFSIZ, fp ) ) exit( 1 ); /* skip */
  if( !fgets( buf, BUFSIZ, fp ) ) exit( 1 ); /* skip */
  zPH3DFScan( fp, &ph );
  fclose( fp );

  zPH3DPrint( &ph );
  return 0;
}
