#include <zeo/zeo_nurbs.h>
#include <zeo/zeo_shape3d.h>

#define TEST_ZTK "test_nurbs.ztk"

void nurbs_test(void)
{
  zNURBS3D nurbs;
  zPH3D ph;
  ZTK ztk;

  ZTKInit( &ztk );
  if( !ZTKParse( &ztk, TEST_ZTK ) ){
    eprintf( "cannot open %s.\n", TEST_ZTK );
    eprintf( "run nurbs_test first.\n" );
    exit( 1 );
  }
  ZTKTagRewind( &ztk );
  zNURBS3DFromZTK( &nurbs, &ztk );
  zNURBS3DToPH( &nurbs, &ph );

  printf( "[shape]\n" );
  printf( "type: polyhedron\n" );
  zPH3DFPrintZTK( stdout, &ph );

  zNURBS3DDestroy( &nurbs );
  ZTKDestroy( &ztk );
  zPH3DDestroy( &ph );
}

void shape_test(void)
{
  zShape3D s;

  if( !zShape3DReadZTK( &s, TEST_ZTK ) ){
    eprintf( "cannot open %s.\n", TEST_ZTK );
    eprintf( "run nurbs_test first.\n" );
    exit( 1 );
  }
  zShape3DToPH( &s );
  printf( "[shape]\n" );
  zShape3DFPrintZTK( stdout, &s );
  zShape3DDestroy( &s );
}

int main(void)
{
  /* nurbs_test(); */
  shape_test();
  return 0;
}
