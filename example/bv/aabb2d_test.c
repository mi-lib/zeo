#include <zeo/zeo.h>

void vec_create_rand(zVec2D v[], int n)
{
  register int i;
  FILE *fp;

  fp = fopen( "src", "w" );
  for( i=0; i<n; i++ ){
    zVec2DCreate( &v[i], zRandF(-5,5), zRandF(-5,5) );
    zVec2DValueNLFPrint( fp, &v[i] );
  }
  fclose( fp );
}

void vec_list_create_rand(zVec2DList *pl, int n)
{
  register int i;
  FILE *fp;
  zVec2D v;

  zListInit( pl );
  fp = fopen( "src", "w" );
  for( i=0; i<n; i++ ){
    zVec2DCreate( &v, zRandF(-5,5), zRandF(-5,5) );
    zVec2DListAdd( pl, &v );
    zVec2DValueNLFPrint( fp, &v );
  }
  fclose( fp );
}

void verify(zAABox2D *bb, zVec2D **vp)
{
  register int i;
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

void verify_list(zAABox2D *bb, zVec2DListCell **vpl)
{
  register int i;
  FILE *fp;

  /* points on the walls */
  fp = fopen( "bd", "w" );
  for( i=0; i<4; i++ )
    zVec2DValueNLFPrint( fp, vpl[i]->data );
  fclose( fp );
  /* AABB */
  fp = fopen( "aabb", "w" );
  zAABox2DValueFPrint( fp, bb );
  fclose( fp );
}

#define N 1000

int main(int argc, char *argv[])
{
  zAABox2D bb;
  zVec2D v[N], *vp[4];
  zVec2DList pl;
  zVec2DListCell *vpl[4];

  zRandInit();
  if( argc > 1 && strcmp( argv[1], "list" ) == 0 ){
    vec_list_create_rand( &pl, N );
    zAABB2DPL( &bb, &pl, vpl );
    verify_list( &bb, vpl );
    zVec2DListDestroy( &pl );
  } else{
    vec_create_rand( v, N );
    zAABB2D( &bb, v, N, vp );
    verify( &bb, vp );
  }
  return 0;
}
