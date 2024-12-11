#include <zeo/zeo_shape3d.h>

void generate_data_sphere(zSphere3D *s, zVec3DData *data, int n)
{
  int i;
  zVec3D v;

  zVec3DDataInitArray( data, n );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
    zVec3DMulDRC( &v, (zSphere3DRadius(s)+zRandF(-0.001,0.001))/zVec3DNorm(&v) );
    zVec3DAddDRC( &v, zSphere3DCenter(s) );
    zVec3DDataAdd( data, &v );
  }
}

void output_data_sphere(zSphere3D *s, zVec3DData *data)
{
  FILE *fp;
  zVec3D *v;
  int i = 0;

  fp = fopen( "fit_sphere.ztk", "w" );

  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.6 0.6 0.6\n" );
  fprintf( fp, "diffuse: 1.0 1.0 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: orange\n" );
  fprintf( fp, "ambient: 0.6 0.6 0.6\n" );
  fprintf( fp, "diffuse: 0.9 0.4 0.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );

  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    fprintf( fp, "[zeo::shape]\n" );
    fprintf( fp, "name: point%02d\n", i++ );
    fprintf( fp, "type: sphere\n" );
    fprintf( fp, "optic: white\n" );
    fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, v );
    fprintf( fp, "radius: 0.01\n" );
    fprintf( fp, "div: 4\n\n" );
  }
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: sphere\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: orange\n" );
  zSphere3DSetDiv( s, 8 );
  zSphere3DFPrintZTK( fp, s );

  fclose( fp );
}

#define N 10

int main(int argc, char *argv[])
{
  zSphere3D sorg, sout;
  zVec3D c;
  zVec3DData data;
  int n;

  n = argc > 1 ? atoi( argv[1] ) : N;
  zRandInit();
  zVec3DCreate( &c, 0.5, 0.6, 0.7 );
  zSphere3DCreate( &sorg, &c, 0.15, 0 );
  eprintf( "<original sphere>\n" );
  zSphere3DFPrintZTK( stderr, &sorg );
  generate_data_sphere( &sorg, &data, n );
  zSphere3DFit( &sout, &data );
  eprintf( "<estimated sphere>\n" );
  zSphere3DFPrintZTK( stderr, &sout );
  output_data_sphere( &sout, &data );
  printf( "number of points = %d\n", n );
  printf( "relative center gap = %g\n", zVec3DDist(zSphere3DCenter(&sorg),zSphere3DCenter(&sout))/zSphere3DRadius(&sorg) );
  printf( "relative radius gap = %g\n", fabs(zSphere3DRadius(&sorg)-zSphere3DRadius(&sout))/zSphere3DRadius(&sorg) );
  zVec3DDataDestroy( &data );
  return 0;
}
