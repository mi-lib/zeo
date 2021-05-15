#include <zeo/zeo_delaunay2d.h>

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
  FILE *fp;

  zRandInit();
  fp = fopen( "v", "w" );
  generate_points( fp, &pl, N );
  fclose( fp );

  zDelaunay2D( &pl, &tl );
  fp = fopen( "t", "w" );
  zDTTri2DListFPrint( fp, &tl );
  fclose( fp );

  zVec2DListDestroy( &pl );
  zListDestroy( zDTTri2DListCell, &tl );
  return 0;
}
