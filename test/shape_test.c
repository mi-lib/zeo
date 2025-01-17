#include <zeo/zeo_shape3d.h>

void assert_box_to_aabox(void)
{
  zAABox3D aabox;
  zBox3D box;
  zVec3D vert_aabox;
  zVec3D vert_box;
  zVec3D ax, ay;
  int i;
  bool result = true;

  zAABox3DCreate( &aabox, -10, -20, -30, 30, 20, 10 );
  zAABox3DToBox3D( &aabox, &box );
  for( i=0; i<8; i++ ){
    zAABox3DVert( &aabox, i, &vert_aabox );
    zBox3DVert( &box, i, &vert_box );
    if( !zVec3DEqual( &vert_aabox, &vert_box ) ) result = false;
  }
  zAssert( zAABox3DVert + zBox3DVert + zAABox3DToBox3D, result );

  zVec3DCreate( &ax, 1, 1, 0 );
  zVec3DCreate( &ay,-1, 1, 0 );
  zBox3DCreate( &box, ZVEC3DZERO, &ax, &ay, ZVEC3DZ, sqrt(2), sqrt(2), 2 );
  zBox3DToAABox3D( &box, &aabox );
  zVec3DCreate( &vert_aabox,-1,-1,-1 );
  if( !zVec3DEqual( &vert_aabox, &aabox.min ) ) result = false;
  zVec3DCreate( &vert_aabox, 1, 1, 1 );
  if( !zVec3DEqual( &vert_aabox, &aabox.max) ) result = false;
  zAssert( zBox3DToAABox3D, result );
}

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
  assert_box_to_aabox();
  assert_shape_inertia();
  return 0;
}
