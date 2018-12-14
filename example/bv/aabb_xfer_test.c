#include <zeo/zeo.h>

#define N 100
zVec3D v[N];

void frame_create_rand(zFrame3D *f)
{
  zVec3D aa;

  zVec3DCreate( zFrame3DPos(f), zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zVec3DCreate( &aa, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DAA( zFrame3DAtt(f), &aa );
}

void veclist_create_rand(zFrame3D *f)
{
  register int i;
  FILE *fp;
  zVec3D vert;

  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v[i], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zXfer3D( f, &v[i], &vert );
    zVec3DDataNLFWrite( fp, &vert );
  }
  fclose( fp );
}

void verify(zAABox3D *bb, zFrame3D *f)
{
  register int i;
  bool ret;
  zBox3D box;
  zVec3D vert;
  FILE *fp;

  eprintf( "++verify+++\n" );
  zAABox3DToBox3D( bb, &box );
  for( i=0; i<N; i++ ){
    zXfer3D( f, &v[i], &vert );
    if( ( ret = zBox3DPointIsInside(&box,&vert,true) ) == false ){
      zVec3DDataNLFWrite( stderr, &vert );
      eprintf( "D=%.15g\n", zBox3DPointDist(&box,&vert) );
    }
  }
  /* AABB */
  fp = fopen( "aabb", "w" );
  for( i=0; i<4; i++ )
    zVec3DDataNLFWrite( fp, zBox3DVert(&box,i,&vert) );
  zVec3DDataNLFWrite( fp, zBox3DVert(&box,0,&vert) );
  fprintf( fp, "\n" );
  for( i=4; i<8; i++ )
    zVec3DDataNLFWrite( fp, zBox3DVert(&box,i,&vert) );
  zVec3DDataNLFWrite( fp, zBox3DVert(&box,4,&vert) );
  fprintf( fp, "\n" );
  for( i=0; i<4; i++ ){
    zVec3DDataNLFWrite( fp, zBox3DVert(&box,i,&vert) );
    zVec3DDataNLFWrite( fp, zBox3DVert(&box,i+4,&vert) );
    fprintf( fp, "\n" );
  }
  fclose( fp );
}

int main(void)
{
  zAABox3D bb;
  zFrame3D f;

  zRandInit();
  frame_create_rand( &f );
  veclist_create_rand( &f );
  zAABBXfer( &bb, v, N, &f );
  verify( &bb, &f );
  return 0;
}
