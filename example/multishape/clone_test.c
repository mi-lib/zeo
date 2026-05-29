#include <zeo/zeo_multishape3d.h>

#define MODEL "../model/scc.ztk"

int main(void)
{
  zMultiShape3D src, *dest;

  zMultiShape3DReadZTK( &src, MODEL );
  dest = zMultiShape3DClone( &src );
  zMultiShape3DFPrintZTK( stdout, dest );
  zMultiShape3DDestroy( &src );
  zMultiShape3DDestroy( dest );
  return 0;
}
