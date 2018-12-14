#include <zeo/zeo.h>

#define MODEL "../model/scc.z3d"

int main(void)
{
  zMShape3D src, *dest;

  zMShape3DReadFile( &src, MODEL );
  dest = zMShape3DClone( &src );
  zMShape3DWrite( dest );
  zMShape3DDestroy( &src );
  zMShape3DDestroy( dest );
  return 0;
}
