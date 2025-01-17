#include <zeo/zeo_vec3d_data.h>

#define N 10

int main(int argc, char *argv[])
{
  zVec3DTree tree;
  zVec3D v;
  int i;

  zRandInit();
  zVec3DTreeInit( &tree );
  zVec3DCreate( &tree.data.vmin, -10, -10, -10);
  zVec3DCreate( &tree.data.vmax, 10, 10, 10 );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DTreeAdd( &tree, &v );
  }
  zVec3DTreeFPrint( stdout, &tree );
  zVec3DTreeDestroy( &tree );
  return 0;
}
