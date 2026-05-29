#include <zeo/zeo_multishape3d.h>
#include <zeo/zeo_bv3d.h>

int main(void)
{
  zMultiShape3D m;

  zMultiShape3DInit( &m );
  zMultiShape3DAllocOpticArray( &m, 1 );
  zOpticalInfoCreate( zMultiShape3DOptic(&m,0),
    0.8, 0.8, 0.8, 1.0, 1.0, 1.0, 0.4, 0.4, 0.4, 1.0, 1.0, 1.0, "white" );
  zMultiShape3DAllocShapeArray( &m, 1 );
  zShape3DInit( zMultiShape3DShape(&m,0) );
  zShape3DBoxCreateAlign( zMultiShape3DShape(&m,0), ZVEC3DZERO, 0.1, 0.6, 0.4 );
  zShape3DToPH( zMultiShape3DShape(&m,0) );
  zNameSet( zMultiShape3DShape(&m,0), "box" );
  zShape3DSetOptic( zMultiShape3DShape(&m,0), zMultiShape3DOptic(&m,0) );

  zMultiShape3DFPrintZTK( stdout, &m );
  zMultiShape3DDestroy( &m );
  return 0;
}
