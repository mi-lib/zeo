#include <zeo/zeo_bv2d.h>

#define N  10000
#define DIV  100

int main(int argc, char *argv[])
{
  zVec2DList pl;
  zVec2D p;
  zDisk2D bd;
  register int i;
  FILE *fp;

  zRandInit();
  fp = fopen( "v", "w" );
  zListInit( &pl );
  for( i=0; i<N; i++ ){
    zVec2DCreatePolar( &p, zRandF(0,10), zRandF(-zPI,zPI) );
    zVec2DListAdd( &pl, &p );
    zVec2DValueFPrint( fp, &p );
    fprintf( fp, "\n" );
  }
  fclose( fp );
  zBoundingDisk2DPL( &bd, &pl, NULL );
  fp = fopen( "d", "w" );
  for( i=0; i<=DIV; i++ ){
    fprintf( fp, "%.10g %.10g\n",
      zDisk2DRadius(&bd)*cos(zPIx2*(double)i/DIV)+zDisk2DCenter(&bd)->c.x,
      zDisk2DRadius(&bd)*sin(zPIx2*(double)i/DIV)+zDisk2DCenter(&bd)->c.y );
  }
  fclose( fp );

  zVec2DListDestroy( &pl );
  return 0;
}
