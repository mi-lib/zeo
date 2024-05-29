#include <zeo/zeo_col.h>
#include <zeo/zeo_bv3d.h>

#define TEST 3

void vec_create_rand(zVec3D v[], int n, double r)
{
  register int i;

  for( i=0; i<n; i++ ){
    zVec2DCreatePolar( (zVec2D *)&v[i], zRandF(0,r), zRandF(-zPI,zPI) );
    v[i].c.z = 0;
  }
}

void output_mshape(char filename[], zVec3D pl[], int n, zVec3D *p, zVec3D *c)
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
  fprintf( fp, "radius: 0.01\n\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: c\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, c );
  fprintf( fp, "radius: 0.01\n\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: rod\n" );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, p );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, c );
  fprintf( fp, "radius: 0.005\n" );
  /* convex set */
  zConvexHull3D( &ch, pl, n );
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
  zVec3D pg[N];
  zVec3D p, c;
  int n = N;

#if TEST == 1
  zRandInit();
  vec_create_rand( pg, N, 0.2 );
  zVec3DCreate( &p, zRandF(-0.4,0.4), zRandF(-0.4,0.4), 0 );
#elif TEST == 2
  zVec3DCreate( &pg[0], 1, 1, 0 );
  zVec3DCreate( &pg[1],-1, 1, 0 );
  zVec3DCreate( &pg[2], 1,-1, 0 );
  zVec3DCreate( &pg[3],-1,-1, 0 );
  n = 4
  zVec3DCreate( &p, argc > 1 ? atof(argv[1]) : -2, 0, 0 );
#else
  zVec3DCreate( &pg[0], 1, 1, 1 );
  zVec3DCreate( &pg[1],-1, 1, 1 );
  zVec3DCreate( &pg[2], 1,-1, 1 );
  zVec3DCreate( &pg[3],-1,-1, 1 );
  zVec3DCreate( &pg[4], 1, 1,-1 );
  zVec3DCreate( &pg[5],-1, 1,-1 );
  zVec3DCreate( &pg[6], 1,-1,-1 );
  zVec3DCreate( &pg[7],-1,-1,-1 );
  n = 8;
  zVec3DCreate( &p, argc > 1 ? atof(argv[1]) : -2, argc > 2 ? atof(argv[2]) : 0, argc > 3 ? atof(argv[3]) : 0 );
#endif
  printf( "in collision? %s\n", zBoolStr( zGJKPoint( pg, n, &p, &c ) ) );
  printf( "test point: " ); zVec3DPrint( &p );
  printf( "proximity:  " ); zVec3DPrint( &c );
  output_mshape( "result.ztk", pg, n, &p, &c );
  return 0;
}
