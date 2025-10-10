#include <zeo/zeo_mshape3d.h>
#include <zeo/zeo_bv3d.h>

int main(void)
{
  zMShape3D m;

  zMShape3DInit( &m );
  zMShape3DAllocOpticArray( &m, 1 );
  zOpticalInfoCreate( zMShape3DOptic(&m,0),
    0.8, 0.8, 0.8, 1.0, 1.0, 1.0, 0.4, 0.4, 0.4, 1.0, 1.0, 1.0, "white" );
  zMShape3DAllocShapeArray( &m, 1 );
  zShape3DInit( zMShape3DShape(&m,0) );
  zShape3DBoxCreateAlign( zMShape3DShape(&m,0), ZVEC3DZERO, 0.1, 0.6, 0.4 );
  zShape3DToPH( zMShape3DShape(&m,0) );
  zNameSet( zMShape3DShape(&m,0), "box" );
  zShape3DSetOptic( zMShape3DShape(&m,0), zMShape3DOptic(&m,0) );

  zMShape3DFPrintZTK( stdout, &m );
  zMShape3DDestroy( &m );
  return 0;
}
