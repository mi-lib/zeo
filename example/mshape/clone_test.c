#include <zeo/zeo_mshape3d.h>

#define MODEL "../model/scc.ztk"

int main(void)
{
  zMShape3D src, *dest;

  zMShape3DReadZTK( &src, MODEL );
  dest = zMShape3DClone( &src );
  zMShape3DFPrintZTK( stdout, dest );
  zMShape3DDestroy( &src );
  zMShape3DDestroy( dest );
  return 0;
}
