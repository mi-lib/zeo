#include <zeo/zeo_nurbs.h>

int main(void)
{
  zNURBS3D nurbs;
  zPH3D ph;
  FILE *fp;
  ZTK ztk;

  ZTKInit( &ztk );
  if( !ZTKParse( &ztk, "test_nurbs.ztk" ) ){
    eprintf( "cannot open test_nurbs.ztk.\n" );
    eprintf( "run nurbs_test first.\n" );
    return 1;
  }
  ZTKTagRewind( &ztk );
  zNURBS3DFromZTK( &nurbs, &ztk );
  zNURBS3DToPH( &nurbs, &ph );

  fp = fopen( "ph.ztk", "w" );
  zPH3DFPrintZTK( fp, &ph );
  fclose( fp );

  zNURBS3DDestroy( &nurbs );
  ZTKDestroy( &ztk );
  zPH3DDestroy( &ph );
  return 0;
}
