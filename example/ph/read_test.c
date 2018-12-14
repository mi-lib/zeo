#include <zeo/zeo.h>

#define MODEL "../model/cube.zph"

int main(void)
{
  zPH3D ph;
  char buf[BUFSIZ];
  FILE *fp;

  if( !( fp = fopen( MODEL, "rt" ) ) ){
    ZOPENERROR( MODEL );
    return 1;
  }
  fgets( buf, BUFSIZ, fp ); /* skip */
  fgets( buf, BUFSIZ, fp ); /* skip */
  zPH3DFRead( fp, &ph );
  fclose( fp );

  zPH3DWrite( &ph );
  return 0;
}
