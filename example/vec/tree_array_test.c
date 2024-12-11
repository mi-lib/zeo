#include <zeo/zeo_vec3d.h>

void array_print(zVec3DArray *array)
{
  int i;

  for( i=0; i<zArraySize(array); i++ ){
    printf( "#%d ", i );
    zVec3DPrint( zArrayElem(array,i) );
  }
}

#define N 10

int main(int argc, char *argv[])
{
  zVec3DArray array1, array2;
  zVec3DTree tree;
  int i;

  zRandInit();
  zVec3DArrayAlloc( &array1, N );
  for( i=0; i<N; i++ ){
    zVec3DCreate( zArrayElemNC(&array1,i), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  }
  zVec3DArrayToTree( &array1, &tree );
  zVec3DTreeToArray( &tree, &array2 );
  printf( ">>> array1\n" );
  array_print( &array1 );
  printf( ">>> array2\n" );
  array_print( &array2 );
  printf( ">>> tree\n" );
  zVec3DTreeFPrint( stdout, &tree );
  zArrayFree( &array1 );
  zArrayFree( &array2 );
  zVec3DTreeDestroy( &tree );
  return 0;
}
