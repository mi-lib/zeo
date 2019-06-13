#include <zeo/zeo.h>

#define MODEL "../model/cube.ztk"

void scan_ph(zPH3D *ph)
{
  char buf[BUFSIZ];
  FILE *fp;

  if( !( fp = fopen( MODEL, "rt" ) ) ){
    ZOPENERROR( MODEL );
    exit( 1 );
  }
  if( !fgets( buf, BUFSIZ, fp ) ) exit( 1 ); /* skip */
  if( !fgets( buf, BUFSIZ, fp ) ) exit( 1 ); /* skip */
  zPH3DFScan( fp, ph );
  fclose( fp );
}

int main(void)
{
  zPH3D src, dest;
  zFrame3D frame;

  zFrame3DFromZYX( &frame, 1, 1,-1, zDeg2Rad(45), 0, zDeg2Rad(30) );
  zFrame3DPrint( &frame );
  scan_ph( &src );
  zPH3DClone( &src, &dest );

  zPH3DXfer( &src, &frame, &dest );
  zPH3DPrint( &src );
  zPH3DPrint( &dest );

  zPH3DDestroy( &src );
  zPH3DDestroy( &dest );
  return 0;
}
