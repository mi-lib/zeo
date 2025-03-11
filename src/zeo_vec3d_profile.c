/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_profile - profiling of 3D pointcloud.
 */

#include <zeo/zeo_vec3d_profile.h>

/* normal vector estimation */

/* estimate the normal vector at a 3D point from a set of vicinity points. */
static zVec3D *_zVec3DDataNormalVecFromVicinity(zVec3DData *vicinity, const zVec3D *point, const zVec3D *center, zVec3D *normal)
{
  zVec3D d;

  if( zVec3DDataSize(vicinity) < ZEO_VEC3DDATA_NORMALVEC_NUM_MIN )
    ZRUNWARN( ZEO_WARN_VEC3DDATA_NORMAL_TOOFEWPOINTS );
  if( zVec3DDataSize(vicinity) > ZEO_VEC3DDATA_NORMALVEC_NUM_MAX )
    ZRUNWARN( ZEO_WARN_VEC3DDATA_NORMAL_TOOMANYPOINTS );
  zVec3DDataMeanNormal( vicinity, point, normal );
  zVec3DSub( point, center, &d );
  if( zVec3DInnerProd( normal, &d ) < 0 )
    zVec3DRevDRC( normal ); /* flip the normal vector to be outward (not a strict way) */
  return normal;
}

/* normal vector cloud of a 3D point cloud that uses kd-tree with k=3. */
zVec3DData *zVec3DDataNormalVec_Tree(zVec3DData *pointdata, double radius, zVec3DData *normaldata)
{
  zVec3DTree tree;
  zAABox3D aabb;
  zVec3DData vicinity, *retval = NULL;
  zVec3D *v, normal, center;

  zVec3DDataAABB( pointdata, &aabb, NULL );
  if( !zVec3DDataToTree( pointdata, &tree ) ) goto TERMINATE;
  zVec3DDataBarycenter( pointdata, &center );
  zVec3DDataInitList( normaldata );
  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    zVec3DDataInitAddrList( &vicinity );
    if( !zVec3DTreeVicinity( &tree, v, radius, &vicinity ) ) goto TERMINATE;
    _zVec3DDataNormalVecFromVicinity( &vicinity, v, &center, &normal );
    if( !zVec3DDataAdd( normaldata, &normal ) ) goto TERMINATE;
    zVec3DDataDestroy( &vicinity );
  }
  retval = normaldata;
 TERMINATE:
  zVec3DTreeDestroy( &tree );
  return retval;
}

/* normal vector cloud of a 3D point cloud that uses 3D octree.  */
zVec3DData *zVec3DDataNormalVec_Octree(zVec3DData *pointdata, double radius, zVec3DData *normaldata)
{
  zVec3DOctree octree;
  zAABox3D aabb;
  zVec3DData vicinity, *retval = NULL;
  zVec3D *v, normal, center;

  zVec3DDataAABB( pointdata, &aabb, NULL );
  if( !zVec3DDataToOctree( pointdata, &octree, zAABox3DXMin(&aabb), zAABox3DYMin(&aabb), zAABox3DZMin(&aabb), zAABox3DXMax(&aabb), zAABox3DYMax(&aabb), zAABox3DZMax(&aabb), radius ) )
    goto TERMINATE;
  zVec3DDataBarycenter( pointdata, &center );
  zVec3DDataInitList( normaldata );
  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    zVec3DDataInitAddrList( &vicinity );
    if( !zVec3DOctreeVicinity( &octree, v, radius, &vicinity ) ) goto TERMINATE;
    _zVec3DDataNormalVecFromVicinity( &vicinity, v, &center, &normal );
    if( !zVec3DDataAdd( normaldata, &normal ) ) goto TERMINATE;
    zVec3DDataDestroy( &vicinity );
  }
  retval = normaldata;
 TERMINATE:
  zVec3DOctreeDestroy( &octree );
  return retval;
}

zVec3DData *(* zVec3DDataNormalVec)(zVec3DData*, double, zVec3DData*) = zVec3DDataNormalVec_Octree;

/* iterative closest point method */

#define ZEO_VEC3DDATA_IDENT_FRAME_MINSIZ 3

/* identify a transformation frame from two sets of 3D points. */
zFrame3D *zVec3DDataIdentFrame(zVec3DData *src, zVec3DData *dest, zFrame3D *frame)
{
  zVec3D *po, *p;
  zVec3D pgo, pg, dpo, dp, d;
  zMat3D cov, u, v;
  double so = 0;

  if( zVec3DDataSize(src) != zVec3DDataSize(dest) ){
    ZRUNERROR( ZEO_ERR_VEC3DDATA_SIZEMISMATCH, zVec3DDataSize(src), zVec3DDataSize(dest) );
    return NULL;
  }
  if( zVec3DDataSize(src)  < ZEO_VEC3DDATA_IDENT_FRAME_MINSIZ ){
    ZRUNERROR( ZEO_ERR_VEC3DDATA_IDENTFRAME_TOOFEWPOINTS, zVec3DDataSize(src) );
    return NULL;
  }
  zVec3DDataBarycenter( src, &pgo );
  zVec3DDataBarycenter( dest, &pg );
  zVec3DDataRewind( src );
  zVec3DDataRewind( dest );
  zMat3DZero( &cov );
  while( ( po = zVec3DDataFetch( src ) ) && ( p = zVec3DDataFetch( dest ) ) ){
    zVec3DSub( po, &pgo, &dpo );
    zVec3DSub( p, &pg, &dp );
    so += zVec3DSqrNorm( &dpo );
    zMat3DAddDyad( &cov, &dpo, &dp );
  }
  so /= zVec3DDataSize(src);
  zMat3DDivDRC( &cov, zVec3DDataSize(src) );
  zMat3DSVD( &cov, &u, &d, &v );
  zMulMat3DMat3DT( &v, &u, zFrame3DAtt(frame) );
  zMulMat3DVec3D( zFrame3DAtt(frame), &pgo, &dp );
  zVec3DMulDRC( &dp, ( d.c.x + d.c.y + d.c.z ) / so );
  zVec3DSub( &pg, &dp, zFrame3DPos(frame) );
  return frame;
}

/* initialize transformation frame based on barycenters of two point clouds. */
static zFrame3D *_zVec3DDataICPInit(zVec3DData *src, zVec3DData *dest, zFrame3D *frame)
{
  zVec3D center_src, center_dest;

  zVec3DDataBarycenter( src, &center_src );
  zVec3DDataBarycenter( dest, &center_dest );
  zVec3DSub( &center_dest, &center_src, zFrame3DPos(frame) );
  zMat3DIdent( zFrame3DAtt(frame) );
  return frame;
}

/* iterative closest point method */
zFrame3D *zVec3DDataICP(zVec3DData *src, zVec3DData *dest, zFrame3D *frame, double sample_rate, double tol)
{
  zVec3DTree dest_tree, *nn;
  zVec3DData sample_src, sample_dest;
  zVec3D *po, p;
  int i, iter = ZEO_VEC3DDATA_ICP_MAXITERNUM;
  double error_max, error;

  zVec3DTreeInit( &dest_tree );
  if( !zVec3DTreeAddData( &dest_tree, dest ) ){
    zVec3DTreeDestroy( &dest_tree );
    return NULL;
  }
  _zVec3DDataICPInit( src, dest, frame );
  for( i=0; i<iter; i++ ){
    zVec3DDataInitAddrList( &sample_src );
    zVec3DDataInitAddrList( &sample_dest );
    zVec3DDataRewind( src );
    error_max = 0;
    while( ( po = zVec3DDataFetch( src ) ) ){
      if( zRandF(0,1) > sample_rate ) continue;
      zXform3D( frame, po, &p );
      if( ( error = zVec3DTreeNN( &dest_tree, &p, &nn ) ) > error_max ) error_max = error;
      zVec3DDataAdd( &sample_src, po );
      zVec3DDataAdd( &sample_dest, &nn->data.point );
    }
    if( error_max < tol ) break;
    zVec3DDataIdentFrame( &sample_src, &sample_dest, frame );
    zVec3DDataDestroy( &sample_src );
    zVec3DDataDestroy( &sample_dest );
  }
  if( i == iter )
    ZITERWARN( iter );
  zVec3DTreeDestroy( &dest_tree );
  return frame;
}
