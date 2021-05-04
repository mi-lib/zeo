#include <zeo/zeo_ph.h>

void output(zPH3D *ph)
{
  FILE *fp;

  fp = fopen( "prism.ztk", "w" );
  /* for visualization */
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 0.8 0.8 0.8\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.8\n" );
  fprintf( fp, "exp: 1.0\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: prism\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: white\n" );
  zPH3DFPrintZTK( fp, ph );
  fclose( fp );
}

int main(void)
{
  zVec3D v[] = {
    { { 0.0,  0.0, 0 } },
    { {-0.05, 0.05, 0 } },
    { {-0.05, 0.1, 0 } },
    { { 0.05, 0.1, 0 } },
    { { 0.1,  0.2, 0 } },
    { { 0.05, 0.25, 0 } },
    { { 0.05, 0.3, 0 } },
    { { 0.15, 0.3, 0 } },
    { { 0.15, 0.0, 0 } },
  };
  zVec3D shift = { { 0, 0.05, 0.25 } };
  zPH3D prism;

  zPH3DPrism( &prism, v, sizeof(v)/sizeof(zVec3D), &shift );
  output( &prism );
  zPH3DDestroy( &prism );
  return 0;
}
