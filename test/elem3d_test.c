#include <zeo/zeo.h>

#define N 1000

void assert_line3D_point(void)
{
  zLine3D line;
  zVec3D pos, dir, p_test, p_result, dp;

  zAssert( zLine3DCreate (null pointer case), !zLine3DCreate( &line, NULL, NULL ) );
  zAssert( zLine3DCreate (zero normal case), !zLine3DCreate( &line, ZVEC3DZERO, ZVEC3DZERO ) );
  do{
    zVec3DCreate( &pos, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &dir, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  } while( !zLine3DCreate( &line, &pos, &dir ) );
  zVec3DCreate( &p_test, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  /* point on the line */
  zLine3DPoint( &line, zRandF(-10,10), &p_result );
  zAssert( zLine3DPoint + zLine3DPointIsOn, zLine3DPointIsOn( &line, &p_result, zTOL ) );
  /* projection of a point */
  zLine3DProjPoint( &line, &p_test, &p_result );
  zVec3DSub( &p_test, &p_result, &dp );
  zAssert( zLine3DProjPoint,
    zIsTiny( zVec3DInnerProd( zLine3DDir(&line), &dp ) ) && zLine3DPointIsOn( &line, &p_result, zTOL ) &&
    zLine3DPointIsOn( &line, &p_result, zTOL ) );
}

void assert_line3D_commonperp(void)
{
  zLine3D line1, line2, commonperp;
  zVec3D pos1, pos2, posp, dir, p1, p2, p1_ans, p2_ans, dp;
  double thetas[] = { 1e-6, 1e-8, 1e-10 };
  double a, h, theta;
  int i;
  bool result = true;

  /* trivial case */
  zVec3DCreate( &pos1, 1, 0, 0 );
  zVec3DCreate( &pos2, 0, 1, 1 );
  zLine3DCreate( &line1, &pos1, ZVEC3DY );
  zLine3DCreate( &line2, &pos2, ZVEC3DX );
  zLine3DCommonPerpEnd( &line1, &line2, &p1, &p2 );
  zVec3DCreate( &p1_ans, 1, 1, 0 );
  zVec3DCreate( &p2_ans, 1, 1, 1 );
  zAssert( zLine3DCommonPerpEnd (trivial case),
    zVec3DEqual( &p1, &p1_ans ) && zVec3DEqual( &p2, &p2_ans ) );
  /* parallel case */
  zLine3DCreate( &line1, &pos1, ZVEC3DX );
  zLine3DCommonPerpEnd( &line1, &line2, &p1, &p2 );
  zAssert( zLine3DCommonPerpEnd (parallel case),
    zVec3DEqual( &p1, &pos1 ) && zVec3DEqual( &p2, &p2_ans ) );
  /* intersection case */
  zVec3DCreate( &pos1, 1, 0, 0 );
  zVec3DCreate( &pos2, 0, 1, 0 );
  zLine3DCreate( &line1, &pos1, ZVEC3DY );
  zLine3DCreate( &line2, &pos2, ZVEC3DX );
  zLine3DCommonPerpEnd( &line1, &line2, &p1, &p2 );
  zVec3DCreate( &p1_ans, 1, 1, 0 );
  zVec3DCreate( &p2_ans, 1, 1, 0 );
  zAssert( zLine3DCommonPerpEnd (intersection case),
    zVec3DEqual( &p1, &p1_ans ) && zVec3DEqual( &p2, &p2_ans ) );
  /* random case */
  zVec3DCreate( &pos1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &pos2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &dir,  zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zLine3DCreate( &line1, &pos1, &dir );
  zVec3DCreate( &dir,  zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zLine3DCreate( &line2, &pos2, &dir );
  zLine3DCommonPerpEnd( &line1, &line2, &p1, &p2 );
  zVec3DSub( &p2, &p1, &dp );
  zAssert( zLine3DCommonPerpEnd (random case),
    zIsTiny( zVec3DInnerProd( zLine3DDir(&line1), &dp ) ) &&
    zIsTiny( zVec3DInnerProd( zLine3DDir(&line2), &dp ) ) );
  /* lines */
  zLine3DCommonPerp( &line1, &line2, &posp, &commonperp );
  zAssert( zLine3DCommonPerp,
    zIsTiny( zVec3DInnerProd( zLine3DDir(&line1), zLine3DDir(&commonperp) ) ) &&
    zIsTiny( zVec3DInnerProd( zLine3DDir(&line2), zLine3DDir(&commonperp) ) ) );
  /* nearly parallel case: line1 is the x-axis, and line2 passes (a,0,h) with the angle theta */
  zVec3DCreate( &pos1, 0, 0, 0 );
  zLine3DCreate( &line1, &pos1, ZVEC3DX );
  for( i=0; i<3; i++ ){
    a = zRandF(-1e3,1e3);
    h = zRandF(-1e3,1e3);
    theta = thetas[i];
    zVec3DCreate( &pos2, a, 0, h );
    zVec3DCreate( &dir, cos(theta), sin(theta), 0 );
    zLine3DCreate( &line2, &pos2, &dir );
    zLine3DCommonPerpEnd( &line1, &line2, &p1, &p2 );
    zVec3DCreate( &p1_ans, a, 0, 0 );
    if( !zVec3DIsTol( zVec3DSubDRC( &p1, &p1_ans ), zTOL*1e3 ) ||
        !zVec3DIsTol( zVec3DSubDRC( &p2, &pos2 ), zTOL*1e3 ) ) result = false;
  }
  zAssert( zLine3DCommonPerpEnd (nearly parallel case), result );
}

void assert_line3D_distance(void)
{
  zLine3D line1, line2;
  zVec3D pos1, pos2, dir, p, pp1, pp2, p11, p12, p21, p22;
  double d;

  zVec3DCreate( &pos1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &pos2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &dir,  zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zLine3DCreate( &line1, &pos1, &dir );
  zVec3DCreate( &dir,  zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zLine3DCreate( &line2, &pos2, &dir );
  /* line-point distance */
  zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  d = zLine3DDistFromPoint( &line1, &p );
  zLine3DProjPoint( &line1, &p, &pp1 );
  zVec3DCat( &pp1, 1e-3, zLine3DDir(&line1), &p11 );
  zVec3DCat( &pp1,-1e-3, zLine3DDir(&line1), &p12 );
  zAssert( zLine3DDistFromPoint, zVec3DDist( &p, &p11 ) > d && zVec3DDist( &p, &p12 ) > d );
  /* line-line distance */
  d = zDistLine3DLine3D( &line1, &line2 );
  zLine3DCommonPerpEnd( &line1, &line2, &pp1, &pp2 );
  zVec3DCat( &pp1, 1.0e-3, zLine3DDir(&line1), &p11 );
  zVec3DCat( &pp1,-1.0e-3, zLine3DDir(&line1), &p12 );
  zVec3DCat( &pp2, 1.0e-3, zLine3DDir(&line2), &p21 );
  zVec3DCat( &pp2,-1.0e-3, zLine3DDir(&line2), &p22 );
  zAssert( zDistLine3DLine3D,
    zVec3DDist( &pp1, &p21 ) > d && zVec3DDist( &pp1, &p22 ) > d &&
    zVec3DDist( &pp2, &p11 ) > d && zVec3DDist( &pp2, &p12 ) > d );
}

void create_plane3D_rand(zVec3D *v, zPlane3D *p)
{
  zVec3D n;

  zVec3DCreate( v, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zVec3DCreate( &n, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DNormalizeDRC( &n, 0 );
  zPlane3DCreate( p, v, &n );
}

void create_edge3D_rand(zVec3D v[], zEdge3D *e)
{
  zVec3DCreate( &v[0], zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zVec3DCreate( &v[1], zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
  zEdge3DCreate( e, &v[0], &v[1] );
}

void create_tri3D_rand(zVec3D v[], zTri3D *t)
{
  zVec3DCreate( &v[0], zRandF(-1e3,1e3), zRandF(-1e3,1e3), zRandF(-1e3,1e3) );
  zVec3DCreate( &v[1], zRandF(-1e3,1e3), zRandF(-1e3,1e3), zRandF(-1e3,1e3) );
  zVec3DCreate( &v[2], zRandF(-1e3,1e3), zRandF(-1e3,1e3), zRandF(-1e3,1e3) );
  zTri3DCreate( t, &v[0], &v[1], &v[2] );
}

void create_tri3D_rand_minmax(zVec3D v[], zTri3D *t, double min, double max)
{
  zVec3DCreate( &v[0], zRandF(min,max), zRandF(min,max), zRandF(min,max) );
  zVec3DCreate( &v[1], zRandF(min,max), zRandF(min,max), zRandF(min,max) );
  zVec3DCreate( &v[2], zRandF(min,max), zRandF(min,max), zRandF(min,max) );
  zTri3DCreate( t, &v[0], &v[1], &v[2] );
}

void assert_edge3D(void)
{
  zEdge3D edge;
  zVec3D p1, p2, v, p, pp;
  double r;

  zVec3DCreate( &p1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreatePolar( &v, ( r = zRandF(1,10) ), zRandF(-zPI,zPI), zRandF(-zPI_2,zPI_2) );
  zVec3DAdd( &p1, &v, &p2 );
  zEdge3DCreate( &edge, &p1, &p2 );
  zAssert( zEdge3DCreate, zVec3DEqual( zEdge3DVec(&edge), &v ) );
  zAssert( zEdge3DLen, zEqual( zEdge3DLen(&edge), r, zTOL ) );

  zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zEdge3DProjPoint( &edge, &p, &pp );
  zVec3DSub( &pp, zEdge3DVert(&edge,1), &v );
  zAssert( zEdge3DProjPoint, zIsTiny( zVec3DOuterProdNorm( &v, zEdge3DVec(&edge) ) ) );
  zAssert( zEdge3DPointIsOnLine, zEdge3DPointIsOnLine( &edge, &pp, zTOL ) );

  zVec3DCat( zEdge3DVert(&edge,0), 0.5, zEdge3DVec(&edge), &p );
  zAssert( zEdge3DPointIsOn (true case), zEdge3DPointIsOn( &edge, &p, zTOL ) );
  zVec3DCat( zEdge3DVert(&edge,0), 2, zEdge3DVec(&edge), &p );
  zAssert( zEdge3DPointIsOn (false case), !zEdge3DPointIsOn( &edge, &p, zTOL ) );
  zVec3DCat( zEdge3DVert(&edge,0),-1, zEdge3DVec(&edge), &p );
  zAssert( zEdge3DPointIsOn (false case), !zEdge3DPointIsOn( &edge, &p, zTOL ) );
  zVec3DOrthonormal( zEdge3DVec(&edge), &v );
  zVec3DAdd( zEdge3DVert(&edge,0), &v, &p );
  zVec3DCatDRC( &p, 0.5, zEdge3DVec(&edge) );
  zAssert( zEdge3DPointIsOn (false case), !zEdge3DPointIsOn( &edge, &p, zTOL ) );
}

void assert_edge3D_closest(void)
{
  zEdge3D edge;
  zVec3D vert[2], point, closestpoint, test_point;
  int i, j;
  const int n = 1000;
  double dist;
  bool result = true;

  /* trivial case 1 */
  zVec3DCreate( &vert[0],-1, 1, 0 );
  zVec3DCreate( &vert[1], 1, 1, 0 );
  zEdge3DCreate( &edge, &vert[0], &vert[1] );
  zEdge3DClosest( &edge, ZVEC3DZERO, &closestpoint );
  zAssert( zEdge3DClosest (trivial case 1), closestpoint.c.x == 0 && closestpoint.c.y == 1 && closestpoint.c.z == 0 );
  /* trivial case 2 */
  zVec3DCreate( &point,-2, 0, 0 );
  zEdge3DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge3DClosest (trivial case 2), zVec3DEqual( &closestpoint, &vert[0] ) );
  /* trivial case 3 */
  zVec3DCreate( &point, 2, 0, 0 );
  zEdge3DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge3DClosest (trivial case 3), zVec3DEqual( &closestpoint, &vert[1] ) );
  /* trivial case 4 */
  zVec3DCreate( &point, 0, 1, 0 );
  zEdge3DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge3DClosest (trivial case 4), closestpoint.c.x == 0 && closestpoint.c.y == 1 && closestpoint.c.z == 0 );
  /* trivial case 5 */
  zVec3DCreate( &point,-2, 1, 0 );
  zEdge3DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge3DClosest (trivial case 5), zVec3DEqual( &closestpoint, &vert[0] ) );
  /* trivial case 6 */
  zVec3DCreate( &point, 2, 1, 0 );
  zEdge3DClosest( &edge, &point, &closestpoint );
  zAssert( zEdge3DClosest (trivial case 6), zVec3DEqual( &closestpoint, &vert[1] ) );
  /* general case */
  zVec3DCreate( &vert[0], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zVec3DCreate( &vert[1], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zEdge3DCreate( &edge, &vert[0], &vert[1] );

  for( i=0; i<n; i++ ){
    zVec3DCreate( &point, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    dist = zEdge3DClosest( &edge, &point, &closestpoint );
    for( j=0; j<2; j++ ){
      zVec3DInterDiv( &closestpoint, zEdge3DVert(&edge,j), 0.1, &test_point );
      if( zVec3DDist( &point, &test_point ) < dist ) result = false;
    }
  }
  zAssert( zEdge3DClosest, result );
}

void assert_edge3D_contig(void)
{
  zVec3D v[2], p;
  zEdge3D e;
  double d[2], dc;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_edge3D_rand( v, &e );
    zVec3DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zEdge3DContigVert( &e, &p, &dc );
    d[0] = zVec3DDist( &v[0], &p );
    d[1] = zVec3DDist( &v[1], &p );
    if( dc != zMin( d[0], d[1] ) ) result = false;
  }
  zAssert( zEdge3DContigVert, result );
}

/* check if the normal vector of a triangle is a unit vector orthogonal to the edges and directed
 * according to the order of the vertices. The error of the normal vector is proportional to the
 * ratio of the longest edge to the minimum height, which is considered in the tolerance. */
bool check_tri3D_norm(const zTri3D *tri)
{
  zVec3D e[3], n;
  double l2max = 0, tol;
  int i;

  for( i=0; i<3; i++ ){
    zVec3DSub( zTri3DVertNext(tri,i), zTri3DVert(tri,i), &e[i] );
    l2max = zMax( l2max, zVec3DSqrNorm( &e[i] ) );
  }
  zVec3DOuterProd( &e[0], &e[1], &n );
  tol = zMax( 100 * DBL_EPSILON * l2max / zVec3DNorm( &n ), zTOL );
  for( i=0; i<3; i++ ){
    zVec3DNormalizeDRC( &e[i], 0 );
    if( !zIsTol( zVec3DInnerProd( &e[i], zTri3DNorm(tri) ), tol ) ) return false;
  }
  return zIsTiny( zVec3DNorm( zTri3DNorm(tri) ) - 1 ) && zVec3DInnerProd( &n, zTri3DNorm(tri) ) > 0;
}

/* create a sliver triangle, the ratio of the height of which to the longest edge is r. */
void create_tri3D_sliver(zVec3D *v0, zVec3D *v1, double r, zVec3D *v2)
{
  zVec3D d;

  zVec3DSub( v1, v0, &d );
  zVec3DOrthonormal( &d, v2 );
  zVec3DMulDRC( v2, r * zVec3DNorm( &d ) );
  zVec3DCatDRC( v2, zRandF(0,1), &d );
  zVec3DAddDRC( v2, v0 );
}

/* check if a triangle is regarded as degenerate, where the normal vector is set for zero. */
bool check_tri3D_degenerate(const zVec3D *v1, const zVec3D *v2, const zVec3D *v3)
{
  zTri3D tri;

  return zTri3DCreate( &tri, v1, v2, v3 ) == NULL && zVec3DMatch( zTri3DNorm(&tri), ZVEC3DZERO );
}

void assert_tri3D_calc_norm(void)
{
  zTri3D tri;
  zVec3D v[3], d;
  double scale;
  const int n = 1000;
  int i, j;
  bool result = true, result_coincide = true, result_collinear = true, result_sliver = true;

  for( i=0; i<n; i++ ){
    scale = pow( 10, zRandF(-8,8) ); /* from tiny to huge triangles */
    for( j=0; j<3; j++ ){
      zVec3DCreate( &v[j], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
      zVec3DMulDRC( &v[j], scale );
    }
    if( !zTri3DCreate( &tri, &v[0], &v[1], &v[2] ) || !check_tri3D_norm( &tri ) ) result = false;
    /* coincident vertices */
    if( !check_tri3D_degenerate( &v[0], &v[0], &v[1] ) ||
        !check_tri3D_degenerate( &v[0], &v[1], &v[1] ) ||
        !check_tri3D_degenerate( &v[0], &v[1], &v[0] ) ||
        !check_tri3D_degenerate( &v[0], &v[0], &v[0] ) ) result_coincide = false;
    /* collinear vertices, where the third vertex is computed in floating-point arithmetics */
    zVec3DInterDiv( &v[0], &v[1], zRandF(-2,2), &v[2] );
    if( !check_tri3D_degenerate( &v[0], &v[1], &v[2] ) ) result_collinear = false;
    /* slivers: the ratio of the height to the longest edge is 1e-14 (degenerate) or 1e-9 (not
     * degenerate). The longest edge is as long as the distance from the origin, so that the rounding
     * error of the vertices does not affect the ratio. */
    zVec3DCreate( &d, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DNormalizeDRC( &d, 0 );
    zVec3DCat( &v[0], scale, &d, &v[1] );
    create_tri3D_sliver( &v[0], &v[1], 1.0e-14, &v[2] );
    if( !check_tri3D_degenerate( &v[0], &v[1], &v[2] ) ) result_sliver = false;
    create_tri3D_sliver( &v[0], &v[1], 1.0e-9, &v[2] );
    if( !zTri3DCreate( &tri, &v[0], &v[1], &v[2] ) || !check_tri3D_norm( &tri ) ) result_sliver = false;
  }
  zAssert( zTri3DCalcNorm (random triangles of various sizes), result );
  zAssert( zTri3DCalcNorm (coincident vertices), result_coincide );
  zAssert( zTri3DCalcNorm (collinear vertices), result_collinear );
  zAssert( zTri3DCalcNorm (slivers), result_sliver );
}

void assert_tri3D_is_degenerate(void)
{
  zTri3D tri;
  zVec3D v[3], d, e;
  double l, h;
  const int n = 1000;
  int i, j;
  bool result = true;

  for( i=0; i<n; i++ ){
    /* the height to the longest edge v[0]-v[1] is h */
    zVec3DCreate( &v[0], zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &d, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    l = zVec3DNorm( &d );
    zVec3DAdd( &v[0], &d, &v[1] );
    zVec3DOrthonormal( &d, &e );
    h = zRandF(0.01,0.3) * l;
    zVec3DInterDiv( &v[0], &v[1], zRandF(0.2,0.8), &v[2] );
    zVec3DCatDRC( &v[2], h, &e );
    for( j=0; j<3; j++ ){
      zTri3DCreate( &tri, &v[j], &v[(j+1)%3], &v[(j+2)%3] );
      if( !zTri3DIsDegenerate( &tri, h*(1+1.0e-6) ) || zTri3DIsDegenerate( &tri, h*(1-1.0e-6) ) ) result = false;
      zTri3DCreate( &tri, &v[j], &v[(j+2)%3], &v[(j+1)%3] );
      if( !zTri3DIsDegenerate( &tri, h*(1+1.0e-6) ) || zTri3DIsDegenerate( &tri, h*(1-1.0e-6) ) ) result = false;
    }
    /* coincident vertices */
    zTri3DCreate( &tri, &v[0], &v[0], &v[1] );
    if( !zTri3DIsDegenerate( &tri, 0 ) ) result = false;
  }
  zAssert( zTri3DIsDegenerate, result );
}

void assert_tri3D_contig(void)
{
  zVec3D v[3], p;
  zTri3D t;
  double d[3], dc;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_tri3D_rand( v, &t );
    zVec3DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zTri3DContigVert( &t, &p, &dc );
    d[0] = zVec3DDist( &v[0], &p );
    d[1] = zVec3DDist( &v[1], &p );
    d[2] = zVec3DDist( &v[2], &p );
    if( dc != zMin( d[0], zMin( d[1], d[2] ) ) ) result = false;
  }
  zAssert( zTri3DContigVert, result );
}

void assert_plane3D_create(void)
{
  zVec3D pos, norm;
  zPlane3D plane;

  zVec3DCreate( &pos,  zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &norm, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zPlane3DCreate( &plane, &pos, &norm );
  zAssert( zPlane3DCreate, zEqual( zVec3DNorm(zPlane3DNorm(&plane)), 1.0, zTOL ) );

  /* a small normal vector of which components are all within zTOL but of which norm is larger than zTOL */
  zVec3DCreate( &norm, 0.9*zTOL, -0.9*zTOL, 0.9*zTOL );
  zAssert( zPlane3DCreate (small normal vector),
    zPlane3DCreate( &plane, &pos, &norm ) && zEqual( zVec3DNorm(zPlane3DNorm(&plane)), 1.0, zTOL ) );
}

bool check_plane3D_create_fail(zVec3D *norm)
{
  zVec3D pos;
  zPlane3D plane;

  zPlane3DInit( &plane );
  zVec3DCreate( &pos, 1, 2, 3 );
  return !zPlane3DCreate( &plane, &pos, norm ) &&
         zPlane3DOrg(&plane) == ZVEC3DZERO && zVec3DIsTiny( zPlane3DNorm(&plane) );
}

void assert_plane3D_create_fail(void)
{
  zVec3D norm;
  bool result = true;

  if( !check_plane3D_create_fail( ZVEC3DZERO ) ) result = false;
  zVec3DCreate( &norm, 0.5*zTOL, 0.5*zTOL, 0.5*zTOL );
  if( !check_plane3D_create_fail( &norm ) ) result = false;
  zVec3DCreate( &norm, 1, NAN, 0 );
  if( !check_plane3D_create_fail( &norm ) ) result = false;
  zAssert( zPlane3DCreate (degenerate case), result );
}

void assert_plane3D_proj_point(void)
{
  zVec3D v, p, cp;
  zPlane3D pl;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_plane3D_rand( &v, &pl );
    zVec3DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zPlane3DProjPoint( &pl, &p, &cp );
    if( !zPlane3DPointIsOn( &pl, &cp, zTOL ) ) result = false;
  }
  zAssert( zPlane3DProjPoint + zPlane3DPointIsOn, result );
}

void assert_tri3D_proj_point(void)
{
  zVec3D v[3], p, cp;
  zTri3D t;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_tri3D_rand( v, &t );
    zVec3DCreate( &p, zRandF(-1e10,1e10), zRandF(-1e10,1e10), zRandF(-1e10,1e10) );
    zTri3DProjPoint( &t, &p, &cp );
    if( !zTri3DPointIsOnPlane( &t, &cp, zTOL ) ) result = false;
  }
  zAssert( zTri3DProjPoint + zTri3DPointIsOnPlane, result );
}

/* the rounding error of the distance from a point on a triangle is proportional to the size of the triangle. */
double tri3D_margin(const zVec3D v[])
{
  return zTOL * sqrt( zMax( zMax( zVec3DSqrDist( &v[0], &v[1] ), zVec3DSqrDist( &v[1], &v[2] ) ), zVec3DSqrDist( &v[2], &v[0] ) ) );
}

void assert_tri3D_point_is_inside(void)
{
  zVec3D v[3], p;
  zTri3D t;
  int i;
  double r1, r2, margin;
  bool result1, result2, result3, result4;

  result1 = result2 = result3 = result4 = true;
  for( i=0; i<N; i++ ){
    create_tri3D_rand( v, &t );
    margin = tri3D_margin( v );
    /* inside case */
    zVec3DZero( &p );
    zVec3DCatDRC( &p, ( r1 = zRandF(0,1) ), &v[0] );
    zVec3DCatDRC( &p, ( r2 = zRandF(0,1-r1) ), &v[1] );
    zVec3DCatDRC( &p, 1 - r1 - r2, &v[2] );
    if( !zTri3DPointIsInside( &t, &p, 1.0e-6 ) ) result1 = false;
    /* outside case */
    zVec3DZero( &p );
    zVec3DCatDRC( &p, ( r1 = zRandF(0,-1) ), &v[0] );
    zVec3DCatDRC( &p, ( r2 = zRandF(0,1-r1) ), &v[1] );
    zVec3DCatDRC( &p, 1 - r1 - r2, &v[2] );
    if( zTri3DPointIsInside( &t, &p, zTOL ) ) result2 = false;
    /* midpoint case */
    zVec3DMid( &v[0], &v[1], &p );
    if( !zTri3DPointIsInside( &t, &p, margin ) ) result3 = false;
    /* vertex case */
    zVec3DCopy( &v[2], &p );
    if( !zTri3DPointIsInside( &t, &p, margin ) ) result4 = false;
  }
  zAssert( zTri3DPointIsInside (true positive), result1 );
  zAssert( zTri3DPointIsInside (false positive), result2 );
  zAssert( zTri3DPointIsInside (midpoint case), result3 );
  zAssert( zTri3DPointIsInside (vertex case), result4 );
}

/* a random triangle, which is thin or degenerate depending on type. */
void create_tri3D_rand_type(zVec3D v[], zTri3D *t, int type)
{
  zVec3D e, n;

  zVec3DCreate( &v[0], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &v[1], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  switch( type ){
  case 1: /* thin triangle */
    zVec3DInterDiv( &v[0], &v[1], zRandF(-0.5,1.5), &v[2] );
    zVec3DSub( &v[1], &v[0], &e );
    zVec3DOrthonormal( &e, &n );
    zVec3DCatDRC( &v[2], zRandF(-1,1)*pow( 10, -zRandI(3,10) ), &n );
    break;
  case 2: /* collinear vertices */
    zVec3DInterDiv( &v[0], &v[1], zRandF(-0.5,1.5), &v[2] );
    break;
  case 3: /* coincident vertices */
    zVec3DCopy( &v[zRandI(0,1)], &v[2] );
    break;
  default:
    zVec3DCreate( &v[2], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  }
  zTri3DCreate( t, &v[0], &v[1], &v[2] );
}

/* a random point near a triangle, which is near a vertex or an edge in some cases. */
void create_tri3D_point_rand(const zVec3D v[], double r, zVec3D *p)
{
  zVec3D d;
  int i;

  switch( zRandI(0,2) ){
  case 0: zVec3DCreate( p, zRandF(-1.5,1.5), zRandF(-1.5,1.5), zRandF(-1.5,1.5) ); return;
  case 1: zVec3DCopy( &v[zRandI(0,2)], p ); break;
  default: i = zRandI(0,2); zVec3DInterDiv( &v[i], &v[(i+1)%3], zRandF(0,1), p );
  }
  zVec3DCreate( &d, zRandF(-r,r), zRandF(-r,r), zRandF(-r,r) );
  zVec3DAddDRC( p, &d );
}

void assert_tri3D_point_is_inside_dist(void)
{
  zVec3D v[3], p;
  zTri3D t;
  double margin, d;
  int i, j;
  bool result = true;

  for( i=0; i<N; i++ ){
    create_tri3D_rand_type( v, &t, i % 4 );
    for( j=0; j<N; j++ ){
      switch( zRandI(0,3) ){
      case 0:  margin = 0; break;
      case 1:  margin = zRandF(-1,1) * pow( 10, -zRandI(1,12) ); break;
      default: margin = zRandF(-0.3,0.3);
      }
      create_tri3D_point_rand( v, 2*fabs(margin), &p );
      d = zTri3DDistFromPoint( &t, &p );
      if( zTri3DPointIsInside( &t, &p, margin ) != ( d < margin ) && fabs( d - margin ) > 1.0e-12 ){
        eprintf( "type=%d, distance=%.16g, margin=%.16g\n", i % 4, d, margin );
        result = false;
      }
    }
  }
  zAssert( zTri3DPointIsInside (consistency with zTri3DDistFromPoint), result );
}

void assert_tri3D_closest_thin(void)
{
  zTri3D tri;
  zVec3D p[3], w[3], c, n, p_test, cp, cp_test;
  double s[3], dist, dist_test;
  int i;
  const int n_test = 1000;
  bool result1 = true, result2 = true;

  for( i=0; i<n_test; i++ ){
    /* a thin triangle whose height is 1e-3 times the longest edge */
    zVec3DCreate( &p[0], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DCreate( &p[1], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DMid( &p[0], &p[1], &c );
    zVec3DCreate( &n, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DSub( &p[1], &p[0], &w[0] );
    zVec3DOuterProd( &w[0], &n, &w[1] );
    zVec3DNormalizeDRC( &w[1], 0 );
    zVec3DCat( &c, 1.0e-3*zVec3DNorm(&w[0]), &w[1], &p[2] );
    if( !zTri3DCreate( &tri, &p[0], &p[1], &p[2] ) ) continue;
    /* a point distant from the plane, of which the projection is the barycenter */
    zTri3DBarycenter( &tri, &cp );
    zVec3DCat( &cp, zRandF(-10,10), zTri3DNorm(&tri), &p_test );
    zTri3DClosest( &tri, &p_test, &cp_test );
    if( !zVec3DEqual( &cp_test, &cp ) ){
      eprintf( "(thin case #%d) error = %.10g\n", i, zVec3DDist( &cp_test, &cp ) );
      result1 = false;
    }
    /* equivalence of zTri3DClosest for the origin and zTri3DClosestFromOrigin */
    zVec3DCreate( &p_test, zRandF(-2,2), zRandF(-2,2), zRandF(-2,2) );
    zVec3DSub( &p[0], &p_test, &w[0] );
    zVec3DSub( &p[1], &p_test, &w[1] );
    zVec3DSub( &p[2], &p_test, &w[2] );
    dist = zTri3DClosestFromOrigin( &w[0], &w[1], &w[2], &s[0], &s[1], &s[2], &cp );
    zTri3DCreate( &tri, &w[0], &w[1], &w[2] );
    dist_test = zTri3DClosest( &tri, ZVEC3DZERO, &cp_test );
    if( !zVec3DMatch( &cp_test, &cp ) || dist_test != dist ) result2 = false;
  }
  zAssert( zTri3DClosest (thin triangle case), result1 );
  zAssert( zTri3DClosest + zTri3DClosestFromOrigin (equivalence), result2 );
}

void assert_tri3D_closest_trivial(void)
{
  zVec3D p[3], p_test, cp, cp_test;
  zTri3D tri;

  zVec3DCreate( &p[0],-1,-1, 100 );
  zVec3DCreate( &p[1], 1,-1, 100 );
  zVec3DCreate( &p[2], 0, 1, 100 );
  zTri3DCreate( &tri, &p[0], &p[1], &p[2] );
  zVec3DCreate( &p_test,-2,-2, 0 );
  zTri3DClosest( &tri, &p_test, &cp_test );
  zVec3DCreate( &cp,-1,-1, 100 );
  zAssert( zTri3DClosest (trivial case 1), zVec3DEqual( &cp_test, &cp ) );
  zVec3DCreate( &p_test, 0,-2, 0 );
  zTri3DClosest( &tri, &p_test, &cp_test );
  zVec3DCreate( &cp, 0,-1, 100 );
  zAssert( zTri3DClosest (trivial case 2), zVec3DEqual( &cp_test, &cp ) );
  zVec3DCreate( &p_test, 2,-2, 0 );
  zTri3DClosest( &tri, &p_test, &cp_test );
  zVec3DCreate( &cp, 1,-1, 100 );
  zAssert( zTri3DClosest (trivial case 3), zVec3DEqual( &cp_test, &cp ) );
  zVec3DCreate( &p_test, 0, 2, 0 );
  zTri3DClosest( &tri, &p_test, &cp_test );
  zVec3DCreate( &cp, 0, 1, 100 );
  zAssert( zTri3DClosest (trivial case 4), zVec3DEqual( &cp_test, &cp ) );
  zVec3DCreate( &p_test, 0, 0, 0 );
  zTri3DClosest( &tri, &p_test, &cp_test );
  zVec3DCreate( &cp, 0, 0, 100 );
  zAssert( zTri3DClosest (trivial case 5), zVec3DEqual( &cp_test, &cp ) );
}

void assert_tri3D_closest(void)
{
  zTri3D tri;
  zVec3D p[3], e[3], ov[3], p_test, cp, cp_test, test_point;
  double dist;
  bool result1, result2, result3;
  int i, j;
  const int n = 1000;

  result1 = result2 = result3 = true;
  for( i=0; i<n; i++ ){
    create_tri3D_rand_minmax( p, &tri, -1, 1 );
    /* edge vectors & outward vectors */
    zVec3DSub( &p[2], &p[1], &e[0] ); zVec3DOuterProd( &e[0], zTri3DNorm(&tri), &ov[0] );
    zVec3DSub( &p[0], &p[2], &e[1] ); zVec3DOuterProd( &e[1], zTri3DNorm(&tri), &ov[1] );
    zVec3DSub( &p[1], &p[0], &e[2] ); zVec3DOuterProd( &e[2], zTri3DNorm(&tri), &ov[2] );
    j = zRandI( 0, 2 );
    /* edge case */
    zVec3DMid( &p[j], &p[(j+1)%3], &cp );
    zVec3DAdd( &cp, &ov[(j+2)%3], &p_test );
    zVec3DCatDRC( &p_test, zRandF(-1,1), zTri3DNorm(&tri) );
    zTri3DClosest( &tri, &p_test, &cp_test );
    if( !zVec3DEqual( &cp_test, &cp ) ){
      eprintf( "(edge case #%d) error = %.10g\n", i, zVec3DDist( &cp_test, &cp ) );
      result1 = false;
    }
    /* vertex case */
    zVec3DAdd( &p[j], &ov[(j+2)%3], &p_test );
    zVec3DAddDRC( &p_test, &ov[(j+1)%3] );
    zVec3DCatDRC( &p_test, zRandF(-1,1), zTri3DNorm(&tri) );
    zTri3DClosest( &tri, &p_test, &cp_test );
    if( !zVec3DEqual( &cp_test, &p[j] ) ){
      eprintf( "(vertex case #%d) error = %.10g\n", i, zVec3DDist( &cp_test, &p[j] ) );
      result2 = false;
    }
    /* inside case */
    zTri3DBarycenter( &tri, &cp );
    zVec3DCat( &cp, zRandF(-10,10), zTri3DNorm(&tri), &p_test );
    zTri3DClosest( &tri, &p_test, &cp_test );
    if( !zVec3DEqual( &cp_test, &cp ) ){
      eprintf( "(inside case #%d) error = %.10g\n", i, zVec3DDist( &cp_test, &cp ) );
      result3 = false;
    }
  }
  zAssert( zTri3DClosest (edge case), result1 );
  zAssert( zTri3DClosest (vertex case), result2 );
  zAssert( zTri3DClosest (inside case), result3 );

  for( i=0; i<3; i++ )
    zVec3DCreate( &p[i], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zTri3DCreate( &tri, &p[0], &p[1], &p[2] );

  result1 = true;
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p_test, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    dist = zTri3DSignedClosest( &tri, &p_test, &cp_test );
    for( j=0; j<3; j++ ){
      zVec3DInterDiv( &cp_test, zTri3DVert(&tri,j), 0.1, &test_point );
      if( zVec3DDist( &p_test, &test_point ) < dist ) result1 = false;
    }
  }
  zAssert( zTri3DClosest (general case), result1 );
}

#define TOL (1.0e-9)

void assert_tri3D_center(void)
{
  zVec3D v[3], c, h[2], tmp;
  zTri3D t;
  zEdge3D e;
  int i, j;
  double r;
  bool ret;

  /* barycenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri3D_rand_minmax( v, &t, -5, 5 );
    zTri3DBarycenter( &t, &c );
    for( j=0; j<3; j++ ){
      zVec3DSub( &c, &v[j], &tmp );
      zVec3DMulDRC( &tmp, 1.5 );
      zVec3DSub( &v[(j+1)%3], &v[j], &h[0] );
      zVec3DSubDRC( &h[0], &tmp );
      zVec3DSub( &v[(j+2)%3], &v[j], &h[1] );
      zVec3DSubDRC( &h[1], &tmp );
      zVec3DOuterProd( &h[0], &h[1], &tmp );
      if( !zVec3DIsTol( &tmp, TOL ) ){
        zVec3DFPrint( stderr, &tmp );
        ret = false;
      }
    }
  }
  zAssert( zTri3DBarycenter, ret );
  /* circumcenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri3D_rand_minmax( v, &t, -5, 5 );
    zTri3DCircumcenter( &t, &c );
    r = zVec3DDist( &v[0], &c );
    if( !zIsTol( zVec3DDist( &v[1], &c ) - r, TOL*r ) || !zIsTol( zVec3DDist( &v[2], &c ) - r, TOL*r ) ){
      eprintf( "%.10g, %.10g\n", zVec3DDist( &v[1], &c ) - r, zVec3DDist( &v[2], &c ) - r );
      ret = false;
    }
  }
  zAssert( zTri3DCircumcenter, ret );
  /* incenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri3D_rand_minmax( v, &t, -5, 5 );
    zTri3DIncenter( &t, &c );
    r = zEdge3DDistFromPoint( zEdge3DCreate( &e, &v[0], &v[1] ), &c );
    if( !zIsTol( zEdge3DDistFromPoint( zEdge3DCreate( &e, &v[1], &v[2] ), &c ) - r, TOL ) ||
        !zIsTol( zEdge3DDistFromPoint( zEdge3DCreate( &e, &v[2], &v[0] ), &c ) - r, TOL ) ){
      ret = false;
    }
  }
  zAssert( zTri3DIncenter, ret );
  /* orthocenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri3D_rand_minmax( v, &t, -5, 5 );
    zTri3DOrthocenter( &t, &c );
    for( j=0; j<3; j++ ){
      zVec3DSub( &c, &v[j], &tmp );
      zVec3DSub( &v[(j+2)%3], &v[(j+1)%3], &h[0] );
      if( !zIsTol( zVec3DInnerProd( &tmp, &h[0] ), TOL*zVec3DNorm(&tmp)*zVec3DNorm(&h[0]) ) ){
        eprintf( "%.10g\n", zVec3DInnerProd( &tmp, &h[0] ) );
        ret = false;
      }
    }
  }
  zAssert( zTri3DOrthocenter, ret );
}

/* check a center of a triangle with the expected point relatively to the scale. */
bool check_tri3D_center(const zVec3D *c, const zVec3D *c_expected, double scale)
{
  return c && zVec3DDist( c, c_expected ) <= TOL * scale;
}

void assert_tri3D_center_thin(void)
{
  zVec3D v[3], o, c, c_expected;
  zTri3D t;
  double h, hs[] = { 1.0e-1, 1.0e-2, 1.0e-3, 1.0e-4, 1.0e-5, 1.0e-6 };
  double offset[] = { 0, 1.0e3 };
  int i, j, k;
  bool result1 = true, result2 = true;

  for( i=0; i<sizeof(offset)/sizeof(double); i++ ){
    zVec3DCreate( &o, offset[i], -offset[i], 0.5*offset[i] );
    for( j=0; j<sizeof(hs)/sizeof(double); j++ ){
      /* isosceles triangle with the base from -1 to 1 along x-axis and the apex at the height h */
      zVec3DCreate( &v[0], o.c.x-1, o.c.y, o.c.z );
      zVec3DCreate( &v[1], o.c.x+1, o.c.y, o.c.z );
      zVec3DCreate( &v[2], o.c.x, o.c.y+hs[j], o.c.z );
      h = v[2].c.y - o.c.y; /* the actual height after rounding */
      for( k=0; k<3; k++ ){ /* every order of vertices */
        zTri3DCreate( &t, &v[k], &v[(k+1)%3], &v[(k+2)%3] );
        zVec3DCreate( &c_expected, o.c.x, o.c.y+0.5*(h*h-1)/h, o.c.z );
        if( !check_tri3D_center( zTri3DCircumcenter( &t, &c ), &c_expected, zVec3DDist( &c_expected, &v[0] ) ) ){
          eprintf( "(offset=%g, h=%g) circumcenter error = %g\n", offset[i], hs[j], zVec3DDist( &c, &c_expected ) );
          result1 = false;
        }
        zVec3DCreate( &c_expected, o.c.x, o.c.y+1/h, o.c.z );
        if( !check_tri3D_center( zTri3DOrthocenter( &t, &c ), &c_expected, zVec3DDist( &c_expected, &v[0] ) ) ){
          eprintf( "(offset=%g, h=%g) orthocenter error = %g\n", offset[i], hs[j], zVec3DDist( &c, &c_expected ) );
          result2 = false;
        }
      }
    }
  }
  zAssert( zTri3DCircumcenter (thin triangle case), result1 );
  zAssert( zTri3DOrthocenter (thin triangle case), result2 );
}

void assert_tri3D_center_special(void)
{
  zVec3D v[3], c, c_expected;
  zTri3D t;
  bool result1 = true, result2 = true;

  /* right triangle */
  zVec3DCreate( &v[0], 1, 2, 3 );
  zVec3DCreate( &v[1], 4, 2, 3 );
  zVec3DCreate( &v[2], 1, 6, 3 );
  zTri3DCreate( &t, &v[0], &v[1], &v[2] );
  zVec3DMid( &v[1], &v[2], &c_expected );
  if( !check_tri3D_center( zTri3DCircumcenter( &t, &c ), &c_expected, zVec3DDist( &c_expected, &v[0] ) ) ) result1 = false;
  if( !check_tri3D_center( zTri3DOrthocenter( &t, &c ), &v[0], zVec3DDist( &v[0], &v[1] ) ) ) result1 = false;
  /* equilateral triangle */
  zVec3DCreate( &v[0], 1, 0, 0 );
  zVec3DCreate( &v[1], 0, 1, 0 );
  zVec3DCreate( &v[2], 0, 0, 1 );
  zTri3DCreate( &t, &v[0], &v[1], &v[2] );
  zTri3DBarycenter( &t, &c_expected );
  if( !check_tri3D_center( zTri3DCircumcenter( &t, &c ), &c_expected, zVec3DDist( &c_expected, &v[0] ) ) ) result1 = false;
  if( !check_tri3D_center( zTri3DOrthocenter( &t, &c ), &c_expected, zVec3DDist( &c_expected, &v[0] ) ) ) result1 = false;
  zAssert( zTri3DCircumcenter + zTri3DOrthocenter (right and equilateral triangles), result1 );
  /* degenerate triangles */
  zVec3DCreate( &v[0], 1, 2, 3 );
  zVec3DCreate( &v[1], 2, 3, 4 );
  zVec3DCreate( &v[2], 3, 4, 5 );
  zTri3DCreate( &t, &v[0], &v[1], &v[2] );
  if( zTri3DCircumcenter( &t, &c ) || zTri3DOrthocenter( &t, &c ) ) result2 = false;
  zTri3DCreate( &t, &v[0], &v[0], &v[0] );
  if( zTri3DCircumcenter( &t, &c ) || zTri3DOrthocenter( &t, &c ) ) result2 = false;
  zAssert( zTri3DCircumcenter + zTri3DOrthocenter (degenerate case), result2 );
}

void assert_tri3D_list_align(void)
{
  zVec3D *varray;
  zTri3D t;
  zTri3DList tlist;
  zTri3DListCell *tp;
  int i;
  const int n = 1000;
  bool result = true;

  varray = zAlloc( zVec3D, 3 * n );
  zListInit( &tlist );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &varray[3*i  ], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zVec3DCreate( &varray[3*i+1], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zVec3DCreate( &varray[3*i+2], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zTri3DCreate( &t, &varray[3*i], &varray[3*i+1], &varray[3*i+2] );
    zTri3DListAdd( &tlist, &t );
  }
  zTri3DListAlign( &tlist, ZVEC3DZ );
  zListForEach( &tlist, tp )
    if( zVec3DInnerProd( zTri3DNorm(&tp->data), ZVEC3DZ ) < 0 ) result = false;
  zFree( varray );
  zTri3DListDestroy( &tlist );
  zAssert( zTri3DListAlign, result );
}

void assert_tetra3D_closest_trivial(void)
{
  zVec3D v[4], point, closestpoint;

  zVec3DCreate( &v[0],-0.5*cos(zPI/6), -0.5,-1.0 );
  zVec3DCreate( &v[1], 0.5*cos(zPI/6), -0.5,-1.0 );
  zVec3DCreate( &v[2],              0,    1,-1.0 );
  zVec3DCreate( &v[3],              0,    0, 2.0 );

  /* trivial case 1 */
  zTetra3DClosest( &v[0], &v[1], &v[2], &v[3], ZVEC3DZERO, &closestpoint );
  zAssert( zTetra3DClosest (trivial case 1), zVec3DEqual( ZVEC3DZERO, &closestpoint ) );
  /* trivial case 2 */
  zVec3DCreate( &point, 0, 0, -2 );
  zTetra3DClosest( &v[0], &v[1], &v[2], &v[3], &point, &closestpoint );
  zAssert( zTetra3DClosest (trivial case 2), closestpoint.c.x == 0 && closestpoint.c.y == 0 && closestpoint.c.z == -1.0 );
  /* trivial case 3 */
  zVec3DCreate( &point, 0, -1, -1 );
  zTetra3DClosest( &v[0], &v[1], &v[2], &v[3], &point, &closestpoint );
  zAssert( zTetra3DClosest (trivial case 3), closestpoint.c.x == 0 && closestpoint.c.y == -0.5 && closestpoint.c.z == -1 );
  /* trivial case 4 */
  zVec3DCreate( &point,-2, -1, -1 );
  zTetra3DClosest( &v[0], &v[1], &v[2], &v[3], &point, &closestpoint );
  zAssert( zTetra3DClosest (trivial case 4), zVec3DEqual( &v[0], &closestpoint ) );
  /* trivial case 5 */
  zVec3DCreate( &point, 2, -1, -1 );
  zTetra3DClosest( &v[0], &v[1], &v[2], &v[3], &point, &closestpoint );
  zAssert( zTetra3DClosest (trivial case 5), zVec3DEqual( &v[1], &closestpoint ) );
  /* trivial case 6 */
  zVec3DCreate( &point, 0, 0, 3 );
  zTetra3DClosest( &v[0], &v[1], &v[2], &v[3], &point, &closestpoint );
  zAssert( zTetra3DClosest (trivial case 6), zVec3DEqual( &v[3], &closestpoint ) );
}

void assert_tetra3D_closest(void)
{
  zVec3D vert[4], point, closestpoint, test_point;
  int i, j;
  const int n = 10000;
  double dist;
  bool result = true;

  zVec3DCreate( &vert[0], zRandF(-5,0), zRandF(-5,0), zRandF(-5,0) );
  zVec3DCreate( &vert[1], zRandF( 0,5), zRandF(-5,0), zRandF(-5,0) );
  zVec3DCreate( &vert[2], zRandF(-5,5), zRandF( 0,5), zRandF(-5,5) );
  zVec3DCreate( &vert[3], zRandF(-5,5), zRandF(-5,5), zRandF( 0,5) );

  for( i=0; i<n; i++ ){
    zVec3DCreate( &point, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    dist = zTetra3DClosest( &vert[0], &vert[1], &vert[2], &vert[3], &point, &closestpoint );
    for( j=0; j<4; j++ ){
      zVec3DInterDiv( &closestpoint, &vert[j], 0.1, &test_point );
      if( zVec3DDist( &point, &test_point ) < dist ) result = false;
    }
  }
  zAssert( zTetra3DClosest, result );
}

void assert_tetra3D_closest_from_origin(void)
{
  zVec3D vert[4], closestpoint, test_point, error;
  double s[4];
  int i, j;
  const int n = 1000;
  bool result = true;

  for( i=0; i<n; i++ ){
    for( j=0; j<4; j++ )
      zVec3DCreate( &vert[j], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zTetra3DClosestFromOrigin( &vert[0], &vert[1], &vert[2], &vert[3], &s[0], &s[1], &s[2], &s[3], &closestpoint );
    zVec3DZero( &test_point );
    for( j=0; j<4; j++ )
      zVec3DCatDRC( &test_point, s[j], &vert[j] );
    if( !zVec3DEqual( &test_point, &closestpoint ) ){
      zVec3DSub( &closestpoint, &test_point, &error );
      eprintf( "error = " ); zVec3DFPrint( stderr, &error );
      result = false;
    }
  }
  zAssert( zTetra3DClosestFromOrigin, result );
}

void assert_aabox3D_create(void)
{
  zAABox3D box;
  int i;
  const int n = 10;
  bool result = true;

  for( i=0; i<n; i++ ){
    zAABox3DCreate( &box, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    if( zAABox3DXMin(&box) > zAABox3DXMax(&box) ||
        zAABox3DYMin(&box) > zAABox3DYMax(&box) ||
        zAABox3DZMin(&box) > zAABox3DZMax(&box) ) result = false;
  }
  zAssert( zAABox3DCreate, result );
}

void assert_aabox3D_equal(void)
{
  zAABox3D box1, box2;

  zAABox3DCreate( &box1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAABox3DCopy( &box1, &box2 );
  zAssert( zAABox3DCopy, zAABox3DEqual( &box1, &box2 ) );
}

void assert_aabox3D_expand(void)
{
  zAABox3D box1, box2, box3, box_test;
  zVec3D center;
  double depth, width, height;

  zVec3DCreate( &center, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  depth  = zRandF( 0.1, 10.0 );
  width  = zRandF( 0.1, 10.0 );
  height = zRandF( 0.1, 10.0 );
  zAABox3DCreateFromSize( &box1, &center, depth, width, height );
  zAABox3DExpand( &box1, 0.5, &box2 );
  zAABox3DExpand( &box1, 2.0, &box3 );
  zAssert( zAABox3DExpand,
    zAABox3DEqual( zAABox3DExpand( &box2, 2.0, &box_test ), &box1 ) &&
    zAABox3DEqual( zAABox3DExpand( &box3, 0.5, &box_test ), &box1 ) &&
    zAABox3DEqual( zAABox3DExpand( &box2, 4.0, &box_test ), &box3 ) );
  zAABox3DCopy( &box1, &box_test );
  zAssert( zAABox3DExpandDRC,
    zAABox3DEqual( zAABox3DExpandDRC( &box1, 0.5 ), &box2 ) &&
    zAABox3DEqual( zAABox3DExpandDRC( &box_test, 2.0 ), &box3 ) );
}

double aabox3D_dist_from_point_base(const zAABox3D *box, const zVec3D *point)
{
  zVec3D cp;
  return zAABox3DClosest( box, point, &cp );
}

void assert_aabox3D_dist(void)
{
  zAABox3D box;
  zVec3D point;
  double dist1, dist2;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    zAABox3DCreate( &box, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &point, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    dist1 = zAABox3DDistFromPoint( &box, &point );
    dist2 = aabox3D_dist_from_point_base( &box, &point );
    if( !zEqual( dist1, dist2, zTOL ) ) result = false;
  }
  zAssert( zAABox3DPointDist, result );
}

void assert_aabox3D_dist_range_from_plane(void)
{
  zAABox3D box;
  zPlane3D plane;
  zVec3D p, n, v;
  double dmin, dmax, d, dmin_ans, dmax_ans;
  int i, j;
  bool result = true;

  for( i=0; i<N; i++ ){
    zAABox3DCreate( &box, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &n, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    if( i % 10 == 0 ) n.e[i/10%3] = 0; /* a plane parallel to an axis */
    if( !zPlane3DCreate( &plane, &p, &n ) ) continue;
    zAABox3DDistRangeFromPlane( &box, &plane, &dmin, &dmax );
    dmin_ans = HUGE_VAL;
    dmax_ans = -HUGE_VAL;
    for( j=0; j<8; j++ ){
      d = zPlane3DDistFromPoint( &plane, zAABox3DVert( &box, j, &v ) );
      if( d < dmin_ans ) dmin_ans = d;
      if( d > dmax_ans ) dmax_ans = d;
    }
    if( !zEqual( dmin, dmin_ans, zTOL ) || !zEqual( dmax, dmax_ans, zTOL ) ) result = false;
  }
  zAssert( zAABox3DDistRangeFromPlane, result );
}

void assert_aabox3D_face_to_plane3D6(void)
{
  zAABox3D box;
  zPlane3D plane[6];
  zVec3D v, c;
  int i, j, k, side, on;
  bool result = true;

  for( i=0; i<N; i++ ){
    zAABox3DCreate( &box, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    if( zAABox3DFaceToPlane3D6( &box, plane ) != plane ) result = false;
    zAABox3DCenter( &box, &c );
    for( k=0; k<3; k++ )
      for( side=0; side<2; side++ ){
        if( zPlane3DNorm(&plane[2*k+side])->e[k] != ( side == 0 ? -1 : 1 ) ||
            zPlane3DNorm(&plane[2*k+side])->e[(k+1)%3] != 0 ||
            zPlane3DNorm(&plane[2*k+side])->e[(k+2)%3] != 0 ) result = false;
        /* the center is behind the plane */
        if( !zEqual( zPlane3DDistFromPoint( &plane[2*k+side], &c ), -0.5*( box.max.e[k] - box.min.e[k] ), zTOL ) ) result = false;
        /* four vertices are on the plane, and the others are behind it */
        for( on=0, j=0; j<8; j++ ){
          zAABox3DVert( &box, j, &v );
          if( zIsTiny( zPlane3DDistFromPoint( &plane[2*k+side], &v ) ) ) on++;
          else if( zPlane3DDistFromPoint( &plane[2*k+side], &v ) > 0 ) result = false;
        }
        if( on != 4 ) result = false;
      }
  }
  zAssert( zAABox3DFaceToPlane3D6, result );
}

bool check_tri3D_linscale(const zTri3D *tri, const zVec3D *p, const zVec3D *cp_ans, double d_ans)
{
  double l0, l1, l2, d;
  zVec3D cp, v;

  d = zTri3DLinScale( tri, p, &l0, &l1, &l2, &cp );
  zVec3DMul( zTri3DVert(tri,0), l0, &v );
  zVec3DCatDRC( &v, l1, zTri3DVert(tri,1) );
  zVec3DCatDRC( &v, l2, zTri3DVert(tri,2) );
  return zEqual( d, d_ans, zTOL ) && zVec3DEqual( &cp, cp_ans ) && zVec3DEqual( &v, cp_ans ) &&
         l0 >= 0 && l1 >= 0 && l2 >= 0 && zEqual( l0+l1+l2, 1, zTOL );
}

void assert_tri3D_linscale_degenerate(void)
{
  zTri3D tri;
  zVec3D v[3], p, cp;
  bool result = true;

  /* collinear vertices */
  zVec3DCreate( &v[0], 0, 0, 0 );
  zVec3DCreate( &v[1], 1, 0, 0 );
  zVec3DCreate( &v[2], 2, 0, 0 );
  zTri3DCreate( &tri, &v[0], &v[1], &v[2] );
  zVec3DCreate( &p, 1.5, 1, 0 );
  zVec3DCreate( &cp, 1.5, 0, 0 );
  if( !check_tri3D_linscale( &tri, &p, &cp, 1 ) ) result = false;
  zVec3DCreate( &p, 3, 1, 0 );
  if( !check_tri3D_linscale( &tri, &p, &v[2], sqrt(2) ) ) result = false;
  /* two coincident vertices */
  zTri3DCreate( &tri, &v[0], &v[2], &v[2] );
  zVec3DCreate( &p, 0.5, 0, 1 );
  zVec3DCreate( &cp, 0.5, 0, 0 );
  if( !check_tri3D_linscale( &tri, &p, &cp, 1 ) ) result = false;
  /* three coincident vertices */
  zTri3DCreate( &tri, &v[1], &v[1], &v[1] );
  zVec3DCreate( &p, 1, 2, 0 );
  if( !check_tri3D_linscale( &tri, &p, &v[1], 2 ) ) result = false;
  zAssert( zTri3DLinScale (degenerate case), result );
}

/* a random simple polygon on a random plane, which is star-shaped around its center and might have
 * collinear vertices. u and w are the orthonormal basis of the plane. */
static void triangulate_polygon_rand(zVec3D v[], int n, zVec3D *u, zVec3D *w, zVec3D *nv)
{
  zVec3D c;
  double th, r;
  int i;

  zVec3DCreate( nv, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DNormalizeDRC( nv, 0 );
  zVec3DOrthonormal( nv, u );
  zVec3DOuterProd( nv, u, w );
  zVec3DCreate( &c, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  for( i=0; i<n; i++ ){
    th = zPIx2 * ( i + zRandF(-0.3,0.3) ) / n;
    r = zRandF(0.2,1);
    zVec3DCopy( &c, &v[i] );
    zVec3DCatDRC( &v[i], r*cos(th), u );
    zVec3DCatDRC( &v[i], r*sin(th), w );
  }
  if( n >= 6 ) zVec3DInterDiv( &v[0], &v[2], 0.5, &v[1] ); /* collinear vertices */
}

/* projection of a point onto the plane of a polygon. */
static void triangulate_proj(const zVec3D *p, const zVec3D *u, const zVec3D *w, double q[2])
{
  q[0] = zVec3DInnerProd( p, u );
  q[1] = zVec3DInnerProd( p, w );
}

/* check if a point d is strictly inside of the circumcircle of a counterclockwise triangle abc
 * beyond a tolerance. */
static bool triangulate_incircle(const double a[2], const double b[2], const double c[2], const double d[2])
{
  double ax = a[0]-d[0], ay = a[1]-d[1], bx = b[0]-d[0], by = b[1]-d[1], cx = c[0]-d[0], cy = c[1]-d[1];
  double det = ( ax*ax + ay*ay ) * ( bx*cy - by*cx ) + ( bx*bx + by*by ) * ( cx*ay - cy*ax ) + ( cx*cx + cy*cy ) * ( ax*by - ay*bx );
  return det > 1.0e-9;
}

/* the triangulation of a simple polygon consists of n-2 triangles oriented in the same direction with
 * the polygon, the total area of which is that of the polygon, and every diagonal of which satisfies
 * the Delaunay condition (constrained Delaunay triangulation). */
void assert_triangulate(void)
{
  zVec3D v[40], u, w, nv, sum, d;
  zVec3DArray array;
  zLoop3D loop;
  zTri3DList tlist, tlist2;
  zTri3DListCell *tp, *sp;
  double area, q[4][2];
  int i, j, k, l, n, nt;
  bool result_num = true, result_area = true, result_delaunay = true, result_loop = true;

  for( i=0; i<N/10; i++ ){
    n = zRandI(3,40);
    triangulate_polygon_rand( v, n, &u, &w, &nv );
    zArrayAssign( &array, n, v );
    if( ( nt = zTriangulate( &array, &tlist ) ) != n - 2 ) result_num = false;
    /* area */
    for( zVec3DZero( &sum ), j=0; j<n; j++ ){
      zVec3DOuterProd( &v[j], &v[(j+1)%n], &d );
      zVec3DAddDRC( &sum, &d );
    }
    area = 0;
    zListForEach( &tlist, tp ){
      if( zVec3DInnerProd( zTri3DNorm(&tp->data), &nv ) <= 0 ) result_area = false;
      area += zTri3DArea( &tp->data );
    }
    if( !zEqual( area, 0.5*zVec3DNorm(&sum), 1.0e-10 ) ) result_area = false;
    /* Delaunay condition across every diagonal */
    zListForEach( &tlist, tp )
      for( k=0; k<3; k++ )
        zListForEach( &tlist, sp ){
          if( sp == tp ) continue;
          for( l=0; l<3; l++ )
            if( zTri3DVert(&sp->data,l) == zTri3DVert(&tp->data,(k+1)%3) &&
                zTri3DVert(&sp->data,(l+1)%3) == zTri3DVert(&tp->data,k) ) break;
          if( l == 3 ) continue;
          triangulate_proj( zTri3DVert(&tp->data,0), &u, &w, q[0] );
          triangulate_proj( zTri3DVert(&tp->data,1), &u, &w, q[1] );
          triangulate_proj( zTri3DVert(&tp->data,2), &u, &w, q[2] );
          triangulate_proj( zTri3DVert(&sp->data,(l+2)%3), &u, &w, q[3] );
          if( triangulate_incircle( q[0], q[1], q[2], q[3] ) ) result_delaunay = false;
        }
    /* zLoop3DTriangulate() gives the same number of triangles */
    zLoop3DCreate( &loop, &array );
    if( zLoop3DTriangulate( &loop, &tlist2 ) != nt ) result_loop = false;
    zTri3DListDestroy( &tlist2 );
    zLoop3DDestroy( &loop );
    zTri3DListDestroy( &tlist );
  }
  zAssert( zTriangulate (number of triangles), result_num );
  zAssert( zTriangulate (orientation and area), result_area );
  zAssert( zTriangulate (Delaunay condition), result_delaunay );
  zAssert( zLoop3DTriangulate, result_loop );
}

int main(void)
{
  zRandInit();
  assert_line3D_point();
  assert_line3D_commonperp();
  assert_line3D_distance();
  assert_edge3D();
  assert_edge3D_closest();
  assert_edge3D_contig();
  assert_plane3D_create();
  assert_plane3D_create_fail();
  assert_plane3D_proj_point();
  assert_tri3D_contig();
  assert_tri3D_proj_point();
  assert_tri3D_point_is_inside();
  assert_tri3D_point_is_inside_dist();
  assert_tri3D_closest_trivial();
  assert_tri3D_closest();
  assert_tri3D_closest_thin();
  assert_tri3D_linscale_degenerate();
  assert_tri3D_center();
  assert_tri3D_center_thin();
  assert_tri3D_center_special();
  assert_tri3D_list_align();
  assert_tetra3D_closest_trivial();
  assert_tetra3D_closest();
  assert_tetra3D_closest_from_origin();
  assert_aabox3D_create();
  assert_aabox3D_equal();
  assert_aabox3D_expand();
  assert_aabox3D_dist();
  assert_aabox3D_dist_range_from_plane();
  assert_aabox3D_face_to_plane3D6();
  assert_tri3D_calc_norm();
  assert_tri3D_is_degenerate();
  assert_triangulate();
  return 0;
}
