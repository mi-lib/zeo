#include <zeo/zeo_mshape3d.h>

#define N 100

int main(int argc, char *argv[])
{
  zMShape3D ms;
  zVec3D p, *cp;
  register int i;
  FILE *fp1, *fp2;

  zRandInit();
  zMShape3DReadZTK( &ms, "../model/octahedron.ztk" );
  fp1 = fopen( "p", "w" );
  fp2 = fopen( "cp", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
    cp = zMShape3DContigVert( &ms, &p, NULL );
    zVec3DValueNLFPrint( fp1, &p );
    zVec3DValueNLFPrint( fp1, cp );
    fprintf( fp1, "\n\n" );
    zVec3DValueNLFPrint( fp2, cp );
  }
  fclose( fp1 );
  fclose( fp2 );
  zMShape3DDestroy( &ms );
  return 0;
}
