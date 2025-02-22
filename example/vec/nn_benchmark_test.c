#include <zeo/zeo_vec3d_data.h>
#include <time.h>

#define N          100
#define NS      100000
#define RESOLUTION 0.5

void try_one(void)
{
  zVec3DOctree octree;
  zVec3DTree tree, *node_nn;
  zVec3DData pointdata;
  int i;
  zVec3D v, *nn_octree, *nn_data;
  clock_t c1, c2;
  double t1, t2, t3;

  zVec3DDataInitArray( &pointdata, NS );
  for( i=0; i<zVec3DDataSize(&pointdata); i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DDataAdd( &pointdata, &v );
  }
  zVec3DOctreeInit( &octree, -10, -10, -10, 10, 10, 10, RESOLUTION );
  zVec3DOctreeAddData( &octree, &pointdata );
  zVec3DTreeInit( &tree );
  zVec3DTreeAddData( &tree, &pointdata );
  zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

  c1 = clock();
  zVec3DTreeNN( &tree, &v, &node_nn );
  c2 = clock();
  t1 = (double)( c2 - c1 ) / CLOCKS_PER_SEC;
  c1 = clock();
  zVec3DOctreeNN( &octree, &v, &nn_octree );
  c2 = clock();
  t2 = (double)( c2 - c1 ) / CLOCKS_PER_SEC;
  c1 = clock();
  zVec3DDataNN( &pointdata, &v, &nn_data );
  c2 = clock();
  t3 = (double)( c2 - c1 ) / CLOCKS_PER_SEC;
  printf( "%.10g %.10g %.10g\n", t1, t2, t3 );

  zVec3DTreeDestroy( &tree );
  zVec3DOctreeDestroy( &octree );
  zVec3DDataDestroy( &pointdata );
}

int main(int argc, char *argv[])
{
  int i;

  zRandInit();
  for( i=0; i<N; i++ ) try_one();
  return 0;
}
