#include <zeo/zeo_prim.h>

#define N 10000

int main(void)
{
  zEllips3D el;
  zVec3D center, ax, ay, az, tmp;
  zVec3D p;
  int i;

  zRandInit();
  zVec3DCreate( &center, 0.1, 0.2, 0.3 );
  zVec3DCreate( &ax, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DNormalizeDRC( &ax );
  zVec3DCreate( &tmp, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DOrthogonalize( &tmp, &ax, &ay );
  zVec3DNormalizeDRC( &ay );
  zVec3DOuterProd( &ax, &ay, &az );
  zEllips3DCreate( &el, &center, &ax, &ay, &az, zRandF(0,1), zRandF(0,1), zRandF(0,1), 0 );

  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    if( zEllips3DPointIsInside( &el, &p, true ) )
      zVec3DDataNLWrite( &p );
  }
  return 0;
}
