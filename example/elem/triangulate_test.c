#include <zeo/zeo_triangulate.h>

int main(void)
{
  zVec3D v[] = {
#if 0
    { { 0.0, 0.0, 0 } },
    { {-0.1, 0.1, 0 } },
    { {-0.1, 0.2, 0 } },
    { { 0.1, 0.2, 0 } },
    { { 0.2, 0.4, 0 } },
    { { 0.1, 0.5, 0 } },
    { { 0.1, 0.6, 0 } },
    { { 0.3, 0.6, 0 } },
    { { 0.3, 0.0, 0 } },
#elif 0
    { { 0.0, 0.0, 0 } },
    { { 0.0, 0.1, 0 } },
    { { 0.1, 0.1, 0 } },
    { { 0.1, 0.2, 0 } },
    { { 0.2, 0.2, 0 } },
    { { 0.2, 0.0, 0 } },
#elif 0
    { { 0.2, 0.0, 0 } },
    { { 0.2, 0.2, 0 } },
    { { 0.1, 0.2, 0 } },
    { { 0.1, 0.1, 0 } },
    { { 0.0, 0.1, 0 } },
    { { 0.0, 0.0, 0 } },
#elif 1
    { { 0,-3, 0 } },
    { {-4, 0, 0 } },
    { {-2, 1, 0 } },
    { {-3, 6, 0 } },
    { {-6,-2, 0 } },
    { {-3,-5, 0 } },
    { {-5,-1, 0 } },
    { { 0,-6, 0 } },
    { { 4,-3, 0 } },
    { { 5,-4, 0 } },
    { { 6,-1, 0 } },
    { { 5, 4, 0 } },
    { { 0, 5, 0 } },
    { { 4, 0, 0 } },
    { { 2,-2, 0 } },
#elif 0
    { {-3, 0, 0 } },
    { { 0, 2, 0 } },
    { { 3, 1, 0 } },
    { {-1, 1, 0 } },
    { {-1,-1, 0 } },
    { { 3,-1, 0 } },
    { { 0,-2, 0 } },
#elif 0
    { { 0, 0, 0 } },
    { {-1, 1, 0 } },
    { {-1, 2, 0 } },
    { { 1, 2, 0 } },
    { { 2, 4, 0 } },
    { { 1, 5, 0 } },
    { { 1, 6, 0 } },
    { { 3, 6, 0 } },
    { { 3, 0, 0 } },
#else
    { { 0, 0, 0 } },
    { { 0, 1, 0 } },
    { { 1, 1, 0 } },
    { { 1, 2, 0 } },
    { { 2, 2, 0 } },
    { { 2, 1, 0 } },
    { { 3, 1, 0 } },
    { { 3, 0, 0 } },
#endif
  };
  zTri3DList tlist;
  zTri3DListCell *tp;
  register int i, n;
  FILE *fp;

  /* source vertices */
  fp = fopen( "org", "w" );
  n = sizeof(v)/sizeof(v[0]);
  for( i=0; i<n; i++ )
    zVec3DDataNLFWrite( fp, &v[i] );
  zVec3DDataNLFWrite( fp, &v[0] );
  fclose( fp );

  /* triangulation */
  n = zTriangulate( v, sizeof(v)/sizeof(zVec3D), &tlist );
  fp = fopen( "tri", "w" );
  zListForEach( &tlist, tp ){
    /* output for gnuplot visualization */
    zVec3DDataNLFWrite( fp, zTri3DVert(tp->data,0) );
    zVec3DDataNLFWrite( fp, zTri3DVert(tp->data,1) );
    zVec3DDataNLFWrite( fp, zTri3DVert(tp->data,2) );
    zVec3DDataNLFWrite( fp, zTri3DVert(tp->data,0) );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );

  printf( "vert.num=%d, tri.num=%d\n", sizeof(v)/sizeof(v[0]), n );
  zTri3DListDestroy( &tlist, true );
  return 0;
}
