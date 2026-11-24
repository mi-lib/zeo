#include <zeo/zeo_bv2d.h>

#define N 100

int main(void)
{
  zVec2D p, cp;
  zVec2D v[N];
  zVec2DData data;
  zPG2D ch;
  FILE *fp;
  int i;

  zRandInit();
  fp = fopen( "v", "w" );
  for( i=0; i<N; i++ ){
    zVec2DCreatePolar( &v[i], zRandF(-10,10), zRandF(-zPI,zPI) );
    zVec2DValueNLFPrint( fp, &v[i] );
  }
  fclose( fp );
  zVec2DDataAssignArrayDirect( &data, v, N );
  zVec2DDataConvexHull( &data, &ch );
  zVec2DDataDestroy( &data );

  /* vertices of the convex hull in counterclockwise order */
  fp = fopen( "c", "w" );
  for( i=0; i<zPG2DVertNum(&ch); i++ )
    zVec2DValueNLFPrint( fp, zPG2DVert(&ch,i) );
  zVec2DValueNLFPrint( fp, zPG2DVert(&ch,0) );
  fclose( fp );

  zVec2DCreate( &p, zRandF(-12,12), zRandF(-12,12) );
  zPG2DClosest( &ch, &p, &cp );
  fp = fopen( "e", "w" );
  zVec2DValueNLFPrint( fp, &p );
  zVec2DValueNLFPrint( fp, &cp );
  fclose( fp );

  zPG2DDestroy( &ch );
  return 0;
}
