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

/* distance from a point to an axis-aligned box [min,max], and the closest point. */
double box_closest(const zVec3D *min, const zVec3D *max, const zVec3D *p, zVec3D *cp)
{
  int k;

  for( k=0; k<3; k++ )
    cp->e[k] = p->e[k] < min->e[k] ? min->e[k] : ( p->e[k] > max->e[k] ? max->e[k] : p->e[k] );
  return zVec3DDist( p, cp );
}

/* a non-convex L-shaped prism, which is the union of two boxes. */
void assert_ph3d_closest_nonconvex(void)
{
  zVec3D bottom[] = {
    { { 0, 0, 0 } }, { { 2, 0, 0 } }, { { 2, 1, 0 } }, { { 1, 1, 0 } }, { { 1, 2, 0 } }, { { 0, 2, 0 } },
  };
  zVec3D shift = { { 0, 0, 1 } };
  zVec3D min1 = { { 0, 0, 0 } }, max1 = { { 2, 1, 1 } }, min2 = { { 0, 0, 0 } }, max2 = { { 1, 2, 1 } };
  zPH3D ph;
  zVec3D p, cp, cp1, cp2;
  double d, d1, d2, margin = 0.1;
  int i, n = 10000;
  bool inside, result_closest = true, result_inside = true, result_margin = true;

  zPH3DCreatePrism( &ph, bottom, 6, &shift );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-1,3), zRandF(-1,3), zRandF(-1,2) );
    d1 = box_closest( &min1, &max1, &p, &cp1 );
    d2 = box_closest( &min2, &max2, &p, &cp2 );
    inside = zIsTiny( d1 ) || zIsTiny( d2 );
    d = zPH3DClosest( &ph, &p, &cp );
    if( inside ){
      if( !zIsTiny( d ) || !zVec3DEqual( &cp, &p ) ) result_closest = false;
    } else
    if( !zIsTiny( d - zMin( d1, d2 ) ) || !zVec3DEqual( &cp, d1 < d2 ? &cp1 : &cp2 ) ) result_closest = false;
    if( zPH3DPointIsInside( &ph, &p, 0 ) != inside ) result_inside = false;
    if( zPH3DPointIsInside( &ph, &p, margin ) != ( inside || zMin( d1, d2 ) < margin ) ) result_margin = false;
  }
  zPH3DDestroy( &ph );
  zAssert( zPH3DClosest (non-convex polyhedron), result_closest );
  zAssert( zPH3DPointIsInside (non-convex polyhedron), result_inside );
  zAssert( zPH3DPointIsInside (non-convex polyhedron with a margin), result_margin );
}

/* a box with a negative margin and a planar convex hull of points on a square. */
void assert_ph3d_closest_degenerate(void)
{
  zVec3D min = { { -1, -1, 0 } }, max = { { 1, 1, 0 } }, bmin = { { -1, -1, -1 } }, bmax = { { 1, 1, 1 } };
  zVec3DData data;
  zShape3D box;
  zPH3D ph;
  zVec3D p, cp, cpt;
  double d, depth, margin = -0.2;
  int i, k, n = 10000;
  bool result_planar = true, result_planar_inside = true, result_shrink = true;

  zVec3DDataInitList( &data );
  for( i=0; i<100; i++ ){
    zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), 0 );
    if( i < 4 ) zVec3DCreate( &p, i % 2 ? 1 : -1, i / 2 ? 1 : -1, 0 ); /* corners */
    zVec3DDataAdd( &data, &p );
  }
  zVec3DDataConvexHull( &data, &ph );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-2,2), zRandF(-2,2), i % 2 ? 0 : zRandF(-1,1) );
    d = zPH3DClosest( &ph, &p, &cp );
    if( !zIsTiny( d - box_closest( &min, &max, &p, &cpt ) ) || !zVec3DEqual( &cp, &cpt ) ) result_planar = false;
    if( zPH3DPointIsInside( &ph, &p, 0 ) ) result_planar_inside = false; /* no inside area */
  }
  zPH3DDestroy( &ph );
  zVec3DDataDestroy( &data );
  zShape3DBoxCreateAlign( &box, ZVEC3DZERO, 2, 2, 2 );
  zShape3DToPH( &box );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-1.5,1.5), zRandF(-1.5,1.5), zRandF(-1.5,1.5) );
    for( depth=HUGE_VAL, k=0; k<3; k++ )
      depth = zMin( depth, zMin( p.e[k] - bmin.e[k], bmax.e[k] - p.e[k] ) );
    if( zPH3DPointIsInside( zShape3DPH(&box), &p, margin ) != ( depth > -margin ) ) result_shrink = false;
  }
  zShape3DDestroy( &box );
  zAssert( zPH3DClosest (planar polyhedron), result_planar );
  zAssert( zPH3DPointIsInside (planar polyhedron), result_planar_inside );
  zAssert( zPH3DPointIsInside (negative margin), result_shrink );
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
  assert_ph3d_closest_nonconvex();
  assert_ph3d_closest_degenerate();
  assert_ph3d_aabb();
  return 0;
}
