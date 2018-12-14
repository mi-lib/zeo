#include <zeo/zeo.h>

#define MODEL "../model/cube.zph"

int main(void)
{
  zPH3D src, dest;
  char buf[BUFSIZ];
  FILE *fp;

  if( !( fp = fopen( MODEL, "rt" ) ) ){
    ZOPENERROR( MODEL );
    return 1;
  }
  fgets( buf, BUFSIZ, fp ); /* skip */
  fgets( buf, BUFSIZ, fp ); /* skip */
  zPH3DFRead( fp, &src );
  fclose( fp );

  zPH3DClone( &src, &dest );
  zPH3DDestroy( &src );
  zPH3DWrite( &dest );
  zPH3DDestroy( &dest );
  return 0;
}
