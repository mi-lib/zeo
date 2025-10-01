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
  zVec3DNormalizeDRC( &n );
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

void assert_tri3D_point_is_inside(void)
{
  zVec3D v[3], p;
  zTri3D t;
  int i;
  double r1, r2;
  bool result1, result2, result3, result4;

  result1 = result2 = result3 = result4 = true;
  for( i=0; i<N; i++ ){
    create_tri3D_rand( v, &t );
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
    if( !zTri3DPointIsInside( &t, &p, zTOL ) ) result3 = false;
    /* vertex case */
    zVec3DCopy( &v[2], &p );
    if( !zTri3DPointIsInside( &t, &p, zTOL ) ) result4 = false;
  }
  zAssert( zTri3DPointIsInside (true positive), result1 );
  zAssert( zTri3DPointIsInside (false positive), result2 );
  zAssert( zTri3DPointIsInside (midpoint case), result3 );
  zAssert( zTri3DPointIsInside (vertex case), result4 );
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
    if( !zIsTol( zVec3DDist( &v[1], &c ) - r, TOL ) || !zIsTol( zVec3DDist( &v[2], &c ) - r, TOL ) ){
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
      if( !zIsTol( zVec3DInnerProd( &tmp, &h[0] ), TOL ) ){
        eprintf( "%.10g\n", zVec3DInnerProd( &tmp, &h[0] ) );
        ret = false;
      }
    }
  }
  zAssert( zTri3DOrthocenter, ret );
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

void assert_aabox_create(void)
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


void assert_aabox_equal(void)
{
  zAABox3D box1, box2;

  zAABox3DCreate( &box1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAABox3DCopy( &box1, &box2 );
  zAssert( zAABox3DCopy, zAABox3DEqual( &box1, &box2 ) );
}

void assert_aabox_expand(void)
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

double aabox_dist_from_point_base(const zAABox3D *box, const zVec3D *point)
{
  zVec3D cp;
  return zAABox3DClosest( box, point, &cp );
}

void assert_aabox_dist(void)
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
    dist2 = aabox_dist_from_point_base( &box, &point );
    if( !zEqual( dist1, dist2, zTOL ) ) result = false;
  }
  zAssert( zAABox3DPointDist, result );
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
  assert_plane3D_proj_point();
  assert_tri3D_contig();
  assert_tri3D_proj_point();
  assert_tri3D_point_is_inside();
  assert_tri3D_closest_trivial();
  assert_tri3D_closest();
  assert_tri3D_center();
  assert_tri3D_list_align();
  assert_tetra3D_closest_trivial();
  assert_tetra3D_closest();
  assert_tetra3D_closest_from_origin();
  assert_aabox_create();
  assert_aabox_equal();
  assert_aabox_expand();
  assert_aabox_dist();
  return 0;
}
