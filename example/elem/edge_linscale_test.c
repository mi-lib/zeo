#include <zeo/zeo_elem.h>

void assertion(zEdge3D *e, zVec3D *cp, double l0, double l1, double dist)
{
  zVec3D p;

  printf( "cp: " ); zVec3DWrite( cp );
  printf( "(l0:l1)=(%g:%g), ||p-cp|| = %g\n", l0, l1, dist );
  zVec3DMul( zEdge3DVert(e,0), l0, &p );
  zVec3DCatDRC( &p, l1, zEdge3DVert(e,1) );
  printf( "+assertion+\n" );
  printf( "cp: " ); zVec3DWrite( &p );
}

int main(void)
{
  zVec3D v0, v1, p, cp;
  zEdge3D e;
  double r, l0, l1;

  zVec3DCreate( &v0, 0, 0, 0 );
  zVec3DCreate( &v1, 2, 0, 0 );
  zEdge3DCreate( &e, &v0, &v1 );

  zRandInit();
  zVec3DCreate( &p, zRandF(-1,3), 0, 0 );
/*
  zVec3DCreate( &p, 2+0.1*zTOL, 0, 0 );
*/
  printf( "v0: " ); zVec3DWrite( &v0 );
  printf( "v1: " ); zVec3DWrite( &v1 );
  printf( "p : " ); zVec3DWrite( &p  );
  r = zEdge3DLinScale( &e, &p, &l0, &l1, &cp );
  assertion( &e, &cp, l0, l1, r );
  return 0;
}
