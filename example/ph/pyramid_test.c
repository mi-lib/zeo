#include <zeo/zeo_ph3d.h>

void output(zPH3D *ph)
{
  FILE *fp;

  fp = fopen( "pyramid.ztk", "w" );
  /* for visualization */
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 0.8 0.8 0.8\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.8\n" );
  fprintf( fp, "esr: 1.0\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: pyramid\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: white\n" );
  zPH3DFPrintZTK( fp, ph );
  fclose( fp );
}

int main(void)
{
  zVec3D v[] = {
#if 1
    { { 0.0, 0.0, 0 } },
    { {-0.1, 0.1, 0 } },
    { {-0.1, 0.2, 0 } },
    { { 0.1, 0.2, 0 } },
    { { 0.2, 0.4, 0 } },
    { { 0.1, 0.5, 0 } },
    { { 0.1, 0.6, 0 } },
    { { 0.3, 0.6, 0 } },
    { { 0.3, 0.0, 0 } },
#elif 0
    { { 0.0, 0.0, 0 } },
    { { 0.0, 0.1, 0 } },
    { { 0.1, 0.0, 0 } },
#else
    { { 0.0, 0.0, 0 } },
    { { 0.1, 0.0, 0 } },
    { { 0.0, 0.1, 0 } },
#endif
  };
  zVec3D vert = { { 0, 0.1, 0.5 } };
  zPH3D pyr;

  zPH3DCreatePyramid( &pyr, v, sizeof(v)/sizeof(zVec3D), &vert );
  output( &pyr );
  zPH3DDestroy( &pyr );
  return 0;
}
