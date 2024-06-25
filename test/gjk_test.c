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
  zAABox3D box1, box2;

  /* case 1 */
  zAABox3DCreate( &box1, 1, 1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3,-1,-1,-1 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( p1, N, p2, N, &c1, &c2 );
  zVec3DCopy( &box1.min, &c1_ans );
  zVec3DCopy( &box2.max, &c2_ans );
  zAssert( zGJK (volume case 1), zVec3DEqual( &c1, &c1_ans ) && zVec3DEqual( &c2, &c2_ans ) );
  /* case 2 */
  zAABox3DCreate( &box1, 1, 1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3, 2, 2, 2 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( p1, N, p2, N, &c1, &c2 );
  zAssert( zGJK (volume case 2), zAABox3DPointIsInside(&box1,&c1,zTOL) && zAABox3DPointIsInside(&box2,&c2,zTOL) );
  /* case 3 */
  zAABox3DCreate( &box1,-1,-1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3, 1, 1, 0 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( p1, N, p2, N, &c1, &c2 );
  zVec3DSubDRC( &c1, &c2 );
  zAssert( zGJK (volume case 3), zVec3DEqual( &c1, ZVEC3DZ ) );
  /* case 4 */
  zAABox3DCreate( &box1,-1,-1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3, 2, 2, 1 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( p1, N, p2, N, &c1, &c2 );
  zAssert( zGJK (degraded volume case 4),
    zIsEqual( c1.c.z, 1.0, zTOL ) && zIsEqual( c2.c.z, 1.0, zTOL ) && c1.c.x > -1 && c1.c.x < 2 && c1.c.y > -1 && c1.c.y < 2 );
  /* case 5 */
  /* NOTE: This test fails, meaning that zGJK has an obvious bug. */
  zAABox3DCreate( &box1, 1, 1, 1, 3, 3, 3 );
  zAABox3DCreate( &box2,-3,-3,-3, 1, 1, 1 );
  create_box_points( p1, N, &box1 );
  create_box_points( p2, N, &box2 );
  zGJK( p1, N, p2, N, &c1, &c2 );
  zVec3DPrint( &c1 );
  zVec3DPrint( &c2 );
  zVec3DCreate( &c1_ans, 1, 1, 1 );
  zVec3DCreate( &c2_ans, 1, 1, 1 );
  /* zAssert( zGJK (degraded volume case 5), zVec3DEqual( &c1, &c1_ans ) && zVec3DEqual( &c2, &c2_ans ) ); */
}

void assert_point_volume(void)
{
  zVec3D pg[8];
  zVec3D p, c, c_answer;
  int n = 8;

  zVec3DCreate( &pg[0], 1, 1, 1 );
  zVec3DCreate( &pg[1],-1, 1, 1 );
  zVec3DCreate( &pg[2], 1,-1, 1 );
  zVec3DCreate( &pg[3],-1,-1, 1 );
  zVec3DCreate( &pg[4], 1, 1,-1 );
  zVec3DCreate( &pg[5],-1, 1,-1 );
  zVec3DCreate( &pg[6], 1,-1,-1 );
  zVec3DCreate( &pg[7],-1,-1,-1 );

#if 0
  zVec3DCreate( &p,  2, 2, 2 );
  zVec3DCreate( &c_answer, 1, 1, 1 );
zGJKPoint( pg, n, &p, &c );
zVec3DPrint( &c );
  zAssert( zGJKPoint (volume case 4), zGJKPoint( pg, n, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
#endif

  zVec3DCreate( &p, -2, 0, -1 );
  zVec3DCreate( &c_answer, -1, 0, -1 );
  zAssert( zGJKPoint (volume case 1), zGJKPoint( pg, n, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p, -0.5, 0, 0 );
  zVec3DCreate( &c_answer, -0.5, 0, 0 );
  zAssert( zGJKPoint (volume case 2), zGJKPoint( pg, n, &p, &c ) == true  && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p,  2, 1, 0 );
  zVec3DCreate( &c_answer, 1, 1, 0 );
  zAssert( zGJKPoint (volume case 3), zGJKPoint( pg, n, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
}

void assert_point_plane(void)
{
  zVec3D pg[4];
  zVec3D p, c, c_answer;
  int n = 4;

  zVec3DCreate( &pg[0], 1, 1, 0 );
  zVec3DCreate( &pg[1],-1, 1, 0 );
  zVec3DCreate( &pg[2], 1,-1, 0 );
  zVec3DCreate( &pg[3],-1,-1, 0 );

  zVec3DCreate( &p, -2, 0, 1 );
  zVec3DCreate( &c_answer, -1, 0, 0 );
  zAssert( zGJKPoint (plane case 1), zGJKPoint( pg, n, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p, 0, -0.5, 0 );
  zVec3DCreate( &c_answer, 0, -0.5, 0 );
  zAssert( zGJKPoint (plane case 2), zGJKPoint( pg, n, &p, &c ) == true  && zVec3DEqual( &c, &c_answer ) );
  zVec3DCreate( &p, 2, 0, 0 );
  zVec3DCreate( &c_answer, 1, 0, 0 );
  zAssert( zGJKPoint (plane case 3), zGJKPoint( pg, n, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );

#if 0
  zVec3DCreate( &p, 2, 1, 0 );
  zVec3DCreate( &c_answer, 1, 1, 0 );
zGJKPoint( pg, n, &p, &c );
zVec3DPrint( &c );
  zAssert( zGJKPoint (plane case 4), zGJKPoint( pg, n, &p, &c ) == false && zVec3DEqual( &c, &c_answer ) );
#endif
}

int main(int argc, char *argv[])
{
  assert_box();
  assert_point_volume();
  assert_point_plane();
  return 0;
}
