/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_profile - profiling of 3D pointcloud.
 */

#include <zeo/zeo_vec3d_profile.h>

/* normal vector cloud of a 3D point cloud that uses kd-tree with k=3. */
zVec3DData *zVec3DDataNormalVec_Tree(zVec3DData *pointdata, double radius, zVec3DData *normaldata)
{
  zVec3DTree tree;
  zAABox3D aabb;
  zVec3DData vicinity;
  zVec3D *v, normal, center, d;

  zVec3DDataInitList( normaldata );
  zVec3DDataAABB( pointdata, &aabb, NULL );
  zVec3DDataToTree( pointdata, &tree );
  zVec3DDataBarycenter( pointdata, &center );
  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    zVec3DDataInitAddrList( &vicinity );
    zVec3DTreeVicinity( &tree, v, radius, &vicinity );
    if( zVec3DDataSize(&vicinity) < ZEO_VEC3DDATA_NORMALVEC_NUM_MIN )
      ZRUNWARN( ZEO_WARN_POINTCLOUD_NORMAL_TOOFEWPOINTS );
    if( zVec3DDataSize(&vicinity) > ZEO_VEC3DDATA_NORMALVEC_NUM_MAX )
      ZRUNWARN( ZEO_WARN_POINTCLOUD_NORMAL_TOOMANYPOINTS );
    zVec3DDataMeanNormal( &vicinity, v, &normal );
    zVec3DSub( v, &center, &d );
    if( zVec3DInnerProd( &normal, &d ) < 0 )
      zVec3DRevDRC( &normal ); /* flip the normal vector to be outward (not a strict way) */
    zVec3DDataAdd( normaldata, &normal );
    zVec3DDataDestroy( &vicinity );
  }
  zVec3DTreeDestroy( &tree );
  return normaldata;
}

/* normal vector cloud of a 3D point cloud that uses 3D octree.  */
zVec3DData *zVec3DDataNormalVec_Octree(zVec3DData *pointdata, double radius, zVec3DData *normaldata)
{
  zVec3DOctree octree;
  zAABox3D aabb;
  zVec3DData vicinity;
  zVec3D *v, normal, center, d;

  zVec3DDataInitList( normaldata );
  zVec3DDataAABB( pointdata, &aabb, NULL );
  zVec3DOctreeInit( &octree, zAABox3DXMin(&aabb), zAABox3DYMin(&aabb), zAABox3DZMin(&aabb), zAABox3DXMax(&aabb), zAABox3DYMax(&aabb), zAABox3DZMax(&aabb), radius );
  zVec3DOctreeEmbedPoints( &octree, pointdata );
  zVec3DDataBarycenter( pointdata, &center );
  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    zVec3DDataInitAddrList( &vicinity );
    zVec3DOctreeVicinity( &octree, v, radius, &vicinity );
    if( zVec3DDataSize(&vicinity) < ZEO_VEC3DDATA_NORMALVEC_NUM_MIN )
      ZRUNWARN( ZEO_WARN_POINTCLOUD_NORMAL_TOOFEWPOINTS );
    if( zVec3DDataSize(&vicinity) > ZEO_VEC3DDATA_NORMALVEC_NUM_MAX )
      ZRUNWARN( ZEO_WARN_POINTCLOUD_NORMAL_TOOMANYPOINTS );
    zVec3DDataMeanNormal( &vicinity, v, &normal );
    zVec3DSub( v, &center, &d );
    if( zVec3DInnerProd( &normal, &d ) < 0 )
      zVec3DRevDRC( &normal ); /* flip the normal vector to be outward (not a strict way) */
    zVec3DDataAdd( normaldata, &normal );
    zVec3DDataDestroy( &vicinity );
  }
  zVec3DOctreeDestroy( &octree );
  return normaldata;
}

zVec3DData *(* zVec3DDataNormalVec)(zVec3DData*, double, zVec3DData*) = zVec3DDataNormalVec_Octree;
