#include <zeo/zeo_vec3d_data.h>
#include <zeo/zeo_vec3d_profile.h>

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

#define NP 1000

int cmp(void *a, void *b, void *dummy)
{
  zVec3D *v1, *v2;

  v1 = &((zVec3DListCell*)a)->data;
  v2 = &((zVec3DListCell*)b)->data;
  if( v1->e[zX] > v2->e[zX] ) return 1;
  if( v1->e[zX] < v2->e[zX] ) return -1;
  if( v1->e[zY] > v2->e[zY] ) return 1;
  if( v1->e[zY] < v2->e[zY] ) return -1;
  if( v1->e[zZ] > v2->e[zZ] ) return 1;
  if( v1->e[zZ] < v2->e[zZ] ) return -1;
  return 0;
}

void assert_vicinity(void)
{
  zVec3DTree tree;
  zVec3DOctree octree;
  zVec3DData vicinity_tree, vicinity_octree;
  zVec3D point, *v;
  bool result = true;
  int i;

  zVec3DTreeInit( &tree );
  zAABox3DCreate( &tree.data.region, -10, -10, -10, 10, 10, 10 );
  zVec3DOctreeInit( &octree, -10, -10, -10, 10, 10, 10, 1 );
  for( i=0; i<NP; i++ ){
    zVec3DCreate( &point, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DTreeAddPoint( &tree, &point );
    zVec3DOctreeAddPoint( &octree, &point );
  }

  zVec3DCreate( &point, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

  zVec3DDataInitAddrList( &vicinity_tree );
  zVec3DTreeVicinity( &tree, &point, 5, &vicinity_tree );

  zVec3DDataInitAddrList( &vicinity_octree );
  zVec3DOctreeVicinity( &octree, &point, 5, &vicinity_octree );

  if( zVec3DDataSize(&vicinity_tree) != zVec3DDataSize(&vicinity_octree) ){
    eprintf( "size of vicinity (by kd-tree) = %d\n", zVec3DDataSize(&vicinity_tree) );
    eprintf( "size of vicinity (by kd-tree) = %d\n", zVec3DDataSize(&vicinity_octree) );
    result = false;
  }
  /* brute-force confirmation */
  zVec3DDataRewind( &vicinity_tree );
  while( ( v = zVec3DDataFetch( &vicinity_tree ) ) )
    if( !zVec3DDataFind( &vicinity_octree, v ) ){
      eprintf( "point not found in the octree: " );
      zVec3DFPrint( stderr, v );
      result = false;
    }
  zVec3DDataRewind( &vicinity_octree );
  while( ( v = zVec3DDataFetch( &vicinity_octree ) ) )
    if( !zVec3DDataFind( &vicinity_tree, v ) ){
      eprintf( "point not found in the kd-tree: " );
      zVec3DFPrint( stderr, v );
      result = false;
    }

  zVec3DDataDestroy( &vicinity_tree );
  zVec3DDataDestroy( &vicinity_octree );
  zVec3DTreeDestroy( &tree );
  zVec3DOctreeDestroy( &octree );
  zAssert( zVec3DTreeVicinity + zVec3DOctreeVicinity, result );
}

void generate_frame(zFrame3D *frame)
{
  zVec3DCreate( zFrame3DPos(frame), zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zMat3DFromZYX( zFrame3DAtt(frame), zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

void generate_points(zVec3DData *src, zVec3DData *dest, zFrame3D *frame, int n, double tol)
{
  zVec3D po, p, noise;

  while( --n >= 0 ){
    zVec3DCreate( &po, zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    zVec3DCreate( &noise, zRandF(-tol,tol), zRandF(-tol,tol), zRandF(-tol,tol) );
    zXform3D( frame, &po, &p );
    zVec3DAddDRC( &p, &noise );
    zVec3DDataAdd( src, &po );
    zVec3DDataAdd( dest, &p );
  }
}

bool check_frame_error(zFrame3D *frame, zFrame3D *frame_ident, double tol)
{
  zVec6D error;

  zFrame3DError( frame, frame_ident, &error );
  return zVec6DIsTol( &error, tol );
}

void assert_frame_ident(void)
{
  int testnum = 1000;
  double tol = 1.0e-4;
  zVec3DData src, dest;
  zFrame3D frame, frame_ident;
  bool result;

  zVec3DDataInitList( &src );
  zVec3DDataInitList( &dest );
  generate_frame( &frame );
  generate_points( &src, &dest, &frame, testnum, tol );
  zVec3DDataIdentFrame( &src, &dest, &frame_ident );
  result = check_frame_error( &frame, &frame_ident, tol );
  zVec3DDataDestroy( &dest );
  zVec3DDataDestroy( &src );
  zAssert( zVec3DDataIdentFrame, result );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_vec3ddata_empty();
  assert_vec3ddata_array();
  assert_vec3ddata_list();
  assert_vec3ddata_addrlist();
  assert_vec3ddata_array_ptr();
  assert_vec3ddata_list_ptr();
  assert_vec3ddata_addrlist_ptr();
  assert_vicinity();
  assert_frame_ident();
  return 0;
}
