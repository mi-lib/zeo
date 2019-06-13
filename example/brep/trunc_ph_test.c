#include <zeo/zeo_bv.h>
#include <zeo/zeo_brep.h>

void test_ph(zPH3D *a, zPH3D *b)
{
#define N 1000
  zVec3D v[N];
  register int i;

  zRandInit();
  for( i=0; i<N; i++ )
    zVec3DCreatePolar( &v[i], zRandF(-0.2,0.2), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
  zCH3D( a, v, N );
  for( i=0; i<N; i++ ){
    zVec3DCreatePolar( &v[i], zRandF(-0.2,0.2), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    v[i].e[zX] += 0.05;
    v[i].e[zY] += 0.05;
    v[i].e[zZ] += 0.05;
  }
  zCH3D( b, v, N );
}

void output_blue(FILE *fp)
{
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.6\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 0.0\n\n" );
}

void output_red(FILE *fp)
{
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 0.0\n\n" );
}

void output(zPH3D *a, zPH3D *b, zPH3D *sa, zPH3D *sb)
{
  FILE *fp;

  /* for visualization */
  fp = fopen( "org", "w" );
  output_blue( fp );
  output_red( fp );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrint( fp, a );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrint( fp, b );
  fclose( fp );

  fp = fopen( "cut", "w" );
  output_blue( fp );
  output_red( fp );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrint( fp, sa );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrint( fp, sb );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zPH3D a, b, sa, sb;
  zBREP ba, bb;

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
