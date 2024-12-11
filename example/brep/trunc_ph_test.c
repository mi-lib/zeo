#include <zeo/zeo_bv3d.h>
#include <zeo/zeo_brep.h>

#define N 1000

void test_ph(zPH3D *a, zPH3D *b)
{
  zVec3DData data;
  zVec3D v;
  int i;

  zVec3DDataInitArray( &data, N );
  for( i=0; i<N; i++ ){
    zVec3DCreatePolar( &v, zRandF(-0.2,0.2), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    zVec3DDataAdd( &data, &v );
  }
  zVec3DDataConvexHull( &data, a );
  zVec3DDataRewind( &data );
  for( i=0; i<N; i++ ){
    zVec3DCreatePolar( &v, zRandF(-0.2,0.2), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    v.e[zX] += 0.05;
    v.e[zY] += 0.05;
    v.e[zZ] += 0.05;
    zVec3DDataAdd( &data, &v );
  }
  zVec3DDataConvexHull( &data, b );
  zVec3DDataDestroy( &data );
}

void output_blue(FILE *fp)
{
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.6\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 0.0\n\n" );
}

void output_red(FILE *fp)
{
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 0.0\n\n" );
}

void output(zPH3D *a, zPH3D *b, zPH3D *sa, zPH3D *sb)
{
  FILE *fp;

  /* for visualization */
  fp = fopen( "org.ztk", "w" );
  output_blue( fp );
  output_red( fp );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrintZTK( fp, a );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrintZTK( fp, b );
  fclose( fp );

  fp = fopen( "cut.ztk", "w" );
  output_blue( fp );
  output_red( fp );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrintZTK( fp, sa );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrintZTK( fp, sb );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zPH3D a, b, sa, sb;
  zBREP ba, bb;

  zRandInit();
  test_ph( &a, &b );
  zPH3D2BREP( &a, &ba );
  zPH3D2BREP( &b, &bb );

  zBREPTruncPH3D( &ba, &b );
  zBREPTruncPH3D( &bb, &a );
  zBREP2PH3D( &ba, &sa );
  zBREP2PH3D( &bb, &sb );

  output( &a, &b, &sa, &sb );

  zPH3DDestroy( &a );
  zPH3DDestroy( &b );
  zPH3DDestroy( &sa );
  zPH3DDestroy( &sb );
  zBREPDestroy( &ba );
  zBREPDestroy( &bb );
  return 0;
}
