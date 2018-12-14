#include <zeo/zeo_mshape.h>

void test1(zMShape3D *ms, int n)
{
  zVec3D p, cp;
  register int i;
  FILE *fp1, *fp2;

  fp1 = fopen( "p", "w" );
  fp2 = fopen( "cp", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-0.4,0.4), zRandF(-0.4,0.4), zRandF(-0.4,0.4) );
    zMShape3DClosest( ms, &p, &cp );
    zVec3DDataNLFWrite( fp1, &p );
    zVec3DDataNLFWrite( fp2, &cp );
  }
  fclose( fp1 );
  fclose( fp2 );
}

void test2(zMShape3D *ms, int n)
{
  zVec3D p, cp;
  register int i;
  FILE *fp;

  fp = fopen( "p", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-0.4,0.4), zRandF(-0.4,0.4), zRandF(-0.4,0.4) );
    zMShape3DClosest( ms, &p, &cp );
    zVec3DDataNLFWrite( fp, &p );
    zVec3DDataNLFWrite( fp, &cp );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );
}

#define N 1000

int main(int argc, char *argv[])
{
  zMShape3D ms;

  zRandInit();
/*
  zMShape3DReadFile( &ms, "../model/octahedron.z3d" );
  test2( &ms, argc > 1 ? atoi(argv[1]) : N );
*/
  zMShape3DReadFile( &ms, "../model/scc.z3d" );
  test1( &ms, argc > 1 ? atoi(argv[1]) : N );
  zMShape3DDestroy( &ms );
  return 0;
}
