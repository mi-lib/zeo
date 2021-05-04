#include <zeo/zeo_elem3d.h>

void vec_create_rand(FILE *fp, zVec3D *v, int n, double x, double y, double z, double r)
{
  register int i;

  zRandInit();
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v[i], x+zRandF(-100,100), y+zRandF(-100,100), z+zRandF(-10,10) );
    zVec3DDataNLFPrint( fp, &v[i] );
  }
}

void verify(FILE *fp, zVec3D v[], int n, zPlane3D *pl)
{
  register int i;
  zVec3D p;

  for( i=0; i<n; i++ ){
    zPlane3DProj( pl, &v[i], &p );
    zVec3DDataNLFPrint( fp, &p );
  }
}

#define N 1000
int main(void)
{
  zVec3D v[N], pc;
  zPlane3D pl;
  FILE *fp[2];

  fp[0] = fopen( "a", "w" );
  fp[1] = fopen( "b", "w" );

  vec_create_rand( fp[0], v, N, -2, 1,-1, 3 );
  zPlane3DMean( &pl, &pc, v, N );
  verify( fp[1], v, N, &pl );

  fclose( fp[0] );
  fclose( fp[1] );
  return 0;
}
