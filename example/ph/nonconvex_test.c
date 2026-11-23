#include <zeo/zeo_shape3d.h>

/* a sample of zPH3DPointIsInside() and zPH3DClosest() for a non-convex polyhedron, namely, a prism
 * of a star-shaped bottom. Two files are output:
 *  nonconvex_inside.ztk:  random points inside of the polyhedron (red)
 *  nonconvex_closest.ztk: random points (blue), the closest points on the polyhedron (red), and tethers
 *                         between them (gray). The points inside of the polyhedron are the closest points
 *                         of themselves.
 * usage: nonconvex_test [margin]
 */

void output_optic(FILE *fp, const char *name, double r, double g, double b, double alpha)
{
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: %s\n", name );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: %g %g %g\n", r, g, b );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: %g\n", alpha );
  fprintf( fp, "esr: 1.0\n\n" );
}

void output_header(FILE *fp)
{
  output_optic( fp, "red",  1.0, 0.0, 0.0, 1.0 );
  output_optic( fp, "blue", 0.0, 0.0, 1.0, 1.0 );
  output_optic( fp, "gray", 0.7, 0.7, 0.7, 1.0 );
  output_optic( fp, "cyan", 0.4, 1.0, 1.0, 0.3 );
}

void output_ph(FILE *fp, zPH3D *ph)
{
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: polyhedron\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: cyan\n" );
  zPH3DFPrintZTK( fp, ph );
}

void output_point(FILE *fp, const char *name, int i, const char *optic, zVec3D *p, double radius)
{
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: %s%d\n", name, i );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: %s\n", optic );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, p );
  fprintf( fp, "radius: %g\n\n", radius );
}

void output_tether(FILE *fp, int i, zVec3D *p, zVec3D *cp)
{
  if( zIsTiny( zVec3DDist( p, cp ) ) ) return;
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: tether%d\n", i );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: gray\n" );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, p );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, cp );
  fprintf( fp, "radius: 0.01\n\n" );
}

/* a prism of a star-shaped bottom, which is non-convex. */
void create_star_prism(zPH3D *ph)
{
  zVec3D bottom[10], shift;
  int i;

  for( i=0; i<10; i++ )
    zVec3DCreatePolar( &bottom[i], i % 2 ? 0.8 : 2.0, zPI_2, zPIx2 * i / 10 );
  for( i=0; i<10; i++ ) bottom[i].c.z = 0; /* exactly on a plane */
  zVec3DCreate( &shift, 0, 0, 1 );
  zPH3DCreatePrism( ph, bottom, 10, &shift );
}

#define N_INSIDE  3000
#define N_CLOSEST 30
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zPH3D ph;
  zVec3D p, cp;
  double margin;
  int i;
  FILE *fp;

  zRandInit();
  create_star_prism( &ph );
  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  /* inside test */
  fp = fopen( "nonconvex_inside.ztk", "w" );
  output_header( fp );
  for( i=0; i<N_INSIDE; i++ ){
    zVec3DCreate( &p, zRandF(-2.5,2.5), zRandF(-2.5,2.5), zRandF(-0.5,1.5) );
    if( zPH3DPointIsInside( &ph, &p, margin ) )
      output_point( fp, "point", i, "red", &p, 0.03 );
  }
  output_ph( fp, &ph );
  fclose( fp );
  /* closest points */
  fp = fopen( "nonconvex_closest.ztk", "w" );
  output_header( fp );
  for( i=0; i<N_CLOSEST; i++ ){
    zVec3DCreate( &p, zRandF(-2.5,2.5), zRandF(-2.5,2.5), zRandF(-0.5,1.5) );
    zPH3DClosest( &ph, &p, &cp );
    output_point( fp, "point", i, "blue", &p, 0.04 );
    output_point( fp, "closest", i, "red", &cp, 0.03 );
    output_tether( fp, i, &p, &cp );
  }
  output_ph( fp, &ph );
  fclose( fp );
  zPH3DDestroy( &ph );
  return 0;
}
