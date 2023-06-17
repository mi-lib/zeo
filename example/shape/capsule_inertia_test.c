#include <zeo/zeo_shape3d.h>

int main(void)
{
  zCapsule3D capsule;
  zVec3D c1, c2;
  zVec3D p, cp;
  zMat3D inertia, inertia_ph;
  zPH3D ph;
  int div;

  zRandInit();
  zVec3DCreate( &c1, zRandF(-0.1,0.1), zRandF(-0.1,0.1), zRandF(-0.1,0.1) );
  zVec3DCreate( &c2, zRandF(-0.1,0.1), zRandF(-0.1,0.1), zRandF(-0.1,0.1)+0.2 );
  zCapsule3DCreate( &capsule, &c1, &c2, 0.1, 36 );

  zVec3DCreate( &p, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.3) );
  zCapsule3DClosest( &capsule, &p, &cp );

  zVec3DCreate( &c1, 0, 0, -1 );
  zVec3DCreate( &c2, 0, 0,  1 );
  for( div=16; div<=512; div*=2 ){
    zCapsule3DCreate( &capsule, &c1, &c2, 0.5, div );
    zCapsule3DBaryInertia( &capsule, 1.0, &inertia );
    zCapsule3DToPH( &capsule, &ph );
    zPH3DBaryInertia( &ph, 1.0, &inertia_ph );
    printf( "div=%d %g %g %g\n", div, (inertia.c.xx-inertia_ph.c.xx)/inertia.c.xx, (inertia.c.yy-inertia_ph.c.yy)/inertia.c.yy, (inertia.c.zz-inertia_ph.c.zz)/inertia.c.zz );
    zPH3DDestroy( &ph );
  }
  return 0;
}
