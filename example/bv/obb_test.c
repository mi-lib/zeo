#include <zeo/zeo_bv.h>

#define N 1000
zVec3D vert[N];

void test_vert(double x, double y, double z, double a, double b, double c, double wx, double wy, double wz)
{
  register int i;
#if 1
  zMat3D ori;
  zVec3D org, d;

  printf( "vol=(%f x %f x %f)=%f\n", 2*wx, 2*wy, 2*wz, 8*wx*wy*wz );
  zVec3DCreate( &org, x, y, z );
  zMat3DZYX( &ori, zDeg2Rad(a), zDeg2Rad(b), zDeg2Rad(c) );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &d, zRandF(-wx,wx), zRandF(-wy,wy), zRandF(-wz,wz) );
    zMulMatVec3DDRC( &ori, &d );
    zVec3DAdd( &org, &d, &vert[i] );
  }
#else
  for( i=0; i<N; i++ )
    zVec3DCreate( &vert[i], x+zRandF(-wx,wx), y+zRandF(-wy,wy), z+zRandF(-wz,wz) );
#endif
}

#define X 1.0
#define Y 1.0
#define Z 1.0
int main(void)
{
  FILE *fp;
  register int i;
  zVec3D p;
  zBox3D obb;

  zRandInit();
  test_vert( X, Y, Z, 10, 20, 30, 2, 1, 3 );

  zOBB( &obb, vert, N );
  printf( "vol=(%f x %f x %f)=%f\n", zBox3DDepth(&obb), zBox3DWidth(&obb), zBox3DHeight(&obb), zBox3DDepth(&obb)*zBox3DWidth(&obb)*zBox3DHeight(&obb) );

  fp = fopen( "a", "w" );
  for( i=0; i<N; i++ )
    zVec3DDataNLFWrite( fp, &vert[i] );
  fclose( fp );

  fp = fopen( "b", "w" );
  zVec3DDataNLFWrite( fp, zBox3DCenter(&obb) );
  for( i=0; i<8; i++ ){
    zBox3DVert( &obb, i, &p );
    zVec3DDataNLFWrite( fp, &p );
  }
  fclose( fp );
  return 0;
}
