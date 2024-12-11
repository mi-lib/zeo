#include <zeo/zeo.h>

void vec_create_rand(zVec2DData *data, int n)
{
  zVec2D v;
  int i;
  FILE *fp;

  zVec2DDataRewind( data );
  fp = fopen( "src", "w" );
  for( i=0; i<n; i++ ){
    zVec2DCreate( &v, zRandF(-5,5), zRandF(-5,5) );
    zVec2DDataAdd( data, &v );
    zVec2DValueNLFPrint( fp, &v );
  }
  fclose( fp );
}

void verify(zAABox2D *bb, zVec2D **vp)
{
  int i;
  FILE *fp;

  /* points on the walls */
  fp = fopen( "bd", "w" );
  for( i=0; i<4; i++ )
    zVec2DValueNLFPrint( fp, vp[i] );
  fclose( fp );
  /* AABB */
  fp = fopen( "aabb", "w" );
  zAABox2DValueFPrint( fp, bb );
  fclose( fp );
}

#define N 1000

int main(int argc, char *argv[])
{
  zVec2DData data;
  zAABox2D bb;
  zVec2D *vp[4];

  zRandInit();
  if( argc > 1 && strcmp( argv[1], "list" ) == 0 ){
    zVec2DDataInitList( &data );
  } else{
    zVec2DDataInitArray( &data, N );
  }
  vec_create_rand( &data, N );
  zVec2DDataAABB( &data, &bb, vp );
  verify( &bb, vp );
  zVec2DDataDestroy( &data );
  return 0;
}
