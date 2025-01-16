#include <zeo/zeo_vec3d_data.h>

#define N 1000

void create_point3D_rand(FILE *fp, zVec3DData *data, int n)
{
  int i;
  zVec3D v;

  zVec3DDataInitArray( data, n );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v, zRandF(-100,100), zRandF(-50,50), zRandF(-10,10) );
    zVec3DValueNLFPrint( fp, &v );
    zVec3DDataAdd( data, &v );
  }
}

void verify(FILE *fp, zVec3DData *data, zPlane3D *pl)
{
  zVec3D p, *v;

  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zPlane3DProjPoint( pl, v, &p );
    zVec3DValueNLFPrint( fp, &p );
  }
}

int main(void)
{
  zVec3DData data;
  zVec3D pc;
  zPlane3D pl;
  FILE *fp[2];

  zRandInit();
  fp[0] = fopen( "a", "w" );
  fp[1] = fopen( "b", "w" );
  create_point3D_rand( fp[0], &data, N );
  zVec3DDataMeanPlane( &data, &pc, &pl );
  verify( fp[1], &data, &pl );
  zVec3DDataDestroy( &data );
  fclose( fp[0] );
  fclose( fp[1] );
  return 0;
}
