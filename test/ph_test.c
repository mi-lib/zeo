#include <zeo/zeo.h>

void assert_ph3d_closestpoint(void)
{
  zShape3D shape1, shape2;
  zVec3D point, cp1, cp2, error;
  double dist1, dist2;
  int i;
  const int n = 1000;
  bool result = true;

  zShape3DBoxCreateAlign( &shape1, ZVEC3DZERO, 1, 1, 1 );
  zShape3DClone( &shape1, &shape2, NULL );
  zShape3DToPH( &shape2 );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &point, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
    dist1 = zShape3DClosest( &shape1, &point, &cp1 );
    dist2 = zShape3DClosest( &shape2, &point, &cp2 );
    if( !zVec3DEqual( &cp1, &cp2 ) ){
      eprintf( "sample       " ); zVec3DFPrint( stderr, &point );
      eprintf( "(box)        " ); zVec3DValueFPrint( stderr, &cp1 ); eprintf( " distance = %g\n", dist1 );
      eprintf( "(polyhedron) " ); zVec3DValueFPrint( stderr, &cp2 ); eprintf( " distance = %g\n", dist2 );
      zVec3DSub( &cp1, &cp2, &error );
      eprintf( "error = " );
      zVec3DFPrint( stderr, &error );
      result = false;
    }
  }
  zShape3DDestroy( &shape1 );
  zShape3DDestroy( &shape2 );
  zAssert( zShape3DClosest (box x polyhedron), result );
}

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
  assert_ph3d_closestpoint();
  assert_ph3d_aabb();
  return 0;
}
