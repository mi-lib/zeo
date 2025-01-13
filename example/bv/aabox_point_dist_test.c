#include <zeo/zeo_bv3d.h>

#define N 1000

void assert_aabox_point_dist(void)
{
  zAABox3D aabox;
  zVec3D point, cp;
  int i;

  zAABox3DCreate( &aabox, zRandF(-5,0), zRandF(-5,0), zRandF(-5,0), zRandF(0,5), zRandF(0,5), zRandF(0,5) );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &point, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zAABox3DClosest( &aabox, &point, &cp );
    zVec3DValueNLPrint( &point );
    zVec3DValueNLPrint( &cp );
    zEndl();
    zEndl();
  }
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_aabox_point_dist();
  return 0;
}
