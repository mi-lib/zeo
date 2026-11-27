#include <zeo/zeo.h>
#include <zeo/zeo_pg2d.h>

/* signed area of a triangle, which is positive if the vertices are counterclockwise. */
double tri_signed_area(zTri2D *t)
{
  zVec2D d1, d2;

  zVec2DSub( zTri2DVert(t,1), zTri2DVert(t,0), &d1 );
  zVec2DSub( zTri2DVert(t,2), zTri2DVert(t,0), &d2 );
  return 0.5 * zVec2DOuterProd( &d1, &d2 );
}

/* create a polygon from vertices and triangles given by indices. */
zPG2D *create_pg(zPG2D *pg, const double v[][2], int nv, const int f[][3], int nf)
{
  int i;

  if( !zPG2DAlloc( pg, nv, nf ) ) return NULL;
  for( i=0; i<nv; i++ ) zVec2DCreate( zPG2DVert(pg,i), v[i][0], v[i][1] );
  for( i=0; i<nf; i++ )
    zTri2DCreate( zPG2DFace(pg,i), zPG2DVert(pg,f[i][0]), zPG2DVert(pg,f[i][1]), zPG2DVert(pg,f[i][2]) );
  return pg;
}

/* a square [-1,1]x[-1,1] */
const double square_vert[][2] = { { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 } };
const int square_face[][3] = { { 0, 1, 2 }, { 0, 2, 3 } };

/* an L-shaped polygon, the union of [0,2]x[0,1] and [0,1]x[0,2], with a clockwise triangle */
const double lshape_vert[][2] = { { 0, 0 }, { 2, 0 }, { 2, 1 }, { 1, 1 }, { 1, 2 }, { 0, 2 } };
const int lshape_face[][3] = { { 0, 1, 2 }, { 0, 2, 3 }, { 0, 3, 5 }, { 3, 5, 4 } };

/* distance from a point to an axis-aligned rectangle [min,max], and the closest point. */
double rect_closest(double xmin, double ymin, double xmax, double ymax, const zVec2D *p, zVec2D *cp)
{
  zVec2DCreate( cp, zLimit( p->c.x, xmin, xmax ), zLimit( p->c.y, ymin, ymax ) );
  return zVec2DDist( p, cp );
}

/* signed distance from a point to the L-shaped polygon and the closest point. */
double lshape_closest(const zVec2D *p, zVec2D *cp)
{
  zVec2D cp1, cp2;
  double d1, d2;

  d1 = rect_closest( 0, 0, 2, 1, p, &cp1 );
  d2 = rect_closest( 0, 0, 1, 2, p, &cp2 );
  zVec2DCopy( d1 < d2 ? &cp1 : &cp2, cp );
  return zMin( d1, d2 );
}

/* distance from a point to a segment ( a, b ). */
double seg_dist(const double a[], const double b[], const zVec2D *p)
{
  double ex, ey, dx, dy, t;

  ex = b[0] - a[0]; ey = b[1] - a[1];
  dx = p->c.x - a[0]; dy = p->c.y - a[1];
  t = zLimit( ( ex*dx + ey*dy ) / ( ex*ex + ey*ey ), 0, 1 );
  return sqrt( zSqr( dx - t*ex ) + zSqr( dy - t*ey ) );
}

/* distance from a point to the boundary of the L-shaped polygon. */
double lshape_depth(const zVec2D *p)
{
  double d = HUGE_VAL;
  int i;

  for( i=0; i<6; i++ )
    d = zMin( d, seg_dist( lshape_vert[i], lshape_vert[(i+1)%6], p ) );
  return d;
}

void assert_alloc_clone(void)
{
  zPG2D pg, pg2;
  int i, j;
  bool result_alloc, result_clone = true;

  zPG2DInit( &pg );
  result_alloc = zPG2DVertNum(&pg) == 0 && zPG2DFaceNum(&pg) == 0;
  create_pg( &pg, lshape_vert, 6, lshape_face, 4 );
  result_alloc = result_alloc && zPG2DVertNum(&pg) == 6 && zPG2DFaceNum(&pg) == 4;
  zPG2DClone( &pg, &pg2 );
  if( zPG2DVertNum(&pg2) != 6 || zPG2DFaceNum(&pg2) != 4 ) result_clone = false;
  for( i=0; i<zPG2DVertNum(&pg); i++ )
    if( !zVec2DEqual( zPG2DVert(&pg,i), zPG2DVert(&pg2,i) ) ) result_clone = false;
  for( i=0; i<zPG2DFaceNum(&pg); i++ )
    for( j=0; j<3; j++ ) /* vertices of faces refer to the vertices of the clone */
      if( zPG2DFaceVert(&pg2,i,j) - zPG2DVertBuf(&pg2) != zPG2DFaceVert(&pg,i,j) - zPG2DVertBuf(&pg) )
        result_clone = false;
  zPG2DDestroy( &pg );
  zPG2DDestroy( &pg2 );
  result_alloc = result_alloc && zPG2DVertNum(&pg) == 0 && zPG2DFaceNum(&pg) == 0;
  zAssert( zPG2DAlloc + zPG2DDestroy, result_alloc );
  zAssert( zPG2DClone, result_clone );
}

void assert_mirror_scale_xform(void)
{
  zPG2D pg, pg2, pg3;
  zFrame2D f;
  zVec2D p;
  double s;
  int i;
  zAxis axis;
  bool result_mirror = true, result_scale = true, result_xform = true;

  create_pg( &pg, lshape_vert, 6, lshape_face, 4 );
  /* mirror: vertices are mirrored, and the orientation of each face is kept. */
  for( axis=zX; axis<=zY; axis++ ){
    zPG2DMirror( &pg, &pg2, axis );
    for( i=0; i<zPG2DVertNum(&pg); i++ ){
      zVec2DCopy( zPG2DVert(&pg,i), &p );
      p.e[(int)axis] *= -1;
      if( !zVec2DEqual( &p, zPG2DVert(&pg2,i) ) ) result_mirror = false;
    }
    for( i=0; i<zPG2DFaceNum(&pg); i++ )
      if( tri_signed_area( zPG2DFace(&pg,i) ) * tri_signed_area( zPG2DFace(&pg2,i) ) <= 0 ) result_mirror = false;
    zPG2DDestroy( &pg2 );
  }
  /* scale */
  s = zRandF( 0.1, 10 );
  zPG2DScale( &pg, &pg2, s );
  for( i=0; i<zPG2DVertNum(&pg); i++ ){
    zVec2DMul( zPG2DVert(&pg,i), s, &p );
    if( !zVec2DEqual( &p, zPG2DVert(&pg2,i) ) ) result_scale = false;
  }
  if( !zIsTiny( zPG2DArea(&pg2) - s*s*zPG2DArea(&pg) ) ) result_scale = false;
  zPG2DScaleDRC( &pg2, 1/s );
  for( i=0; i<zPG2DVertNum(&pg); i++ )
    if( !zVec2DEqual( zPG2DVert(&pg,i), zPG2DVert(&pg2,i) ) ) result_scale = false;
  /* transformation and the inverse */
  zVec2DCreate( &p, zRandF(-10,10), zRandF(-10,10) );
  zFrame2DCreateAngle( &f, &p, zRandF(-zPI,zPI) );
  zPG2DClone( &pg, &pg3 );
  zPG2DXform( &pg, &f, &pg2 );
  if( !zIsTiny( zPG2DArea(&pg2) - zPG2DArea(&pg) ) ) result_xform = false;
  zPG2DXformInv( &pg2, &f, &pg3 );
  for( i=0; i<zPG2DVertNum(&pg); i++ )
    if( !zVec2DEqual( zPG2DVert(&pg,i), zPG2DVert(&pg3,i) ) ) result_xform = false;
  zPG2DDestroy( &pg );
  zPG2DDestroy( &pg2 );
  zPG2DDestroy( &pg3 );
  zAssert( zPG2DMirror, result_mirror );
  zAssert( zPG2DScale + zPG2DScaleDRC, result_scale );
  zAssert( zPG2DXform + zPG2DXformInv, result_xform );
}

void assert_contigvert(void)
{
  zPG2D pg;
  zVec2D p;
  const zVec2D *v;
  double d, dmin;
  int i, k, n = 1000;
  bool result = true;

  create_pg( &pg, lshape_vert, 6, lshape_face, 4 );
  for( k=0; k<n; k++ ){
    zVec2DCreate( &p, zRandF(-3,3), zRandF(-3,3) );
    v = zPG2DContigVert( &pg, &p, &d );
    for( dmin=HUGE_VAL, i=0; i<zPG2DVertNum(&pg); i++ )
      dmin = zMin( dmin, zVec2DDist( &p, zPG2DVert(&pg,i) ) );
    if( d != dmin || zVec2DDist( &p, v ) != dmin ) result = false;
  }
  zPG2DDestroy( &pg );
  zAssert( zPG2DContigVert, result );
}

void assert_closest_inside(void)
{
  zPG2D pg;
  zVec2D p, cp, cpt;
  double d, dt, depth, margin;
  int k, n = 10000;
  bool result_convex = true, result_nonconvex = true;
  bool result_inside0 = true, result_inside_pos = true, result_inside_neg = true, result_signed = true;

  create_pg( &pg, square_vert, 4, square_face, 2 );
  for( k=0; k<n; k++ ){
    zVec2DCreate( &p, zRandF(-3,3), zRandF(-3,3) );
    d = zPG2DClosest( &pg, &p, &cp );
    dt = rect_closest( -1, -1, 1, 1, &p, &cpt );
    if( !zIsTiny( d - dt ) || !zVec2DEqual( &cp, &cpt ) ) result_convex = false;
  }
  zPG2DDestroy( &pg );
  create_pg( &pg, lshape_vert, 6, lshape_face, 4 );
  for( k=0; k<n; k++ ){
    zVec2DCreate( &p, zRandF(-1,3), zRandF(-1,3) );
    d = zPG2DClosest( &pg, &p, &cp );
    dt = lshape_closest( &p, &cpt );
    if( !zIsTiny( d - dt ) || !zVec2DEqual( &cp, &cpt ) ) result_nonconvex = false;
    depth = dt > 0 ? -dt : lshape_depth( &p ); /* signed distance is -depth */
    if( !zIsTiny( zPG2DSignedDistFromPoint( &pg, &p ) + depth ) ) result_signed = false;
    if( zPG2DPointIsInside( &pg, &p, 0 ) != ( -depth < 0 ) ) result_inside0 = false;
    margin = 0.2;
    if( zPG2DPointIsInside( &pg, &p, margin ) != ( -depth < margin ) ) result_inside_pos = false;
    margin = -0.2;
    if( zPG2DPointIsInside( &pg, &p, margin ) != ( -depth < margin ) ) result_inside_neg = false;
  }
  zPG2DDestroy( &pg );
  zAssert( zPG2DClosest (convex polygon), result_convex );
  zAssert( zPG2DClosest (non-convex polygon), result_nonconvex );
  zAssert( zPG2DSignedDistFromPoint (non-convex polygon), result_signed );
  zAssert( zPG2DPointIsInside (zero margin), result_inside0 );
  zAssert( zPG2DPointIsInside (positive margin), result_inside_pos );
  zAssert( zPG2DPointIsInside (negative margin), result_inside_neg );
}

/* a square with a vertex on an edge, triangulated with a degenerate triangle */
const double square2_vert[][2] = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 } };
const int square2_face[][3] = { { 0, 1, 2 }, { 0, 2, 3 }, { 0, 3, 4 } };

void assert_closest_degenerate(void)
{
  zPG2D pg;
  zVec2D p, cp, cpt;
  int k, n = 1000;
  bool result = true;

  create_pg( &pg, square2_vert, 5, square2_face, 3 );
  for( k=0; k<n; k++ ){
    zVec2DCreate( &p, zRandF(-3,3), zRandF(-3,3) );
    zPG2DClosest( &pg, &p, &cp );
    rect_closest( -1, -1, 1, 1, &p, &cpt );
    if( !zVec2DEqual( &cp, &cpt ) ) result = false;
  }
  zPG2DDestroy( &pg );
  zAssert( zPG2DClosest (degenerate triangle), result );
}

void assert_area_barycenter(void)
{
  zPG2D pg;
  zVec2D c, ct;

  create_pg( &pg, square_vert, 4, square_face, 2 );
  zAssert( zPG2DArea (square), zIsTiny( zPG2DArea(&pg) - 4 ) );
  zPG2DBarycenter( &pg, &c );
  zAssert( zPG2DBarycenter (square), zVec2DIsTiny( &c ) );
  zPG2DDestroy( &pg );
  create_pg( &pg, lshape_vert, 6, lshape_face, 4 );
  zAssert( zPG2DArea (non-convex polygon), zIsTiny( zPG2DArea(&pg) - 3 ) );
  zPG2DBarycenter( &pg, &c );
  zVec2DCreate( &ct, 2.5/3, 2.5/3 );
  zAssert( zPG2DBarycenter (non-convex polygon), zVec2DEqual( &c, &ct ) );
  zPG2DDestroy( &pg );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_alloc_clone();
  assert_mirror_scale_xform();
  assert_contigvert();
  assert_closest_inside();
  assert_closest_degenerate();
  assert_area_barycenter();
  return EXIT_SUCCESS;
}
