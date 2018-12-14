#include <zeo/zeo_mat3d.h>

int main(void)
{
  zMat3D m1, m2, m;
  zVec3D om;
  double az, el, tl;

  printf( "enter azimuth-elevation-tilt: " );
  scanf( "%lf %lf %lf", &az, &el, &tl );
  zMat3DZYX( &m1, az, el, tl );
  printf( "entered matrix M1: " );
  zMat3DWrite( &m1 );

  printf( "enter outer product vector: " );
  zVec3DRead( &om );
  printf( "entered vector V:\n" );
  zVec3DWrite( &om );

  zVec3DOuterProdMat3D( &om, &m2 );
  printf( "outer product matrix M2: " );
  zMat3DWrite( &m2 );
  zMulMatMat3D( &m2, &m1, &m );
  printf( "M2 M1 = " );
  zMat3DWrite( &m );

  printf( "(direct multiplication of Vx to M1)\n" );
  zMulVecOPMat3D( &om, &m1, &m );
  printf( "Vx M1 = " );
  zMat3DWrite( &m );

  printf( "(destructive multiplication of Vx to M1)\n" );
  zMulVecOPMat3DDRC( &om, &m1 );
  printf( "Vx M1 = " );
  zMat3DWrite( &m1 );
  return 0;
}
