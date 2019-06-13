#include <zeo/zeo_bv.h>
#include <zeo/zeo_mshape.h>

void src(zPH3D *ph)
{
  register int i;
  FILE *fp;

  fp = fopen( "src", "w" );
  for( i=0; i<zPH3DVertNum(ph); i++ )
    zVec3DDataNLFPrint( fp, zPH3DVert(ph,i) );
  fclose( fp );
}

void output(zPH3D *ph)
{
  register int i;
  FILE *fp;

  fp = fopen( "ch", "w" );
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    zVec3DDataNLFPrint( fp, zTri3DVert(zPH3DFace(ph,i),0) );
    zVec3DDataNLFPrint( fp, zTri3DVert(zPH3DFace(ph,i),1) );
    fprintf( fp, "\n" );
    zVec3DDataNLFPrint( fp, zTri3DVert(zPH3DFace(ph,i),2) );
    zVec3DDataNLFPrint( fp, zTri3DVert(zPH3DFace(ph,i),2) );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zMShape3D shape;
  zPH3D *ph, ch;

  if( argc < 2 ) return 1;
  zMShape3DScanFile( &shape, argv[1] );

  src( ( ph = zShape3DPH(zMShape3DShape(&shape,0)) ) );
  zCH3D( &ch, zPH3DVertBuf(ph), zPH3DVertNum(ph) );
  output( &ch );
  printf( "src: vert=%d, face=%d\n", zPH3DVertNum(ph), zPH3DFaceNum(ph) );
  printf( "dst: vert=%d, face=%d\n", zPH3DVertNum(&ch), zPH3DFaceNum(&ch) );

  zPH3DDestroy( &ch );
  zMShape3DDestroy( &shape );
  return 0;
}
