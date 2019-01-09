#include <zeo/zeo.h>

void assert_aa(void)
{
  zVec3D aa, test_aa;
  zEP ep, test_ep;

  zVec3DCreate( &aa, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DNormalizeDRC( &aa );
  zVec3DMulDRC( &aa, zRandF(0,zPI) );

  zAA2EP( &aa, &ep );
  zEP2AA( &ep, &test_aa );
  zAssert( zAA2EP -> zEP2AA, zVec3DEqual( &aa, &test_aa ) );

  zEPCreate( &ep, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zEP2AA( &ep, &aa );
  zAA2EP( &aa, &test_ep );
  zAssert( zEP2AA -> zAA2EP, zIsTiny(ep.ex.w-test_ep.ex.w) && zVec3DEqual(&ep.ex.v,&test_ep.ex.v) );
}

void assert_mat(void)
{
  zMat3D m1, m2;
  zVec3D aa;
  zEP ep;

  zVec3DCreate( &aa, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zMat3DFromAA( &m1, &aa );
  zAA2EP( &aa, &ep );
  zMat3DFromEP( &m2, &ep );
  zAssert( zMat3DFromEP, zMat3DEqual(&m1,&m2) );

  zMat3DToEP( &m1, &ep );
  zMat3DFromEP( &m2, &ep );
  zAssert( zMat3DToEP, zMat3DEqual(&m1,&m2) );
}

void assert_rotate(void)
{
  zVec3D aa, v1, v2, v3, v4;
  zEP ep;
  zMat3D m;

  zVec3DCreate( &aa, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DNormalizeDRC( &aa );
  zVec3DMulDRC( &aa, zRandF(0,zPI) );
  zAA2EP( &aa, &ep );
  zMat3DFromAA( &m, &aa );

  zVec3DCreate( &v1, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DRot( &v1, &aa, &v2 );
  zEPRotVec3D( &ep, &v1, &v3 );
  zMulMat3DVec3D( &m, &v1, &v4 );
  zAssert( zEPRotVec3D, zVec3DEqual(&v2,&v3) && zVec3DEqual(&v3,&v4) );
}

void assert_vel(void)
{
  zEP ep, epvel;
  zVec3D angvel, test_angvel;

  zEPCreate( &ep, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &angvel, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );

  zAngVel2EPVel( &angvel, &ep, &epvel );
  zEPVel2AngVel( &epvel, &ep, &test_angvel );
  zAssert( zEPVel2AngVel -> zAngVel2EPVel, zVec3DEqual(&angvel,&test_angvel) );
}

void assert_cascade(void)
{
  zEP ep1, ep2, ep3;
  zMat3D m1, m2, m3, test_m;

  zEPCreate( &ep1, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zEPCreate( &ep2, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zMat3DFromEP( &m1, &ep1 );
  zMat3DFromEP( &m2, &ep2 );
  zEPCascade( &ep1, &ep2, &ep3 );
  zMulMat3DMat3D( &m2, &m1, &m3 );
  zMat3DFromEP( &test_m, &ep3 );
  zAssert( zEPCascade, zMat3DEqual( &m3, &test_m ) );
}

int main(void)
{
  zRandInit();
  assert_aa();
  assert_mat();
  assert_rotate();
  assert_vel();
  assert_cascade();
  return EXIT_SUCCESS;
}
