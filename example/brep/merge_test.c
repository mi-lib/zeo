#include <zeo/zeo_bv.h>
#include <zeo/zeo_brep.h>

/* test */

void test_ph(zPH3D *a)
{
  FILE *fp;

  fp = fopen( "../model/octahedron.zph", "rt" );
  zPH3DFScan( fp, a );
  fclose( fp );
}

void output(zPH3D *a, zPH3D *b, zPH3D *c)
{
  FILE *fp;

  /* for visualization */
  fp = fopen( "org.z3d", "w" );
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

  fp = fopen( "mrg.z3d", "w" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 0.0\n\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrint( fp, c );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zPH3D oct, a, b, c;
  zBREP ba, bb;
  zPlane3D pl;
  zVec3D n;

  /* original octahedra */
  test_ph( &oct );

  zVec3DCopy( ZVEC3DZ, &n );
  zPlane3DCreate( &pl, ZVEC3DZERO, &n );
  zPH3D2BREP( &oct, &ba );
  zBREPTrunc( &ba, &pl );
  zBREP2PH3D( &ba, &a );

  zVec3DRevDRC( &n );
  zPlane3DCreate( &pl, ZVEC3DZERO, &n );
  zPH3D2BREP( &oct, &bb );
  zBREPTrunc( &bb, &pl );
  zBREP2PH3D( &bb, &b );

  /* merge */
  zBREPMerge( &ba, &bb );
  zBREP2PH3D( &ba, &c );

  /* output */
  output( &a, &b, &c );

  /* terminate */
  zPH3DDestroy( &oct );
  zPH3DDestroy( &a );
  zPH3DDestroy( &b );
  zPH3DDestroy( &c );
  zBREPDestroy( &ba );
  zBREPDestroy( &bb );
  return 0;
}
