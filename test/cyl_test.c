#include <zeo/zeo.h>

void generate_cyl_rand(zCyl3D *cyl)
{
  zVec3D c1, c2;

  zVec3DCreate( &c1, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DCreate( &c2, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zCyl3DCreate( cyl, &c1, &c2, zRandF(0,5), 0 );
}

/* inertia test */

void test_volume_inertia(zCyl3D *cyl, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  zPH3D ph;
  double vol;
  zMat3D inertia, ierr;

  zCyl3DSetDiv( cyl, div );
  zCyl3DToPH( cyl, &ph );
  vol = zPH3DVolume( &ph );
  zPH3DBaryInertia( &ph, 1, &inertia );
  zPH3DDestroy( &ph );
  *ev = vol - v0;
  *ei = zMat3DNorm( zMat3DSub( &inertia, i0, &ierr ) );
}

bool eval_volume_inertia(zCyl3D *cyl, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  double evtest, eitest;

  test_volume_inertia( cyl, div, v0, i0, &evtest, &eitest );
  if( evtest / *ev > 0.5 || eitest / *ei > 0.5 ) return false;
  *ev = evtest;
  *ei = eitest;
  return true;
}

void assert_volume_inertia(void)
{
  zCyl3D cyl;
  double vol, ev, ei;
  zMat3D i;
  int div=4, div_max = 2 << 16;
  bool ret = true;

  generate_cyl_rand( &cyl );
  vol = zCyl3DVolume( &cyl );
  zCyl3DBaryInertia( &cyl, 1, &i );
  test_volume_inertia( &cyl, div, vol, &i, &ev, &ei );
  for( div*=2; div<=div_max; div*=2 )
    if( !eval_volume_inertia( &cyl, div, vol, &i, &ev, &ei ) ) ret = false;
  zAssert( zCyl3DVolume + zCyl3DInertia, ret );
}

/* inside/outside test */

bool cyl_cat(zCyl3D *cyl, zVec3D *d1, zVec3D *d2, zVec3D *d3, double x, double y, double z, zVec3D *p)
{
  zVec3DCat( zCyl3DCenter(cyl,0), x, d2, p );
  zVec3DCatDRC( p, y, d3 );
  zVec3DCatDRC( p, z, d1 );
  return x*x+y*y < zSqr(zCyl3DRadius(cyl)) && z > 0 && z < 1;
}

void assert_inside(void)
{
  zCyl3D cyl;
  zVec3D d1, d2, d3, p;
  double x, y, z;
  register int i;
  int n = 1000, ni, no, nitest, notest;

  generate_cyl_rand( &cyl );
  zCyl3DAxis( &cyl, &d1 );
  zVec3DOrthoSpace( &d1, &d2, &d3 );
  zVec3DNormalizeDRC( &d2 );
  zVec3DNormalizeDRC( &d3 );
  for( ni=no=nitest=notest=0, i=0; i<n; i++ ){
    x = zRandF(-2,2);
    y = zRandF(-2,2);
    z = zRandF(-1,2);
    cyl_cat( &cyl, &d1, &d2, &d3, x, y, z, &p ) ? ni++ : no++;
    zCyl3DPointIsInside( &cyl, &p, true ) ? nitest++ : notest++;
  }
  zAssert( zCyl3DPointIsInside, nitest == ni && notest == no );
}

int main(void)
{
  zRandInit();
  assert_volume_inertia();
  assert_inside();
  return EXIT_SUCCESS;
}
