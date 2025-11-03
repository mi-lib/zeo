#include <zeo/zeo_vec3d_data.h>

bool vec_data_test(zVec3DData *data)
{
  const zVec3D point{ 0, 1, 1 };
  const zVec3D nn_answer{ 1, 1, 1 };
  const zVec3D sp_answer{ 4, 4, 4 };
  zVec3D *nn;

  data->nearestNeighbor( &point, &nn );
  return *nn == nn_answer && *data->supportMap( ZVEC3DZ ) == sp_answer;
}

#define N 5

void assert_vec3ddata_empty(void)
{
  zVec3DData data{ 0 };
  zAssert( C++::zVec3DData (empty set), data.isEmpty() && !data.supportMap( ZVEC3DZ ) );
}

void assert_vec3ddata_array(void)
{
  zVec3DData data{ N };
  zVec3D v;
  for(int i=0; i<N; i++ )
    data.add( &v.create( i, i, i ) );
  zAssert( C++::zVec3DData (array), vec_data_test( &data ) );
}

void assert_vec3ddata_array_direct(void)
{
  zVec3D v[N];
  for(int i=0; i<N; i++ )
    v[i].create( i, i, i );
  zVec3DData data{ v, N };
  zAssert( C++::zVec3DData (direct array), vec_data_test( &data ) );
}

void assert_vec3ddata_list(void)
{
  zVec3DData data;
  data.initList();
  zVec3D v;
  for(int i=0; i<N; i++ )
    data.add( &v.create( i, i, i ) );
  zAssert( C++::zVec3DData (list), vec_data_test( &data ) );
}

void assert_vec3ddata_addrlist(void)
{
  zVec3DData data;
  zVec3DArray array;

  array.alloc( N );
  data.initAddrList();
  for(int i=0; i<N; i++ )
    data.add( &array.get(i)->create( i, i, i ) );
  zAssert( C++::zVec3DData (address list), vec_data_test( &data ) );
}

void assert_vec3ddata_array_ptr(void)
{
  zVec3DData data;
  zVec3DArray array;

  zAssert( C++::zVec3DData.assignArray (null pointer), !data.assignArray( NULL ) );
  array.alloc( N );
  for(int i=0; i<N; i++ )
    array.get(i)->create( i, i, i );
  data.assignArray( &array );
  zAssert( C++::zVec3DData (array pointer), vec_data_test( &data ) );
}

void assert_vec3ddata_list_ptr(void)
{
  zVec3DData data;
  zVec3DList list;
  zVec3D v;

  zAssert( C++::zVec3DData.assignList (null pointer), !data.assignList( NULL ) );
  zListInit( &list );
  for(int i=0; i<N; i++ )
    zVec3DListAdd( &list, &v.create( i, i, i ) );
  data.assignList( &list );
  zAssert( zVec3DData (list pointer), vec_data_test( &data ) );
}

void assert_vec3ddata_addrlist_ptr(void)
{
  zVec3DData data;
  zVec3DArray array;
  zVec3DAddrList list;

  zAssert( C++::zVec3DData.assignAddrList (null pointer), !data.assignAddrList( NULL ) );
  zListInit( &list );
  array.alloc( N );
  for(int i=0; i<N; i++ )
    zVec3DAddrListAdd( &list, &array.get(i)->create( i, i, i ) );
  data.assignAddrList( &list );
  zAssert( zVec3DData (address list pointer), vec_data_test( &data ) );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_vec3ddata_empty();
  assert_vec3ddata_array();
  assert_vec3ddata_array_direct();
  assert_vec3ddata_list();
  assert_vec3ddata_addrlist();
  assert_vec3ddata_array_ptr();
  assert_vec3ddata_list_ptr();
  assert_vec3ddata_addrlist_ptr();
  return 0;
}
