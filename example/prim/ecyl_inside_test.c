#include <zeo/zeo_prim.h>

#define N 10000

int main(void)
{
  zECyl3D ecyl;
  zVec3D c[2];
  zVec3D p;
  int i;

  zRandInit();
  zVec3DCreate( &c[0],-0.05,-0.05,-0.05 );
  zVec3DCreate( &c[1], 0.05, 0.05, 0.05 );
  zECyl3DCreate( &ecyl, &c[0], &c[1], 0.06, 0.1, ZVEC3DX, 0 );

  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
    if( zECyl3DPointIsInside( &ecyl, &p, true ) )
      zVec3DDataNLPrint( &p );
  }
  return 0;
}
