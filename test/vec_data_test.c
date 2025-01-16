#include <zeo/zeo_vec3d_data.h>

bool vec_data_test(zVec3DData *data)
{
  zVec3D point, *nn;
  zVec3D nn_answer = { { 1, 1, 1 } };
  zVec3D sp_answer = { { 4, 4, 4 } };

  zVec3DCreate( &point, 0, 1, 1 );
  zVec3DDataNN( data, &point, &nn );
  return zVec3DEqual( nn, &nn_answer ) && zVec3DEqual( zVec3DDataSupportMap( data, ZVEC3DZ ), &sp_answer );
}

#define N 5

void assert_vec3ddata_empty(void)
{
  zVec3DData data;
  bool result;

  zVec3DDataInitArray( &data, 0 );
  result = zVec3DDataIsEmpty( &data ) && !zVec3DDataSupportMap( &data, ZVEC3DZ );
  zVec3DDataDestroy( &data );
  zAssert( zVec3DData (empty set), result );
}

void assert_vec3ddata_array(void)
{
  zVec3DData data;
  zVec3D v;
  int i;
  bool result;

  zVec3DDataInitArray( &data, N );
  for( i=0; i<N; i++ )
    zVec3DDataAdd( &data, zVec3DCreate( &v, i, i, i ) );
  result = vec_data_test( &data );
  zVec3DDataDestroy( &data );
  zAssert( zVec3DData (array), result );
}

void assert_vec3ddata_list(void)
{
  zVec3DData data;
  zVec3D v;
  int i;
  bool result;

  zVec3DDataInitList( &data );
  for( i=0; i<N; i++ )
    zVec3DDataAdd( &data, zVec3DCreate( &v, i, i, i ) );
  result = vec_data_test( &data );
  zVec3DDataDestroy( &data );
  zAssert( zVec3DData (list), result );
}

void assert_vec3ddata_addrlist(void)
{
  zVec3DData data;
  zVec3DArray array;
  int i;
  bool result;

  zVec3DArrayAlloc( &array, N );
  zVec3DDataInitAddrList( &data );
  for( i=0; i<N; i++ ){
    zVec3DDataAdd( &data, zVec3DCreate( zArrayElemNC(&array,i), i, i, i ) );
  }
  result = vec_data_test( &data );
  zVec3DDataDestroy( &data );
  zVec3DArrayFree( &array );
  zAssert( zVec3DData (address list), result );
}

void assert_vec3ddata_array_ptr(void)
{
  zVec3DData data;
  zVec3DArray array;
  int i;
  bool result;

  zAssert( zVec3DDataAssignArray (null pointer), !zVec3DDataAssignArray( &data, NULL ) );
  zVec3DArrayAlloc( &array, N );
  for( i=0; i<N; i++ )
    zVec3DCreate( zArrayElemNC(&array,i), i, i, i );
  zVec3DDataAssignArray( &data, &array );
  result = vec_data_test( &data );
  zVec3DDataDestroy( &data );
  zVec3DArrayFree( &array );
  zAssert( zVec3DData (array pointer), result );
}

void assert_vec3ddata_list_ptr(void)
{
  zVec3DData data;
  zVec3DList list;
  zVec3D v;
  int i;
  bool result;

  zAssert( zVec3DDataAssignList (null pointer), !zVec3DDataAssignList( &data, NULL ) );
  zListInit( &list );
  for( i=0; i<N; i++ )
    zVec3DListAdd( &list, zVec3DCreate( &v, i, i, i ) );
  zVec3DDataAssignList( &data, &list );
  result = vec_data_test( &data );
  zVec3DDataDestroy( &data );
  zVec3DListDestroy( &list );
  zAssert( zVec3DData (list pointer), result );
}

void assert_vec3ddata_addrlist_ptr(void)
{
  zVec3DData data;
  zVec3DArray array;
  zVec3DAddrList list;
  int i;
  bool result;

  zAssert( zVec3DDataAssignAddrList (null pointer), !zVec3DDataAssignAddrList( &data, NULL ) );
  zListInit( &list );
  zVec3DArrayAlloc( &array, N );
  for( i=0; i<N; i++ )
    zVec3DAddrListAdd( &list, zVec3DCreate( zArrayElemNC(&array,i), i, i, i ) );
  zVec3DDataAssignAddrList( &data, &list );
  result = vec_data_test( &data );
  zVec3DDataDestroy( &data );
  zVec3DAddrListDestroy( &list );
  zVec3DArrayFree( &array );
  zAssert( zVec3DData (address list pointer), result );
}

int main(int argc, char *argv[])
{
  assert_vec3ddata_empty();
  assert_vec3ddata_array();
  assert_vec3ddata_list();
  assert_vec3ddata_addrlist();
  assert_vec3ddata_array_ptr();
  assert_vec3ddata_list_ptr();
  assert_vec3ddata_addrlist_ptr();
  return 0;
}
