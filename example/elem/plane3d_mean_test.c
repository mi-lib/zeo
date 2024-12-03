#include <zeo/zeo_elem3d.h>

#define N 1000

void create_point3D_rand(FILE *fp, zVec3D v[], int n)
{
  int i;

  for( i=0; i<n; i++ ){
    zVec3DCreate( &v[i], zRandF(-100,100), zRandF(-50,50), zRandF(-10,10) );
    zVec3DValueNLFPrint( fp, &v[i] );
  }
}

void verify(FILE *fp, zVec3D v[], int n, zPlane3D *pl)
{
  register int i;
  zVec3D p;

  for( i=0; i<n; i++ ){
    zPlane3DProjPoint( pl, &v[i], &p );
    zVec3DValueNLFPrint( fp, &p );
  }
}

int main(void)
{
  zVec3D v[N], pc;
  zPlane3D pl;
  FILE *fp[2];

  zRandInit();

  fp[0] = fopen( "a", "w" );
  fp[1] = fopen( "b", "w" );

  create_point3D_rand( fp[0], v, N );
  zPlane3DMean( &pl, &pc, v, N );
  verify( fp[1], v, N, &pl );

  fclose( fp[0] );
  fclose( fp[1] );
  return 0;
}
