#include <zeo/zeo_nurbs.h>
#include <zeo/zeo_shape3d.h>

#define TEST_ZTK   "test_nurbs.ztk"
#define OUTPUT_ZTK "output.ztk"

void nurbs_test(void)
{
  zNURBS3D nurbs;
  zPH3D ph;
  ZTK ztk;
  FILE *fp;

  ZTKInit( &ztk );
  if( !ZTKParse( &ztk, TEST_ZTK ) ){
    eprintf( "cannot open %s.\n", TEST_ZTK );
    eprintf( "run nurbs_test first.\n" );
    exit( 1 );
  }
  ZTKTagRewind( &ztk );
  zNURBS3DFromZTK( &nurbs, &ztk );
  ZTKDestroy( &ztk );
  zNURBS3DToPH( &nurbs, &ph );
  zNURBS3DDestroy( &nurbs );

  fp = fopen( OUTPUT_ZTK, "w" );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "type: polyhedron\n" );
  zPH3DFPrintZTK( fp, &ph );
  fclose( fp );
  zPH3DDestroy( &ph );
}

void shape_test(void)
{
  zShape3D s;
  FILE *fp;

  if( !zShape3DReadZTK( &s, TEST_ZTK ) ){
    eprintf( "cannot open %s.\n", TEST_ZTK );
    eprintf( "run nurbs_test first.\n" );
    exit( 1 );
  }
  zShape3DToPH( &s );
  fp = fopen( OUTPUT_ZTK, "w" );
  fprintf( fp, "[zeo::shape]\n" );
  zShape3DFPrintZTK( fp, &s );
  fclose( fp );
  zShape3DDestroy( &s );
}

int main(void)
{
  nurbs_test();
/*  shape_test(); */
  return 0;
}
