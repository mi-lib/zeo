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
    zVec3DValueNLFPrint( fp, &v[i] );
  }
  fclose( fp );
  zConvexHull2D( &ch, v, N );

  fp = fopen( "c", "w" );
  zListForEach( &ch, vc )
    zVec3DValueNLFPrint( fp, vc->data );
  zVec3DValueNLFPrint( fp, zListTail(&ch)->data );
  fclose( fp );

  zVec3DCreate( &p, zRandF(-12,12), zRandF(-12,12), 0 );
  zConvexHull2DClosest( &ch, &p, &cp );
  fp = fopen( "e", "w" );
  zVec3DValueNLFPrint( fp, &p );
  zVec3DValueNLFPrint( fp, &cp );
  fclose( fp );

  zLoop3DDestroy( &ch );
  return 0;
}
