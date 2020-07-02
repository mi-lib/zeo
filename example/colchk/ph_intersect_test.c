#include <zeo/zeo_bv.h>
#include <zeo/zeo_col.h>
#include <zeo/zeo_brep.h> /* for comparison */

void output(char filename[], zPH3D *a, zPH3D *b, zPH3D *c)
{
  FILE *fp;

  fp = fopen( filename, "w" );
  /* for visualization */
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.8 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.4\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.8\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.4\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.4\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.4\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  /* intersection convex */
  if( c ){
    fprintf( fp, "[shape]\n" );
    fprintf( fp, "name: c\n" );
    fprintf( fp, "type: polyhedron\n" );
    fprintf( fp, "optic: yellow\n" );
    zPH3DFPrintZTK( fp, c );
  }
  /* convex 1 */
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrintZTK( fp, a );
  /* convex 2 */
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrintZTK( fp, b );

  fclose( fp );
}

#define N 10000
void test_ph(zPH3D *a, zPH3D *b, int n)
{
  zVec3D v[N];
  register int i;

  if( n > N ) n = N;
  zRandInit();
  for( i=0; i<n; i++ )
    zVec3DCreatePolar( &v[i], zRandF(-0.1,0.1), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
  zCH3D( a, v, n );

  for( i=0; i<n; i++ ){
    zVec3DCreatePolar( &v[i], zRandF(-0.1,0.1), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    v[i].e[zX] += 0.08;
    v[i].e[zY] += 0.08;
    v[i].e[zZ] += 0.08;
  }
  zCH3D( b, v, n );
}

int main(int argc, char *argv[])
{
  zPH3D a, b, ip;
  clock_t t1, t2;
  int n;
  int dt1, dt2, dt3, dt4;

  test_ph( &a, &b, ( n = argc > 1 ? atoi(argv[1]) : N ) );
  output( "org", &a, &b, NULL );

  t1 = clock();
  if( !zIntersectPH3D( &a, &b, &ip ) ){
    eprintf( "not intersect.\n" );
    return 1;
  }
  t2 = clock();
  dt1 = t2 - t1;
  eprintf( "[MP         ] time=%d\n", dt1 );
  output( "mp", &a, &b, &ip );
  zPH3DDestroy( &ip );

  t1 = clock();
  zIntersectPH3DFast( &a, &b, &ip );
  t2 = clock();
  dt2 = t2 - t1;
  eprintf( "[MP(fast)   ] time=%d\n", dt2 );
  output( "mp2", &a, &b, &ip );
  zPH3DDestroy( &ip );

  t1 = clock();
  zIntersectPH3DBREP( &a, &b, &ip );
  t2 = clock();
  dt3 = t2 - t1;
  eprintf( "[B-rep      ] time=%d\n", dt3 );
  output( "brep", &a, &b, &ip );
  zPH3DDestroy( &ip );

  t1 = clock();
  zIntersectPH3DBREPFast( &a, &b, &ip );
  t2 = clock();
  dt4 = t2 - t1;
  eprintf( "[B-rep(fast)] time=%d\n", dt4 );
  output( "brep2", &a, &b, &ip );
  zPH3DDestroy( &ip );
  printf( "%d %d %d %d %d\n", n, dt1, dt2, dt3, dt4 );

  zPH3DDestroy( &a );
  zPH3DDestroy( &b );
  return 0;
}
