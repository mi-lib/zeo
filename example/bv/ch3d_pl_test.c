#include <zeo/zeo_bv.h>

#define N 10000

void vec_create_rand(zVec3DList *pl)
{
  register int i;
  FILE *fp;
  zVec3D v;

  fp = fopen( "src", "w" );
  zRandInit();
  zListInit( pl );
  for( i=0; i<N; i++ ){
    zVec3DCreatePolar( &v, zRandF(-0.5,0.5), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    v.e[0] += 3;
    zVec3DDataNLFPrint( fp, &v );
    zVec3DListInsert( pl, &v );
  }
  fclose( fp );
}

void output(zPH3D *ph)
{
  register int i;
  FILE *fp;

  eprintf( "%d vertices, %d faces.\n", zPH3DVertNum(ph), zPH3DFaceNum(ph) );
  fp = fopen( "ch", "w" );
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    zVec3DDataNLFPrint( fp, zPH3DFaceVert(ph,i,0) );
    zVec3DDataNLFPrint( fp, zPH3DFaceVert(ph,i,1) );
    fprintf( fp, "\n" );
    zVec3DDataNLFPrint( fp, zPH3DFaceVert(ph,i,2) );
    zVec3DDataNLFPrint( fp, zPH3DFaceVert(ph,i,2) );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );
  fp = fopen( "chv", "w" );
  for( i=0; i<zPH3DVertNum(ph); i++ )
    zVec3DDataNLFPrint( fp, zPH3DVert(ph,i) );
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
  zPH3DPrint( ph );
}

int main(void)
{
  zPH3D ch;
  zVec3DList pl;

  vec_create_rand( &pl );
  zCH3DPL( &ch, &pl );
  output( &ch );
  zPH3DDestroy( &ch );
  zVec3DListDestroy( &pl );
  return 0;
}
