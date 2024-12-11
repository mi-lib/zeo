#include <zeo/zeo_bv2d.h>

#define N 1000

void generate_points(zVec3DData *data_array, zVec3DData *data_list, int n, const char filename[])
{
  zVec3D v;
  int i;
  FILE *fp;

  zVec3DDataInitArray( data_array, n );
  zVec3DDataInitList( data_list );
  fp = fopen( filename, "w" );
  for( i=0; i<n; i++ ){
#if 0
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), 0 );
#else
    zVec3DCreatePolar( &v, zRandF(-10,10), 0.5*zPI, zRandF(-zPI,zPI) );
#endif
    zVec3DValueNLFPrint( fp, &v );
    zVec3DDataAdd( data_array, &v );
    zVec3DDataAdd( data_list, &v );
  }
  fclose( fp );
}

void output_convexhull(zLoop3D *ch, const char filename[])
{
  FILE *fp;
  zLoop3DCell *vc;

  fp = fopen( filename, "w" );
  zListForEach( ch, vc )
    zVec3DValueNLFPrint( fp, vc->data );
  zVec3DValueNLFPrint( fp, zListTail(ch)->data );
  fclose( fp );
}

void output_convexhull_ph(zPH3D *ph, const char filename[])
{
  FILE *fp;
  int i;

  fp = fopen( filename, "w" );
  for( i=0; i<zPH3DVertNum(ph); i++ )
    zVec3DValueNLFPrint( fp, zPH3DVert(ph,i) );
  zVec3DValueNLFPrint( fp, zPH3DVert(ph,0) );
  fclose( fp );
}

int main(void)
{
  zVec3DData data_array, data_list;
  zLoop3D ch;
  zPH3D ph;

  zRandInit();
  generate_points( &data_array, &data_list, N, "v" );

  zVec3DDataConvexHull2D( &data_array, &ch );
  output_convexhull( &ch, "a" );
  zLoop3DDestroy( &ch );

  zVec3DDataConvexHull2D( &data_list, &ch );
  output_convexhull( &ch, "l" );
  zLoop3DDestroy( &ch );

  zVec3DDataConvexHull2DPH3D( &data_array, &ph );
  output_convexhull_ph( &ph, "h" );

  zPH3DDestroy( &ph );
  zVec3DDataDestroy( &data_array );
  zVec3DDataDestroy( &data_list );
  return 0;
}
