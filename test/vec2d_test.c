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

  zVec2DNormalize( &v1, 0, &v2 );
  zAssert( zVec2DNormalize, zIsTiny(v2.c.x*v2.c.x+v2.c.y*v2.c.y-1) );
  zVec2DNormalizeDRC( &v1, 0 );
  zAssert( zVec2DNormalizeDRC, zIsTiny(v1.c.x*v1.c.x+v1.c.y*v1.c.y-1) );
  /* tiny vector with the norm 5e-13 */
  zVec2DCreate( &v1, 3.0e-13, 4.0e-13 );
  zVec2DCreate( &v2, 1, 2 );
  zAssert( zVec2DNormalize (default tolerance), zVec2DNormalize( &v1, 0, &v2 ) == 0 && v2.c.x == 0 && v2.c.y == 0 );
  zAssert( zVec2DNormalize (specified tolerance), zVec2DNormalize( &v1, 1.0e-20, &v2 ) > 0 &&
    zIsTol( v2.c.x - 0.6, 1.0e-15 ) && zIsTol( v2.c.y - 0.8, 1.0e-15 ) );
  zAssert( zVec2DNormalizeDRC (specified tolerance), zVec2DNormalizeDRC( &v1, 1.0e-20 ) > 0 && zIsTiny( zVec2DNorm( &v1 ) - 1 ) );
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

/* division by very small or large values, where each component is accessed as a double array */
bool check_div_array_vec2d(const double *a, double k, const double *q, int n)
{
  int i;

  for( i=0; i<n; i++ )
    if( !zIsFinite( q[i] ) || fabs( q[i] - a[i]/k ) > DBL_EPSILON * fabs( a[i]/k ) ) return false;
  return true;
}

void assert_div_range_vec2d(void)
{
  zVec2D a, q, d;
  double k[] = { 3.0, 1.0e-13, 4.0e-309, 1.0e308 }; /* including a subnormal divisor and a divisor with the subnormal reciprocal */
  const int n = sizeof(zVec2D) / sizeof(double);
  int i, j;
  bool result = true, result_drc = true, result_zero;

  for( j=0; j<sizeof(k)/sizeof(double); j++ ){
    for( i=0; i<n; i++ ) ((double *)&a)[i] = zRandF(-1,1) * ( k[j] < 1.0e-300 ? 1.0e-300 : 1 ); /* avoid overflow */
    if( !zVec2DDiv( &a, k[j], &q ) || !check_div_array_vec2d( (double *)&a, k[j], (double *)&q, n ) ) result = false;
    d = a;
    if( !zVec2DDivDRC( &d, k[j] ) ) result_drc = false;
    for( i=0; i<n; i++ )
      if( ((double *)&d)[i] != ((double *)&q)[i] ) result_drc = false;
  }
  result_zero = zVec2DDiv( &a, 0, &q ) == NULL;
  zAssert( zVec2DDiv (very small or large divisor), result );
  zAssert( zVec2DDivDRC (very small or large divisor), result_drc );
  zAssert( zVec2DDiv (division by zero), result_zero );
}

int main(void)
{
  zRandInit();
  assert_create();
  assert_arith();
  assert_vecprod();
  assert_geometry();
  assert_div_range_vec2d();
  return EXIT_SUCCESS;
}
