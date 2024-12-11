#include <zeo/zeo.h>

void vec_create_rand(zVec3DData *data, int n)
{
  zVec3D v;
  int i;
  FILE *fp;

  fp = fopen( "src", "w" );
  zVec3DDataRewind( data );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zVec3DValueNLFPrint( fp, &v );
    zVec3DDataAdd( data, &v );
  }
  fclose( fp );
}

void verify(zVec3DData *data, zAABox3D *bb, zVec3D **vp)
{
  int i;
  bool ret;
  zBox3D box;
  zVec3D vert, *v;
  FILE *fp;

  zAABox3DToBox3D( bb, &box );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    if( ( ret = zBox3DPointIsInside(&box,v,zTOL) ) == false ){
      zVec3DValueNLFPrint( stderr, v );
      eprintf( "D=%.15g\n", zBox3DDistFromPoint(&box,v) );
    }
  }
  /* points on the walls */
  fp = fopen( "bd", "w" );
  for( i=0; i<6; i++ )
    zVec3DValueNLFPrint( fp, vp[i] );
  fclose( fp );
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

#define N 1000

int main(int argc, char *argv[])
{
  zAABox3D bb;
  zVec3DData data;
  zVec3D *vp[6];

  zRandInit();
#if 1
  zVec3DDataInitArray( &data, N );
#else
  zVec3DDataInitList( &data );
#endif
  vec_create_rand( &data, N );
  zVec3DDataAABB( &data, &bb, vp );
  verify( &data, &bb, vp );
  zVec3DDataDestroy( &data );
  return 0;
}
