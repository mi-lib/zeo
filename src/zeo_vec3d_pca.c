/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_pca - principal component analysis for 3D vectors.
 */

#include <zeo/zeo_mat3d.h>

/* barycenter of a point cloud. */
zVec3D *zVec3DDataBarycenter(zVec3DData *data, zVec3D *c)
{
  zVec3D *v;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zVec3DZero( c );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) )
    zVec3DAddDRC( c, v );
  return zVec3DDivDRC( c, zVec3DDataSize(data) );
}

/* PCA to vector cloud. */
zVec3D *zVec3DDataPCA(zVec3DData *data, zVec3D evec[])
{
  zMat3D vm;
  zVec3D *v;
  double eval[3];

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zMat3DZero( &vm );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) )
    zMat3DAddDyad( &vm, v, v );
  zMat3DSymEig( &vm, eval, evec );
  return evec;
}

/* barycenter of and PCA to vector cloud. */
zVec3D *zVec3DDataBaryPCA(zVec3DData *data, zVec3D *c, zVec3D evec[])
{
  zMat3D vm;
  zVec3D dp, *v;
  double eval[3];

  if( !zVec3DDataBarycenter( data, c ) ) return NULL;
  zMat3DZero( &vm );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zVec3DSub( v, c, &dp );
    zMat3DAddDyad( &vm, &dp, &dp );
  }
  zMat3DSymEig( &vm, eval, evec );
  return c;
}
