#include <zeo/zeo.h>
#include <zeo/zeo_nurbs3d_shape.h>

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

/* the minimum distance from a point p to the points on the surface of an axis-aligned ellipsoid with
 * radii r parameterized by the polar angles about the z-axis, which is found on a grid and refined
 * by a pattern search followed by the Newton method. */
static double ellips3D_dist_param(const double r[], const zVec3D *p)
{
  zVec3D x;
  double th, ph, thmin = 0, phmin = 0, d, dmin = HUGE_VAL, step, dth[] = { 1, -1, 0, 0 }, dph[] = { 0, 0, 1, -1 };
  int i, j, iter;
  bool moved;

#define ELLIPS3D_POINT(t,f) zVec3DCreate( &x, r[0]*sin(t)*cos(f), r[1]*sin(t)*sin(f), r[2]*cos(t) )
#define ELLIPS3D_POINT2(t,f) zVec3DCreate( &x, r[0]*sin(t)*cos(f), r[1]*sin(t)*sin(f), r[2]*cos(t) )
  for( i=0; i<=200; i++ )
    for( j=0; j<400; j++ ){
      ELLIPS3D_POINT( zPI*i/200, zPIx2*j/400 );
      if( ( d = zVec3DSqrDist( &x, p ) ) < dmin ){
        dmin = d; thmin = zPI*i/200; phmin = zPIx2*j/400;
      }
    }
  for( step=zPI/200, iter=0; step>1.0e-15 && iter<100000; iter++ ){
    for( moved=false, i=0; i<4; i++ ){
      th = thmin + step*dth[i];
      ph = phmin + step*dph[i];
      ELLIPS3D_POINT( th, ph );
      if( ( d = zVec3DSqrDist( &x, p ) ) < dmin ){
        dmin = d; thmin = th; phmin = ph; moved = true;
      }
    }
    if( !moved ) step *= 0.5;
  }
#undef ELLIPS3D_POINT
  /* the Newton method on the squared distance with the analytic derivatives */
  for( iter=0; iter<20; iter++ ){
    double st = sin(thmin), ct = cos(thmin), sf = sin(phmin), cf = cos(phmin), g[2], h[3], det, dt, df, a;
    zVec3D e, xt, xf, xtt, xtf, xff;
    zVec3DCreate( &x, r[0]*st*cf, r[1]*st*sf, r[2]*ct );
    zVec3DCreate( &xt, r[0]*ct*cf, r[1]*ct*sf, -r[2]*st );
    zVec3DCreate( &xf, -r[0]*st*sf, r[1]*st*cf, 0 );
    zVec3DCreate( &xtt, -r[0]*st*cf, -r[1]*st*sf, -r[2]*ct );
    zVec3DCreate( &xtf, -r[0]*ct*sf, r[1]*ct*cf, 0 );
    zVec3DCreate( &xff, -r[0]*st*cf, -r[1]*st*sf, 0 );
    zVec3DSub( &x, p, &e );
    g[0] = zVec3DInnerProd( &e, &xt );
    g[1] = zVec3DInnerProd( &e, &xf );
    h[0] = zVec3DInnerProd( &xt, &xt ) + zVec3DInnerProd( &e, &xtt );
    h[1] = zVec3DInnerProd( &xt, &xf ) + zVec3DInnerProd( &e, &xtf );
    h[2] = zVec3DInnerProd( &xf, &xf ) + zVec3DInnerProd( &e, &xff );
    if( h[0] <= 0 || ( det = h[0]*h[2] - h[1]*h[1] ) <= 0 ) break; /* not convex */
    dt = -( h[2]*g[0] - h[1]*g[1] ) / det;
    df = -( h[0]*g[1] - h[1]*g[0] ) / det;
    for( a=1; a>1.0e-10; a*=0.5 ){
      ELLIPS3D_POINT2( thmin+a*dt, phmin+a*df );
      if( ( d = zVec3DSqrDist( &x, p ) ) <= dmin ){
        dmin = d; thmin += a*dt; phmin += a*df; break;
      }
    }
    if( a <= 1.0e-10 ) break;
  }
#undef ELLIPS3D_POINT2
  return sqrt( dmin );
}

/* the signed distance from a point p to an axis-aligned ellipsoid with radii r, which is computed
 * independently of the library. Since the parameterization is singular at the poles, the minimum of
 * those with the poles on the three axes is taken. */
static double ellips3D_signed_dist_param(const double r[], const zVec3D *p)
{
  double rp[3], d, dmin = HUGE_VAL;
  zVec3D pp;
  int k, j;

  for( k=0; k<3; k++ ){
    for( j=0; j<3; j++ ){
      rp[j] = r[(j+k+1)%3];
      pp.e[j] = p->e[(j+k+1)%3];
    }
    if( ( d = ellips3D_dist_param( rp, &pp ) ) < dmin ) dmin = d;
  }
  return zSqr(p->c.x/r[0]) + zSqr(p->c.y/r[1]) + zSqr(p->c.z/r[2]) < 1 ? -dmin : dmin;
}

/* the 2D counterpart of the above. */
static double ellips2D_signed_dist_param(const double r[], const zVec2D *p)
{
  zVec2D x;
  double th, thmin = 0, d, dmin = HUGE_VAL, step;
  int i, iter, sgn;
  bool moved;

  for( i=0; i<NDIR; i++ ){
    zVec2DCreate( &x, r[0]*cos(zPIx2*i/NDIR), r[1]*sin(zPIx2*i/NDIR) );
    if( ( d = zVec2DSqrDist( &x, p ) ) < dmin ){
      dmin = d; thmin = zPIx2*i/NDIR;
    }
  }
  for( step=zPIx2/NDIR, iter=0; step>1.0e-15 && iter<100000; iter++ ){
    for( moved=false, sgn=-1; sgn<=1; sgn+=2 ){
      th = thmin + sgn*step;
      zVec2DCreate( &x, r[0]*cos(th), r[1]*sin(th) );
      if( ( d = zVec2DSqrDist( &x, p ) ) < dmin ){
        dmin = d; thmin = th; moved = true;
      }
    }
    if( !moved ) step *= 0.5;
  }
  d = sqrt( dmin );
  return zSqr(p->c.x/r[0]) + zSqr(p->c.y/r[1]) < 1 ? -d : d;
}

/* check if the signed distance is not less than the reference d computed by the approximation from
 * below, and not larger than it by DELTA. */
#define CHECK_SIGNED_DIST(func,shape,p,d) ( func( shape, p ) >= (d) - 1.0e-12 && func( shape, p ) <= (d) + DELTA )

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

static double support_ellips3D(const void *shape, const zVec3D *u)
{
  const zEllips3D *ellips = (const zEllips3D *)shape;
  return zVec3DInnerProd( u, zEllips3DCenter(ellips) )
    + sqrt( zSqr( zEllips3DRadiusX(ellips) * zVec3DInnerProd( u, zEllips3DAxis(ellips,zX) ) )
          + zSqr( zEllips3DRadiusY(ellips) * zVec3DInnerProd( u, zEllips3DAxis(ellips,zY) ) )
          + zSqr( zEllips3DRadiusZ(ellips) * zVec3DInnerProd( u, zEllips3DAxis(ellips,zZ) ) ) );
}

static double support_ecyl3D(const void *shape, const zVec3D *u)
{
  const zECyl3D *ecyl = (const zECyl3D *)shape;
  return zMax( zVec3DInnerProd( u, zECyl3DCenter(ecyl,0) ), zVec3DInnerProd( u, zECyl3DCenter(ecyl,1) ) )
    + sqrt( zSqr( zECyl3DRadius(ecyl,0) * zVec3DInnerProd( u, zECyl3DRadVec(ecyl,0) ) )
          + zSqr( zECyl3DRadius(ecyl,1) * zVec3DInnerProd( u, zECyl3DRadVec(ecyl,1) ) ) );
}

static double support_ellips2D(const void *shape, const zVec2D *u)
{
  const zEllips2D *ellips = (const zEllips2D *)shape;
  return zVec2DInnerProd( u, zEllips2DCenter(ellips) )
    + sqrt( zSqr( zEllips2DRadius(ellips,0) * u->c.x ) + zSqr( zEllips2DRadius(ellips,1) * u->c.y ) );
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
  zEllips3D ellips;
  zECyl3D ecyl;
  zVec3D c1, c2, p, a, xyz[3];
  int i;
  bool result_aabox = true, result_box = true, result_cyl = true, result_cone = true, result_capsule = true;
  bool result_ellips = true, result_ecyl = true;
  bool result_sd = true;
  double d;

  for( i=0; i<N/10; i++ ){
    zAABox3DCreate( &aabox, zRandF(-2,-0.5), zRandF(-2,-0.5), zRandF(-2,-0.5), zRandF(0.5,2), zRandF(0.5,2), zRandF(0.5,2) );
    box3D_rand( &box );
    vec3D_rand( &c1, 1 );
    vec3D_rand( &c2, 1 );
    zVec3DCatDRC( &c2, 1, ZVEC3DZ ); /* to keep the axis */
    zCyl3DCreate( &cyl, &c1, &c2, zRandF(0.5,2), 0 );
    zCone3DCreate( &cone, &c1, &c2, zRandF(0.5,2), 0 );
    zCapsule3DCreate( &capsule, &c1, &c2, zRandF(0.5,2), 0 );
    zEllips3DCreate( &ellips, &c1, zBox3DAxis(&box,zX), zBox3DAxis(&box,zY), zBox3DAxis(&box,zZ), zRandF(0.5,2), zRandF(0.5,2), zRandF(0.5,2), 0 );
    zECyl3DCreate( &ecyl, &c1, &c2, zRandF(0.5,2), zRandF(0.5,2), zBox3DAxis(&box,zX), 0 );
    vec3D_rand( &p, 4 );
    zVec3DSub( &c2, &c1, &a );
    zVec3DCopy( ZVEC3DX, &xyz[0] );
    zVec3DCopy( ZVEC3DY, &xyz[1] );
    zVec3DCopy( ZVEC3DZ, &xyz[2] );
    d = signed_dist3D( &aabox, support_aabox3D, &p, xyz, 3 );
    if( !CHECK_INSIDE( zAABox3DPointIsInside, &aabox, &p, d ) ) result_aabox = false;
    if( !CHECK_SIGNED_DIST( zAABox3DSignedDistFromPoint, &aabox, &p, d ) ) result_sd = false;
    d = signed_dist3D( &box, support_box3D, &p, zBox3DAxis(&box,zX), 3 );
    if( !CHECK_INSIDE( zBox3DPointIsInside, &box, &p, d ) ) result_box = false;
    if( !CHECK_SIGNED_DIST( zBox3DSignedDistFromPoint, &box, &p, d ) ) result_sd = false;
    d = signed_dist3D( &cyl, support_cyl3D, &p, &a, 1 );
    if( !CHECK_INSIDE( zCyl3DPointIsInside, &cyl, &p, d ) ) result_cyl = false;
    if( !CHECK_SIGNED_DIST( zCyl3DSignedDistFromPoint, &cyl, &p, d ) ) result_sd = false;
    d = signed_dist3D( &cone, support_cone3D, &p, &a, 1 );
    if( !CHECK_INSIDE( zCone3DPointIsInside, &cone, &p, d ) ) result_cone = false;
    if( !CHECK_SIGNED_DIST( zCone3DSignedDistFromPoint, &cone, &p, d ) ) result_sd = false;
    d = signed_dist3D( &capsule, support_capsule3D, &p, NULL, 0 );
    if( !CHECK_INSIDE( zCapsule3DPointIsInside, &capsule, &p, d ) ) result_capsule = false;
    if( !CHECK_SIGNED_DIST( zCapsule3DSignedDistFromPoint, &capsule, &p, d ) ) result_sd = false;
    d = signed_dist3D( &ellips, support_ellips3D, &p, NULL, 0 );
    if( !CHECK_INSIDE( zEllips3DPointIsInside, &ellips, &p, d ) ) result_ellips = false;
    if( !CHECK_SIGNED_DIST( zEllips3DSignedDistFromPoint, &ellips, &p, d ) ) result_sd = false;
    d = signed_dist3D( &ecyl, support_ecyl3D, &p, &a, 1 );
    if( !CHECK_INSIDE( zECyl3DPointIsInside, &ecyl, &p, d ) ) result_ecyl = false;
    if( !CHECK_SIGNED_DIST( zECyl3DSignedDistFromPoint, &ecyl, &p, d ) ) result_sd = false;
  }
  zAssert( zAABox3DPointIsInside (signed distance), result_aabox );
  zAssert( zBox3DPointIsInside (signed distance), result_box );
  zAssert( zCyl3DPointIsInside (signed distance), result_cyl );
  zAssert( zCone3DPointIsInside (signed distance), result_cone );
  zAssert( zCapsule3DPointIsInside (signed distance), result_capsule );
  zAssert( zEllips3DPointIsInside (signed distance), result_ellips );
  zAssert( zECyl3DPointIsInside (signed distance), result_ecyl );
  zAssert( z...SignedDistFromPoint (3D shapes), result_sd );
}

void assert_inside_2D(void)
{
  zAABox2D box;
  zTri2D tri;
  zEllips2D ellips;
  zVec2D v[3], p;
  int i, j;
  bool result_aabox = true, result_tri = true, result_ellips = true, result_sd = true;
  double d;

  for( i=0; i<N/10; i++ ){
    zAABox2DCreate( &box, zRandF(-2,-0.5), zRandF(-2,-0.5), zRandF(0.5,2), zRandF(0.5,2) );
    for( j=0; j<3; j++ ) zVec2DCreatePolar( &v[j], zRandF(1,2), zPIx2*j/3 + zRandF(-0.5,0.5) );
    zTri2DCreate( &tri, &v[0], &v[1], &v[2] );
    zEllips2DCreate( &ellips, &v[0], zRandF(0.5,2), zRandF(0.5,2) );
    zVec2DCreate( &p, zRandF(-4,4), zRandF(-4,4) );
    d = signed_dist2D( &box, support_aabox2D, &p );
    if( !CHECK_INSIDE( zAABox2DPointIsInside, &box, &p, d ) ) result_aabox = false;
    if( !CHECK_SIGNED_DIST( zAABox2DSignedDistFromPoint, &box, &p, d ) ) result_sd = false;
    d = signed_dist2D( &tri, support_tri2D, &p );
    if( !CHECK_INSIDE( zTri2DPointIsInside, &tri, &p, d ) ) result_tri = false;
    if( !CHECK_SIGNED_DIST( zTri2DSignedDistFromPoint, &tri, &p, d ) ) result_sd = false;
    d = signed_dist2D( &ellips, support_ellips2D, &p );
    if( !CHECK_INSIDE( zEllips2DPointIsInside, &ellips, &p, d ) ) result_ellips = false;
    if( !CHECK_SIGNED_DIST( zEllips2DSignedDistFromPoint, &ellips, &p, d ) ) result_sd = false;
  }
  zAssert( zAABox2DPointIsInside (signed distance), result_aabox );
  zAssert( zTri2DPointIsInside (signed distance), result_tri );
  zAssert( zEllips2DPointIsInside (signed distance), result_ellips );
  zAssert( z...SignedDistFromPoint (2D shapes), result_sd );
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

/* the depths of the centers of ellipses, an ellipsoid and an elliptic cylinder are the minimum radii. */
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
  zAssert( zEllips2DPointIsInside + zEllips3DPointIsInside + zECyl3DPointIsInside (center), result );
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

/* the signed distances from points around thin ellipses, ellipsoids and elliptic cylinders, the
 * radii of which differ up to 1000 times, are compared with the references computed from the
 * parameterized boundaries, and the inside checks with margins around them are tested. The signed
 * distance from an elliptic cylinder is computed from those from the ellipse of the section and the
 * both ends. */
void assert_signed_dist_thin(void)
{
  zEllips2D e2;
  zEllips3D e3;
  zECyl3D ec;
  zBox3D box;
  zVec2D p2, pl2;
  zVec3D p, pl, c2;
  double r[3], d, dref, e1, e2s, tol;
  int i, k;
  bool result2 = true, result3 = true, result_ecyl = true;

  for( i=0; i<N/10; i++ ){
    box3D_rand( &box );
    for( k=0; k<3; k++ ) r[k] = pow( 10, zRandF(-2,1) );
    tol = 1.0e-9 * zMax3( r[0], r[1], r[2] );
    /* ellipse */
    zEllips2DCreate( &e2, (zVec2D*)zBox3DCenter(&box)->e, r[0], r[1] );
    zVec2DCreate( &pl2, r[0]*zRandF(-1.5,1.5), r[1]*zRandF(-1.5,1.5) );
    if( i % 5 == 0 ) pl2.c.y = 0; /* on an axis */
    zVec2DAdd( zEllips2DCenter(&e2), &pl2, &p2 );
    d = zEllips2DSignedDistFromPoint( &e2, &p2 );
    dref = ellips2D_signed_dist_param( r, &pl2 );
    if( !zIsTol( d - dref, tol ) || ( d > 0 ) != ( zEllips2DDistFromPoint( &e2, &p2 ) > 0 ) ) result2 = false;
    if( !zEllips2DPointIsInside( &e2, &p2, d + tol ) || zEllips2DPointIsInside( &e2, &p2, d - tol ) ) result2 = false;
    /* ellipsoid */
    zEllips3DCreate( &e3, zBox3DCenter(&box), zBox3DAxis(&box,zX), zBox3DAxis(&box,zY), zBox3DAxis(&box,zZ), r[0], r[1], r[2], 0 );
    for( k=0; k<3; k++ ) pl.e[k] = r[k] * zRandF(-1.5,1.5);
    if( i % 5 == 0 ) pl.e[i/5%3] = 0; /* on a principal plane */
    zXform3D( &e3.f, &pl, &p );
    d = zEllips3DSignedDistFromPoint( &e3, &p );
    dref = ellips3D_signed_dist_param( r, &pl );
    if( !zIsTol( d - dref, tol ) || ( d > 0 ) != ( zEllips3DDistFromPoint( &e3, &p ) > 0 ) ) result3 = false;
    if( !zEllips3DPointIsInside( &e3, &p, d + tol ) || zEllips3DPointIsInside( &e3, &p, d - tol ) ) result3 = false;
    /* elliptic cylinder along the z-axis of the frame, the height of which is r[2] */
    zVec3DCat( zBox3DCenter(&box), r[2], zBox3DAxis(&box,zZ), &c2 );
    zECyl3DCreate( &ec, zBox3DCenter(&box), &c2, r[0], r[1], zBox3DAxis(&box,zX), 0 );
    pl.e[zZ] += 0.5 * r[2]; /* around the middle of the axis */
    zXform3D( &e3.f, &pl, &p );
    d = zECyl3DSignedDistFromPoint( &ec, &p );
    e1 = ellips2D_signed_dist_param( r, (zVec2D*)pl.e );
    e2s = zMax( -pl.c.z, pl.c.z - r[2] );
    dref = e1 <= 0 && e2s <= 0 ? zMax( e1, e2s ) : sqrt( zSqr( zMax( e1, 0 ) ) + zSqr( zMax( e2s, 0 ) ) );
    if( !zIsTol( d - dref, tol ) || ( d > 0 ) != ( zECyl3DDistFromPoint( &ec, &p ) > 0 ) ) result_ecyl = false;
    if( !zECyl3DPointIsInside( &ec, &p, d + tol ) || zECyl3DPointIsInside( &ec, &p, d - tol ) ) result_ecyl = false;
  }
  zAssert( zEllips2DSignedDistFromPoint + zEllips2DPointIsInside (thin ellipses), result2 );
  zAssert( zEllips3DSignedDistFromPoint + zEllips3DPointIsInside (thin ellipsoids), result3 );
  zAssert( zECyl3DSignedDistFromPoint + zECyl3DPointIsInside (thin elliptic cylinders), result_ecyl );
}

/* zShape3DSignedDistFromPoint() calls the function of each type of shapes. */
void assert_shape3D_signed_dist(void)
{
  zShape3D shape[8];
  zVec3D c1, c2, p;
  double d[8];
  int i, k;
  bool result = true;

  zVec3DCreate( &c1, 0, 0, -1 );
  zVec3DCreate( &c2, 0, 0, 1 );
  zShape3DBoxCreateAlign( &shape[0], ZVEC3DZERO, 2, 3, 4 );
  zShape3DSphereCreate( &shape[1], ZVEC3DZERO, 1.5, 0 );
  zShape3DCylCreate( &shape[2], &c1, &c2, 1, 0 );
  zShape3DCapsuleCreate( &shape[3], &c1, &c2, 1, 0 );
  zShape3DConeCreate( &shape[4], &c1, &c2, 1, 0 );
  zShape3DEllipsCreateAlign( &shape[5], ZVEC3DZERO, 1, 2, 3, 0 );
  zShape3DECylCreate( &shape[6], &c1, &c2, 1, 2, ZVEC3DX, 0 );
  zShape3DBoxCreateAlign( &shape[7], ZVEC3DZERO, 2, 3, 4 );
  zShape3DToPH( &shape[7] );
  for( i=0; i<N; i++ ){
    vec3D_rand( &p, 3 );
    d[0] = zBox3DSignedDistFromPoint( zShape3DBox(&shape[0]), &p );
    d[1] = zSphere3DSignedDistFromPoint( zShape3DSphere(&shape[1]), &p );
    d[2] = zCyl3DSignedDistFromPoint( zShape3DCyl(&shape[2]), &p );
    d[3] = zCapsule3DSignedDistFromPoint( zShape3DCapsule(&shape[3]), &p );
    d[4] = zCone3DSignedDistFromPoint( zShape3DCone(&shape[4]), &p );
    d[5] = zEllips3DSignedDistFromPoint( zShape3DEllips(&shape[5]), &p );
    d[6] = zECyl3DSignedDistFromPoint( zShape3DECyl(&shape[6]), &p );
    d[7] = zPH3DSignedDistFromPoint( zShape3DPH(&shape[7]), &p );
    for( k=0; k<8; k++ )
      if( zShape3DSignedDistFromPoint( &shape[k], &p ) != d[k] ) result = false;
    /* the polyhedron of the box */
    if( !zIsTiny( d[7] - d[0] ) ) result = false;
  }
  for( k=0; k<8; k++ ) zShape3DDestroy( &shape[k] );
  zAssert( zShape3DSignedDistFromPoint, result );
}

/* the signed distances from points to closed NURBS surfaces of a sphere, an ellipsoid, a cone, a box
 * and a cylinder coincide with those to the original shapes, and the inside checks follow them. The
 * closest points on the surfaces are found also for points around the seams, the poles and the
 * creases. */
void assert_nurbs3D_signed_dist(void)
{
  zSphere3D sphere;
  zEllips3D ellips;
  zCone3D cone;
  zBox3D box;
  zCyl3D cyl;
  zNURBS3D nurbs[5];
  zShape3D shape;
  zVec3D c, c2, p, cp;
  double d, dref, u, v;
  int i, k;
  bool result_dist = true, result_inside = true, result_shape = true;

  zVec3DCreate( &c, 0.1, 0.2, 0.3 );
  zSphere3DCreate( &sphere, &c, 1, 0 );
  zNURBS3DSphere( &nurbs[0], &sphere );
  zEllips3DCreateAlign( &ellips, &c, 1, 2, 3, 0 );
  zNURBS3DEllips( &nurbs[1], &ellips );
  zVec3DCreate( &c2, 0.3, 0.1, 2.3 );
  zCone3DCreate( &cone, &c, &c2, 1, 0 );
  zNURBS3DCone( &nurbs[2], &cone );
  zBox3DCreateAlign( &box, &c, 1, 2, 3 );
  zNURBS3DBox( &nurbs[3], &box );
  zCyl3DCreate( &cyl, &c, &c2, 1, 0 );
  zNURBS3DCyl( &nurbs[4], &cyl );
  for( i=0; i<N/10; i++ ){
    vec3D_rand( &p, 3 );
    if( i % 10 == 0 ){ p.c.x = c.c.x + zRandF(-1.0e-3,1.0e-3); p.c.y = c.c.y; } /* around the axis */
    for( k=0; k<5; k++ ){
      d = zNURBS3DSignedDistFromPoint( &nurbs[k], &p );
      switch( k ){
      case 0: dref = zSphere3DSignedDistFromPoint( &sphere, &p ); break;
      case 1: dref = zEllips3DSignedDistFromPoint( &ellips, &p ); break;
      case 2: dref = zCone3DSignedDistFromPoint( &cone, &p ); break;
      case 3: dref = zBox3DSignedDistFromPoint( &box, &p ); break;
      default: dref = zCyl3DSignedDistFromPoint( &cyl, &p );
      }
      /* the points closest to creases are found with a less accuracy */
      if( !zIsTol( d - dref, 1.0e-5 ) ) result_dist = false;
      if( !zEqual( fabs( d ), zNURBS3DClosest( &nurbs[k], &p, &cp, &u, &v ), zTOL ) ||
          !zEqual( fabs( d ), zNURBS3DDistFromPoint( &nurbs[k], &p ), zTOL ) ) result_dist = false;
      if( !zNURBS3DPointIsInside( &nurbs[k], &p, d + 1.0e-9 ) || zNURBS3DPointIsInside( &nurbs[k], &p, d - 1.0e-9 ) ) result_inside = false;
    }
    shape.body = &nurbs[0];
    shape.com = &zeo_shape3d_nurbs_com;
    if( zShape3DSignedDistFromPoint( &shape, &p ) != zNURBS3DSignedDistFromPoint( &nurbs[0], &p ) ||
        zShape3DDistFromPoint( &shape, &p ) != zNURBS3DDistFromPoint( &nurbs[0], &p ) ) result_shape = false;
  }
  for( k=0; k<5; k++ ) zNURBS3DDestroy( &nurbs[k] );
  zAssert( zNURBS3DDistFromPoint + zNURBS3DSignedDistFromPoint, result_dist );
  zAssert( zNURBS3DPointIsInside, result_inside );
  zAssert( zShape3DDistFromPoint + zShape3DSignedDistFromPoint (NURBS), result_shape );
}

/* the NURBS surfaces of a box and an elliptic cylinder are continuous across the knots. */
void assert_nurbs3D_closed(void)
{
  zBox3D box;
  zECyl3D ecyl;
  zNURBS3D nurbs[2];
  zVec3D c1, c2, p1, p2;
  double t, s;
  int i, j, k, l;
  bool result = true;

  zVec3DCreate( &c1, 0.1, 0.2, 0.3 );
  zVec3DCreate( &c2, 0.3, 0.1, 2.3 );
  zBox3DCreateAlign( &box, &c1, 1, 2, 3 );
  zNURBS3DBox( &nurbs[0], &box );
  zECyl3DCreate( &ecyl, &c1, &c2, 1, 2, ZVEC3DX, 0 );
  zNURBS3DECyl( &nurbs[1], &ecyl );
  for( l=0; l<2; l++ )
    for( k=0; k<2; k++ )
      for( i=0; i<zNURBS3DKnotNum(&nurbs[l],k); i++ ){
        t = zNURBS3DKnot(&nurbs[l],k,i);
        if( t <= zNURBS3DKnotS(&nurbs[l],k) || t >= zNURBS3DKnotE(&nurbs[l],k) ) continue;
        for( j=0; j<=10; j++ ){
          s = zNURBS3DKnotS(&nurbs[l],1-k) + ( zNURBS3DKnotE(&nurbs[l],1-k) - zNURBS3DKnotS(&nurbs[l],1-k) ) * j / 10;
          if( k == 0 ){
            zNURBS3DVec( &nurbs[l], t-zTOL, s, &p1 );
            zNURBS3DVec( &nurbs[l], t+zTOL, s, &p2 );
          } else{
            zNURBS3DVec( &nurbs[l], s, t-zTOL, &p1 );
            zNURBS3DVec( &nurbs[l], s, t+zTOL, &p2 );
          }
          if( zVec3DDist( &p1, &p2 ) > 1.0e-9 ) result = false;
        }
      }
  zNURBS3DDestroy( &nurbs[0] );
  zNURBS3DDestroy( &nurbs[1] );
  zAssert( zNURBS3DBox + zNURBS3DECyl (continuity), result );
}

int main(void)
{
  zRandInit();
  dir3D_init();
  assert_inside_3D();
  assert_inside_2D();
  assert_inside_boundary();
  assert_inside_ellips();
  assert_signed_dist_thin();
  assert_shape3D_signed_dist();
  assert_nurbs3D_signed_dist();
  assert_nurbs3D_closed();
  assert_capsule_dist();
  return 0;
}
