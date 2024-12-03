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

void output_box(FILE *fp, zBox3D *box)
{
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: box\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: cyan\n" );
  fprintf( fp, "center: " ); zVec3DValueNLFPrint( fp, zBox3DCenter(box) );
  fprintf( fp, "ax: " ); zVec3DValueNLFPrint( fp, zBox3DAxis(box,zX) );
  fprintf( fp, "ay: " ); zVec3DValueNLFPrint( fp, zBox3DAxis(box,zY) );
  fprintf( fp, "az: " ); zVec3DValueNLFPrint( fp, zBox3DAxis(box,zZ) );
  fprintf( fp, "depth: %g\n\n", zBox3DDepth(box) );
  fprintf( fp, "width: %g\n\n", zBox3DWidth(box) );
  fprintf( fp, "height: %g\n\n", zBox3DHeight(box) );
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

#define N 100
#define DEFAULT_MARGIN zTOL

int main(int argc, char *argv[])
{
  zVec3D c, ax, ay, az, tmp, v;
  zBox3D box;
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
  zBox3DCreate( &box, &c, &ax, &ay, &az, 5, 3, 2 );
  margin = argc > 1 ? atof( argv[1] ) : DEFAULT_MARGIN;
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    if( zBox3DPointIsInside( &box, &v, margin ) )
      output_point_inside( fp, &v, i );
    else
      output_point_outside( fp, &v, i );
  }
  output_box( fp, &box );
  fclose( fp );
  return 0;
}
