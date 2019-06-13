#include <zeo/zeo_elem.h>

void create_tri(zTri3D *t, zVec3D *p1, zVec3D *p2, zVec3D *p3)
{
  FILE *fp;

  zVec3DCreate( p1, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( p2, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( p3, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zTri3DCreate( t, p1, p2, p3 );

  fp = fopen( "tri", "w" );
  zVec3DDataNLFPrint( fp, p1 );
  zVec3DDataNLFPrint( fp, p2 );
  fprintf( fp, "\n" );
  zVec3DDataNLFPrint( fp, p3 );
  zVec3DDataNLFPrint( fp, p3 );
  fclose( fp );
}

void output(zVec3D *p, zVec3D *cp)
{
  FILE *fp;

  fp = fopen( "p", "w" );
  zVec3DDataNLFPrint( fp, p );
  zVec3DDataNLFPrint( fp, cp );
  fclose( fp );
}

int main(void)
{
  zVec3D p1, p2, p3, p, cp;
  zTri3D t;

  zRandInit();
  create_tri( &t, &p1, &p2, &p3 );

  zVec3DCreate( &p, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  printf( "d = %f\n", zTri3DClosest( &t, &p, &cp ) );
  output( &p, &cp );
  return 0;
}
