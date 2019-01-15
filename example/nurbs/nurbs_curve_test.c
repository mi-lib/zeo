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

  n = sizeof(cp)/sizeof(zVec3D);
  zNURBS3D1Alloc( &nurbs, n, 3 );
  for( i=0; i<n; i++ )
    zNURBS3D1SetCP( &nurbs, i, &cp[i] );
  zNURBS3D1SetWeight( &nurbs, 4, argc > 1 ? atof(argv[1]) : 1.0 );
  zNURBS3DSetSliceNum( &nurbs, 0, 100 );
  for( i=0; i<=nurbs.ns[1]; i++ ){
    t = zNURBS3D1KnotSlice( &nurbs, i );
    if( zNURBS3D1Vec( &nurbs, t, &p ) ){
      printf( "%f ",t );
      zVec3DDataWrite( &p );
      zEndl();
    }
  }
  zNURBS3DDestroy( &nurbs );
  return 0;
}
