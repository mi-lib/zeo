#include <zeo/zeo.h>

#define MODEL "../model/scc.z3d"

int main(void)
{
  zMShape3D m;

  zMShape3DReadFile( &m, MODEL );
  zMShape3DToPH( &m );
  zMShape3DWrite( &m );
  zMShape3DDestroy( &m );
  return 0;
}
