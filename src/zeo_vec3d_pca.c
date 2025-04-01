/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_pca - principal component analysis on 3D points.
 */

#include <zeo/zeo_vec3d_data.h>

/* barycenter of a set of 3D points. */
zVec3D *zVec3DDataBarycenter(zVec3DData *data, zVec3D *center)
{
  zVec3D *v;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zVec3DZero( center );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) )
    zVec3DAddDRC( center, v );
  return zVec3DDivDRC( center, zVec3DDataSize(data) );
}

/* covariance matrix of a set of 3D points . */
bool zVec3DDataCov(zVec3DData *data, const zVec3D *center, zMat3D *cov)
{
  zVec3D *v, p;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return false;
  }
  zMat3DZero( cov );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zVec3DSub( v, center, &p );
    zMat3DAddDyad( cov, &p, &p );
  }
  zMat3DDivDRC( cov, zVec3DDataSize(data) );
  return true;
}

/* barycenter and covariance matrix of a set of 3D points. */
bool zVec3DDataBaryCov(zVec3DData *data, zVec3D *center, zMat3D *cov)
{
  zVec3D p, *v;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return false;
  }
  zMat3DZero( cov );
  zVec3DZero( &p );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zVec3DAddDRC( &p, v );
    zMat3DAddDyad( cov, v, v );
  }
  zVec3DDiv( &p, zVec3DDataSize(data), center );
  zMat3DSubDyad( cov, &p, center );
  zMat3DDivDRC( cov, zVec3DDataSize(data) );
  return true;
}

/* principal component analysis on a set of 3D points. */
bool zVec3DDataPCA(zVec3DData *data, const zVec3D *center, double pc[3], zVec3D pa[3])
{
  zMat3D vm;
  double _pc[3];
  zVec3D _pa[3];

  if( !zVec3DDataCov( data, center, &vm ) ) return false;
  if( !pc ) pc = _pc;
  if( !pa ) pa = _pa;
  zMat3DSymEig( &vm, (zVec3D *)pc, (zMat3D *)pa );
  return true;
}

/* barycenter and principal components of a set of 3D points. */
bool zVec3DDataBaryPCA(zVec3DData *data, zVec3D *center, double pc[3], zVec3D pa[3])
{
  zMat3D vm;
  double _pc[3];
  zVec3D _pa[3];

  if( !zVec3DDataBaryCov( data, center, &vm ) ) return false;
  if( !pc ) pc = _pc;
  if( !pa ) pa = _pa;
  zMat3DSymEig( &vm, (zVec3D *)pc, (zMat3D *)pa );
  return true;
}

/* find a coordinate frame that spans principal components of a set of 3D points from its barycenter. */
zFrame3D *zVec3DDataPCAFrame(zVec3DData *data, zFrame3D *frame)
{
  if( !zVec3DDataBaryPCA( data, zFrame3DPos(frame), NULL, zFrame3DAtt(frame)->v ) ) return NULL;
  zVec3DOuterProd( zFrame3DVec(frame,zX), zFrame3DVec(frame,zY), zFrame3DVec(frame,zZ) );
  return frame;
}

/* mean normal of a set of 3D points. */
zVec3D *zVec3DDataMeanNormal(zVec3DData *data, const zVec3D *center, zVec3D *normal)
{
  double pc[3];
  zVec3D pa[3];

  if( !zVec3DDataPCA( data, center, pc, pa ) ) return NULL;
  return zVec3DCopy( &pa[_zMat3DEigMinID( pc )], normal );
}

/* mean plane of a set of 3D points. */
zPlane3D *zVec3DDataMeanPlane(zVec3DData *data, zVec3D *center, zPlane3D *plane)
{
  double pc[3];
  zVec3D pa[3];

  if( !zVec3DDataBaryPCA( data, center, pc, pa ) ) return NULL;
  return zPlane3DCreate( plane, center, &pa[_zMat3DEigMinID( pc )] );
}
