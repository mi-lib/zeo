#include <zeo/zeo.h>

#define N 1000

/* 2D bounding ball (disk) */

bool generate_points2D(zVec2DArray *array, int n, double radius)
{
  int i;

  zVec2DArrayAlloc( array, n );
  if( zArraySize(array) == 0 ) return false;
  for( i=0; i<n; i++ )
    zVec2DCreatePolar( zArrayElem(array,i), zRandF(-radius,radius), zRandF(-zPI,zPI) );
  return true;
}

bool test_boundingball2D(zDisk2D *bb, zVec2D *vp[], int n, zVec2DArray *array)
{
  int i;
  double r1, r;
  int count;
  int errcode = 0;

  r = zDisk2DRadius(bb);
  for( count=0, i=0; i<zArraySize(array); i++ ){
    r1 = zVec2DDist( zDisk2DCenter(bb), zArrayElem(array,i) );
    if( r1 > r + zTOL ){
      eprintf( "a point outside of the bounding ball found. (%.10g > %.10g)\n", r1, r );
      errcode = 1;
    }
    if( zIsTol( r1-r, zTOL*2 ) ) count++;
  }
  if( count <= 1 ){
    eprintf( "bounding ball not correctly built due to the lack of points." );
    errcode = 2;
  }
  if( count != n ){
    eprintf( "mismatch of the number of points." );
    errcode = 3;
  }
  for( i=0; i<n; i++ ){
    r1 = zVec2DDist( zDisk2DCenter(bb), vp[i] );
    if( !zIsTol( r1 - r, zTOL*2 ) ){
      eprintf( "extrema points not on the bounding ball (error = %g).\n", r1 - r );
      errcode = 4;
    }
  }
  eprintf( "error code = %d ... ", errcode );
  return errcode == 0;
}

void assert_boundingball2D(void)
{
  zVec2DArray array;
  zVec2DList plist;
  zVec2DData data;
  zDisk2D bb;
  zVec2D *vp[3]; /* vertices on the disk up to three. */
  int n;

  generate_points2D( &array, N, 5 );
  zVec2DArrayToList( &array, &plist );

  zVec2DDataAssignArray( &data, &array );
  n = zVec2DDataBoundingBall( &data, &bb, vp );
  zAssert( zVec2DDataBoundingBall (array), test_boundingball2D( &bb, vp, n, &array ) );
  zVec2DDataDestroy( &data );

  zVec2DDataAssignList( &data, &plist );
  n = zVec2DDataBoundingBall( &data, &bb, vp );
  zAssert( zVec2DDataBoundingBall (list), test_boundingball2D( &bb, vp, n, &array ) );
  zVec2DDataDestroy( &data );

  zVec2DListDestroy( &plist );
  zArrayFree( &array );
}

int cmp_dist2D(void *v1, void *v2, void *dummy)
{
  double d;

  d = zVec2DSqrNorm((zVec2D *)v1) - zVec2DSqrNorm((zVec2D *)v2);
  return d < 0 ? -1 : ( d > 0 ? 1 : 0 );
}

void assert_boundingball2D_sorted(void)
{
  zVec2DArray array;
  zVec2DData data;
  zDisk2D bb, bb_rec;
  zVec2D *vp[3];
  int n;
  bool result;

  generate_points2D( &array, N, 5 );
  zQuickSort( zArrayBuf(&array), zArraySize(&array), sizeof(zVec2D), cmp_dist2D, NULL );
  zVec2DDataAssignArray( &data, &array );
  n = zVec2DDataBoundingBall( &data, &bb, vp );
  zAssert( zVec2DDataBoundingBall (points sorted by distance), test_boundingball2D( &bb, vp, n, &array ) );
  n = zVec2DDataBoundingBallRecursive( &data, &bb_rec, vp );
  result = test_boundingball2D( &bb_rec, vp, n, &array );
  zAssert( zVec2DDataBoundingBallRecursive (points sorted by distance), result &&
    zIsTol( zDisk2DRadius(&bb_rec) - zDisk2DRadius(&bb), zTOL ) );
  zVec2DDataDestroy( &data );
  zArrayFree( &array );
}

/* check if all points are in a disk. */
bool check_bounding2D(zDisk2D *bb, zVec2DArray *array)
{
  int i;

  for( i=0; i<zArraySize(array); i++ )
    if( zVec2DDist( zDisk2DCenter(bb), zArrayElem(array,i) ) > zDisk2DRadius(bb) + zTOL ) return false;
  return true;
}

/* compare the recursive and non-recursive implementations for many sets of points. */
void assert_boundingball2D_recursive(void)
{
  zVec2DArray array;
  zVec2DData data;
  zDisk2D bb, bb_rec;
  int i;
  bool result = true;
  const int testnum = 1000; /* the failure of the recursive version used to be rare in 2D */

  for( i=0; i<testnum; i++ ){
    generate_points2D( &array, N, 5 );
    zVec2DDataAssignArray( &data, &array );
    zVec2DDataBoundingBall( &data, &bb, NULL );
    zVec2DDataBoundingBallRecursive( &data, &bb_rec, NULL );
    if( !check_bounding2D( &bb, &array ) || !check_bounding2D( &bb_rec, &array ) ||
        !zIsTol( zDisk2DRadius(&bb_rec) - zDisk2DRadius(&bb), zTOL ) ) result = false;
    zVec2DDataDestroy( &data );
    zArrayFree( &array );
  }
  zAssert( zVec2DDataBoundingBall + zVec2DDataBoundingBallRecursive (random sets), result );
}

/* 3D bounding ball (sphere) */

bool generate_points3D(zVec3DArray *array, int n, double radius)
{
  int i;

  zVec3DArrayAlloc( array, n );
  if( zArraySize(array) == 0 ) return false;
  for( i=0; i<n; i++ )
    zVec3DCreatePolar( zArrayElem(array,i), zRandF(-radius,radius), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
  return true;
}

bool test_boundingball3D(zSphere3D *bb, zVec3D *vp[], int n, zVec3DArray *array)
{
  int i;
  double r1, r;
  int count;
  int errcode = 0;

  r = zSphere3DRadius(bb);
  for( count=0, i=0; i<zArraySize(array); i++ ){
    r1 = zVec3DDist( zSphere3DCenter(bb), zArrayElem(array,i) );
    if( r1 > r + zTOL ){
      eprintf( "a point outside of the bounding ball found. (%.10g > %.10g)\n", r1, r );
      errcode = 1;
    }
    if( zIsTol( r1-r, zTOL*2 ) ) count++;
  }
  if( count <= 1 ){
    eprintf( "bounding ball not correctly built due to the lack of points." );
    errcode = 2;
  }
  if( count != n ){
    eprintf( "mismatch of the number of points." );
    errcode = 3;
  }
  for( i=0; i<n; i++ ){
    r1 = zVec3DDist( zSphere3DCenter(bb), vp[i] );
    if( !zIsTol( r1 - r, zTOL*2 ) ){
      eprintf( "extrema points not on the bounding ball (error = %g).\n", r1 - r );
      errcode = 4;
    }
  }
  eprintf( "error code = %d ... ", errcode );
  return errcode == 0;
}

void assert_boundingball3D(void)
{
  zVec3DArray array;
  zVec3DList plist;
  zVec3DData data;
  zSphere3D bb;
  zVec3D *vp[4]; /* vertices on the sphere up to four. */
  int n;

  generate_points3D( &array, N, 5 );
  zVec3DArrayToList( &array, &plist );

  zVec3DDataAssignArray( &data, &array );
  n = zVec3DDataBoundingBall( &data, &bb, vp );
  zAssert( zVec3DDataBoundingBall (array), test_boundingball3D( &bb, vp, n, &array ) );
  zVec3DDataDestroy( &data );

  zVec3DDataAssignList( &data, &plist );
  n = zVec3DDataBoundingBall( &data, &bb, vp );
  zAssert( zVec3DDataBoundingBall (list), test_boundingball3D( &bb, vp, n, &array ) );
  zVec3DDataDestroy( &data );

  zVec3DListDestroy( &plist );
  zArrayFree( &array );
}

int cmp_dist3D(void *v1, void *v2, void *dummy)
{
  double d;

  d = zVec3DSqrNorm((zVec3D *)v1) - zVec3DSqrNorm((zVec3D *)v2);
  return d < 0 ? -1 : ( d > 0 ? 1 : 0 );
}

void assert_boundingball3D_sorted(void)
{
  zVec3DArray array;
  zVec3DData data;
  zSphere3D bb, bb_rec;
  zVec3D *vp[4];
  int n;
  bool result;

  generate_points3D( &array, N, 5 );
  zQuickSort( zArrayBuf(&array), zArraySize(&array), sizeof(zVec3D), cmp_dist3D, NULL );
  zVec3DDataAssignArray( &data, &array );
  n = zVec3DDataBoundingBall( &data, &bb, vp );
  zAssert( zVec3DDataBoundingBall (points sorted by distance), test_boundingball3D( &bb, vp, n, &array ) );
  n = zVec3DDataBoundingBallRecursive( &data, &bb_rec, vp );
  result = test_boundingball3D( &bb_rec, vp, n, &array );
  zAssert( zVec3DDataBoundingBallRecursive (points sorted by distance), result &&
    zIsTol( zSphere3DRadius(&bb_rec) - zSphere3DRadius(&bb), zTOL ) );
  zVec3DDataDestroy( &data );
  zArrayFree( &array );
}

/* check if all points are in a ball. */
bool check_bounding3D(zSphere3D *bb, zVec3DArray *array)
{
  int i;

  for( i=0; i<zArraySize(array); i++ )
    if( zVec3DDist( zSphere3DCenter(bb), zArrayElem(array,i) ) > zSphere3DRadius(bb) + zTOL ) return false;
  return true;
}

/* compare the recursive and non-recursive implementations for many sets of points. */
void assert_boundingball3D_recursive(void)
{
  zVec3DArray array;
  zVec3DData data;
  zSphere3D bb, bb_rec;
  int i;
  bool result = true;
  const int testnum = 100;

  for( i=0; i<testnum; i++ ){
    generate_points3D( &array, N, 5 );
    zVec3DDataAssignArray( &data, &array );
    zVec3DDataBoundingBall( &data, &bb, NULL );
    zVec3DDataBoundingBallRecursive( &data, &bb_rec, NULL );
    if( !check_bounding3D( &bb, &array ) || !check_bounding3D( &bb_rec, &array ) ||
        !zIsTol( zSphere3DRadius(&bb_rec) - zSphere3DRadius(&bb), zTOL ) ) result = false;
    zVec3DDataDestroy( &data );
    zArrayFree( &array );
  }
  zAssert( zVec3DDataBoundingBall + zVec3DDataBoundingBallRecursive (random sets), result );
}

int main(void)
{
  zRandInit();
  assert_boundingball2D();
  assert_boundingball2D_sorted();
  assert_boundingball2D_recursive();
  assert_boundingball3D();
  assert_boundingball3D_sorted();
  assert_boundingball3D_recursive();
  return 0;
}
