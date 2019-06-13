#include <zeo/zeo_col.h>

void test_plane_box(zVec3D *p0, zPlane3D *p, zAABox3D *b)
{
  zVec3D norm;

  zRandInit();
  zVec3DCreate( p0, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
  zVec3DCreate( &norm, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
  zVec3DNormalizeDRC( &norm );
  zPlane3DCreate( p, p0, &norm );
  zAABox3DCreate( b, zRandF(-0.3,0), zRandF(-0.3,0), zRandF(-0.3,0), zRandF(0,0.3), zRandF(0,0.3), zRandF(0,0.3) );
}

void output(zPlane3D *p, zAABox3D *b, zVec3D ip[], int n, const char filename[])
{
  FILE *fp;
  zBox3D box;
  zVec3D sv1, sv2, v[4];
  int i;

  zVec3DOrthoSpace( zPlane3DNorm(p), &sv1, &sv2 );

  /* for visualization */
  fp = fopen( filename, "w" );

  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.6\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 0.0\n\n" );

  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 0.0\n\n" );

  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.6 0.6 0.0\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.4\n" );
  fprintf( fp, "exp: 0.0\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: c\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: red\n" );
  zAABox3DToBox3D( b, &box );
  zBox3DFPrint( fp, &box );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: plane\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: yellow\n" );
  zVec3DCat( zPlane3DVert(p), 1.0, &sv1, &v[0] );
  zVec3DCatDRC( &v[0], 1.0, &sv2 );
  zVec3DCat( zPlane3DVert(p), 1.0, &sv1, &v[1] );
  zVec3DCatDRC( &v[1],-1.0, &sv2 );
  zVec3DCat( zPlane3DVert(p),-1.0, &sv1, &v[2] );
  zVec3DCatDRC( &v[2],-1.0, &sv2 );
  zVec3DCat( zPlane3DVert(p),-1.0, &sv1, &v[3] );
  zVec3DCatDRC( &v[3], 1.0, &sv2 );
  fprintf( fp, "vert 0: " ); zVec3DDataNLFPrint( fp, &v[0] );
  fprintf( fp, "vert 1: " ); zVec3DDataNLFPrint( fp, &v[1] );
  fprintf( fp, "vert 2: " ); zVec3DDataNLFPrint( fp, &v[2] );
  fprintf( fp, "vert 3: " ); zVec3DDataNLFPrint( fp, &v[3] );
  fprintf( fp, "face 0 1 2\n" );
  fprintf( fp, "face 0 2 3\n" );

  for( i=0; i<n; i++ ){
    fprintf( fp, "[shape]\n" );
    fprintf( fp, "name: p%d\n", i );
    fprintf( fp, "type: sphere\n" );
    fprintf( fp, "optic: blue\n" );
    fprintf( fp, "center: " );
    zVec3DDataNLFPrint( fp, &ip[i] );
    fprintf( fp, "radius: 0.01\n" );
  }
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zPlane3D p;
  zAABox3D b;
  zVec3D p0, ip[6];
  int n;

  test_plane_box( &p0, &p, &b );

  eprintf( "collision = %s\n", zBoolExpr( zColChkPlaneAABox3D( &p, &b ) ) );
  n = zIntersectPlaneAABox3D( &p, &b, ip );
  eprintf( "num. of intersection point = %d\n", n );
  output( &p, &b, ip, n, "box" );
  return 0;
}
