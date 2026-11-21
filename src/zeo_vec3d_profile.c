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
  zVec3DData vicinity, *retval = NULL;
  zVec3D *v, normal, center;

  if( !zVec3DDataToTree( pointdata, &tree ) ) goto TERMINATE;
  zVec3DDataBarycenter( pointdata, &center );
  zVec3DDataInitList( normaldata );
  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) ){
    zVec3DDataInitAddrList( &vicinity );
    if( !zVec3DTreeVicinity( &tree, v, radius, &vicinity ) ||
        !zVec3DDataAdd( normaldata, _zVec3DDataNormalVecFromVicinity( &vicinity, v, &center, &normal ) ) ){
      zVec3DDataDestroy( &vicinity );
      goto TERMINATE;
    }
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
    if( !zVec3DOctreeVicinity( &octree, v, radius, &vicinity ) ||
        !zVec3DDataAdd( normaldata, _zVec3DDataNormalVecFromVicinity( &vicinity, v, &center, &normal ) ) ){
      zVec3DDataDestroy( &vicinity );
      goto TERMINATE;
    }
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
  double ss_src = 0, ss_dest = 0, tol;
  int imin;

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
    zMat3DAddDyad( &cov, &dpo, &dp );
    ss_src += zVec3DSqrNorm( po );
    ss_dest += zVec3DSqrNorm( p );
  }
  zMat3DSVD( &cov, &u, &d, &v );
  /* the rotation about a line is not determined if the points are collinear or coincident, namely,
   * if less than two singular values are larger than rounding errors relative to the coordinates. */
  tol = zTOL * sqrt( ss_src * ss_dest );
  if( ( d.c.x > tol ) + ( d.c.y > tol ) + ( d.c.z > tol ) < 2 ){
    ZRUNERROR( ZEO_ERR_VEC3DDATA_IDENTFRAME_DEGENERATE );
    return NULL;
  }
  /* avoid a reflection by flipping the singular vector of the smallest singular value. */
  if( zMat3DDet( &u ) * zMat3DDet( &v ) < 0 ){
    imin = d.c.x < d.c.y ? ( d.c.x < d.c.z ? 0 : 2 ) : ( d.c.y < d.c.z ? 1 : 2 );
    zVec3DRevDRC( zMat3DVec(&v,imin) );
  }
  zMulMat3DMat3DT( &v, &u, zFrame3DAtt(frame) );
  /* the frame is rigid, so that the barycenter of src is transformed to that of dest without scaling. */
  zMulMat3DVec3D( zFrame3DAtt(frame), &pgo, &dp );
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
  zFrame3D *ret = frame;
  bool converged;

  if( !zVec3DDataToTree( dest, &dest_tree ) ){
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
    converged = false;
    /* an iteration with too few samples is skipped, and the points are re-sampled. */
    if( zVec3DDataSize(&sample_src) >= ZEO_VEC3DDATA_IDENT_FRAME_MINSIZ ){
      if( !( converged = error_max < tol ) &&
          !zVec3DDataIdentFrame( &sample_src, &sample_dest, frame ) ) ret = NULL;
    }
    zVec3DDataDestroy( &sample_src );
    zVec3DDataDestroy( &sample_dest );
    if( converged || !ret ) break;
  }
  if( i == iter )
    ZITERWARN( iter );
  zVec3DTreeDestroy( &dest_tree );
  return ret;
}
