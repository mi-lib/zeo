#include <zeo/zeo.h>

void generate_cone_rand(zCone3D *cone)
{
  zVec3D c, v;

  zVec3DCreate( &c, zRandF(-3,3), zRandF(-3,3), zRandF(-3,0) );
  zVec3DCreate( &v, zRandF(-3,3), zRandF(-3,3), zRandF( 0,3) );
  zCone3DCreate( cone, &c, &v, zRandF(0,1), 0 );
}

void test_volume_inertia(zCone3D *cone, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  zPH3D ph;
  double vol;
  zVec3D bc;
  zMat3D inertia, ierr;

  zCone3DSetDiv( cone, div );
  zCone3DToPH( cone, &ph );
  vol = zPH3DVolume( &ph );
  zPH3DBaryInertia( &ph, &bc, &inertia );
  zPH3DDestroy( &ph );
  *ev = vol - v0;
  *ei = zMat3DNorm( zMat3DSub( &inertia, i0, &ierr ) );
}

bool eval_volume_inertia(zCone3D *cone, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  double evtest, eitest;

  test_volume_inertia( cone, div, v0, i0, &evtest, &eitest );
  if( evtest / *ev > 0.5 || eitest / *ei > 0.5 ) return false;
  *ev = evtest;
  *ei = eitest;
  return true;
}

void assert_volume_inertia(void)
{
  zCone3D cone;
  double vol, ev, ei;
  zMat3D i;
  int div=4, div_max = 2 << 16;
  bool ret = true;

  generate_cone_rand( &cone );
  vol = zCone3DVolume( &cone );
  zCone3DInertia( &cone, &i );
  test_volume_inertia( &cone, div, vol, &i, &ev, &ei );
  for( div*=2; div<=div_max; div*=2 )
    if( !eval_volume_inertia( &cone, div, vol, &i, &ev, &ei ) ) ret = false;
  zAssert( zCone3DVolume + zCone3DInertia, ret );
}

int main(void)
{
  zRandInit();
  assert_volume_inertia();
  return EXIT_SUCCESS;
}
