#include <zeo/zeo_prim.h>

void output(zPH3D *ph)
{
  /* for visualization */
  printf( "[optic]\n" );
  printf( "name: white\n" );
  printf( "ambient: 0.8 0.8 0.8\n" );
  printf( "diffuse: 0.8 0.8 0.8\n" );
  printf( "specular: 0.0 0.0 0.0\n" );
  printf( "alpha: 0.8\n" );
  printf( "exp: 1.0\n\n" );

  printf( "[shape]\n" );
  printf( "name: prism\n" );
  printf( "type: polyhedron\n" );
  printf( "optic: white\n" );
  zPH3DPrint( ph );
}

#define DIV 100
int main(void)
{
  zVec3D v[] = {
#if 0
    { { 0.10, 0.0, 0.1 } },
    { { 0.15, 0.0, 0.0 } },
    { { 0.10, 0.0,-0.1 } },
#else /* reversed volume case */
    { { 0.10, 0.0,-0.1 } },
    { { 0.15, 0.0, 0.0 } },
    { { 0.10, 0.0, 0.1 } },
#endif
  };
  zVec3D center = { { 0, 0, 0 } };
  zVec3D axis   = { { 0, 0, 1 } };
  zPH3D lathe;

  zPH3DLathe( &lathe, v, sizeof(v)/sizeof(zVec3D), DIV, &center, &axis );
  output( &lathe );
  eprintf( "volume=%f\n", zPH3DVolume(&lathe) );
  zPH3DDestroy( &lathe );
  return 0;
}
