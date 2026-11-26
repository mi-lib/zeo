#include <zeo/zeo.h>

void generate_ellips_rand(zEllips3D *ellips)
{
  zVec3D center, ax, ay, az, tmp;

  zVec3DCreate( &center, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DCreate( &ax, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &tmp, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DOrthogonalize( &tmp, &ax, &ay );
  zVec3DOuterProd( &ax, &ay, &az );
  zVec3DNormalizeDRC( &ax, 0 );
  zVec3DNormalizeDRC( &ay, 0 );
  zVec3DNormalizeDRC( &az, 0 );

  zEllips3DCreate( ellips, &center, &ax, &ay, &az, zRandF(0,5), zRandF(0,5), zRandF(0,5), 0 );
}

/* inertia test */

void test_volume_inertia(zEllips3D *ellips, int div, double v0, zMat3D *i0, double *ev, double *ei)
{
  zPH3D ph;
  double vol;
  zMat3D inertia, ierr;

  zEllips3DSetDiv( ellips, div );
  zEllips3DToPH( ellips, &ph );
  vol = zPH3DVolume( &ph );
  zPH3DBaryInertia( &ph, 1, &inertia );
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
  zEllips3DBaryInertia( &ellips, 1, &i );
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
  int i, n = 1000, ni, no, nitest, notest;

  generate_ellips_rand( &ellips );
  for( ni=no=nitest=notest=0, i=0; i<n; i++ ){
    x = zRandF(-2,2);
    y = zRandF(-2,2);
    z = zRandF(-1,2);
    ellips_cat( &ellips, x, y, z, &p ) ? ni++ : no++;
    zEllips3DPointIsInside( &ellips, &p, zTOL ) ? nitest++ : notest++;
  }
  zAssert( zEllips3DPointIsInside, nitest == ni && notest == no );
}

/* the closest point on an ellipsoid is on the boundary, the vector from it to the given point is
 * along the outward normal, and the distance is not larger than that to sampled boundary points. */
void assert_closest(void)
{
  zEllips3D ellips;
  zVec3D center, ax, ay, az, p, pl, cp, cl, d, n, q;
  double r[3], rmin, rmax, dist, dmin, th, ph, s;
  int i, j, k, l;
  bool result = true;

  for( i=0; i<100; i++ ){
    zVec3DCreate( &center, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
    zVec3DCreate( &ax, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DCreate( &q, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DOrthogonalize( &q, &ax, &ay );
    zVec3DOuterProd( &ax, &ay, &az );
    zVec3DNormalizeDRC( &ax, 0 );
    zVec3DNormalizeDRC( &ay, 0 );
    zVec3DNormalizeDRC( &az, 0 );
    for( k=0; k<3; k++ ) r[k] = pow( 10, zRandF(-2,1) );
    rmin = zMin3( r[0], r[1], r[2] );
    rmax = zMax3( r[0], r[1], r[2] );
    zEllips3DCreate( &ellips, &center, &ax, &ay, &az, r[0], r[1], r[2], 0 );
    /* a point outside in the local frame */
    th = zRandF(0,zPI);
    ph = zRandF(-zPI,zPI);
    zVec3DCreate( &pl, r[0]*sin(th)*cos(ph), r[1]*sin(th)*sin(ph), r[2]*cos(th) );
    zVec3DMulDRC( &pl, zRandF(1,10) );
    if( i % 10 == 0 ){ /* on a principal plane */
      pl.e[i/10%3] = 0;
      if( ( s = sqrt( zSqr(pl.c.x/r[0]) + zSqr(pl.c.y/r[1]) + zSqr(pl.c.z/r[2]) ) ) <= 1 )
        zVec3DMulDRC( &pl, zRandF(1,10) / s ); /* to be outside */
    }
    if( i % 20 == 0 ){ /* on a principal axis */
      zVec3DZero( &pl );
      pl.e[i/20%3] = r[i/20%3] * zRandF(1,10);
    }
    zXform3D( &ellips.f, &pl, &p );
    dist = zEllips3DClosest( &ellips, &p, &cp );
    zXform3DInv( &ellips.f, &cp, &cl );
    if( !zIsTol( zSqr(cl.c.x/r[0]) + zSqr(cl.c.y/r[1]) + zSqr(cl.c.z/r[2]) - 1, 1.0e-12 ) ) result = false;
    zVec3DSub( &pl, &cl, &d );
    zVec3DCreate( &n, cl.c.x/zSqr(r[0]), cl.c.y/zSqr(r[1]), cl.c.z/zSqr(r[2]) );
    zVec3DNormalizeDRC( &n, 0 );
    zVec3DOuterProd( &d, &n, &q );
    /* the rounding error of the transformation is amplified by 1/rmin^2 in the normal vector. */
    if( !zIsTol( zVec3DNorm( &q ), 1.0e-14 * zSqr( rmax / rmin ) * ( zVec3DNorm( &d ) + rmax + zVec3DNorm( &center ) ) ) || zVec3DInnerProd( &d, &n ) < 0 ) result = false;
    for( dmin=HUGE_VAL, j=0; j<=300; j++ )
      for( l=0; l<600; l++ ){
        zVec3DCreate( &q, r[0]*sin(zPI*j/300)*cos(zPIx2*l/600), r[1]*sin(zPI*j/300)*sin(zPIx2*l/600), r[2]*cos(zPI*j/300) );
        dmin = zMin( dmin, zVec3DDist( &pl, &q ) );
      }
    if( dist > dmin + 1.0e-12 ) result = false; /* not the global minimum */
  }
  zAssert( zEllips3DClosest, result );
}

int main(void)
{
  zRandInit();
  assert_volume_inertia();
  assert_inside();
  assert_closest();
  return EXIT_SUCCESS;
}
