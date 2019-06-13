#include <zeo/zeo.h>

int main(void)
{
  zVec3D v[3], p;
  zEdge3D edge;
  zTri3D t;
  double d;

  zVec3DCreate( &v[0], 0, 0, 0 );
  zVec3DCreate( &v[1], 1, 0, 0 );
  zVec3DCreate( &v[2], 0, 1, 0 );

  zEdge3DCreate( &edge, &v[0], &v[1] );
  zTri3DCreate( &t, &v[0], &v[1], &v[2] );

  printf( "[contiguous vertex of edge]\n" );
  do{
    zEdge3DPrint( &edge );
    printf( "enter three values> " );
    if( scanf( "%lf %lf %lf", &p.e[0], &p.e[1], &p.e[2] ) != 3 ) break;
    printf( "(entered point): " );
    zVec3DPrint( &p );
    printf( "contiguous point: " );
    zVec3DPrint( zEdge3DContigVert( &edge, &p, &d ) );
    printf( "( distance = %g )\n", d );
  } while( !zVec3DIsTiny( &p ) );
  printf( "[contiguous vertex of triangle]\n" );
  do{
    zTri3DPrint( &t );
    printf( "enter three values> " );
    if( scanf( "%lf %lf %lf", &p.e[0], &p.e[1], &p.e[2] ) != 3 ) break;
    printf( "(entered point): " );
    zVec3DPrint( &p );
    printf( "contiguous point: " );
    zVec3DPrint( zTri3DContigVert( &t, &p, &d ) );
    printf( "( distance = %g )\n", d );
  } while( !zVec3DIsTiny( &p ) );
  return 0;
}
