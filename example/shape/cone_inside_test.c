#include <zeo/zeo_shape.h>

#define N 100000

int main(void)
{
  zCone3D con;
  zVec3D c1, c2, p;
  int i;

  zRandInit();
  zVec3DCreate( &c1, zRandF(-0.1,0.1), zRandF(-0.1,0.1), zRandF(-0.1,0.1) );
  zVec3DCreate( &c2, zRandF(-0.1,0.1), zRandF(-0.1,0.1), zRandF(-0.1,0.1)+0.5 );
  zCone3DCreate( &con, &c1, &c2, zRandF(0,0.5), 0 );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    if( zCone3DPointIsInside( &con, &p, true ) )
      zVec3DDataNLPrint( &p );
  }
  return 0;
}
