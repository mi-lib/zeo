#include <zeo/zeo_voronoi2d.h>

void generate_points(FILE *fp, zVec2DList *pl, int n)
{
  register int i;
  zVec2D p;

  zListInit( pl );
  for( i=0; i<n; i++ ){
#if 1
    zVec2DCreate( &p, zRandF(-10,10), zRandF(-10,10) );
#else
    zVec2DCreate( &p, i / 10, i % 10 );
#endif
    zVec2DListAdd( pl, &p );
    zVec2DDataFPrint( fp, &p );
    fprintf( fp, "\n" );
  }
}

#define N  100

int main(int argc, char *argv[])
{
  zVec2DList pl;
  zDTTri2DList tl;
  zVD2DList vl;
  FILE *fp;
  int n;

  n = argc > 1 ? atoi( argv[1] ) : N;
  zRandInit();
  fp = fopen( "v", "w" );
  generate_points( fp, &pl, n );
  fclose( fp );

  zDelaunay2D( &pl, &tl );
  fp = fopen( "t", "w" );
  zDTTri2DListFPrint( fp, &tl );
  fclose( fp );

  zVoronoi2D( &pl, &vl );
  fp = fopen( "c", "w" );
  zVD2DListFPrint( fp, &vl );
  fclose( fp );

  zVec2DListDestroy( &pl );
  zDTTri2DListDestroy( &tl );
  zVD2DListDestroy( &vl );
  return 0;
}
