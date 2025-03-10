#include <time.h>
#include <zeo/zeo_vec3d_data.h>

#define N 1000

void output_boundingbox(zFrame3D *frame, double wx, double wy, double wz)
{
  zVec3D v[8];
  FILE *fp;
  uint i;

  for( i=0; i<8; i++ ){
    zVec3DCreate( &v[i], i & 0x1 ? -wx : wx, i & 0x2 ? -wy : wy, i & 0x4 ? -wz : wz );
    zXform3DDRC( frame, &v[i] );
  }
  fp = fopen( "s", "w" );
  zVec3DValueNLFPrint( fp, &v[0] );
  zVec3DValueNLFPrint( fp, &v[1] );
  zVec3DValueNLFPrint( fp, &v[3] );
  zVec3DValueNLFPrint( fp, &v[2] );
  zVec3DValueNLFPrint( fp, &v[0] );
  fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[0] ); zVec3DValueNLFPrint( fp, &v[4] ); fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[1] ); zVec3DValueNLFPrint( fp, &v[5] ); fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[3] ); zVec3DValueNLFPrint( fp, &v[7] ); fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[2] ); zVec3DValueNLFPrint( fp, &v[6] ); fprintf( fp, "\n" );

  zVec3DValueNLFPrint( fp, &v[4] );
  zVec3DValueNLFPrint( fp, &v[5] );
  zVec3DValueNLFPrint( fp, &v[7] );
  zVec3DValueNLFPrint( fp, &v[6] );
  zVec3DValueNLFPrint( fp, &v[4] );
  fclose( fp );
}

void test_vert(zVec3DData *data, double x, double y, double z, double a, double b, double c, double wx, double wy, double wz)
{
  zFrame3D frame;
  zVec3D v;
  int i;

  zVec3DDataInitArray( data, N );
  zFrame3DFromZYX( &frame, x, y, z, zDeg2Rad(a), zDeg2Rad(b), zDeg2Rad(c) );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-wx,wx), zRandF(-wy,wy), zRandF(-wz,wz) );
    zXform3DDRC( &frame, &v );
    zVec3DDataAdd( data, &v );
  }
  output_boundingbox( &frame, wx, wy, wz );
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
  printf( "center: " );
  zVec3DPrint( &c );
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
