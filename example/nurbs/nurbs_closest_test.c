#include <zeo/zeo_nurbs.h>

int main(void)
{
  zNURBS3D nurbs;
  register int i, j;
  double u, v;
  zVec3D p, nn;
  FILE *fp;

  zRandInit();
  if( !( fp = fopen( "test_nurbs.tkf", "r" ) ) ){
    eprintf( "cannot open test_nurbs.tkf.\n" );
    eprintf( "run nurbs_test first.\n" );
    return 1;
  }
  zNURBS3DFRead( fp, &nurbs );
  fclose( fp );

  fp = fopen( "sfc", "w" );
  for( i=0; i<=nurbs.ns[0]; i++ ){
    u = zNURBS3DKnotSlice( &nurbs, 0, i );
    for( j=0; j<=nurbs.ns[1]; j++ ){
      v = zNURBS3DKnotSlice( &nurbs, 1, j );
      if( zNURBS3DVec( &nurbs, u, v, &p ) ){
        zVec3DDataFWrite( fp, &p );
        fprintf( fp, "\n" );
      }
    }
    fprintf( fp, "\n" );
  }
  fclose( fp );

  fp = fopen( "nn", "w" );
  zVec3DCreate( &p, zRandF(2,6), zRandF(-1,1), zRandF(-3,3) );
  zNURBS3DClosest( &nurbs, &p, &nn, &u, &v );
  zVec3DDataFWrite( fp, &p );
  fprintf( fp, "\n" );
  zVec3DDataFWrite( fp, &nn );
  fprintf( fp, "\n\n\n" );
  fclose( fp );

  zNURBS3DDestroy( &nurbs );
  return 0;
}
