#include <zeo/zeo_col.h>

#define S 0.3
#define W 0.4

void box_create(zBox3D *box)
{
  zBox3DInit( box );
  zVec3DCreate( zBox3DCenter(box), zRandF(-S,S), zRandF(-S,S), zRandF(-S,S) );
  zMat3DFromZYX( zFrame3DAtt(&box->f), zRandF(-zPI,zPI),zRandF(-zPI_2,zPI_2),zRandF(-zPI,zPI) );
  zBox3DSetWidth( box, zRandF(0.5*W,W) );
  zBox3DSetHeight( box, zRandF(0.5*W,W) );
  zBox3DSetDepth( box, zRandF(0.5*W,W) );
}

void box_output(zBox3D *b1, zBox3D *b2)
{
  FILE *fp;

  /* for visualization */
  fp = fopen( "box.ztk", "w" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.8 0.8 1.0\n" );
  fprintf( fp, "diffuse: 0.8 0.8 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 1.0\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 1.0 0.6 0.6\n" );
  fprintf( fp, "diffuse: 1.0 0.6 0.6\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 1.0\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: blue\n" );
  zBox3DFPrintZTK( fp, b1 );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: red\n" );
  zBox3DFPrintZTK( fp, b2 );
  fclose( fp );
}

int main(void)
{
  zBox3D b1, b2;

  zRandInit();
  box_create( &b1 );
  box_create( &b2 );
  printf( "in collision? %s\n", zBoolStr( zColChkBox3D( &b1, &b2 ) ) );
  box_output( &b1, &b2 );
  return 0;
}
