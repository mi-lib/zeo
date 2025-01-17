#include <zeo/zeo_vec3d_profile.h>

#define RADIUS 0.003

int main(int argc, char *argv[])
{
  zVec3DData pointdata;
  zVec3DData normaldata;
  clock_t t1, t2;

  if( argc < 2 ){
    ZRUNERROR( "PCD file not specified" );
    return 1;
  }
  zVec3DDataReadPCDFile( &pointdata, argv[1] );

  t1 = clock();
  zVec3DDataNormalVec_Octree( &pointdata, RADIUS, &normaldata );
  t2 = clock();
  eprintf( "[octree ] time=%d\n", (int)(t2-t1) );
  zVec3DDataDestroy( &normaldata );

  t1 = clock();
  zVec3DDataNormalVec_Tree( &pointdata, RADIUS, &normaldata );
  t2 = clock();
  eprintf( "[kd-tree] time=%d\n", (int)(t2-t1) );
  zVec3DDataWritePCDFile( &normaldata, "normal.pcd", "binary" );
  zVec3DDataDestroy( &normaldata );

  zVec3DDataDestroy( &pointdata );
  return 0;
}
