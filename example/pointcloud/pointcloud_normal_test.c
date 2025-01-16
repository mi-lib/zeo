#include <zeo/zeo_col.h>

#define RESOLUTION 0.005
#define RADIUS     0.003

int main(int argc, char *argv[])
{
  zVec3DData pointdata;
  zVec3DData normaldata;

  if( argc < 2 ){
    ZRUNERROR( "PCD file not specified" );
    return 1;
  }
  zVec3DDataReadPCDFile( &pointdata, argv[1] );

  zVec3DDataNormalVec( &pointdata, RADIUS, &normaldata );
  zVec3DDataWritePCDFile( &normaldata, "normal.pcd", "binary" );

  zVec3DDataDestroy( &normaldata );
  zVec3DDataDestroy( &pointdata );
  return 0;
}
