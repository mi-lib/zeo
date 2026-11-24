#include <zeo/zeo.h>

#define N 100

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

/* regular polygons with interior points of various sizes, the vertices of which have to be those
 * of the convex hulls */
void assert_convexhull_pg_polygon(void)
{
  zVec2D p[100];
  zVec2DData data;
  zPG2D ch;
  double scale, r;
  int i, j, k, n, num;
  bool result = true;

  for( i=0; i<N; i++ ){
    scale = pow( 10, zRandF(-6,6) );
    n = zRandI( 3, 12 );
    num = zRandI( n, 100 );
    for( j=0; j<num; j++ ){
      r = j < n ? scale : scale * zRandF(0,0.5) ; /* vertices and interior points */
      zVec2DCreatePolar( &p[j], r, zPIx2*( j < n ? (double)j/n : zRandF(0,1) ) );
      zVec2DCreate( &p[j], p[j].c.x + scale, p[j].c.y - 2*scale ); /* translated */
    }
    zVec2DDataAssignArrayDirect( &data, p, num );
    if( !check_convexhull_pg( &data, zVec2DDataConvexHull( &data, &ch ) ) || zPG2DVertNum(&ch) != n )
      result = false;
    else
      for( k=0; k<n; k++ ){
        for( j=0; j<n; j++ )
          if( zPG2DVert(&ch,k)->c.x == p[j].c.x && zPG2DVert(&ch,k)->c.y == p[j].c.y ) break;
        if( j == n ) result = false;
      }
    zPG2DDestroy( &ch );
    zVec2DDataDestroy( &data );
  }
  zAssert( zVec2DDataConvexHull (polygons of various sizes), result );
}

int main(void)
{
  zRandInit();
  assert_convexhull_pg_polygon();
  assert_convexhull_pg();
  assert_convexhull_pg_degenerate();
  return EXIT_SUCCESS;
}
