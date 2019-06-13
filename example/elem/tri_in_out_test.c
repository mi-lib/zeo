#include <zeo/zeo.h>

int main(void)
{
  zVec3D v[3], p;
  zTri3D t;

#if 1
  zVec3DCreate( &v[0], 0, 0, 0 );
  zVec3DCreate( &v[1], 1, 0, 0 );
  zVec3DCreate( &v[2], 0, 1, 0 );
#else
  zVec3DCreate( &v[0],-1, 0,-1 );
  zVec3DCreate( &v[1],-1,-1, 0 );
  zVec3DCreate( &v[2],-1,-1, 1 );
#endif
  zTri3DCreate( &t, &v[0], &v[1], &v[2] );
  printf( "<polygon>\n" );
  zTri3DPrint( &t );

#if 1
  zVec3DCreate( &p, -2, 0, 0 );
#else
  zVec3DCreate( &p,-1,-1, 0 );
#endif

  printf( "\n+++ the nearest point detection test (polygon) +++\n" );
  printf( "point -> " ); zVec3DPrint( &p );
  printf( " (%s when accepting the rim)\n", zTri3DPointIsInside(&t,&p,true) ? "inside" : "outside" );
  printf( " (%s when unaccepting the rim)\n", zTri3DPointIsInside(&t,&p,false) ? "inside" : "outside" );
  return 0;
}
