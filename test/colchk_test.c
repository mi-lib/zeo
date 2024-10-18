#include <zeo/zeo_col.h>

#define N 1000

void assert_intersect_plane(void)
{
  zPlane3D plane1, plane2;
  zVec3D p1, p2, d1, d2;
  zLine3D line;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    zVec3DCreate( &p1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &d1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zPlane3DCreate( &plane1, &p1, &d1 );
    zVec3DCreate( &p2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &d2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zPlane3DCreate( &plane2, &p2, &d2 );
    zIntersectPlane3D( &plane1, &plane2, &line );
    if( !zPlane3DPointIsOn( &plane1, zLine3DOrg(&line), zTOL ) ||
        !zPlane3DPointIsOn( &plane2, zLine3DOrg(&line), zTOL ) ||
        !zIsTiny( zVec3DInnerProd( zPlane3DNorm(&plane1), zLine3DDir(&line) ) ) ||
        !zIsTiny( zVec3DInnerProd( zPlane3DNorm(&plane2), zLine3DDir(&line) ) ) ) result = false;
  }
  zAssert( zIntersectPlane3D (random case), result );

  zVec3DCreate( &p1, 0, 0, 0 );
  zVec3DCreate( &d1, 0, 0, 1 );
  zPlane3DCreate( &plane1, &p1, &d1 );
  zVec3DCreate( &p2, 1, 1, 0 );
  zVec3DCreate( &d2, 0, 0, 1 );
  zPlane3DCreate( &plane2, &p2, &d2 );
  zAssert( zIntersectPlane3D (identical case), zIntersectPlane3D( &plane1, &plane2, &line ) == -1 );
  zVec3DCreate( &p2, 1, 1, 1 );
  zPlane3DCreate( &plane2, &p2, &d2 );
  zAssert( zIntersectPlane3D (parallel case), zIntersectPlane3D( &plane1, &plane2, &line ) == 0 );
}

int main(void)
{
  zRandInit();
  assert_intersect_plane();
  return 0;
}
