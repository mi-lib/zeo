#include <zeo/zeo_ph.h>

void output(zPH3D *ph)
{
  FILE *fp;

  fp = fopen( "lathe.ztk", "w" );
  /* for visualization */
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 0.8 0.8 0.8\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 1.0\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: lathe\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: white\n" );
  zPH3DFPrint( fp, ph );
  fclose( fp );
}

#define DIV 100
int main(void)
{
  zVec3D v[] = {
    { { 0.10, 0.0, 0.1 } },
    { { 0.15, 0.0, 0.0 } },
    { { 0.10, 0.0,-0.1 } },
  };
  zVec3D center = { { 0, 0, 0 } };
  zVec3D axis   = { { 0, 0, 1 } };
  zPH3D lathe;

  zPH3DLathe( &lathe, v, sizeof(v)/sizeof(zVec3D), DIV, &center, &axis );
  output( &lathe );
  zPH3DDestroy( &lathe );
  return 0;
}
