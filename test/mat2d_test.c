#include <zeo/zeo.h>

void assert_matstruct(void)
{
  zMat2D m, tm, mc;
  zVec2D v1, v2;
  double a11, a12, a21, a22;

  a11 = zRandF(-10,10);
  a12 = zRandF(-10,10);
  a21 = zRandF(-10,10);
  a22 = zRandF(-10,10);
  zMat2DCreate( &m, a11, a12, a21, a22 );
  zAssert( zMat2DCreate, m.c.xx == a11 && m.c.yx == a12 && m.c.xy == a21 && m.c.yy == a22 );
  zMat2DRow( &m, 0, &v1 );
  zMat2DRow( &m, 1, &v2 );
  zAssert( zMat2DRow, v1.c.x == m.c.xx && v1.c.y == m.c.yx && v2.c.x == m.c.xy && v2.c.y == m.c.yy );
  zMat2DCol( &m, 0, &v1 );
  zMat2DCol( &m, 1, &v2 );
  zAssert( zMat2DCol, v1.c.x == m.c.xx && v1.c.y == m.c.xy && v2.c.x == m.c.yx && v2.c.y == m.c.yy );
  zMat2DT( &m, &tm );
  zAssert( zMat2DT, tm.c.xx == m.c.xx && tm.c.yx == m.c.xy && tm.c.xy == m.c.yx && tm.c.yy == m.c.yy );
  zMat2DCopy( &m, &mc );
  zMat2DTDRC( &mc );
  zAssert( zMat2DTDRC, zMat2DEqual( &tm, &mc ) );
}

void assert_arith(void)
{
  zMat2D m1, m2, m3;
  zVec2D v1, v2;
  double k;

  zMat2DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat2DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( &v1, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( &v2, zRandF(-10,10), zRandF(-10,10) );
  k = zRandF(-10,10);
  zMat2DAdd( &m1, &m2, &m3 );
  zAssert( zMat2DAdd, m1.c.xx+m2.c.xx==m3.c.xx && m1.c.yx+m2.c.yx==m3.c.yx && m1.c.xy+m2.c.xy==m3.c.xy && m1.c.yy+m2.c.yy==m3.c.yy );
  zMat2DSub( &m1, &m2, &m3 );
  zAssert( zMat2DSub, m1.c.xx-m2.c.xx==m3.c.xx && m1.c.yx-m2.c.yx==m3.c.yx && m1.c.xy-m2.c.xy==m3.c.xy && m1.c.yy-m2.c.yy==m3.c.yy );
  zMat2DRev( &m1, &m3 );
  zAssert( zMat2DRev, m1.c.xx+m3.c.xx==0 && m1.c.yx+m3.c.yx==0 && m1.c.xy+m3.c.xy==0 && m1.c.yy+m3.c.yy==0 );
  zMat2DMul( &m1, k, &m3 );
  zAssert( zMat2DMul, m1.c.xx*k==m3.c.xx && m1.c.yx*k==m3.c.yx && m1.c.xy*k==m3.c.xy && m1.c.yy*k==m3.c.yy );
  zMat2DDiv( &m1, k, &m3 );
  zAssert( zMat2DDiv, zIsTiny(m1.c.xx/k-m3.c.xx) && zIsTiny(m1.c.yx/k-m3.c.yx) && zIsTiny(m1.c.xy/k-m3.c.xy) && zIsTiny(m1.c.yy/k-m3.c.yy) );
  zMat2DCat( &m1, k, &m2, &m3 );
  zAssert( zMat2DCat, m1.c.xx+m2.c.xx*k==m3.c.xx && m1.c.yx+m2.c.yx*k==m3.c.yx && m1.c.xy+m2.c.xy*k==m3.c.xy && m1.c.yy+m2.c.yy*k==m3.c.yy );

  zMat2DCopy( &m1, &m3 ); zMat2DAddDRC( &m3, &m2 );
  zAssert( zMat2DAddDRC, m1.c.xx+m2.c.xx==m3.c.xx && m1.c.yx+m2.c.yx==m3.c.yx && m1.c.xy+m2.c.xy==m3.c.xy && m1.c.yy+m2.c.yy==m3.c.yy );
  zMat2DCopy( &m1, &m3 ); zMat2DSubDRC( &m3, &m2 );
  zAssert( zMat2DSubDRC, m1.c.xx-m2.c.xx==m3.c.xx && m1.c.yx-m2.c.yx==m3.c.yx && m1.c.xy-m2.c.xy==m3.c.xy && m1.c.yy-m2.c.yy==m3.c.yy );
  zMat2DCopy( &m1, &m3 ); zMat2DRevDRC( &m3 );
  zAssert( zMat2DRevDRC, m1.c.xx+m3.c.xx==0 && m1.c.yx+m3.c.yx==0 && m1.c.xy+m3.c.xy==0 && m1.c.yy+m3.c.yy==0 );
  zMat2DCopy( &m1, &m3 ); zMat2DMulDRC( &m3, k );
  zAssert( zMat2DMulDRC, m1.c.xx*k==m3.c.xx && m1.c.yx*k==m3.c.yx && m1.c.xy*k==m3.c.xy && m1.c.yy*k==m3.c.yy );
  zMat2DCopy( &m1, &m3 ); zMat2DDivDRC( &m3, k );
  zAssert( zMat2DDivDRC, zIsTiny(m1.c.xx/k-m3.c.xx) && zIsTiny(m1.c.yx/k-m3.c.yx) && zIsTiny(m1.c.xy/k-m3.c.xy) && zIsTiny(m1.c.yy/k-m3.c.yy) );
  zMat2DCopy( &m1, &m3 ); zMat2DCatDRC( &m3, k, &m2 );
  zAssert( zMat2DCatDRC, m1.c.xx+m2.c.xx*k==m3.c.xx && m1.c.yx+m2.c.yx*k==m3.c.yx && m1.c.xy+m2.c.xy*k==m3.c.xy && m1.c.yy+m2.c.yy*k==m3.c.yy );

  zMat2DDyad( &m3, &v1, &v2 );
  zAssert( zMat2DDyad, m3.c.xx==v1.c.x*v2.c.x && m3.c.yx==v1.c.x*v2.c.y && m3.c.xy==v1.c.y*v2.c.x && m3.c.yy==v1.c.y*v2.c.y );
}

void assert_inv(void)
{
  zMat2D m1, m2, m3;

  zMat2DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat2DInv( &m1, &m2 );
  zMulMat2DMat2D( &m1, &m2, &m3 );
  zAssert( zMat2DInv, zIsTiny(m3.c.xx-1) && zIsTiny(m3.c.yx) && zIsTiny(m3.c.xy) && zIsTiny(m3.c.yy-1) );
}

void assert_mul(void)
{
  zMat2D m1, m2, m3, m4;
  zVec2D v1, v2, v3;

  zMat2DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat2DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( &v1, zRandF(-10,10), zRandF(-10,10) );
  zMulMat2DVec2D( &m1, &v1, &v2 );
  zAssert( zMulMat2DVec2D, m1.c.xx*v1.c.x+m1.c.yx*v1.c.y==v2.c.x && m1.c.xy*v1.c.x+m1.c.yy*v1.c.y==v2.c.y );
  zVec2DCopy( &v1, &v3 );
  zMulMat2DVec2DDRC( &m1, &v3 );
  zAssert( zMulMat2DVec2DDRC, zVec2DEqual( &v2, &v3 ) );
  zMulMat2DTVec2D( &m1, &v1, &v2 );
  zAssert( zMulMat2DTVec2D, m1.c.xx*v1.c.x+m1.c.xy*v1.c.y==v2.c.x && m1.c.yx*v1.c.x+m1.c.yy*v1.c.y==v2.c.y );
  zVec2DCopy( &v1, &v3 );
  zMulMat2DTVec2DDRC( &m1, &v3 );
  zAssert( zMulMat2DVec2DDRC, zVec2DEqual( &v2, &v3 ) );

  zMat2DInv( &m1, &m3 );
  zMulInvMat2DVec2D( &m1, &v1, &v2 );
  zMulMat2DVec2D( &m3, &v1, &v3 );
  zAssert( zMulInvMat2DVec2D, zIsTiny(v2.c.x-v3.c.x) && zIsTiny(v2.c.y-v3.c.y) );

  zMulMat2DMat2D( &m1, &m2, &m3 );
  zAssert( zMulMat2DMat2D,
       m1.c.xx*m2.c.xx+m1.c.yx*m2.c.xy==m3.c.xx && m1.c.xx*m2.c.yx+m1.c.yx*m2.c.yy==m3.c.yx
    && m1.c.xy*m2.c.xx+m1.c.yy*m2.c.xy==m3.c.xy && m1.c.xy*m2.c.yx+m1.c.yy*m2.c.yy==m3.c.yy );
  zMat2DCopy( &m2, &m4 );
  zMulMat2DMat2DDRC( &m1, &m4 );
  zAssert( zMulMat2DMat2DDRC, zMat2DEqual( &m3, &m4 ) );
  zMulMat2DTMat2D( &m1, &m2, &m3 );
  zAssert( zMulMat2DTMat2D,
       m1.c.xx*m2.c.xx+m1.c.xy*m2.c.xy==m3.c.xx && m1.c.xx*m2.c.yx+m1.c.xy*m2.c.yy==m3.c.yx
    && m1.c.yx*m2.c.xx+m1.c.yy*m2.c.xy==m3.c.xy && m1.c.yx*m2.c.yx+m1.c.yy*m2.c.yy==m3.c.yy );
  zMat2DCopy( &m2, &m4 );
  zMulMat2DTMat2DDRC( &m1, &m4 );
  zAssert( zMulMat2DTMat2DDRC, zMat2DEqual( &m3, &m4 ) );
  zMulMat2DMat2DT( &m1, &m2, &m3 );
  zAssert( zMulMat2DMat2DT,
       m1.c.xx*m2.c.xx+m1.c.yx*m2.c.yx==m3.c.xx && m1.c.xx*m2.c.xy+m1.c.yx*m2.c.yy==m3.c.yx
    && m1.c.xy*m2.c.xx+m1.c.yy*m2.c.yx==m3.c.xy && m1.c.xy*m2.c.xy+m1.c.yy*m2.c.yy==m3.c.yy );
  zMat2DCopy( &m1, &m4 );
  zMulMat2DMat2DTDRC( &m4, &m2 );
  zAssert( zMulMat2DMat2DTDRC, zMat2DEqual( &m3, &m4 ) );

  zMulInvMat2DMat2D( &m1, &m1, &m3 );
  zAssert( zMat2DInvMat2DMat2D, zMat2DIsIdent(&m3) );
}

void assert_rotation(void)
{
  zMat2D m, m1, m2, m3;
#ifdef __cplusplus
  zVec2D v1, v2, v3, vx{ 1.0, 0.0 }, vy{ 0.0, 1.0 };
#else
  zVec2D v1, v2, v3, vx = { { 1.0, 0.0 } }, vy = { { 0.0, 1.0 } };
#endif
  double angle, error;

  angle = zRandF(-zPI,zPI);

  zMat2DCreateAngle( &m, angle );
  zVec2DCreate( &v1, zRandF(-10,10), zRandF(-10,10) );
  zMulMat2DVec2D( &m, &v1, &v2 );
  zVec2DRot( &v1, angle, &v3 );
  zAssert( zMat2DCreateAngle + zMulMat2DVec2D, zVec2DEqual( &v2, &v3 ) );

  zMat2DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMulMat2DMat2D( &m, &m1, &m2 );
  zMat2DRot( &m1, angle, &m3 );
  zAssert( zMat2DCreateAngle + zMulMat2DMat2D, zMat2DEqual( &m2, &m3 ) );

  zMat2DIdent( &m1 );
  zMat2DRot( &m1, angle, &m2 );
  zMat2DCol( &m2, 0, &v1 );
  zMat2DCol( &m2, 1, &v2 );
  zAssert( zMat2DRot, zIsTiny( zVec2DAngle(&vx,&v1) - angle ) && zIsTiny( zVec2DAngle(&vy,&v2) - angle ) );
  error = zMat2DError( &m2, &m1 );
  zAssert( zMat2DError, zIsTiny( angle - error ) );
}

/* division by very small or large values, where each component is accessed as a double array */
bool check_div_array_mat2d(const double *a, double k, const double *q, int n)
{
  int i;

  for( i=0; i<n; i++ )
    if( !zIsFinite( q[i] ) || fabs( q[i] - a[i]/k ) > DBL_EPSILON * fabs( a[i]/k ) ) return false;
  return true;
}

void assert_div_range_mat2d(void)
{
  zMat2D a, q, d;
  double k[] = { 3.0, 1.0e-13, 4.0e-309, 1.0e308 }; /* including a subnormal divisor and a divisor with the subnormal reciprocal */
  const int n = sizeof(zMat2D) / sizeof(double);
  int i, j;
  bool result = true, result_drc = true, result_zero;

  for( j=0; j<sizeof(k)/sizeof(double); j++ ){
    for( i=0; i<n; i++ ) ((double *)&a)[i] = zRandF(-1,1) * ( k[j] < 1.0e-300 ? 1.0e-300 : 1 ); /* avoid overflow */
    if( !zMat2DDiv( &a, k[j], &q ) || !check_div_array_mat2d( (double *)&a, k[j], (double *)&q, n ) ) result = false;
    d = a;
    if( !zMat2DDivDRC( &d, k[j] ) ) result_drc = false;
    for( i=0; i<n; i++ )
      if( ((double *)&d)[i] != ((double *)&q)[i] ) result_drc = false;
  }
  result_zero = zMat2DDiv( &a, 0, &q ) == NULL;
  zAssert( zMat2DDiv (very small or large divisor), result );
  zAssert( zMat2DDivDRC (very small or large divisor), result_drc );
  zAssert( zMat2DDiv (division by zero), result_zero );
}

int main(void)
{
  zRandInit();
  assert_matstruct();
  assert_arith();
  assert_inv();
  assert_mul();
  assert_rotation();
  assert_div_range_mat2d();
  return EXIT_SUCCESS;
}
