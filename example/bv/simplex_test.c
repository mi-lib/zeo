#include <zeo/zeo_bv3d.h>

int zTetra(zVec3D p[], int n, zVec3D *v[])
{
  zEdge3D e;
  zTri3D t;
  double d, d_max, d_min;
  register int i;

  d_max = d_min = p[0].e[zX];
  for( i=0; i<n; i++ ){
    if( p[i].e[zX] > d_max ){
      d_max = p[i].e[zX];
      v[0] = &p[i];
    }
    if( p[i].e[zX] < d_min ){
      d_min = p[i].e[zX];
      v[1] = &p[i];
    }
  }
  v[2] = v[3] = v[0];
  zEdge3DCreate( &e, v[0], v[1] );
  if( zVec3DIsTiny( zEdge3DVec(&e) ) ){
    ZRUNERROR( ZEO_ERR_CH_DEG1 );
    return 1;
  }
  d_max = 0;
  for( i=0; i<n; i++ )
    if( ( d = zEdge3DPointIsOn( &e, &p[i], zTOL ) ) > d_max ){
      d_max = d;
      v[2] = &p[i];
    }
  if( !zTri3DCreate( &t, v[0], v[1], v[2] ) ){
    ZRUNERROR( ZEO_ERR_CH_DEG2 );
    return 2;
  }
  d_max = 0;
  for( i=0; i<n; i++ )
    if( ( d = zTri3DPointIsOnPlane( &t, &p[i], zTOL ) ) > d_max ){
      d_max = d;
      v[3] = &p[i];
    }
  if( d_max == 0 ){
    ZRUNERROR( ZEO_ERR_CH_DEG2 );
    return 3;
  }
  return 4;
}


#define N 100
zVec3D p[N];

void test_point(void)
{
  register int i;
  FILE *fp;

  fp = fopen( "src", "w" );
  zRandInit();
  for( i=0; i<N; i++ ){
    zVec3DCreate( &p[i], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DDataNLFPrint( fp, &p[i] );
  }
  fclose( fp );
}

void output_face(FILE *fp, zVec3D *v[], int i1, int i2, int i3)
{
  zVec3DDataNLFPrint( fp, v[i1] );
  zVec3DDataNLFPrint( fp, v[i2] );
  fprintf( fp, "\n" );
  zVec3DDataNLFPrint( fp, v[i3] );
  zVec3DDataNLFPrint( fp, v[i3] );
  fprintf( fp, "\n\n" );
}

void output(zVec3D *v[])
{
  FILE *fp;

  fp = fopen( "v", "w" );
  output_face( fp, v, 0, 1, 2 );
  output_face( fp, v, 0, 2, 3 );
  output_face( fp, v, 0, 3, 1 );
  output_face( fp, v, 1, 2, 3 );
  fclose( fp );
}

int main(void)
{
  zVec3D *v[4];
  int dim;

  test_point();
  dim = zTetra( p, N, v );
  printf( "dim = %d\n", dim );
  output( v );
  return 0;
}
