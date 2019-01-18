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
    v[i].e[zX] += 0.15;
    v[i].e[zY] += 0.15;
    v[i].e[zZ] += 0.15;
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
  fprintf( fp, "alpha: 0.7\n" );
  fprintf( fp, "exp: 0.0\n\n" );
}

void output_red(FILE *fp)
{
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.7\n" );
  fprintf( fp, "exp: 0.0\n\n" );
}

void output_yellow(FILE *fp)
{
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.6 0.6 0.2\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 0.0\n\n" );
}

void output(zPH3D *a, zPH3D *b, zPH3D *sa, zPH3D *sb, zPH3D *c)
{
  FILE *fp;

  /* for visualization */
  fp = fopen( "org.ztk", "w" );
  output_blue( fp );
  output_red( fp );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFWrite( fp, a );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFWrite( fp, b );
  fclose( fp );

  fp = fopen( "cut.ztk", "w" );
  output_blue( fp );
  output_red( fp );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFWrite( fp, sa );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFWrite( fp, sb );
  fclose( fp );

  fp = fopen( "mrg.ztk", "w" );
  output_yellow( fp );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: c\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: yellow\n" );
  zPH3DFWrite( fp, c );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zPH3D a, b, sa, sb, c;
  clock_t t1, t2;

  test_ph( &a, &b );

  zPH3DTruncBREP( &a, &b, &sa );
  zPH3DTruncBREP( &b, &a, &sb );

  t1 = clock();
  zIntersectPH3DBREP( &a, &b, &c );
  t2 = clock();
  eprintf( "[BREP    ] time=%d\n", (int)(t2-t1) );
  output( &a, &b, &sa, &sb, &c );
  zPH3DDestroy( &c );

  t1 = clock();
  zIntersectPH3DBREPFast( &a, &b, &c );
  t2 = clock();
  eprintf( "[BREPFast] time=%d\n", (int)(t2-t1) );
  output( &a, &b, &sa, &sb, &c );
  zPH3DDestroy( &c );

  zPH3DDestroy( &a );
  zPH3DDestroy( &b );
  zPH3DDestroy( &sa );
  zPH3DDestroy( &sb );
  return 0;
}
