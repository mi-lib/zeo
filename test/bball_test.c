#include <zeo/zeo.h>

#define N 2000

int main(void)
{
  zSphere3D bb;
  zVec3D *vp[4]; /* vertices on the sphere up to four. */
  register int i, n;
  double r1, r;
  int count;
  int errcode = 0;
  zVec3DArray arr;

  zRandInit();
  zArrayAlloc( &arr, zVec3D, N );
  for( i=0; i<N; i++ )
    zVec3DCreatePolar( zArrayElem(&arr,i), zRandF(-5,5), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
  n = zBBall3D( &bb, &arr, vp );
  r = zSphere3DRadius(&bb);

  for( count=0, i=0; i<N; i++ ){
    r1 = zVec3DDist( zSphere3DCenter(&bb), zArrayElem(&arr,i) );
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
    r1 = zVec3DDist( zSphere3DCenter(&bb), vp[i] );
    if( !zIsTol( r1-r, zTOL*2 ) ){
      eprintf( "extrema points not on the bounding ball.\n" );
      errcode = 4;
    }
  }
  printf( "error code = %d\n", errcode );
  zAssert( zBBall3D, errcode == 0 );
  return 0;
}
