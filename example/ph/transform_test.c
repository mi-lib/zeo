#include <zeo/zeo.h>

#define MODEL "../model/cube.zph"

void read_ph(zPH3D *ph)
{
  char buf[BUFSIZ];
  FILE *fp;

  if( !( fp = fopen( MODEL, "rt" ) ) ){
    ZOPENERROR( MODEL );
    exit( 1 );
  }
  if( !fgets( buf, BUFSIZ, fp ) ); /* skip */
  if( !fgets( buf, BUFSIZ, fp ) ); /* skip */
  zPH3DFRead( fp, ph );
  fclose( fp );
}

int main(void)
{
  zPH3D src, dest;
  zFrame3D frame;

  zFrame3DZYX( &frame, 1, 1,-1, zDeg2Rad(45), 0, zDeg2Rad(30) );
  zFrame3DWrite( &frame );
  read_ph( &src );
  zPH3DClone( &src, &dest );

  zPH3DXfer( &src, &frame, &dest );
  zPH3DWrite( &src );
  zPH3DWrite( &dest );

  zPH3DDestroy( &src );
  zPH3DDestroy( &dest );
  return 0;
}
