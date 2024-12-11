#include <zeo/zeo.h>

bool generate_points(zVec3DArray *array, int n, double radius)
{
  int i;

  zVec3DArrayAlloc( array, n );
  if( zArraySize(array) == 0 ) return false;
  for( i=0; i<n; i++ )
    zVec3DCreatePolar( zArrayElem(array,i), zRandF(-radius,radius), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
  return true;
}

bool test_boundingball(zSphere3D *bb, zVec3D *vp[], int n, zVec3DArray *array)
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

#define N 1000

int main(void)
{
  zVec3DArray array;
  zVec3DList plist;
  zVec3DData data;
  zSphere3D bb;
  zVec3D *vp[4]; /* vertices on the sphere up to four. */
  int n;

  zRandInit();
  generate_points( &array, N, 5 );
  zVec3DArrayToList( &array, &plist );

  zVec3DDataAssignArray( &data, &array );
  n = zVec3DDataBoundingBall( &data, &bb, vp );
  zAssert( zVec3DDataBoundingBall (array), test_boundingball( &bb, vp, n, &array ) );
  zVec3DDataDestroy( &data );

  zVec3DDataAssignList( &data, &plist );
  n = zVec3DDataBoundingBall( &data, &bb, vp );
  zAssert( zBoundingBall3D (list), test_boundingball( &bb, vp, n, &array ) );
  zVec3DDataDestroy( &data );

  zVec3DListDestroy( &plist );
  zArrayFree( &array );
  return 0;
}
