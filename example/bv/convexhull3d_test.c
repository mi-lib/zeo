#include <zeo/zeo_bv3d.h>

#define N 100

void generate_points(zVec3DData *data_array, zVec3DData *data_list, int n, const char filename[])
{
  zVec3D v;
  int i;
  FILE *fp;

  zVec3DDataInitArray( data_array, n );
  zVec3DDataInitList( data_list );
  fp = fopen( filename, "w" );
  for( i=0; i<n; i++ ){
#if 0
    zVec3DCreatePolar( &v, zRandF(-5,5), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    v.c.x += 3;
#elif 1
    zVec3DCreate( &v, zRandI(-5,5), zRandI(-5,5), zRandI(-5,5) );
#else
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), 0 );
#endif
    zVec3DValueNLFPrint( fp, &v );
    zVec3DDataAdd( data_array, &v );
    zVec3DDataAdd( data_list, &v );
  }
  fclose( fp );
}

void output_convexhull(zPH3D *ph, const char shapename[], const char colorname[], double r, double g, double b)
{
  FILE *fp;
  char filename[BUFSIZ];

  eprintf( "%d vertices, %d faces.\n", zPH3DVertNum(ph), zPH3DFaceNum(ph) );
  zAddSuffix( shapename, "ztk", filename, BUFSIZ );
  fp = fopen( filename, "w" );
  /* for visualization */
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: %s\n", colorname );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: %g %g %g\n", r, g, b );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );

  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: %s\n", shapename );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: %s\n", colorname );
  zPH3DFPrintZTK( fp, ph );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zPH3D ch1, ch2;
  zVec3DData data_array, data_list;

  zRandInit();
  generate_points( &data_array, &data_list, N, "v" );
  zVec3DDataConvexHull( &data_array, &ch1 );
  zVec3DDataConvexHull( &data_list, &ch2 );
  zVec3DDataDestroy( &data_array );
  zVec3DDataDestroy( &data_list );

  output_convexhull( &ch1, "ch1", "red", 1.0, 0.2, 0.2 );
  output_convexhull( &ch2, "ch2", "cyan", 0.5, 1.0, 1.0 );
  zPH3DDestroy( &ch1 );
  zPH3DDestroy( &ch2 );
  return 0;
}
