#include <zeo/zeo_bv.h>

#define N 1000

int main(void)
{
  register int i;
  zLoop3D ch;
  zLoop3DCell *vc;
  zVec3DList pl;
  zVec3D v;
  FILE *fp;

  zRandInit();
  zListInit( &pl );
  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
#if 0
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), 0 );
#else
    zVec3DCreatePolar( &v, zRandF(-10,10), 0.5*zPI, zRandF(-zPI,zPI) );
#endif
    zVec3DDataNLFWrite( fp, &v );
    zVec3DListInsert( &pl, &v );
  }
  fclose( fp );

  zCH2DPL( &ch, &pl );

  fp = fopen( "dest", "w" );
  zListForEach( &ch, vc )
    zVec3DDataNLFWrite( fp, vc->data );
  zVec3DDataNLFWrite( fp, zListTail(&ch)->data );
  fclose( fp );
  zLoop3DDestroy( &ch );
  return 0;
}
