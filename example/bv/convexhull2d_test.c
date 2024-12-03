#include <zeo/zeo_bv2d.h>

#define N 1000

void generate_points(int n, zVec3D p[], zVec3DList *pl, const char filename[])
{
  FILE *fp;
  int i;

  zListInit( pl );
  fp = fopen( filename, "w" );
  for( i=0; i<n; i++ ){
#if 0
    zVec3DCreate( &p[i], zRandF(-10,10), zRandF(-10,10), 0 );
#else
    zVec3DCreatePolar( &p[i], zRandF(-10,10), 0.5*zPI, zRandF(-zPI,zPI) );
#endif
    zVec3DValueNLFPrint( fp, &p[i] );
    zVec3DListAdd( pl, &p[i] );
  }
  fclose( fp );
}

void output_convexhull(zLoop3D *ch, const char filename[])
{
  FILE *fp;
  zLoop3DCell *vc;

  fp = fopen( filename, "w" );
  zListForEach( ch, vc )
    zVec3DValueNLFPrint( fp, vc->data );
  zVec3DValueNLFPrint( fp, zListTail(ch)->data );
  fclose( fp );
}

void output_convexhull_ph(zPH3D *ph, const char filename[])
{
  FILE *fp;
  int i;

  fp = fopen( filename, "w" );
  for( i=0; i<zPH3DVertNum(ph); i++ )
    zVec3DValueNLFPrint( fp, zPH3DVert(ph,i) );
  zVec3DValueNLFPrint( fp, zPH3DVert(ph,0) );
  fclose( fp );
}

int main(void)
{
  zVec3DList pl;
  zVec3D p[N];
  zLoop3D ch;
  zPH3D ph;

  zRandInit();
  generate_points( N, p, &pl, "v" );

  zConvexHull2D( &ch, p, N ); /* array version */
  output_convexhull( &ch, "a" );
  zLoop3DDestroy( &ch );

  zConvexHull2DPL( &ch, &pl ); /* list version */
  output_convexhull( &ch, "l" );
  zLoop3DDestroy( &ch );

  zConvexHull2D2PH3D( &ph, p, N );
  output_convexhull_ph( &ph, "h" );

  zPH3DDestroy( &ph );
  zVec3DListDestroy( &pl );
  return 0;
}
