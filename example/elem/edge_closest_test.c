#include <zeo/zeo_elem.h>

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
  zVec3DDataNLFWrite( fp, zEdge3DVert(&e,0) );
  zVec3DDataNLFWrite( fp, zEdge3DVert(&e,1) );
  fclose( fp );

  fp = fopen( "s", "w" );
  zVec3DDataNLFWrite( fp, &p );
  fclose( fp );

  fp = fopen( "r", "w" );
  zVec3DDataNLFWrite( fp, &p );
  zVec3DDataNLFWrite( fp, &cp );
  fclose( fp );
  return 0;
}
