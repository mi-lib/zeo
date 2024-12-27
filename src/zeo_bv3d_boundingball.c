/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_boundingball - 3D bounding volume: bounding ball.
 */

#include <zeo/zeo_bv3d.h>

/* test if a ball with its poles at the first two points bounds the last two points. */
static zSphere3D *_zBoundingBall3DTest2(zSphere3D *bb, zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v4, zVec3D **vp)
{
  zSphere3DFrom2( bb, v1, v2 );
  if( !zSphere3DPointIsInside(bb,v3,zTOL) || !zSphere3DPointIsInside(bb,v4,zTOL) ) return NULL;
  if( vp ){
    vp[0] = v1; vp[1] = v2;
  }
  return bb;
}

/* test if a circum ball of the first three points bounds the last point. */
static zSphere3D *_zBoundingBall3DTest3(zSphere3D *bb, zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v4, zVec3D **vp)
{
  zSphere3DFrom3( bb, v1, v2, v3 );
  if( !zSphere3DPointIsInside(bb,v4,zTOL) ) return NULL;
  if( vp ){
    vp[0] = v1; vp[1] = v2; vp[2] = v3;
  }
  return bb;
}

/* a circum ball of the four points. */
static zSphere3D *_zBoundingBall3DTest4(zSphere3D *bb, zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v4, zVec3D **vp)
{
  if( vp ){
    vp[0] = v1; vp[1] = v2; vp[2] = v3; vp[3] = v4;
  }
  return zSphere3DFrom4( bb, v1, v2, v3, v4 );
}

/* bounding ball of the four points. */
static int _zBoundingBall3D4(zSphere3D *bb, zVec3D *v[], zVec3D **vp)
{
  if( _zBoundingBall3DTest2( bb, v[0], v[1], v[2], v[3], vp ) ||
      _zBoundingBall3DTest2( bb, v[0], v[2], v[1], v[3], vp ) ||
      _zBoundingBall3DTest2( bb, v[0], v[3], v[1], v[2], vp ) ||
      _zBoundingBall3DTest2( bb, v[1], v[2], v[0], v[3], vp ) ||
      _zBoundingBall3DTest2( bb, v[1], v[3], v[0], v[2], vp ) ||
      _zBoundingBall3DTest2( bb, v[2], v[3], v[0], v[1], vp ) ) return 2;
  if( _zBoundingBall3DTest3( bb, v[0], v[1], v[2], v[3], vp ) ||
      _zBoundingBall3DTest3( bb, v[0], v[1], v[3], v[2], vp ) ||
      _zBoundingBall3DTest3( bb, v[1], v[2], v[3], v[0], vp ) ) return 3;
  _zBoundingBall3DTest4( bb, v[0], v[1], v[2], v[3], vp );
  return 4;
}

/* bounding ball of up to four points. */
static int _zBoundingBall3DDirect(zSphere3D *bb, zVec3DAddrList *pl, zVec3D **vp)
{
  zVec3D *v[4];

  switch( zListSize(pl) ){
  case 0:
    zSphere3DCreate( bb, ZVEC3DZERO, -1, 0 ); /* vague */
    return 0;
  case 1:
    zSphere3DCreate( bb, zListHead(pl)->data, 0, 0 );
    if( vp ) vp[0] = zListHead(pl)->data;
    return 1;
  case 2:
    zSphere3DFrom2( bb, zListHead(pl)->data, zListTail(pl)->data );
    if( vp ){
      vp[0] = zListHead(pl)->data;
      vp[1] = zListTail(pl)->data;
    }
    return 2;
  case 3:
  case 4:
    v[0] = zListTail(pl)->data;
    v[1] = zListCellNext(zListTail(pl))->data;
    v[2] = zListCellNext(zListCellNext(zListTail(pl)))->data;
    v[3] = zListHead(pl)->data;
    return _zBoundingBall3D4( bb, v, vp );
  default:
    ZRUNERROR( ZEO_ERR_FATAL );
  }
  return 0;
}

/* a recursive implementation */
ZEO_BOUNDINGBALL_RECURSIVE_DEF_METHOD( 3D, 4, zSphere3D )

/* a non-recursive implementation */
ZEO_BOUNDINGBALL_DEF_METHOD( 3D, 4, zSphere3D )
