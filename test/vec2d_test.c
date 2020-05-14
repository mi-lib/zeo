#include <zeo/zeo.h>

void assert_create(void)
{
  zVec2D v1, v2;

  zVec2DCreate( &v1, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreatePolar( &v2, zVec2DNorm(&v1), atan2(v1.c.y,v1.c.x) );
  zAssert( zVec2DCreatePolar, zVec2DEqual( &v1, &v2 ) );
}

void assert_arith(void)
{
  zVec2D v1, v2, v3;
  double k;

  zVec2DCreate( &v1, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( &v1, zRandF(-10,10), zRandF(-10,10) );
  k = zRandF(-10,10);
  zVec2DAdd( &v1, &v2, &v3 );
  zAssert( zVec2DAdd, v1.c.x+v2.c.x==v3.c.x && v1.c.y+v2.c.y==v3.c.y );
  zVec2DSub( &v1, &v2, &v3 );
  zAssert( zVec2DSub, v1.c.x-v2.c.x==v3.c.x && v1.c.y-v2.c.y==v3.c.y );
  zVec2DRev( &v1, &v3 );
  zAssert( zVec2DRev, v1.c.x+v3.c.x==0 && v1.c.y+v3.c.y==0 );
  zVec2DMul( &v1, k, &v3 );
  zAssert( zVec2DMul, v1.c.x*k==v3.c.x && v1.c.y*k==v3.c.y );
  zVec2DDiv( &v1, k, &v3 );
  zAssert( zVec2DDiv, zIsTiny(v1.c.x/k-v3.c.x) && zIsTiny(v1.c.y/k-v3.c.y) );
  zVec2DCat( &v1, k, &v2, &v3 );
  zAssert( zVec2DCat, v1.c.x+k*v2.c.x==v3.c.x && v1.c.y+k*v2.c.y==v3.c.y );

  zVec2DCopy( &v1, &v3 ); zVec2DAddDRC( &v3, &v2 );
  zAssert( zVec2DAddDRC, v1.c.x+v2.c.x==v3.c.x && v1.c.y+v2.c.y==v3.c.y );
  zVec2DCopy( &v1, &v3 ); zVec2DSubDRC( &v3, &v2 );
  zAssert( zVec2DSubDRC, v1.c.x-v2.c.x==v3.c.x && v1.c.y-v2.c.y==v3.c.y );
  zVec2DCopy( &v1, &v3 ); zVec2DRevDRC( &v3 );
  zAssert( zVec2DRevDRC, v1.c.x+v3.c.x==0 && v1.c.y+v3.c.y==0 );
  zVec2DCopy( &v1, &v3 ); zVec2DMulDRC( &v3, k );
  zAssert( zVec2DMulDRC, v1.c.x*k==v3.c.x && v1.c.y*k==v3.c.y );
  zVec2DCopy( &v1, &v3 ); zVec2DDivDRC( &v3, k );
  zAssert( zVec2DDivDRC, zIsTiny(v1.c.x/k-v3.c.x) && zIsTiny(v1.c.y/k-v3.c.y) );
  zVec2DCopy( &v1, &v3 ); zVec2DCatDRC( &v3, k, &v2 );
  zAssert( zVec2DCatDRC, v1.c.x+k*v2.c.x==v3.c.x && v1.c.y+k*v2.c.y==v3.c.y );
}

void assert_vecprod(void)
{
  zVec2D v1, v2;

  zVec2DCreate( &v1, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( &v2, zRandF(-10,10), zRandF(-10,10) );
  zAssert( zVec2DInnerProd, v1.c.x*v2.c.x+v1.c.y*v2.c.y == zVec2DInnerProd(&v1,&v2) );
  zAssert( zVec2DOuterProd, v1.c.x*v2.c.y-v1.c.y*v2.c.x == zVec2DOuterProd(&v1,&v2) );

  zVec2DNormalize( &v1, &v2 );
  zAssert( zVec2DNormalize, zIsTiny(v2.c.x*v2.c.x+v2.c.y*v2.c.y-1) );
  zVec2DNormalizeDRC( &v1 );
  zAssert( zVec2DNormalizeDRC, zIsTiny(v1.c.x*v1.c.x+v1.c.y*v1.c.y-1) );
}

void assert_geometry(void)
{
  zVec2D v1, v2, v3;
  double angle, r, r1, r2;

  zVec2DCreate( &v1, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( &v2, zRandF(-10,10), zRandF(-10,10) );
  r = zRandF(0,1);
  angle = zRandF(-zPI,zPI);
  zVec2DInterDiv( &v1, &v2, r, &v3 );
  r1 = zVec2DDist( &v1, &v3 );
  r2 = zVec2DDist( &v2, &v3 );
  zAssert( zVec2DInterDiv, zIsTiny( r - r1 / ( r1 + r2 ) ) );
  zVec2DMid( &v1, &v2, &v3 );
  r1 = zVec2DDist( &v1, &v3 );
  r2 = zVec2DDist( &v2, &v3 );
  zAssert( zVec2DMid, zIsTiny( r1 - r2 ) );
  zVec2DProj( &v1, &v2, &v3 );
  zAssert( zVec2DProj, zIsTiny( zVec2DOuterProd(&v2,&v3) ) );
  angle = zVec2DAngle( &v1, &v2 );
  zVec2DRot( &v1, angle, &v3 );
  zAssert( zVec2DRot, zIsTiny( zVec2DOuterProd(&v2,&v3) ) );
}

int main(void)
{
  zRandInit();
  assert_create();
  assert_arith();
  assert_vecprod();
  assert_geometry();
  return EXIT_SUCCESS;
}
