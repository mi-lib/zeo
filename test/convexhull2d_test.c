#include <zeo/zeo.h>

#define N 100

/* create a random frame whose position is at a distance of the order of scale. */
void create_frame(zFrame3D *frame, double scale)
{
  zFrame3DFromPosAA( frame,
    scale*zRandF(-1,1), scale*zRandF(-1,1), scale*zRandF(-1,1),
    zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

/* check if a convex hull consists of the vertices of a polygon. */
bool check_convexhull2d(zLoop3D *ch, zVec3D vert[], int n, double tol)
{
  zLoop3DCell *cp;
  int i;

  if( zListSize(ch) != n ) return false;
  zListForEach( ch, cp ){
    for( i=0; i<n; i++ )
      if( zVec3DDist( cp->data, &vert[i] ) <= tol ) break;
    if( i == n ) return false;
  }
  return true;
}

/* regular polygons with interior points on a plane of various sizes */
void assert_convexhull2d_polygon(void)
{
  zVec3D p[100];
  zVec3DData data;
  zLoop3D ch;
  zFrame3D frame;
  double scale, r;
  int i, j, n, num;
  bool result = true;

  for( i=0; i<N; i++ ){
    scale = pow( 10, zRandF(-6,6) );
    create_frame( &frame, scale );
    n = zRandI( 3, 12 );
    num = zRandI( n, 100 );
    for( j=0; j<num; j++ ){
      r = j < n ? scale : scale * zRandF(0,0.5) ; /* vertices and interior points */
      zVec3DCreatePolar( &p[j], r, 0.5*zPI, zPIx2*( j < n ? (double)j/n : zRandF(0,1) ) );
      zXform3DDRC( &frame, &p[j] );
    }
    zVec3DDataAssignArrayDirect( &data, p, num );
    if( !zVec3DDataConvexHull2D( &data, &ch ) || !check_convexhull2d( &ch, p, n, zTOL*scale ) )
      result = false;
    zLoop3DDestroy( &ch );
    zVec3DDataDestroy( &data );
  }
  zAssert( zVec3DDataConvexHull2D (polygons of various sizes), result );
}

/* degenerate cases: coincident points and collinear points with rounding errors */
void assert_convexhull2d_degenerate(void)
{
  zVec3D p[10], d;
  zVec3DData data;
  zLoop3D ch;
  double scale;
  int i, j;
  bool result_point = true, result_line = true;

  zEchoOff();
  for( i=0; i<N; i++ ){
    scale = pow( 10, zRandF(-6,6) );
    zVec3DCreate( &p[0], scale*zRandF(-1,1), scale*zRandF(-1,1), scale*zRandF(-1,1) );
    for( j=1; j<10; j++ ) zVec3DCopy( &p[0], &p[j] );
    zVec3DDataAssignArrayDirect( &data, p, 10 );
    if( zVec3DDataConvexHull2D( &data, &ch ) ) result_point = false;
    zVec3DDataDestroy( &data );
    zVec3DCreate( &d, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DMulDRC( &d, scale );
    for( j=1; j<10; j++ ) zVec3DCat( &p[0], zRandF(-1,1), &d, &p[j] );
    zVec3DDataAssignArrayDirect( &data, p, 10 );
    if( zVec3DDataConvexHull2D( &data, &ch ) ) result_line = false;
    zVec3DDataDestroy( &data );
  }
  zEchoOn();
  zAssert( zVec3DDataConvexHull2D (coincident points), result_point );
  zAssert( zVec3DDataConvexHull2D (collinear points), result_line );
}

/* check if a 2D polygon is the convex hull of a set of 2D points:
 * - every vertex is a point of the set,
 * - the vertices are strictly convex in counterclockwise order,
 * - every point is inside of or on the polygon,
 * - the faces fan out from the first vertex. */
bool check_convexhull_pg(zVec2DData *data, zPG2D *ch)
{
  zVec2D *v, *p;
  int i, n;

  if( !ch || ( n = zPG2DVertNum(ch) ) < 3 || zPG2DFaceNum(ch) != n - 2 ) return false;
  for( i=0; i<n; i++ ){
    zVec2DDataRewind( data );
    while( ( p = zVec2DDataFetch( data ) ) )
      if( p->c.x == zPG2DVert(ch,i)->c.x && p->c.y == zPG2DVert(ch,i)->c.y ) break;
    if( !p ) return false;
    if( zVec2DOrient2D( zPG2DVert(ch,i), zPG2DVert(ch,(i+1)%n), zPG2DVert(ch,(i+2)%n) ) <= 0 ) return false;
    zVec2DDataRewind( data );
    while( ( p = zVec2DDataFetch( data ) ) )
      if( zVec2DOrient2D( zPG2DVert(ch,i), zPG2DVert(ch,(i+1)%n), p ) < 0 ) return false;
  }
  for( i=0; i<n-2; i++ ){
    v = zPG2DVert(ch,0);
    if( zPG2DFaceVert(ch,i,0) != v || zPG2DFaceVert(ch,i,1) != v+i+1 || zPG2DFaceVert(ch,i,2) != v+i+2 ) return false;
  }
  return true;
}

/* types of 2D point sets */
enum{ PG_RANDOM = 0, PG_CIRCLE, PG_GRID, PG_DUPLICATE, PG_TYPE_NUM };

/* generate a set of 2D points, which are scaled by a power of two, translated and rotated. */
void create_points_pg(zVec2DData *data, int type, int n, bool rotate)
{
  zVec2D p, q[10];
  zMat2D r;
  double scale, th;
  int i, k, m = 10;

  scale = ldexp( 1.0, zRandI(-40,40) );
  if( rotate ) zMat2DCreateAngle( &r, zRandF(-zPI,zPI) ); else zMat2DIdent( &r );
  for( k=0; k<10; k++ ) zVec2DCreate( &q[k], zRandF(-1,1), zRandF(-1,1) );
  zVec2DDataInitArray( data, n );
  for( i=0; i<n; i++ ){
    switch( type ){
    case PG_RANDOM:    zVec2DCreate( &p, zRandF(-1,1), zRandF(-1,1) ); break;
    case PG_CIRCLE:    th = zPIx2 * i / n; zVec2DCreate( &p, cos(th), sin(th) ); break;
    case PG_GRID:      zVec2DCreate( &p, i%m, (i/m)%m ); break;
    case PG_DUPLICATE: zVec2DCopy( &q[i%10], &p ); break;
    default: ;
    }
    zMulMat2DVec2DDRC( &r, &p );
    zVec2DCreate( &p, ( p.c.x + 3 ) * scale, ( p.c.y - 5 ) * scale );
    zVec2DDataAdd( data, &p );
  }
}

/* convex hulls of various sets of 2D points */
void assert_convexhull_pg(void)
{
  zVec2DData data;
  zPG2D ch;
  int type, i, testnum = 100;
  bool result = true, result_grid = true;

  for( type=0; type<PG_TYPE_NUM; type++ )
    for( i=0; i<testnum; i++ ){
      create_points_pg( &data, type, 200, i % 2 == 1 );
      if( !check_convexhull_pg( &data, zVec2DDataConvexHull( &data, &ch ) ) ) result = false;
      if( type == PG_GRID && i % 2 == 0 && zPG2DVertNum(&ch) != 4 ) result_grid = false;
      zPG2DDestroy( &ch );
      zVec2DDataDestroy( &data );
    }
  zAssert( zVec2DDataConvexHull, result );
  zAssert( zVec2DDataConvexHull (grid points), result_grid );
}

/* degenerate sets of 2D points */
void assert_convexhull_pg_degenerate(void)
{
  zVec2DData data;
  zVec2D p;
  zPG2D ch;
  int i;
  bool result_empty, result_point, result_line, result_triangle;

  zEchoOff();
  zVec2DDataInitArray( &data, 10 );
  result_empty = zVec2DDataConvexHull( &data, &ch ) == NULL;
  for( i=0; i<10; i++ ) zVec2DDataAdd( &data, zVec2DCreate( &p, 1.5, -2.5 ) );
  result_point = zVec2DDataConvexHull( &data, &ch ) == NULL;
  zVec2DDataDestroy( &data );
  zVec2DDataInitArray( &data, 10 );
  for( i=0; i<10; i++ ) zVec2DDataAdd( &data, zVec2DCreate( &p, ( i % 5 ) * 3 - 1, ( i % 5 ) * 2 + 1 ) );
  result_line = zVec2DDataConvexHull( &data, &ch ) == NULL;
  zVec2DDataDestroy( &data );
  zEchoOn();
  zVec2DDataInitArray( &data, 3 );
  zVec2DDataAdd( &data, zVec2DCreate( &p, 0, 0 ) );
  zVec2DDataAdd( &data, zVec2DCreate( &p, 1, 1 ) );
  zVec2DDataAdd( &data, zVec2DCreate( &p, 1, 0 ) );
  result_triangle = check_convexhull_pg( &data, zVec2DDataConvexHull( &data, &ch ) ) && zPG2DVertNum(&ch) == 3;
  zPG2DDestroy( &ch );
  zVec2DDataDestroy( &data );
  zAssert( zVec2DDataConvexHull (empty set), result_empty );
  zAssert( zVec2DDataConvexHull (coincident points), result_point );
  zAssert( zVec2DDataConvexHull (collinear points), result_line );
  zAssert( zVec2DDataConvexHull (three points), result_triangle );
}

int main(void)
{
  zRandInit();
  assert_convexhull2d_polygon();
  assert_convexhull2d_degenerate();
  assert_convexhull_pg();
  assert_convexhull_pg_degenerate();
  return EXIT_SUCCESS;
}
