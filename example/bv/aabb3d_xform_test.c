#include <zeo/zeo.h>

void frame_create_rand(zFrame3D *f)
{
  zVec3D aa;

  zVec3DCreate( zFrame3DPos(f), zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zVec3DCreate( &aa, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DFromAA( zFrame3DAtt(f), &aa );
}

void vec_create_rand(zVec3DData *data, int n, zFrame3D *f)
{
  zVec3D v, vert;
  int i;
  FILE *fp;

  zVec3DDataRewind( data );
  fp = fopen( "src", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zVec3DDataAdd( data, &v );
    zXform3D( f, &v, &vert );
    zVec3DValueNLFPrint( fp, &vert );
  }
  fclose( fp );
}

void verify(zVec3DData *data, zAABox3D *bb, zFrame3D *f)
{
  int i;
  bool ret;
  zBox3D box;
  zVec3D *v, vert;
  FILE *fp;

  zAABox3DToBox3D( bb, &box );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zXform3D( f, v, &vert );
    if( ( ret = zBox3DPointIsInside(&box,&vert,true) ) == false ){
      zVec3DValueNLFPrint( stderr, &vert );
      eprintf( "D=%.15g\n", zBox3DDistFromPoint(&box,&vert) );
    }
  }
  /* AABB */
  fp = fopen( "aabb", "w" );
  for( i=0; i<4; i++ )
    zVec3DValueNLFPrint( fp, zBox3DVert(&box,i,&vert) );
  zVec3DValueNLFPrint( fp, zBox3DVert(&box,0,&vert) );
  fprintf( fp, "\n" );
  for( i=4; i<8; i++ )
    zVec3DValueNLFPrint( fp, zBox3DVert(&box,i,&vert) );
  zVec3DValueNLFPrint( fp, zBox3DVert(&box,4,&vert) );
  fprintf( fp, "\n" );
  for( i=0; i<4; i++ ){
    zVec3DValueNLFPrint( fp, zBox3DVert(&box,i,&vert) );
    zVec3DValueNLFPrint( fp, zBox3DVert(&box,i+4,&vert) );
    fprintf( fp, "\n" );
  }
  fclose( fp );
}

#define N 100

int main(void)
{
  zVec3DData data;
  zAABox3D bb;
  zFrame3D f;

  zRandInit();
  zVec3DDataInitArray( &data, N );
  frame_create_rand( &f );
  vec_create_rand( &data, N, &f );
  zVec3DDataAABBXform( &data, &bb, &f );
  verify( &data, &bb, &f );
  zVec3DDataDestroy( &data );
  return 0;
}
