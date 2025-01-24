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

/* principal component analysis on a set of 3D points. */
bool zVec3DDataPCA(zVec3DData *data, const zVec3D *center, double eval[3], zVec3D evec[3])
{
  zMat3D vm;
  zVec3D *v, p;
  double _eval[3];
  zVec3D _evec[3];

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return false;
  }
  if( !eval ) eval = _eval;
  if( !evec ) evec = _evec;
  zMat3DZero( &vm );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zVec3DSub( v, center, &p );
    zMat3DAddDyad( &vm, &p, &p );
  }
  zMat3DSymEig( &vm, eval, evec );
  return true;
}

/* barycenter of and principal component analysis on a set of 3D points. */
bool zVec3DDataBaryPCA(zVec3DData *data, zVec3D *center, double eval[3], zVec3D evec[3])
{
  zMat3D vm;
  zVec3D p, *v;
  double _eval[3];
  zVec3D _evec[3];

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return false;
  }
  if( !eval ) eval = _eval;
  if( !evec ) evec = _evec;
  zVec3DZero( &p );
  zMat3DZero( &vm );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zVec3DAddDRC( &p, v );
    zMat3DAddDyad( &vm, v, v );
  }
  zVec3DDiv( &p, zVec3DDataSize(data), center );
  zMat3DSubDyad( &vm, &p, center );
  zMat3DSymEig( &vm, eval, evec );
  return true;
}

/* find a coordinate frame that spans principal components of a set of 3D points from its barycenter. */
zFrame3D *zVec3DDataPCAFrame(zVec3DData *data, zFrame3D *frame)
{
  double eval[3];

  if( !zVec3DDataBaryPCA( data, zFrame3DPos(frame), eval, zFrame3DAtt(frame)->v ) ) return NULL;
  zVec3DOuterProd( zFrame3DVec(frame,zX), zFrame3DVec(frame,zY), zFrame3DVec(frame,zZ) );
  return frame;
}

/* mean normal of a set of 3D points. */
zVec3D *zVec3DDataMeanNormal(zVec3DData *data, const zVec3D *center, zVec3D *normal)
{
  double eval[3];
  zVec3D evec[3];
  int i;

  if( !zVec3DDataPCA( data, center, eval, evec ) ) return NULL;
  if( eval[0] < eval[1] )
    i = eval[0] < eval[2] ? 0 : 2;
  else
    i = eval[1] < eval[2] ? 1 : 2;
  return zVec3DCopy( &evec[i], normal );
}

/* mean plane of a set of 3D points. */
zPlane3D *zVec3DDataMeanPlane(zVec3DData *data, zVec3D *center, zPlane3D *plane)
{
  double eval[3];
  zVec3D evec[3];
  int i;

  if( !zVec3DDataBaryPCA( data, center, eval, evec ) ) return NULL;
  if( eval[0] < eval[1] )
    i = eval[0] < eval[2] ? 0 : 2;
  else
    i = eval[1] < eval[2] ? 1 : 2;
  return zPlane3DCreate( plane, center, &evec[i] );
}
