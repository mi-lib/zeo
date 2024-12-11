#include <zeo/zeo.h>

#define NS 1000
#define N   100

bool assert_nn_one(void)
{
  zVec3DData data; /* for comparison */
  zVec3DTree tree, *node;
  zVec3D v, *nn;
  int i;
  bool ret = true;

  zVec3DDataInitList( &data );
  zVec3DTreeInit( &tree );
  zVec3DCreate( &tree.data.vmin,-10,-10,-10 );
  zVec3DCreate( &tree.data.vmax, 10, 10, 10 );

  for( i=0; i<NS; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DDataAdd( &data, &v );
    zVec3DTreeAdd( &tree, &v );
  }
  zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

  zVec3DTreeNN( &tree, &v, &node );
  zVec3DDataNN( &data, &v, &nn );  /* for comparison */
  if( !zVec3DEqual( &node->data.v, nn ) ) ret = false;
  zVec3DDataDestroy( &data );
  zVec3DTreeDestroy( &tree );
  return ret;
}

void assert_nn(void)
{
  int i;
  bool ret = true;

  for( i=0; i<N; i++ )
    if( !assert_nn_one() ) ret = false;
  zAssert( zVec3DTreeNN, ret );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_nn();
  return 0;
}
