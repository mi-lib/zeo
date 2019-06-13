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

  printf( "<original>\n" );
  zPH3DPrint( &src );
  zPH3DMirror( &src, &dest, zY );
  zPH3DDestroy( &src );
  printf( "<mirrored: y>\n" );
  zPH3DPrint( &dest );
  zPH3DDestroy( &dest );
  return 0;
}
