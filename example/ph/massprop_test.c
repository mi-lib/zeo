#include <zeo/zeo.h>

#define MODEL "../model/cube.ztk"

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
  if( !fgets( buf, BUFSIZ, fp ) ) exit( 1 ); /* skip */
  if( !fgets( buf, BUFSIZ, fp ) ) exit( 1 ); /* skip */
  zPH3DFScan( fp, &ph );
  fclose( fp );

  zPH3DPrint( &ph );
  printf( "volume = %f\n", zPH3DVolume( &ph ) );
  zPH3DBarycenter( &ph, &p );
  printf( "barycenter: " ); zVec3DPrint( &p );
  zPH3DInertia( &ph, &i );
  printf( "inertia: " ); zMat3DPrint( &i );
  zPH3DBaryInertia( &ph, &p, &i );
  printf( "barycenter: " ); zVec3DPrint( &p );
  printf( "inertia about barycenter: " ); zMat3DPrint( &i );
  return 0;
}
