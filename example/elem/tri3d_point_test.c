#include <zeo/zeo_elem3d.h>

zVec3D *vec_rand_create(zVec3D *p, zVec3D *u, zVec3D *v, zVec3D *r)
{
  zVec3DCat( p, zRandF(-1,1), u, r );
  zVec3DCatDRC( r, zRandF(-1,1), v );
  return r;
}

#define N 1000

int main(int argc, char *argv[])
{
  zVec3D p, u, v, v1, v2, v3, vn;
  zTri3D tri;
  register int i;
  FILE *fp1, *fp2;

  zRandInit();
  zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &u, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  vec_rand_create( &p, &u, &v, &v1 );
  vec_rand_create( &p, &u, &v, &v2 );
  vec_rand_create( &p, &u, &v, &v3 );
  zTri3DCreate( &tri, &v1, &v2, &v3 );
  fp1 = fopen( "t", "w" );
  zVec3DDataNLFPrint( fp1, &v1 );
  zVec3DDataNLFPrint( fp1, &v2 );
  zVec3DDataNLFPrint( fp1, &v3 );
  zVec3DDataNLFPrint( fp1, &v1 ); fprintf( fp1, "\n" );
  fclose( fp1 );
  fp1 = fopen( "i", "w" );
  fp2 = fopen( "o", "w" );
  for( i=0; i<N; i++ ){
    vec_rand_create( &p, &u, &v, &vn );
    if( zTri3DPointIsInside( &tri, &vn, true ) ){
      zVec3DDataFPrint( fp1, &vn ); fprintf( fp1, "\n" );
    } else{
      zVec3DDataFPrint( fp2, &vn ); fprintf( fp2, "\n" );
    }
  }
  fclose( fp1 );
  fclose( fp2 );
  return 0;
}
