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
  zMat3D m1, m2, m3;

  zMat3DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v3, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DOuterProd2Mat3D( &v1, &m1 );
  zMulMatMat3D( &m1, &m2, &m3 );
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
  zMulMatMat3D( &m1, &m2, &m3 );
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
}

void assert_inv(void)
{
  zMat3D m1, m2, m3;

  zMat3DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DInv( &m1, &m2 );
  zMulMatMat3D( &m1, &m2, &m3 );
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
  zMulMatVec3D( &m1, &v1, &v2 );
  zAssert( zMulMatVec3D, m1.e[0][0]*v1.e[0]+m1.e[1][0]*v1.e[1]+m1.e[2][0]*v1.e[2]==v2.e[0]
                      && m1.e[0][1]*v1.e[0]+m1.e[1][1]*v1.e[1]+m1.e[2][1]*v1.e[2]==v2.e[1]
                      && m1.e[0][2]*v1.e[0]+m1.e[1][2]*v1.e[1]+m1.e[2][2]*v1.e[2]==v2.e[2] );
  zVec3DCopy( &v1, &v3 ); zMulMatVec3DDRC( &m1, &v3 );
  zAssert( zMulMatVec3DDRC, v2.e[0]==v3.e[0] && v2.e[1]==v3.e[1] && v2.e[2]==v3.e[2] );
  zMulMatTVec3D( &m1, &v1, &v2 );
  zAssert( zMulMatTVec3D, m1.e[0][0]*v1.e[0]+m1.e[0][1]*v1.e[1]+m1.e[0][2]*v1.e[2]==v2.e[0]
                       && m1.e[1][0]*v1.e[0]+m1.e[1][1]*v1.e[1]+m1.e[1][2]*v1.e[2]==v2.e[1]
                       && m1.e[2][0]*v1.e[0]+m1.e[2][1]*v1.e[1]+m1.e[2][2]*v1.e[2]==v2.e[2] );
  zVec3DCopy( &v1, &v3 ); zMulMatTVec3DDRC( &m1, &v3 );
  zAssert( zMulMatTVec3DDRC, v2.e[0]==v3.e[0] && v2.e[1]==v3.e[1] && v2.e[2]==v3.e[2] );

  zMat3DInv( &m1, &m3 );
  zMulInvMatVec3D( &m1, &v1, &v2 );
  zMulMatVec3D( &m3, &v1, &v3 );
  zAssert( zMulInvMatVec3D, zIsTiny(v2.e[0]-v3.e[0]) && zIsTiny(v2.e[1]-v3.e[1]) && zIsTiny(v2.e[2]-v3.e[2]) );

  zVec3DCatRatio( &m1.v[0], &m1.v[1], &m1.v[2], &v1, w.e );
  zMulMatVec3D( &m1, &w, &v2 );
  zAssert( zVec3DCatRatio, zIsTiny(v1.e[0]-v2.e[0]) && zIsTiny(v1.e[1]-v2.e[1]) && zIsTiny(v1.e[2]-v2.e[2]) );

  zMulMatMat3D( &m1, &m2, &m3 );
  zAssert( zMulMatMat3D,
       m1.e[0][0]*m2.e[0][0]+m1.e[1][0]*m2.e[0][1]+m1.e[2][0]*m2.e[0][2]==m3.e[0][0]
    && m1.e[0][1]*m2.e[0][0]+m1.e[1][1]*m2.e[0][1]+m1.e[2][1]*m2.e[0][2]==m3.e[0][1]
    && m1.e[0][2]*m2.e[0][0]+m1.e[1][2]*m2.e[0][1]+m1.e[2][2]*m2.e[0][2]==m3.e[0][2]
    && m1.e[0][0]*m2.e[1][0]+m1.e[1][0]*m2.e[1][1]+m1.e[2][0]*m2.e[1][2]==m3.e[1][0]
    && m1.e[0][1]*m2.e[1][0]+m1.e[1][1]*m2.e[1][1]+m1.e[2][1]*m2.e[1][2]==m3.e[1][1]
    && m1.e[0][2]*m2.e[1][0]+m1.e[1][2]*m2.e[1][1]+m1.e[2][2]*m2.e[1][2]==m3.e[1][2]
    && m1.e[0][0]*m2.e[2][0]+m1.e[1][0]*m2.e[2][1]+m1.e[2][0]*m2.e[2][2]==m3.e[2][0]
    && m1.e[0][1]*m2.e[2][0]+m1.e[1][1]*m2.e[2][1]+m1.e[2][1]*m2.e[2][2]==m3.e[2][1]
    && m1.e[0][2]*m2.e[2][0]+m1.e[1][2]*m2.e[2][1]+m1.e[2][2]*m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m2, &m4 ); zMulMatMat3DDRC( &m1, &m4 );
  zAssert( zMulMatMat3DDRC,
       m3.e[0][0]==m4.e[0][0] && m3.e[0][1]==m4.e[0][1] && m3.e[0][2]==m4.e[0][2]
    && m3.e[1][0]==m4.e[1][0] && m3.e[1][1]==m4.e[1][1] && m3.e[1][2]==m4.e[1][2]
    && m3.e[2][0]==m4.e[2][0] && m3.e[2][1]==m4.e[2][1] && m3.e[2][2]==m4.e[2][2] );
  zMulMatTMat3D( &m1, &m2, &m3 );
  zAssert( zMulMatTMat3D,
       m1.e[0][0]*m2.e[0][0]+m1.e[0][1]*m2.e[0][1]+m1.e[0][2]*m2.e[0][2]==m3.e[0][0]
    && m1.e[1][0]*m2.e[0][0]+m1.e[1][1]*m2.e[0][1]+m1.e[1][2]*m2.e[0][2]==m3.e[0][1]
    && m1.e[2][0]*m2.e[0][0]+m1.e[2][1]*m2.e[0][1]+m1.e[2][2]*m2.e[0][2]==m3.e[0][2]
    && m1.e[0][0]*m2.e[1][0]+m1.e[0][1]*m2.e[1][1]+m1.e[0][2]*m2.e[1][2]==m3.e[1][0]
    && m1.e[1][0]*m2.e[1][0]+m1.e[1][1]*m2.e[1][1]+m1.e[1][2]*m2.e[1][2]==m3.e[1][1]
    && m1.e[2][0]*m2.e[1][0]+m1.e[2][1]*m2.e[1][1]+m1.e[2][2]*m2.e[1][2]==m3.e[1][2]
    && m1.e[0][0]*m2.e[2][0]+m1.e[0][1]*m2.e[2][1]+m1.e[0][2]*m2.e[2][2]==m3.e[2][0]
    && m1.e[1][0]*m2.e[2][0]+m1.e[1][1]*m2.e[2][1]+m1.e[1][2]*m2.e[2][2]==m3.e[2][1]
    && m1.e[2][0]*m2.e[2][0]+m1.e[2][1]*m2.e[2][1]+m1.e[2][2]*m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m2, &m4 ); zMulMatTMat3DDRC( &m1, &m4 );
  zAssert( zMulMatTMat3DDRC,
       m3.e[0][0]==m4.e[0][0] && m3.e[0][1]==m4.e[0][1] && m3.e[0][2]==m4.e[0][2]
    && m3.e[1][0]==m4.e[1][0] && m3.e[1][1]==m4.e[1][1] && m3.e[1][2]==m4.e[1][2]
    && m3.e[2][0]==m4.e[2][0] && m3.e[2][1]==m4.e[2][1] && m3.e[2][2]==m4.e[2][2] );
  zMulMatMatT3D( &m1, &m2, &m3 );
  zAssert( zMulMatMatT3D,
       m1.e[0][0]*m2.e[0][0]+m1.e[1][0]*m2.e[1][0]+m1.e[2][0]*m2.e[2][0]==m3.e[0][0]
    && m1.e[0][1]*m2.e[0][0]+m1.e[1][1]*m2.e[1][0]+m1.e[2][1]*m2.e[2][0]==m3.e[0][1]
    && m1.e[0][2]*m2.e[0][0]+m1.e[1][2]*m2.e[1][0]+m1.e[2][2]*m2.e[2][0]==m3.e[0][2]
    && m1.e[0][0]*m2.e[0][1]+m1.e[1][0]*m2.e[1][1]+m1.e[2][0]*m2.e[2][1]==m3.e[1][0]
    && m1.e[0][1]*m2.e[0][1]+m1.e[1][1]*m2.e[1][1]+m1.e[2][1]*m2.e[2][1]==m3.e[1][1]
    && m1.e[0][2]*m2.e[0][1]+m1.e[1][2]*m2.e[1][1]+m1.e[2][2]*m2.e[2][1]==m3.e[1][2]
    && m1.e[0][0]*m2.e[0][2]+m1.e[1][0]*m2.e[1][2]+m1.e[2][0]*m2.e[2][2]==m3.e[2][0]
    && m1.e[0][1]*m2.e[0][2]+m1.e[1][1]*m2.e[1][2]+m1.e[2][1]*m2.e[2][2]==m3.e[2][1]
    && m1.e[0][2]*m2.e[0][2]+m1.e[1][2]*m2.e[1][2]+m1.e[2][2]*m2.e[2][2]==m3.e[2][2] );
  zMat3DCopy( &m1, &m4 ); zMulMatMatT3DDRC( &m4, &m2 );
  zAssert( zMulMatMatT3DDRC,
       m3.e[0][0]==m4.e[0][0] && m3.e[0][1]==m4.e[0][1] && m3.e[0][2]==m4.e[0][2]
    && m3.e[1][0]==m4.e[1][0] && m3.e[1][1]==m4.e[1][1] && m3.e[1][2]==m4.e[1][2]
    && m3.e[2][0]==m4.e[2][0] && m3.e[2][1]==m4.e[2][1] && m3.e[2][2]==m4.e[2][2] );

  zMulInvMatMat3D( &m1, &m1, &m3 );
  zAssert( zMat3DInv, zIsTiny(m3.e[0][0]-1) && zIsTiny(m3.e[1][0]) && zIsTiny(m3.e[2][0])
                   && zIsTiny(m3.e[0][1]) && zIsTiny(m3.e[1][1]-1) && zIsTiny(m3.e[2][1])
                   && zIsTiny(m3.e[0][2]) && zIsTiny(m3.e[1][2]) && zIsTiny(m3.e[2][2]-1) );
}

int main(void)
{
  zRandInit();
  assert_matstruct();
  assert_arith();
  assert_outerprod();
  assert_inv();
  assert_mul();
  return EXIT_SUCCESS;
}
