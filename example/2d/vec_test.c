#include <zeo/zeo_vec2d.h>

int main(void)
{
  zVec2D v1, v2, v;

  zVec2DCreate( v1, 1, 2 );
  zVec2DCreate( v2, -1, 2 );
  printf( "v1: " ); zVec2DWrite( v1 );
  printf( "v2: " ); zVec2DWrite( v2 );

  zVec2DAdd( v1, v2, v );
  printf( "v1+v2= " ); zVec2DWrite( v );
  zVec2DSub( v1, v2, v );
  printf( "v1-v2= " ); zVec2DWrite( v );
  zVec2DRev( v1, v );
  printf( "-v1= " ); zVec2DWrite( v );
  zVec2DMul( v1, 2, v );
  printf( "v1*2= " ); zVec2DWrite( v );
  zVec2DDiv( v1, 2, v );
  printf( "v1/2= " ); zVec2DWrite( v );
  zVec2DCat( v1, 2, v2, v );
  printf( "v1+2*v2= " ); zVec2DWrite( v );

  printf( "|v1|= %f\n", zVec2DNorm(v1) );
  printf( "|v1-v2|= %f\n", zVec2DDist(v1,v2) );
  zVec2DNormalize( v1, v );
  printf( "v1/|v1|= " ); zVec2DWrite( v );
  printf( "(|v|= %f)\n", zVec2DNorm(v) );

  printf( "v1.v2= %f\n", zVec2DInnerProd(v1,v2) );
  printf( "|v1xv2|= %f\n", zVec2DOuterProd(v1,v2) );

  return 0;
}
