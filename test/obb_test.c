#include <zeo/zeo.h>

#define N 500

/* check if the axes of a box are right-handed orthonormal vectors. */
bool check_axes(zBox3D *box)
{
  zVec3D z;

  if( !zIsTiny( zVec3DNorm( zBox3DAxis(box,zX) ) - 1 ) ||
      !zIsTiny( zVec3DNorm( zBox3DAxis(box,zY) ) - 1 ) ||
      !zIsTiny( zVec3DInnerProd( zBox3DAxis(box,zX), zBox3DAxis(box,zY) ) ) ) return false;
  zVec3DOuterProd( zBox3DAxis(box,zX), zBox3DAxis(box,zY), &z );
  return zVec3DEqual( &z, zBox3DAxis(box,zZ) );
}

/* check if all points are inside of a box within a tolerance. */
bool check_inside(zBox3D *box, zVec3DData *data, double tol)
{
  zVec3D *v, d;
  int k;

  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zVec3DSub( v, zBox3DCenter(box), &d );
    for( k=zX; k<=zZ; k++ )
      if( fabs( zVec3DInnerProd( &d, zBox3DAxis(box,k) ) ) > 0.5 * zBox3DSpan(box,k) + tol ) return false;
  }
  return true;
}

/* random frame */
void create_frame(zFrame3D *f, double offset)
{
  zFrame3DFromPosAA( f, offset*zRandF(-1,1), offset*zRandF(-1,1), offset*zRandF(-1,1),
    zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

/* points in a box of half-sizes a, b and c including the corners */
void assert_obb_box(void)
{
  zVec3DData data;
  zFrame3D f;
  zBox3D obb;
  zVec3D p;
  double s[3], so[3], scale;
  int i, k, testnum = 100;
  bool result = true;

  for( k=0; k<testnum; k++ ){
    scale = pow( 10, zRandF(-3,3) );
    create_frame( &f, 10*scale );
    s[0] = scale*zRandF(0.1,1); s[1] = scale*zRandF(1.1,2); s[2] = scale*zRandF(2.1,3); /* distinct sizes */
    zVec3DDataInitArray( &data, N + 8 );
    for( i=0; i<N+8; i++ ){
      if( i < 8 ) zVec3DCreate( &p, i&1 ? s[0] : -s[0], i&2 ? s[1] : -s[1], i&4 ? s[2] : -s[2] );
      else zVec3DCreate( &p, zRandF(-s[0],s[0]), zRandF(-s[1],s[1]), zRandF(-s[2],s[2]) );
      zXform3DDRC( &f, &p );
      zVec3DDataAdd( &data, &p );
    }
    if( !zVec3DDataOBB( &data, &obb ) || !check_axes( &obb ) || !check_inside( &obb, &data, zTOL*scale ) ){
      result = false;
    } else{
      for( i=0; i<3; i++ ) so[i] = 0.5 * zBox3DSpan(&obb,i);
      zDataSort( so, 3 );
      for( i=0; i<3; i++ )
        if( !zIsTol( so[i] - s[i], zTOL*scale ) ) result = false;
    }
    zVec3DDataDestroy( &data );
  }
  zAssert( zVec3DDataOBB (box), result );
}

/* area of the minimum rectangle enclosing a convex polygon, found by brute force */
double min_rect_area(zPG2D *pg)
{
  zVec2D e, n, d;
  double smin, smax, tmin, tmax, s, t, area, area_min = HUGE_VAL;
  int i, j, nv;

  nv = zPG2DVertNum(pg);
  for( i=0; i<nv; i++ ){
    zVec2DSub( zPG2DVert(pg,(i+1)%nv), zPG2DVert(pg,i), &e );
    zVec2DNormalizeDRC( &e, 0 );
    zVec2DCreate( &n, -e.c.y, e.c.x );
    smin = tmin = HUGE_VAL; smax = tmax = -HUGE_VAL;
    for( j=0; j<nv; j++ ){
      zVec2DSub( zPG2DVert(pg,j), zPG2DVert(pg,0), &d );
      s = zVec2DInnerProd( &d, &e ); t = zVec2DInnerProd( &d, &n );
      smin = zMin( smin, s ); smax = zMax( smax, s );
      tmin = zMin( tmin, t ); tmax = zMax( tmax, t );
    }
    if( ( area = ( smax - smin ) * ( tmax - tmin ) ) < area_min ) area_min = area;
  }
  return area_min;
}

/* random points on a plane, which are exactly on a plane or rotated with rounding errors */
void assert_obb_planar(void)
{
  zVec3DData data, data_rot;
  zVec2DData data2d;
  zFrame3D f;
  zBox3D obb;
  zPG2D pg;
  zVec3D p;
  zVec2D q;
  int i, k, testnum = 100;
  bool result_exact = true, result_rect = true, result_rotated = true;

  for( k=0; k<testnum; k++ ){
    zVec3DDataInitArray( &data, N );
    zVec3DDataInitArray( &data_rot, N );
    zVec2DDataInitArray( &data2d, N );
    create_frame( &f, 10 );
    for( i=0; i<N; i++ ){
      if( k % 2 == 0 ) zVec2DCreate( &q, zRandF(-2,2), zRandF(-1,1) );
      else zVec2DCreate( &q, i%20, (i/20)%20 ); /* grid */
      zVec2DDataAdd( &data2d, &q );
      zVec3DDataAdd( &data, zVec3DCreate( &p, q.c.x, q.c.y, 0 ) );
      zVec3DDataAdd( &data_rot, zXform3DDRC( &f, &p ) );
    }
    if( !zVec3DDataOBB( &data, &obb ) || !check_axes( &obb ) || !check_inside( &obb, &data, zTOL ) ||
        zBox3DHeight(&obb) != 0 ) result_exact = false;
    else{
      zVec2DDataConvexHull( &data2d, &pg );
      if( !zIsTol( zBox3DDepth(&obb) * zBox3DWidth(&obb) - min_rect_area( &pg ), zTOL ) ) result_rect = false;
      zPG2DDestroy( &pg );
    }
    /* rotated points, which are not coplanar due to rounding errors */
    if( !zVec3DDataOBB( &data_rot, &obb ) || !check_axes( &obb ) || !check_inside( &obb, &data_rot, zTOL ) ||
        zBox3DHeight(&obb) > zTOL ) result_rotated = false;
    zVec3DDataDestroy( &data );
    zVec3DDataDestroy( &data_rot );
    zVec2DDataDestroy( &data2d );
  }
  zAssert( zVec3DDataOBB (coplanar points), result_exact );
  zAssert( zVec3DDataOBB (minimum rectangle), result_rect );
  zAssert( zVec3DDataOBB (nearly coplanar points), result_rotated );
}

/* random points in a cube and on a sphere */
void assert_obb_random(void)
{
  zVec3DData data;
  zBox3D obb;
  zVec3D p;
  int i, k, testnum = 100;
  bool result = true;

  for( k=0; k<testnum; k++ ){
    zVec3DDataInitArray( &data, N );
    for( i=0; i<N; i++ ){
      zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
      if( k % 2 ) zVec3DNormalizeDRC( &p, 0 );
      zVec3DDataAdd( &data, &p );
    }
    if( !zVec3DDataOBB( &data, &obb ) || !check_axes( &obb ) || !check_inside( &obb, &data, zTOL ) ) result = false;
    zVec3DDataDestroy( &data );
  }
  zAssert( zVec3DDataOBB (random points), result );
}

/* degenerate cases */
void assert_obb_degenerate(void)
{
  zVec3DData data;
  zBox3D obb;
  zVec3D p;
  int i;
  bool result_point, result_line;

  zEchoOff();
  zVec3DDataInitArray( &data, 10 );
  for( i=0; i<10; i++ ) zVec3DDataAdd( &data, zVec3DCreate( &p, 1, 2, 3 ) );
  result_point = zVec3DDataOBB( &data, &obb ) == NULL;
  zVec3DDataDestroy( &data );
  zVec3DDataInitArray( &data, 10 );
  for( i=0; i<10; i++ ) zVec3DDataAdd( &data, zVec3DCreate( &p, i, 2*i, 3*i ) );
  result_line = zVec3DDataOBB( &data, &obb ) == NULL;
  zVec3DDataDestroy( &data );
  zEchoOn();
  zAssert( zVec3DDataOBB (coincident points), result_point );
  zAssert( zVec3DDataOBB (collinear points), result_line );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_obb_box();
  assert_obb_planar();
  assert_obb_random();
  assert_obb_degenerate();
  return EXIT_SUCCESS;
}
