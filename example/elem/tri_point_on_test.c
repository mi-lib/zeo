#include <zeo/zeo.h>

#define N  10000

int main(void)
{
  zVec3D v1, v2, v3, p;
  zTri3D t;
  int i;
  FILE *fp[2];

  zRandInit();
  zVec3DCreate( &v1, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
  zVec3DCreate( &v2, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
  zVec3DCreate( &v3, zRandF(0,1), zRandF(0,1), zRandF(0,1) );
  zTri3DCreate( &t, &v1, &v2, &v3 );
  fp[0] = fopen( "t", "w" );
  zVec3DDataNLFWrite( fp[0], &v1 );
  zVec3DDataNLFWrite( fp[0], &v2 );
  zVec3DDataNLFWrite( fp[0], &v3 );
  zVec3DDataNLFWrite( fp[0], &v1 );
  fclose( fp[0] );

  fp[0] = fopen( "s", "w" );
  fp[1] = fopen( "o", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p,  zRandF(0,1), zRandF(0,1), zRandF(0,1) );
    if( zRandI(0,9) == 0 )
      zTri3DProj( &t, &p, &p );
    zVec3DDataNLFWrite( fp[0], &p );
    if( zTri3DPointIsOn( &t, &p ) )
      zVec3DDataNLFWrite( fp[1], &p );
  }
  fclose( fp[0] );
  fclose( fp[1] );
  return 0;
}
