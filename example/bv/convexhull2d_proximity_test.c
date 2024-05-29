#include <zeo/zeo_bv2d.h>

#define N 100

int main(void)
{
  zVec3D p, cp;
  zVec3D v[N];
  zLoop3D ch;
  zLoop3DCell *vc;
  FILE *fp;
  int i;

  zRandInit();
  fp = fopen( "v", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreatePolar( &v[i], zRandF(-10,10), 0.5*zPI, zRandF(-zPI,zPI) );
    zVec3DDataNLFPrint( fp, &v[i] );
  }
  fclose( fp );
  zConvexHull2D( &ch, v, N );

  fp = fopen( "c", "w" );
  zListForEach( &ch, vc )
    zVec3DDataNLFPrint( fp, vc->data );
  zVec3DDataNLFPrint( fp, zListTail(&ch)->data );
  fclose( fp );

  zVec3DCreate( &p, zRandF(-12,12), zRandF(-12,12), 0 );
  zConvexHull2DClosest( &ch, &p, &cp );
  fp = fopen( "e", "w" );
  zVec3DDataNLFPrint( fp, &p );
  zVec3DDataNLFPrint( fp, &cp );
  fclose( fp );

  zLoop3DDestroy( &ch );
  return 0;
}
