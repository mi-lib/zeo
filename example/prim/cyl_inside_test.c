#include <zeo/zeo_prim.h>

#define N 100000

int main(void)
{
  zCyl3D cyl;
  zVec3D c1, c2, p;
  int i;

  zRandInit();
  zVec3DCreate( &c1, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zVec3DCreate( &c2, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zCyl3DCreate( &cyl, &c1, &c2, zRandF(0,0.5), 0 );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(-1.5,1.5), zRandF(-1.5,1.5), zRandF(-1.5,1.5) );
    if( zCyl3DPointIsInside( &cyl, &p, true ) )
      zVec3DDataNLPrint( &p );
  }
  return 0;
}
