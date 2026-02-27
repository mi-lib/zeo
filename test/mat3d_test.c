#include <zeo/zeo.h>

void vec3D_create_rand(zVec3D *v)
{
  zVec3DCreate( v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
}

void vec3D_create_aa_rand(zVec3D *a)
{
  zVec3DCreate( a, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

void mat3D_create_rand(zMat3D *m)
{
  zMat3DCreate( m,
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
    zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
}

void assert_mat3D_struct(void)
{
  zMat3D m, mc, tm;
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
  zMat3DCopy( &m, &mc );
  zMat3DT( &mc, &mc );
  zAssert( zMat3DT (direct transpose), zMat3DEqual( &tm, &mc ) );
  zMat3DTDRC( &m );
  zAssert( zMat3DTDRC, zMat3DEqual( &tm, &m ) );
}

void assert_mat3D_symmetrize(void)
{
  zMat3D m1, m2;
  zVec3D v, u;
  double qval1, qval2;
  int i;
  const int n = 1000;
  bool result1 = true, result2 = true, result3 = true;

  for( i=0; i<n; i++ ){
    mat3D_create_rand( &m1 );
    vec3D_create_rand( &v );
    zMulMat3DVec3D( &m1, &v, &u );
    qval1 = zVec3DInnerProd( &v, &u );
    zMat3DSymmetrize( &m1, &m2 );
    if( !zMat3DIsSymmetric( &m2 ) ) result3 = false;
    zMulMat3DVec3D( &m2, &v, &u );
    qval2 = zVec3DInnerProd( &v, &u );
    if( !zIsTiny( qval1 - qval2 ) ) result1 = false;
    zMat3DSymmetrizeDRC( &m1 );
    if( !zMat3DEqual( &m1, &m2 ) ) result2 = false;
  }
  zAssert( zMat3DSymmetrize, result1 );
  zAssert( zMat3DSymmetrizeDRC, result2 );
  zAssert( zMat3DIsSymmetric, result3 );
}

void assert_mat3D_arith(void)
{
  zMat3D m1, m2, m3;
  zVec3D v1, v2;
  double k;

  mat3D_create_rand( &m1 );
  mat3D_create_rand( &m2 );
  vec3D_create_rand( &v1 );
  vec3D_create_rand( &v2 );
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

void assert_mat3D_outerprod(void)
{
  zVec3D v1, v2, v3, v4, v5, v6;
  zMat3D m1, m2, m3, m4;
  double k;

  mat3D_create_rand( &m2 );
  vec3D_create_rand( &v1 );
  vec3D_create_rand( &v2 );
  vec3D_create_rand( &v3 );
  k = zRandF(-10,10);
  zVec3DOuterProdToMat3D( &v1, &m1 );
  zMulMat3DMat3D( &m1, &m2, &m3 );
  zVec3DOuterProd( &v1, &m2.v[0], &v4 );
  zVec3DOuterProd( &v1, &m2.v[1], &v5 );
  zVec3DOuterProd( &v1, &m2.v[2], &v6 );
  zAssert( zVec3DOuterProdToMat3D,
       m1.e[0][0]==0 && m1.e[1][0]==-v1.e[2] && m1.e[2][0]==v1.e[1]
    && m1.e[0][1]==v1.e[2] && m1.e[1][1]==0 && m1.e[2][1]==-v1.e[0]
    && m1.e[0][2]==-v1.e[1] && m1.e[1][2]==v1.e[0] && m1.e[2][2]==0
    && m3.e[0][0]==v4.e[0] && m3.e[0][1]==v4.e[1] && m3.e[0][2]==v4.e[2]
    && m3.e[1][0]==v5.e[0] && m3.e[1][1]==v5.e[1] && m3.e[1][2]==v5.e[2]
    && m3.e[2][0]==v6.e[0] && m3.e[2][1]==v6.e[1] && m3.e[2][2]==v6.e[2] );
  zVec3DTripleProdToMat3D( &v1, &v2, &m1 );
  zMulMat3DMat3D( &m1, &m2, &m3 );
  zVec3DTripleProd( &v1, &v2, &m2.v[0], &v4 );
  zVec3DTripleProd( &v1, &v2, &m2.v[1], &v5 );
  zVec3DTripleProd( &v1, &v2, &m2.v[2], &v6 );
  zAssert( zVec3DTripleProdToMat3D,
       m1.e[0][0]==-v1.e[1]*v2.e[1]-v1.e[2]*v2.e[2] && m1.e[1][0]==v1.e[1]*v2.e[0] && m1.e[2][0]==v1.e[2]*v2.e[0]
    && m1.e[0][1]==v1.e[0]*v2.e[1] && m1.e[1][1]==-v1.e[2]*v2.e[2]-v1.e[0]*v2.e[0] && m1.e[2][1]==v1.e[2]*v2.e[1]
    && m1.e[0][2]==v1.e[0]*v2.e[2] && m1.e[1][2]==v1.e[1]*v2.e[2] && m1.e[2][2]==-v1.e[0]*v2.e[0]-v1.e[1]*v2.e[1]
    && zIsTiny(m3.e[0][0]-v4.e[0]) && zIsTiny(m3.e[0][1]-v4.e[1]) && zIsTiny(m3.e[0][2]-v4.e[2])
    && zIsTiny(m3.e[1][0]-v5.e[0]) && zIsTiny(m3.e[1][1]-v5.e[1]) && zIsTiny(m3.e[1][2]-v5.e[2])
    && zIsTiny(m3.e[2][0]-v6.e[0]) && zIsTiny(m3.e[2][1]-v6.e[1]) && zIsTiny(m3.e[2][2]-v6.e[2]) );
  zVec3DTripleProdToMat3D( &v1, &v1, &m3 );
  zVec3DDoubleOuterProdToMat3D( &v1, &m4 );
  zAssert( zVec3DDoubleOuterProdMat3D, zMat3DEqual( &m3, &m4 ) );
  zMat3DCatVec3DDoubleOuterProd( &m2, k, &v1, &m3 );
  zVec3DDoubleOuterProdToMat3D( &v1, &m1 );
  zMat3DCat( &m2, k, &m1, &m4 );
  zAssert( zMat3DCatVec3DDoubleOuterProd, zMat3DEqual( &m3, &m4 ) );
  zMulVec3DOuterProdMat3D( &v1, &m2, &m3 );
  zVec3DOuterProdToMat3D( &v1, &m1 );
  zMulMat3DMat3D( &m1, &m2, &m4 );
  zAssert( zMulVec3DOuterProdMat3D, zMat3DEqual( &m3, &m4 ) );
  zMulVec3DOuterProdMat3DDRC( &v1, &m2 );
  zAssert( zMulVec3DOuterProdMat3DDRC, zMat3DEqual( &m2, &m4 ) );
}

void assert_mat3D_is_orthonormal(void)
{
  zMat3D m;
  zVec3D aa;
  int i;
  const int n = 1000;
  bool result1 = true, result2 = true, result3 = true, result4 = true;

  for( i=0; i<n; i++ ){
    zVec3DCreate( &aa, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zMat3DFromAA( &m, &aa );
    if( !zMat3DIsOrthonormal( &m ) ) result1 = false;
    if( !zMat3DIsRightHand( &m ) ) result2 = false;
    zVec3DCatDRC( zMat3DVec(&m,0), 0.5, zMat3DVec(&m,1) );
    if( zMat3DIsOrthonormal( &m ) ) result3 = false;
    zVec3DRevDRC( zMat3DVec(&m,2) );
    if( zMat3DIsRightHand( &m ) ) result4 = false;
  }
  zAssert( zMat3DIsOrthonormal (positive cases), result1 );
  zAssert( zMat3DIsOrthonormal (negative cases), result3 );
  zAssert( zMat3DIsRightHand (positive cases), result2 );
  zAssert( zMat3DIsRightHand (negative cases), result4 );
}

void assert_mat3D_orthonormalize(void)
{
  zMat3D m1, m2, *ret;
  zAxis axis1, axis2;
  const int n = 20;
  int i;
  bool result1 = true, result2 = true;

  for( i=0; i<n; i++ ){
    zMat3DCreate( &m1,
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    axis1 = zRandI( zX, zZ );
    axis2 = zRandI( zX, zZ );
    ret = zMat3DOrthonormalize( &m1, axis1, axis2, &m2 );
    if( ret == &m2 ){
      if( !zMat3DIsOrthonormal( &m2 ) ) result1 = false;
      zMat3DOrthonormalizeDRC( &m1, axis1, axis2 );
      if( !zMat3DEqual( &m1, &m2 ) ) result2 = false;
    } else{
      if( axis1 != axis2 ) result1 = false;
    }
  }
  zAssert( zMat3DOrthonormalize, result1 );
  zAssert( zMat3DOrthonormalizeDRC, result2 );
}

void assert_mat3D_mul(void)
{
  zMat3D m1, m2, m3, m4;
  zVec3D v1, v2, v3, w;

  mat3D_create_rand( &m1 );
  mat3D_create_rand( &m2 );
  vec3D_create_rand( &v1 );
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
  zAssert( zMulInvMat3DMat3D,
       zIsTiny(m3.e[0][0]-1) && zIsTiny(m3.e[1][0]) && zIsTiny(m3.e[2][0])
    && zIsTiny(m3.e[0][1]) && zIsTiny(m3.e[1][1]-1) && zIsTiny(m3.e[2][1])
    && zIsTiny(m3.e[0][2]) && zIsTiny(m3.e[1][2]) && zIsTiny(m3.e[2][2]-1) );
}

void assert_mat3D_adj(void)
{
  zMat3D mat, adj, mul_adj_mat, det_diag, mat2, mul_adj_mat2;
  zVec3D vec, adj_vec1, adj_vec2;
  int i;
  const int n = 1000;
  bool result1 = true, result2 = true, result3 = true;

  /* non-regular case */
  zMat3DCreate( &mat,
    1, 2, 3,
    1, 2, 3,
    1, 2, 3 );
  zMat3DAdj( &mat, &adj );
  zMulMat3DMat3D( &mat, &adj, &mul_adj_mat );
  zAssert( zMat3DAdj (non-regular case), zMat3DIsTiny( &mul_adj_mat ) );
  /* general case */
  for( i=0; i<n; i++ ){
    zMat3DCreate( &mat,
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zMat3DAdj( &mat, &adj );
    zMulMat3DMat3D( &mat, &adj, &mul_adj_mat );
    zMat3DMul( ZMAT3DIDENT, zMat3DDet( &mat ), &det_diag );
    if( !zMat3DEqual( &mul_adj_mat, &det_diag ) ) result1 = false;

    zVec3DCreate( &vec, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zMulAdjMat3DVec3D( &mat, &vec, &adj_vec1 );
    zMulMat3DVec3D( &adj, &vec, &adj_vec2 );
    if( !zVec3DEqual( &adj_vec1, &adj_vec2 ) ) result2 = false;

    zMat3DCreate( &mat2,
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10),
      zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zMulMat3DMat3D( &adj, &mat2, &mul_adj_mat );
    zMulAdjMat3DMat3D( &mat, &mat2, &mul_adj_mat2 );
    if( !zMat3DEqual( &mul_adj_mat, &mul_adj_mat2 ) ) result3 = false;
  }
  zAssert( zMat3DAdj, result1 );
  zAssert( zMulAdjMat3DVec3D, result2 );
  zAssert( zMulAdjMat3DMat3D, result3 );
}

void assert_mat3D_inv(void)
{
  int i, testnum = 100;
  zMat3D m, im, m2, m3, m2_test, me;
  zVec3D v1, v2, v1_test, e;
  double mag1 = 1.0e-6, mag2 = 1.0e-6;
  const double tol = 1.0e-6;
  bool result_mul_inv_vec, result_mul_inv_mat, result_mat_inv;

  result_mul_inv_vec = result_mul_inv_mat = result_mat_inv = true;
  for( i=0; i<testnum; i++ ){
    zMat3DCreate( &m,
      zRandF(-1,1), zRandF(-1,1)*mag2, zRandF(-1,1),
      zRandF(-1,1)*mag1, zRandF(-1,1)*mag1*mag2, zRandF(-1,1)*mag1,
      zRandF(-1,1), zRandF(-1,1)*mag2, zRandF(-1,1) );
    mat3D_create_rand( &m2 );
    vec3D_create_rand( &v1 );

    zMulInvMat3DVec3D( &m, &v1, &v2 );
    zMulMat3DVec3D( &m, &v2, &v1_test );
    zVec3DSub( &v1, &v1_test, &e );
    if( !zVec3DIsTol( &e, tol ) ) result_mul_inv_vec = false;

    zMulInvMat3DMat3D( &m, &m2, &m3 );
    zMulMat3DMat3D( &m, &m3, &m2_test );
    zMat3DSub( &m2, &m2_test, &me );
    if( !zMat3DIsTol( &me, tol ) ) result_mul_inv_mat = false;

    zMat3DInv( &m, &im );
    zMulMat3DMat3D( &m, &im, &me );
    zMat3DSubDRC( &me, ZMAT3DIDENT );
    if( !zMat3DIsTol( &me, tol ) ) result_mat_inv = false;
  }
  zAssert( zMulInvMat3DVec3D (severe case), result_mul_inv_vec );
  zAssert( zMulInvMat3DMat3D (severe case), result_mul_inv_mat );
  zAssert( zMul3DInv (severe case), result_mat_inv );
}

void assert_mat3D_eulerangles(void)
{
  int i, testnum = 100;
  zMat3D m1, m2, m3, m4, m5;
  zVec3D r;
  bool result_zyx, result_zyz;

  result_zyx = result_zyz = true;
  for( i=0; i<testnum; i++ ){
    vec3D_create_aa_rand( &r );
    zMat3DFromAA( &m1, &r );

    zMat3DToZYX( &m1, &r );
    zMat3DFromZYX( &m2, r.c.x, r.c.y, r.c.z );
    if( !zMat3DEqual( &m1, &m2 ) ) result_zyx = false;
    zMat3DToZYZ( &m1, &r );
    zMat3DFromZYZ( &m2, r.c.x, r.c.y, r.c.z );
    if( !zMat3DEqual( &m1, &m2 ) ) result_zyz = false;
  }
  zAssert( zMat3DToZYX (random test), result_zyx );
  zAssert( zMat3DToZYZ (random test), result_zyz );

  for( result_zyx=true, i=0; i<testnum; i++ ){
    vec3D_create_aa_rand( &r );
    zMat3DRotYaw( ZMAT3DIDENT, r.c.x, &m1 );
    zMat3DRotPitch( ZMAT3DIDENT, r.c.y, &m2 );
    zMat3DRotRoll( ZMAT3DIDENT, r.c.z, &m3 );
    zMulMat3DMat3D( &m1, &m2, &m5 );
    zMulMat3DMat3D( &m5, &m3, &m4 );
    zMat3DFromZYX( &m5, r.c.x, r.c.y, r.c.z );
    if( !zMat3DEqual( &m4, &m5 ) ) result_zyx = false;
  }
  zAssert( zMat3DRotYaw + zMat3DRotPitch + zMat3DRotRoll / zMat3DFromZYX, result_zyx );
  for( result_zyz=true, i=0; i<testnum; i++ ){
    vec3D_create_aa_rand( &r );
    zMat3DRotYaw( ZMAT3DIDENT, r.c.x, &m1 );
    zMat3DRotPitch( ZMAT3DIDENT, r.c.y, &m2 );
    zMat3DRotYaw( ZMAT3DIDENT, r.c.z, &m3 );
    zMulMat3DMat3D( &m1, &m2, &m5 );
    zMulMat3DMat3D( &m5, &m3, &m4 );
    zMat3DFromZYZ( &m5, r.c.x, r.c.y, r.c.z );
    if( !zMat3DEqual( &m4, &m5 ) ) result_zyz = false;
  }
  zAssert( zMat3DRotYaw + zMat3DRotPitch + zMat3DRotYaw / zMat3DFromZYZ, result_zyz );
}

void assert_mat3D_to_angleaxis(void)
{
  zVec3D aa;
  zMat3D rot_src, rot_dest;
  bool result = true;
  const int n = 100;
  int i;

  /* random cases */
  for( i=0; i<n; i++ ){
    zVec3DCreate( &aa, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zMat3DFromAA( &rot_src, &aa );
    zMat3DToAA( &rot_src, &aa );
    zMat3DFromAA( &rot_dest, &aa );
    if( !zMat3DEqual( &rot_dest, &rot_src ) ) result = false;
  }
  zAssert( zMat3DFromAA + zMat3DToAA (regular cases), result );
  /* singular cases */
  for( i=0; i<n; i++ ){
    zVec3DCreate( &aa, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DNormalizeDRC( &aa );
    zMat3DCreate( &rot_src,
      2*aa.c.x*aa.c.x - 1, 2*aa.c.x*aa.c.y, 2*aa.c.x*aa.c.z,
      2*aa.c.y*aa.c.x, 2*aa.c.y*aa.c.y - 1, 2*aa.c.y*aa.c.z,
      2*aa.c.z*aa.c.x, 2*aa.c.z*aa.c.y, 2*aa.c.z*aa.c.z - 1 );
    zMat3DToAA( &rot_src, &aa );
    zMat3DFromAA( &rot_dest, &aa );
    if( !zMat3DEqual( &rot_dest, &rot_src ) ) result = false;
  }
  zAssert( zMat3DFromAA + zMat3DToAA (singular cases), result );
  /* identity case */
  zMat3DIdent( &rot_src );
  zMat3DToAA( &rot_src, &aa );
  zAssert( zMat3DFromAA + zMat3DToAA (identity cases), zVec3DIsTiny( &aa ) );
}

void assert_mat3D_rot(void)
{
  int i, testnum = 100;
  zMat3D m1, m2, m3;
  zVec3D r, aa, aa1, aa2;
  bool result;

  for( result=true, i=0; i<testnum; i++ ){
    vec3D_create_aa_rand( &aa );
    vec3D_create_aa_rand( &r );
    zMat3DFromAA( &m1, &r );
    zMat3DRot( &m1, &aa, &m2 );
    zVec3DRot( &m1.b.x, &aa, &m3.b.x );
    zVec3DRot( &m1.b.y, &aa, &m3.b.y );
    zVec3DRot( &m1.b.z, &aa, &m3.b.z );
    if( !zMat3DEqual( &m2, &m3 ) ) result = false;
  }
  zAssert( zMat3DRot, result );

  for( result=true, i=0; i<testnum; i++ ){
    vec3D_create_aa_rand( &aa );
    vec3D_create_aa_rand( &r );
    zMat3DFromAA( &m1, &r );
    zMat3DRot( &m1, &aa, &m2 );
    zMat3DRotDRC( &m1, &aa );
    if( !zMat3DEqual( &m1, &m2 ) ) result = false;
  }
  zAssert( zMat3DRotDRC, result );

  for( result=true, i=0; i<testnum; i++ ){
    vec3D_create_aa_rand( &r );
    zMat3DFromAA( &m1, &r );
    vec3D_create_aa_rand( &r );
    zMat3DFromAA( &m2, &r );
    zMat3DError( &m1, &m2, &aa );
    zMat3DRot( &m2, &aa, &m3 );
    if( !zMat3DEqual( &m1, &m3 ) ) result = false;
  }
  zAssert( zMat3DError, result );

  for( result=true, i=0; i<testnum; i++ ){
    vec3D_create_aa_rand( &aa1 );
    vec3D_create_aa_rand( &aa2 );
    zAAError( &aa2, &aa1, &r );
    zAACascade( &aa1, &r, &aa );
    if( !zVec3DIsTiny( zAAError( &aa2, &aa, &r ) ) ) result = false;
  }
  zAssert( zAAError + zAACascade, result );
}

void assert_mat3D_eig_power(void)
{
  zMat3D m, eigbase;
  zVec3D v, eigval, error;
  double lambda, lambda1, lambda2;
  const int num = 10, testnum = 100;
  int i, k;
  const double tol1 = 1.0e-10, tol2 = 1.0e-6;
  bool result1 = true, result2 = true;

  for( k=0; k<testnum; k++ ){
    zMat3DZero( &m );
    for( i=0; i<num; i++ ){
      zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
      zMat3DAddDyad( &m, &v, &v );
    }
    zMat3DSymEig( &m, &eigval, &eigbase );
    lambda1 = zMax3( eigval.e[0], eigval.e[1], eigval.e[2] );
    lambda2 = zMin3( eigval.e[0], eigval.e[1], eigval.e[2] );

    lambda = zMat3DEigPower( &m, &v, 0 );
    if( !zEqual( lambda, lambda1, zTOL ) ){
      eprintf( "different largest principal component: %g / %g\n", lambda, lambda1 );
      result1 = false;
    }
    zMulMat3DVec3D( &m, &v, &error );
    zVec3DCatDRC( &error, -lambda, &v );
    if( !zVec3DIsTol( &error, tol1 ) ){
      eprintf( "error for largest principal component: " );
      zVec3DFPrint( stderr, &error );
      result1 = false;
    }
    lambda = zMat3DEigPowerInv( &m, &v, 0 );
    if( !zEqual( lambda, lambda2, zTOL ) ){
      eprintf( "different smallest principal component: %g / %g\n", lambda, lambda2 );
      result2 = false;
    }
    zMulMat3DVec3D( &m, &v, &error );
    zVec3DCatDRC( &error, -lambda, &v );
    if( !zVec3DIsTol( &error, tol2 ) ){
      eprintf( "error for smallest principal component: " );
      zVec3DFPrint( stderr, &error );
      result2 = false;
    }
  }
  zAssert( zMat3DEigPower, result1 );
  zAssert( zMat3DEigPowerInv, result2 );
}

void assert_mat3D_sym_eig(void)
{
  int i, testnum = 100;
  zMat3D m, eigbase;
  double xx, yy, zz, xy, yz, zx;
  zVec3D eigval, v[3];
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
    zMat3DSymEig( &m, &eigval, &eigbase );
    for( i=0; i<3; i++ ){
      zMulMat3DVec3D( &m, &eigbase.v[i], &v[i] );
      zVec3DCatDRC( &v[i], -eigval.e[i], &eigbase.v[i] );
      if( !zVec3DIsTol( &v[i], test_tol ) ) result = false;
    }
  }
  zAssert( zMat3DSymEig, result );
}

void assert_mat3D_sym_eig_min(void)
{
  zMat3D m;
  zVec3D v, *eigvec1, *eigvec2, error;
  const int testnum = 1000;
  const int num = 100;
  int i, k;
  const double tol = 1.0e-6;
  clock_t c1, c2;
  long c_jacobi, c_power;
  bool result = true;

  eigvec1 = zAlloc( zVec3D, testnum );
  eigvec2 = zAlloc( zVec3D, testnum );
  zMat3DZero( &m );
  for( i=0; i<num; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zMat3DAddDyad( &m, &v, &v );
  }
  c1 = clock();
  for( k=0; k<testnum; k++ )
    zMat3DSymEigMin( &m, &eigvec1[k] );
  c2 = clock();
  c_jacobi = c2 - c1;
  c1 = clock();
  for( k=0; k<testnum; k++ )
    zMat3DEigPowerInv( &m, &eigvec2[k], 0 );
  c2 = clock();
  c_power = c2 - c1;
  eprintf( "zMat3DSymEigMin   clock=%ld\n", c_jacobi );
  eprintf( "zMat3DEigPowerInv clock=%ld\n", c_power );
  for( k=0; k<testnum; k++ )
    if( !zVec3DIsTol( zVec3DSub( &eigvec1[k], &eigvec2[k], &error ), tol ) &&
        !zVec3DIsTol( zVec3DAdd( &eigvec1[k], &eigvec2[k], &error ), tol ) ){
      zVec3DFPrint( stderr, &eigvec1[k] );
      zVec3DFPrint( stderr, &eigvec2[k] );
      result = false;
    }
  free( eigvec1 );
  free( eigvec2 );
  zAssert( zMat3DSymEigMin / zMat3DEigPowerInv, result );
}

bool check_mat3D_svd(const zMat3D *m, const zMat3D *u, const zVec3D *s, const zMat3D *v)
{
  zMat3D us, m2;
  int i;

  for( i=0; i<3; i++ )
    zVec3DMul( zMat3DVec(u,i), s->e[i], zMat3DVec(&us,i) );
  zMulMat3DMat3DT( &us, v, &m2 );
  return zMat3DEqual( m, &m2 );
}

void assert_mat3D_svd(void)
{
  zMat3D m, u, v;
  zVec3D s;
  int i, testnum = 1000;
  bool result = true;

  for( i=0; i<testnum; i++ ){
    zMat3DCreate( &m,
      zRandF(-5,5), zRandF(-5,5), zRandF(-5,5),
      zRandF(-5,5), zRandF(-5,5), zRandF(-5,5),
      zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zMat3DSVD( &m, &u, &s, &v );
    if( !check_mat3D_svd( &m, &u, &s, &v ) ) result = false;
  }
  zAssert( zMat3DSVD, result );
  zMat3DCreate( &m, zRandF(-5,5), 0, 0, 0, zRandF(-5,5), 0, 0, 0, 0 );
  zMat3DSVD( &m, &u, &s, &v );
  zAssert( zMat3DSVD (singular case), check_mat3D_svd( &m, &u, &s, &v ) );
}

void assert_mat3D_ztk(void)
{
  ZTK ztk;
  zMat3D mat_src, mat_dest;

  zMat3DCreate( &mat_src, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  ZTKInit( &ztk );
  ZTKAddTag( &ztk, "" );
  ZTKAddKey( &ztk, "" );
  zMat3DToZTK( &mat_src, &ztk );
  ZTKRewind( &ztk );
  zMat3DFromZTK( &mat_dest, &ztk );
  ZTKDestroy( &ztk );
  zMat3DSubDRC( &mat_src, &mat_dest );
  zAssert( zMat3DFromZTK + zMat3DToZTK, zMat3DIsTol( &mat_src, 1.0e-9 ) );
}

int main(void)
{
  zRandInit();
  assert_mat3D_struct();
  assert_mat3D_symmetrize();
  assert_mat3D_arith();
  assert_mat3D_outerprod();
  assert_mat3D_is_orthonormal();
  assert_mat3D_orthonormalize();
  assert_mat3D_adj();
  assert_mat3D_inv();
  assert_mat3D_mul();
  assert_mat3D_eulerangles();
  assert_mat3D_to_angleaxis();
  assert_mat3D_rot();
  assert_mat3D_eig_power();
  assert_mat3D_sym_eig();
  assert_mat3D_sym_eig_min();
  assert_mat3D_svd();
  assert_mat3D_ztk();
  return EXIT_SUCCESS;
}
