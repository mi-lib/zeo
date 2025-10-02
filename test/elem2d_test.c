#include <zeo/zeo_elem2d.h>

#define N 1000

void assert_line2D_point(void)
{
  zLine2D line;
  zVec2D pos, dir, p_test, p_result, dp;

  zAssert( zLine2DCreate (null pointer case), !zLine2DCreate( &line, NULL, NULL ) );
  zAssert( zLine2DCreate (zero normal case), !zLine2DCreate( &line, ZVEC2DZERO, ZVEC2DZERO ) );
  do{
    zVec2DCreate( &pos, zRandF(-10,10), zRandF(-10,10) );
    zVec2DCreate( &dir, zRandF(-10,10), zRandF(-10,10) );
  } while( !zLine2DCreate( &line, &pos, &dir ) );
  zVec2DCreate( &p_test, zRandF(-10,10), zRandF(-10,10) );
  /* point on the line */
  zLine2DPoint( &line, zRandF(-10,10), &p_result );
  zAssert( zLine2DPoint + zLine2DPointIsOn, zLine2DPointIsOn( &line, &p_result, zTOL ) );
  /* projection of a point */
  zLine2DProjPoint( &line, &p_test, &p_result );
  zVec2DSub( &p_test, &p_result, &dp );
  zAssert( zLine2DProjPoint,
    zIsTiny( zVec2DInnerProd( zLine2DDir(&line), &dp ) ) && zLine2DPointIsOn( &line, &p_result, zTOL ) &&
    zLine2DPointIsOn( &line, &p_result, zTOL ) );
  zAssert( zLine2DDistFromPoint,
    zEqual( zVec2DNorm( &dp ), zLine2DDistFromPoint( &line, &p_test ), zTOL ) );
}

void assert_line2D_intersection(void)
{
  zLine2D line1, line2;
  zVec2D pos1, pos2, dir, p;

  zVec2DCreate( &pos1, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( &pos2, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreate( &dir,  zRandF(-10,10), zRandF(-10,10) );
  zLine2DCreate( &line1, &pos1, &dir );
  zVec2DCreate( &dir,  zRandF(-10,10), zRandF(-10,10) );
  zLine2DCreate( &line2, &pos2, &dir );
  zIntersectLine2D( &line1, &line2, &p );
  zAssert( zIntersectLine2D, zLine2DPointIsOn( &line1, &p, zTOL ) && zLine2DPointIsOn( &line2, &p, zTOL ) );
}

void assert_edge2D(void)
{
  zEdge2D edge;
  zVec2D p1, p2, v, p, pp;
  double r;

  zVec2DCreate( &p1, zRandF(-10,10), zRandF(-10,10) );
  zVec2DCreatePolar( &v, ( r = zRandF(1,10) ), zRandF(-zPI,zPI) );
  zVec2DAdd( &p1, &v, &p2 );
  zEdge2DCreate( &edge, &p1, &p2 );
  zAssert( zEdge2DCreate, zVec2DEqual( zEdge2DVec(&edge), &v ) );
  zAssert( zEdge2DLen, zEqual( zEdge2DLen(&edge), r, zTOL ) );

  zVec2DCreate( &p, zRandF(-10,10), zRandF(-10,10) );
  zEdge2DProjPoint( &edge, &p, &pp );
  zVec2DSub( &pp, zEdge2DVert(&edge,1), &v );
  zAssert( zEdge2DProjPoint, zIsTiny( zVec2DOuterProd( &v, zEdge2DVec(&edge) ) ) );
  zAssert( zEdge2DPointIsOnLine, zEdge2DPointIsOnLine( &edge, &pp, zTOL ) );

  zVec2DCat( zEdge2DVert(&edge,0), 0.5, zEdge2DVec(&edge), &p );
  zAssert( zEdge2DPointIsOn (true case), zEdge2DPointIsOn( &edge, &p, zTOL ) );
  zVec2DCat( zEdge2DVert(&edge,0), 2, zEdge2DVec(&edge), &p );
  zAssert( zEdge2DPointIsOn (false case), !zEdge2DPointIsOn( &edge, &p, zTOL ) );
  zVec2DCat( zEdge2DVert(&edge,0),-1, zEdge2DVec(&edge), &p );
  zAssert( zEdge2DPointIsOn (false case), !zEdge2DPointIsOn( &edge, &p, zTOL ) );
  zVec2DRot( zEdge2DVec(&edge), zPI_2, &v );
  zVec2DAdd( zEdge2DVert(&edge,0), &v, &p );
  zVec2DCatDRC( &p, 0.5, zEdge2DVec(&edge) );
  zAssert( zEdge2DPointIsOn (false case), !zEdge2DPointIsOn( &edge, &p, zTOL ) );
  zVec2DAdd( zEdge2DVert(&edge,0), &v, &p );
  zVec2DCatDRC( &p, -0.5, zEdge2DVec(&edge) );
  zAssert( zEdge2DContigVert (case 1), zEdge2DContigVert( &edge, &p, NULL ) == zEdge2DVert(&edge,0) );
  zVec2DAdd( zEdge2DVert(&edge,0), &v, &p );
  zVec2DCatDRC( &p, 2.0, zEdge2DVec(&edge) );
  zAssert( zEdge2DContigVert (case 2), zEdge2DContigVert( &edge, &p, NULL ) == zEdge2DVert(&edge,1) );
}

void assert_edge2D_closest(void)
{
  zEdge2D edge;
  zVec2D vert[2], point, closestpoint, test_point;
  int i, j;
  const int n = 1000;
  double dist;
  bool result = true;

  /* trivial case 1 */
  zVec2DCreate( &vert[0],-1, 1 );
  zVec2DCreate( &vert[1], 1, 1 );
  zEdge2DCreate( &edge, &vert[0], &vert[1] );
  zEdge2DClosest( &edge, ZVEC2DZERO, &closestpoint );
  zAssert( zEdge2DClosest (trivial case 1), closestpoint.c.x == 0 && closestpoint.c.y == 1 );
  /* trivial case 2 */
  zVec2DCreate( &point,-2, 0 );
  zEdge2DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge2DClosest (trivial case 2), zVec2DEqual( &closestpoint, &vert[0] ) );
  /* trivial case 3 */
  zVec2DCreate( &point, 2, 0 );
  zEdge2DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge2DClosest (trivial case 3), zVec2DEqual( &closestpoint, &vert[1] ) );
  /* trivial case 4 */
  zVec2DCreate( &point, 0, 1 );
  zEdge2DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge2DClosest (trivial case 4), closestpoint.c.x == 0 && closestpoint.c.y == 1 );
  /* trivial case 5 */
  zVec2DCreate( &point,-2, 1 );
  zEdge2DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge2DClosest (trivial case 5), zVec2DEqual( &closestpoint, &vert[0] ) );
  /* trivial case 6 */
  zVec2DCreate( &point, 2, 1 );
  zEdge2DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge2DClosest (trivial case 6), zVec2DEqual( &closestpoint, &vert[1] ) );
  /* general case */
  zVec2DCreate( &vert[0], zRandF(-5,5), zRandF(-5,5) );
  zVec2DCreate( &vert[1], zRandF(-5,5), zRandF(-5,5) );
  zEdge2DCreate( &edge, &vert[0], &vert[1] );
  for( i=0; i<n; i++ ){
    zVec2DCreate( &point, zRandF(-10,10), zRandF(-10,10) );
    dist = zEdge2DClosest( &edge, &point, &closestpoint );
    for( j=0; j<2; j++ ){
      zVec2DInterDiv( &closestpoint, zEdge2DVert(&edge,j), 0.1, &test_point );
      if( zVec2DDist( &point, &test_point ) < dist ) result = false;
    }
  }
  zAssert( zEdge2DClosest, result );
}

void create_tri2D_rand(zVec2D v[], zTri2D *t, double min, double max)
{
  zVec2DCreate( &v[0], zRandF(min,max), zRandF(min,max) );
  zVec2DCreate( &v[1], zRandF(min,max), zRandF(min,max) );
  zVec2DCreate( &v[2], zRandF(min,max), zRandF(min,max) );
  zTri2DCreate( t, &v[0], &v[1], &v[2] );
}

#define TOL (1.0e-5)

void assert_tri2D_contig(void)
{
  zVec2D v[3], p;
  zTri2D t;
  double d[3], dc;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_tri2D_rand( v, &t, -1e10, 1e10 );
    zVec2DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zTri2DContigVert( &t, &p, &dc );
    d[0] = zVec2DDist( &v[0], &p );
    d[1] = zVec2DDist( &v[1], &p );
    d[2] = zVec2DDist( &v[2], &p );
    if( dc != zMin( d[0], zMin( d[1], d[2] ) ) ) result = false;
  }
  zAssert( zTri2DContigVert, result );
}

void assert_tri2D_center(void)
{
  zVec2D v[3], c, h[2], tmp;
  zTri2D t;
  zEdge2D e;
  int i, j;
  double r;
  bool ret;

  /* barycenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri2D_rand( v, &t, -5, 5 );
    zTri2DBarycenter( &t, &c );
    for( j=0; j<3; j++ ){
      zVec2DSub( &c, &v[j], &tmp );
      zVec2DMulDRC( &tmp, 1.5 );
      zVec2DSub( &v[(j+1)%3], &v[j], &h[0] );
      zVec2DSubDRC( &h[0], &tmp );
      zVec2DSub( &v[(j+2)%3], &v[j], &h[1] );
      zVec2DSubDRC( &h[1], &tmp );
      if( !zIsTol( zVec2DOuterProd( &h[0], &h[1] ), TOL ) ) ret = false;
    }
  }
  zAssert( zTri2DBarycenter, ret );
  /* circumcenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri2D_rand( v, &t, -5, 5 );
    zTri2DCircumcenter( &t, &c );
    r = zVec2DDist( &v[0], &c );
    if( !zIsTol( zVec2DDist( &v[1], &c ) - r, TOL ) || !zIsTol( zVec2DDist( &v[2], &c ) - r, TOL ) ){
      eprintf( "%.10g, %.10g\n", zVec2DDist( &v[1], &c ) - r, zVec2DDist( &v[2], &c ) - r );
      ret = false;
    }
  }
  zAssert( zTri2DCircumcenter, ret );
  /* incenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri2D_rand( v, &t, -5, 5 );
    zTri2DIncenter( &t, &c );
    r = zEdge2DDistFromPoint( zEdge2DCreate( &e, &v[0], &v[1] ), &c );
    if( !zIsTol( zEdge2DDistFromPoint( zEdge2DCreate( &e, &v[1], &v[2] ), &c ) - r, TOL ) ||
        !zIsTol( zEdge2DDistFromPoint( zEdge2DCreate( &e, &v[2], &v[0] ), &c ) - r, TOL ) )
      ret = false;
  }
  zAssert( zTri2DIncenter, ret );
  /* orthocenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri2D_rand( v, &t, -5, 5 );
    zTri2DOrthocenter( &t, &c );
    for( j=0; j<3; j++ ){
      zVec2DSub( &c, &v[j], &tmp );
      zVec2DSub( &v[(j+2)%3], &v[(j+1)%3], &h[0] );
      if( !zIsTol( zVec2DInnerProd( &tmp, &h[0] ), TOL ) ){
        eprintf( "%.10g\n",zVec2DInnerProd( &tmp, &h[0] ) );
        ret = false;
      }
    }
  }
  zAssert( zTri2DOrthocenter, ret );
}

void assert_tri2D_closest_trivial(void)
{
  zTri2D tri;
  zVec2D vert[3], point, closestpoint;

  zVec2DCreate( &vert[0],-0.5*cos(zPI/6), -0.5 );
  zVec2DCreate( &vert[1], 0.5*cos(zPI/6), -0.5 );
  zVec2DCreate( &vert[2],              0,    1 );
  zTri2DCreate( &tri, &vert[0], &vert[1], &vert[2] );
  /* trivial case 1 */
  zTri2DClosest( &tri, ZVEC2DZERO, &closestpoint );
  zAssert( zTri2DClosest (trivial case 1), zVec2DEqual( &closestpoint, ZVEC2DZERO ) );
  /* trivial case 2 */
  zVec2DCreate( &point, 0, -2 );
  zTri2DClosest( &tri, &point, &closestpoint );
  zAssert( zTri2DClosest (trivial case 2), closestpoint.c.x == 0 && closestpoint.c.y == -0.5 );
  /* trivial case 3 */
  zVec2DCreate( &point, 0, 2 );
  zTri2DClosest( &tri, &point, &closestpoint );
  zAssert( zTri2DClosest (trivial case 3), closestpoint.c.x == 0 && closestpoint.c.y == 1 );
  /* trivial case 4 */
  zVec2DCreate( &point, 2, -2 );
  zTri2DClosest( &tri, &point, &closestpoint );
  zAssert( zTri2DClosest (trivial case 4), zVec2DEqual( &closestpoint, &vert[1] ) );
  /* trivial case 5 */
  zVec2DCreate( &point,-2, -2 );
  zTri2DClosest( &tri, &point, &closestpoint );
  zAssert( zTri2DClosest (trivial case 5), zVec2DEqual( &closestpoint, &vert[0] ) );
}

void assert_tri2D_closest(void)
{
  zTri2D tri;
  zVec2D vert[3], point, closestpoint, test_point;
  int i, j;
  const int n = 1000;
  double dist;
  bool result = true;

  for( i=0; i<3; i++ )
    zVec2DCreate( &vert[i], zRandF(-5,5), zRandF(-5,5) );
  zTri2DCreate( &tri, &vert[0], &vert[1], &vert[2] );

  for( i=0; i<n; i++ ){
    zVec2DCreate( &point, zRandF(-10,10), zRandF(-10,10) );
    dist = zTri2DClosest( &tri, &point, &closestpoint );
    for( j=0; j<3; j++ ){
      zVec2DInterDiv( &closestpoint, zTri2DVert(&tri,j), 0.1, &test_point );
      if( zVec2DDist( &point, &test_point ) < dist ) result = false;
    }
  }
  zAssert( zTri2DClosest, result );
}

void assert_aabox2D_create(void)
{
  zAABox2D box;
  int i;
  const int n = 10;
  bool result = true;

  for( i=0; i<n; i++ ){
    zAABox2DCreate( &box, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    if( zAABox2DXMin(&box) > zAABox2DXMax(&box) ||
        zAABox2DYMin(&box) > zAABox2DYMax(&box) ) result = false;
  }
  zAssert( zAABox2DCreate, result );
}

void assert_aabox2D_equal(void)
{
  zAABox2D box1, box2;

  zAABox2DCreate( &box1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAABox2DCopy( &box1, &box2 );
  zAssert( zAABox2DCopy, zAABox2DEqual( &box1, &box2 ) );
}

void assert_aabox2D_expand(void)
{
  zAABox2D box1, box2, box3, box_test;
  zVec2D center;
  double depth, width;

  zVec2DCreate( &center, zRandF(-10,10), zRandF(-10,10) );
  depth  = zRandF( 0.1, 10.0 );
  width  = zRandF( 0.1, 10.0 );
  zAABox2DCreateFromSize( &box1, &center, depth, width );
  zAABox2DExpand( &box1, 0.5, &box2 );
  zAABox2DExpand( &box1, 2.0, &box3 );
  zAssert( zAABox2DExpand,
    zAABox2DEqual( zAABox2DExpand( &box2, 2.0, &box_test ), &box1 ) &&
    zAABox2DEqual( zAABox2DExpand( &box3, 0.5, &box_test ), &box1 ) &&
    zAABox2DEqual( zAABox2DExpand( &box2, 4.0, &box_test ), &box3 ) );
  zAABox2DCopy( &box1, &box_test );
  zAssert( zAABox2DExpandDRC,
    zAABox2DEqual( zAABox2DExpandDRC( &box1, 0.5 ), &box2 ) &&
    zAABox2DEqual( zAABox2DExpandDRC( &box_test, 2.0 ), &box3 ) );
}

double aabox2D_dist_from_point_base(const zAABox2D *box, const zVec2D *point)
{
  zVec2D cp;
  return zAABox2DClosest( box, point, &cp );
}

void assert_aabox2D_dist(void)
{
  zAABox2D box;
  zVec2D point;
  double dist1, dist2;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    zAABox2DCreate( &box, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec2DCreate( &point, zRandF(-10,10), zRandF(-10,10) );
    dist1 = zAABox2DDistFromPoint( &box, &point );
    dist2 = aabox2D_dist_from_point_base( &box, &point );
    if( !zEqual( dist1, dist2, zTOL ) ) result = false;
  }
  zAssert( zAABox2DPointDist, result );
}

zVec2D *test_ellips2D_norm(zEllips2D *e, zVec2D *p, zVec2D *n)
{
  zVec2DCreate( n,
    ( p->c.x - zEllips2DCenter(e)->c.x ) / zSqr( zEllips2DRadius(e,0) ),
    ( p->c.y - zEllips2DCenter(e)->c.y ) / zSqr( zEllips2DRadius(e,1) ) );
  zVec2DNormalizeDRC( n );
  return n;
}

double test_ellips2D_residual(zEllips2D *e, zVec2D *p, double margin)
{
  return zSqr( ( p->c.x - zEllips2DCenter(e)->c.x ) / ( zEllips2DRadius(e,0) + margin ) )
       + zSqr( ( p->c.y - zEllips2DCenter(e)->c.y ) / ( zEllips2DRadius(e,1) + margin ) ) - 1;
}

void assert_ellips2D_inside(void)
{
  zEllips2D e;
  zVec2D p, c, cp;
  zVec2D d, n;
  int i;
  bool result = true;

  zVec2DCreate( &c, zRandF(-1,1), zRandF(-1,1) );
  zEllips2DCreate( &e, &c, zRandF(1,3), zRandF(1,3) );
  for( i=0; i<N; i++ ){
    zVec2DCreate( &p, zRandF(-4,4), zRandF(-4,4) );
    if( zEllips2DPointIsInside( &e, &p, zTOL ) ){
      if( test_ellips2D_residual( &e, &p, zTOL ) > 0 ) result = false;
    } else{
      zEllips2DClosest( &e, &p, &cp );
      zVec2DSub( &p, &cp, &d );
      test_ellips2D_norm( &e, &cp, &n );
      if( ! zIsTiny( zVec2DOuterProd( &d, &n ) ) ) result = false;
    }
  }
  zAssert( zEllips2DClosest, result );
}

int main(void)
{
  zRandInit();
  assert_line2D_point();
  assert_line2D_intersection();
  assert_edge2D();
  assert_edge2D_closest();
  assert_tri2D_center();
  assert_tri2D_contig();
  assert_tri2D_closest_trivial();
  assert_tri2D_closest();
  assert_aabox2D_create();
  assert_aabox2D_equal();
  assert_aabox2D_expand();
  assert_aabox2D_dist();
  assert_ellips2D_inside();
  return 0;
}
