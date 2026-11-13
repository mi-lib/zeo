#include <zeo/zeo.h>

void generate_ecyl_rand(zECyl3D *ecyl)
{
  zVec3D c1, c2, a, d;

  zVec3DCreate( &c1, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DCreate( &c2, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DSub( &c2, &c1, &a );
  zVec3DCreate( &d, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DOrthogonalize( &d, &a, &d );
  zVec3DNormalizeDRC( &d, 0 );
  zECyl3DCreate( ecyl, &c1, &c2, zRandF(0,5), zRandF(0,5), &d, 0 );
}

/* inertia test */

void test_volume_inertia(zECyl3D *ecyl, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  zPH3D ph;
  double vol;
  zMat3D inertia, ierr;

  zECyl3DSetDiv( ecyl, div );
  zECyl3DToPH( ecyl, &ph );
  vol = zPH3DVolume( &ph );
  zPH3DBaryInertia( &ph, 1, &inertia );
  zPH3DDestroy( &ph );
  *ev = vol - v0;
  *ei = zMat3DNorm( zMat3DSub( &inertia, i0, &ierr ) );
}

bool eval_volume_inertia(zECyl3D *ecyl, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  double evtest, eitest;

  test_volume_inertia( ecyl, div, v0, i0, &evtest, &eitest );
  if( evtest / *ev > 0.5 || eitest / *ei > 0.5 ) return false;
  *ev = evtest;
  *ei = eitest;
  return true;
}

void assert_volume_inertia(void)
{
  zECyl3D ecyl;
  double vol, ev, ei;
  zMat3D i;
  int div=4, div_max = 2 << 16;
  bool ret = true;

  generate_ecyl_rand( &ecyl );
  vol = zECyl3DVolume( &ecyl );
  zECyl3DBaryInertia( &ecyl, 1, &i );
  test_volume_inertia( &ecyl, div, vol, &i, &ev, &ei );
  for( div*=2; div<=div_max; div*=2 )
    if( !eval_volume_inertia( &ecyl, div, vol, &i, &ev, &ei ) ) ret = false;
  zAssert( zECyl3DVolume + zECyl3DInertia, ret );
}

/* inside/outside test */

bool ecyl_cat(zECyl3D *ecyl, double x, double y, double z, zVec3D *p)
{
  zVec3D a;

  zVec3DCat( zECyl3DCenter(ecyl,0), x, zECyl3DRadVec(ecyl,0), p );
  zVec3DCatDRC( p, y, zECyl3DRadVec(ecyl,1) );
  zVec3DCatDRC( p, z, zECyl3DAxis( ecyl, &a ) );
  return zSqr(x/zECyl3DRadius(ecyl,0)) + zSqr(y/zECyl3DRadius(ecyl,1)) < 1 && z > 0 && z < 1;
}

void assert_inside(void)
{
  zECyl3D ecyl;
  zVec3D p;
  double x, y, z;
  int i;
  int n = 1000, ni, no, nitest, notest;

  generate_ecyl_rand( &ecyl );
  for( ni=no=nitest=notest=0, i=0; i<n; i++ ){
    x = zRandF(-2,2);
    y = zRandF(-2,2);
    z = zRandF(-1,2);
    ecyl_cat( &ecyl, x, y, z, &p ) ? ni++ : no++;
    zECyl3DPointIsInside( &ecyl, &p, zTOL ) ? nitest++ : notest++;
  }
  zAssert( zECyl3DPointIsInside, nitest == ni && notest == no );
}

/* check if the radial vectors are orthonormal to the axis, and the first one is on the plane spanned
 * by the axis and the reference vector if specified. */
bool check_ecyl_axis(zECyl3D *ecyl, const zVec3D *ref)
{
  zVec3D axis, r, d;
  zMat3D m;

  zECyl3DAxis( ecyl, &axis );
  zVec3DNormalizeDRC( &axis, 0 );
  zMat3DCreate( &m,
    zECyl3DRadVec(ecyl,0)->c.x, zECyl3DRadVec(ecyl,1)->c.x, axis.c.x,
    zECyl3DRadVec(ecyl,0)->c.y, zECyl3DRadVec(ecyl,1)->c.y, axis.c.y,
    zECyl3DRadVec(ecyl,0)->c.z, zECyl3DRadVec(ecyl,1)->c.z, axis.c.z );
  if( !zMat3DIsOrthonormal( &m ) || !zMat3DIsRightHand( &m ) ) return false;
  if( !ref ) return true;
  zVec3DNormalize( ref, 0, &r );
  zVec3DOuterProd( &axis, &r, &d );
  return zVec3DInnerProd( &r, zECyl3DRadVec(ecyl,0) ) > 0 && zIsTiny( zVec3DInnerProd( &d, zECyl3DRadVec(ecyl,0) ) );
}

void assert_def_axis(void)
{
  zECyl3D ecyl, org;
  zVec3D c1, c2, ref;
  const int n = 1000;
  double scale[] = { 1.0e-9, 1.0e-6, -1.0, 1.0e6 };
  int i, j;
  bool result = true, result_auto = true, result_parallel = true, result_fail;

  for( i=0; i<n; i++ ){
    zVec3DCreate( &c1, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
    zVec3DCreate( &c2, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
    zVec3DCreate( &ref, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
    zVec3DMulDRC( &ref, scale[i%(sizeof(scale)/sizeof(double))] );
    if( !zECyl3DCreate( &ecyl, &c1, &c2, 1, 2, &ref, 0 ) || !check_ecyl_axis( &ecyl, &ref ) ) result = false;
    if( !zECyl3DCreate( &ecyl, &c1, &c2, 1, 2, NULL, 0 ) || !check_ecyl_axis( &ecyl, NULL ) ) result_auto = false;
    if( !zECyl3DCreate( &ecyl, &c1, &c2, 1, 2, ZVEC3DZERO, 0 ) || !check_ecyl_axis( &ecyl, NULL ) ) result_auto = false;
    /* reference vector parallel to the axis */
    for( j=0; j<sizeof(scale)/sizeof(double); j++ ){
      zVec3DSub( &c2, &c1, &ref );
      zVec3DMulDRC( &ref, scale[j] );
      if( !zECyl3DCreate( &ecyl, &c1, &c2, 1, 2, &ref, 0 ) || !check_ecyl_axis( &ecyl, NULL ) ) result_parallel = false;
    }
  }
  /* identical centers */
  zECyl3DCopy( &ecyl, &org );
  zECyl3DSetCenter( &ecyl, 1, zECyl3DCenter(&ecyl,0) );
  result_fail = !zECyl3DDefAxis( &ecyl, &ref ) &&
    memcmp( zECyl3DRadVec(&ecyl,0), zECyl3DRadVec(&org,0), sizeof(zVec3D) ) == 0 &&
    memcmp( zECyl3DRadVec(&ecyl,1), zECyl3DRadVec(&org,1), sizeof(zVec3D) ) == 0 &&
    !zECyl3DCreate( &ecyl, &c1, &c1, 1, 2, &ref, 0 );
  zAssert( zECyl3DDefAxis, result );
  zAssert( zECyl3DDefAxis (automatic definition), result_auto );
  zAssert( zECyl3DDefAxis (reference vector parallel to the axis), result_parallel );
  zAssert( zECyl3DDefAxis (identical centers), result_fail );
}

int main(void)
{
  zRandInit();
  assert_def_axis();
  assert_volume_inertia();
  assert_inside();
  return EXIT_SUCCESS;
}
