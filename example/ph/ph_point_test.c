#include <zeo/zeo.h>

#define MODEL "../model/cube.zph"

void check(zPH3D *ph, zVec3D *p)
{
  zVec3D v;

  printf( "point: " ); zVec3DWrite( p );
  zPH3DClosest( ph, p, &v );
  printf( "near.: " ); zVec3DWrite( &v );
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
  fgets( buf, BUFSIZ, fp ); /* skip */
  fgets( buf, BUFSIZ, fp ); /* skip */
  zPH3DFRead( fp, &ph );
  fclose( fp );

  zPH3DWrite( &ph );
  zVec3DCreate( &p, 2, 0.5, 0.5 );
  check( &ph, &p );
  zVec3DCreate( &p, 0.8, 0.1, 0.2 );
  check( &ph, &p );
  zVec3DCreate( &p, 0.5, 0.5, 0.5 );
  check( &ph, &p );
  zPH3DDestroy( &ph );
  return 0;
}
