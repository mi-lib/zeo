#include <zeo/zeo.h>

void generate_ecyl_rand(zECyl3D *ecyl)
{
  zVec3D c1, c2, a, d;

  zVec3DCreate( &c1, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DCreate( &c2, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DSub( &c2, &c1, &a );
  zVec3DCreate( &d, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DOrthogonalize( &d, &a, &d );
  zVec3DNormalizeDRC( &d );
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
  register int i;
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

int main(void)
{
  zRandInit();
  assert_volume_inertia();
  assert_inside();
  return EXIT_SUCCESS;
}
