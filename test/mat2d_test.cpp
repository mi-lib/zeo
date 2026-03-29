#include <zeo/zeo.h>

void assert_matstruct(void)
{
  zMat2D tm, mc;
  zVec2D v1, v2;
  double a11, a12, a21, a22;

  a11 = zRandF(-10,10);
  a12 = zRandF(-10,10);
  a21 = zRandF(-10,10);
  a22 = zRandF(-10,10);
  zMat2D m( a11, a12, a21, a22 );
  zAssert( zMat2D(), m.c.xx == a11 && m.c.yx == a12 && m.c.xy == a21 && m.c.yy == a22 );
  m.row( 0, v1 );
  m.row( 1, v2 );
  zAssert( zMat2D &zMat2D::row(double, zVec2D &), v1.c.x == m.c.xx && v1.c.y == m.c.yx && v2.c.x == m.c.xy && v2.c.y == m.c.yy );
  m.col( 0, v1 );
  m.col( 1, v2 );
  zAssert( zMat2D &zMat2D::col(double, zVec2D &), v1.c.x == m.c.xx && v1.c.y == m.c.xy && v2.c.x == m.c.yx && v2.c.y == m.c.yy );
  m.transpose( tm );
  zAssert( zMat2D &zMat2::transpose(zMat2D &), tm.c.xx == m.c.xx && tm.c.yx == m.c.xy && tm.c.xy == m.c.yx && tm.c.yy == m.c.yy );
  mc.copy( m );
  mc.transpose();
  zAssert( zMat2D zMat2D::transpose(), zMat2DEqual( &tm, &mc ) );
}

void assert_arith(void)
{
  zMat2D m3;
  double k;

  zMat2D m1( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat2D m2( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec2D v1( zRandF(-10,10), zRandF(-10,10) );
  zVec2D v2( zRandF(-10,10), zRandF(-10,10) );
  k = zRandF(-10,10);
  m3 = m1 + m2;
  zAssert( zMat2D operator+(const zMat2D &, const zMat2D &), m1.c.xx+m2.c.xx==m3.c.xx && m1.c.yx+m2.c.yx==m3.c.yx && m1.c.xy+m2.c.xy==m3.c.xy && m1.c.yy+m2.c.yy==m3.c.yy );
  m3 = m1 - m2;
  zAssert( zMat2D operator-(const zMat2D &, const zMat2D &), m1.c.xx-m2.c.xx==m3.c.xx && m1.c.yx-m2.c.yx==m3.c.yx && m1.c.xy-m2.c.xy==m3.c.xy && m1.c.yy-m2.c.yy==m3.c.yy );
  m3 = -m1;
  zAssert( zMat2D zMat2D::operator-(), m1.c.xx+m3.c.xx==0 && m1.c.yx+m3.c.yx==0 && m1.c.xy+m3.c.xy==0 && m1.c.yy+m3.c.yy==0 );
  m3 = k * m1;
  zAssert( zMat2D operator*(double, const zMat2D &), m1.c.xx*k==m3.c.xx && m1.c.yx*k==m3.c.yx && m1.c.xy*k==m3.c.xy && m1.c.yy*k==m3.c.yy );
  m3 = m1 / k;
  zAssert( zMat2D operator-(const zMat2D &, double), zIsTiny(m1.c.xx/k-m3.c.xx) && zIsTiny(m1.c.yx/k-m3.c.yx) && zIsTiny(m1.c.xy/k-m3.c.xy) && zIsTiny(m1.c.yy/k-m3.c.yy) );
  m3 = m1 + k * m2;
  zAssert( m1 + k * m2, m1.c.xx+m2.c.xx*k==m3.c.xx && m1.c.yx+m2.c.yx*k==m3.c.yx && m1.c.xy+m2.c.xy*k==m3.c.xy && m1.c.yy+m2.c.yy*k==m3.c.yy );

  m3.copy( m1 ); m3 += m2;
  zAssert( zMat2D &zMat2D::operator+=(zMat2D &), m1.c.xx+m2.c.xx==m3.c.xx && m1.c.yx+m2.c.yx==m3.c.yx && m1.c.xy+m2.c.xy==m3.c.xy && m1.c.yy+m2.c.yy==m3.c.yy );
  m3.copy( m1 ); m3 -= m2;
  zAssert( zMat2D &zMat2D::operator-=(zMat2D &), m1.c.xx-m2.c.xx==m3.c.xx && m1.c.yx-m2.c.yx==m3.c.yx && m1.c.xy-m2.c.xy==m3.c.xy && m1.c.yy-m2.c.yy==m3.c.yy );
  m3.copy( m1 ); m3 = -m3;
  zAssert( zMat2D zMat2D::operator-(), m1.c.xx+m3.c.xx==0 && m1.c.yx+m3.c.yx==0 && m1.c.xy+m3.c.xy==0 && m1.c.yy+m3.c.yy==0 );
  m3.copy( m1 ); m3 *= k;
  zAssert( zMat2D &zMat2D::operator*=(double), m1.c.xx*k==m3.c.xx && m1.c.yx*k==m3.c.yx && m1.c.xy*k==m3.c.xy && m1.c.yy*k==m3.c.yy );
  m3.copy( m1 ); m3 /= k;
  zAssert( zMat2D &zMat2D::operator/=(double), zIsTiny(m1.c.xx/k-m3.c.xx) && zIsTiny(m1.c.yx/k-m3.c.yx) && zIsTiny(m1.c.xy/k-m3.c.xy) && zIsTiny(m1.c.yy/k-m3.c.yy) );
  m3.copy( m1 ); m3 = m3 + k * m2;
  zAssert( m3 = m3 + k * m2, m1.c.xx+m2.c.xx*k==m3.c.xx && m1.c.yx+m2.c.yx*k==m3.c.yx && m1.c.xy+m2.c.xy*k==m3.c.xy && m1.c.yy+m2.c.yy*k==m3.c.yy );

  m3.createDyad( v1, v2 );
  zAssert( zMat2D &zMat2D::createDyad(zVec2D &, zVec2D &), m3.c.xx==v1.c.x*v2.c.x && m3.c.yx==v1.c.x*v2.c.y && m3.c.xy==v1.c.y*v2.c.x && m3.c.yy==v1.c.y*v2.c.y );
}

void assert_inv(void)
{
  zMat2D m1, m2, m3;

  m1.create( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  m2.inv( m1 );
  m3 = m1 * m2;
  zAssert( zMat2D &zMat2D::inv(zMat2D &), zIsTiny(m3.c.xx-1) && zIsTiny(m3.c.yx) && zIsTiny(m3.c.xy) && zIsTiny(m3.c.yy-1) );
}

void assert_mul(void)
{
  zMat2D m1, m2, m3, m4;
  zVec2D v1, v2, v3;

  m1.create( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  m2.create( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  v1.create( zRandF(-10,10), zRandF(-10,10) );
  v2 = m1 * v1;
  zAssert( zVec2D operator*(const zMat2D &, const zVec2D &), m1.c.xx*v1.c.x+m1.c.yx*v1.c.y==v2.c.x && m1.c.xy*v1.c.x+m1.c.yy*v1.c.y==v2.c.y );
  v3.copy( v1 );
  v3 = m1 * v3;
  zAssert( v3 = m1 * v3, v2 == v3 );
  v2 = m1.mulT( v1 );
  zAssert( zVec2D zMat2D::mulT(zVec2D &), m1.c.xx*v1.c.x+m1.c.xy*v1.c.y==v2.c.x && m1.c.yx*v1.c.x+m1.c.yy*v1.c.y==v2.c.y );
  v3.copy( v1 );
  v3 = m1.mulT( v3 );
  zAssert( v3 = mi.mulT( v3 ), v2 == v3 );

  m3.inv( m1 );
  v2 = m1.mulInv( v1 );
  v3 = m3 * v1;
  zAssert( zVec2D zMat2D::mulInv(zVec2D &), zIsTiny(v2.c.x-v3.c.x) && zIsTiny(v2.c.y-v3.c.y) );

  m3 = m1 * m2;
  zAssert( zMat2D operator*(const zMat2D &, const zMat2D &),
       m1.c.xx*m2.c.xx+m1.c.yx*m2.c.xy==m3.c.xx && m1.c.xx*m2.c.yx+m1.c.yx*m2.c.yy==m3.c.yx
    && m1.c.xy*m2.c.xx+m1.c.yy*m2.c.xy==m3.c.xy && m1.c.xy*m2.c.yx+m1.c.yy*m2.c.yy==m3.c.yy );
  m4.copy( m2 );
  m4 = m1 * m4;
  zAssert( m4 = m1 * m4, m3 == m4 );
  m3 = m1.mulT( m2 );
  zAssert( zMat2D zMat2D::mulT(zMat2D &),
       m1.c.xx*m2.c.xx+m1.c.xy*m2.c.xy==m3.c.xx && m1.c.xx*m2.c.yx+m1.c.xy*m2.c.yy==m3.c.yx
    && m1.c.yx*m2.c.xx+m1.c.yy*m2.c.xy==m3.c.xy && m1.c.yx*m2.c.yx+m1.c.yy*m2.c.yy==m3.c.yy );
  m4.copy( m2 );
  m4 = m1.mulT( m4 );
  zAssert( m4 = m1.mulT( m4 ), m3 == m4 );
  // TODO write as C++ format
  zMulMat2DMat2DT( &m1, &m2, &m3 );
  zAssert( zMulMat2DMat2DT,
       m1.c.xx*m2.c.xx+m1.c.yx*m2.c.yx==m3.c.xx && m1.c.xx*m2.c.xy+m1.c.yx*m2.c.yy==m3.c.yx
    && m1.c.xy*m2.c.xx+m1.c.yy*m2.c.yx==m3.c.xy && m1.c.xy*m2.c.xy+m1.c.yy*m2.c.yy==m3.c.yy );
  m4.copy( m1 );
  // TODO write as C++ format
  zMulMat2DMat2DTDRC( &m4, &m2 );
  zAssert( zMulMat2DMat2DTDRC, m3 == m4 );

  m3 = m1.mulInv( m1 );
  zAssert( zMat2DInvMat2DMat2D, m3.isIdent() );
}

void assert_rotation(void)
{
  zMat2D m, m1, m2, m3;
  zVec2D v1, v2, v3, vx( 1.0, 0.0 ), vy( 0.0, 1.0 );
  double angle, error;

  angle = zRandF(-zPI,zPI);

  m.createAngle( angle );
  v1.create( zRandF(-10,10), zRandF(-10,10) );
  v2 = m * v1;
  zVec2DRot( &v1, angle, &v3 );
  zAssert( zMat2D::createAngle(double) + operator*(const zMat2D &, const zVec2D &), v2 == v3 );

  m1.create( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  m2 = m * m1;
  zMat2DRot( &m1, angle, &m3 );
  zAssert( zMat2D::create(double, double, double) + operator*(const zMat2D &, const zMat2D &), m2 == m3 );

  m1.ident();
  zMat2DRot( &m1, angle, &m2 );
  m2.col( 0, v1 );
  m2.col( 1, v2 );
  zAssert( zMat2DRot, zIsTiny( zVec2DAngle(&vx,&v1) - angle ) && zIsTiny( zVec2DAngle(&vy,&v2) - angle ) );
  error = zMat2DError( &m2, &m1 );
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
