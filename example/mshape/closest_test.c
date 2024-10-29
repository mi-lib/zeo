#include <zeo/zeo_mshape3d.h>

void test1(zMShape3D *ms, int n)
{
  zVec3D p, cp;
  int i;
  FILE *fp1, *fp2;

  fp1 = fopen( "p", "w" );
  fp2 = fopen( "cp", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-0.4,0.4), zRandF(-0.4,0.4), zRandF(-0.4,0.4) );
    zMShape3DClosest( ms, &p, &cp );
    zVec3DDataNLFPrint( fp1, &p );
    zVec3DDataNLFPrint( fp2, &cp );
  }
  fclose( fp1 );
  fclose( fp2 );
}

void test2(zMShape3D *ms, int n)
{
  zVec3D p, cp;
  int i;
  FILE *fp;

  fp = fopen( "p", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-0.4,0.4), zRandF(-0.4,0.4), zRandF(-0.4,0.4) );
    zMShape3DClosest( ms, &p, &cp );
    zVec3DDataNLFPrint( fp, &p );
    zVec3DDataNLFPrint( fp, &cp );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );
}

#define N 1000

int main(int argc, char *argv[])
{
  zMShape3D ms;

  zRandInit();
#if 0
  zMShape3DReadZTK( &ms, "../model/octahedron.ztk" );
  test2( &ms, argc > 1 ? atoi(argv[1]) : N );
#else
  zMShape3DReadZTK( &ms, "../model/scc.ztk" );
  test1( &ms, argc > 1 ? atoi(argv[1]) : N );
#endif
  zMShape3DDestroy( &ms );
  return 0;
}
