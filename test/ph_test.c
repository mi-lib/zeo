#include <zeo/zeo.h>

void assert_ph3d_aabb(void)
{
  zVec3DData data;
  zVec3D p;
  zPH3D ph;
  zAABox3D aabb1, aabb2;
  int i;
  const int pointnum = 100;

  zVec3DDataInitList( &data );
  for( i=0; i<pointnum; i++ ){
    zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DDataAdd( &data, &p );
  }
  zVec3DDataConvexHull( &data, &ph );
  zPH3DAABB( &ph, &aabb1 );
  zVec3DDataAABB( &data, &aabb2, NULL );
  zVec3DDataDestroy( &data );
  zPH3DDestroy( &ph );
  zAssert( zPH3DAABB, zAABox3DEqual( &aabb1, &aabb2 ) );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_ph3d_aabb();
  return 0;
}
