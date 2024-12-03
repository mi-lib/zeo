#include <zeo/zeo_elem2d.h>

#define N 1000
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zVec2D v1, v2, v3, v;
  zTri2D t;
  double margin;
  int i;
  FILE *fp1, *fp2;

  zRandInit();
  fp1 = fopen( "t", "w" );
  zVec2DCreate( &v1, -2, -3 ); zVec2DValueNLFPrint( fp1, &v1 );
  zVec2DCreate( &v2,  4, -1 ); zVec2DValueNLFPrint( fp1, &v2 );
  zVec2DCreate( &v3,  1,  5 ); zVec2DValueNLFPrint( fp1, &v3 );
  zVec2DValueNLFPrint( fp1, &v1 );
  fclose( fp1 );

  zTri2DCreate( &t, &v1, &v2, &v3 );
  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  fp1 = fopen( "pi", "w" );
  fp2 = fopen( "po", "w" );
  for( i=0; i<N; i++ ){
    zVec2DCreate( &v, zRandF(-10,10), zRandF(-10,10) );
    if( zTri2DPointIsInside( &t, &v, margin ) )
      zVec2DValueNLFPrint( fp1, &v );
    else
      zVec2DValueNLFPrint( fp2, &v );
  }
  fclose( fp1 );
  fclose( fp2 );
  return 0;
}
