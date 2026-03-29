#include <zeo/zeo_elem3d.h>

#define TEST 0

zVec3D test_loop[] = {
#if TEST == 0
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 0, 0 ),
#elif TEST == 1
  zVec3D( 0.0, 0.0, 0 ),
  zVec3D(-0.1, 0.1, 0 ),
  zVec3D(-0.1, 0.2, 0 ),
  zVec3D( 0.1, 0.2, 0 ),
  zVec3D( 0.2, 0.4, 0 ),
  zVec3D( 0.1, 0.5, 0 ),
  zVec3D( 0.1, 0.6, 0 ),
  zVec3D( 0.3, 0.6, 0 ),
  zVec3D( 0.3, 0.0, 0 ),
#elif TEST == 2
  zVec3D( 0.0, 0.0, 0 ),
  zVec3D( 0.0, 0.1, 0 ),
  zVec3D( 0.1, 0.1, 0 ),
  zVec3D( 0.1, 0.2, 0 ),
  zVec3D( 0.2, 0.2, 0 ),
  zVec3D( 0.2, 0.0, 0 ),
#elif TEST == 3
  zVec3D( 0.2, 0.0, 0 ),
  zVec3D( 0.2, 0.2, 0 ),
  zVec3D( 0.1, 0.2, 0 ),
  zVec3D( 0.1, 0.1, 0 ),
  zVec3D( 0.0, 0.1, 0 ),
  zVec3D( 0.0, 0.0, 0 ),
#elif TEST == 4
  zVec3D( 0,-3, 0 ),
  zVec3D(-4, 0, 0 ),
  zVec3D(-2, 1, 0 ),
  zVec3D(-3, 6, 0 ),
  zVec3D(-6,-2, 0 ),
  zVec3D(-3,-5, 0 ),
  zVec3D(-5,-1, 0 ),
  zVec3D( 0,-6, 0 ),
  zVec3D( 4,-3, 0 ),
  zVec3D( 5,-4, 0 ),
  zVec3D( 6,-1, 0 ),
  zVec3D( 5, 4, 0 ),
  zVec3D( 0, 5, 0 ),
  zVec3D( 4, 0, 0 ),
  zVec3D( 2,-2, 0 ),
#elif TEST == 5
  zVec3D(-3, 0, 0 ),
  zVec3D( 0, 2, 0 ),
  zVec3D( 3, 1, 0 ),
  zVec3D(-1, 1, 0 ),
  zVec3D(-1,-1, 0 ),
  zVec3D( 3,-1, 0 ),
  zVec3D( 0,-2, 0 ),
#elif TEST ==  6
  zVec3D( 0, 0, 0 ),
  zVec3D(-1, 1, 0 ),
  zVec3D(-1, 2, 0 ),
  zVec3D( 1, 2, 0 ),
  zVec3D( 2, 4, 0 ),
  zVec3D( 1, 5, 0 ),
  zVec3D( 1, 6, 0 ),
  zVec3D( 3, 6, 0 ),
  zVec3D( 3, 0, 0 ),
#elif TEST == 7
  zVec3D( 0, 0, 0 ),
  zVec3D( 0, 1, 0 ),
  zVec3D( 1, 1, 0 ),
  zVec3D( 1, 2, 0 ),
  zVec3D( 2, 2, 0 ),
  zVec3D( 2, 1, 0 ),
  zVec3D( 3, 1, 0 ),
  zVec3D( 3, 0, 0 ),
#else
#warning "This example doesn't work correctly."
#endif
};

void generate_loop(zLoop3D &loop, double xmin, double ymin, double xmax, double ymax)
{
  zVec3DArray va;
#if TEST == 0
  double s, c, x0, y0, rx, ry, r;
  int i, n;

  x0 = 0.5 * ( xmin + xmax );
  y0 = 0.5 * ( ymin + ymax );
  rx = 0.5 * fabs( xmax - xmin );
  ry = 0.5 * fabs( ymax - ymin );
  n = sizeof(test_loop)/sizeof(zVec3D);
  for( i=0; i<n; i++ ){
    zSinCos( zPIx2*i/n, &s, &c );
    r = zRandF( 0, 1 );
    test_loop[i].create( r*rx*c+x0, r*ry*s+y0, 0 );
  }
#endif
  zArraySize(&va) = sizeof(test_loop)/sizeof(zVec3D);
  zArrayBuf(&va) = test_loop;
  zVec3DAddrListCreate( &loop, &va );
}

void output_loop(const zLoop3D &loop, char filename[])
{
  FILE *fp;
  zLoop3DCell *cp;

  fp = fopen( filename, "w" );
  zListForEach( &loop, cp )
    zVec3DValueNLFPrint( fp, cp->data );
  zVec3DValueNLFPrint( fp, zListTail(&loop)->data );
  fclose( fp );
}

void output_triangles(const zTri3DList &tlist, char filename[])
{
  FILE *fp;
  zTri3DListCell *tp;

  fp = fopen( filename, "w" );
  zListForEach( &tlist, tp ){
    // for gnuplot visualization
    zVec3DValueNLFPrint( fp, zTri3DVert(&tp->data,0) );
    zVec3DValueNLFPrint( fp, zTri3DVert(&tp->data,1) );
    zVec3DValueNLFPrint( fp, zTri3DVert(&tp->data,2) );
    zVec3DValueNLFPrint( fp, zTri3DVert(&tp->data,0) );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );
}

#define XMIN 0
#define YMIN 0
#define XMAX 10
#define YMAX 10

int main(void)
{
  zLoop3D loop;
  zTri3DList tlist;

  zRandInit();
  // source vertices
  generate_loop( loop, XMIN, YMIN, XMAX, YMAX );
  output_loop( loop, "org.dat" );

  // triangulation of a loop
  printf( "vert.num=%d, tri.num=%d\n", zListSize(&loop), zLoop3DTriangulate( &loop, &tlist ) );
  output_triangles( tlist, "tri.dat" );

  zTri3DListDestroy( &tlist );
  zLoop3DDestroy( &loop );
  return 0;
}
