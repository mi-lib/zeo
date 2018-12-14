#include <zeo/zeo_mat3d.h>

int main(void)
{
  zVec3D v1, v2;
  zMat3D m;

  zVec3DCreate( &v1, 1, 2, 3 );
  zVec3DCreate( &v2, 4, 5, 6 );
  zMat3DDyad( &v1, &v2, &m );
  printf( "v1=" );
  zVec3DWrite( &v1 );
  printf( "v2=" );
  zVec3DWrite( &v2 );
  printf( "v1 v2^T=" );
  zMat3DWrite( &m );
  return 0;
}
