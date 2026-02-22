#include <zeo/zeo_multishape3d.h>

int main(int argc, char *argv[])
{
  zShape3D shape;
  zVec3DOctree octree;
  zMultiShape3D ms;

  zShape3DConeCreate( &shape, ZVEC3DZERO, ZVEC3DZ, 0.5, 0 );
  zVec3DOctreeInit( &octree, -1.5, -1.5, -1.5, 1.5, 1.5, 1.5, 0.04 );
  zShape3DPutInOctree( &shape, &octree );
  zMultiShape3DFromOctree( &ms, &octree, NULL );
  zMultiShape3DWriteZTK( &ms, "octree_mshape.ztk" );
  zMultiShape3DDestroy( &ms );
  zVec3DOctreeDestroy( &octree );
  return EXIT_SUCCESS;
}
