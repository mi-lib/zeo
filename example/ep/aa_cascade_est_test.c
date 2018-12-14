#include <zeo/zeo_ep.h>

#define N 100000

int main(void)
{
  zEP e1, e2, e;
  zVec3D a1, a2, a;
  zMat3D m1, m2, m, ma, me;
  clock_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t0;
  register int i;

  zRandInit();
  zVec3DCreate( &a1, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &a2, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  printf( "<AA1> " ); zVec3DWrite( &a1 );
  printf( "<AA2> " ); zVec3DWrite( &a2 );

  zMat3DAA( &m1, &a1 );
  zAA2EP( &a1, &e1 );
  t1 = clock();
  for( i=0; i<N; i++ )
    zMat3DAA( &m2, &a2 );
  t2 = clock();
  for( i=0; i<N; i++ )
    zMat3DToAA( &m2, &a2 );
  t3 = clock();
  for( i=0; i<N; i++ )
    zAA2EP( &a2, &e2 );
  t4 = clock();
  for( i=0; i<N; i++ )
    zEP2AA( &e2, &a2 );
  t5 = clock();
  for( i=0; i<N; i++ )
    zAACascade( &a1, &a2, &a );
  t6 = clock();
  for( i=0; i<N; i++ )
    zEPCascade( &e1, &e2, &e );
  t7 = clock();
  for( i=0; i<N; i++ )
    zMulMatMat3D( &m2, &m1, &m );
  t8 = clock();
  for( i=0; i<N; i++ )
    zMat3DEP( &me, &e );
  t9 = clock();
  for( i=0; i<N; i++ )
    zMat3DToEP( &me, &e );
  t0 = clock();
  zMat3DAA( &ma, &a );

  printf( "+++ c.time +++\n" );
  printf( "zMat3DAA:     %ld\n", ( t2 - t1 ) / 10000 );
  printf( "zMat3DToAA:   %ld\n", ( t3 - t2 ) / 10000 );
  printf( "zAA2EP:       %ld\n", ( t4 - t3 ) / 10000 );
  printf( "zEP2AA:       %ld\n", ( t5 - t4 ) / 10000 );
  printf( "zAACascade:   %ld\n", ( t6 - t5 ) / 10000 );
  printf( "zEPCascade:   %ld\n", ( t7 - t6 ) / 10000 );
  printf( "zMulMatMat3D: %ld\n", ( t8 - t7 ) / 10000 );
  printf( "zMat3DEP:     %ld\n", ( t9 - t8 ) / 10000 );
  printf( "zMat3DToEP:   %ld\n", ( t0 - t9 ) / 10000 );
  printf( "+++ total time +++\n" );
  printf( "zMat3DAA -> zMulMatMat3D -> zMat3DToAA: %ld\n", (2*(t2-t1)+(t8-t7)+(t3-t2))/10000 );
  printf( "zAA2EP -> zMat3DEP -> zMulMatMat3D -> zMat3DToEP -> zEP2AA: %ld\n", (2*(t4-t3)+2*(t9-t8)+(t8-t7)+(t0-t9)+(t5-t4))/10000 );
  printf( "zAA2EP -> zEPCascade -> zEP2AA: %ld\n", (2*(t4-t3)+(t7-t6)+(t5-t4))/10000 );
  printf( "zAACascade: %ld\n", (t6-t5)/10000 );
  printf( "+++ assertion +++\n" );
  printf( "<M > " ); zMat3DWrite( &m );
  printf( "<Ma> " ); zMat3DWrite( &ma );
  printf( "<Me> " ); zMat3DWrite( &me );
  return 0;
}
