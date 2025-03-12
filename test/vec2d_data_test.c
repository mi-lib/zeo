#include <zeo/zeo_vec2d_data.h>

bool vec_data_test(zVec2DData *data)
{
  zVec2D point, *nn;
  zVec2D nn_answer = { { 1, 1 } };
  zVec2D sp_answer = { { 4, 4 } };

  zVec2DCreate( &point, 0.5, 1 );
  zVec2DDataNN( data, &point, &nn );
  return zVec2DEqual( nn, &nn_answer ) && zVec2DEqual( zVec2DDataSupportMap( data, ZVEC2DY ), &sp_answer );
}

#define N 5

void assert_vec2ddata_empty(void)
{
  zVec2DData data;
  bool result;

  zVec2DDataInitArray( &data, 0 );
  result = zVec2DDataIsEmpty( &data ) && !zVec2DDataSupportMap( &data, ZVEC2DY );
  zVec2DDataDestroy( &data );
  zAssert( zVec2DData (empty set), result );
}

void assert_vec2ddata_array(void)
{
  zVec2DData data;
  zVec2D v;
  int i;
  bool result;

  zVec2DDataInitArray( &data, N );
  for( i=0; i<N; i++ )
    zVec2DDataAdd( &data, zVec2DCreate( &v, i, i ) );
  result = vec_data_test( &data );
  zVec2DDataDestroy( &data );
  zAssert( zVec2DData (array), result );
}

void assert_vec2ddata_list(void)
{
  zVec2DData data;
  zVec2D v;
  int i;
  bool result;

  zVec2DDataInitList( &data );
  for( i=0; i<N; i++ )
    zVec2DDataAdd( &data, zVec2DCreate( &v, i, i ) );
  result = vec_data_test( &data );
  zVec2DDataDestroy( &data );
  zAssert( zVec2DData (list), result );
}

void assert_vec2ddata_addrlist(void)
{
  zVec2DData data;
  zVec2DArray array;
  int i;
  bool result;

  zVec2DArrayAlloc( &array, N );
  zVec2DDataInitAddrList( &data );
  for( i=0; i<N; i++ ){
    zVec2DDataAdd( &data, zVec2DCreate( zArrayElemNC(&array,i), i, i ) );
  }
  result = vec_data_test( &data );
  zVec2DDataDestroy( &data );
  zVec2DArrayFree( &array );
  zAssert( zVec2DData (address list), result );
}

void assert_vec2ddata_array_ptr(void)
{
  zVec2DData data;
  zVec2DArray array;
  int i;
  bool result;

  zAssert( zVec2DDataAssignArray (null pointer), !zVec2DDataAssignArray( &data, NULL ) );
  zVec2DArrayAlloc( &array, N );
  for( i=0; i<N; i++ )
    zVec2DCreate( zArrayElemNC(&array,i), i, i );
  zVec2DDataAssignArray( &data, &array );
  result = vec_data_test( &data );
  zVec2DDataDestroy( &data );
  zVec2DArrayFree( &array );
  zAssert( zVec2DData (array pointer), result );
}

void assert_vec2ddata_list_ptr(void)
{
  zVec2DData data;
  zVec2DList list;
  zVec2D v;
  int i;
  bool result;

  zAssert( zVec2DDataAssignList (null pointer), !zVec2DDataAssignList( &data, NULL ) );
  zListInit( &list );
  for( i=0; i<N; i++ )
    zVec2DListAdd( &list, zVec2DCreate( &v, i, i ) );
  zVec2DDataAssignList( &data, &list );
  result = vec_data_test( &data );
  zVec2DDataDestroy( &data );
  zVec2DListDestroy( &list );
  zAssert( zVec2DData (list pointer), result );
}

void assert_vec2ddata_addrlist_ptr(void)
{
  zVec2DData data;
  zVec2DArray array;
  zVec2DAddrList list;
  int i;
  bool result;

  zAssert( zVec2DDataAssignAddrList (null pointer), !zVec2DDataAssignAddrList( &data, NULL ) );
  zListInit( &list );
  zVec2DArrayAlloc( &array, N );
  for( i=0; i<N; i++ )
    zVec2DAddrListAdd( &list, zVec2DCreate( zArrayElemNC(&array,i), i, i ) );
  zVec2DDataAssignAddrList( &data, &list );
  result = vec_data_test( &data );
  zVec2DDataDestroy( &data );
  zVec2DAddrListDestroy( &list );
  zVec2DArrayFree( &array );
  zAssert( zVec2DData (address list pointer), result );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_vec2ddata_empty();
  assert_vec2ddata_array();
  assert_vec2ddata_list();
  assert_vec2ddata_addrlist();
  assert_vec2ddata_array_ptr();
  assert_vec2ddata_list_ptr();
  assert_vec2ddata_addrlist_ptr();
  return 0;
}
