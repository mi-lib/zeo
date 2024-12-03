#include <zeo/zeo_elem3d.h>

#define N 1000
zVec3D vert[N];

void test_vert(double x, double y, double z, double a, double b, double c, double wx, double wy, double wz)
{
  register int i;
  zMat3D ori;

  zMat3DFromZYX( &ori, zDeg2Rad(a), zDeg2Rad(b), zDeg2Rad(c) );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &vert[i],
      zRandF(-wx,wx), zRandF(-wy,wy), zRandF(-wz,wz) );
    zMulMat3DVec3DDRC( &ori, &vert[i] );
    vert[i].e[zX] += x;
    vert[i].e[zY] += y;
    vert[i].e[zZ] += z;
  }
}

void output_vert(void)
{
  FILE *fp;
  register int i;
  zPlane3D pl;
  zVec3D v, c, d[3];

  fp = fopen( "a", "w" );
  for( i=0; i<N; i++ )
    zVec3DValueNLFPrint( fp, &vert[i] );
  fclose( fp );

  zVec3DPCA( vert, N, d );
  fp = fopen( "b", "w" );
  zPlane3DCreate( &pl, ZVEC3DZERO, &d[0] );
  for( i=0; i<N; i++ ){
    zPlane3DProjPoint( &pl, &vert[i], &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );
  fp = fopen( "c", "w" );
  zPlane3DCreate( &pl, ZVEC3DZERO, &d[1] );
  for( i=0; i<N; i++ ){
    zPlane3DProjPoint( &pl, &vert[i], &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );
  fp = fopen( "d", "w" );
  zPlane3DCreate( &pl, ZVEC3DZERO, &d[2] );
  for( i=0; i<N; i++ ){
    zPlane3DProjPoint( &pl, &vert[i], &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );

  zVec3DBaryPCA( vert, N, &c, d );
  fp = fopen( "e", "w" );
  zPlane3DCreate( &pl, &c, &d[0] );
  for( i=0; i<N; i++ ){
    zPlane3DProjPoint( &pl, &vert[i], &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );
  fp = fopen( "f", "w" );
  zPlane3DCreate( &pl, &c, &d[1] );
  for( i=0; i<N; i++ ){
    zPlane3DProjPoint( &pl, &vert[i], &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );
  fp = fopen( "g", "w" );
  zPlane3DCreate( &pl, &c, &d[2] );
  for( i=0; i<N; i++ ){
    zPlane3DProjPoint( &pl, &vert[i], &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );
}

#define X 1
#define Y 0.5
#define Z 1.5
int main(void)
{
  zRandInit();
  test_vert( X, Y, Z, 10, 20, 30, 2, 1, 3 );
  output_vert();
  return 0;
}
