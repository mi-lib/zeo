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

int main(int argc, char *argv[])
{
  assert_box();
  assert_point_volume();
  assert_point_plane();
  assert_mpr_scale();
  assert_gjk_mpr_scale();
  return 0;
}
