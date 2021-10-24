#include <zeo/zeo.h>

void generate_ellips_rand(zEllips3D *ellips)
{
  zVec3D center, ax, ay, az, tmp;

  zVec3DCreate( &center, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DCreate( &ax, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &tmp, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DOrthogonalize( &tmp, &ax, &ay );
  zVec3DOuterProd( &ax, &ay, &az );
  zVec3DNormalizeDRC( &ax );
  zVec3DNormalizeDRC( &ay );
  zVec3DNormalizeDRC( &az );

  zEllips3DCreate( ellips, &center, &ax, &ay, &az, zRandF(0,5), zRandF(0,5), zRandF(0,5), 0 );
}

/* inertia test */

void test_volume_inertia(zEllips3D *ellips, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  zPH3D ph;
  double vol;
  zVec3D bc;
  zMat3D inertia, ierr;

  zEllips3DSetDiv( ellips, div );
  zEllips3DToPH( ellips, &ph );
  vol = zPH3DVolume( &ph );
  zPH3DBaryInertia( &ph, 1, &bc, &inertia );
  zPH3DDestroy( &ph );
  *ev = vol - v0;
  *ei = zMat3DNorm( zMat3DSub( &inertia, i0, &ierr ) );
}

bool eval_volume_inertia(zEllips3D *ellips, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  double evtest, eitest;

  test_volume_inertia( ellips, div, v0, i0, &evtest, &eitest );
  if( evtest / *ev > 0.5 || eitest / *ei > 0.5 ) return false;
  *ev = evtest;
  *ei = eitest;
  return true;
}

void assert_volume_inertia(void)
{
  zEllips3D ellips;
  double vol, ev, ei;
  zMat3D i;
  int div=4, div_max = 2 << 8;
  bool ret = true;

  generate_ellips_rand( &ellips );
  vol = zEllips3DVolume( &ellips );
  zEllips3DInertia( &ellips, 1, &i );
  test_volume_inertia( &ellips, div, vol, &i, &ev, &ei );
  for( div*=2; div<=div_max; div*=2 )
    if( !eval_volume_inertia( &ellips, div, vol, &i, &ev, &ei ) ) ret = false;
  zAssert( zEllips3DVolume + zEllips3DInertia, ret );
}

/* inside/outside test */

bool ellips_cat(zEllips3D *ellips, double x, double y, double z, zVec3D *p)
{
  zVec3DCat( zEllips3DCenter(ellips), x, zEllips3DAxis(ellips,zX), p );
  zVec3DCatDRC( p, y, zEllips3DAxis(ellips,zY) );
  zVec3DCatDRC( p, z, zEllips3DAxis(ellips,zZ) );
  return zSqr(x/zEllips3DRadius(ellips,0)) + zSqr(y/zEllips3DRadius(ellips,1)) + zSqr(z/zEllips3DRadius(ellips,2)) < 1;
}

void assert_inside(void)
{
  zEllips3D ellips;
  zVec3D p;
  double x, y, z;
  register int i;
  int n = 1000, ni, no, nitest, notest;

  generate_ellips_rand( &ellips );
  for( ni=no=nitest=notest=0, i=0; i<n; i++ ){
    x = zRandF(-2,2);
    y = zRandF(-2,2);
    z = zRandF(-1,2);
    ellips_cat( &ellips, x, y, z, &p ) ? ni++ : no++;
    zEllips3DPointIsInside( &ellips, &p, true ) ? nitest++ : notest++;
  }
  zAssert( zEllips3DPointIsInside, nitest == ni && notest == no );
}

int main(void)
{
  zRandInit();
  assert_volume_inertia();
  assert_inside();
  return EXIT_SUCCESS;
}
