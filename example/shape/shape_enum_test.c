#include <zeo/zeo_shape.h>

void check(char *str)
{
  printf( "%s -> %s\n", str, zShapeTypeExpr(zShapeTypeByStr(str) ) );
}

int main(void)
{
  check( "none" );
  check( "polyhedron" );
  check( "box" );
  check( "sphere" );
  check( "ellipsoid" );
  check( "cylinder" );
  check( "cone" );
  check( "nurbs" );
  return 0;
}
