#include <zeo/zeo.h>

#define N 1000

int main(void)
{
  register int i;
  zVec3D v[N];
  zPH3D ch;
  double r;
  FILE *fp;

  zRandInit();
  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
    r = zRandF(-0.1,0.1);
    zVec3DCreate( &v[i], r*0.75, zRandF(-0.1,0.1), r*0.25 );
    zVec3DDataNLFWrite( fp, &v[i] );
  }
  fclose( fp );
  zCH2D2PH3D( &ch, v, N );
  fp = fopen( "ch", "w" );
  for( i=0; i<zPH3DVertNum(&ch); i++ )
    zVec3DDataNLFWrite( fp, zPH3DVert(&ch,i) );
  zVec3DDataNLFWrite( fp, zPH3DVert(&ch,0) );
  fclose( fp );

  /* for visualization */
  printf( "[optic]\n" );
  printf( "name: white\n" );
  printf( "ambient: 0.8 0.8 0.8\n" );
  printf( "diffuse: 0.8 0.8 0.8\n" );
  printf( "specular: 0.0 0.0 0.0\n" );
  printf( "alpha: 0.8\n" );
  printf( "exp: 1.0\n\n" );
  printf( "[shape]\n" );
  printf( "name: ch\n" );
  printf( "type: polyhedron\n" );
  printf( "optic: white\n" );
  zPH3DWrite( &ch );
  zPH3DDestroy( &ch );
  return 0;
}
