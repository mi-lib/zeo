#include <zeo/zeo_bv3d.h>

#define N 1000

void generate_points(zVec3DData *data, double x, double y, double z, double a, double b, double c, double wx, double wy, double wz, const char *filename)
{
  zMat3D ori;
  zVec3D org, d, v;
  int i;
  FILE *fp;

  zVec3DDataInitArray( data, N );
  printf( "vol=(%f x %f x %f)=%f\n", 2*wx, 2*wy, 2*wz, 8*wx*wy*wz );
  zVec3DCreate( &org, x, y, z );
  zMat3DFromZYX( &ori, zDeg2Rad(a), zDeg2Rad(b), zDeg2Rad(c) );
  fp = fopen( filename, "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &d, zRandF(-wx,wx), zRandF(-wy,wy), zRandF(-wz,wz) );
    zMulMat3DVec3DDRC( &ori, &d );
    zVec3DAdd( &org, &d, &v );
    zVec3DDataAdd( data, &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );
}

void output_obb(zBox3D *obb, const char *filename)
{
  FILE *fp;

  fp = fopen( filename, "w" );
  zBox3DValueFPrint( fp, obb );
  fclose( fp );
}

#define X 1.0
#define Y 1.0
#define Z 1.0
int main(void)
{
  zVec3DData data;
  zBox3D obb;

  zRandInit();
  generate_points( &data, X, Y, Z, 10, 20, 30, 2, 1, 3, "v" );
  zVec3DDataOBB( &data, &obb );
  printf( "vol=(%f x %f x %f)=%f\n", zBox3DDepth(&obb), zBox3DWidth(&obb), zBox3DHeight(&obb), zBox3DDepth(&obb)*zBox3DWidth(&obb)*zBox3DHeight(&obb) );
  output_obb( &obb, "b" );
  zVec3DDataDestroy( &data );
  return 0;
}
