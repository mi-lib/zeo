#include <zeo/zeo.h>

#define MODEL "../model/cube.ztk"

void check(zPH3D *ph, zVec3D *p)
{
  zVec3D v;

  printf( "point: " ); zVec3DPrint( p );
  zPH3DClosest( ph, p, &v );
  printf( "near.: " ); zVec3DPrint( &v );
  printf( "inside? - %s\n", zPH3DPointIsInside(ph,p,false) ? "yes" : "no" );
}

int main(void)
{
  zPH3D ph;
  zVec3D p;
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
  zVec3DCreate( &p, 2, 0.5, 0.5 );
  check( &ph, &p );
  zVec3DCreate( &p, 0.8, 0.1, 0.2 );
  check( &ph, &p );
  zVec3DCreate( &p, 0.5, 0.5, 0.5 );
  check( &ph, &p );
  zPH3DDestroy( &ph );
  return 0;
}
