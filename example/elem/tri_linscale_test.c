#include <zeo/zeo_elem.h>

void assertion(zTri3D *t, zVec3D *cp, double l0, double l1, double l2, double dist)
{
  zVec3D p;

  printf( "cp: " ); zVec3DPrint( cp );
  printf( "(l0:l1:l2)=(%g:%g:%g), ||p-cp|| = %g\n", l0, l1, l2, dist );
  zVec3DMul( zTri3DVert(t,0), l0, &p );
  zVec3DCatDRC( &p, l1, zTri3DVert(t,1) );
  zVec3DCatDRC( &p, l2, zTri3DVert(t,2) );
  printf( "+assertion+\n" );
  zVec3DPrint( &p );
}

int main(void)
{
  zVec3D v0, v1, v2, p, cp;
  zTri3D t;
  double r, l0, l1, l2;

  zVec3DCreate( &v0, 2.0, 0, 0 );
  zVec3DCreate( &v1, 0, 3.0, 0 );
  zVec3DCreate( &v2, 0, 0, zTOL );
  zTri3DCreate( &t, &v0, &v1, &v2 );

  zRandInit();
  zVec3DCreate( &p, zRandF(-1,3), zRandF(-1,4), 0.5*zRandF(-zTOL,zTOL) );
/*
  zVec3DCreate( &p, 0.1, 0.1, 0 );
  zVec3DCreate( &p, 2.0,-0.1, 0 );
  zVec3DCreate( &p, 1.0, 1.0, 0 );
  zVec3DCreate( &p,-0.1, 2.0, 0 );
  zVec3DCreate( &p,-0.1, 0.1, 0 );
  zVec3DCreate( &p,-0.1,-0.1, 0 );
  zVec3DCreate( &p, 0.1,-0.1, 0 );
*/
  printf( "v0: " ); zVec3DPrint( &v0 );
  printf( "v1: " ); zVec3DPrint( &v1 );
  printf( "v2: " ); zVec3DPrint( &v2 );
  printf( "p : " ); zVec3DPrint( &p  );

  r = zTri3DLinScale( &t, &p, &l0, &l1, &l2, &cp );
  assertion( &t, &cp, l0, l1, l2, r );
  return 0;
}
