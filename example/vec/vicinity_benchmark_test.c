#include <zeo/zeo_vec3d_data.h>
#include <time.h>

#define TEST 1

#define NS       10000
#define  N         100
#define RESOLUTION   0.5

void try_one(int num_points)
{
  zVec3DOctree octree;
  zVec3DTree tree;
  zVec3DData pointdata;
  zVec3DData vicinity1, vicinity2, vicinity3;
  int i;
  zVec3D v;
  const double xmin = -10, xmax = 10, ymin = -10, ymax = 10, zmin = -10, zmax = 10;
  const double radius = 1.0;
  clock_t c1, c2;

  zVec3DDataInitArray( &pointdata, num_points );
  for( i=0; i<zVec3DDataSize(&pointdata); i++ ){
#if TEST == 0
    zVec3DCreate( &v, zRandF(xmin,xmax), zRandF(ymin,ymax), zRandF(zmin,zmax) );
#else
    zVec3DCreatePolar( &v, 4, zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
#endif
    zVec3DDataAdd( &pointdata, &v );
  }
  zVec3DOctreeInit( &octree, xmin, ymin , zmin, xmax, ymax, zmax, RESOLUTION );
  zVec3DOctreeAddData( &octree, &pointdata );
  zVec3DTreeInit( &tree );
  zVec3DTreeAddData( &tree, &pointdata );

  zVec3DCreate( &v, zRandF(xmin,xmax), zRandF(ymin,ymax), zRandF(zmin,zmax) );

  c1 = clock();
  zVec3DTreeVicinity( &tree, &v, radius, &vicinity1 );
  c2 = clock();
  printf( "%ld", (long)( c2 - c1 ) );
  c1 = clock();
  zVec3DOctreeVicinity( &octree, &v, radius, &vicinity2 );
  c2 = clock();
  printf( " %ld", (long)( c2 - c1 ) );
  c1 = clock();
  zVec3DDataVicinity( &pointdata, &v, radius, &vicinity3 );
  c2 = clock();
  printf( " %ld\n", (long)( c2 - c1 ) );

  zVec3DDataDestroy( &vicinity1 );
  zVec3DDataDestroy( &vicinity2 );
  zVec3DDataDestroy( &vicinity3 );
  zVec3DTreeDestroy( &tree );
  zVec3DOctreeDestroy( &octree );
  zVec3DDataDestroy( &pointdata );
}

int main(int argc, char *argv[])
{
  int i;

  zRandInit();
  for( i=0; i<N; i++ )
    try_one( argc > 1 ? atoi( argv[1] ) : NS );
  return 0;
}
