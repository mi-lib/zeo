#include <zeo/zeo_ep.h>

#define N 10000

int main(void)
{
  zMat3D m1, m2, m;
  zVec3D e1, e2, e, em;
  double t;
  int i;
#if 0
  FILE *fp[2];
#endif

  zRandInit();
  zMat3DFromZYX( &m1, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DFromZYX( &m2, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DToAA( &m1, &e1 );
  zMat3DToAA( &m2, &e2 );
#if 0
  fp[0] = fopen( "slerp.zvs", "w" );
  fp[1] = fopen( "vecip.zvs", "w" );
#endif
  for( i=0; i<=N; i++ ){
    t = (double)i / N;
    zMat3DInterDiv( &m1, &m2, t, &m );
    zMat3DToAA( &m, &e );
    zVec3DInterDiv( &e1, &e2, t, &em );
    printf( "%f %f %f %f ",  e1.e[zX], e2.e[zX], e.e[zX], em.e[zX] );
    printf( "%f %f %f %f ",  e1.e[zY], e2.e[zY], e.e[zY], em.e[zY] );
    printf( "%f %f %f %f\n", e1.e[zZ], e2.e[zZ], e.e[zZ], em.e[zZ] );
#if 0
    fprintf( fp[0], "0.1 3 " ); zVec3DValueNLFPrint(fp[0],&e);
    fprintf( fp[1], "0.1 3 " ); zVec3DValueNLFPrint(fp[1],&em);
#endif
  }
#if 0
  fclose( fp[0] );
  fclose( fp[1] );
#endif
  return 0;
}
