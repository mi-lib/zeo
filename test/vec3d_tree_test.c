#include <zeo/zeo.h>

#define NS 1000
#define N   100

#define RESOLUTION 0.5

bool cmp_vec_array(zVec3DArray *array1, zVec3DArray *array2)
{
  int i;

  if( zArraySize(array1) != zArraySize(array2) ){
    ZRUNERROR( "mismatch size: %d / %d", zArraySize(array1), zArraySize(array2) );
    return false;
  }
  for( i=0; i<zArraySize(array1); i++ ){
    if( !zVec3DEqual( zArrayElemNC(array1,i), zArrayElemNC(array2,i) ) ){
      ZRUNERROR( "different vectors at %d", i );
      return false;
    }
  }
  return true;
}

bool cmp_vec_list(zVec3DList *list1, zVec3DList *list2)
{
  zVec3DListCell *cp1, *cp2;

  if( zListSize(list1) != zListSize(list2) ){
    ZRUNERROR( "mismatch size: %d / %d", zListSize(list1), zListSize(list2) );
    return false;
  }
  cp2 = zListTail(list2);
  zListForEach( list1, cp1 ){
    if( !zVec3DEqual( &cp1->data, &cp2->data ) ){
      ZRUNERROR( "different vectors" );
      return false;
    }
    cp2 = zListCellNext(cp2);
  }
  return true;
}

bool cmp_vec_data(zVec3DData *data1, zVec3DData *data2)
{
  zVec3D *v, *nn;

  if( zVec3DDataSize(data1) != zVec3DDataSize(data2) ){
    ZRUNERROR( "mismatch size: %d / %d", zVec3DDataSize(data1), zVec3DDataSize(data2) );
    return false;
  }
  zVec3DDataRewind( data1 );
  while( ( v = zVec3DDataFetch( data1 ) ) ){
    if( !zIsTiny( zVec3DDataNN( data2, v, &nn ) ) ) return false;
  }
  zVec3DDataRewind( data2 );
  while( ( v = zVec3DDataFetch( data2 ) ) ){
    if( !zIsTiny( zVec3DDataNN( data1, v, &nn ) ) ) return false;
  }
  return true;
}

void assert_nn(void)
{
  zVec3DOctree octree;
  zVec3DTree tree, *node_nn;
  zVec3DData pointdata;
  int i;
  zVec3D v, *nn_octree, *nn_data;
  bool result;
  const double xmin = -10, xmax = 10, ymin = -10, ymax = 10, zmin = -10, zmax = 10;

  zRandInit();
  zVec3DDataInitArray( &pointdata, NS );
  for( i=0; i<zVec3DDataSize(&pointdata); i++ ){
    zVec3DCreate( &v, zRandF(xmin,xmax), zRandF(ymin,ymax), zRandF(zmin,zmax) );
    zVec3DDataAdd( &pointdata, &v );
  }
  zVec3DOctreeInit( &octree, xmin, ymin , zmin, xmax, ymax, zmax, RESOLUTION );
  zVec3DOctreeAddData( &octree, &pointdata );
  zVec3DTreeInit( &tree );
  zVec3DTreeAddData( &tree, &pointdata );

  zVec3DCreate( &v, zRandF(xmin,xmax), zRandF(ymin,ymax), zRandF(zmin,zmax) );

  zVec3DTreeNN( &tree, &v, &node_nn );
  zVec3DOctreeNN( &octree, &v, &nn_octree );
  zVec3DDataNN( &pointdata, &v, &nn_data );
  result = zVec3DEqual( &node_nn->data.v, nn_octree ) && zVec3DEqual( nn_data, nn_octree );

  zVec3DTreeDestroy( &tree );
  zVec3DOctreeDestroy( &octree );
  zVec3DDataDestroy( &pointdata );

  zAssert( zVec3DTreeNN / zVec3DOctreeNN, result );
}

void assert_vicinity(void)
{
  zVec3DOctree octree;
  zVec3DTree tree;
  zVec3DData pointdata;
  zVec3DData vicinity1, vicinity2;
  int i;
  zVec3D v;
  bool result;
  const double xmin = -10, xmax = 10, ymin = -10, ymax = 10, zmin = -10, zmax = 10;
  const double radius = 5.0;

  zVec3DDataInitArray( &pointdata, NS );
  for( i=0; i<zVec3DDataSize(&pointdata); i++ ){
    zVec3DCreate( &v, zRandF(xmin,xmax), zRandF(ymin,ymax), zRandF(zmin,zmax) );
    zVec3DDataAdd( &pointdata, &v );
  }
  zVec3DOctreeInit( &octree, xmin, ymin , zmin, xmax, ymax, zmax, RESOLUTION );
  zVec3DOctreeAddData( &octree, &pointdata );
  zVec3DTreeInit( &tree );
  zVec3DTreeAddData( &tree, &pointdata );

  zVec3DCreate( &v, zRandF(xmin,xmax), zRandF(ymin,ymax), zRandF(zmin,zmax) );

  zVec3DDataInitAddrList( &vicinity1 );
  zVec3DTreeVicinity( &tree, &v, radius, &vicinity1 );
  zVec3DDataInitAddrList( &vicinity2 );
  zVec3DOctreeVicinity( &octree, &v, radius, &vicinity2 );
  result = cmp_vec_data( &vicinity1, &vicinity2 );

  zVec3DDataDestroy( &vicinity1 );
  zVec3DDataDestroy( &vicinity2 );
  zVec3DTreeDestroy( &tree );
  zVec3DOctreeDestroy( &octree );
  zVec3DDataDestroy( &pointdata );
  zAssert( zVec3DTreeVicinity / zVec3DOctreeVicinity, result );
}

void assert_vec_tree_to_array(void)
{
  zVec3DArray array1, array2;
  zVec3DTree tree;
  int i;
  bool result;

  zVec3DArrayAlloc( &array1, N );
  for( i=0; i<N; i++ ){
    zVec3DCreate( zArrayElemNC(&array1,i), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  }
  zVec3DArrayToTree( &array1, &tree );
  zVec3DTreeToArray( &tree, &array2 );
  result = cmp_vec_array( &array1, &array2 );
  zArrayFree( &array1 );
  zArrayFree( &array2 );
  zVec3DTreeDestroy( &tree );
  zAssert( zVec3DArrayToTree + zVec3DTreeToArray, result );
}

void assert_vec_tree_to_list(void)
{
  zVec3DList list1, list2;
  zVec3DTree tree;
  zVec3D v;
  int i;
  bool result;

  zListInit( &list1 );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DListAdd( &list1, &v );
  }
  zVec3DListToTree( &list1, &tree );
  zVec3DTreeToList( &tree, &list2 );
  zVec3DListQuickSortDefault( &list1 );
  zVec3DListQuickSortDefault( &list2 );
  result = cmp_vec_list( &list1, &list2 );
  zVec3DListDestroy( &list1 );
  zVec3DListDestroy( &list2 );
  zVec3DTreeDestroy( &tree );
  zAssert( zVec3DListToTree + zVec3DTreeToList, result );
}

void assert_vec_tree_to_data(void)
{
  zVec3DData data1, data2;
  zVec3DTree tree;
  zVec3D v;
  int i;
  bool result;

  zVec3DDataInitArray( &data1, N );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DDataAdd( &data1, &v );
  }
  zVec3DTreeInit( &tree );
  zVec3DTreeAddData( &tree, &data1 );
  zVec3DDataInitArray( &data2, tree.size );
  zVec3DTreeToData( &tree, &data2 );
  result = cmp_vec_data( &data1, &data2 );
  zVec3DTreeDestroy( &tree );
  zVec3DDataDestroy( &data1 );
  zVec3DDataDestroy( &data2 );
  zAssert( zVec3DTreeAddData + zVec3DTreeToData, result );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_nn();
  assert_vicinity();
  assert_vec_tree_to_array();
  assert_vec_tree_to_list();
  assert_vec_tree_to_data();
  return 0;
}
