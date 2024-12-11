#include <zeo/zeo_voronoi2d.h>

void generate_points(FILE *fp, zVec2DData *data, int n)
{
  zVec2D p;
  int i;

  zVec2DDataInitList( data );
  for( i=0; i<n; i++ ){
#if 1
    zVec2DCreate( &p, zRandF(-10,10), zRandF(-10,10) );
#else
    zVec2DCreate( &p, i / 10, i % 10 );
#endif
    zVec2DDataAdd( data, &p );
    zVec2DValueNLFPrint( fp, &p );
  }
}

#define N 100

int main(int argc, char *argv[])
{
  zVec2DData data;
  zDelaunayTri2DList tl;
  zVoronoi2DList vl;
  FILE *fp;
  int n;

  n = argc > 1 ? atoi( argv[1] ) : N;
  zRandInit();
  fp = fopen( "v", "w" );
  generate_points( fp, &data, n );
  fclose( fp );

  zVec2DDataDelaunayTriangulate( &data, &tl );
  fp = fopen( "t", "w" );
  zDelaunayTri2DListFPrint( fp, &tl );
  fclose( fp );

  zVec2DDataVoronoiDiagram( &data, &vl );
  fp = fopen( "c", "w" );
  zVoronoi2DListFPrint( fp, &vl );
  fclose( fp );

  zVec2DDataDestroy( &data );
  zDelaunayTri2DListDestroy( &tl );
  zVoronoi2DListDestroy( &vl );
  return 0;
}
