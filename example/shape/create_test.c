#include <zeo/zeo.h>

void shape_output(zShape3D *s)
{
  printf( "[shape]\n" );
  zShape3DPrint( s );
  zShape3DDestroy( s );
}

int main(void)
{
  zShape3D s;
  zVec3D center = { { 0.1, 0.2, 0.3 } };
  zVec3D c1[] = { { { 0, 0, 0 } }, { { 0.1,-0.1, 0.3 } } };
  zVec3D c2[] = { { { 0, 0, 0 } }, { {-0.3, 0.2, 0.1 } } };

  zShape3DInit( &s );

  /* box */
  zShape3DCreateBoxAlign( &s, ZVEC3DZERO, 0.1, 0.1, 0.1 );
  shape_output( &s );
  /* sphere */
  zShape3DCreateSphere( &s, &center, 0.1, 0 );
  shape_output( &s );
  /* ellipsoid */
  zShape3DCreateEllipsAlign( &s, &c2[1], 0.1, 0.15, 0.05, 0 );
  shape_output( &s );
  /* cylinder */
  zShape3DCreateCyl( &s, &c1[0], &c1[1], 0.06, 0 );
  shape_output( &s );
  /* cone */
  zShape3DCreateCone( &s, &c2[0], &c2[1], 0.06, 0 );
  shape_output( &s );

  return 0;
}
