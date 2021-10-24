#include <zeo/zeo.h>

void generate_sphere_rand(zSphere3D *sphere)
{
  zVec3D center;

  zVec3DCreate( &center, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zSphere3DCreate( sphere, &center, zRandF(0,5), 0 );
}

/* inertia test */

void test_volume_inertia(zSphere3D *sphere, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  zPH3D ph;
  double vol;
  zVec3D bc;
  zMat3D inertia, ierr;

  zSphere3DSetDiv( sphere, div );
  zSphere3DToPH( sphere, &ph );
  vol = zPH3DVolume( &ph );
  zPH3DBaryInertia( &ph, 1, &bc, &inertia );
  zPH3DDestroy( &ph );
  *ev = vol - v0;
  *ei = zMat3DNorm( zMat3DSub( &inertia, i0, &ierr ) );
}

bool eval_volume_inertia(zSphere3D *sphere, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  double evtest, eitest;

  test_volume_inertia( sphere, div, v0, i0, &evtest, &eitest );
  if( evtest / *ev > 0.5 || eitest / *ei > 0.5 ) return false;
  *ev = evtest;
  *ei = eitest;
  return true;
}

void assert_volume_inertia(void)
{
  zSphere3D sphere;
  double vol, ev, ei;
  zMat3D i;
  int div=4, div_max = 2 << 8;
  bool ret = true;

  generate_sphere_rand( &sphere );
  vol = zSphere3DVolume( &sphere );
  zSphere3DInertia( &sphere, 1, &i );
  test_volume_inertia( &sphere, div, vol, &i, &ev, &ei );
  for( div*=2; div<=div_max; div*=2 )
    if( !eval_volume_inertia( &sphere, div, vol, &i, &ev, &ei ) ) ret = false;
  zAssert( zSphere3DVolume + zSphere3DInertia, ret );
}

/* inside/outside test */

bool sphere_cat(zSphere3D *sphere, double x, double y, double z, zVec3D *p)
{
  zVec3DCopy( zSphere3DCenter(sphere), p );
  p->c.x += x;
  p->c.y += y;
  p->c.z += z;
  return x*x + y*y + z*z < zSqr(zSphere3DRadius(sphere));
}

void assert_inside(void)
{
  zSphere3D sphere;
  zVec3D p;
  double x, y, z;
  register int i;
  int n = 1000, ni, no, nitest, notest;

  generate_sphere_rand( &sphere );
  for( ni=no=nitest=notest=0, i=0; i<n; i++ ){
    x = zRandF(-2,2);
    y = zRandF(-2,2);
    z = zRandF(-2,2);
    sphere_cat( &sphere, x, y, z, &p ) ? ni++ : no++;
    zSphere3DPointIsInside( &sphere, &p, true ) ? nitest++ : notest++;
  }
  zAssert( zSphere3DPointIsInside, nitest == ni && notest == no );
}

int main(void)
{
  zRandInit();
  assert_volume_inertia();
  assert_inside();
  return EXIT_SUCCESS;
}
