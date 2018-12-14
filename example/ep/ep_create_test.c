#include <zeo/zeo.h>

int main(void)
{
  double angle;
  zEP ep;
  zVec3D aa;

  zRandInit();
  angle = zRandF(-zPI,zPI);
  zVec3DCreate( &aa, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );

  zEPCreate( &ep, angle, &aa );
  zVec3DNormalizeDRC( &aa );
  zVec3DMulDRC( &aa, angle );

  printf( "++ assertion ++\n" );
  printf( "org. AA: "); zVec3DWrite( &aa );
  printf( "EP:      "); zEPWrite( &ep );
  zEP2AA( &ep, &aa );
  printf( "EP-> AA: "); zVec3DWrite( &aa );
  zAA2EP( &aa, &ep );
  printf( "AA-> EP: "); zEPWrite( &ep );
  return 0;
}
