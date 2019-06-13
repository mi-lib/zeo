#include <zeo/zeo_mshape.h>

#define N 100

int main(int argc, char *argv[])
{
  zMShape3D ms;
  zVec3D p, *cp;
  register int i;
  FILE *fp1, *fp2;

  zRandInit();
  zMShape3DScanFile( &ms, "../model/octahedron.z3d" );
  fp1 = fopen( "p", "w" );
  fp2 = fopen( "cp", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
    cp = zMShape3DContigVert( &ms, &p, NULL );
    zVec3DDataNLFPrint( fp1, &p );
    zVec3DDataNLFPrint( fp1, cp );
    fprintf( fp1, "\n\n" );
    zVec3DDataNLFPrint( fp2, cp );
  }
  fclose( fp1 );
  fclose( fp2 );
  zMShape3DDestroy( &ms );
  return 0;
}
