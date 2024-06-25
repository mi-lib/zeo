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
    zIsEqual( zVec2DNorm( &dp ), zLine2DDistFromPoint( &line, &p_test ), zTOL ) );
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
  zAssert( zEdge2DLen, zIsEqual( zEdge2DLen(&edge), r, zTOL ) );

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

void create_tri2D_rand(zVec2D v[], zTri2D *t, double min, double max)
{
  zVec2DCreate( &v[0], zRandF(min,max), zRandF(min,max) );
  zVec2DCreate( &v[1], zRandF(min,max), zRandF(min,max) );
  zVec2DCreate( &v[2], zRandF(min,max), zRandF(min,max) );
  zTri2DCreate( t, &v[0], &v[1], &v[2] );
}

#define TOL (1.0e-5)

void assert_tri2D_center(void)
{
  zVec2D v[3], c, h[2], tmp;
  zTri2D t;
  zEdge2D e;
  register int i, j;
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
  assert_tri2D_center();
  assert_ellips2D_inside();
  return 0;
}
