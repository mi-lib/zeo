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
  printf( "name: pyramid\n" );
  printf( "type: polyhedron\n" );
  printf( "optic: white\n" );
  zPH3DWrite( ph );
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

  zPH3DPyramid( &pyr, v, sizeof(v)/sizeof(zVec3D), &vert );
  output( &pyr );
  zPH3DDestroy( &pyr );
  return 0;
}
