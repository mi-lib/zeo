#include <zeo/zeo_shape3d.h>

void shape_output(zShape3D *s)
{
  printf( "[shape]\n" );
  zShape3DFPrintZTK( stdout, s );
}

void shape_toph_and_destroy(zShape3D *s)
{
  zShape3DToPH( s );
  printf( "[shape]\n" );
  zShape3DFPrintZTK( stdout, s );
  zShape3DDestroy( s );
}

#define DIV 24

int main(void)
{
  zShape3D s;
  zVec3D c1, c2;

  zShape3DInit( &s );
  /* box */
  zVec3DCreate( &c1, 0, -5, 0 );
  zShape3DBoxCreateAlign( &s, &c1, 4, 2, 1 );
  shape_output( &s );
  c1.c.z -= 5;
  zShape3DBoxCreateAlign( &s, &c1, 4, 2, 1 );
  shape_toph_and_destroy( &s );
  /* sphere */
  zVec3DCreate( &c1, 0, -3, 0 );
  zShape3DSphereCreate( &s, &c1, 1, DIV );
  shape_output( &s );
  c1.c.z -= 5;
  zShape3DSphereCreate( &s, &c1, 1, DIV );
  shape_toph_and_destroy( &s );
  /* cylinder */
  zVec3DCreate( &c1, 0, -1,-2 );
  zVec3DCreate( &c2, 0, -1, 2 );
  zShape3DCylCreate( &s, &c1, &c2, 1, DIV );
  shape_output( &s );
  c1.c.z -= 5;
  c2.c.z -= 5;
  zShape3DCylCreate( &s, &c1, &c2, 1, DIV );
  shape_toph_and_destroy( &s );
  /* cone */
  zVec3DCreate( &c1, 0, 1,-2 );
  zVec3DCreate( &c2, 0, 1, 2 );
  zShape3DConeCreate( &s, &c1, &c2, 1, DIV );
  shape_output( &s );
  c1.c.z -= 5;
  c2.c.z -= 5;
  zShape3DConeCreate( &s, &c1, &c2, 1, DIV );
  shape_toph_and_destroy( &s );
  /* ellipsoid */
  zVec3DCreate( &c1, 0, 3, 0 );
  zShape3DEllipsCreateAlign( &s, &c1, 2, 0.5, 1, DIV );
  shape_output( &s );
  c1.c.z -= 5;
  zShape3DEllipsCreateAlign( &s, &c1, 2, 0.5, 1, DIV );
  shape_toph_and_destroy( &s );

  return 0;
}
