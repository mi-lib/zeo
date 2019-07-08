#include <zeo/zeo.h>

void assert_create(void)
{
  zVec3D v1, v2;
  double r, theta, phi;

  zVec3DCreate( &v1, 1, 2, 3 );
  zAssert( zVec3DCreate, v1.e[0] == 1 && v1.e[1] == 2 && v1.e[2] == 3 );
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  r = zVec3DNorm( &v1 );
  theta = zVec3DAngle( &v1, ZVEC3DZ, NULL );
  phi = atan2( v1.e[1], v1.e[0] );
  zVec3DCreatePolar( &v2, r, theta, phi );
  zAssert( zVec3DCreatePolar, zVec3DEqual( &v1, &v2 ) );
  zVec3DCopy( &v1, &v2 );
  zAssert( zVec3DCopy, v1.e[0] == v2.e[0] && v1.e[1] == v2.e[1] && v1.e[2] == v2.e[2] );
  zAssert( zVec3DMatch, zVec3DMatch( &v1, &v2 ) );
  zVec3DZero( &v1 );
  zAssert( zVec3DIsTiny, zVec3DIsTiny( &v1 ) );
  zVec3DCreate( &v1, 1, 2, HUGE_VAL );
  zVec3DCreate( &v2, 1, 2, NAN );
  zAssert( zVec3DIsNan, zVec3DIsNan(&v1) && zVec3DIsNan(&v2) );
}

void assert_arith(void)
{
  zVec3D v1, v2, v3;
  double k;

  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  k = zRandF(-10,10);
  zVec3DAdd( &v1, &v2, &v3 );
  zAssert( zVec3DAdd, v1.e[0]+v2.e[0]==v3.e[0] && v1.e[1]+v2.e[1]==v3.e[1] && v1.e[2]+v2.e[2]==v3.e[2] );
  zVec3DSub( &v1, &v2, &v3 );
  zAssert( zVec3DSub, v1.e[0]-v2.e[0]==v3.e[0] && v1.e[1]-v2.e[1]==v3.e[1] && v1.e[2]-v2.e[2]==v3.e[2] );
  zVec3DRev( &v1, &v3 );
  zAssert( zVec3DRev, v1.e[0]+v3.e[0]==0 && v1.e[1]+v3.e[1]==0 && v1.e[2]+v3.e[2]==0 );
  zVec3DMul( &v1, k, &v3 );
  zAssert( zVec3DMul, v1.e[0]*k==v3.e[0] && v1.e[1]*k==v3.e[1] && v1.e[2]*k==v3.e[2] );
  zVec3DDiv( &v1, k, &v3 );
  zAssert( zVec3DDiv, zIsTiny(v1.e[0]/k-v3.e[0]) && zIsTiny(v1.e[1]/k-v3.e[1]) && zIsTiny(v1.e[2]/k-v3.e[2]) );
  zVec3DAmp( &v1, &v2, &v3 );
  zAssert( zVec3DAmp, v1.e[0]*v2.e[0]==v3.e[0] && v1.e[1]*v2.e[1]==v3.e[1] && v1.e[2]*v2.e[2]==v3.e[2] );
  zVec3DCat( &v1, k, &v2, &v3 );
  zAssert( zVec3DCat, v1.e[0]+k*v2.e[0]==v3.e[0] && v1.e[1]+k*v2.e[1]==v3.e[1] && v1.e[2]+k*v2.e[2]==v3.e[2] );

  zVec3DCopy( &v1, &v3 ); zVec3DAddDRC( &v3, &v2 );
  zAssert( zVec3DAddDRC, v1.e[0]+v2.e[0]==v3.e[0] && v1.e[1]+v2.e[1]==v3.e[1] && v1.e[2]+v2.e[2]==v3.e[2] );
  zVec3DCopy( &v1, &v3 ); zVec3DSubDRC( &v3, &v2 );
  zAssert( zVec3DSubDRC, v1.e[0]-v2.e[0]==v3.e[0] && v1.e[1]-v2.e[1]==v3.e[1] && v1.e[2]-v2.e[2]==v3.e[2] );
  zVec3DCopy( &v1, &v3 ); zVec3DRevDRC( &v3 );
  zAssert( zVec3DRevDRC, v1.e[0]+v3.e[0]==0 && v1.e[1]+v3.e[1]==0 && v1.e[2]+v3.e[2]==0 );
  zVec3DCopy( &v1, &v3 ); zVec3DMulDRC( &v3, k );
  zAssert( zVec3DMulDRC, v1.e[0]*k==v3.e[0] && v1.e[1]*k==v3.e[1] && v1.e[2]*k==v3.e[2] );
  zVec3DCopy( &v1, &v3 ); zVec3DDivDRC( &v3, k );
  zAssert( zVec3DDivDRC, zIsTiny(v1.e[0]/k-v3.e[0]) && zIsTiny(v1.e[1]/k-v3.e[1]) && zIsTiny(v1.e[2]/k-v3.e[2]) );
  zVec3DCopy( &v1, &v3 ); zVec3DAmpDRC( &v3, &v2 );
  zAssert( zVec3DAmpDRC, v1.e[0]*v2.e[0]==v3.e[0] && v1.e[1]*v2.e[1]==v3.e[1] && v1.e[2]*v2.e[2]==v3.e[2] );
  zVec3DCopy( &v1, &v3 ); zVec3DCatDRC( &v3, k, &v2 );
  zAssert( zVec3DCatDRC, v1.e[0]+k*v2.e[0]==v3.e[0] && v1.e[1]+k*v2.e[1]==v3.e[1] && v1.e[2]+k*v2.e[2]==v3.e[2] );
}

void assert_vecprod(void)
{
  zVec3D v1, v2, v3, v4, v5, v6, v7;

  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAssert( zVec3DInnerProd, v1.e[0]*v2.e[0]+v1.e[1]*v2.e[1]+v1.e[2]*v2.e[2] == zVec3DInnerProd(&v1,&v2) );
  zVec3DOuterProd( &v1, &v2, &v3 );
  zVec3DCopy( &v1, &v4 ); zVec3DOuterProd( &v4, &v2, &v4 );
  zVec3DCopy( &v2, &v5 ); zVec3DOuterProd( &v1, &v5, &v5 );
  zAssert( zVec3DOuterProd,
       v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1] == v3.e[0]
    && v1.e[2]*v2.e[0]-v1.e[0]*v2.e[2] == v3.e[1]
    && v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0] == v3.e[2]
    && v4.e[0]==v3.e[0] && v4.e[1]==v3.e[1] && v4.e[2]==v3.e[2]
    && v5.e[0]==v3.e[0] && v5.e[1]==v3.e[1] && v5.e[2]==v3.e[2] );
  zAssert( zVec3DOuterProdNorm, zVec3DOuterProdNorm(&v1,&v2) == zVec3DNorm(&v3) );
  zVec3DCreate( &v3, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAssert( zVec3DGrassmannProd, zIsTiny(zVec3DGrassmannProd(&v1,&v2,&v3)-((v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1])*v3.e[0]+(v1.e[2]*v2.e[0]-v1.e[0]*v2.e[2])*v3.e[1]+(v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0])*v3.e[2])) );
  zVec3DTripleProd( &v1, &v2, &v3, &v4 );
  zVec3DCopy( &v1, &v5 ); zVec3DTripleProd( &v5, &v2, &v3, &v5 );
  zVec3DCopy( &v2, &v6 ); zVec3DTripleProd( &v1, &v6, &v3, &v6 );
  zVec3DCopy( &v3, &v7 ); zVec3DTripleProd( &v1, &v2, &v7, &v7 );
  zAssert( zVec3DTripleProd,
       zIsTiny(v1.e[1]*v2.e[0]*v3.e[1]-v1.e[1]*v2.e[1]*v3.e[0]-v1.e[2]*v2.e[2]*v3.e[0]+v1.e[2]*v2.e[0]*v3.e[2]-v4.e[0])
    && zIsTiny(v1.e[2]*v2.e[1]*v3.e[2]-v1.e[2]*v2.e[2]*v3.e[1]-v1.e[0]*v2.e[0]*v3.e[1]+v1.e[0]*v2.e[1]*v3.e[0]-v4.e[1])
    && zIsTiny(v1.e[0]*v2.e[2]*v3.e[0]-v1.e[0]*v2.e[0]*v3.e[2]-v1.e[1]*v2.e[1]*v3.e[2]+v1.e[1]*v2.e[2]*v3.e[1]-v4.e[2])
    && v4.e[0]==v5.e[0] && v4.e[1]==v5.e[1] && v4.e[2]==v5.e[2]
    && v4.e[0]==v6.e[0] && v4.e[1]==v6.e[1] && v4.e[2]==v6.e[2]
    && v4.e[0]==v7.e[0] && v4.e[1]==v7.e[1] && v4.e[2]==v7.e[2] );

  zVec3DNormalize( &v1, &v2 );
  zAssert( zVec3DNormalize, zIsTiny(v2.e[0]*v2.e[0]+v2.e[1]*v2.e[1]+v2.e[2]*v2.e[2]-1) );
  zVec3DNormalizeDRC( &v1 );
  zAssert( zVec3DNormalizeDRC, zIsTiny(v1.e[0]*v1.e[0]+v1.e[1]*v1.e[1]+v1.e[2]*v1.e[2]-1) );
}

void assert_geometry(void)
{
  zVec3D v1, v2, v3, v4;

  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DProj( &v1, &v2, &v3 );
  zVec3DOuterProd( &v2, &v3, &v4 );
  zAssert( zVec3DProj, zIsTiny(zVec3DInnerProd(&v1,&v3)-zVec3DSqrNorm(&v3)) && zVec3DIsTiny(&v4) );
  zVec3DOrthogonalize( &v1, &v2, &v3 );
  zAssert( zVec3DOrthogonalize, zIsTiny(zVec3DInnerProd(&v2,&v3)) );
  zVec3DOrthoSpace( &v1, &v2, &v3 );
  zAssert( zVec3DOrthoSpace, zIsTiny(zVec3DInnerProd(&v1,&v2)) && zIsTiny(zVec3DInnerProd(&v2,&v3)) && zIsTiny(zVec3DInnerProd(&v3,&v1)) );
  zVec3DRot( &v1, &v2, &v3 );
  zVec3DRevDRC( &v2 );
  zAssert( zVec3DRot + zVec3DAngle, zIsTiny(zVec3DAngle(&v1,&v3,&v2)+zVec3DNorm(&v2)) );
}

void assert_angvel_conv(void)
{
  zVec3D ea, eavel, angvel, test;

  zVec3DCreate( &ea, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zVec3DCreate( &angvel, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );

  zAngVelToZYXVel( &angvel, &ea, &eavel );
  zZYXVelToAngVel( &eavel, &ea, &test );
  zAssert( zAngVelToZYXVel + zZYXVelToAngVel, zVec3DEqual(&angvel,&test) );

  zAngVelToZYZVel( &angvel, &ea, &eavel );
  zZYZVelToAngVel( &eavel, &ea, &angvel );
  zAssert( zAngVelToZYZVel + zZYZVelToAngVel, zVec3DEqual(&angvel,&test) );
}

int main(void)
{
  zRandInit();
  assert_create();
  assert_arith();
  assert_vecprod();
  assert_geometry();
  assert_angvel_conv();
  return EXIT_SUCCESS;
}
