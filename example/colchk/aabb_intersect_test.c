#include <zeo/zeo_col.h>

void test_box(zAABox3D *b1, zAABox3D *b2)
{
  zRandInit();
  zAABox3DCreate( b1, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zAABox3DCreate( b2, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
}

void output(zAABox3D *b1, zAABox3D *b2, zAABox3D *b)
{
  FILE *fp;
  zBox3D box;

  /* for visualization */
  fp = fopen( "box.ztk", "w" );
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

  if( b ){
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
    zBox3DFPrintZTK( fp, &box );
  }

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: blue\n" );
  zAABox3DToBox3D( b1, &box );
  zBox3DFPrintZTK( fp, &box );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: red\n" );
  zAABox3DToBox3D( b2, &box );
  zBox3DFPrintZTK( fp, &box );

  fclose( fp );
}

int main(int argc, char *argv[])
{
  zAABox3D b1, b2, b;
  bool is_collide;

  test_box( &b1, &b2 );
  printf( "in collision? %s\n", zBoolStr( ( is_collide = zColChkAABox3D( &b1, &b2 ) ) ) );
  if( is_collide ){
    zIntersectAABox3D( &b, &b1, &b2 );
    output( &b1, &b2, &b );
  } else
    output( &b1, &b2, NULL );
  return 0;
}
