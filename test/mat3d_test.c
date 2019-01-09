#include <zeo/zeo.h>

void assert_matstruct(void)
{
  zMat3D m, tm;
  zVec3D v1, v2, v3;
  double a11, a12, a13, a21, a22, a23, a31, a32, a33;

  a11 = zRandF(-10,10);
  a12 = zRandF(-10,10);
  a13 = zRandF(-10,10);
  a21 = zRandF(-10,10);
  a22 = zRandF(-10,10);
  a23 = zRandF(-10,10);
  a31 = zRandF(-10,10);
  a32 = zRandF(-10,10);
  a33 = zRandF(-10,10);
  zMat3DCreate( &m, a11, a12, a13, a21, a22, a23, a31, a32, a33 );
  zAssert( zMat3DCreate,
       m.e[0][0] == a11 && m.v[0].e[0] == a11 && m.b.x.e[0] == a11 && m.c.xx == a11
    && m.e[0][1] == a21 && m.v[0].e[1] == a21 && m.b.x.e[1] == a21 && m.c.xy == a21
    && m.e[0][2] == a31 && m.v[0].e[2] == a31 && m.b.x.e[2] == a31 && m.c.xz == a31
    && m.e[1][0] == a12 && m.v[1].e[0] == a12 && m.b.y.e[0] == a12 && m.c.yx == a12
    && m.e[1][1] == a22 && m.v[1].e[1] == a22 && m.b.y.e[1] == a22 && m.c.yy == a22
    && m.e[1][2] == a32 && m.v[1].e[2] == a32 && m.b.y.e[2] == a32 && m.c.yz == a32
    && m.e[2][0] == a13 && m.v[2].e[0] == a13 && m.b.z.e[0] == a13 && m.c.zx == a13
    && m.e[2][1] == a23 && m.v[2].e[1] == a23 && m.b.z.e[1] == a23 && m.c.zy == a23
    && m.e[2][2] == a33 && m.v[2].e[2] == a33 && m.b.z.e[2] == a33 && m.c.zz == a33 );
  zMat3DRow( &m, 0, &v1 );
  zMat3DRow( &m, 1, &v2 );
  zMat3DRow( &m, 2, &v3 );
  zAssert( zMat3DRow,
       v1.e[0] == m.e[0][0] && v1.e[1] == m.e[1][0] && v1.e[2] == m.e[2][0]
    && v2.e[0] == m.e[0][1] && v2.e[1] == m.e[1][1] && v2.e[2] == m.e[2][1]
    && v3.e[0] == m.e[0][2] && v3.e[1] == m.e[1][2] && v3.e[2] == m.e[2][2] );
  zMat3DCol( &m, 0, &v1 );
  zMat3DCol( &m, 1, &v2 );
  zMat3DCol( &m, 2, &v3 );
  zAssert( zMat3DCol,
       v1.e[0] == m.e[0][0] && v1.e[1] == m.e[0][1] && v1.e[2] == m.e[0][2]
    && v2.e[0] == m.e[1][0] && v2.e[1] == m.e[1][1] && v2.e[2] == m.e[1][2]
    && v3.e[0] == m.e[2][0] && v3.e[1] == m.e[2][1] && v3.e[2] == m.e[2][2] );
  zMat3DT( &m, &tm );
  zAssert( zMat3DT,
       tm.e[0][0] == m.e[0][0] && tm.e[1][0] == m.e[0][1] && tm.e[2][0] == m.e[0][2]
    && tm.e[0][1] == m.e[1][0] && tm.e[1][1] == m.e[1][1] && tm.e[2][1] == m.e[1][2]
    && tm.e[0][2] == m.e[2][0] && tm.e[1][2] == m.e[2][1] && tm.e[2][2] == m.e[2][2] );
  zMat3DTDRC( &m );
  zAssert( zMat3DTDRC,
       tm.e[0][0] == m.e[0][0] && tm.e[0][1] == m.e[0][1] && tm.e[0][2] == m.e[0][2]
    && tm.e[1][0] == m.e[1][0] && tm.e[1][1] == m.e[1][1] && tm.e[1][2] == m.e[1][2]
    && tm.e[2][0] == m.e[2][0] && tm.e[2][1] == m.e[2][1] && tm.e[2][2] == m.e[2][2] );
}

void assert_arith(void)
{
  zMat3D m1, m2, m3;
  zVec3D v1, v2;
  double k;

  zMat3DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  k = zRandF(-10,10);
  zMat3DAdd( &m1, &m2, &m3 );
  zAssert( zMat3DAdd,
       m1.e[0][0]+m2.e[0][0]==m3.e[0][0] && m1.e[0][1]+m2.e[0][1]==m3.e[0][1] && m1.e[0][2]+m2.e[0][2]==m3.e[0][2]
    && m1.e[1][0]+m2.e[1][0]==m3.e[1][0] && m1.e[1][1]+m2.e[1][1]==m3.e[1][1] && m1.e[1][2]+m2.e[1][2]==m3.e[1][2]
    && m1.e[2][0]+m2.e[2][0]==m3.e[2][0] && m1.e[2][1]+m2.e[2][1]==m3.e[2][1] && m1.e[2][2]+m2.e[2][2]==m3.e[2][2] );
  zMat3DSub( &m1, &m2, &m3 );
  zAssert( zMat3DSub,
       m1.e[0][0]-m2.e[0][0]==m3.e[0][0] && m1.e[0][1]-m2.e[0][1]==m3.e[0][1] && m1.e[0][2]-m2.e[0][2]==m3.e[0][2]
    && m1.e[1][0]-m2.e[1][0]==m3.e[1][0] && m1.e[1][1]-m2.e[1][1]==m3.e[1][1] && m1.e[1][2]-m2.e[1][2]==m3.e[1][2]
    && m1.e[2][0]-m2.e[2][0]==m3.e[2][0] && m1.e[2][1]-m2.e[2][1]==m3.e[2][1] && m1.e[2][2]-m2.e[2][2]==m3.e[2][2] );
  zMat3DRev( &m1, &m3 );
  zAssert( zMat3DRev,
       m1.e[0][0]+m3.e[0][0]==0 && m1.e[0][1]+m3.e[0][1]==0 && m1.e[0][2]+m3.e[0][2]==0
    && m1.e[1][0]+m3.e[1][0]==0 && m1.e[1][1]+m3.e[1][1]==0 && m1.e[1][2]+m3.e[1][2]==0
    && m1.e[2][0]+m3.e[2][0]==0 && m1.e[2][1]+m3.e[2][1]==0 && m1.e[2][2]+m3.e[2][2]==0 );
  zMat3DMul( &m1, k, &m3 );
  zAssert( zMat3DMul,
       m1.e[0][0]*k==m3.e[0][0] && m1.e[0][1]*k==m3.e[0][1] && m1.e[0][2]*k==m3.e[0][2]
    && m1.e[1][0]*k==m3.e[1][0] && m1.e[1][1]*k==m3.e[1][1] && m1.e[1][2]*k==m3.e[1][2]
    && m1.e[2][0]*k==m3.e[2][0] && m1.e[2][1]*k==m3.e[2][1] && m1.e[2][2]*k==m3.e[2][2] );
  zMat3DDiv( &m1, k, &m3 );
  zAssert( zMat3DDiv,
       zIsTiny(m1.e[0][0]/k-m3.e[0][0]) && zIsTiny(m1.e[0][1]/k-m3.e[0][1]) && zIsTiny(m1.e[0][2]/k-m3.e[0][2])
    && zIsTiny(m1.e[1][0]/k-m3.e[1][0]) && zIsTiny(m1.e[1][1]/k-m3.e[1][1]) && zIsTiny(m1.e[1][2]/k-m3.e[1][2])
    && zIsTiny(m1.e[2][0]/k-m3.e[2][0]) && zIsTiny(m1.e[2][1]/k-m3.e[2][1]) && zIsTiny(m1.e[2][2]/k-m3.e[2][2]) );
  zMat3DCat( &m1, k, &m2, &m3 );
  zAssert( zMat3DCat,
       m1.e[0][0]+k*m2.e[0][0]==m3.e[0][0] && m1.e[0][1]+k*m2.e[0][1]==m3.e[0][1] && m1.e[0][2]+k*m2.e[0][2]==m3.e[0][2]
    && m1.e[1][0]+k*m2.e[1][0]==m3.e[1][0] && m1.e[1][1]+k*m2.e[1][1]==m3.e[1][1] && m1.e[1][2]+k*m2.e[1][2]==m3.e[1][2]
    && m1.e[2][0]+k*m2.e[2][0]==m3.e[2][0] && m1.e[2][1]+k*m2.e[2][1]==m3.e[2][1] && m1.e[2][2]+k*m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m1, &m3 ); zMat3DAddDRC( &m3, &m2 );
  zAssert( zMat3DAddDRC,
       m1.e[0][0]+m2.e[0][0]==m3.e[0][0] && m1.e[0][1]+m2.e[0][1]==m3.e[0][1] && m1.e[0][2]+m2.e[0][2]==m3.e[0][2]
    && m1.e[1][0]+m2.e[1][0]==m3.e[1][0] && m1.e[1][1]+m2.e[1][1]==m3.e[1][1] && m1.e[1][2]+m2.e[1][2]==m3.e[1][2]
    && m1.e[2][0]+m2.e[2][0]==m3.e[2][0] && m1.e[2][1]+m2.e[2][1]==m3.e[2][1] && m1.e[2][2]+m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m1, &m3 ); zMat3DSubDRC( &m3, &m2 );
  zAssert( zMat3DSubDRC,
       m1.e[0][0]-m2.e[0][0]==m3.e[0][0] && m1.e[0][1]-m2.e[0][1]==m3.e[0][1] && m1.e[0][2]-m2.e[0][2]==m3.e[0][2]
    && m1.e[1][0]-m2.e[1][0]==m3.e[1][0] && m1.e[1][1]-m2.e[1][1]==m3.e[1][1] && m1.e[1][2]-m2.e[1][2]==m3.e[1][2]
    && m1.e[2][0]-m2.e[2][0]==m3.e[2][0] && m1.e[2][1]-m2.e[2][1]==m3.e[2][1] && m1.e[2][2]-m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m1, &m3 ); zMat3DRevDRC( &m3 );
  zAssert( zMat3DRevDRC,
       m1.e[0][0]+m3.e[0][0]==0 && m1.e[0][1]+m3.e[0][1]==0 && m1.e[0][2]+m3.e[0][2]==0
    && m1.e[1][0]+m3.e[1][0]==0 && m1.e[1][1]+m3.e[1][1]==0 && m1.e[1][2]+m3.e[1][2]==0
    && m1.e[2][0]+m3.e[2][0]==0 && m1.e[2][1]+m3.e[2][1]==0 && m1.e[2][2]+m3.e[2][2]==0 );
  zMat3DCopy( &m1, &m3 ); zMat3DMulDRC( &m3, k );
  zAssert( zMat3DMulDRC,
       m1.e[0][0]*k==m3.e[0][0] && m1.e[0][1]*k==m3.e[0][1] && m1.e[0][2]*k==m3.e[0][2]
    && m1.e[1][0]*k==m3.e[1][0] && m1.e[1][1]*k==m3.e[1][1] && m1.e[1][2]*k==m3.e[1][2]
    && m1.e[2][0]*k==m3.e[2][0] && m1.e[2][1]*k==m3.e[2][1] && m1.e[2][2]*k==m3.e[2][2] );
  zMat3DCopy( &m1, &m3 ); zMat3DDivDRC( &m3, k );
  zAssert( zMat3DDivDRC,
       zIsTiny(m1.e[0][0]/k-m3.e[0][0]) && zIsTiny(m1.e[0][1]/k-m3.e[0][1]) && zIsTiny(m1.e[0][2]/k-m3.e[0][2])
    && zIsTiny(m1.e[1][0]/k-m3.e[1][0]) && zIsTiny(m1.e[1][1]/k-m3.e[1][1]) && zIsTiny(m1.e[1][2]/k-m3.e[1][2])
    && zIsTiny(m1.e[2][0]/k-m3.e[2][0]) && zIsTiny(m1.e[2][1]/k-m3.e[2][1]) && zIsTiny(m1.e[2][2]/k-m3.e[2][2]) );
  zMat3DCopy( &m1, &m3 ); zMat3DCatDRC( &m3, k, &m2 );
  zAssert( zMat3DCatDRC,
       m1.e[0][0]+k*m2.e[0][0]==m3.e[0][0] && m1.e[0][1]+k*m2.e[0][1]==m3.e[0][1] && m1.e[0][2]+k*m2.e[0][2]==m3.e[0][2]
    && m1.e[1][0]+k*m2.e[1][0]==m3.e[1][0] && m1.e[1][1]+k*m2.e[1][1]==m3.e[1][1] && m1.e[1][2]+k*m2.e[1][2]==m3.e[1][2]
    && m1.e[2][0]+k*m2.e[2][0]==m3.e[2][0] && m1.e[2][1]+k*m2.e[2][1]==m3.e[2][1] && m1.e[2][2]+k*m2.e[2][2]==m3.e[2][2] );
  zMat3DDyad( &m3, &v1, &v2 );
  zAssert( zMat3DDyad,
       m3.e[0][0]==v1.e[0]*v2.e[0] && m3.e[1][0]==v1.e[0]*v2.e[1] && m3.e[2][0]==v1.e[0]*v2.e[2]
    && m3.e[0][1]==v1.e[1]*v2.e[0] && m3.e[1][1]==v1.e[1]*v2.e[1] && m3.e[2][1]==v1.e[1]*v2.e[2]
    && m3.e[0][2]==v1.e[2]*v2.e[0] && m3.e[1][2]==v1.e[2]*v2.e[1] && m3.e[2][2]==v1.e[2]*v2.e[2] );
  zMat3DCopy( &m1, &m3 ); zMat3DAddDyad( &m3, &v1, &v2 );
  zAssert( zMat3DAddDyad,
       m3.e[0][0]==m1.e[0][0]+v1.e[0]*v2.e[0] && m3.e[1][0]==m1.e[1][0]+v1.e[0]*v2.e[1] && m3.e[2][0]==m1.e[2][0]+v1.e[0]*v2.e[2]
    && m3.e[0][1]==m1.e[0][1]+v1.e[1]*v2.e[0] && m3.e[1][1]==m1.e[1][1]+v1.e[1]*v2.e[1] && m3.e[2][1]==m1.e[2][1]+v1.e[1]*v2.e[2]
    && m3.e[0][2]==m1.e[0][2]+v1.e[2]*v2.e[0] && m3.e[1][2]==m1.e[1][2]+v1.e[2]*v2.e[1] && m3.e[2][2]==m1.e[2][2]+v1.e[2]*v2.e[2] );
  zMat3DCopy( &m1, &m3 ); zMat3DSubDyad( &m3, &v1, &v2 );
  zAssert( zMat3DSubDyad,
       m3.e[0][0]==m1.e[0][0]-v1.e[0]*v2.e[0] && m3.e[1][0]==m1.e[1][0]-v1.e[0]*v2.e[1] && m3.e[2][0]==m1.e[2][0]-v1.e[0]*v2.e[2]
    && m3.e[0][1]==m1.e[0][1]-v1.e[1]*v2.e[0] && m3.e[1][1]==m1.e[1][1]-v1.e[1]*v2.e[1] && m3.e[2][1]==m1.e[2][1]-v1.e[1]*v2.e[2]
    && m3.e[0][2]==m1.e[0][2]-v1.e[2]*v2.e[0] && m3.e[1][2]==m1.e[1][2]-v1.e[2]*v2.e[1] && m3.e[2][2]==m1.e[2][2]-v1.e[2]*v2.e[2] );
}

void assert_outerprod(void)
{
  zVec3D v1, v2, v3, v4, v5, v6;
  zMat3D m1, m2, m3, m4;

  zMat3DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v3, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DOuterProd2Mat3D( &v1, &m1 );
  zMulMat3DMat3D( &m1, &m2, &m3 );
  zVec3DOuterProd( &v1, &m2.v[0], &v4 );
  zVec3DOuterProd( &v1, &m2.v[1], &v5 );
  zVec3DOuterProd( &v1, &m2.v[2], &v6 );
  zAssert( zVec3DOuterProd2Mat3D,
       m1.e[0][0]==0 && m1.e[1][0]==-v1.e[2] && m1.e[2][0]==v1.e[1]
    && m1.e[0][1]==v1.e[2] && m1.e[1][1]==0 && m1.e[2][1]==-v1.e[0]
    && m1.e[0][2]==-v1.e[1] && m1.e[1][2]==v1.e[0] && m1.e[2][2]==0
    && m3.e[0][0]==v4.e[0] && m3.e[0][1]==v4.e[1] && m3.e[0][2]==v4.e[2]
    && m3.e[1][0]==v5.e[0] && m3.e[1][1]==v5.e[1] && m3.e[1][2]==v5.e[2]
    && m3.e[2][0]==v6.e[0] && m3.e[2][1]==v6.e[1] && m3.e[2][2]==v6.e[2] );
  zVec3DTripleProd2Mat3D( &v1, &v2, &m1 );
  zMulMat3DMat3D( &m1, &m2, &m3 );
  zVec3DTripleProd( &v1, &v2, &m2.v[0], &v4 );
  zVec3DTripleProd( &v1, &v2, &m2.v[1], &v5 );
  zVec3DTripleProd( &v1, &v2, &m2.v[2], &v6 );
  zAssert( zVec3DTripleProd2Mat3D,
       m1.e[0][0]==-v1.e[1]*v2.e[1]-v1.e[2]*v2.e[2] && m1.e[1][0]==v1.e[1]*v2.e[0] && m1.e[2][0]==v1.e[2]*v2.e[0]
    && m1.e[0][1]==v1.e[0]*v2.e[1] && m1.e[1][1]==-v1.e[2]*v2.e[2]-v1.e[0]*v2.e[0] && m1.e[2][1]==v1.e[2]*v2.e[1]
    && m1.e[0][2]==v1.e[0]*v2.e[2] && m1.e[1][2]==v1.e[1]*v2.e[2] && m1.e[2][2]==-v1.e[0]*v2.e[0]-v1.e[1]*v2.e[1]
    && zIsTiny(m3.e[0][0]-v4.e[0]) && zIsTiny(m3.e[0][1]-v4.e[1]) && zIsTiny(m3.e[0][2]-v4.e[2])
    && zIsTiny(m3.e[1][0]-v5.e[0]) && zIsTiny(m3.e[1][1]-v5.e[1]) && zIsTiny(m3.e[1][2]-v5.e[2])
    && zIsTiny(m3.e[2][0]-v6.e[0]) && zIsTiny(m3.e[2][1]-v6.e[1]) && zIsTiny(m3.e[2][2]-v6.e[2]) );

  zMulVec3DOPMat3D( &v1, &m2, &m3 );
  zVec3DOuterProd2Mat3D( &v1, &m1 );
  zMulMat3DMat3D( &m1, &m2, &m4 );
  zAssert( zMulVec3DOPMat3D, zMat3DEqual( &m3, &m4 ) );
  zMulVec3DOPMat3DDRC( &v1, &m2 );
  zAssert( zMulVec3DOPMat3DDRC, zMat3DEqual( &m2, &m4 ) );
}

void assert_inv(void)
{
  zMat3D m1, m2, m3;

  zMat3DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DInv( &m1, &m2 );
  zMulMat3DMat3D( &m1, &m2, &m3 );
  zAssert( zMat3DInv, zIsTiny(m3.e[0][0]-1) && zIsTiny(m3.e[1][0]) && zIsTiny(m3.e[2][0])
                   && zIsTiny(m3.e[0][1]) && zIsTiny(m3.e[1][1]-1) && zIsTiny(m3.e[2][1])
                   && zIsTiny(m3.e[0][2]) && zIsTiny(m3.e[1][2]) && zIsTiny(m3.e[2][2]-1) );
}

void assert_mul(void)
{
  zMat3D m1, m2, m3, m4;
  zVec3D v1, v2, v3, w;

  zMat3DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMulMat3DVec3D( &m1, &v1, &v2 );
  zAssert( zMulMat3DVec3D, m1.e[0][0]*v1.e[0]+m1.e[1][0]*v1.e[1]+m1.e[2][0]*v1.e[2]==v2.e[0]
                      && m1.e[0][1]*v1.e[0]+m1.e[1][1]*v1.e[1]+m1.e[2][1]*v1.e[2]==v2.e[1]
                      && m1.e[0][2]*v1.e[0]+m1.e[1][2]*v1.e[1]+m1.e[2][2]*v1.e[2]==v2.e[2] );
  zVec3DCopy( &v1, &v3 ); zMulMat3DVec3DDRC( &m1, &v3 );
  zAssert( zMulMat3DVec3DDRC, v2.e[0]==v3.e[0] && v2.e[1]==v3.e[1] && v2.e[2]==v3.e[2] );
  zMulMat3DTVec3D( &m1, &v1, &v2 );
  zAssert( zMulMat3DTVec3D, m1.e[0][0]*v1.e[0]+m1.e[0][1]*v1.e[1]+m1.e[0][2]*v1.e[2]==v2.e[0]
                       && m1.e[1][0]*v1.e[0]+m1.e[1][1]*v1.e[1]+m1.e[1][2]*v1.e[2]==v2.e[1]
                       && m1.e[2][0]*v1.e[0]+m1.e[2][1]*v1.e[1]+m1.e[2][2]*v1.e[2]==v2.e[2] );
  zVec3DCopy( &v1, &v3 ); zMulMat3DTVec3DDRC( &m1, &v3 );
  zAssert( zMulMat3DTVec3DDRC, v2.e[0]==v3.e[0] && v2.e[1]==v3.e[1] && v2.e[2]==v3.e[2] );

  zMat3DInv( &m1, &m3 );
  zMulInvMat3DVec3D( &m1, &v1, &v2 );
  zMulMat3DVec3D( &m3, &v1, &v3 );
  zAssert( zMulInvMat3DVec3D, zIsTiny(v2.e[0]-v3.e[0]) && zIsTiny(v2.e[1]-v3.e[1]) && zIsTiny(v2.e[2]-v3.e[2]) );

  zVec3DCatRatio( &m1.v[0], &m1.v[1], &m1.v[2], &v1, w.e );
  zMulMat3DVec3D( &m1, &w, &v2 );
  zAssert( zVec3DCatRatio, zIsTiny(v1.e[0]-v2.e[0]) && zIsTiny(v1.e[1]-v2.e[1]) && zIsTiny(v1.e[2]-v2.e[2]) );

  zMulMat3DMat3D( &m1, &m2, &m3 );
  zAssert( zMulMat3DMat3D,
       m1.e[0][0]*m2.e[0][0]+m1.e[1][0]*m2.e[0][1]+m1.e[2][0]*m2.e[0][2]==m3.e[0][0]
    && m1.e[0][1]*m2.e[0][0]+m1.e[1][1]*m2.e[0][1]+m1.e[2][1]*m2.e[0][2]==m3.e[0][1]
    && m1.e[0][2]*m2.e[0][0]+m1.e[1][2]*m2.e[0][1]+m1.e[2][2]*m2.e[0][2]==m3.e[0][2]
    && m1.e[0][0]*m2.e[1][0]+m1.e[1][0]*m2.e[1][1]+m1.e[2][0]*m2.e[1][2]==m3.e[1][0]
    && m1.e[0][1]*m2.e[1][0]+m1.e[1][1]*m2.e[1][1]+m1.e[2][1]*m2.e[1][2]==m3.e[1][1]
    && m1.e[0][2]*m2.e[1][0]+m1.e[1][2]*m2.e[1][1]+m1.e[2][2]*m2.e[1][2]==m3.e[1][2]
    && m1.e[0][0]*m2.e[2][0]+m1.e[1][0]*m2.e[2][1]+m1.e[2][0]*m2.e[2][2]==m3.e[2][0]
    && m1.e[0][1]*m2.e[2][0]+m1.e[1][1]*m2.e[2][1]+m1.e[2][1]*m2.e[2][2]==m3.e[2][1]
    && m1.e[0][2]*m2.e[2][0]+m1.e[1][2]*m2.e[2][1]+m1.e[2][2]*m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m2, &m4 ); zMulMat3DMat3DDRC( &m1, &m4 );
  zAssert( zMulMat3DMat3DDRC,
       m3.e[0][0]==m4.e[0][0] && m3.e[0][1]==m4.e[0][1] && m3.e[0][2]==m4.e[0][2]
    && m3.e[1][0]==m4.e[1][0] && m3.e[1][1]==m4.e[1][1] && m3.e[1][2]==m4.e[1][2]
    && m3.e[2][0]==m4.e[2][0] && m3.e[2][1]==m4.e[2][1] && m3.e[2][2]==m4.e[2][2] );
  zMulMat3DTMat3D( &m1, &m2, &m3 );
  zAssert( zMulMat3DTMat3D,
       m1.e[0][0]*m2.e[0][0]+m1.e[0][1]*m2.e[0][1]+m1.e[0][2]*m2.e[0][2]==m3.e[0][0]
    && m1.e[1][0]*m2.e[0][0]+m1.e[1][1]*m2.e[0][1]+m1.e[1][2]*m2.e[0][2]==m3.e[0][1]
    && m1.e[2][0]*m2.e[0][0]+m1.e[2][1]*m2.e[0][1]+m1.e[2][2]*m2.e[0][2]==m3.e[0][2]
    && m1.e[0][0]*m2.e[1][0]+m1.e[0][1]*m2.e[1][1]+m1.e[0][2]*m2.e[1][2]==m3.e[1][0]
    && m1.e[1][0]*m2.e[1][0]+m1.e[1][1]*m2.e[1][1]+m1.e[1][2]*m2.e[1][2]==m3.e[1][1]
    && m1.e[2][0]*m2.e[1][0]+m1.e[2][1]*m2.e[1][1]+m1.e[2][2]*m2.e[1][2]==m3.e[1][2]
    && m1.e[0][0]*m2.e[2][0]+m1.e[0][1]*m2.e[2][1]+m1.e[0][2]*m2.e[2][2]==m3.e[2][0]
    && m1.e[1][0]*m2.e[2][0]+m1.e[1][1]*m2.e[2][1]+m1.e[1][2]*m2.e[2][2]==m3.e[2][1]
    && m1.e[2][0]*m2.e[2][0]+m1.e[2][1]*m2.e[2][1]+m1.e[2][2]*m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m2, &m4 ); zMulMat3DTMat3DDRC( &m1, &m4 );
  zAssert( zMulMat3DTMat3DDRC,
       m3.e[0][0]==m4.e[0][0] && m3.e[0][1]==m4.e[0][1] && m3.e[0][2]==m4.e[0][2]
    && m3.e[1][0]==m4.e[1][0] && m3.e[1][1]==m4.e[1][1] && m3.e[1][2]==m4.e[1][2]
    && m3.e[2][0]==m4.e[2][0] && m3.e[2][1]==m4.e[2][1] && m3.e[2][2]==m4.e[2][2] );
  zMulMat3DMat3DT( &m1, &m2, &m3 );
  zAssert( zMulMat3DMat3DT,
       m1.e[0][0]*m2.e[0][0]+m1.e[1][0]*m2.e[1][0]+m1.e[2][0]*m2.e[2][0]==m3.e[0][0]
    && m1.e[0][1]*m2.e[0][0]+m1.e[1][1]*m2.e[1][0]+m1.e[2][1]*m2.e[2][0]==m3.e[0][1]
    && m1.e[0][2]*m2.e[0][0]+m1.e[1][2]*m2.e[1][0]+m1.e[2][2]*m2.e[2][0]==m3.e[0][2]
    && m1.e[0][0]*m2.e[0][1]+m1.e[1][0]*m2.e[1][1]+m1.e[2][0]*m2.e[2][1]==m3.e[1][0]
    && m1.e[0][1]*m2.e[0][1]+m1.e[1][1]*m2.e[1][1]+m1.e[2][1]*m2.e[2][1]==m3.e[1][1]
    && m1.e[0][2]*m2.e[0][1]+m1.e[1][2]*m2.e[1][1]+m1.e[2][2]*m2.e[2][1]==m3.e[1][2]
    && m1.e[0][0]*m2.e[0][2]+m1.e[1][0]*m2.e[1][2]+m1.e[2][0]*m2.e[2][2]==m3.e[2][0]
    && m1.e[0][1]*m2.e[0][2]+m1.e[1][1]*m2.e[1][2]+m1.e[2][1]*m2.e[2][2]==m3.e[2][1]
    && m1.e[0][2]*m2.e[0][2]+m1.e[1][2]*m2.e[1][2]+m1.e[2][2]*m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m1, &m4 ); zMulMat3DMat3DTDRC( &m4, &m2 );
  zAssert( zMulMat3DMat3DTDRC,
       m3.e[0][0]==m4.e[0][0] && m3.e[0][1]==m4.e[0][1] && m3.e[0][2]==m4.e[0][2]
    && m3.e[1][0]==m4.e[1][0] && m3.e[1][1]==m4.e[1][1] && m3.e[1][2]==m4.e[1][2]
    && m3.e[2][0]==m4.e[2][0] && m3.e[2][1]==m4.e[2][1] && m3.e[2][2]==m4.e[2][2] );

  zMulInvMat3DMat3D( &m1, &m1, &m3 );
  zAssert( zMat3DInv, zIsTiny(m3.e[0][0]-1) && zIsTiny(m3.e[1][0]) && zIsTiny(m3.e[2][0])
                   && zIsTiny(m3.e[0][1]) && zIsTiny(m3.e[1][1]-1) && zIsTiny(m3.e[2][1])
                   && zIsTiny(m3.e[0][2]) && zIsTiny(m3.e[1][2]) && zIsTiny(m3.e[2][2]-1) );
}

void assert_rot(void)
{
  register int i, testnum = 100;
  zMat3D m1, m2, m3, m4, m5;
  zVec3D r, aa;
  bool result_zyx, result_zyz, result_aa;

  result_zyx = result_zyz = result_aa = true;
  for( i=0; i<testnum; i++ ){
    zVec3DCreate( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zMat3DFromAA( &m1, &r );

    zMat3DToZYX( &m1, &r );
    zMat3DFromZYX( &m2, r.c.x, r.c.y, r.c.z );
    if( !zMat3DEqual( &m1, &m2 ) ) result_zyx = false;
    zMat3DToZYZ( &m1, &r );
    zMat3DFromZYZ( &m2, r.c.x, r.c.y, r.c.z );
    if( !zMat3DEqual( &m1, &m2 ) ) result_zyz = false;
    zMat3DToAA( &m1, &r );
    zMat3DFromAA( &m2, &r );
    if( !zMat3DEqual( &m1, &m2 ) ) result_aa = false;
  }
  zAssert( zMat3DToZYX (random test), result_zyx );
  zAssert( zMat3DToZYZ (random test), result_zyz );
  zAssert( zMat3DToAA (random test), result_aa );

  for( result_zyx=true, i=0; i<testnum; i++ ){
    zVec3DCreate( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zMat3DRotYaw( ZMAT3DIDENT, r.c.x, &m1 );
    zMat3DRotPitch( ZMAT3DIDENT, r.c.y, &m2 );
    zMat3DRotRoll( ZMAT3DIDENT, r.c.z, &m3 );
    zMulMat3DMat3D( &m1, &m2, &m5 );
    zMulMat3DMat3D( &m5, &m3, &m4 );
    zMat3DFromZYX( &m5, r.c.x, r.c.y, r.c.z );
    if( !zMat3DEqual( &m4, &m5 ) ) result_zyx = false;
  }
  zAssert( zMat3DRotYaw + zMat3DRotPitch + zMat3DRotRoll, result_zyx );

  zMat3DIdent( &m1 );
  zMat3DToAA( &m1, &r );
  zAssert( zMat3DToAA (ident test), zVec3DIsTiny(&r) );
  zMat3DCreate( &m1,
    1.0/sqrt(2),-1.0/sqrt(2), 0,
    1.0/sqrt(2), 1.0/sqrt(2), 0,
              0,           0, 1 );
  zMat3DToAA( &m1, &r );
  zAssert( zMat3DToAA (singular non-ident), r.c.x == 0 && r.c.y == 0 && zIsTiny( r.c.z - zDeg2Rad(45) ) );

  for( result_aa=true, i=0; i<testnum; i++ ){
    zVec3DCreate( &aa, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zVec3DCreate( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zMat3DFromAA( &m1, &r );
    zMat3DRot( &m1, &aa, &m2 );
    zVec3DRot( &m1.b.x, &aa, &m3.b.x );
    zVec3DRot( &m1.b.y, &aa, &m3.b.y );
    zVec3DRot( &m1.b.z, &aa, &m3.b.z );
    if( !zMat3DEqual( &m2, &m3 ) ) result_aa = false;
  }
  zAssert( zMat3DRot, result_aa );

  for( result_aa=true, i=0; i<testnum; i++ ){
    zVec3DCreate( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zMat3DFromAA( &m1, &r );
    zVec3DCreate( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zMat3DFromAA( &m2, &r );
    zMat3DError( &m1, &m2, &aa );
    zMat3DRot( &m2, &aa, &m3 );
    if( !zMat3DEqual( &m1, &m3 ) ) result_aa = false;
  }
  zAssert( zMat3DError, result_aa );
}

void assert_sym_eig(void)
{
  register int testnum = 100;
  register int i;
  zMat3D m;
  double xx, yy, zz, xy, yz, zx;
  double eval[3];
  zVec3D evec[3], v[3];
  bool result = true;
  double test_tol = 1.0e-6;

  for( ; testnum>0; testnum-- ){
    xx = zRandF(-10,10);
    yy = zRandF(-10,10);
    zz = zRandF(-10,10);
    xy = zRandF(-10,10);
    yz = zRandF(-10,10);
    zx = zRandF(-10,10);
    zMat3DCreate( &m, xx, xy, zx, xy, yy, yz, zx, yz, zz );
    zMat3DSymEig( &m, eval, evec );
    for( i=0; i<3; i++ ){
      zMulMat3DVec3D( &m, &evec[i], &v[i] );
      zVec3DCatDRC( &v[i], -eval[i], &evec[i] );
      if( !zVec3DIsTol( &v[i], test_tol ) ) result = false;
    }
  }
  zAssert( zMat3DSymEig, result );
}

int main(void)
{
  zRandInit();
  assert_matstruct();
  assert_arith();
  assert_outerprod();
  assert_inv();
  assert_mul();
  assert_rot();
  assert_sym_eig();
  return EXIT_SUCCESS;
}
