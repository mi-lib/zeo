#include <zeo/zeo_elem2d.h>

#define N 100

int main(int argc, char *argv[])
{
  zVec2D p1, p2, p, cp;
  zEdge2D e;
  FILE *fp;
  int i;

  zRandInit();
  zVec2DCreate( &p1, zRandF(0.2,0.8), zRandF(0.2,0.8) );
  zVec2DCreate( &p2, zRandF(0.2,0.8), zRandF(0.2,0.8) );
  zEdge2DCreate( &e, &p1, &p2 );
  fp = fopen( "e", "w" );
  zVec2DDataNLFPrint( fp, zEdge2DVert(&e,0) );
  zVec2DDataNLFPrint( fp, zEdge2DVert(&e,1) );
  fclose( fp );
  fp = fopen( "r", "w" );
  for( i=0; i<N; i++ ){
    zVec2DCreate( &p, zRandF(0,1), zRandF(0,1) );
    zEdge2DClosest( &e, &p, &cp );
    zVec2DDataNLFPrint( fp, &p );
    zVec2DDataNLFPrint( fp, &cp );
    fprintf( fp, "\n" );
  }
  fclose( fp );
  return 0;
}
