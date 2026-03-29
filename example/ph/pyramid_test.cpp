#include <zeo/zeo_ph3d.h>

void output(const zPH3D &ph)
{
  FILE *fp;

  fp = fopen( "pyramid.ztk", "w" );
  // for visualization
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 0.8 0.8 0.8\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.8\n" );
  fprintf( fp, "esr: 1.0\n\n" );

  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: pyramid\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: white\n" );
  ph.fprintZTK( fp );
  fclose( fp );
}

int main(void)
{
  zVec3D v[] = {
#if 1
    zVec3D( 0.0, 0.0, 0 ),
    zVec3D(-0.1, 0.1, 0 ),
    zVec3D(-0.1, 0.2, 0 ),
    zVec3D( 0.1, 0.2, 0 ),
    zVec3D( 0.2, 0.4, 0 ),
    zVec3D( 0.1, 0.5, 0 ),
    zVec3D( 0.1, 0.6, 0 ),
    zVec3D( 0.3, 0.6, 0 ),
    zVec3D( 0.3, 0.0, 0 ),
#elif 0
    zVec3D( 0.0, 0.0, 0 ),
    zVec3D( 0.0, 0.1, 0 ),
    zVec3D( 0.1, 0.0, 0 ),
#else
    zVec3D( 0.0, 0.0, 0 ),
    zVec3D( 0.1, 0.0, 0 ),
    zVec3D( 0.0, 0.1, 0 )
#endif
  };
  zVec3D vert( 0, 0.1, 0.5 );
  zPH3D pyr;

  pyr.createPyramid( v, sizeof(v)/sizeof(zVec3D), &vert );
  output( pyr );
  return 0;
}
