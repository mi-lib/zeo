/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_minkowski - collision checking: Minkowski sum
 */

#include <zeo/zeo_col.h>

/* Minkowski subtract. */
zPH3D *zVec3DDataMinkowskiSub(zVec3DData *data1, zVec3DData *data2, zPH3D *ph)
{
  zVec3DData data;
  zVec3D *v1, *v2, v;

  if( !zVec3DDataInitArray( &data, zVec3DDataSize(data1) * zVec3DDataSize(data2) ) )
    return NULL;
  zVec3DDataRewind( data1 );
  zVec3DDataRewind( data2 );
  while( ( v1 = zVec3DDataFetch( data1 ) ) ){
    while( ( v2 = zVec3DDataFetch( data2 ) ) ){
      zVec3DSub( v1, v2, &v );
      zVec3DDataAdd( &data, &v );
    }
  }
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
  return ph;
}
