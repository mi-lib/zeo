#include <zeo/zeo_col.h>

void output_src(zVec3D p1[], int n1, zVec3D p2[], int n2)
{
  zPH3D ch;
  FILE *fp;

  fp = fopen( "src", "w" );
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
  /* convex set 1 */
  zCH3D( &ch, p1, n1 );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: ch1\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrint( fp, &ch );
  zPH3DDestroy( &ch );
  /* convex set 2 */
  zCH3D( &ch, p2, n2 );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: ch2\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrint( fp, &ch );
  zPH3DDestroy( &ch );

  fclose( fp );
}

void output_ms(zPH3D *ph)
{
  FILE *fp;

  fp = fopen( "ms", "w" );
  /* for visualization */
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 1.0 1.0 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.4\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.4\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  /* convex set */
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: minkowski_sub\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: white\n" );
  zPH3DFPrint( fp, ph );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: origin\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: 0 0 0\n" );
  fprintf( fp, "radius: 0.01\n" );

  fclose( fp );
}

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

zVec3D p1[4];
zVec3D p2[4];

int main(int argc, char *argv[])
{
  zPH3D ph;

  vec_create( p1, p2, argc > 1 ? atof(argv[1]) : 0 );
  zPH3DMinkowskiSub( &ph, p1, 4, p2, 4 );
  output_src( p1, 4, p2, 4 );
  output_ms( &ph );
  zPH3DDestroy( &ph );
  return 0;
}
