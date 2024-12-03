#include <zeo/zeo_shape3d.h>

void generate_pc_sphere(zSphere3D *s, zVec3DList *pc, int n)
{
  register int i;
  zVec3D v;

  zListInit( pc );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
    zVec3DMulDRC( &v, (zSphere3DRadius(s)+zRandF(-0.001,0.001))/zVec3DNorm(&v) );
    zVec3DAddDRC( &v, zSphere3DCenter(s) );
    zVec3DListAdd( pc, &v );
  }
}

void output_pc_sphere(zSphere3D *s, zVec3DList *pc)
{
  FILE *fp;
  zVec3DListCell *c;
  register int i = 0;

  fp = fopen( "sf.ztk", "w" );

  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.6 0.6 0.6\n" );
  fprintf( fp, "diffuse: 1.0 1.0 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: orange\n" );
  fprintf( fp, "ambient: 0.6 0.6 0.6\n" );
  fprintf( fp, "diffuse: 0.9 0.4 0.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );

  zListForEach( pc, c ){
    fprintf( fp, "[shape]\n" );
    fprintf( fp, "name: point%02d\n", i++ );
    fprintf( fp, "type: sphere\n" );
    fprintf( fp, "optic: white\n" );
    fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, c->data );
    fprintf( fp, "radius: 0.01\n\n" );
    fprintf( fp, "div: 4\n\n" );
  }
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: sphere\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: orange\n" );
  zSphere3DFPrintZTK( fp, s );
  fprintf( fp, "div: 8\n\n" );

  fclose( fp );
}

#define N 10

int main(int argc, char *argv[])
{
  zSphere3D sorg, sout;
  zVec3D c;
  zVec3DList pc;
  int n;

  n = argc > 1 ? atoi( argv[1] ) : N;
  zRandInit();
  zVec3DCreate( &c, 0.05, 0.06, 0.07 );
  zSphere3DCreate( &sorg, &c, 0.15, 0 );
  eprintf( "<original sphere>\n" );
  zSphere3DFPrintZTK( stderr, &sorg );
  generate_pc_sphere( &sorg, &pc, n );
  zSphere3DFit( &sout, &pc );
  eprintf( "<estimated sphere>\n" );
  zSphere3DFPrintZTK( stderr, &sout );
  output_pc_sphere( &sout, &pc );
  printf( "number of points = %d\n", n );
  printf( "relative center gap = %g\n", zVec3DDist(zSphere3DCenter(&sorg),zSphere3DCenter(&sout))/zSphere3DRadius(&sorg) );
  printf( "relative radius gap = %g\n", fabs(zSphere3DRadius(&sorg)-zSphere3DRadius(&sout))/zSphere3DRadius(&sorg) );
  zVec3DListDestroy( &pc );
  return 0;
}
