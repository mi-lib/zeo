#include <zeo/zeo_mshape.h>
#include <zeo/zeo_bv.h>

int main(void)
{
  zMShape3D m;
  zAABox3D aabb;
  zBox3D box;

  zMShape3DInit( &m );
  zArrayAlloc( &m.optic, zOpticalInfo, 1 );
  zOpticalInfoCreate( zMShape3DOptic(&m,0),
    0.8, 0.8, 0.8, 1.0, 1.0, 1.0, 0.4, 0.4, 0.4, 1.0, 1.0, 1.0, "white" );
  zArrayAlloc( &m.shape, zShape3D, 1 );
  zShape3DInit( zMShape3DShape(&m,0) );
  zNameSet( zMShape3DShape(&m,0), "box" );
  zShape3DSetOptic( zMShape3DShape(&m,0), zMShape3DOptic(&m,0) );
  zBox3DCreateAlign( &box, ZVEC3DZERO, 0.1, 0.6, 0.4 );
  zMShape3DShape(&m,0)->type = ZSHAPE_PH;
  zMShape3DShape(&m,0)->com = &zprim_ph3d_com;
  zBox3DToPH( &box, zShape3DPH(zMShape3DShape(&m,0)) );
  zAABB( &aabb, zShape3DVertBuf(zMShape3DShape(&m,0)), zShape3DVertNum(zMShape3DShape(&m,0)), NULL );
  zAABox3DToBox3D( &aabb, zShape3DBB(zMShape3DShape(&m,0)) );

  zMShape3DPrint( &m );
  zMShape3DDestroy( &m );
  return 0;
}
