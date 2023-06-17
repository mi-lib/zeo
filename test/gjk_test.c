#include <zeo/zeo.h>

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
  assert_point_volume();
  assert_point_plane();
  return 0;
}
