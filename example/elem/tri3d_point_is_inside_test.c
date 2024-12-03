#include <zeo/zeo_elem3d.h>

#define N 1000
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zVec3D v1, v2, v3, v;
  zTri3D t;
  double margin;
  int i;
  FILE *fp1, *fp2;

  zRandInit();
  fp1 = fopen( "t", "w" );
  zVec3DCreate( &v1, -4, -5, 1 ); zVec3DValueNLFPrint( fp1, &v1 );
  zVec3DCreate( &v2, -1,  6, 1 ); zVec3DValueNLFPrint( fp1, &v2 );
  zVec3DCreate( &v3,  6, -1, 1 ); zVec3DValueNLFPrint( fp1, &v3 );
  zVec3DValueNLFPrint( fp1, &v1 );
  fclose( fp1 );

  zTri3DCreate( &t, &v1, &v2, &v3 );
  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  fp1 = fopen( "pi", "w" );
  fp2 = fopen( "po", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    if( zTri3DPointIsInside( &t, &v, margin ) )
      zVec3DValueNLFPrint( fp1, &v );
    else
      zVec3DValueNLFPrint( fp2, &v );
  }
  fclose( fp1 );
  fclose( fp2 );
  return 0;
}
