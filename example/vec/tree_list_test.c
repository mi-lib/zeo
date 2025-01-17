#include <zeo/zeo_vec3d_data.h>

#define N 10

int main(int argc, char *argv[])
{
  zVec3DList list1, list2;
  zVec3DTree tree;
  zVec3D v;
  int i;

  zRandInit();
  zListInit( &list1 );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DListAdd( &list1, &v );
  }
  zVec3DListToTree( &list1, &tree );
  zVec3DTreeToList( &tree, &list2 );
  zVec3DListPrint( &list1 );
  zVec3DListPrint( &list2 );
  zVec3DListDestroy( &list1 );
  zVec3DListDestroy( &list2 );
  zVec3DTreeDestroy( &tree );
  return 0;
}
