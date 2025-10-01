#include <zeo/zeo_shape3d.h>
#include <zeo/zeo_col.h>

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

void output(char filename[], zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2)
{
  zPH3D ch;
  FILE *fp;

  fp = fopen( filename, "w" );
  /* for visualization */
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.8 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.8\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.4\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.4\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  /* pair of proximities */
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: p1\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, c1 );
  fprintf( fp, "radius: 0.01\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: p2\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, c2 );
  fprintf( fp, "radius: 0.01\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: rod\n" );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, c1 );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, c2 );
  fprintf( fp, "radius: 0.005\n" );
  /* convex set 1 */
  zVec3DDataConvexHull( data1, &ch );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: ch1\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrintZTK( fp, &ch );
  zPH3DDestroy( &ch );
  /* convex set 2 */
  zVec3DDataConvexHull( data2, &ch );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: ch2\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrintZTK( fp, &ch );
  zPH3DDestroy( &ch );

  fclose( fp );
}

#define N 4

int main(int argc, char *argv[])
{
  zVec3D a[N], b[N];
  zVec3DData data1, data2;
  zVec3D ca, cb;

  vec_create( a, b, argc > 1 ? atof(argv[1]) : 0 );
  zVec3DDataAssignArrayDirect( &data1, a, N );
  zVec3DDataAssignArrayDirect( &data2, b, N );
  printf( "in collision? %s\n", zBoolStr( zGJK( &data1, &data2, &ca, &cb ) ) );
  zVec3DPrint( &ca );
  zVec3DPrint( &cb );
  output( "a", &data1, &data2, &ca, &cb );
  return 0;
}
