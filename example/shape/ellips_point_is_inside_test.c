#include <zeo/zeo_shape3d.h>

void output_header(FILE *fp)
{
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 1.0 1.0 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.8\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 1.0 0.0 0.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: cyan\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 0.4 1.0 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.3\n" );
  fprintf( fp, "esr: 1.0\n\n" );
}

void output_ellips(FILE *fp, zEllips3D *ellips)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: ellipsoid\n" );
  fprintf( fp, "type: ellipsoid\n" );
  fprintf( fp, "optic: cyan\n" );
  fprintf( fp, "center: " );
  zVec3DValueNLFPrint( fp, zEllips3DCenter(ellips) );
  fprintf( fp, "ax: " );
  zVec3DValueNLFPrint( fp, zEllips3DAxis(ellips,zX) );
  fprintf( fp, "ay: " );
  zVec3DValueNLFPrint( fp, zEllips3DAxis(ellips,zY) );
  fprintf( fp, "az: " );
  zVec3DValueNLFPrint( fp, zEllips3DAxis(ellips,zZ) );
  fprintf( fp, "rx: %g\n\n", zEllips3DRadiusX(ellips) );
  fprintf( fp, "ry: %g\n\n", zEllips3DRadiusY(ellips) );
  fprintf( fp, "rz: %g\n\n", zEllips3DRadiusZ(ellips) );
}

void output_point_inside(FILE *fp, zVec3D *p, int i)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: point%d\n", i );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: red\n" );
  fprintf( fp, "center: " );
  zVec3DValueNLFPrint( fp, p );
  fprintf( fp, "radius: 0.1\n\n" );
}

void output_point_outside(FILE *fp, zVec3D *p, int i)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: point%d\n", i );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: white\n" );
  fprintf( fp, "center: " );
  zVec3DValueNLFPrint( fp, p );
  fprintf( fp, "radius: 0.05\n\n" );
}

#define N 500
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zVec3D c, v, ax, ay, az, tmp;
  zEllips3D ellips;
  double margin;
  int i;
  FILE *fp;

  zRandInit();
  fp = fopen( "result.ztk", "w" );
  output_header( fp );
  zVec3DCreate( &c, 1, 1, 1 );
  zVec3DCreate( &ax, 5, 1, -1 ); zVec3DNormalizeDRC( &ax );
  zVec3DCreate( &tmp, -1, -1, 8 ); zVec3DOuterProd( &ax, &tmp, &ay ); zVec3DNormalizeDRC( &ay );
  zVec3DOuterProd( &ax, &ay, &az ); zVec3DNormalizeDRC( &az );
  zEllips3DCreate( &ellips, &c, &ax, &ay, &az, 5, 3, 2, 0 );

  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    if( zEllips3DPointIsInside( &ellips, &v, margin ) )
      output_point_inside( fp, &v, i );
#if 0
    else
      output_point_outside( fp, &v, i );
#endif
  }
  output_ellips( fp, &ellips );
  fclose( fp );
  return 0;
}
