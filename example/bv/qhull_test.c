#include <zeo/zeo_bv.h>

#define N 10000
zVec3D v[N];
void vec_create_rand(void)
{
  register int i;
  FILE *fp;

  fp = fopen( "src", "w" );
  zRandInit();
#if 1
  for( i=0; i<N; i++ ){
    zVec3DCreatePolar( &v[i], zRandF(-0.5,0.5), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    v[i].e[0] += 3;
    zVec3DDataNLFWrite( fp, &v[i] );
  }
#elif 0
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v[i], zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
    zVec3DDataNLFWrite( fp, &v[i] );
  }
#elif 0
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v[i], zRandI(-5,5), zRandI(-5,5), zRandI(-5,5) );
    zVec3DDataNLFWrite( fp, &v[i] );
  }
#else
  for( i=0; i<N-1; i++ ){
    zVec3DCreate( &v[i], zRandF(-0.5,0.5), zRandF(-0.5,0.5), 0 );
    zVec3DDataNLFWrite( fp, &v[i] );
  }
  zVec3DCreate( &v[N-1], 0, 0, -0.5 );
  zVec3DDataNLFWrite( fp, &v[N-1] );
#endif
  fclose( fp );
}

void output(zPH3D *ph)
{
  register int i;
  FILE *fp;

  eprintf( "%d vertices, %d faces.\n", zPH3DVertNum(ph), zPH3DFaceNum(ph) );
  fp = fopen( "ch", "w" );
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    zVec3DDataNLFWrite( fp, zPH3DFaceVert(ph,i,0) );
    zVec3DDataNLFWrite( fp, zPH3DFaceVert(ph,i,1) );
    fprintf( fp, "\n" );
    zVec3DDataNLFWrite( fp, zPH3DFaceVert(ph,i,2) );
    zVec3DDataNLFWrite( fp, zPH3DFaceVert(ph,i,2) );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );
  fp = fopen( "chv", "w" );
  for( i=0; i<zPH3DVertNum(ph); i++ )
    zVec3DDataNLFWrite( fp, zPH3DVert(ph,i) );
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
  zPH3DWrite( ph );
}

int main(void)
{
  zPH3D ch;

  vec_create_rand();
  zCH3D( &ch, v, N );
  output( &ch );
  zPH3DDestroy( &ch );
  return 0;
}
