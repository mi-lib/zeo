#include <zeo/zeo.h>

#define N 1000

void create_box_points(zVec3D p[], int n, zAABox3D *box)
{
  int i;

  for( i=0; i<n-8; i++ )
    zVec3DCreate( &p[i], zRandF(box->min.c.x,box->max.c.x), zRandF(box->min.c.y,box->max.c.y), zRandF(box->min.c.z,box->max.c.z) );
  zVec3DCreate( &p[n-8], box->min.c.x, box->min.c.y, box->min.c.z );
  zVec3DCreate( &p[n-7], box->max.c.x, box->min.c.y, box->min.c.z );
  zVec3DCreate( &p[n-6], box->max.c.x, box->max.c.y, box->min.c.z );
  zVec3DCreate( &p[n-5], box->min.c.x, box->max.c.y, box->min.c.z );
  zVec3DCreate( &p[n-4], box->min.c.x, box->min.c.y, box->max.c.z );
  zVec3DCreate( &p[n-3], box->max.c.x, box->min.c.y, box->max.c.z );
  zVec3DCreate( &p[n-2], box->max.c.x, box->max.c.y, box->max.c.z );
  zVec3DCreate( &p[n-1], box->min.c.x, box->max.c.y, box->max.c.z );
}

void assert_box(void)
{
  zVec3D p1[N], p2[N], c1, c2, c1_ans, c2_ans;
  zVec3DData data1, data2;
  zAABox3D box1, box2;

  zVec3DDataAssignArrayDirect( &data1, p1, N );
  zVec3DDataAssignArrayDirect( &data2, p2, N );
  /* case 1 */
  zAABox3DCreate( &box1, 1, 1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3,-1,-1,-1 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( &data1, &data2, &c1, &c2 );
  zVec3DCopy( &box1.min, &c1_ans );
  zVec3DCopy( &box2.max, &c2_ans );
  zAssert( zGJK (volume case 1), zVec3DEqual( &c1, &c1_ans ) && zVec3DEqual( &c2, &c2_ans ) );
  /* case 2 */
  zAABox3DCreate( &box1, 1, 1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3, 2, 2, 2 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( &data1, &data2, &c1, &c2 );
  zAssert( zGJK (volume case 2), zAABox3DPointIsInside(&box1,&c1,zTOL) && zAABox3DPointIsInside(&box2,&c2,zTOL) );
  /* case 3 */
  zAABox3DCreate( &box1,-1,-1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3, 1, 1, 0 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( &data1, &data2, &c1, &c2 );
  zVec3DSubDRC( &c1, &c2 );
  zAssert( zGJK (volume case 3), zVec3DEqual( &c1, ZVEC3DZ ) );
  /* case 4 */
  zAABox3DCreate( &box1,-1,-1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3, 2, 2, 1 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( &data1, &data2, &c1, &c2 );
  zAssert( zGJK (degraded volume case 4),
    zEqual( c1.c.z, 1.0, zTOL ) && zEqual( c2.c.z, 1.0, zTOL ) && c1.c.x > -1 && c1.c.x < 2 && c1.c.y > -1 && c1.c.y < 2 );
  /* case 5 */
  zAABox3DCreate( &box1, 1, 1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3, 1, 1, 1 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( &data1, &data2, &c1, &c2 );
  zVec3DCreate( &c1_ans, 1, 1, 1 );
  zVec3DCreate( &c2_ans, 1, 1, 1 );
  zAssert( zGJK (degraded volume case 5), zVec3DEqual( &c1, &c1_ans ) && zVec3DEqual( &c2, &c2_ans ) );
}

void assert_point_volume(void)
{
  zVec3D pg[8];
  zVec3DData data;
  zVec3D p, c, c_answer;

  zVec3DCreate( &pg[0], 1, 1, 1 );
  zVec3DCreate( &pg[1],-1, 1, 1 );
  zVec3DCreate( &pg[2], 1,-1, 1 );
  zVec3DCreate( &pg[3],-1,-1, 1 );
  zVec3DCreate( &pg[4], 1, 1,-1 );
  zVec3DCreate( &pg[5],-1, 1,-1 );
  zVec3DCreate( &pg[6], 1,-1,-1 );
  zVec3DCreate( &pg[7],-1,-1,-1 );
  zVec3DDataAssignArrayDirect( &data, pg, 8 );

  zVec3DCreate( &p, -2, 0, -1 );
  zVec3DCreate( &c_answer, -1, 0, -1 );
  zAssert( zGJKPoint (volume case 1), zGJKPoint( &data, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p, -0.5, 0, 0 );
  zVec3DCreate( &c_answer, -0.5, 0, 0 );
  zAssert( zGJKPoint (volume case 2), zGJKPoint( &data, &p, &c ) == true  && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p,  2, 1, 0 );
  zVec3DCreate( &c_answer, 1, 1, 0 );
  zAssert( zGJKPoint (volume case 3), zGJKPoint( &data, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p,  2, 2, 2 );
  zVec3DCreate( &c_answer, 1, 1, 1 );
  zAssert( zGJKPoint (volume case 4), zGJKPoint( &data, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
}

void assert_point_plane(void)
{
  zVec3D pg[4];
  zVec3DData data;
  zVec3D p, c, c_answer;

  zVec3DCreate( &pg[0], 1, 1, 0 );
  zVec3DCreate( &pg[1],-1, 1, 0 );
  zVec3DCreate( &pg[2], 1,-1, 0 );
  zVec3DCreate( &pg[3],-1,-1, 0 );
  zVec3DDataAssignArrayDirect( &data, pg, 4 );

  zVec3DCreate( &p, -2, 0, 1 );
  zVec3DCreate( &c_answer, -1, 0, 0 );
  zAssert( zGJKPoint (plane case 1), zGJKPoint( &data, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p, 0, -0.5, 0 );
  zVec3DCreate( &c_answer, 0, -0.5, 0 );
  zAssert( zGJKPoint (plane case 2), zGJKPoint( &data, &p, &c ) == true  && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p, 2, 0, 0 );
  zVec3DCreate( &c_answer, 1, 0, 0 );
  zAssert( zGJKPoint (plane case 3), zGJKPoint( &data, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p, 2, 1, 0 );
  zVec3DCreate( &c_answer, 1, 1, 0 );
  zAssert( zGJKPoint (plane case 4), zGJKPoint( &data, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
}

/* a slanted thin rod near the origin and the origin, of various sizes, where the gap between them is
 * about 4.5% of the length of the rod if separated. */
bool check_mpr_rod(double scale, double width, bool expected)
{
  zVec3D a[8], b, c1, c2;
  zVec3DData data1, data2;
  int i;
  bool result;

  for( i=0; i<8; i++ )
    zVec3DCreate( &a[i], scale*( i&1 ? 2 : -2 ), scale*( ( i&1 ? 0.9 : -1.1 ) + ( i&2 ? width : -width ) ), scale*( i&4 ? 0.01 : -0.01 ) );
  zVec3DZero( &b );
  zVec3DDataAssignArrayDirect( &data1, a, 8 );
  zVec3DDataAssignArrayDirect( &data2, &b, 1 );
  result = zMPR( &data1, &data2 ) == expected && zGJK( &data1, &data2, &c1, &c2 ) == expected;
  zVec3DDataDestroy( &data1 );
  zVec3DDataDestroy( &data2 );
  return result;
}

void assert_mpr_scale(void)
{
  int i;
  double scale;
  bool result_separate = true, result_collide = true;

  for( i=0; i<N; i++ ){
    scale = pow( 10, zRandF(-8,4) );
    if( !check_mpr_rod( scale, 0.01, false ) ) result_separate = false;
    if( !check_mpr_rod( scale, 0.2, true ) ) result_collide = false;
  }
  zAssert( zMPR + zGJK (separated objects of various sizes), result_separate );
  zAssert( zMPR + zGJK (colliding objects of various sizes), result_collide );
}

/* random sets of points scaled from tiny to huge, for which the results of zGJK and zMPR have to
 * coincide with each other and with those at the unit scale, and the distance computed by zGJK has
 * to be proportional to the scale. The cases in which the scaled distance or penetration depth is
 * as small as zTOL are excluded, since the convex hulls are regarded as touching each other. */
#define NP 20
void assert_gjk_mpr_scale(void)
{
  zVec3D a0[NP], b0[NP], a[NP], b[NP], c1, c2, off;
  zVec3DData data1, data2;
  double scale[] = { 1.0e-8, 1.0e-6, 1.0e-4, 1.0e-2, 1.0e2, 1.0e4 };
  double d0, d;
  bool col0, col_gjk, col_mpr, result_col = true, result_dist = true;
  int i, j, k;

  for( i=0; i<N; i++ ){
    zVec3DCreate( &off, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
    for( j=0; j<NP; j++ ){
      zVec3DCreate( &a0[j], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
      zVec3DCreate( &b0[j], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
      zVec3DAddDRC( &b0[j], &off );
    }
    zVec3DDataAssignArrayDirect( &data1, a0, NP );
    zVec3DDataAssignArrayDirect( &data2, b0, NP );
    col0 = zGJK( &data1, &data2, &c1, &c2 );
    if( col0 ) zGJKDepth( &data1, &data2, &c1, &c2 );
    d0 = zVec3DDist( &c1, &c2 ); /* distance or penetration depth */
    if( zMPR( &data1, &data2 ) != col0 ) result_col = false;
    zVec3DDataDestroy( &data1 );
    zVec3DDataDestroy( &data2 );
    for( k=0; k<sizeof(scale)/sizeof(double); k++ ){
      for( j=0; j<NP; j++ ){
        zVec3DMul( &a0[j], scale[k], &a[j] );
        zVec3DMul( &b0[j], scale[k], &b[j] );
      }
      zVec3DDataAssignArrayDirect( &data1, a, NP );
      zVec3DDataAssignArrayDirect( &data2, b, NP );
      col_gjk = zGJK( &data1, &data2, &c1, &c2 );
      col_mpr = zMPR( &data1, &data2 );
      d = zVec3DDist( &c1, &c2 );
      if( d0 * scale[k] > 10 * zTOL && ( col_gjk != col0 || col_mpr != col0 ) ) result_col = false;
      /* the distance is computed within zTOL */
      if( !col0 && fabs( d - d0 * scale[k] ) > zMax( 10 * zTOL, 10 * DBL_EPSILON * d ) ) result_dist = false;
      zVec3DDataDestroy( &data1 );
      zVec3DDataDestroy( &data2 );
    }
  }
  zAssert( zGJK + zMPR (consistency for various sizes), result_col );
  zAssert( zGJK (distance for various sizes), result_dist );
}

/* points in a box [x0,x0+1]x[0,1]x[0,1] including the corners, which are rotated, translated by
 * offset and scaled by scale. */
/* a point p shifted by shift (if not NULL), rotated, translated by offset and scaled by scale. */
zVec3D *create_box_point_xform(zVec3D *p, zVec3D *shift, zMat3D *r, double offset, double scale, zVec3D *q)
{
  if( shift ) zVec3DAdd( p, shift, q );
  else zVec3DCopy( p, q );
  zMulMat3DVec3DDRC( r, q );
  return zVec3DCreate( q, ( q->c.x + offset ) * scale, ( q->c.y - offset ) * scale, ( q->c.z + offset ) * scale );
}

void create_box_points_xform(zVec3DData *data, double x0, zMat3D *r, double offset, double scale, int n)
{
  zVec3D p;
  int i;

  zVec3DDataInitArray( data, n + 8 );
  for( i=0; i<n+8; i++ ){
    if( i < 8 ) zVec3DCreate( &p, x0 + ( i & 1 ), ( i >> 1 ) & 1, ( i >> 2 ) & 1 );
    else zVec3DCreate( &p, x0 + zRandF(0,1), zRandF(0,1), zRandF(0,1) );
    zMulMat3DVec3DDRC( r, &p );
    zVec3DCreate( &p, ( p.c.x + offset ) * scale, ( p.c.y - offset ) * scale, ( p.c.z + offset ) * scale );
    zVec3DDataAdd( data, &p );
  }
}

/* two boxes separated by a gap, touching each other (gap = 0) or overlapping (gap < 0), of various
 * sizes and positions, for which zGJK has to terminate and the distance has to be proportional to the
 * scale. */
void assert_gjk_box_scale(void)
{
  zVec3DData data1, data2;
  zVec3D c1, c2;
  zMat3D r;
  double gap[] = { 0.1, 1.0e-6, 0, -0.1 }, scale, offset, d;
  int i, j, n = 50;
  bool result_col = true, result_dist = true;

  for( i=0; i<N; i++ ){
    j = i % 4;
    scale = pow( 10, zRandF(-9,9) );
    offset = i % 2 ? 1.0e3 : 0;
    zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    create_box_points_xform( &data1, 0, &r, offset, scale, n );
    create_box_points_xform( &data2, 1 + gap[j], &r, offset, scale, n );
    if( zGJK( &data1, &data2, &c1, &c2 ) != ( gap[j] <= 0 ) ) result_col = false;
    d = zVec3DDist( &c1, &c2 ) / scale;
    if( gap[j] > 0 && fabs( d - gap[j] ) > 1.0e-9 ) result_dist = false;
    zVec3DDataDestroy( &data1 );
    zVec3DDataDestroy( &data2 );
  }
  zAssert( zGJK (boxes of various sizes and positions), result_col );
  zAssert( zGJK (distance between boxes of various sizes and positions), result_dist );
}

/* two boxes of the same orientation overlapping each other of various sizes and positions, for
 * which the penetration depth is the minimum overlap along the axes. If aligned is true, the ranges
 * of the boxes along the x-axis coincide, and the boxes are not translated. In this case, the
 * Minkowski difference has many points on its edges, and nearly degenerate faces tend to appear in
 * the expanded polytope of EPA. */
void check_gjk_depth_box(bool aligned, bool *result_col, bool *result_depth)
{
  zVec3DData data1, data2;
  zVec3D c1, c2, p, shift;
  zMat3D r;
  double scale, offset, d, depth;
  int i, j, k, n = 50;

  *result_col = *result_depth = true;
  for( i=0; i<N; i++ ){
    scale = pow( 10, zRandF(-9,9) );
    offset = !aligned && i % 2 ? 1.0e3 : 0;
    zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zVec3DCreate( &shift, zRandF(-0.9,0.9), zRandF(-0.9,0.9), zRandF(-0.9,0.9) );
    if( aligned ) shift.c.x = 0;
    else if( i % 4 == 0 ) shift.c.x = i % 8 ? 1 : 0; /* touching or coinciding boxes */
    for( depth=HUGE_VAL, k=0; k<3; k++ )
      if( 1 - fabs( shift.e[k] ) < depth ) depth = 1 - fabs( shift.e[k] );
    zVec3DDataInitArray( &data1, n + 8 );
    zVec3DDataInitArray( &data2, n + 8 );
    for( j=0; j<n+8; j++ ){
      if( j < 8 ) zVec3DCreate( &p, j & 1, ( j >> 1 ) & 1, ( j >> 2 ) & 1 );
      else zVec3DCreate( &p, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
      zVec3DDataAdd( &data1, create_box_point_xform( &p, NULL, &r, offset, scale, &c1 ) );
      zVec3DDataAdd( &data2, create_box_point_xform( &p, &shift, &r, offset, scale, &c1 ) );
    }
    if( !zGJKDepth( &data1, &data2, &c1, &c2 ) ) *result_col = false;
    d = zVec3DDist( &c1, &c2 ) / scale;
    if( fabs( d - depth ) > 1.0e-9 ) *result_depth = false;
    zVec3DDataDestroy( &data1 );
    zVec3DDataDestroy( &data2 );
  }
}

void assert_gjk_depth_box(void)
{
  bool result_col, result_depth;

  check_gjk_depth_box( false, &result_col, &result_depth );
  zAssert( zGJKDepth (boxes of various sizes and positions), result_col );
  zAssert( zGJKDepth (penetration depth of boxes of various sizes and positions), result_depth );
  check_gjk_depth_box( true, &result_col, &result_depth );
  zAssert( zGJKDepth (aligned boxes of various sizes), result_col );
  zAssert( zGJKDepth (penetration depth of aligned boxes of various sizes), result_depth );
}

/* a point outside of, on the face of and inside of a box of various sizes and positions. */
void assert_gjk_point_scale(void)
{
  zVec3DData data;
  zVec3D p, c, pc;
  zMat3D r;
  double depth[] = { -0.1, -1.0e-6, 0, 0.1 }, scale, offset;
  int i, j, n = 50;
  bool result_col = true, result_dist = true;

  for( i=0; i<N; i++ ){
    j = i % 4;
    scale = pow( 10, zRandF(-9,9) );
    offset = i % 2 ? 1.0e3 : 0;
    zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    create_box_points_xform( &data, 0, &r, offset, scale, n );
    /* the point is at depth[j] from the face x = 0 inward, and pc is the closest point on the face */
    zVec3DCreate( &pc, 0, zRandF(0.2,0.8), zRandF(0.2,0.8) );
    zVec3DCreate( &p, depth[j], pc.c.y, pc.c.z );
    zMulMat3DVec3DDRC( &r, &p );
    zVec3DCreate( &p, ( p.c.x + offset ) * scale, ( p.c.y - offset ) * scale, ( p.c.z + offset ) * scale );
    if( zGJKPoint( &data, &p, &c ) != ( depth[j] >= 0 ) ) result_col = false;
    if( depth[j] < 0 && fabs( zVec3DDist( &p, &c ) / scale + depth[j] ) > 1.0e-9 ) result_dist = false;
    zVec3DDataDestroy( &data );
  }
  zAssert( zGJKPoint (boxes of various sizes and positions), result_col );
  zAssert( zGJKPoint (distance to boxes of various sizes and positions), result_dist );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_box();
  assert_point_volume();
  assert_point_plane();
  assert_mpr_scale();
  assert_gjk_mpr_scale();
  assert_gjk_box_scale();
  assert_gjk_point_scale();
  assert_gjk_depth_box();
  return 0;
}
