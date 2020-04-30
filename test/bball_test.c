#include <zeo/zeo.h>

#define N 2000

int main(void)
{
  zSphere3D bb;
  zVec3D v[N];
  zVec3D *vp[4]; /* vertices on the sphere up to four. */
  register int i, n;
  double r1, r;
  int count;
  int errcode = 0;

  zRandInit();
  for( i=0; i<N; i++ )
    zVec3DCreatePolar( &v[i], zRandF(-5,5), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
  n = zBBall( &bb, v, N, vp );
  r = zSphere3DRadius(&bb);

  for( count=0, i=0; i<N; i++ ){
    r1 = zVec3DDist( zSphere3DCenter(&bb), &v[i] );
    if( r1 > r ) errcode = 1;
    if( zIsTol( r1-r, zTOL*2 ) ) count++;
  }
  if( count <= 1 ) errcode = 2;
  if( count != n ) errcode = 3;

  for( i=0; i<n; i++ ){
    r1 = zVec3DDist( zSphere3DCenter(&bb), vp[i] );
    if( !zIsTol( r1-r, zTOL*2 ) ) errcode = 4;
  }
  printf( "error code = %d\n", errcode );
  zAssert( zBBall, errcode == 0 );
  return 0;
}
