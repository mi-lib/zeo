#include <zeo/zeo.h>

#define N 1000000

int main(void)
{
  register int i;
  zEP ep;
  zMat3D m;
  zVec3D aa, v, v1, v2, v3;
  clock_t t1, t2, t3, t4;

  zRandInit();
  zVec3DCreate( &aa, zRandF(-1.0,1.0), zRandF(-1.0,1.0), zRandF(-1.0,1.0) );
  zAA2EP( &aa, &ep );
  zMat3DAA( &m, &aa );
  zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

  t1 = clock();
  for( i=0; i<N; i++ )
    zVec3DRot( &v, &aa, &v1 );
  t2 = clock();
  for( i=0; i<N; i++ )
    zEPRotVec( &ep, &v, &v2 );
  t3 = clock();
  for( i=0; i<N; i++ )
    zMulMatVec3D( &m, &v, &v3 );
  t4 = clock();
  printf( "+++ c.time +++\n" );
  printf( "zVec3DRot:    %ld\n", t2 - t1 );
  printf( "zEPRotVec:    %ld\n", t3 - t2 );
  printf( "zMulMatVec3D: %ld\n", t4 - t3 );
  printf( "+++ assertion +++\n" );
  zVec3DWrite( &v1 );
  zVec3DWrite( &v2 );
  zVec3DWrite( &v3 );

  return 0;
}
