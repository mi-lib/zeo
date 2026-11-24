/* operations of a 3D cell complex on two convex polyhedra.
 * The results are output to ZTK files, which can be viewed by rk_view (in RoKi-GL), e.g.,
 *   % rk_view org.ztk
 *   % rk_view subtract.ztk
 */
#include <zeo/zeo.h>

/* convex hull of random points on a sphere. */
void create_sphere(zPH3D *ph, int n, double r, double x, double y, double z)
{
  zVec3DData data;
  zVec3D v;
  int i;

  zVec3DDataInitArray( &data, n );
  for( i=0; i<n; i++ ){
    zVec3DCreatePolar( &v, r, acos( zRandF(-1,1) ), zRandF(-zPI,zPI) );
    v.c.x += x; v.c.y += y; v.c.z += z;
    zVec3DDataAdd( &data, &v );
  }
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
}

/* box rotated about the z-axis. */
void create_box(zPH3D *ph, double dx, double dy, double dz, double x, double y, double z, double angle)
{
  zVec3DData data;
  zVec3D v;
  int i;

  zVec3DDataInitArray( &data, 8 );
  for( i=0; i<8; i++ ){
    zVec3DCreate( &v, i & 1 ? 0.5*dx : -0.5*dx, i & 2 ? 0.5*dy : -0.5*dy, i & 4 ? 0.5*dz : -0.5*dz );
    zVec3DCreate( &v, v.c.x*cos(angle) - v.c.y*sin(angle) + x, v.c.x*sin(angle) + v.c.y*cos(angle) + y, v.c.z + z );
    zVec3DDataAdd( &data, &v );
  }
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
}

void output_optic(FILE *fp, const char *name, double r, double g, double b, double alpha)
{
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: %s\n", name );
  fprintf( fp, "ambient: %g %g %g\n", 0.6*r, 0.6*g, 0.6*b );
  fprintf( fp, "diffuse: %g %g %g\n", r, g, b );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: %g\n", alpha );
  fprintf( fp, "esr: 0.0\n\n" );
}

void output_shape(FILE *fp, const char *name, const char *optic, zPH3D *ph)
{
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: %s\n", name );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: %s\n", optic );
  zPH3DFPrintZTK( fp, ph );
}

void output_org(zPH3D *a, zPH3D *b)
{
  FILE *fp;

  fp = fopen( "org.ztk", "w" );
  output_optic( fp, "blue", 0.2, 0.2, 1.0, 0.6 );
  output_optic( fp, "red", 1.0, 0.2, 0.2, 0.6 );
  output_shape( fp, "a", "blue", a );
  output_shape( fp, "b", "red", b );
  fclose( fp );
}

/* output the solid of a cell complex, and print its volume and Euler characteristic. */
void output_result(zCC3D *cc, const char *filename)
{
  FILE *fp;
  zPH3D ph;

  if( !zCC3DToPH3D( cc, &ph ) ){
    printf( "%s: empty\n", filename );
    return;
  }
  printf( "%s: vertices=%d faces=%d volume=%g Euler characteristic=%d (cells=%d)\n", filename,
    zPH3DVertNum(&ph), zPH3DFaceNum(&ph), zPH3DVolume(&ph),
    zPH3DVertNum(&ph) - zPH3DFaceNum(&ph) / 2, zCC3DCellNum(cc) );
  fp = fopen( filename, "w" );
  output_optic( fp, "yellow", 1.0, 1.0, 0.2, 1.0 );
  output_shape( fp, "result", "yellow", &ph );
  fclose( fp );
  zPH3DDestroy( &ph );
}

int main(int argc, char *argv[])
{
  zPH3D a, b;
  zCC3D cc;
  zPlane3D plane;
  zVec3D p, n;

  zRandInit();
  create_sphere( &a, argc > 1 ? atoi(argv[1]) : 300, 1.0, 0, 0, 0 );
  create_box( &b, 0.8, 0.8, 3.0, 0.5, 0.3, 0.2, zDeg2Rad(30) );
  output_org( &a, &b );

  /* cut by a plane */
  zCC3DFromPH3D( &cc, &a );
  zVec3DCreate( &p, 0.3, 0, 0 );
  zVec3DCreate( &n, 1, 0.5, 0.5 );
  zPlane3DCreate( &plane, &p, &n );
  zCC3DCutByPlane3D( &cc, &plane );
  output_result( &cc, "cut.ztk" );
  zCC3DDestroy( &cc );

  /* intersection */
  zCC3DFromPH3D( &cc, &a );
  zCC3DIntersect( &cc, &b );
  output_result( &cc, "intersect.ztk" );
  zCC3DDestroy( &cc );

  /* subtraction (the box penetrates the sphere near its side, so that a thin piece is separated) */
  zCC3DFromPH3D( &cc, &a );
  zCC3DSubtract( &cc, &b );
  output_result( &cc, "subtract.ztk" );
  zCC3DDestroy( &cc );

  /* merge */
  zCC3DFromPH3D( &cc, &a );
  zCC3DMerge( &cc, &b );
  output_result( &cc, "merge.ztk" );
  zCC3DDestroy( &cc );

  zPH3DDestroy( &a );
  zPH3DDestroy( &b );
  return 0;
}
