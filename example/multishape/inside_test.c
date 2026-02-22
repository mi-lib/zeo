#include <zeo/zeo_multishape3d.h>

void test(zMultiShape3D *ms, int n)
{
  zVec3D p;
  int i;
  FILE *fp;

  fp = fopen( "p", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &p, zRandF(-0.4,0.4), zRandF(-0.4,0.4), zRandF(-0.4,0.4) );
    if( zMultiShape3DPointIsInside( ms, &p, true ) )
      zVec3DValueNLFPrint( fp, &p );
  }
  fclose( fp );
}

#define N 100000

int main(int argc, char *argv[])
{
  zMultiShape3D ms;

  zRandInit();
  zMultiShape3DReadZTK( &ms, "../model/scc.ztk" );
  test( &ms, argc > 1 ? atoi(argv[1]) : N );
  zMultiShape3DDestroy( &ms );
  return 0;
}
