#include <zeo/zeo_mshape.h>

void test(zMShape3D *ms, int n)
{
  zVec3D p;
  register int i;
  FILE *fp;

  fp = fopen( "p", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-0.4,0.4), zRandF(-0.4,0.4), zRandF(-0.4,0.4) );
    if( zMShape3DPointIsInside( ms, &p, true ) )
      zVec3DDataNLFPrint( fp, &p );
  }
  fclose( fp );
}

#define N 100000

int main(int argc, char *argv[])
{
  zMShape3D ms;

  zRandInit();
  zMShape3DScanFile( &ms, "../model/scc.z3d" );
  test( &ms, argc > 1 ? atoi(argv[1]) : N );
  zMShape3DDestroy( &ms );
  return 0;
}
