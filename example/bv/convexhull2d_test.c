#include <zeo/zeo_bv2d.h>

#define N 1000

void generate_points(zVec2DData *data_array, zVec2DData *data_list, int n, const char filename[])
{
  zVec2D v;
  int i;
  FILE *fp;

  zVec2DDataInitArray( data_array, n );
  zVec2DDataInitList( data_list );
  fp = fopen( filename, "w" );
  for( i=0; i<n; i++ ){
#if 0
    zVec2DCreate( &v, zRandF(-10,10), zRandF(-10,10) );
#else
    zVec2DCreatePolar( &v, zRandF(-10,10), zRandF(-zPI,zPI) );
#endif
    zVec2DValueNLFPrint( fp, &v );
    zVec2DDataAdd( data_array, &v );
    zVec2DDataAdd( data_list, &v );
  }
  fclose( fp );
}

/* output vertices of a convex hull, which are in counterclockwise order, as a closed loop. */
void output_convexhull(zPG2D *ch, const char filename[])
{
  FILE *fp;
  int i;

  fp = fopen( filename, "w" );
  for( i=0; i<zPG2DVertNum(ch); i++ )
    zVec2DValueNLFPrint( fp, zPG2DVert(ch,i) );
  zVec2DValueNLFPrint( fp, zPG2DVert(ch,0) );
  fclose( fp );
}

int main(void)
{
  zVec2DData data_array, data_list;
  zPG2D ch;

  zRandInit();
  generate_points( &data_array, &data_list, N, "v" );

  zVec2DDataConvexHull( &data_array, &ch );
  output_convexhull( &ch, "a" );
  zPG2DDestroy( &ch );

  zVec2DDataConvexHull( &data_list, &ch );
  output_convexhull( &ch, "l" );
  zPG2DDestroy( &ch );

  zVec2DDataDestroy( &data_array );
  zVec2DDataDestroy( &data_list );
  return 0;
}
