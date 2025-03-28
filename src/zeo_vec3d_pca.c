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
  return true;
}

/* add a new 3D point to a set of 3D points, and update its covariance matrix. */
bool zVec3DDataAddAndUpdateCov(zVec3DData *data, const zVec3D *v, const zVec3D *center, zMat3D *cov)
{
  zVec3D p;

  if( !zVec3DDataAdd( data, v ) ) return false;
  zVec3DSub( v, center, &p );
  zMat3DAddDyad( cov, &p, &p );
  return true;
}

/* add a new 3D point to a set of 3D points, and update its barycenter and covariance matrix. */
bool zVec3DDataAddAndUpdateBaryCov(zVec3DData *data, const zVec3D *v, zVec3D *center, zMat3D *cov)
{
  zVec3D p;
  int n;

  n = zVec3DDataSize( data );
  if( !zVec3DDataAdd( data, v ) ) return false;
  zVec3DMul( center, n, &p );
  zMat3DAddDyad( cov, &p, center );
  zMat3DAddDyad( cov, v, v );
  zVec3DAddDRC( &p, v );
  zVec3DDiv( &p, n+1, center );
  zMat3DSubDyad( cov, &p, center );
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

#define _zMin3ID( val )  ( ( val[0] < val[1] ) ? ( val[0] < val[2] ? 0 : 2 ) : ( val[1] < val[2] ? 1 : 2 ) )

/* mean normal of a set of 3D points. */
zVec3D *zVec3DDataMeanNormal(zVec3DData *data, const zVec3D *center, zVec3D *normal)
{
  double pc[3];
  zVec3D pa[3];
  int i;

  if( !zVec3DDataPCA( data, center, pc, pa ) ) return NULL;
  i = _zMin3ID( pc );
  return zVec3DCopy( &pa[i], normal );
}

/* mean plane of a set of 3D points. */
zPlane3D *zVec3DDataMeanPlane(zVec3DData *data, zVec3D *center, zPlane3D *plane)
{
  double pc[3];
  zVec3D pa[3];
  int i;

  if( !zVec3DDataBaryPCA( data, center, pc, pa ) ) return NULL;
  i = _zMin3ID( pc );
  return zPlane3DCreate( plane, center, &pa[i] );
}
