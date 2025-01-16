#include <zeo/zeo_vec3d_data.h>

#define N 1000

void test_vert(zVec3DData *data, double x, double y, double z, double a, double b, double c, double wx, double wy, double wz)
{
  zMat3D ori;
  zVec3D v;
  int i;

  zVec3DDataInitArray( data, N );
  zMat3DFromZYX( &ori, zDeg2Rad(a), zDeg2Rad(b), zDeg2Rad(c) );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-wx,wx), zRandF(-wy,wy), zRandF(-wz,wz) );
    zMulMat3DVec3DDRC( &ori, &v );
    v.e[zX] += x;
    v.e[zY] += y;
    v.e[zZ] += z;
    zVec3DDataAdd( data, &v );
  }
}

void output_projection(FILE *fp, zVec3DData *data, zVec3D *center, zVec3D *norm)
{
  zPlane3D pl;
  zVec3D *v, p;

  zPlane3DCreate( &pl, center, norm );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zPlane3DProjPoint( &pl, v, &p );
    zVec3DValueNLFPrint( fp, &p );
  }
}

void output_vert(zVec3DData *data)
{
  FILE *fp;
  double eval[3];
  zVec3D evec[3], c;

  fp = fopen( "a", "w" );
  zVec3DDataValueFPrint( fp, data );
  fclose( fp );

  zVec3DDataPCA( data, ZVEC3DZERO, eval, evec );
  fp = fopen( "b", "w" );
  output_projection( fp, data, ZVEC3DZERO, &evec[0] );
  fclose( fp );
  fp = fopen( "c", "w" );
  output_projection( fp, data, ZVEC3DZERO, &evec[1] );
  fclose( fp );
  fp = fopen( "d", "w" );
  output_projection( fp, data, ZVEC3DZERO, &evec[2] );
  fclose( fp );

  zVec3DDataBaryPCA( data, &c, eval, evec );
  fp = fopen( "e", "w" );
  output_projection( fp, data, &c, &evec[0] );
  fclose( fp );
  fp = fopen( "f", "w" );
  output_projection( fp, data, &c, &evec[1] );
  fclose( fp );
  fp = fopen( "g", "w" );
  output_projection( fp, data, &c, &evec[2] );
  fclose( fp );
}

#define X 1
#define Y 0.5
#define Z 1.5

int main(void)
{
  zVec3DData data;

  zRandInit();
  test_vert( &data, X, Y, Z, 10, 20, 30, 2, 1, 3 );
  output_vert( &data );
  return 0;
}
