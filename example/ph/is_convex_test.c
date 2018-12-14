#include <zeo/zeo_ph.h>

bool zPH3DIsConvex(zPH3D *ph)
{
  register int i;

  for( i=0; i<zPH3DFaceNum(ph); i++ )
    if( zTri3DConeVolume( zPH3DFace(ph,i), zPH3DVert(ph,0) ) < 0 )
      return false;
  return true;
}

#define MODEL "../model/cube.zph"

int main(int argc, char *argv[])
{
  zPH3D ph;
  char buf[BUFSIZ], *filename;
  FILE *fp;

  filename = argc > 1 ? argv[1] : MODEL;
  if( !( fp = fopen( filename, "rt" ) ) ){
    ZOPENERROR( filename );
    return 1;
  }
  fgets( buf, BUFSIZ, fp ); /* skip */
  fgets( buf, BUFSIZ, fp ); /* skip */
  zPH3DFRead( fp, &ph );
  fclose( fp );

  printf( "polyhedron is convex: %s\n", zBoolExpr( zPH3DIsConvex( &ph ) ) );
  zPH3DDestroy( &ph );
  return 0;
}
