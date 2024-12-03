#include <zeo/zeo.h>

void vec_create_rand(zVec3D v[], int n)
{
  register int i;
  FILE *fp;

  fp = fopen( "src", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v[i], zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zVec3DValueNLFPrint( fp, &v[i] );
  }
  fclose( fp );
}

void verify(zVec3D v[], int n, zAABox3D *bb, zVec3D **vp)
{
  register int i;
  bool ret;
  zBox3D box;
  zVec3D vert;
  FILE *fp;

  zAABox3DToBox3D( bb, &box );
  for( i=0; i<n; i++ ){
    if( ( ret = zBox3DPointIsInside(&box,&v[i],true) ) == false ){
      zVec3DValueNLFPrint( stderr, &v[i] );
      eprintf( "D=%.15g\n", zBox3DDistFromPoint(&box,&v[i]) );
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

void vec_list_create_rand(zVec3DList *pl, int n)
{
  register int i;
  FILE *fp;
  zVec3D v;

  zListInit( pl );
  fp = fopen( "src", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zVec3DListAdd( pl, &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );
}

void verify_list(zAABox3D *bb, zVec3DList *pl, zVec3DListCell **vp)
{
  register int i;
  bool ret;
  zBox3D box;
  zVec3D vert;
  FILE *fp;
  zVec3DListCell *pc;

  zAABox3DToBox3D( bb, &box );
  zListForEach( pl, pc ){
    if( ( ret = zBox3DPointIsInside(&box,pc->data,true) ) == false ){
      zVec3DValueNLFPrint( stderr, pc->data );
      eprintf( "D=%.15g\n", zBox3DDistFromPoint(&box,pc->data) );
    }
  }
  /* points on the walls */
  fp = fopen( "bd", "w" );
  for( i=0; i<6; i++ )
    zVec3DValueNLFPrint( fp, vp[i]->data );
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
  zVec3D v[N];
  zVec3D *vp[6];
  zVec3DList pl;
  zVec3DListCell *vpl[6];

  zRandInit();
  if( argc > 1 && strcmp( argv[1], "list" ) == 0 ){
    vec_list_create_rand( &pl, N );
    zAABB3DPL( &bb, &pl, vpl );
    verify_list( &bb, &pl, vpl );
    zVec3DListDestroy( &pl );
  } else{
    vec_create_rand( v, N );
    zAABB3D( &bb, v, N, vp );
    verify( v, N, &bb, vp );
  }
  return 0;
}
