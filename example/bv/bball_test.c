#include <zeo/zeo.h>

#define N 2000
zVec3D v[N];

void veclist_create_rand(void)
{
  register int i;

  for( i=0; i<N; i++ ){
    zVec3DCreatePolar( &v[i], zRandF(-5,5), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    zVec3DDataNLWrite( &v[i] );
  }
}

void verify(zSphere3D *bb, int n, zVec3D *vp[])
{
  register int i;
  double r1, r;
  int count;

  zSphere3DFWrite( stderr, bb );
  eprintf( "++verify+++\n" );
  r = zSphere3DRadius(bb);
  for( count=0, i=0; i<N; i++ ){
    r1 = zVec3DDist(zSphere3DCenter(bb),&v[i]);
    if( r1 > r ) ZRUNWARN( "%f > %f, impossible error, must be a bug", r1, r );
    if( zIsTol( r1-r, zTOL*2 ) ) count++;
  }
  eprintf( "rim points: %d\n", count );
  if( count <= 1 ) ZRUNWARN( "impossible error, must be a bug" );

  eprintf( "points on the sphere: %d\n", n );
  for( i=0; i<n; i++ ){
    r1 = zVec3DDist(zSphere3DCenter(bb),vp[i]);
    eprintf( "r = %.10f\n", r1 );
    if( !zIsTol( r1-r, zTOL*2 ) )
      ZRUNWARN( "%f VS %f, impossible error, must be a bug", r1, r );
  }
}

int main(void)
{
  zSphere3D bb;
  zVec3D *vp[4]; /* vertices on the sphere up to four. */
  int n;

  zRandInit();
  veclist_create_rand();
  n = zBBall( &bb, v, N, vp );
  verify( &bb, n, vp );
  return 0;
}
