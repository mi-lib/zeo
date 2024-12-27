#include <zeo/zeo_bv2d.h>

#define N    100
#define DIV  100

int main(int argc, char *argv[])
{
  zVec2DData data;
  zVec2D p;
  zDisk2D bb;
  int i;
  FILE *fp;

  zRandInit();
  fp = fopen( "v", "w" );
  zVec2DDataInitList( &data );
  for( i=0; i<N; i++ ){
    zVec2DCreatePolar( &p, zRandF(0,10), zRandF(-zPI,zPI) );
    zVec2DDataAdd( &data, &p );
    zVec2DValueFPrint( fp, &p );
    fprintf( fp, "\n" );
  }
  fclose( fp );
  zVec2DDataBoundingBall( &data, &bb, NULL );
  fp = fopen( "d", "w" );
  for( i=0; i<=DIV; i++ ){
    fprintf( fp, "%.10g %.10g\n",
      zDisk2DRadius(&bb)*cos(zPIx2*(double)i/DIV)+zDisk2DCenter(&bb)->c.x,
      zDisk2DRadius(&bb)*sin(zPIx2*(double)i/DIV)+zDisk2DCenter(&bb)->c.y );
  }
  fclose( fp );

  zVec2DDataDestroy( &data );
  return 0;
}
