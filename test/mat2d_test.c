#include <zeo/zeo.h>

void assert_matstruct(void)
{
  zMat2D m, tm;
  zVec2D v1, v2;
  double a11, a12, a21, a22;

  a11 = zRandF(-10,10);
  a12 = zRandF(-10,10);
  a21 = zRandF(-10,10);
  a22 = zRandF(-10,10);
  zMat2DCreate( m, a11, a12, a21, a22 );
  zAssert( zMat2DCreate, m[0] == a11 && m[1] == a12 && m[2] == a21 && m[3] == a22 );
  zMat2DRow( m, v1, v2 );
  zAssert( zMat2DRow, v1[0] == m[0] && v1[1] == m[1] && v2[0] == m[2] && v2[1] == m[3] );
  zMat2DCol( m, v1, v2 );
  zAssert( zMat2DCol, v1[0] == m[0] && v1[1] == m[2] && v2[0] == m[1] && v2[1] == m[3] );
  zMat2DT( m, tm );
  zAssert( zMat2DT, tm[0] == m[0] && tm[1] == m[2] && tm[2] == m[1] && tm[3] == m[3] );
}

void assert_arith(void)
{
  zMat2D m1, m2, m3;
  zVec2D v1, v2;
  double k;

  zMat2DCreate( m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat2DCreate( m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( v1, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( v2, zRandF(-10,10), zRandF(-10,10) );
  k = zRandF(-10,10);
  zMat2DAdd( m1, m2, m3 );
  zAssert( zMat2DAdd, m1[0]+m2[0]==m3[0] && m1[1]+m2[1]==m3[1] && m1[2]+m2[2]==m3[2] && m1[3]+m2[3]==m3[3] );
  zMat2DSub( m1, m2, m3 );
  zAssert( zMat2DSub, m1[0]-m2[0]==m3[0] && m1[1]-m2[1]==m3[1] && m1[2]-m2[2]==m3[2] && m1[3]-m2[3]==m3[3] );
  zMat2DRev( m1, m3 );
  zAssert( zMat2DRev, m1[0]+m3[0]==0 && m1[1]+m3[1]==0 && m1[2]+m3[2]==0 && m1[3]+m3[3]==0 );
  zMat2DMul( m1, k, m3 );
  zAssert( zMat2DMul, m1[0]*k==m3[0] && m1[1]*k==m3[1] && m1[2]*k==m3[2] && m1[3]*k==m3[3] );
  zMat2DDiv( m1, k, m3 );
  zAssert( zMat2DDiv, zIsTiny(m1[0]/k-m3[0]) && zIsTiny(m1[1]/k-m3[1]) && zIsTiny(m1[2]/k-m3[2]) && zIsTiny(m1[3]/k-m3[3]) );
  zMat2DCat( m1, k, m2, m3 );
  zAssert( zMat2DCat, m1[0]+m2[0]*k==m3[0] && m1[1]+m2[1]*k==m3[1] && m1[2]+m2[2]*k==m3[2] && m1[3]+m2[3]*k==m3[3] );

  zMat2DCopy( m1, m3 ); zMat2DAddDRC( m3, m2 );
  zAssert( zMat2DAddDRC, m1[0]+m2[0]==m3[0] && m1[1]+m2[1]==m3[1] && m1[2]+m2[2]==m3[2] && m1[3]+m2[3]==m3[3] );
  zMat2DCopy( m1, m3 ); zMat2DSubDRC( m3, m2 );
  zAssert( zMat2DSubDRC, m1[0]-m2[0]==m3[0] && m1[1]-m2[1]==m3[1] && m1[2]-m2[2]==m3[2] && m1[3]-m2[3]==m3[3] );
  zMat2DCopy( m1, m3 ); zMat2DRevDRC( m3 );
  zAssert( zMat2DRevDRC, m1[0]+m3[0]==0 && m1[1]+m3[1]==0 && m1[2]+m3[2]==0 && m1[3]+m3[3]==0 );
  zMat2DCopy( m1, m3 ); zMat2DMulDRC( m3, k );
  zAssert( zMat2DMulDRC, m1[0]*k==m3[0] && m1[1]*k==m3[1] && m1[2]*k==m3[2] && m1[3]*k==m3[3] );
  zMat2DCopy( m1, m3 ); zMat2DDivDRC( m3, k );
  zAssert( zMat2DDivDRC, zIsTiny(m1[0]/k-m3[0]) && zIsTiny(m1[1]/k-m3[1]) && zIsTiny(m1[2]/k-m3[2]) && zIsTiny(m1[3]/k-m3[3]) );
  zMat2DCopy( m1, m3 ); zMat2DCatDRC( m3, k, m2 );
  zAssert( zMat2DCatDRC, m1[0]+m2[0]*k==m3[0] && m1[1]+m2[1]*k==m3[1] && m1[2]+m2[2]*k==m3[2] && m1[3]+m2[3]*k==m3[3] );

  zMat2DDyad( m3, v1, v2 );
  zAssert( zMat2DDyad, m3[0]==v1[0]*v2[0] && m3[1]==v1[0]*v2[1] && m3[2]==v1[1]*v2[0] && m3[3]==v1[1]*v2[1] );
}

void assert_inv(void)
{
  zMat2D m1, m2, m3;

  zMat2DCreate( m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat2DInv( m1, m2 );
  zMulMat2DMat2D( m1, m2, m3 );
  zAssert( zMat2DInv, zIsTiny(m3[0]-1) && zIsTiny(m3[1]) && zIsTiny(m3[2]) && zIsTiny(m3[3]-1) );
}

void assert_mul(void)
{
  zMat2D m1, m2, m3;
  zVec2D v1, v2, v3;

  zMat2DCreate( m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat2DCreate( m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( v1, zRandF(-10,10), zRandF(-10,10) );
  zMulMat2DVec2D( m1, v1, v2 );
  zAssert( zMulMat2DVec2D, m1[0]*v1[0]+m1[1]*v1[1]==v2[0] && m1[2]*v1[0]+m1[3]*v1[1]==v2[1] );
  zMulMat2DTVec2D( m1, v1, v2 );
  zAssert( zMulMat2DTVec2D, m1[0]*v1[0]+m1[2]*v1[1]==v2[0] && m1[1]*v1[0]+m1[3]*v1[1]==v2[1] );

  zMat2DInv( m1, m3 );
  zMulInvMat2DVec2D( m1, v1, v2 );
  zMulMat2DVec2D( m3, v1, v3 );
  zAssert( zMulInvMat2DVec2D, zIsTiny(v2[0]-v3[0]) && zIsTiny(v2[1]-v3[1]) );

  zMulMat2DMat2D( m1, m2, m3 );
  zAssert( zMulMat2DMat2D,
       m1[0]*m2[0]+m1[1]*m2[2]==m3[0] && m1[0]*m2[1]+m1[1]*m2[3]==m3[1]
    && m1[2]*m2[0]+m1[3]*m2[2]==m3[2] && m1[2]*m2[1]+m1[3]*m2[3]==m3[3] );
  zMulMat2DTMat2D( m1, m2, m3 );
  zAssert( zMulMat2DTMat2D,
       m1[0]*m2[0]+m1[2]*m2[2]==m3[0] && m1[0]*m2[1]+m1[2]*m2[3]==m3[1]
    && m1[1]*m2[0]+m1[3]*m2[2]==m3[2] && m1[1]*m2[1]+m1[3]*m2[3]==m3[3] );
  zMulMat2DMat2DT( m1, m2, m3 );
  zAssert( zMulMat2DMat2DT,
       m1[0]*m2[0]+m1[1]*m2[1]==m3[0] && m1[0]*m2[2]+m1[1]*m2[3]==m3[1]
    && m1[2]*m2[0]+m1[3]*m2[1]==m3[2] && m1[2]*m2[2]+m1[3]*m2[3]==m3[3] );

  zMulInvMat2DMat2D( m1, m1, m3 );
  zAssert( zMat2DInv, zIsTiny(m3[0]-1) && zIsTiny(m3[1]) && zIsTiny(m3[2]) && zIsTiny(m3[3]-1) );
}

void assert_rotation(void)
{
  zMat2D m1, m2;
  zVec2D v1, v2, vx = { 1.0, 0.0 }, vy = { 0.0, 1.0 };
  double angle, error;

  zMat2DIdent( m1 );
  angle = zRandF(-zPI,zPI);
  zMat2DRot( m1, angle, m2 );
  zMat2DCol( m2, v1, v2 );
  zAssert( zMat2DRot, zIsTiny( zVec2DAngle(vx,v1) - angle ) && zIsTiny( zVec2DAngle(vy,v2) - angle ) );
  error = zMat2DError( m2, m1 );
  zAssert( zMat2DError, zIsTiny( angle - error ) );
}

int main(void)
{
  zRandInit();
  assert_matstruct();
  assert_arith();
  assert_inv();
  assert_mul();
  assert_rotation();
  return EXIT_SUCCESS;
}
