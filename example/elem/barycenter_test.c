#include <zeo/zeo_elem.h>

#define N 1000
zVec3D vert[N];

void test_vert(double x, double y, double z, double wx, double wy, double wz)
{
  register int i;

  for( i=0; i<N; i++ )
    zVec3DCreate( &vert[i], x+zRandF(-wx,wx), y+zRandF(-wy,wy), z+zRandF(-wz,wz) );
}

void output_vert(double x, double y, double z)
{
  FILE *fp;
  register int i;
  zVec3D c;

  zVec3DBarycenter( vert, N, &c );

  fp = fopen( "a", "w" );
  for( i=0; i<N; i++ )
    zVec3DDataNLFWrite( fp, &vert[i] );
  fclose( fp );

  fp = fopen( "b", "w" );
  zVec3DBarycenter( vert, N, &c );
  zVec3DDataNLFWrite( fp, &c );
  fclose( fp );

  fp = fopen( "c", "w" );
  fprintf( fp, "%g %g %g\n", x, y, z );
  fclose( fp );
}

#define X 10
#define Y 20
#define Z 30
int main(void)
{
  zRandInit();
  test_vert( X, Y, Z, 2, 1, 3 );
  output_vert( X, Y, Z );
  return 0;
}
