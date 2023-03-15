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

void output_cyl(FILE *fp, zCyl3D *cyl)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: cylinder\n" );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: cyan\n" );
  fprintf( fp, "center: " );
  zVec3DDataNLFPrint( fp, zCyl3DCenter(cyl,0) );
  fprintf( fp, "center: " );
  zVec3DDataNLFPrint( fp, zCyl3DCenter(cyl,1) );
  fprintf( fp, "radius: %g\n\n", zCyl3DRadius(cyl) );
}

void output_point_inside(FILE *fp, zVec3D *p, int i)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: point%d\n", i );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: red\n" );
  fprintf( fp, "center: " );
  zVec3DDataNLFPrint( fp, p );
  fprintf( fp, "radius: 0.1\n\n" );
}

void output_point_outside(FILE *fp, zVec3D *p, int i)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: point%d\n", i );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: white\n" );
  fprintf( fp, "center: " );
  zVec3DDataNLFPrint( fp, p );
  fprintf( fp, "radius: 0.05\n\n" );
}

#define N 500
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zVec3D c1, c2, v;
  zCyl3D cyl;
  double margin;
  int i;
  FILE *fp;

  zRandInit();
  fp = fopen( "result.ztk", "w" );
  output_header( fp );
  zVec3DCreate( &c1, 1, 1, -2 );
  zVec3DCreate( &c2,-1,-1, 2 );
  zCyl3DCreate( &cyl, &c1, &c2, 2, 0 );

  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    if( zCyl3DPointIsInside( &cyl, &v, margin ) )
      output_point_inside( fp, &v, i );
    else
      output_point_outside( fp, &v, i );
  }
  output_cyl( fp, &cyl );
  fclose( fp );
  return 0;
}
