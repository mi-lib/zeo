#include <zeo/zeo_elem3d.h>

#define N 100

int main(void)
{
  zVec3D p1, p2, p, cp;
  zEdge3D e;
  FILE *fp;
  int i;

  zRandInit();
  zVec3DCreate( &p1, zRandF(0.2,0.8), zRandF(0.2,0.8), 0 );
  zVec3DCreate( &p2, zRandF(0.2,0.8), zRandF(0.2,0.8), 0 );
  zEdge3DCreate( &e, &p1, &p2 );
  fp = fopen( "e", "w" );
  zVec3DDataNLFPrint( fp, zEdge3DVert(&e,0) );
  zVec3DDataNLFPrint( fp, zEdge3DVert(&e,1) );
  fclose( fp );
  fp = fopen( "r", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(0,1), zRandF(0,1), 0 );
    zEdge3DClosest( &e, &p, &cp );
    zVec3DDataNLFPrint( fp, &p );
    zVec3DDataNLFPrint( fp, &cp );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );
  return 0;
}
