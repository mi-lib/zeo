#include <zeo/zeo_col.h>

void output_src(zVec3DData *data1, zVec3DData *data2)
{
  zPH3D ch;
  FILE *fp;

  fp = fopen( "src", "w" );
  /* for visualization */
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.8 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.8\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  /* convex set 1 */
  zVec3DDataConvexHull( data1, &ch );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: ch1\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrintZTK( fp, &ch );
  zPH3DDestroy( &ch );
  /* convex set 2 */
  zVec3DDataConvexHull( data2, &ch );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: ch2\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrintZTK( fp, &ch );
  zPH3DDestroy( &ch );

  fclose( fp );
}

void output_ms(zPH3D *ph)
{
  FILE *fp;

  fp = fopen( "ms", "w" );
  /* for visualization */
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 1.0 1.0 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.4\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.4\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  /* convex set */
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: minkowski_sub\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: white\n" );
  zPH3DFPrintZTK( fp, ph );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: origin\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: 0 0 0\n" );
  fprintf( fp, "radius: 0.01\n" );

  fclose( fp );
}

void vec_create(zVec3D p1[], zVec3D p2[], double z)
{
  printf( "%g\n", z );

  zVec3DCreate( &p1[0], 0.1, 0.0, 0.0 );
  zVec3DCreate( &p1[1],-0.1, 0.1, 0.0 );
  zVec3DCreate( &p1[2],-0.1,-0.1, 0.0 );
  zVec3DCreate( &p1[3], 0.0, 0.0,-0.1 );

  zVec3DCreate( &p2[0], 0.2, 0.0, 0.1 );
  zVec3DCreate( &p2[1],-0.2, 0.2, 0.1 );
  zVec3DCreate( &p2[2],-0.2,-0.2, 0.1 );
  zVec3DCreate( &p2[3], 0.0, 0.0,   z );
}

int main(int argc, char *argv[])
{
  zPH3D ph;
  zVec3D p1[4], p2[4];
  zVec3DData data1, data2;

  vec_create( p1, p2, argc > 1 ? atof(argv[1]) : 0 );
  zVec3DDataAssignArrayDirect( &data1, p1, 4 );
  zVec3DDataAssignArrayDirect( &data2, p2, 4 );
  zVec3DDataMinkowskiSub( &data1, &data2, &ph );
  output_src( &data1, &data2 );
  output_ms( &ph );
  zPH3DDestroy( &ph );
  return 0;
}
