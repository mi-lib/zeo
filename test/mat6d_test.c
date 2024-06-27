#include <zeo/zeo.h>

void assert_create(void)
{
  zMat3D m1, m2, m3, m4;
  zMat6D m61, m62;
  zVec6D v1, v2, v3, v4, v5, v6;
  int i, j, k, l;
  bool result;

  zMat3DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m3, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m4, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat6DCreate( &m61, &m1, &m2, &m3, &m4 );
  zAssert( zMat6DCreate,
       zMat3DMatch(&m1,&m61.m[0]) && zMat3DMatch(&m1,&m61.e[0][0]) && zMat3DMatch(&m1,&m61.c.ll)
    && zMat3DMatch(&m2,&m61.m[2]) && zMat3DMatch(&m2,&m61.e[1][0]) && zMat3DMatch(&m2,&m61.c.al)
    && zMat3DMatch(&m3,&m61.m[1]) && zMat3DMatch(&m3,&m61.e[0][1]) && zMat3DMatch(&m3,&m61.c.la)
    && zMat3DMatch(&m4,&m61.m[3]) && zMat3DMatch(&m4,&m61.e[1][1]) && zMat3DMatch(&m4,&m61.c.aa) );
  zMat6DRow( &m61, 0, &v1 );
  zMat6DRow( &m61, 1, &v2 );
  zMat6DRow( &m61, 2, &v3 );
  zMat6DRow( &m61, 3, &v4 );
  zMat6DRow( &m61, 4, &v5 );
  zMat6DRow( &m61, 5, &v6 );
  zAssert( zMat6DRow,
       v1.e[0]==m1.e[0][0] && v1.e[1]==m1.e[1][0] && v1.e[2]==m1.e[2][0] && v1.e[3]==m2.e[0][0] && v1.e[4]==m2.e[1][0] && v1.e[5]==m2.e[2][0]
    && v2.e[0]==m1.e[0][1] && v2.e[1]==m1.e[1][1] && v2.e[2]==m1.e[2][1] && v2.e[3]==m2.e[0][1] && v2.e[4]==m2.e[1][1] && v2.e[5]==m2.e[2][1]
    && v3.e[0]==m1.e[0][2] && v3.e[1]==m1.e[1][2] && v3.e[2]==m1.e[2][2] && v3.e[3]==m2.e[0][2] && v3.e[4]==m2.e[1][2] && v3.e[5]==m2.e[2][2]
    && v4.e[0]==m3.e[0][0] && v4.e[1]==m3.e[1][0] && v4.e[2]==m3.e[2][0] && v4.e[3]==m4.e[0][0] && v4.e[4]==m4.e[1][0] && v4.e[5]==m4.e[2][0]
    && v5.e[0]==m3.e[0][1] && v5.e[1]==m3.e[1][1] && v5.e[2]==m3.e[2][1] && v5.e[3]==m4.e[0][1] && v5.e[4]==m4.e[1][1] && v5.e[5]==m4.e[2][1]
    && v6.e[0]==m3.e[0][2] && v6.e[1]==m3.e[1][2] && v6.e[2]==m3.e[2][2] && v6.e[3]==m4.e[0][2] && v6.e[4]==m4.e[1][2] && v6.e[5]==m4.e[2][2] );
  zMat6DCol( &m61, 0, &v1 );
  zMat6DCol( &m61, 1, &v2 );
  zMat6DCol( &m61, 2, &v3 );
  zMat6DCol( &m61, 3, &v4 );
  zMat6DCol( &m61, 4, &v5 );
  zMat6DCol( &m61, 5, &v6 );
  zAssert( zMat6DCol,
       zVec3DMatch(zVec6DLin(&v1),&m1.b.x) && zVec3DMatch(zVec6DAng(&v1),&m3.b.x)
    && zVec3DMatch(zVec6DLin(&v2),&m1.b.y) && zVec3DMatch(zVec6DAng(&v2),&m3.b.y)
    && zVec3DMatch(zVec6DLin(&v3),&m1.b.z) && zVec3DMatch(zVec6DAng(&v3),&m3.b.z)
    && zVec3DMatch(zVec6DLin(&v4),&m2.b.x) && zVec3DMatch(zVec6DAng(&v4),&m4.b.x)
    && zVec3DMatch(zVec6DLin(&v5),&m2.b.y) && zVec3DMatch(zVec6DAng(&v5),&m4.b.y)
    && zVec3DMatch(zVec6DLin(&v6),&m2.b.z) && zVec3DMatch(zVec6DAng(&v6),&m4.b.z) );
  zMat6DT( &m61, &m62 );
  for( result=true, i=0; i<2; i++ )
    for( j=0; j<2; j++ )
      for( k=0; k<3; k++ )
        for( l=0; l<3; l++ )
          if( m61.e[i][j].e[k][l] != m62.e[j][i].e[l][k] ) result = false;
  zAssert( zMat6DT, result );
}

void assert_arith(void)
{
  zMat3D m1, m2, m3, m4, m5, m6, m7, m8;
  zMat6D m61, m62, m63, m64;
  double s;
  int i, j, k, l;
  bool result;

  zMat3DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m3, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m4, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m5, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m6, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m7, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m8, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat6DCreate( &m61, &m1, &m2, &m3, &m4 );
  zMat6DCreate( &m62, &m5, &m6, &m7, &m8 );
  s = zRandF(-10,10);

  zMat6DAdd( &m61, &m62, &m63 );
  for( result=true, i=0; i<2; i++ )
    for( j=0; j<2; j++ )
      for( k=0; k<3; k++ )
        for( l=0; l<3; l++ )
          if( m61.e[i][j].e[k][l] + m62.e[i][j].e[k][l] != m63.e[i][j].e[k][l] ) result = false;
  zAssert( zMat6DAdd, result );
  zMat6DCopy( &m61, &m64 ); zMat6DAddDRC( &m64, &m62 );
  zAssert( zMat6DAddDRC,
       zMat3DMatch(&m63.m[0],&m64.m[0]) && zMat3DMatch(&m63.m[1],&m64.m[1])
    && zMat3DMatch(&m63.m[2],&m64.m[2]) && zMat3DMatch(&m63.m[3],&m64.m[3]) );

  zMat6DSub( &m61, &m62, &m63 );
  for( result=true, i=0; i<2; i++ )
    for( j=0; j<2; j++ )
      for( k=0; k<3; k++ )
        for( l=0; l<3; l++ )
          if( m61.e[i][j].e[k][l] - m62.e[i][j].e[k][l] != m63.e[i][j].e[k][l] ) result = false;
  zAssert( zMat6DSub, result );

  zMat6DCopy( &m61, &m64 ); zMat6DSubDRC( &m64, &m62 );
  zAssert( zMat6DSubDRC,
       zMat3DMatch(&m63.m[0],&m64.m[0]) && zMat3DMatch(&m63.m[1],&m64.m[1])
    && zMat3DMatch(&m63.m[2],&m64.m[2]) && zMat3DMatch(&m63.m[3],&m64.m[3]) );

  zMat6DMul( &m61, s, &m63 );
  for( result=true, i=0; i<2; i++ )
    for( j=0; j<2; j++ )
      for( k=0; k<3; k++ )
        for( l=0; l<3; l++ )
          if( m61.e[i][j].e[k][l] * s != m63.e[i][j].e[k][l] ) result = false;
  zAssert( zMat6DMul, result );
  zMat6DCopy( &m61, &m64 ); zMat6DMulDRC( &m64, s );
  zAssert( zMat6DMulDRC,
       zMat3DMatch(&m63.m[0],&m64.m[0]) && zMat3DMatch(&m63.m[1],&m64.m[1])
    && zMat3DMatch(&m63.m[2],&m64.m[2]) && zMat3DMatch(&m63.m[3],&m64.m[3]) );

  zMat6DDiv( &m61, s, &m63 );
  for( result=true, i=0; i<2; i++ )
    for( j=0; j<2; j++ )
      for( k=0; k<3; k++ )
        for( l=0; l<3; l++ )
          if( !zIsTiny( m61.e[i][j].e[k][l] / s - m63.e[i][j].e[k][l] ) ) result = false;
  zAssert( zMat6DDiv, result );
  zMat6DCopy( &m61, &m64 ); zMat6DDivDRC( &m64, s );
  zAssert( zMat6DDivDRC,
       zMat3DMatch(&m63.m[0],&m64.m[0]) && zMat3DMatch(&m63.m[1],&m64.m[1])
    && zMat3DMatch(&m63.m[2],&m64.m[2]) && zMat3DMatch(&m63.m[3],&m64.m[3]) );
}

void assert_mul_mat_vec(void)
{
  zMat3D m1, m2, m3, m4, m5, m6, m7, m8, ma1, ma2, ma3, ma4, mt1, mt2;
  zMat6D m61, m62, m63;
  zVec3D v1, v2, v3, v4, v5, v6;
  zVec6D v61, v62;

  zMat3DCreate( &m1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m3, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m4, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m5, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m6, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m7, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat3DCreate( &m8, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zMat6DCreate( &m61, &m1, &m2, &m3, &m4 );
  zMat6DCreate( &m62, &m5, &m6, &m7, &m8 );
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec6DFromVec3D( &v61, &v1, &v2 );

  zMulMat6DVec6D( &m61, &v61, &v62 );
  zMulMat3DVec3D( &m1, &v1, &v3 );
  zMulMat3DVec3D( &m2, &v2, &v4 );
  zVec3DAdd( &v3, &v4, &v5 );
  zMulMat3DVec3D( &m3, &v1, &v3 );
  zMulMat3DVec3D( &m4, &v2, &v4 );
  zVec3DAdd( &v3, &v4, &v6 );
  zAssert( zMulMat6DVec6D, zVec3DMatch(&v5,zVec6DLin(&v62)) && zVec3DMatch(&v6,zVec6DAng(&v62)) );

  zMulMat6DTVec6D( &m61, &v61, &v62 );
  zMulMat3DTVec3D( &m1, &v1, &v3 );
  zMulMat3DTVec3D( &m3, &v2, &v4 );
  zVec3DAdd( &v3, &v4, &v5 );
  zMulMat3DTVec3D( &m2, &v1, &v3 );
  zMulMat3DTVec3D( &m4, &v2, &v4 );
  zVec3DAdd( &v3, &v4, &v6 );
  zAssert( zMulMat6DTVec6D, zVec3DMatch(&v5,zVec6DLin(&v62)) && zVec3DMatch(&v6,zVec6DAng(&v62)) );

  zMulMat6DMat6D( &m61, &m62, &m63 );
  zMulMat3DMat3D( &m1, &m5, &mt1 );
  zMulMat3DMat3D( &m2, &m7, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma1 );
  zMulMat3DMat3D( &m1, &m6, &mt1 );
  zMulMat3DMat3D( &m2, &m8, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma2 );
  zMulMat3DMat3D( &m3, &m5, &mt1 );
  zMulMat3DMat3D( &m4, &m7, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma3 );
  zMulMat3DMat3D( &m3, &m6, &mt1 );
  zMulMat3DMat3D( &m4, &m8, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma4 );
  zAssert( zMulMat6DMat6D,
       zMat3DMatch(&m63.c.ll,&ma1) && zMat3DMatch(&m63.c.al,&ma2)
    && zMat3DMatch(&m63.c.la,&ma3) && zMat3DMatch(&m63.c.aa,&ma4) );

  zMulMat6DTMat6D( &m61, &m62, &m63 );
  zMulMat3DTMat3D( &m1, &m5, &mt1 );
  zMulMat3DTMat3D( &m3, &m7, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma1 );
  zMulMat3DTMat3D( &m1, &m6, &mt1 );
  zMulMat3DTMat3D( &m3, &m8, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma2 );
  zMulMat3DTMat3D( &m2, &m5, &mt1 );
  zMulMat3DTMat3D( &m4, &m7, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma3 );
  zMulMat3DTMat3D( &m2, &m6, &mt1 );
  zMulMat3DTMat3D( &m4, &m8, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma4 );
  zAssert( zMulMat6DTMat6D,
       zMat3DMatch(&m63.c.ll,&ma1) && zMat3DMatch(&m63.c.al,&ma2)
    && zMat3DMatch(&m63.c.la,&ma3) && zMat3DMatch(&m63.c.aa,&ma4) );

  zMulMat6DMat6DT( &m61, &m62, &m63 );
  zMulMat3DMat3DT( &m1, &m5, &mt1 );
  zMulMat3DMat3DT( &m2, &m6, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma1 );
  zMulMat3DMat3DT( &m1, &m7, &mt1 );
  zMulMat3DMat3DT( &m2, &m8, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma2 );
  zMulMat3DMat3DT( &m3, &m5, &mt1 );
  zMulMat3DMat3DT( &m4, &m6, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma3 );
  zMulMat3DMat3DT( &m3, &m7, &mt1 );
  zMulMat3DMat3DT( &m4, &m8, &mt2 );
  zMat3DAdd( &mt1, &mt2, &ma4 );
  zAssert( zMulMat6DMat6DT,
       zMat3DMatch(&m63.c.ll,&ma1) && zMat3DMatch(&m63.c.al,&ma2)
    && zMat3DMatch(&m63.c.la,&ma3) && zMat3DMatch(&m63.c.aa,&ma4) );

  zVec3DCreate( &v3, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v4, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec6DFromVec3D( &v62, &v3, &v4 );
  zMat6DDyad( &m61, &v61, &v62 );
  zMat3DDyad( &ma1, &v1, &v3 );
  zMat3DDyad( &ma2, &v1, &v4 );
  zMat3DDyad( &ma3, &v2, &v3 );
  zMat3DDyad( &ma4, &v2, &v4 );
  zAssert( zMat6DDyad,
       zMat3DMatch(&m61.c.ll,&ma1) && zMat3DMatch(&m61.c.al,&ma2)
    && zMat3DMatch(&m61.c.la,&ma3) && zMat3DMatch(&m61.c.aa,&ma4) );
}

int main(void)
{
  zRandInit();
  assert_create();
  assert_arith();
  assert_mul_mat_vec();
  return EXIT_SUCCESS;
}
