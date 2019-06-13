#include <zeo/zeo_elem.h>

#define N  1000
#define NN  100

void print_arrow(FILE *fp, zVec3D *p1, zVec3D *p2)
{
  fprintf( fp, "set arrow from %.10g, %.10g, %.10g to %.10g, %.10g, %.10g\n",
    p1->e[zX], p1->e[zY], p1->e[zZ], p2->e[zX], p2->e[zY], p2->e[zZ] );
}

int main(void)
{
  zVec3D v1, v2, p, v;
  zEdge3D e;
  int i;
  FILE *fp[3];

  zRandInit();
  zVec3DCreate( &v1, zRandF(0,1), zRandF(0,1), 0 );
  zVec3DCreate( &v2, zRandF(0,1), zRandF(0,1), 0 );
  zEdge3DCreate( &e, &v1, &v2 );
  fp[0] = fopen( "e", "w" );
  zVec3DDataNLFPrint( fp[0], &v1 );
  zVec3DDataNLFPrint( fp[0], &v2 );
  fclose( fp[0] );

  fp[0] = fopen( "d", "w" );
  fp[1] = fopen( "l", "w" );
  fp[2] = fopen( "a", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p,  zRandF(0,1), zRandF(0,1), 0 );
    if( zEdge3DPointIsOn( &e, &p ) )
      zVec3DDataNLFPrint( fp[1], &p );
    zEdge3DClosest( &e, &p, &v );
    p.e[zZ] = zEdge3DPointDist( &e, &p );
    zVec3DDataNLFPrint( fp[0], &p );
    print_arrow( fp[2], &p, &v );
  }
  for( i=0; i<NN; i++ ){
    zVec3DMul( zEdge3DVec(&e), zRandF(-1.5,1.5), &p );
    zVec3DAddDRC( &p, zEdge3DVert(&e,0) );
    if( zEdge3DPointIsOn( &e, &p ) )
      zVec3DDataNLFPrint( fp[1], &p );
    zEdge3DClosest( &e, &p, &v );
    p.e[zZ] = zEdge3DPointDist( &e, &p );
    zVec3DDataNLFPrint( fp[0], &p );
    print_arrow( fp[2], &p, &v );
  }
  fclose( fp[0] );
  fclose( fp[1] );
  fclose( fp[2] );
  return 0;
}
