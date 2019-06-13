#include <zeo/zeo_col.h>
#include <zeo/zeo_bv.h>
#include <time.h>

void vec_create_rand(zVec3D p[], int n, double x, double y, double z, double r)
{
  register int i;

  for( i=0; i<n; i++ ){
    zVec3DCreatePolar( &p[i], zRandF(0,r), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    p[i].e[zX] += x;
    p[i].e[zY] += y;
    p[i].e[zZ] += z;
  }
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
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, c1 );
  fprintf( fp, "radius: 0.01\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: p2\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, c2 );
  fprintf( fp, "radius: 0.01\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: rod\n" );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, c1 );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, c2 );
  fprintf( fp, "radius: 0.005\n" );
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

#define N 1000

int main(int argc, char *argv[])
{
  zVec3D a[N], b[N], ca, cb, pos, dir;
  double x, y, z, depth;
  int i, loop = 100;
  clock_t start;
  bool result_gjk, result_mpr;

  zRandInit();
  x = zRandF( 0.1, 0.3 );
  y = zRandF( 0.1, 0.3 );
  z = zRandF( 0.1, 0.3 );
  vec_create_rand( a, N, 0, 0, 0, 0.2 );
  vec_create_rand( b, N, x, y, z, 0.2 );

  start = clock();
  for( i=0; i<loop; i++ )
    result_gjk = zGJK( a, N, b, N, &ca, &cb );
  printf( "zGJK time = %ld\n", clock() - start );
  printf( "GJK in collision? %s\n", zBoolExpr( result_gjk ) );
  output( "gjk.ztk", a, N, b, N, &ca, &cb );
  zVec3DPrint( &ca );
  zVec3DPrint( &cb );

  start = clock();
  for( i=0; i<loop; i++ )
    zMPR( a, N, b, N );
  printf( "zMPR time = %ld\n", clock() - start );

  start = clock();
  for( i=0; i<loop; i++ )
    result_mpr = zMPRDepth( a, N, b, N, &depth, &pos, &dir );
  printf( "zMPRDepth time = %ld\n", clock() - start );
  printf( "MPR in collision? %s\n", zBoolExpr( result_mpr ) );
  if( depth > 0 ){
    printf( "depth = %f\n", depth );
    zVec3DCat( &pos, depth/2, &dir, &ca );
    zVec3DCat( &pos,-depth/2, &dir, &cb );
    zVec3DPrint( &ca );
    zVec3DPrint( &cb );
    output( "mpr.ztk", a, N, b, N, &ca, &cb );
  }
  return 0;
}
