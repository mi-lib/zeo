#include <zeo/zeo.h>

#define MODEL "../model/cube.ztk"

int main(void)
{
  zPH3D src, dest;
  char buf[BUFSIZ];
  FILE *fp;

  if( !( fp = fopen( MODEL, "rt" ) ) ){
    ZOPENERROR( MODEL );
    return 1;
  }
  if( !fgets( buf, BUFSIZ, fp ) ) exit( 1 ); /* skip */
  if( !fgets( buf, BUFSIZ, fp ) ) exit( 1 ); /* skip */
  zPH3DFScan( fp, &src );
  fclose( fp );

  zPH3DClone( &src, &dest );
  zPH3DDestroy( &src );
  zPH3DPrint( &dest );
  zPH3DDestroy( &dest );
  return 0;
}
