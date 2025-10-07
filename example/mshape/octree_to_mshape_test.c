#include <zeo/zeo_mshape3d.h>

int main(int argc, char *argv[])
{
  zShape3D shape;
  zVec3DOctree octree;
  zMShape3D ms;

  zShape3DConeCreate( &shape, ZVEC3DZERO, ZVEC3DZ, 0.5, 0 );
  zVec3DOctreeInit( &octree, -1.5, -1.5, -1.5, 1.5, 1.5, 1.5, 0.04 );
  zShape3DPutInOctree( &shape, &octree );
  zMShape3DFromOctree( &ms, &octree, NULL );
  zMShape3DWriteZTK( &ms, "octree_mshape.ztk" );
  zMShape3DDestroy( &ms );
  zVec3DOctreeDestroy( &octree );
  return EXIT_SUCCESS;
}
