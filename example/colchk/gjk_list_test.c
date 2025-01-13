#include <zeo/zeo_col.h>
#include <zeo/zeo_bv3d.h>

#define DEGENERATE

void vec_create_rand(zVec3DData *data, int n, double x, double y, double z, double r)
{
  zVec3D v;
  register int i;

  zVec3DDataInitList( data );
  for( i=0; i<n; i++ ){
#ifdef DEGENERATE
    zVec2DCreatePolar( (zVec2D *)&v, zRandF(0,r), zRandF(-zPI,zPI) );
    v.e[zX] += x;
    v.e[zY] += y;
    v.e[zZ]  = 0;
#else
    zVec3DCreatePolar( &v, zRandF(0,r), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    v.e[zX] += x;
    v.e[zY] += y;
    v.e[zZ] += z;
#endif
    zVec3DDataAdd( data, &v );
  }
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

#define N 1000

int main(int argc, char *argv[])
{
  zVec3DData a, b;
  zVec3D ca, cb;
  double x, y, z;

  zRandInit();
  x = argc > 1 ? atof(argv[1]) : zRandF(0.1,0.4);
  y = argc > 2 ? atof(argv[2]) : zRandF(0.1,0.4);
  z = argc > 3 ? atof(argv[3]) : zRandF(0.1,0.4);
  vec_create_rand( &a, N, 0, 0, 0, 0.2 );
  vec_create_rand( &b, N, x, y, z, 0.2 );
  printf( "in collision? %s\n", zBoolStr( zGJK( &a, &b, &ca, &cb ) ) );
  output( "a", &a, &b, &ca, &cb );
  zVec3DDataDestroy( &a );
  zVec3DDataDestroy( &b );
  return 0;
}
