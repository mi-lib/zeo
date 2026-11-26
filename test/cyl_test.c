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
  int i;
  int n = 1000, ni, no, nitest, notest;

  generate_cyl_rand( &cyl );
  zCyl3DAxis( &cyl, &d1 );
  zVec3DOrthoSpace( &d1, &d2, &d3 );
  for( ni=no=nitest=notest=0, i=0; i<n; i++ ){
    x = zRandF(-2,2);
    y = zRandF(-2,2);
    z = zRandF(-1,2);
    cyl_cat( &cyl, &d1, &d2, &d3, x, y, z, &p ) ? ni++ : no++;
    zCyl3DPointIsInside( &cyl, &p, zTOL ) ? nitest++ : notest++;
  }
  zAssert( zCyl3DPointIsInside, nitest == ni && notest == no );
}

/* the distance from a point to a cylinder is non-negative, zero if and only if the point is inside,
 * equal to the distance to the closest point, and not larger than those to sampled points on the
 * surface. Points beyond the both ends are included. */
void assert_closest(void)
{
  zCyl3D cyl;
  zVec3D c1, c2, p, cp, axis, u, w, q;
  double dist, dmin, r, h, th;
  int i, j, k;
  bool result = true;

  for( i=0; i<100; i++ ){
    zVec3DCreate( &c1, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DCreate( &c2, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    r = zRandF(0.1,1);
    zCyl3DCreate( &cyl, &c1, &c2, r, 0 );
    zVec3DSub( &c2, &c1, &axis );
    h = zVec3DNormalizeDRC( &axis, 0 );
    zVec3DOrthonormal( &axis, &u );
    zVec3DOuterProd( &axis, &u, &w );
    /* a point around the cylinder, beyond the ends in some cases */
    th = zRandF(-zPI,zPI);
    zVec3DCat( &c1, zRandF(-1,1)*h + zRandF(-0.5,1.5)*h, &axis, &p );
    zVec3DCatDRC( &p, zRandF(0,2)*r*cos(th), &u );
    zVec3DCatDRC( &p, zRandF(0,2)*r*sin(th), &w );
    dist = zCyl3DClosest( &cyl, &p, &cp );
    if( dist < 0 || !zEqual( dist, zCyl3DDistFromPoint( &cyl, &p ), zTOL ) ||
        !zEqual( dist, zVec3DDist( &p, &cp ), zTOL ) ) result = false;
    if( ( dist == 0 ) != zCyl3DPointIsInside( &cyl, &p, 0 ) && !zIsTiny( dist ) ) result = false;
    if( dist == 0 ) continue;
    for( dmin=HUGE_VAL, j=0; j<=100; j++ ) /* sampled points on the side and the caps */
      for( k=0; k<200; k++ ){
        zVec3DCat( &c1, h*j/100, &axis, &q );
        zVec3DCatDRC( &q, r*cos(zPIx2*k/200), &u );
        zVec3DCatDRC( &q, r*sin(zPIx2*k/200), &w );
        dmin = zMin( dmin, zVec3DDist( &p, &q ) );
        zVec3DCopy( j <= 50 ? &c1 : &c2, &q );
        zVec3DCatDRC( &q, r*(j%51)/50*cos(zPIx2*k/200), &u );
        zVec3DCatDRC( &q, r*(j%51)/50*sin(zPIx2*k/200), &w );
        dmin = zMin( dmin, zVec3DDist( &p, &q ) );
      }
    if( dist > dmin + zTOL ) result = false;
  }
  zAssert( zCyl3DClosest + zCyl3DDistFromPoint, result );
}

int main(void)
{
  zRandInit();
  assert_volume_inertia();
  assert_inside();
  assert_closest();
  return EXIT_SUCCESS;
}
