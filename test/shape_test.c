#include <zeo/zeo_shape3d.h>

void generate_box_rand(zShape3D *box)
{
  zVec3D center, ax, ay, az, tmp;

  zVec3DCreate( &center, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DCreate( &ax, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &tmp, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DOrthogonalize( &tmp, &ax, &ay );
  zVec3DOuterProd( &ax, &ay, &az );
  zVec3DNormalizeDRC( &ax );
  zVec3DNormalizeDRC( &ay );
  zVec3DNormalizeDRC( &az );
  zShape3DBoxCreate( box, &center, &ax, &ay, &az, zRandF(0.1,5), zRandF(0.1,5), zRandF(0.1,5) );
}

void assert_shape_inertia(void)
{
  zShape3D box, ph;
  zMat3D i, iph;

  generate_box_rand( &box );
  zShape3DClone( &box, &ph, NULL );
  zShape3DToPH( &ph );
  zAssert( zShape3DVolume, zIsTiny( zShape3DVolume( &box ) - zShape3DVolume( &ph ) ) );

  zShape3DBaryInertia( &box, 1, &i );
  zShape3DBaryInertia( &ph, 1, &iph );
  zMat3DSubDRC( &i, &iph );
  zAssert( zShape3DBaryInertia, zMat3DIsTiny( &i ) );
  zShape3DInertia( &box, 1, &i );
  zShape3DInertia( &ph, 1, &iph );
  zMat3DSubDRC( &i, &iph );
  zAssert( zShape3DInertia, zMat3DIsTiny( &i ) );
  zShape3DDestroy( &box );
  zShape3DDestroy( &ph );
}

int main(int argc, char *argv[])
{
  assert_shape_inertia();
  return 0;
}
