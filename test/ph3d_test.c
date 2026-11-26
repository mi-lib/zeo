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
  int i, k;
  bool result = true;
  const int pointnum = 100;

  for( k=0; k<2; k++ ){ /* the second set of points does not surround the origin */
    zVec3DDataInitList( &data );
    for( i=0; i<pointnum; i++ ){
      if( k == 0 )
        zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
      else
        zVec3DCreate( &p, zRandF(10,20), zRandF(-20,-10), zRandF(5,10) );
      zVec3DDataAdd( &data, &p );
    }
    zVec3DDataConvexHull( &data, &ph );
    if( !zPH3DAABB( &ph, &aabb1 ) ) result = false;
    zVec3DDataAABB( &data, &aabb2, NULL );
    zVec3DDataDestroy( &data );
    zPH3DDestroy( &ph );
    if( !zAABox3DEqual( &aabb1, &aabb2 ) ) result = false;
  }
  zPH3DInit( &ph );
  if( zPH3DAABB( &ph, &aabb1 ) || !zAABox3DEqual( &aabb1, zAABox3DInit( &aabb2 ) ) ) result = false;
  zAssert( zPH3DAABB, result );
}

void assert_ph3d_is_closed(void)
{
  zVec3DData data;
  zVec3D p;
  zPH3D ph;
  zTri3D tri;
  int i;
  const int pointnum = 100;
  bool result_closed, result_flipped, result_open, result_twisted, result_duplicate, result_nonmanifold;

  zVec3DDataInitList( &data );
  for( i=0; i<pointnum; i++ ){
    zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DDataAdd( &data, &p );
  }
  zVec3DDataConvexHull( &data, &ph );
  zVec3DDataDestroy( &data );
  result_closed = zPH3DIsClosed( &ph );
  zPH3DFlipDRC( &ph );
  result_flipped = zPH3DIsClosed( &ph );
  /* a face removed */
  zCopy( zTri3D, zPH3DFace(&ph,0), &tri );
  zPH3DSetFaceNum( &ph, zPH3DFaceNum(&ph) - 1 );
  result_open = !zPH3DIsClosed( &ph );
  zPH3DSetFaceNum( &ph, zPH3DFaceNum(&ph) + 1 );
  /* a face flipped */
  zTri3DFlip( &tri, zPH3DFace(&ph,0) );
  result_twisted = !zPH3DIsClosed( &ph );
  zCopy( zTri3D, &tri, zPH3DFace(&ph,0) );
  zPH3DDestroy( &ph );
  /* a tetrahedron, and the same one with a duplicate vertex used in a face */
  zPH3DAlloc( &ph, 5, 4 );
  zVec3DCreate( zPH3DVert(&ph,0), 0, 0, 0 );
  zVec3DCreate( zPH3DVert(&ph,1), 1, 0, 0 );
  zVec3DCreate( zPH3DVert(&ph,2), 0, 1, 0 );
  zVec3DCreate( zPH3DVert(&ph,3), 0, 0, 1 );
  zVec3DCopy( zPH3DVert(&ph,0), zPH3DVert(&ph,4) );
  zTri3DCreate( zPH3DFace(&ph,0), zPH3DVert(&ph,0), zPH3DVert(&ph,2), zPH3DVert(&ph,1) );
  zTri3DCreate( zPH3DFace(&ph,1), zPH3DVert(&ph,0), zPH3DVert(&ph,1), zPH3DVert(&ph,3) );
  zTri3DCreate( zPH3DFace(&ph,2), zPH3DVert(&ph,0), zPH3DVert(&ph,3), zPH3DVert(&ph,2) );
  zTri3DCreate( zPH3DFace(&ph,3), zPH3DVert(&ph,1), zPH3DVert(&ph,2), zPH3DVert(&ph,3) );
  result_closed = result_closed && zPH3DIsClosed( &ph );
  zTri3DCreate( zPH3DFace(&ph,2), zPH3DVert(&ph,4), zPH3DVert(&ph,3), zPH3DVert(&ph,2) );
  result_duplicate = !zPH3DIsClosed( &ph );
  zPH3DDestroy( &ph );
  /* two tetrahedra sharing an edge, which is shared by four faces */
  zPH3DAlloc( &ph, 6, 8 );
  zVec3DCreate( zPH3DVert(&ph,0), 0, 0, 0 );
  zVec3DCreate( zPH3DVert(&ph,1), 0, 0, 1 );
  zVec3DCreate( zPH3DVert(&ph,2), 1, 0, 0 );
  zVec3DCreate( zPH3DVert(&ph,3), 0, 1, 0 );
  zVec3DCreate( zPH3DVert(&ph,4), -1, 0, 0 );
  zVec3DCreate( zPH3DVert(&ph,5), 0, -1, 0 );
  for( i=0; i<2; i++ ){
    zTri3DCreate( zPH3DFace(&ph,4*i  ), zPH3DVert(&ph,0), zPH3DVert(&ph,2*i+3), zPH3DVert(&ph,2*i+2) );
    zTri3DCreate( zPH3DFace(&ph,4*i+1), zPH3DVert(&ph,0), zPH3DVert(&ph,2*i+2), zPH3DVert(&ph,1) );
    zTri3DCreate( zPH3DFace(&ph,4*i+2), zPH3DVert(&ph,0), zPH3DVert(&ph,1), zPH3DVert(&ph,2*i+3) );
    zTri3DCreate( zPH3DFace(&ph,4*i+3), zPH3DVert(&ph,1), zPH3DVert(&ph,2*i+2), zPH3DVert(&ph,2*i+3) );
  }
  result_nonmanifold = zPH3DIsClosed( &ph ) && zIsTol( zPH3DVolume( &ph ) - 1.0/3, zTOL );
  zPH3DDestroy( &ph );
  zAssert( zPH3DIsClosed, result_closed && result_flipped && result_open && result_twisted && result_duplicate && result_nonmanifold );
}

/* tetrahedron at a position. */
void create_tetrahedron(zPH3D *ph, int offset, double x)
{
  zVec3DCreate( zPH3DVert(ph,offset  ), x, 0, 0 );
  zVec3DCreate( zPH3DVert(ph,offset+1), x+1, 0, 0 );
  zVec3DCreate( zPH3DVert(ph,offset+2), x, 1, 0 );
  zVec3DCreate( zPH3DVert(ph,offset+3), x, 0, 1 );
  zTri3DCreate( zPH3DFace(ph,offset  ), zPH3DVert(ph,offset), zPH3DVert(ph,offset+2), zPH3DVert(ph,offset+1) );
  zTri3DCreate( zPH3DFace(ph,offset+1), zPH3DVert(ph,offset), zPH3DVert(ph,offset+1), zPH3DVert(ph,offset+3) );
  zTri3DCreate( zPH3DFace(ph,offset+2), zPH3DVert(ph,offset), zPH3DVert(ph,offset+3), zPH3DVert(ph,offset+2) );
  zTri3DCreate( zPH3DFace(ph,offset+3), zPH3DVert(ph,offset+1), zPH3DVert(ph,offset+2), zPH3DVert(ph,offset+3) );
}

void assert_ph3d_is_convex(void)
{
  zVec3DData data;
  zVec3D p, c;
  zPH3D ph;
  zSphere3D sphere;
  int i, j;
  double scale;
  const int testnum = 100, pointnum = 100;
  bool result_convex = true, result_dented = true, result_sphere = true, result_apart, result_nonmanifold;

  for( i=0; i<testnum; i++ ){
    scale = pow( 10, zRandF(-3,3) );
    zVec3DDataInitList( &data );
    for( j=0; j<pointnum; j++ ){
      zVec3DCreate( &p, zRandF(-scale,scale), zRandF(-scale,scale), zRandF(-scale,scale) );
      zVec3DDataAdd( &data, &p );
    }
    zVec3DDataConvexHull( &data, &ph );
    zVec3DDataDestroy( &data );
    if( !zPH3DIsConvex( &ph ) ) result_convex = false;
    /* a vertex pushed toward the barycenter, which makes the polyhedron non-convex */
    zPH3DBarycenter( &ph, &c );
    j = zRandI( 0, zPH3DVertNum(&ph)-1 );
    zVec3DMid( zPH3DVert(&ph,j), &c, &p );
    zVec3DCopy( &p, zPH3DVert(&ph,j) );
    for( j=0; j<zPH3DFaceNum(&ph); j++ ) zTri3DCalcNorm( zPH3DFace(&ph,j) );
    if( zPH3DIsConvex( &ph ) ) result_dented = false;
    zPH3DDestroy( &ph );
    /* a polyhedron converted from a sphere */
    zSphere3DCreate( &sphere, ZVEC3DZERO, scale, zRandI(4,64) );
    zSphere3DToPH( &sphere, &ph );
    if( !zPH3DIsConvex( &ph ) ) result_sphere = false;
    zPH3DDestroy( &ph );
  }
  /* two tetrahedra apart from each other */
  zPH3DAlloc( &ph, 8, 8 );
  create_tetrahedron( &ph, 0, 0 );
  create_tetrahedron( &ph, 4, 3 );
  result_apart = zPH3DIsClosed( &ph ) && !zPH3DIsConvex( &ph );
  zPH3DDestroy( &ph );
  /* two tetrahedra sharing an edge, which is shared by four faces */
  zPH3DAlloc( &ph, 6, 8 );
  zVec3DCreate( zPH3DVert(&ph,0), 0, 0, 0 );
  zVec3DCreate( zPH3DVert(&ph,1), 0, 0, 1 );
  zVec3DCreate( zPH3DVert(&ph,2), 1, 0, 0 );
  zVec3DCreate( zPH3DVert(&ph,3), 0, 1, 0 );
  zVec3DCreate( zPH3DVert(&ph,4), -1, 0, 0 );
  zVec3DCreate( zPH3DVert(&ph,5), 0, -1, 0 );
  for( i=0; i<2; i++ ){
    zTri3DCreate( zPH3DFace(&ph,4*i  ), zPH3DVert(&ph,0), zPH3DVert(&ph,2*i+3), zPH3DVert(&ph,2*i+2) );
    zTri3DCreate( zPH3DFace(&ph,4*i+1), zPH3DVert(&ph,0), zPH3DVert(&ph,2*i+2), zPH3DVert(&ph,1) );
    zTri3DCreate( zPH3DFace(&ph,4*i+2), zPH3DVert(&ph,0), zPH3DVert(&ph,1), zPH3DVert(&ph,2*i+3) );
    zTri3DCreate( zPH3DFace(&ph,4*i+3), zPH3DVert(&ph,1), zPH3DVert(&ph,2*i+2), zPH3DVert(&ph,2*i+3) );
  }
  result_nonmanifold = zPH3DIsClosed( &ph ) && !zPH3DIsConvex( &ph );
  zPH3DDestroy( &ph );
  zAssert( zPH3DIsConvex (convex hulls), result_convex );
  zAssert( zPH3DIsConvex (dented polyhedra), result_dented );
  zAssert( zPH3DIsConvex (spheres), result_sphere );
  zAssert( zPH3DIsConvex (two separate tetrahedra), result_apart );
  zAssert( zPH3DIsConvex (two tetrahedra sharing an edge), result_nonmanifold );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_ph3d_closestpoint();
  assert_ph3d_closest_nonconvex();
  assert_ph3d_closest_degenerate();
  assert_ph3d_aabb();
  assert_ph3d_is_closed();
  assert_ph3d_is_convex();
  return 0;
}
