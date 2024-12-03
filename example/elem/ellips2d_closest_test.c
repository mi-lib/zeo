#include <zeo/zeo_elem2d.h>

#define N 1000

int main(int argc, char *argv[])
{
  zEllips2D e;
  zVec2D p, c, cp;
  int i;
  double margin;
  FILE *fp1, *fp2;

  zRandInit();
  zVec2DCreate( &c, zRandF(-1,1), zRandF(-1,1) );
  zEllips2DCreate( &e, &c, zRandF(1,3), zRandF(1,3) );
  margin = argc > 1 ? atof( argv[1] ) : zTOL;
  fp1 = fopen( "pi", "w" );
  fp2 = fopen( "po", "w" );
  for( i=0; i<N; i++ ){
    zVec2DCreate( &p, zRandF(-4,4), zRandF(-4,4) );
    if( zEllips2DPointIsInside( &e, &p, margin ) ){
      zVec2DValueNLFPrint( fp1, &p );
    } else{
      zEllips2DClosest( &e, &p, &cp );
      zVec2DValueNLFPrint( fp2, &p );
      zVec2DValueNLFPrint( fp2, &cp );
      zFEndl( fp2 );
      zFEndl( fp2 );
    }
  }
  fclose( fp1 );
  fclose( fp2 );
  return 0;
}
