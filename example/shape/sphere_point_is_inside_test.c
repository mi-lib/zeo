#include <zeo/zeo_shape3d.h>

void output_header(FILE *fp)
{
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 1.0 1.0 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.8\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 1.0 0.0 0.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: cyan\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 0.4 1.0 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.3\n" );
  fprintf( fp, "esr: 1.0\n\n" );
}

void output_sphere(FILE *fp, zSphere3D *sphere)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: sphere\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: cyan\n" );
  fprintf( fp, "center: " );
  zVec3DValueNLFPrint( fp, zSphere3DCenter(sphere) );
  fprintf( fp, "radius: %g\n\n", zSphere3DRadius(sphere) );
}

void output_point_inside(FILE *fp, zVec3D *p, int i)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: point%d\n", i );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: red\n" );
  fprintf( fp, "center: " );
  zVec3DValueNLFPrint( fp, p );
  fprintf( fp, "radius: 0.1\n\n" );
}

void output_point_outside(FILE *fp, zVec3D *p, int i)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: point%d\n", i );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: white\n" );
  fprintf( fp, "center: " );
  zVec3DValueNLFPrint( fp, p );
  fprintf( fp, "radius: 0.05\n\n" );
}

#define N 500
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zVec3D c, v;
  zSphere3D sphere;
  double margin;
  int i;
  FILE *fp;

  zRandInit();
  fp = fopen( "result.ztk", "w" );
  output_header( fp );
  zVec3DCreate( &c, 1, 1, 1 );
  zSphere3DCreate( &sphere, &c, 2, 0 );

  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    if( zSphere3DPointIsInside( &sphere, &v, margin ) )
      output_point_inside( fp, &v, i );
    else
      output_point_outside( fp, &v, i );
  }
  output_sphere( fp, &sphere );
  fclose( fp );
  return 0;
}
