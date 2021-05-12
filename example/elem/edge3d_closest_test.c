#include <zeo/zeo_elem3d.h>

int main(void)
{
  zVec3D p1, p2, p, cp;
  zEdge3D e;
  FILE *fp;

  zRandInit();
  zVec3DCreate( &p1, zRandF(0,1), zRandF(0,1), 0 );
  zVec3DCreate( &p2, zRandF(0,1), zRandF(0,1), 0 );
  zEdge3DCreate( &e, &p1, &p2 );

  zVec3DCreate( &p, zRandF(0,1), zRandF(0,1), 0 );
  zEdge3DClosest( &e, &p, &cp );

  fp = fopen( "e", "w" );
  zVec3DDataNLFPrint( fp, zEdge3DVert(&e,0) );
  zVec3DDataNLFPrint( fp, zEdge3DVert(&e,1) );
  fclose( fp );

  fp = fopen( "s", "w" );
  zVec3DDataNLFPrint( fp, &p );
  fclose( fp );

  fp = fopen( "r", "w" );
  zVec3DDataNLFPrint( fp, &p );
  zVec3DDataNLFPrint( fp, &cp );
  fclose( fp );
  return 0;
}
