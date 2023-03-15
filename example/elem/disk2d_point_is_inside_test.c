#include <zeo/zeo_elem2d.h>

#define N 1000
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zVec2D c, v;
  zDisk2D d;
  double margin;
  int i;
  FILE *fp1, *fp2;

  zRandInit();
  zVec2DCreate( &c, 1, 1 );
  zDisk2DCreate( &d, &c, 2 );

  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  fp1 = fopen( "pi", "w" );
  fp2 = fopen( "po", "w" );
  for( i=0; i<N; i++ ){
    zVec2DCreate( &v, zRandF(-5,5), zRandF(-5,5) );
    if( zDisk2DPointIsInside( &d, &v, margin ) )
      zVec2DDataNLFPrint( fp1, &v );
    else
      zVec2DDataNLFPrint( fp2, &v );
  }
  fclose( fp1 );
  fclose( fp2 );
  return 0;
}
