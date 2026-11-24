/* operations of a 3D cell complex on non-convex polyhedra.
 * A cross of three bars is made as a non-convex polyhedron, and is subtracted from, intersected
 * with and merged to a sphere, and the surface of the sphere is cut by it. The results are output
 * to ZTK files cross.ztk, nc_subtract.ztk, nc_intersect.ztk, nc_merge.ztk and nc_cut.ztk, which
 * can be viewed by rk_view (in RoKi-GL), e.g.,
 *   % rk_view nc_subtract.ztk
 */
#include <zeo/zeo.h>

/* box rotated about the z-axis. */
void create_box(zPH3D *ph, double dx, double dy, double dz, double angle)
{
  zVec3DData data;
  zVec3D v;
  int i;

  zVec3DDataInitArray( &data, 8 );
  for( i=0; i<8; i++ ){
    zVec3DCreate( &v, i & 1 ? 0.5*dx : -0.5*dx, i & 2 ? 0.5*dy : -0.5*dy, i & 4 ? 0.5*dz : -0.5*dz );
    zVec3DCreate( &v, v.c.x*cos(angle) - v.c.y*sin(angle), v.c.x*sin(angle) + v.c.y*cos(angle), v.c.z );
    zVec3DDataAdd( &data, &v );
  }
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
}

/* convex hull of random points on a sphere. */
void create_sphere(zPH3D *ph, int n, double r)
{
  zVec3DData data;
  zVec3D v;
  int i;

  zVec3DDataInitArray( &data, n );
  for( i=0; i<n; i++ ){
    zVec3DCreatePolar( &v, r, acos( zRandF(-1,1) ), zRandF(-zPI,zPI) );
    zVec3DDataAdd( &data, &v );
  }
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
}

/* a cross of three bars, which is a non-convex polyhedron. */
void create_cross(zPH3D *ph)
{
  zCC3D cc;
  zPH3D bar;

  create_box( &bar, 2.6, 0.6, 0.6, zDeg2Rad(20) );
  zCC3DFromPH3D( &cc, &bar );
  zPH3DDestroy( &bar );
  create_box( &bar, 0.6, 2.6, 0.6, zDeg2Rad(20) );
  zCC3DMerge( &cc, &bar );
  zPH3DDestroy( &bar );
  create_box( &bar, 0.6, 0.6, 2.6, zDeg2Rad(20) );
  zCC3DMerge( &cc, &bar );
  zPH3DDestroy( &bar );
  zCC3DToPH3D( &cc, ph );
  zCC3DDestroy( &cc );
}

/* output a polyhedron, and print its volume and Euler characteristic. */
void output_ph(zPH3D *ph, const char *filename)
{
  FILE *fp;

  if( zPH3DIsClosed( ph ) )
    printf( "%s: vertices=%d faces=%d volume=%g Euler characteristic=%d\n", filename,
      zPH3DVertNum(ph), zPH3DFaceNum(ph), zPH3DVolume(ph), zPH3DVertNum(ph) - zPH3DFaceNum(ph) / 2 );
  else
    printf( "%s: vertices=%d faces=%d (open)\n", filename, zPH3DVertNum(ph), zPH3DFaceNum(ph) );
  fp = fopen( filename, "w" );
  fprintf( fp, "[zeo::optic]\nname: yellow\nambient: 0.6 0.6 0.2\ndiffuse: 1.0 1.0 0.2\nspecular: 0 0 0\nalpha: 1.0\nesr: 0\n\n" );
  fprintf( fp, "[zeo::shape]\nname: result\ntype: polyhedron\noptic: yellow\n" );
  zPH3DFPrintZTK( fp, ph );
  fclose( fp );
}

/* output the solid of a cell complex. */
void output_result(zCC3D *cc, const char *filename)
{
  zPH3D ph;

  if( !zCC3DToPH3D( cc, &ph ) ){
    printf( "%s: empty\n", filename );
    return;
  }
  output_ph( &ph, filename );
  zPH3DDestroy( &ph );
}

int main(int argc, char *argv[])
{
  zPH3D sphere, cross, cut;
  zCC3D cc;

  zRandInit();
  create_sphere( &sphere, argc > 1 ? atoi(argv[1]) : 300, 1.0 );
  create_cross( &cross );
  output_ph( &cross, "cross.ztk" );

  /* subtraction of the cross from the sphere */
  zCC3DFromPH3D( &cc, &sphere );
  zCC3DSubtract( &cc, &cross );
  output_result( &cc, "nc_subtract.ztk" );
  zCC3DDestroy( &cc );

  /* intersection of the sphere and the cross */
  zCC3DFromPH3D( &cc, &sphere );
  zCC3DIntersect( &cc, &cross );
  output_result( &cc, "nc_intersect.ztk" );
  zCC3DDestroy( &cc );

  /* cut of the surface of the sphere by the cross, which is an open surface */
  zCC3DFromPH3D( &cc, &sphere );
  if( zCC3DCut( &cc, &cross, &cut ) ){
    output_ph( &cut, "nc_cut.ztk" );
    zPH3DDestroy( &cut );
  }
  zCC3DDestroy( &cc );

  /* merge of the cross to the sphere, which starts from the non-convex polyhedron */
  zCC3DFromPH3D( &cc, &cross );
  zCC3DMerge( &cc, &sphere );
  output_result( &cc, "nc_merge.ztk" );
  printf( "consistency: %s\n", zCC3DCheck( &cc ) ? "OK" : "broken" );
  zCC3DDestroy( &cc );

  zPH3DDestroy( &sphere );
  zPH3DDestroy( &cross );
  return 0;
}
