#include <zeo/zeo_nurbs.h>

void nurbs_fprint(FILE *fp, zNURBS3D *nurbs)
{
  register int i, j;
  double u, v;
  zVec3D p;

  for( i=0; i<=nurbs->ns[0]; i++ ){
    u = zNURBS3DKnotSlice( nurbs, 0, i );
    for( j=0; j<=nurbs->ns[1]; j++ ){
      v = zNURBS3DKnotSlice( nurbs, 1, j );
      if( zNURBS3DVec( nurbs, u, v, &p ) ){
        zVec3DDataFPrint( fp, &p );
        fprintf( fp, "\n" );
      }
    }
    fprintf( fp, "\n" );
  }
}

int main(void)
{
  zNURBS3D src, dest;
  zFrame3D f;
  FILE *fp;
  ZTK ztk;

  ZTKInit( &ztk );
  if( !ZTKParse( &ztk, "test_nurbs.ztk" ) ){
    eprintf( "cannot open test_nurbs.ztk.\n" );
    eprintf( "run nurbs_test first.\n" );
    return 1;
  }
  ZTKTagRewind( &ztk );
  zNURBS3DFromZTK( &src, &ztk );
  ZTKDestroy( &ztk );
  zNURBS3DClone( &src, &dest );

  fp = fopen( "src", "w" );
  nurbs_fprint( fp, &src );
  fclose( fp );

  fp = fopen( "dst", "w" );
  nurbs_fprint( fp, &dest );
  fclose( fp );

  zNURBS3DMirror( &src, &dest, zX );
  fp = fopen( "mrr", "w" );
  nurbs_fprint( fp, &dest );
  fclose( fp );

  zFrame3DFromZYX( &f, 1, 1, 1, zDeg2Rad(45), zDeg2Rad(10), zDeg2Rad(5) );
  zNURBS3DXform( &src, &f, &dest );
  fp = fopen( "xfr", "w" );
  nurbs_fprint( fp, &dest );
  fclose( fp );

  zNURBS3DXformInv( &src, &f, &dest );
  fp = fopen( "xfi", "w" );
  nurbs_fprint( fp, &dest );
  fclose( fp );

  zNURBS3DDestroy( &src );
  zNURBS3DDestroy( &dest );
  return 0;
}
