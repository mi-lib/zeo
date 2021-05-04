#include <zeo/zeo_bv3d.h>

#define N 3000
zVec3D v[N];
void vec_create_rand(FILE *fp)
{
  register int i;

  zRandInit();
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v[i], zRandF(-0.1,0.1), zRandF(-0.1,0.1), 0 );
    zVec3DDataNLFPrint( fp, &v[i] );
  }
}

void output(zPH3D *ph, FILE *fp)
{
  register int i;

  for( i=0; i<zPH3DVertNum(ph); i++ )
    zVec3DDataNLFPrint( fp, zPH3DVert(ph,i) );

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
  zPH3DFPrintZTK( stdout, ph );
}

int main(void)
{
  zPH3D ch;
  FILE *fp1, *fp2;

  fp1 = fopen( "src", "w" );
  fp2 = fopen( "ch", "w" );
  vec_create_rand( fp1 );

  zCH3D( &ch, v, N );

  output( &ch, fp2 );
  fclose( fp1 );
  fclose( fp2 );

  zPH3DDestroy( &ch );
  return 0;
}
