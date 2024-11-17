#include <zeo/zeo_nurbs.h>

int main(int argc, char *argv[])
{
  zNURBS3D nurbs;
  zVec3D cp[] = {
    { { 2.0, 3.0, 0.0 } },
    { { 3.0,-1.0, 0.0 } },
    { { 5.0,-2.0, 0.0 } },
    { { 4.0, 0.0, 0.0 } },
    { { 5.0, 4.0, 0.0 } },
    { { 7.0, 1.5, 0.0 } },
  };
  register int i, n;
  double t;
  zVec3D p;
  zVec3D tv;
  FILE *fpp, *fpt;

  n = sizeof(cp)/sizeof(zVec3D);
  zNURBS3D1Alloc( &nurbs, n, 3 );
  for( i=0; i<n; i++ )
    zNURBS3D1SetCP( &nurbs, i, &cp[i] );
  zNURBS3D1SetWeight( &nurbs, 4, argc > 1 ? atof(argv[1]) : 1.0 );
  zNURBS3DSetSlice( &nurbs, 0, 100 );
  fpp = fopen( "p", "w" );
  fpt = fopen( "t", "w" );
  for( i=0; i<=zNURBS3DSlice(&nurbs,1); i++ ){
    t = zNURBS3D1KnotSlice( &nurbs, i );
    if( zNURBS3D1VecTan( &nurbs, 0, t, &p, &tv ) ){
      zVec3DDataNLFPrint( fpp, &p );
      zVec3DDataNLFPrint( fpt, &p );
      zVec3DAddDRC( &p, &tv );
      zVec3DDataNLFPrint( fpt, &p );
      zFEndl( fpt );
    }
  }
  zNURBS3DDestroy( &nurbs );
  return 0;
}
