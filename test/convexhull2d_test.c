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

int main(void)
{
  zRandInit();
  assert_convexhull2d_polygon();
  assert_convexhull2d_degenerate();
  return EXIT_SUCCESS;
}
