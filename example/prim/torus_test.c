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
    { { 0.10, 0.01, 0.00 } },
    { { 0.15, 0.00, 0.05 } },
    { { 0.20, 0.00, 0.00 } },
    { { 0.15,-0.01,-0.05 } },
  };
#if 1
  zVec3D center = { { 0, 0, 0 } };
  zVec3D axis   = { { 0, 0, 1 } };
#elif 0
  zVec3D center = { { 0, 0, 0 } };
  zVec3D axis   = { { 1, 0, 0 } };
#elif 0
  zVec3D center = { { 0, 0, 0.2 } };
  zVec3D axis   = { { 1, 0, 0 } };
#else
  zVec3D center = { { 0, 0, 0 } };
  zVec3D axis   = { { 0, 1, 0 } };
#endif
  zPH3D torus;

  zPH3DTorus( &torus, v, sizeof(v)/sizeof(zVec3D), DIV, &center, &axis );
  output( &torus );
  eprintf( "volume=%f\n", zPH3DVolume(&torus) );
  zPH3DDestroy( &torus );
  return 0;
}
