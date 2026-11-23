#include <zeo/zeo.h>

#define N 200

/* random rotation */
void create_rot(zMat3D *r)
{
  zVec3D aa;

  zVec3DCreate( &aa, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DFromAA( r, &aa );
}

/* types of point sets */
enum{ RANDOM_CUBE = 0, SPHERE, CUBE_SURFACE, GRID, BOX_EDGES, CYLINDER, DUPLICATE, NEAR_PLANAR, PLANAR, PLANAR_GRID, PLANAR_CIRCLE, TYPE_NUM };

/* generate a set of points, which are rotated, scaled and translated. */
void create_points(zVec3DData *data, int type, double scale, double offset, bool rotate)
{
  zVec3D p, q[10], t;
  zMat3D r;
  double th;
  int i, j, k, m;

  zVec3DDataInitList( data );
  if( rotate ) create_rot( &r ); else zMat3DIdent( &r );
  zVec3DCreate( &t, offset, 0.7*offset, -0.3*offset );
  for( j=0; j<10; j++ ) zVec3DCreate( &q[j], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  m = type == GRID ? 5 : 14;
  for( i=0; i<N; i++ ){
    if( type == GRID && i >= m*m*m ) break; /* a complete cubic grid */
    switch( type ){
    case RANDOM_CUBE:  zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) ); break;
    case SPHERE:       zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) ); zVec3DNormalizeDRC( &p, 0 ); break;
    case CUBE_SURFACE: zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) ); p.e[zRandI(0,2)] = zRandI(0,1) ? 1 : -1; break;
    case GRID:         zVec3DCreate( &p, i%m, (i/m)%m, i/(m*m) ); break;
    case BOX_EDGES:    zVec3DCreate( &p, zRandI(0,1)?1:-1, zRandI(0,1)?1:-1, zRandI(0,1)?1:-1 ); p.e[zRandI(0,2)] = zRandF(-1,1); break;
    case CYLINDER:     th = zRandF(0,zPI*2); zVec3DCreate( &p, cos(th), sin(th), i%3 == 0 ? zRandF(-1,1) : ( i%3 == 1 ? 1 : -1 ) ); break;
    case DUPLICATE:    zVec3DCopy( &q[i%10], &p ); break;
    case NEAR_PLANAR:  zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1.0e-9,1.0e-9) ); break;
    case PLANAR:       zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), 0 ); break;
    case PLANAR_GRID:  zVec3DCreate( &p, i%m, (i/m)%m, 0 ); break;
    case PLANAR_CIRCLE: zVec3DCreate( &p, cos(zPIx2*i/N), sin(zPIx2*i/N), 0 ); break;
    default: ;
    }
    zMulMat3DVec3DDRC( &r, &p );
    for( k=0; k<3; k++ ) p.e[k] = p.e[k] * scale + t.e[k] * scale;
    zVec3DDataAdd( data, &p );
  }
}

/* check if a point is inside of a triangle on a plane, where the projection along an axis is used. */
bool check_tri_inside(zTri3D *f, zVec3D *p, zAxis axis)
{
  int o0, o1, o2;

  o0 = zVec3DOrient2D( zTri3DVert(f,0), zTri3DVert(f,1), p, axis );
  o1 = zVec3DOrient2D( zTri3DVert(f,1), zTri3DVert(f,2), p, axis );
  o2 = zVec3DOrient2D( zTri3DVert(f,2), zTri3DVert(f,0), p, axis );
  return ( o0 >= 0 && o1 >= 0 && o2 >= 0 ) || ( o0 <= 0 && o1 <= 0 && o2 <= 0 );
}

/* check if the normal vector of a triangle is the zero vector or the unit vector facing the front side. */
bool check_tri_norm(zTri3D *f)
{
  zVec3D e1, e2, n;

  if( zVec3DIsTiny( zTri3DNorm(f) ) ) return true;
  zVec3DSub( zTri3DVert(f,1), zTri3DVert(f,0), &e1 );
  zVec3DSub( zTri3DVert(f,2), zTri3DVert(f,0), &e2 );
  zVec3DOuterProd( &e1, &e2, &n );
  return zIsTiny( zVec3DNorm( zTri3DNorm(f) ) - 1 ) && zVec3DInnerProd( zTri3DNorm(f), &n ) > 0;
}

/* check if no face of a polyhedron has the zero normal vector. */
bool check_nonzero_norm(zPH3D *ch)
{
  int i;

  for( i=0; i<zPH3DFaceNum(ch); i++ )
    if( zVec3DIsTiny( zPH3DFaceNorm(ch,i) ) ) return false;
  return true;
}

/* check if a polyhedron is the convex hull of a set of points within a tolerance:
 * - every vertex is a point of the set,
 * - no face is degenerate,
 * - the normal vector of every face is the zero vector or the unit vector facing the front side,
 * - every edge is shared by two faces in the opposite directions,
 * - every vertex is beneath or on every face (the convexity),
 * - every point is beneath or on every face, or within the tolerance from the polyhedron.
 * For a planar convex hull, which is exactly judged, the last three are replaced by that every point
 * is on the plane, and is inside of a face or within the tolerance from a face. */
bool check_convexhull(zVec3DData *data, zPH3D *ch, double tol)
{
  zVec3D *v, *a, *b;
  zTri3D *f;
  zAxis axis;
  int i, j, k, l, n;
  double d, dmin;
  bool planar = true;

  if( !ch || zPH3DFaceNum(ch) == 0 ) return false;
  for( i=0; i<zPH3DVertNum(ch); i++ ){
    zVec3DDataRewind( data );
    while( ( v = zVec3DDataFetch( data ) ) )
      if( zVec3DEqual( v, zPH3DVert(ch,i) ) ) break;
    if( !v ) return false;
    if( zVec3DOrient3D( zTri3DVert(zPH3DFace(ch,0),0), zTri3DVert(zPH3DFace(ch,0),1), zTri3DVert(zPH3DFace(ch,0),2), zPH3DVert(ch,i) ) != 0 )
      planar = false;
  }
  for( i=0; i<zPH3DFaceNum(ch); i++ ){
    f = zPH3DFace(ch,i);
    if( zVec3DOrient2D( zTri3DVert(f,0), zTri3DVert(f,1), zTri3DVert(f,2), zX ) == 0 &&
        zVec3DOrient2D( zTri3DVert(f,0), zTri3DVert(f,1), zTri3DVert(f,2), zY ) == 0 &&
        zVec3DOrient2D( zTri3DVert(f,0), zTri3DVert(f,1), zTri3DVert(f,2), zZ ) == 0 ) return false;
    if( !check_tri_norm( f ) ) return false;
  }
  if( planar ){
    f = zPH3DFace(ch,0);
    for( axis=zX; zVec3DOrient2D( zTri3DVert(f,0), zTri3DVert(f,1), zTri3DVert(f,2), axis ) == 0; axis++ );
    zVec3DDataRewind( data );
    while( ( v = zVec3DDataFetch( data ) ) ){
      if( zVec3DOrient3D( zTri3DVert(f,0), zTri3DVert(f,1), zTri3DVert(f,2), v ) != 0 ) return false;
      for( i=0; i<zPH3DFaceNum(ch); i++ )
        if( check_tri_inside( zPH3DFace(ch,i), v, axis ) ) break;
      if( i < zPH3DFaceNum(ch) ) continue;
      for( dmin=HUGE_VAL, i=0; i<zPH3DFaceNum(ch); i++ )
        if( ( d = zTri3DDistFromPoint( zPH3DFace(ch,i), v ) ) < dmin ) dmin = d;
      if( dmin > tol ) return false;
    }
    return true;
  }
  for( i=0; i<zPH3DFaceNum(ch); i++ ){
    f = zPH3DFace(ch,i);
    for( j=0; j<3; j++ ){
      a = zTri3DVert(f,j);
      b = zTri3DVert(f,(j+1)%3);
      for( n=0, k=0; k<zPH3DFaceNum(ch); k++ )
        for( l=0; l<3; l++ )
          if( zTri3DVert(zPH3DFace(ch,k),l) == b && zTri3DVert(zPH3DFace(ch,k),(l+1)%3) == a ) n++;
      if( n != 1 ) return false;
    }
    for( j=0; j<zPH3DVertNum(ch); j++ )
      if( zVec3DOrient3D( zTri3DVert(f,0), zTri3DVert(f,1), zTri3DVert(f,2), zPH3DVert(ch,j) ) > 0 ) return false;
  }
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    for( i=0; i<zPH3DFaceNum(ch); i++ ){
      f = zPH3DFace(ch,i);
      if( zVec3DOrient3D( zTri3DVert(f,0), zTri3DVert(f,1), zTri3DVert(f,2), v ) > 0 ) break;
    }
    if( i == zPH3DFaceNum(ch) ) continue;
    for( dmin=HUGE_VAL, i=0; i<zPH3DFaceNum(ch); i++ )
      if( ( d = zTri3DDistFromPoint( zPH3DFace(ch,i), v ) ) < dmin ) dmin = d;
    if( dmin > tol ) return false;
  }
  return true;
}

/* check if a vertex of a convex hull is on a face or an edge of the convex hull of the other vertices. */
bool check_convexhull_redundant(zPH3D *ch)
{
  zVec3DData data;
  zPH3D ch2;
  bool result = false;
  int i, j;

  for( i=0; i<zPH3DVertNum(ch); i++ ){
    zVec3DDataInitList( &data );
    for( j=0; j<zPH3DVertNum(ch); j++ )
      if( j != i ) zVec3DDataAdd( &data, zPH3DVert(ch,j) );
    if( zVec3DDataConvexHull( &data, &ch2 ) ){
      for( j=0; j<zPH3DFaceNum(&ch2); j++ )
        if( zVec3DOrient3D( zTri3DVert(zPH3DFace(&ch2,j),0), zTri3DVert(zPH3DFace(&ch2,j),1), zTri3DVert(zPH3DFace(&ch2,j),2), zPH3DVert(ch,i) ) > 0 ) break;
      if( j == zPH3DFaceNum(&ch2) ) result = true; /* the i-th vertex is redundant */
    }
    zPH3DDestroy( &ch2 );
    zVec3DDataDestroy( &data );
    if( result ) break;
  }
  return result;
}

/* exact convex hulls of various sets of points, scales and positions */
void assert_convexhull3d(void)
{
  zVec3DData data;
  zPH3D ch;
  double scale[] = { 1.0e-6, 1, 1.0e6 }, offset[] = { 0, 1.0e3 };
  int type, i, j, k, testnum = 3;
  bool result = true, result_redundant = true;

  for( type=0; type<TYPE_NUM; type++ )
    for( i=0; i<3; i++ )
      for( j=0; j<2; j++ )
        for( k=0; k<testnum; k++ ){
          create_points( &data, type, scale[i], offset[j], k > 0 );
          if( !check_convexhull( &data, zVec3DDataConvexHull( &data, &ch ), 0 ) ) result = false;
          if( k == 0 && type < PLANAR && check_convexhull_redundant( &ch ) ) result_redundant = false;
          zPH3DDestroy( &ch );
          zVec3DDataDestroy( &data );
        }
  zAssert( zVec3DDataConvexHull (exact), result );
  zAssert( zVec3DDataConvexHull (no vertex on faces and edges), result_redundant );
}

/* convex hulls with a tolerance of nearly coplanar points */
void assert_convexhull3d_tol(void)
{
  zVec3DData data;
  zPH3D ch;
  double tol = 1.0e-9;
  int i, testnum = 10;
  bool result = true, result_grid = true, result_norm = true;

  for( i=0; i<testnum; i++ ){
    create_points( &data, GRID, 1, 0, true ); /* the corners of a cube are left */
    if( !check_convexhull( &data, zVec3DDataConvexHullSimplify( &data, &ch, tol ), tol ) ) result = false;
    if( zPH3DVertNum(&ch) != 8 || zPH3DFaceNum(&ch) != 12 ) result_grid = false;
    if( !check_nonzero_norm( &ch ) ) result_norm = false;
    zPH3DDestroy( &ch );
    zVec3DDataDestroy( &data );
    create_points( &data, i % 2 ? BOX_EDGES : CUBE_SURFACE, 1, 1.0e3, true );
    if( !check_convexhull( &data, zVec3DDataConvexHullSimplify( &data, &ch, tol ), tol ) ) result = false;
    if( !check_nonzero_norm( &ch ) ) result_norm = false;
    zPH3DDestroy( &ch );
    zVec3DDataDestroy( &data );
  }
  zAssert( zVec3DDataConvexHullSimplify, result );
  zAssert( zVec3DDataConvexHullSimplify (rotated grid), result_grid );
  zAssert( zVec3DDataConvexHullSimplify (no zero normal vector), result_norm );
}

/* convex hulls with a tolerance of exactly coplanar points */
void assert_convexhull3d_tol_planar(void)
{
  zVec3DData data;
  zPH3D ch;
  double tol = 1.0e-2;
  int i, testnum = 10;
  bool result = true, result_reduce = true;

  for( i=0; i<testnum; i++ ){
    create_points( &data, i % 2 ? PLANAR : PLANAR_CIRCLE, 1, 0, false );
    if( !check_convexhull( &data, zVec3DDataConvexHullSimplify( &data, &ch, tol ), tol ) ) result = false;
    /* a planar convex hull consists of triangles facing both sides */
    if( zPH3DFaceNum(&ch) != 2 * ( zPH3DVertNum(&ch) - 2 ) ) result = false;
    if( i % 2 == 0 && zPH3DVertNum(&ch) > N / 4 ) result_reduce = false;
    zPH3DDestroy( &ch );
    zVec3DDataDestroy( &data );
  }
  zAssert( zVec3DDataConvexHullSimplify (coplanar points), result );
  zAssert( zVec3DDataConvexHullSimplify (coplanar points on a circle), result_reduce );
}

/* degenerate sets of points */
void assert_convexhull3d_degenerate(void)
{
  zVec3DData data;
  zPH3D ch;
  zVec3D p;
  int i;
  bool result_point, result_line, result_triangle;

  zVec3DDataInitList( &data );
  zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  for( i=0; i<10; i++ ) zVec3DDataAdd( &data, &p );
  result_point = zVec3DDataConvexHull( &data, &ch ) == NULL;
  zPH3DDestroy( &ch );
  zVec3DDataDestroy( &data );
  zVec3DDataInitList( &data );
  for( i=0; i<10; i++ ){
    zVec3DCreate( &p, i, 2*i, -3*i );
    zVec3DDataAdd( &data, &p );
  }
  result_line = zVec3DDataConvexHull( &data, &ch ) == NULL;
  zPH3DDestroy( &ch );
  zVec3DDataDestroy( &data );
  zVec3DDataInitList( &data );
  for( i=0; i<3; i++ ){
    zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DDataAdd( &data, &p );
  }
  result_triangle = zVec3DDataConvexHull( &data, &ch ) && zPH3DVertNum(&ch) == 3 && zPH3DFaceNum(&ch) == 2;
  zPH3DDestroy( &ch );
  zVec3DDataDestroy( &data );
  zAssert( zVec3DDataConvexHull (coincident points), result_point );
  zAssert( zVec3DDataConvexHull (collinear points), result_line );
  zAssert( zVec3DDataConvexHull (three points), result_triangle );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_convexhull3d();
  assert_convexhull3d_tol();
  assert_convexhull3d_tol_planar();
  assert_convexhull3d_degenerate();
  return EXIT_SUCCESS;
}
