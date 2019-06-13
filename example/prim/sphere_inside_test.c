#include <zeo/zeo_prim.h>

#define N 10000

int main(void)
{
  zSphere3D sphere;
  zVec3D center, p;
  int i;

  zRandInit();
  zVec3DCreate( &center, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zSphere3DCreate( &sphere, &center, zRandF(0,0.5), 0 );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    if( zSphere3DPointIsInside( &sphere, &p, true ) )
      zVec3DDataNLPrint( &p );
  }
  return 0;
}
