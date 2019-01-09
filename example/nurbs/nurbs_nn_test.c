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
  register int i, j, n1, n2;
  const int uslice = 50, vslice = 50;
  double u, v;
  zVec3D p, nn;
  FILE *fp;

  zRandInit();
  n1 = n2 = sqrt(sizeof(cp)/sizeof(zVec3D));
  zNURBS3DAlloc( &nurbs, n1, n2, 3, 3 );
  for( i=0; i<n1; i++ )
    for( j=0; j<n2; j++ )
      zNURBS3DSetCP( &nurbs, i, j, &cp[i*n2+j] );

  fp = fopen( "sfc", "w" );
  for( i=0; i<=uslice; i++ ){
    u = zNURBS3DKnotSlice( &nurbs, 0, i, uslice );
    for( j=0; j<=vslice; j++ ){
      v = zNURBS3DKnotSlice( &nurbs, 1, j, vslice );
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
  zNURBS3DVecNN( &nurbs, &p, &nn, &u, &v );
  zVec3DDataFWrite( fp, &p );
  fprintf( fp, "\n" );
  zVec3DDataFWrite( fp, &nn );
  fprintf( fp, "\n\n\n" );
  fclose( fp );

  zNURBS3DDestroy( &nurbs );
  return 0;
}
