#include <zeo/zeo.h>

void assert_create(void)
{
  zVec6D v1, v2;

  zVec6DCreate( &v1, 1, 2, 3, 4, 5, 6 );
  zAssert( zVec6DCreate, v1.e[0] == 1 && v1.e[1] == 2 && v1.e[2] == 3 && v1.e[3] == 4 && v1.e[4] == 5 && v1.e[5] == 6 );
  zVec6DCopy( &v1, &v2 );
  zAssert( zVec6DCopy, v1.e[0] == v2.e[0] && v1.e[1] == v2.e[1] && v1.e[2] == v2.e[2] && v1.e[3] == v2.e[3] && v1.e[4] == v2.e[4] && v1.e[5] == v2.e[5] );
  zAssert( zVec6DMatch, zVec6DMatch( &v1, &v2 ) );
  zVec6DZero( &v1 );
  zAssert( zVec6DIsTiny, zVec6DIsTiny( &v1 ) );
}

void assert_arith(void)
{
  zVec6D v1, v2, v3;
  double k;

  zVec6DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec6DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  k = zRandF(-10,10);
  zVec6DAdd( &v1, &v2, &v3 );
  zAssert( zVec6DAdd, v1.e[0]+v2.e[0]==v3.e[0] && v1.e[1]+v2.e[1]==v3.e[1] && v1.e[2]+v2.e[2]==v3.e[2]
                   && v1.e[3]+v2.e[3]==v3.e[3] && v1.e[4]+v2.e[4]==v3.e[4] && v1.e[5]+v2.e[5]==v3.e[5] );
  zVec6DSub( &v1, &v2, &v3 );
  zAssert( zVec6DSub, v1.e[0]-v2.e[0]==v3.e[0] && v1.e[1]-v2.e[1]==v3.e[1] && v1.e[2]-v2.e[2]==v3.e[2]
                   && v1.e[3]-v2.e[3]==v3.e[3] && v1.e[4]-v2.e[4]==v3.e[4] && v1.e[5]-v2.e[5]==v3.e[5] );
  zVec6DRev( &v1, &v3 );
  zAssert( zVec6DRev, v1.e[0]+v3.e[0]==0 && v1.e[1]+v3.e[1]==0 && v1.e[2]+v3.e[2]==0
                   && v1.e[3]+v3.e[3]==0 && v1.e[4]+v3.e[4]==0 && v1.e[5]+v3.e[5]==0 );
  zVec6DMul( &v1, k, &v3 );
  zAssert( zVec6DMul, v1.e[0]*k==v3.e[0] && v1.e[1]*k==v3.e[1] && v1.e[2]*k==v3.e[2]
                   && v1.e[3]*k==v3.e[3] && v1.e[4]*k==v3.e[4] && v1.e[5]*k==v3.e[5] );
  zVec6DDiv( &v1, k, &v3 );
  zAssert( zVec6DDiv, zIsTiny(v1.e[0]/k-v3.e[0]) && zIsTiny(v1.e[1]/k-v3.e[1]) && zIsTiny(v1.e[2]/k-v3.e[2])
                   && zIsTiny(v1.e[3]/k-v3.e[3]) && zIsTiny(v1.e[4]/k-v3.e[4]) && zIsTiny(v1.e[5]/k-v3.e[5]) );
  zVec6DCat( &v1, k, &v2, &v3 );
  zAssert( zVec6DCat, v1.e[0]+k*v2.e[0]==v3.e[0] && v1.e[1]+k*v2.e[1]==v3.e[1] && v1.e[2]+k*v2.e[2]==v3.e[2]
                   && v1.e[3]+k*v2.e[3]==v3.e[3] && v1.e[4]+k*v2.e[4]==v3.e[4] && v1.e[5]+k*v2.e[5]==v3.e[5] );

  zVec6DCopy( &v1, &v3 ); zVec6DAddDRC( &v3, &v2 );
  zAssert( zVec6DAddDRC, v1.e[0]+v2.e[0]==v3.e[0] && v1.e[1]+v2.e[1]==v3.e[1] && v1.e[2]+v2.e[2]==v3.e[2]
                      && v1.e[3]+v2.e[3]==v3.e[3] && v1.e[4]+v2.e[4]==v3.e[4] && v1.e[5]+v2.e[5]==v3.e[5] );
  zVec6DCopy( &v1, &v3 ); zVec6DSubDRC( &v3, &v2 );
  zAssert( zVec6DSubDRC, v1.e[0]-v2.e[0]==v3.e[0] && v1.e[1]-v2.e[1]==v3.e[1] && v1.e[2]-v2.e[2]==v3.e[2]
                      && v1.e[3]-v2.e[3]==v3.e[3] && v1.e[4]-v2.e[4]==v3.e[4] && v1.e[5]-v2.e[5]==v3.e[5] );
  zVec6DCopy( &v1, &v3 ); zVec6DRevDRC( &v3 );
  zAssert( zVec6DRevDRC, v1.e[0]+v3.e[0]==0 && v1.e[1]+v3.e[1]==0 && v1.e[2]+v3.e[2]==0
                      && v1.e[3]+v3.e[3]==0 && v1.e[4]+v3.e[4]==0 && v1.e[5]+v3.e[5]==0 );
  zVec6DCopy( &v1, &v3 ); zVec6DMulDRC( &v3, k );
  zAssert( zVec6DMulDRC, v1.e[0]*k==v3.e[0] && v1.e[1]*k==v3.e[1] && v1.e[2]*k==v3.e[2]
                      && v1.e[3]*k==v3.e[3] && v1.e[4]*k==v3.e[4] && v1.e[5]*k==v3.e[5] );
  zVec6DCopy( &v1, &v3 ); zVec6DDivDRC( &v3, k );
  zAssert( zVec6DDivDRC, zIsTiny(v1.e[0]/k-v3.e[0]) && zIsTiny(v1.e[1]/k-v3.e[1]) && zIsTiny(v1.e[2]/k-v3.e[2])
                      && zIsTiny(v1.e[3]/k-v3.e[3]) && zIsTiny(v1.e[4]/k-v3.e[4]) && zIsTiny(v1.e[5]/k-v3.e[5]) );
  zVec6DCopy( &v1, &v3 ); zVec6DCatDRC( &v3, k, &v2 );
  zAssert( zVec6DCatDRC, v1.e[0]+k*v2.e[0]==v3.e[0] && v1.e[1]+k*v2.e[1]==v3.e[1] && v1.e[2]+k*v2.e[2]==v3.e[2]
                      && v1.e[3]+k*v2.e[3]==v3.e[3] && v1.e[4]+k*v2.e[4]==v3.e[4] && v1.e[5]+k*v2.e[5]==v3.e[5] );
}

void assert_vecprod(void)
{
  zVec6D v1, v2;

  zVec6DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec6DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAssert( zVec6DInnerProd, zIsTiny(v1.e[0]*v2.e[0]+v1.e[1]*v2.e[1]+v1.e[2]*v2.e[2]+v1.e[3]*v2.e[3]+v1.e[4]*v2.e[4]+v1.e[5]*v2.e[5]-zVec6DInnerProd(&v1,&v2)) );
}

void assert_shift(void)
{
  zVec6D u1, u2;
  zVec3D p, w, v1, v2, f, n1, n2;

  zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &w, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &f, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &n1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

  zVec3DOuterProd( &w, &p, &v2 );
  zVec3DAddDRC( &v2, &v1 );
  zVec6DFromVec3D( &u1, &v1, &w );
  zVec6DLinShift( &u1, &p, &u2 );
  zAssert( zVec6DLinShift, u2.e[0]==v2.e[0] && u2.e[1]==v2.e[1] && u2.e[2]==v2.e[2]
                        && u2.e[3]==w.e[0] && u2.e[4]==w.e[1] && u2.e[5]==w.e[2] );
  zVec3DOuterProd( &p, &f, &n2 );
  zVec3DAddDRC( &n2, &n1 );
  zVec6DFromVec3D( &u1, &f, &n1 );
  zVec6DAngShift( &u1, &p, &u2 );
  zAssert( zVec6DAngShift, u2.e[0]==f.e[0] && u2.e[1]==f.e[1] && u2.e[2]==f.e[2]
                        && u2.e[3]==n2.e[0] && u2.e[4]==n2.e[1] && u2.e[5]==n2.e[2] );
}

/* division by very small or large values, where each component is accessed as a double array */
bool check_div_array_vec6d(const double *a, double k, const double *q, int n)
{
  int i;

  for( i=0; i<n; i++ )
    if( !zIsFinite( q[i] ) || fabs( q[i] - a[i]/k ) > DBL_EPSILON * fabs( a[i]/k ) ) return false;
  return true;
}

void assert_div_range_vec6d(void)
{
  zVec6D a, q, d;
  double k[] = { 3.0, 1.0e-13, 4.0e-309, 1.0e308 }; /* including a subnormal divisor and a divisor with the subnormal reciprocal */
  const int n = sizeof(zVec6D) / sizeof(double);
  int i, j;
  bool result = true, result_drc = true, result_zero;

  for( j=0; j<sizeof(k)/sizeof(double); j++ ){
    for( i=0; i<n; i++ ) ((double *)&a)[i] = zRandF(-1,1) * ( k[j] < 1.0e-300 ? 1.0e-300 : 1 ); /* avoid overflow */
    if( !zVec6DDiv( &a, k[j], &q ) || !check_div_array_vec6d( (double *)&a, k[j], (double *)&q, n ) ) result = false;
    d = a;
    if( !zVec6DDivDRC( &d, k[j] ) ) result_drc = false;
    for( i=0; i<n; i++ )
      if( ((double *)&d)[i] != ((double *)&q)[i] ) result_drc = false;
  }
  result_zero = zVec6DDiv( &a, 0, &q ) == NULL;
  zAssert( zVec6DDiv (very small or large divisor), result );
  zAssert( zVec6DDivDRC (very small or large divisor), result_drc );
  zAssert( zVec6DDiv (division by zero), result_zero );
}

int main(void)
{
  zRandInit();
  assert_create();
  assert_arith();
  assert_vecprod();
  assert_shift();
  assert_div_range_vec6d();
  return EXIT_SUCCESS;
}
