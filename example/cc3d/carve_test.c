/* successive operations of a 3D cell complex, which make a non-convex solid with holes.
 * The results are output to ZTK files carve1.ztk ... carve4.ztk, which can be viewed by rk_view
 * (in RoKi-GL), e.g.,
 *   % rk_view carve4.ztk
 */
#include <zeo/zeo.h>

/* axis-aligned box. */
void create_box(zPH3D *ph, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
{
  zVec3DData data;
  zVec3D v;
  int i;

  zVec3DDataInitArray( &data, 8 );
  for( i=0; i<8; i++ ){
    zVec3DCreate( &v, i & 1 ? xmax : xmin, i & 2 ? ymax : ymin, i & 4 ? zmax : zmin );
    zVec3DDataAdd( &data, &v );
  }
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
}

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
  fprintf( fp, "[zeo::optic]\nname: yellow\nambient: 0.6 0.6 0.2\ndiffuse: 1.0 1.0 0.2\nspecular: 0 0 0\nalpha: 1.0\nesr: 0\n\n" );
  fprintf( fp, "[zeo::shape]\nname: result\ntype: polyhedron\noptic: yellow\n" );
  zPH3DFPrintZTK( fp, &ph );
  fclose( fp );
  zPH3DDestroy( &ph );
}

int main(int argc, char *argv[])
{
  zCC3D cc;
  zAABox3D universe;
  zPH3D ph;
  zPlane3D plane;
  zVec3D p, n;

  zRandInit();
  zAABox3DCreate( &universe, -3, -3, -3, 3, 3, 3 );
  zCC3DCreate( &cc, &universe );

  /* a cube */
  create_box( &ph, -1, -1, -1, 1, 1, 1 );
  zCC3DMerge( &cc, &ph );
  zPH3DDestroy( &ph );
  output_result( &cc, "carve1.ztk" );

  /* three holes along the axes */
  create_box( &ph, -2, -0.4, -0.4, 2, 0.4, 0.4 );
  zCC3DSubtract( &cc, &ph );
  zPH3DDestroy( &ph );
  create_box( &ph, -0.4, -2, -0.4, 0.4, 2, 0.4 );
  zCC3DSubtract( &cc, &ph );
  zPH3DDestroy( &ph );
  create_box( &ph, -0.4, -0.4, -2, 0.4, 0.4, 2 );
  zCC3DSubtract( &cc, &ph );
  zPH3DDestroy( &ph );
  output_result( &cc, "carve2.ztk" );

  /* a sphere on a corner */
  create_sphere( &ph, 200, 0.7, 1, 1, 1 );
  zCC3DMerge( &cc, &ph );
  zPH3DDestroy( &ph );
  output_result( &cc, "carve3.ztk" );

  /* cut by a slanted plane */
  zVec3DCreate( &p, 0, 0, 0.5 );
  zVec3DCreate( &n, 0.3, 0.2, 1 );
  zPlane3DCreate( &plane, &p, &n );
  zCC3DCutByPlane3D( &cc, &plane );
  output_result( &cc, "carve4.ztk" );

  printf( "consistency: %s\n", zCC3DCheck( &cc ) ? "OK" : "broken" );
  zCC3DDestroy( &cc );
  return 0;
}
