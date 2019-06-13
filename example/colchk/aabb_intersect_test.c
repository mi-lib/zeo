#include <zeo/zeo_col.h>

void test_box(zAABox3D *b1, zAABox3D *b2)
{
  zRandInit();
  zAABox3DCreate( b1, zRandF(-0.2,0), zRandF(-0.2,0), zRandF(-0.2,0), zRandF(0,0.2), zRandF(0,0.2), zRandF(0,0.2) );
  zAABox3DCreate( b2, zRandF(-0.2,0), zRandF(-0.2,0), zRandF(-0.2,0), zRandF(0,0.2), zRandF(0,0.2), zRandF(0,0.2) );
}

void output(zAABox3D *b1, zAABox3D *b2, zAABox3D *b)
{
  FILE *fp;
  zBox3D box;

  /* for visualization */
  fp = fopen( "boxes", "w" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.6\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.4\n" );
  fprintf( fp, "exp: 0.0\n\n" );

  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.4\n" );
  fprintf( fp, "exp: 0.0\n\n" );

  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.6 0.6 0.0\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.4\n" );
  fprintf( fp, "exp: 0.0\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: c\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: yellow\n" );
  zAABox3DToBox3D( b, &box );
  zBox3DFPrint( fp, &box );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: blue\n" );
  zAABox3DToBox3D( b1, &box );
  zBox3DFPrint( fp, &box );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: red\n" );
  zAABox3DToBox3D( b2, &box );
  zBox3DFPrint( fp, &box );

  fclose( fp );
}

int main(int argc, char *argv[])
{
  zAABox3D b1, b2, b;

  test_box( &b1, &b2 );
  zIntersectAABox3D( &b, &b1, &b2 );
  output( &b1, &b2, &b );
  return 0;
}
