#include <zeo/zeo_vec3d_data.h>
#include <zeo/zeo_vec3d_profile.h>

bool vec_data_test(zVec3DData &data)
{
  zVec3D point, *nn;
  zVec3D nn_answer( 1, 1, 1 );
  zVec3D sp_answer( 4, 4, 4 );

  point.create( 0, 1, 1 );
  data.nearestNeighbor( &point, &nn );
  return ((*nn) == nn_answer) && (*data.supportMap( ZVEC3DZ ) == sp_answer);
}

#define N 5

void assert_vec3ddata_empty(void)
{
  bool result;

  {
    zVec3DData data;
    data.initArray( 0 );
    result = data.isEmpty() && !data.supportMap( ZVEC3DZ );
  }

  zAssert( zVec3DData (empty set), result );
}

void assert_vec3ddata_array(void)
{
  int i;
  bool result;

  {
    zVec3DData data;
    data.initArray( N );
    for( i=0; i<N; i++ ){
      zVec3D v( i, i, i );
      data.add( &v );
    }
    result = vec_data_test( data );
  }
  zAssert( zVec3DData (array), result );
}

void assert_vec3ddata_array_direct(void)
{
  zVec3D v[N];
  int i;
  bool result;

  {
    zVec3DData data;
    for( i=0; i<N; i++ )
      v[i].create( i, i, i );
    data.assignArrayDirect( v, N );
    result = vec_data_test( data );
  }
  zAssert( zVec3DData (direct array), result );
}

void assert_vec3ddata_list(void)
{
  int i;
  bool result;

  {
    zVec3DData data;
    data.initList();
    for( i=0; i<N; i++ ){
      zVec3D v( i, i, i );
      data.add( &v );
    }
    result = vec_data_test( data );
  }
  zAssert( zVec3DData (list), result );
}

void assert_vec3ddata_addrlist(void)
{
  int i;
  bool result;

  {
    zVec3DData data;
    zVec3DArray array( N );
    data.initAddrList();
    for( i=0; i<N; i++ ){
      array.get(i)->create( i, i, i );
      data.add( array.get(i) );
    }
    result = vec_data_test( data );
  }
  zAssert( zVec3DData (address list), result );
}

void assert_vec3ddata_array_ptr(void)
{

  int i;
  bool result;

  {
    zVec3DData data;
    zAssert( zVec3DDataAssignArray (null pointer), !data.assignArray( NULL ) );
    zVec3DArray array( N );
    for( i=0; i<N; i++ )
      array.get(i)->create( i, i, i );
    data.assignArray( &array );
    result = vec_data_test( data );
  }
  zAssert( zVec3DData (array pointer), result );
}

void assert_vec3ddata_list_ptr(void)
{
  zVec3D v;
  int i;
  bool result;

  {
    zVec3DData data;
    zVec3DList list;
    zAssert( zVec3DDataAssignList (null pointer), !data.assignList( NULL ) );
    list.init( );
    for( i=0; i<N; i++ ){
      v.create( i, i, i );
      zVec3DListAdd( &list, &v );
    }
    data.assignList( &list );
    result = vec_data_test( data );
    zVec3DListDestroy( &list );
  }
  zAssert( zVec3DData (list pointer), result );
}

void assert_vec3ddata_addrlist_ptr(void)
{
  int i;
  bool result;

  {
    zVec3DData data;
    zVec3DAddrList list;
    zAssert( zVec3DDataAssignAddrList (null pointer), !data.assignAddrList( NULL ) );
    list.init();
    zVec3DArray array( N );
    for( i=0; i<N; i++ ){
      array.get(i)->create( i, i, i );
      zVec3DAddrListAdd( &list, array.get(i) );
    }
    data.assignAddrList( &list );
    result = vec_data_test( data );
    zVec3DAddrListDestroy( &list );
  }
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
  zVec3D point, *v;
  bool result = true;
  int i;

  {
    zVec3DTree tree;
    zVec3DOctree octree;
    zVec3DData vicinity_tree, vicinity_octree;

    zVec3DTreeInit( &tree );
    zAABox3DCreate( &tree.data.region, -10, -10, -10, 10, 10, 10 );
    octree.init( -10, -10, -10, 10, 10, 10, 1 );
    for( i=0; i<NP; i++ ){
      point.create( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
      zVec3DTreeAddPoint( &tree, &point ); // @TODO memory leak! but how does the tree free?
      octree.addPoint( point );
    }
    point.create( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

    zVec3DTreeVicinity( &tree, &point, 5, &vicinity_tree );
    zVec3DTreeDestroy( &tree );

    octree.vicinity( point, 5, &vicinity_octree );

    if( vicinity_tree.size() != vicinity_octree.size() ){
      eprintf( "size of vicinity (by kd-tree) = %d\n", vicinity_tree.size() );
      eprintf( "size of vicinity (by kd-tree) = %d\n", vicinity_octree.size() );
      result = false;
    }
    // brute-force confirmation
    vicinity_tree.rewind();
    while( ( v = vicinity_tree.fetch() ) )
      if( !vicinity_octree.find( v ) ){
        eprintf( "point not found in the octree: " );
        zVec3DFPrint( stderr, v );
        result = false;
      }
    vicinity_octree.rewind();
    while( ( v = vicinity_octree.fetch() ) )
      if( !vicinity_tree.find( v ) ){
        eprintf( "point not found in the kd-tree: " );
        zVec3DFPrint( stderr, v );
        result = false;
      }
  }
  zAssert( zVec3DTreeVicinity + zVec3DOctreeVicinity, result );
}

void generate_frame(zFrame3D &frame)
{
  zFrame3DPos(&frame)->create( zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
  zFrame3DAtt(&frame)->createFromZYX( zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

void generate_points(zVec3DData &src, zVec3DData &dest, const zFrame3D &frame, int n, double tol)
{
  zVec3D po, p, noise;

  while( --n >= 0 ){
    po.create( zRandF(-5,5), zRandF(-5,5), zRandF(-5,5) );
    noise.create( zRandF(-tol,tol), zRandF(-tol,tol), zRandF(-tol,tol) );
    p.copy( frame.xform( po ) );
    p += noise;
    src.add( &po );
    dest.add( &p );
  }
}

bool check_frame_error(const zFrame3D &frame, const zFrame3D &frame_ident, double tol)
{
  zVec6D error( (frame - frame_ident) );
  return error.isTol( tol );
}

void assert_frame_ident(void)
{
  int testnum = 1000;
  double tol = 1.0e-4;
  bool result;

  {
    zVec3DData src, dest;
    zFrame3D frame, frame_ident;

    src.initList();
    dest.initList();
    generate_frame( frame );
    generate_points( src, dest, frame, testnum, tol );
    zVec3DDataIdentFrame( &src, &dest, &frame_ident );
    result = check_frame_error( frame, frame_ident, tol );
  }
  zAssert( zVec3DDataIdentFrame, result );
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
  assert_vicinity();
  assert_frame_ident();
  return 0;
}
