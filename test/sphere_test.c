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
  zMat3D inertia, ierr;

  zSphere3DSetDiv( sphere, div );
  zSphere3DToPH( sphere, &ph );
  vol = zPH3DVolume( &ph );
  zPH3DBaryInertia( &ph, 1, &inertia );
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
  zSphere3DBaryInertia( &sphere, 1, &i );
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
  int i, n = 1000, ni, no, nitest, notest;

  generate_sphere_rand( &sphere );
  for( ni=no=nitest=notest=0, i=0; i<n; i++ ){
    x = zRandF(-2,2);
    y = zRandF(-2,2);
    z = zRandF(-2,2);
    sphere_cat( &sphere, x, y, z, &p ) ? ni++ : no++;
    zSphere3DPointIsInside( &sphere, &p, zTOL ) ? nitest++ : notest++;
  }
  zAssert( zSphere3DPointIsInside, nitest == ni && notest == no );
}

/* the signed distance is negative inside, while the distance and the closest point are zero and
 * the point itself, respectively. The inside check accepts a negative margin. */
void assert_dist(void)
{
  zSphere3D sphere;
  zVec3D c, p, cp, v;
  double r, l, sd, d;
  int i;
  bool result = true;

  for( i=0; i<1000; i++ ){
    zVec3DCreate( &c, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
    r = zRandF(0.1,5);
    zSphere3DCreate( &sphere, &c, r, 0 );
    zVec3DCreate( &v, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DNormalizeDRC( &v, 0 );
    l = r * zRandF(0,2);
    zVec3DCat( &c, l, &v, &p );
    sd = zSphere3DSignedDistFromPoint( &sphere, &p );
    d = zSphere3DClosest( &sphere, &p, &cp );
    if( !zEqual( sd, l - r, zTOL ) ) result = false;
    if( !zEqual( d, zSphere3DDistFromPoint( &sphere, &p ), zTOL ) || !zEqual( d, zMax( l - r, 0 ), zTOL ) ) result = false;
    if( l <= r ){
      if( !zVec3DEqual( &cp, &p ) ) result = false;
    } else{
      if( !zEqual( zVec3DDist( &cp, &c ), r, zTOL ) || !zEqual( zVec3DDist( &cp, &p ), d, zTOL ) ) result = false;
    }
    if( !zSphere3DPointIsInside( &sphere, &p, sd + zTOL ) || zSphere3DPointIsInside( &sphere, &p, sd - zTOL ) ) result = false;
  }
  zAssert( zSphere3DClosest + zSphere3DSignedDistFromPoint + zSphere3DDistFromPoint, result );
}

int main(void)
{
  zRandInit();
  assert_volume_inertia();
  assert_inside();
  assert_dist();
  return EXIT_SUCCESS;
}
