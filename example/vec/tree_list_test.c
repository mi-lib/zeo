#include <zeo/zeo_vec3d.h>

#define N 10

int main(int argc, char *argv[])
{
  zVec3DList list, list2;
  zVec3DTree tree;
  zVec3D v;
  int i;

  zRandInit();
  zListInit( &list );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DListInsert( &list, &v );
  }
  zVec3DList2Tree( &list, &tree );
  zVec3DTree2List( &tree, &list2 );
  zVec3DListPrint( &list );
  zVec3DListPrint( &list2 );
  zVec3DListDestroy( &list );
  zVec3DListDestroy( &list2 );
  zVec3DTreeDestroy( &tree );
  return 0;
}
