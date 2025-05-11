#include <zeo/zeo_ep.h>

#define N 100000

int main(void)
{
  zEP e1, e2, e;
  zVec3D a1, a2, a;
  zMat3D m1, m2, m, ma, me;
  clock_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t0;
  int i;

  zRandInit();
  zVec3DCreate( &a1, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &a2, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  printf( "<AA1> " ); zVec3DPrint( &a1 );
  printf( "<AA2> " ); zVec3DPrint( &a2 );

  zMat3DFromAA( &m1, &a1 );
  zAA2EP( &a1, &e1 );
  t1 = clock();
  for( i=0; i<N; i++ )
    zMat3DFromAA( &m2, &a2 );
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
    zMulMat3DMat3D( &m2, &m1, &m );
  t8 = clock();
  for( i=0; i<N; i++ )
    zMat3DFromEP( &me, &e );
  t9 = clock();
  for( i=0; i<N; i++ )
    zMat3DToEP( &me, &e );
  t0 = clock();
  zMat3DFromAA( &ma, &a );

  printf( "+++ c.time +++\n" );
  printf( "zMat3DFromAA:   %ld\n", t2 - t1 );
  printf( "zMat3DToAA:     %ld\n", t3 - t2 );
  printf( "zAA2EP:         %ld\n", t4 - t3 );
  printf( "zEP2AA:         %ld\n", t5 - t4 );
  printf( "zAACascade:     %ld\n", t6 - t5 );
  printf( "zEPCascade:     %ld\n", t7 - t6 );
  printf( "zMulMat3DMat3D: %ld\n", t8 - t7 );
  printf( "zMat3DFromEP:   %ld\n", t9 - t8 );
  printf( "zMat3DToEP:     %ld\n", t0 - t9 );
  printf( "+++ total time +++\n" );
  printf( "zMat3DFromAA -> zMulMat3DMat3D -> zMat3DToAA: %ld\n", (2*(t2-t1)+(t8-t7)+(t3-t2)) );
  printf( "zAA2EP -> zMat3DFromEP -> zMulMat3DMat3D -> zMat3DToEP -> zEP2AA: %ld\n", (2*(t4-t3)+2*(t9-t8)+(t8-t7)+(t0-t9)+(t5-t4)) );
  printf( "zAA2EP -> zEPCascade -> zEP2AA: %ld\n", (2*(t4-t3)+(t7-t6)+(t5-t4)) );
  printf( "zAACascade: %ld\n", (t6-t5)/10000 );
  return 0;
}
