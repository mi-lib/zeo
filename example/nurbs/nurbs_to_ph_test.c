#include <zeo/zeo_nurbs.h>

int main(void)
{
  zNURBS3D nurbs;
  zPH3D ph;
  FILE *fp;

  if( !( fp = fopen( "test_nurbs.ztk", "r" ) ) ){
    eprintf( "cannot open test_nurbs.ztk.\n" );
    eprintf( "run nurbs_test first.\n" );
    return 1;
  }
  zNURBS3DFScan( fp, &nurbs );
  fclose( fp );
  zNURBS3DToPH( &nurbs, &ph );

  fp = fopen( "ph", "w" );
  zPH3DFPrint( fp, &ph );
  fclose( fp );

  zNURBS3DDestroy( &nurbs );
  zPH3DDestroy( &ph );
  return 0;
}
