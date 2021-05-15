#include <zeo/zeo_col.h>
#include <zeo/zeo_bv3d.h>

void vec_create_rand(zVec3D v[], int n, double r)
{
  register int i;

  for( i=0; i<n; i++ ){
    zVec2DCreatePolar( (zVec2D *)&v[i], zRandF(0,r), zRandF(-zPI,zPI) );
    v[i].c.z = 0;
  }
}

void output(char filename[], zVec3D pl[], int n, zVec3D *p, zVec3D *c)
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
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.8\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.4\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.4\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  /* pair of proximities */
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: p\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, p );
  fprintf( fp, "radius: 0.01\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: c\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, c );
  fprintf( fp, "radius: 0.01\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: rod\n" );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, p );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, c );
  fprintf( fp, "radius: 0.005\n" );
  /* convex set */
  zCH3D( &ch, pl, n );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: ch\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrintZTK( fp, &ch );
  zPH3DDestroy( &ch );

  fclose( fp );
}

#define N 1000

int main(int argc, char *argv[])
{
  zVec3D a[N];
  zVec3D p, c;

  zRandInit();
  vec_create_rand( a, N, 0.2 );
  zVec3DCreate( &p, zRandF(-0.4,0.4), zRandF(-0.4,0.4), 0 );
  printf( "in collision? %s\n", zBoolStr( zGJKPoint( a, N, &p, &c ) ) );
  output( "a", a, N, &p, &c );
  return 0;
}
