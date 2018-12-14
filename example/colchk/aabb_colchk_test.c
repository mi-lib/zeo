#include <zeo/zeo_col.h>

void box_output(zAABox3D *b1, zAABox3D *b2)
{
  FILE *fp;
  zBox3D box1, box2;

  zAABox3DToBox3D( b1, &box1 );
  zAABox3DToBox3D( b2, &box2 );
  /* for visualization */
  fp = fopen( "box", "w" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.8 0.8 1.0\n" );
  fprintf( fp, "diffuse: 0.8 0.8 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 1.0 0.6 0.6\n" );
  fprintf( fp, "diffuse: 1.0 0.6 0.6\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: blue\n" );
  zBox3DFWrite( fp, &box1 );
  fprintf( fp, "\n[shape]\n" );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: red\n" );
  zBox3DFWrite( fp, &box2 );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zAABox3D b1, b2;

  zRandInit();
  zAABox3DCreate( &b1, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zAABox3DCreate( &b2, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );

  printf( "in collision? %s\n", zBoolExpr( zColChkAABox3D( &b1, &b2 ) ) );

  box_output( &b1, &b2 );
  return 0;
}
