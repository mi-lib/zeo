#include <zeo/zeo_bv3d.h>
#include <zeo/zeo_mshape3d.h>

#define N 100

int read_mshape_points(zVec3D p[], int n, const char filename[])
{
  zMShape3D ms;
  zPH3D *ph;
  int i, j, k;

  if( !zMShape3DReadZTK( &ms, filename ) ) return 0;
  for( k=0, i=0; i<zMShape3DShapeNum(&ms); i++ ){
    ph = zShape3DPH(zMShape3DShape(&ms,i));
    for( j=0; j<zPH3DVertNum(ph); j++ ){
      zVec3DCopy( zPH3DVert(ph,j), &p[k++] );
      if( k >= n ) goto TERMINATE;
    }
  }
 TERMINATE:
  zMShape3DDestroy( &ms );
  return k;
}

void generate_points(zVec3D p[], int n, zVec3DList *pl, const char filename[])
{
  FILE *fp;
  int i;

  zRandInit();
  zListInit( pl );
  fp = fopen( filename, "w" );
  for( i=0; i<n; i++ ){
#if 0
    zVec3DCreatePolar( &p[i], zRandF(-5,5), zRandF(-zPI,zPI), zRandF(-0.5*zPI,0.5*zPI) );
    p[i].c.x += 3;
#elif 0
    zVec3DCreate( &p[i], zRandI(-5,5), zRandI(-5,5), zRandI(-5,5) );
#else
    zVec3DCreate( &p[i], zRandF(-5,5), zRandF(-5,5), 0 );
#endif
    zVec3DListAdd( pl, &p[i] );
    zVec3DValueNLFPrint( fp, &p[i] );
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
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: %s\n", colorname );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: %g %g %g\n", r, g, b );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );

  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: %s\n", shapename );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: %s\n", colorname );
  zPH3DFPrintZTK( fp, ph );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zPH3D ch1, ch2;
  zVec3D p[N];
  zVec3DList pl;
  int n = N;

  if( argc > 1 ){
    n = read_mshape_points( p, N, argv[1] );
  } else{
    generate_points( p, N, &pl, "v" );
  }
  zConvexHull3D( &ch1, p, n );
  zConvexHull3DPL( &ch2, &pl );

  output_convexhull( &ch1, "ch1", "red", 1.0, 0.2, 0.2 );
  output_convexhull( &ch2, "ch2", "cyan", 0.5, 1.0, 1.0 );
  zPH3DDestroy( &ch1 );
  zPH3DDestroy( &ch2 );
  zVec3DListDestroy( &pl );
  return 0;
}
