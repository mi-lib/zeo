/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_aabb - 3D bounding volume: axis-aligned bounding box.
 */

#include <zeo/zeo_bv3d.h>

/* enlarge bounding box if a 3D point is out of the current box. */
static int _zAABB3DTest(zAABox3D *bb, zVec3D *p, zAxis axis)
{
  if( p->e[(int)axis] > bb->max.e[(int)axis] ){
    bb->max.e[(int)axis] = p->e[(int)axis];
    return 0;
  }
  if( p->e[(int)axis] < bb->min.e[(int)axis] ){
    bb->min.e[(int)axis] = p->e[(int)axis];
    return 3;
  }
  return -1;
}

/* enlarge bounding box if a 3D point is outside of the box. */
static zAABox3D *_zAABB3DInc(zAABox3D *bb, zVec3D *p, zVec3D **vp)
{
  int s;

  if( ( s = _zAABB3DTest( bb, p, zX ) ) != -1 && vp ) vp[zX+s] = p;
  if( ( s = _zAABB3DTest( bb, p, zY ) ) != -1 && vp ) vp[zY+s] = p;
  if( ( s = _zAABB3DTest( bb, p, zZ ) ) != -1 && vp ) vp[zZ+s] = p;
  return bb;
}

/* bounding box of 3D points. */
zAABox3D *zVec3DDataAABB(zVec3DData *data, zAABox3D *bb, zVec3D **vp)
{
  zVec3D *v;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zAABox3DInit( bb );
  zVec3DDataRewind( data );
  v = zVec3DDataFetch( data );
  zVec3DCopy( v, &bb->min );
  zVec3DCopy( v, &bb->max );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = vp[4] = vp[5] = v;
  while( ( v = zVec3DDataFetch( data ) ) ){
    _zAABB3DInc( bb, v, vp );
  }
  return bb;
}

/* bounding box of 3D points in a specified frame. */
zAABox3D *zVec3DDataAABBXform(zVec3DData *data, zAABox3D *bb, zFrame3D *frame)
{
  zVec3D *v, px;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zAABox3DInit( bb );
  zVec3DDataRewind( data );
  v = zVec3DDataFetch( data );
  zXform3D( frame, v, &px );
  zVec3DCopy( &px, &bb->min );
  zVec3DCopy( &px, &bb->max );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zXform3D( frame, v, &px );
    _zAABB3DInc( bb, &px, NULL );
  }
  return bb;
}
