#include <zeo/zeo.h>

#define N 1000
#define NDIR 20000
#define DELTA 0.02

/* The signed distance of a point from a convex shape is the supremum of u.p - h(u) over unit
 * vectors u, where h is the support function of the shape. It is approximated from below by the
 * maximum over many directions, the error of which is much smaller than DELTA. The point is
 * expected to be inside if the margin exceeds the signed distance by DELTA, and not inside if the
 * margin is less than it by DELTA. */

static zVec3D dir3D[NDIR];

static void dir3D_init(void)
{
  double z, r, phi;
  int i;

  for( i=0; i<NDIR; i++ ){ /* Fibonacci sphere */
    z = 1 - ( 2.0 * i + 1 ) / NDIR;
    r = sqrt( 1 - z*z );
    phi = i * zPI * ( 3 - sqrt(5) );
    zVec3DCreate( &dir3D[i], r*cos(phi), r*sin(phi), z );
  }
}

/* the normal vectors of flat faces, at which the support function is not smooth, are added to the
 * directions, since the error of the approximation is of the first order around them. */
static double signed_dist3D(const void *shape, double (* support)(const void*,const zVec3D*), const zVec3D *p, const zVec3D normal[], int nn)
{
  zVec3D u;
  double s, smax = -HUGE_VAL;
  int i;

  for( i=0; i<NDIR; i++ )
    if( ( s = zVec3DInnerProd( &dir3D[i], p ) - support( shape, &dir3D[i] ) ) > smax ) smax = s;
  for( i=0; i<nn; i++ ){
    zVec3DNormalize( &normal[i], 0, &u );
    if( ( s = zVec3DInnerProd( &u, p ) - support( shape, &u ) ) > smax ) smax = s;
    zVec3DRevDRC( &u );
    if( ( s = zVec3DInnerProd( &u, p ) - support( shape, &u ) ) > smax ) smax = s;
  }
  return smax;
}

static double signed_dist2D(const void *shape, double (* support)(const void*,const zVec2D*), const zVec2D *p)
{
  zVec2D u;
  double s, smax = -HUGE_VAL;
  int i;

  for( i=0; i<NDIR; i++ ){
    zVec2DCreatePolar( &u, 1, zPIx2*i/NDIR );
    if( ( s = zVec2DInnerProd( &u, p ) - support( shape, &u ) ) > smax ) smax = s;
  }
  return smax;
}

/* check if the inside check with a margin around the signed distance is correct. */
#define CHECK_INSIDE(func,shape,p,d) \
  ( func( shape, p, (d) + DELTA ) && !func( shape, p, (d) - DELTA ) )

static double support_aabox3D(const void *shape, const zVec3D *u)
{
  const zAABox3D *box = (const zAABox3D *)shape;
  double h = 0;
  int k;

  for( k=0; k<3; k++ )
    h += u->e[k] * ( u->e[k] > 0 ? box->max.e[k] : box->min.e[k] );
  return h;
}

static double support_box3D(const void *shape, const zVec3D *u)
{
  const zBox3D *box = (const zBox3D *)shape;
  double h;
  int k;

  h = zVec3DInnerProd( u, zBox3DCenter(box) );
  for( k=zX; k<=zZ; k++ )
    h += 0.5 * zBox3DSpan(box,(zAxis)k) * fabs( zVec3DInnerProd( u, zBox3DAxis(box,(zAxis)k) ) );
  return h;
}

static double support_disk_ends(const zVec3D *u, const zVec3D *c1, const zVec3D *c2, double r)
{
  zVec3D a;
  double ua;

  zVec3DSub( c2, c1, &a );
  zVec3DNormalizeDRC( &a, 0 );
  ua = zVec3DInnerProd( u, &a );
  return zMax( zVec3DInnerProd( u, c1 ), zVec3DInnerProd( u, c2 ) ) + r * sqrt( zMax( 1 - ua*ua, 0 ) );
}

static double support_cyl3D(const void *shape, const zVec3D *u)
{
  const zCyl3D *cyl = (const zCyl3D *)shape;
  return support_disk_ends( u, zCyl3DCenter(cyl,0), zCyl3DCenter(cyl,1), zCyl3DRadius(cyl) );
}

static double support_cone3D(const void *shape, const zVec3D *u)
{
  const zCone3D *cone = (const zCone3D *)shape;
  zVec3D a;
  double ua;

  zVec3DSub( zCone3DVert(cone), zCone3DCenter(cone), &a );
  zVec3DNormalizeDRC( &a, 0 );
  ua = zVec3DInnerProd( u, &a );
  return zMax( zVec3DInnerProd( u, zCone3DVert(cone) ),
    zVec3DInnerProd( u, zCone3DCenter(cone) ) + zCone3DRadius(cone) * sqrt( zMax( 1 - ua*ua, 0 ) ) );
}

static double support_capsule3D(const void *shape, const zVec3D *u)
{
  const zCapsule3D *capsule = (const zCapsule3D *)shape;
  return zMax( zVec3DInnerProd( u, zCapsule3DCenter(capsule,0) ), zVec3DInnerProd( u, zCapsule3DCenter(capsule,1) ) ) + zCapsule3DRadius(capsule);
}

static double support_aabox2D(const void *shape, const zVec2D *u)
{
  const zAABox2D *box = (const zAABox2D *)shape;
  return u->c.x * ( u->c.x > 0 ? box->max.c.x : box->min.c.x )
       + u->c.y * ( u->c.y > 0 ? box->max.c.y : box->min.c.y );
}

static double support_tri2D(const void *shape, const zVec2D *u)
{
  const zTri2D *tri = (const zTri2D *)shape;
  double h, hmax = -HUGE_VAL;
  int i;

  for( i=0; i<3; i++ )
    if( ( h = zVec2DInnerProd( u, zTri2DVert(tri,i) ) ) > hmax ) hmax = h;
  return hmax;
}

static void vec3D_rand(zVec3D *v, double r)
{
  zVec3DCreate( v, zRandF(-r,r), zRandF(-r,r), zRandF(-r,r) );
}

static void box3D_rand(zBox3D *box)
{
  zVec3D center, ax, ay, az, tmp;

  vec3D_rand( &center, 1 );
  vec3D_rand( &ax, 1 );
  vec3D_rand( &tmp, 1 );
  zVec3DOrthogonalize( &tmp, &ax, &ay );
  zVec3DOuterProd( &ax, &ay, &az );
  zVec3DNormalizeDRC( &ax, 0 );
  zVec3DNormalizeDRC( &ay, 0 );
  zVec3DNormalizeDRC( &az, 0 );
  zBox3DCreate( box, &center, &ax, &ay, &az, zRandF(1,3), zRandF(1,3), zRandF(1,3) );
}

void assert_inside_3D(void)
{
  zAABox3D aabox;
  zBox3D box;
  zCyl3D cyl;
  zCone3D cone;
  zCapsule3D capsule;
  zVec3D c1, c2, p, a, xyz[3];
  int i;
  bool result_aabox = true, result_box = true, result_cyl = true, result_cone = true, result_capsule = true;

  for( i=0; i<N/10; i++ ){
    zAABox3DCreate( &aabox, zRandF(-2,-0.5), zRandF(-2,-0.5), zRandF(-2,-0.5), zRandF(0.5,2), zRandF(0.5,2), zRandF(0.5,2) );
    box3D_rand( &box );
    vec3D_rand( &c1, 1 );
    vec3D_rand( &c2, 1 );
    zVec3DCatDRC( &c2, 1, ZVEC3DZ ); /* to keep the axis */
    zCyl3DCreate( &cyl, &c1, &c2, zRandF(0.5,2), 0 );
    zCone3DCreate( &cone, &c1, &c2, zRandF(0.5,2), 0 );
    zCapsule3DCreate( &capsule, &c1, &c2, zRandF(0.5,2), 0 );
    vec3D_rand( &p, 4 );
    zVec3DSub( &c2, &c1, &a );
    zVec3DCopy( ZVEC3DX, &xyz[0] );
    zVec3DCopy( ZVEC3DY, &xyz[1] );
    zVec3DCopy( ZVEC3DZ, &xyz[2] );
    if( !CHECK_INSIDE( zAABox3DPointIsInside, &aabox, &p, signed_dist3D( &aabox, support_aabox3D, &p, xyz, 3 ) ) ) result_aabox = false;
    if( !CHECK_INSIDE( zBox3DPointIsInside, &box, &p, signed_dist3D( &box, support_box3D, &p, zBox3DAxis(&box,zX), 3 ) ) ) result_box = false;
    if( !CHECK_INSIDE( zCyl3DPointIsInside, &cyl, &p, signed_dist3D( &cyl, support_cyl3D, &p, &a, 1 ) ) ) result_cyl = false;
    if( !CHECK_INSIDE( zCone3DPointIsInside, &cone, &p, signed_dist3D( &cone, support_cone3D, &p, &a, 1 ) ) ) result_cone = false;
    if( !CHECK_INSIDE( zCapsule3DPointIsInside, &capsule, &p, signed_dist3D( &capsule, support_capsule3D, &p, NULL, 0 ) ) ) result_capsule = false;
  }
  zAssert( zAABox3DPointIsInside (signed distance), result_aabox );
  zAssert( zBox3DPointIsInside (signed distance), result_box );
  zAssert( zCyl3DPointIsInside (signed distance), result_cyl );
  zAssert( zCone3DPointIsInside (signed distance), result_cone );
  zAssert( zCapsule3DPointIsInside (signed distance), result_capsule );
}

void assert_inside_2D(void)
{
  zAABox2D box;
  zTri2D tri;
  zVec2D v[3], p;
  int i, j;
  bool result_aabox = true, result_tri = true;

  for( i=0; i<N/10; i++ ){
    zAABox2DCreate( &box, zRandF(-2,-0.5), zRandF(-2,-0.5), zRandF(0.5,2), zRandF(0.5,2) );
    for( j=0; j<3; j++ ) zVec2DCreatePolar( &v[j], zRandF(1,2), zPIx2*j/3 + zRandF(-0.5,0.5) );
    zTri2DCreate( &tri, &v[0], &v[1], &v[2] );
    zVec2DCreate( &p, zRandF(-4,4), zRandF(-4,4) );
    if( !CHECK_INSIDE( zAABox2DPointIsInside, &box, &p, signed_dist2D( &box, support_aabox2D, &p ) ) ) result_aabox = false;
    if( !CHECK_INSIDE( zTri2DPointIsInside, &tri, &p, signed_dist2D( &tri, support_tri2D, &p ) ) ) result_tri = false;
  }
  zAssert( zAABox2DPointIsInside (signed distance), result_aabox );
  zAssert( zTri2DPointIsInside (signed distance), result_tri );
}

/* points on the boundary are not inside with a zero margin. */
void assert_inside_boundary(void)
{
  zAABox3D aabox;
  zAABox2D aabox2;
  zVec3D p;
  zVec2D p2;
  bool result = true;

  zAABox3DCreate( &aabox, -1, -2, -3, 1, 2, 3 );
  zVec3DCreate( &p, 1, 0, 0 );
  if( zAABox3DPointIsInside( &aabox, &p, 0 ) || !zAABox3DPointIsInside( &aabox, &p, zTOL ) ) result = false;
  zVec3DCreate( &p, -1, 0, 0 );
  if( zAABox3DPointIsInside( &aabox, &p, 0 ) || !zAABox3DPointIsInside( &aabox, &p, zTOL ) ) result = false;
  zAABox2DCreate( &aabox2, -1, -2, 1, 2 );
  zVec2DCreate( &p2, 0, 2 );
  if( zAABox2DPointIsInside( &aabox2, &p2, 0 ) || !zAABox2DPointIsInside( &aabox2, &p2, zTOL ) ) result = false;
  zAssert( z...PointIsInside (boundary), result );
}

/* the approximate inside check of ellipses never accepts a non-positive radius. */
void assert_inside_ellips(void)
{
  zEllips2D e2;
  zEllips3D e3;
  zECyl3D ec;
  zVec3D c1, c2;
  bool result = true;

  zEllips2DCreate( &e2, ZVEC2DZERO, 2, 1 );
  if( !zEllips2DPointIsInside( &e2, ZVEC2DZERO, -0.9 ) || zEllips2DPointIsInside( &e2, ZVEC2DZERO, -1 ) ||
      zEllips2DPointIsInside( &e2, ZVEC2DZERO, -3 ) ) result = false;
  zEllips3DCreateAlign( &e3, ZVEC3DZERO, 3, 2, 1, 0 );
  if( !zEllips3DPointIsInside( &e3, ZVEC3DZERO, -0.9 ) || zEllips3DPointIsInside( &e3, ZVEC3DZERO, -1 ) ||
      zEllips3DPointIsInside( &e3, ZVEC3DZERO, -4 ) ) result = false;
  zVec3DCreate( &c1, 0, 0, -5 );
  zVec3DCreate( &c2, 0, 0, 5 );
  zECyl3DCreate( &ec, &c1, &c2, 2, 1, ZVEC3DX, 0 );
  if( !zECyl3DPointIsInside( &ec, ZVEC3DZERO, -0.9 ) || zECyl3DPointIsInside( &ec, ZVEC3DZERO, -1 ) ||
      zECyl3DPointIsInside( &ec, ZVEC3DZERO, -3 ) ) result = false;
  zAssert( zEllips2DPointIsInside + zEllips3DPointIsInside + zECyl3DPointIsInside (non-positive radius), result );
}

/* the distance from a point beyond an end of a capsule is positive. */
void assert_capsule_dist(void)
{
  zCapsule3D capsule;
  zVec3D c1, c2, p;
  bool result = true;

  zVec3DCreate( &c1, 0, 0, 0 );
  zVec3DCreate( &c2, 0, 0, 2 );
  zCapsule3DCreate( &capsule, &c1, &c2, 1, 0 );
  zVec3DCreate( &p, 0, 0, 5 );
  if( !zEqual( zCapsule3DDistFromPoint( &capsule, &p ), 2, zTOL ) ) result = false;
  zVec3DCreate( &p, 0, 0, -3 );
  if( !zEqual( zCapsule3DDistFromPoint( &capsule, &p ), 2, zTOL ) ) result = false;
  zAssert( zCapsule3DDistFromPoint (beyond the ends), result );
}

int main(void)
{
  zRandInit();
  dir3D_init();
  assert_inside_3D();
  assert_inside_2D();
  assert_inside_boundary();
  assert_inside_ellips();
  assert_capsule_dist();
  return 0;
}
