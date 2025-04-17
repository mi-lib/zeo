#include <zeo/zeo_nurbs3d.h>

zVec3D cp[] = {
  { { 1.0,-2.0, 0.0 } },
  { { 1.0,-1.0, 0.0 } },
  { { 1.0, 0.0, 0.0 } },
  { { 1.0, 1.0, 0.0 } },
  { { 1.0, 2.0, 0.0 } },
  { { 3.0,-2.0, 0.0 } },
  { { 3.0,-1.0,-1.0 } },
  { { 3.0, 0.0,-1.0 } },
  { { 3.0, 1.0,-1.0 } },
  { { 3.0, 2.0, 0.0 } },
  { { 4.0,-2.0, 0.0 } },
  { { 4.0,-1.0,-1.0 } },
  { { 4.0, 0.0, 2.0 } },
  { { 4.0, 1.0,-1.0 } },
  { { 4.0, 2.0, 0.0 } },
  { { 5.0,-2.0, 0.0 } },
  { { 5.0,-1.0,-1.0 } },
  { { 5.0, 0.0,-1.0 } },
  { { 5.0, 1.0,-1.0 } },
  { { 5.0, 2.0, 0.0 } },
  { { 7.0,-2.0, 0.0 } },
  { { 7.0,-1.0, 0.0 } },
  { { 7.0, 0.0, 0.0 } },
  { { 7.0, 1.0, 0.0 } },
  { { 7.0, 2.0, 0.0 } },
};

int main(void)
{
  zNURBS3D nurbs;
  int i, j, n1, n2;
  double u, v;
  zVec3D p, n, t1, t2;
  FILE *fp;

  zRandInit();
  fp = fopen( "src", "w" );
  n1 = n2 = sqrt(sizeof(cp)/sizeof(zVec3D));
  zNURBS3DAlloc( &nurbs, n1, n2, 3, 3 );
  zNURBS3DSetWeight( &nurbs, 2, 2, 2.0 );
  for( i=0; i<n1; i++ )
    for( j=0; j<n2; j++ ){
      zNURBS3DSetCP( &nurbs, i, j, &cp[i*n2+j] );
      zVec3DValueFPrint( fp, zNURBS3DCP(&nurbs,i,j) );
      fprintf( fp, "\n" );
    }
  fclose( fp );

  fp = fopen( "sfc", "w" );
  zNURBS3DSetSlice( &nurbs, 30, 30 );
  for( i=0; i<=zNURBS3DSlice(&nurbs,0); i++ ){
    u = zNURBS3DKnotSlice( &nurbs, 0, i );
    for( j=0; j<=zNURBS3DSlice(&nurbs,1); j++ ){
      v = zNURBS3DKnotSlice( &nurbs, 1, j );
      if( zNURBS3DVec( &nurbs, u, v, &p ) ){
        zVec3DValueFPrint( fp, &p );
        fprintf( fp, "\n" );
      }
    }
    fprintf( fp, "\n" );
  }
  fclose( fp );

  fp = fopen( "nrm", "w" );
 RETRY:
  u = zRandF( zNURBS3DKnotS(&nurbs,0), zNURBS3DKnotE(&nurbs,0) );
  v = zRandF( zNURBS3DKnotS(&nurbs,1), zNURBS3DKnotE(&nurbs,1) );
  if( zNURBS3DVecTSpace( &nurbs, u, v, &p, &n, &t1, &t2 ) ){
    if( zVec3DIsTiny( &n ) ) goto RETRY;
    zVec3DValueFPrint( fp, &p );
    fprintf( fp, "\n" );
    zVec3DAddDRC( &n, &p );
    zVec3DValueFPrint( fp, &n );
    fprintf( fp, "\n\n\n" );
    zVec3DValueFPrint( fp, &p );
    fprintf( fp, "\n" );
    zVec3DAddDRC( &t1, &p );
    zVec3DValueFPrint( fp, &t1 );
    fprintf( fp, "\n\n\n" );
    zVec3DValueFPrint( fp, &p );
    fprintf( fp, "\n" );
    zVec3DAddDRC( &t2, &p );
    zVec3DValueFPrint( fp, &t2 );
  }
  fclose( fp );

  fp = fopen( "test_nurbs.ztk", "w" );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "type: nurbs\n" );
  zNURBS3DFPrintZTK( fp, &nurbs );
  fclose( fp );

  zNURBS3DDestroy( &nurbs );
  return 0;
}
