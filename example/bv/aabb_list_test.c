#include <zeo/zeo.h>

#define N 100

void veclist_create_rand(zVec3DList *pl)
{
  register int i;
  FILE *fp;
  zVec3D v;

  zListInit( pl );
  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zVec3DListAdd( pl, &v );
    zVec3DDataNLFPrint( fp, &v );
  }
  fclose( fp );
}

void verify(zAABox3D *bb, zVec3DList *pl, zVec3DListCell **vp)
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
      zVec3DDataNLFPrint( stderr, pc->data );
      eprintf( "D=%.15g\n", zBox3DPointDist(&box,pc->data) );
    }
  }
  /* points on the walls */
  fp = fopen( "bd", "w" );
  for( i=0; i<6; i++ )
    zVec3DDataNLFPrint( fp, vp[i]->data );
  fclose( fp );
  /* AABB */
  fp = fopen( "aabb", "w" );
  for( i=0; i<4; i++ )
    zVec3DDataNLFPrint( fp, zBox3DVert(&box,i,&vert) );
  zVec3DDataNLFPrint( fp, zBox3DVert(&box,0,&vert) );
  fprintf( fp, "\n" );
  for( i=4; i<8; i++ )
    zVec3DDataNLFPrint( fp, zBox3DVert(&box,i,&vert) );
  zVec3DDataNLFPrint( fp, zBox3DVert(&box,4,&vert) );
  fprintf( fp, "\n" );
  for( i=0; i<4; i++ ){
    zVec3DDataNLFPrint( fp, zBox3DVert(&box,i,&vert) );
    zVec3DDataNLFPrint( fp, zBox3DVert(&box,i+4,&vert) );
    fprintf( fp, "\n" );
  }
  fclose( fp );
}

int main(void)
{
  zAABox3D bb;
  zVec3DListCell *vp[6];
  zVec3DList pl;

  zRandInit();
  veclist_create_rand( &pl );
  zAABBPL( &bb, &pl, vp );
  verify( &bb, &pl, vp );
  zVec3DListDestroy( &pl );
  return 0;
}
