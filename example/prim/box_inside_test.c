#include <zeo/zeo.h>

#define N 10000

int main(void)
{
  zBox3D box;
  zVec3D center, ax, ay, az, tmp;
  zVec3D p;
  int i;
  FILE *fp1, *fp2;

  zRandInit();
  zVec3DCreate( &center, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &ax, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DNormalizeDRC( &ax );
  zVec3DCreate( &tmp, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DOrthogonalize( &tmp, &ax, &ay );
  zVec3DNormalizeDRC( &ay );
  zVec3DOuterProd( &ax, &ay, &az );
  zBox3DCreate( &box, &center, &ax, &ay, &az, zRandF(0.5,1), zRandF(0.5,1), zRandF(0.5,1) );
  fp1 = fopen( "b", "w" );
  for( i=0; i<8; i++ ){
    zBox3DVert( &box, i, &p );
    zVec3DDataNLFWrite( fp1, &p );
  }
  fclose( fp1 );

  fp1 = fopen( "p", "w" );
  fp2 = fopen( "i", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(-1.5,1.5), zRandF(-1.5,1.5), zRandF(-1.5,1.5) );
    zVec3DDataNLFWrite( fp1, &p );
    if( zBox3DPointIsInside( &box, &p, true ) )
      zVec3DDataNLFWrite( fp2, &p );
  }
  fclose( fp1 );
  fclose( fp2 );
  return 0;
}
