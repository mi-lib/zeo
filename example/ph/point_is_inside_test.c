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

void output_ph(FILE *fp, zPH3D *ph)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: polyhedron\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: cyan\n" );
  zPH3DFPrintZTK( fp, ph );
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

#define N 1000
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zVec3D bottom_loop[] = {
    { {-1,-1,-1 } },
    { {-2, 0,-1 } },
    { {-1, 1,-1 } },
    { { 1, 1,-1 } },
    { { 2, 0,-1 } },
    { { 1,-1,-1 } },
  };
  zVec3D vert = { { 0, 0, 2 } };
  zVec3D v;
  zPH3D ph;
  double margin;
  int i;
  FILE *fp;

  zRandInit();
  fp = fopen( "result.ztk", "w" );
  output_header( fp );
  zPH3DCreatePyramid( &ph, bottom_loop, sizeof(bottom_loop)/sizeof(zVec3D), &vert );
  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-2,2), zRandF(-2,2), zRandF(-2,2) );
    if( zPH3DPointIsInside( &ph, &v, margin ) )
      output_point_inside( fp, &v, i );
#if 0
    else
      output_point_outside( fp, &v, i );
#endif
  }
  output_ph( fp, &ph );
  fclose( fp );
  zPH3DDestroy( &ph );
  return 0;
}
