#include <zeo/zeo_mat3d.h>

void test1(zMat3D *m)
{
  zMat3DZYX( m, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

void test2(zMat3D *m)
{
  /* irregular case */
  zVec3D a;

  zVec3DCreate( &a, zRandF(-1.0,1.0), zRandF(-1.0,1.0), zRandF(-1.0,1.0) );
  zVec3DNormalizeDRC( &a );
#if 0
  zVec3DOuterProd2Mat3D( &a, &a, m );
  zMat3DMulDRC( m, 2 );
  zMat3DAddDRC( m, Z_IDENTMAT3D );
#else
  zVec3DMulDRC( &a, zPI );
  zMat3DAA( m, &a );
#endif
}

int main(int argc, char *argv[])
{
  zMat3D m, md;
  zVec3D aa, e;

  zRandInit();
  argc > 1 ? test2( &m ) : test1( &m );
  zMat3DToAA( &m, &aa );
  zMat3DAA( &md, &aa );
  zMat3DWrite( &m );
  zMat3DWrite( &md );
  zMat3DError( &m, &md, &e );
  zVec3DWrite( &e );
  return 0;
}
