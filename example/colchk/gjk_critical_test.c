#include <zeo/zeo_col.h>
#include <zeo/zeo_bv.h>

void vec_create(zVec3D p1[], zVec3D p2[], double z)
{
  printf( "%g\n", z );

  zVec3DCreate( &p1[0], 0.1, 0.0, 0.0 );
  zVec3DCreate( &p1[1],-0.1, 0.1, 0.0 );
  zVec3DCreate( &p1[2],-0.1,-0.1, 0.0 );
  zVec3DCreate( &p1[3], 0.0, 0.0,-0.1 );

  zVec3DCreate( &p2[0], 0.2, 0.0, 0.1 );
  zVec3DCreate( &p2[1],-0.2, 0.2, 0.1 );
  zVec3DCreate( &p2[2],-0.2,-0.2, 0.1 );
  zVec3DCreate( &p2[3], 0.0, 0.0,   z );
}

void output(char filename[], zVec3D p1[], int n1, zVec3D p2[], int n2, zVec3D *c1, zVec3D *c2)
{
  zPH3D ch;
  FILE *fp;

  fp = fopen( filename, "w" );
  /* for visualization */
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.8 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.8\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.4\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.4\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  /* pair of proximities */
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: p1\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFWrite( fp, c1 );
  fprintf( fp, "radius: 0.01\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: p2\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFWrite( fp, c2 );
  fprintf( fp, "radius: 0.01\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: rod\n" );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFWrite( fp, c1 );
  fprintf( fp, "center: " ); zVec3DDataNLFWrite( fp, c2 );
  fprintf( fp, "radius: 0.005\n" );
  /* convex set 1 */
  zCH3D( &ch, p1, n1 );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: ch1\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFWrite( fp, &ch );
  zPH3DDestroy( &ch );
  /* convex set 2 */
  zCH3D( &ch, p2, n2 );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: ch2\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFWrite( fp, &ch );
  zPH3DDestroy( &ch );

  fclose( fp );
}

#define N 4

int main(int argc, char *argv[])
{
  zVec3D a[N], b[N];
  zVec3D ca, cb;

  vec_create( a, b, argc > 1 ? atof(argv[1]) : 0 );

  printf( "in collision? %s\n", zBoolExpr( zGJK( a, N, b, N, &ca, &cb ) ) );
  zVec3DWrite( &ca );
  zVec3DWrite( &cb );
  output( "a", a, N, b, N, &ca, &cb );
  return 0;
}
