#include <zeo/zeo_nurbs.h>

int main(void)
{
  zNURBS3D nurbs;
  int i, j;
  double u, v;
  zVec3D p, nn;
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
  ZTKDestroy( &ztk );

  fp = fopen( "sfc", "w" );
  for( i=0; i<=zNURBS3DSlice(&nurbs,0); i++ ){
    u = zNURBS3DKnotSlice( &nurbs, 0, i );
    for( j=0; j<=zNURBS3DSlice(&nurbs,1); j++ ){
      v = zNURBS3DKnotSlice( &nurbs, 1, j );
      if( zNURBS3DVec( &nurbs, u, v, &p ) ){
        zVec3DDataFPrint( fp, &p );
        fprintf( fp, "\n" );
      }
    }
    fprintf( fp, "\n" );
  }
  fclose( fp );

  zRandInit();
  fp = fopen( "nn", "w" );
  zVec3DCreate( &p, zRandF(2,6), zRandF(-1,1), zRandF(-3,3) );
  zNURBS3DClosest( &nurbs, &p, &nn, &u, &v );
  zVec3DDataFPrint( fp, &p );
  fprintf( fp, "\n" );
  zVec3DDataFPrint( fp, &nn );
  fprintf( fp, "\n\n\n" );
  fclose( fp );

  zNURBS3DDestroy( &nurbs );
  return 0;
}
