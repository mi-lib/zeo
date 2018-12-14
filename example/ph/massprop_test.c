#include <zeo/zeo.h>

#define MODEL "../model/cube.zph"

int main(void)
{
  zPH3D ph;
  zVec3D p;
  zMat3D i;
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
  printf( "volume = %f\n", zPH3DVolume( &ph ) );
  zPH3DBarycenter( &ph, &p );
  printf( "barycenter: " ); zVec3DWrite( &p );
  zPH3DInertia( &ph, &i );
  printf( "inertia: " ); zMat3DWrite( &i );
  zPH3DBaryInertia( &ph, &p, &i );
  printf( "barycenter: " ); zVec3DWrite( &p );
  printf( "inertia about barycenter: " ); zMat3DWrite( &i );
  return 0;
}
