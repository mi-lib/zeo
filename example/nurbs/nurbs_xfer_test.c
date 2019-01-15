#include <zeo/zeo_nurbs.h>

void nurbs_fwrite(FILE *fp, zNURBS3D *nurbs)
{
  register int i, j;
  double u, v;
  zVec3D p;

  for( i=0; i<=nurbs->ns[0]; i++ ){
    u = zNURBS3DKnotSlice( nurbs, 0, i );
    for( j=0; j<=nurbs->ns[1]; j++ ){
      v = zNURBS3DKnotSlice( nurbs, 1, j );
      if( zNURBS3DVec( nurbs, u, v, &p ) ){
        zVec3DDataFWrite( fp, &p );
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

  if( !( fp = fopen( "test_nurbs.tkf", "r" ) ) ){
    eprintf( "cannot open test_nurbs.tkf.\n" );
    eprintf( "run nurbs_test first.\n" );
    return 1;
  }
  zNURBS3DFRead( fp, &src );
  fclose( fp );
  zNURBS3DClone( &src, &dest );

  fp = fopen( "src", "w" );
  nurbs_fwrite( fp, &src );
  fclose( fp );

  fp = fopen( "dst", "w" );
  nurbs_fwrite( fp, &dest );
  fclose( fp );

  zNURBS3DMirror( &src, &dest, zX );
  fp = fopen( "mrr", "w" );
  nurbs_fwrite( fp, &dest );
  fclose( fp );

  zFrame3DFromZYX( &f, 1, 1, 1, zDeg2Rad(45), zDeg2Rad(10), zDeg2Rad(5) );
  zNURBS3DXfer( &src, &f, &dest );
  fp = fopen( "xfr", "w" );
  nurbs_fwrite( fp, &dest );
  fclose( fp );

  zNURBS3DXferInv( &src, &f, &dest );
  fp = fopen( "xfi", "w" );
  nurbs_fwrite( fp, &dest );
  fclose( fp );

  zNURBS3DDestroy( &src );
  zNURBS3DDestroy( &dest );
  return 0;
}
