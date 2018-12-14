#include <zeo/zeo_vec3d.h>

void test(double a, double b, double c)
{
  zVec3D v;

  zVec3DCreate( &v, a, b, c );
  zVec3DWrite( &v );
  printf( "vector is %s.\n", zVec3DIsNan(&v) ? "NaN" : "aN" );
}

int main(void)
{
  test( 0, 0, 0 );
  test( NAN, 0, 0 );
  test( 0, NAN, 0 );
  test( 0, 0, NAN );
  test( HUGE_VAL, 0, 0 );
  test( 0, HUGE_VAL, 0 );
  test( 0, 0, HUGE_VAL );
  test( NAN, HUGE_VAL, 0 );
  return 0;
}
