#include <zeo/zeo_bv.h>

#define N 1000
zVec3D v[N];

int main(void)
{
  register int i;
  zLoop3D ch;
  zLoop3DCell *vc;
  FILE *fp;

  zRandInit();
  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
#if 0
    zVec3DCreate( &v[i], zRandF(-10,10), zRandF(-10,10), 0 );
#else
    zVec3DCreatePolar( &v[i], zRandF(-10,10), 0.5*zPI, zRandF(-zPI,zPI) );
#endif
    zVec3DDataNLFPrint( fp, &v[i] );
  }
  fclose( fp );

  zCH2D( &ch, v, N );

  fp = fopen( "dest", "w" );
  zListForEach( &ch, vc )
    zVec3DDataNLFPrint( fp, vc->data );
  zVec3DDataNLFPrint( fp, zListTail(&ch)->data );
  fclose( fp );
  zLoop3DDestroy( &ch );
  return 0;
}
