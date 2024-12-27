/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_boundingball - 2D bounding volume: bounding ball.
 */

#include <zeo/zeo_bv2d.h>

/* test if a disk with its poles at the first two points bounds the last two points. */
static zDisk2D *_zBoundingBall2DTest2(zDisk2D *bb, zVec2D *v1, zVec2D *v2, zVec2D *v3, zVec2D **vp)
{
  zDisk2DFrom2( bb, v1, v2 );
  if( !zDisk2DPointIsInside( bb, v3, zTOL ) ) return NULL;
  if( vp ){
    vp[0] = v1; vp[1] = v2;
  }
  return bb;
}

/* a circum disk of the three points. */
static zDisk2D *_zBoundingBall2DTest3(zDisk2D *bb, zVec2D *v1, zVec2D *v2, zVec2D *v3, zVec2D **vp)
{
  zDisk2DFrom3( bb, v1, v2, v3 );
  if( vp ){
    vp[0] = v1; vp[1] = v2; vp[2] = v3;
  }
  return bb;
}

/* bounding ball of the three points. */
static int _zBoundingBall2D3(zDisk2D *bb, zVec2D *v[], zVec2D **vp)
{
  if( _zBoundingBall2DTest2( bb, v[0], v[1], v[2], vp ) ||
      _zBoundingBall2DTest2( bb, v[1], v[2], v[0], vp ) ||
      _zBoundingBall2DTest2( bb, v[2], v[0], v[1], vp ) ) return 2;
  _zBoundingBall2DTest3( bb, v[0], v[1], v[2], vp );
  return 3;
}

/* bounding ball of up to three points. */
static int _zBoundingBall2DDirect(zDisk2D *bb, zVec2DAddrList *pl, zVec2D **vp)
{
  zVec2D *v[3];

  switch( zListSize(pl) ){
  case 0:
    zDisk2DCreate( bb, ZVEC2DZERO, -1 ); /* vague */
    return 0;
  case 1:
    zDisk2DCreate( bb, zListHead(pl)->data, 0 );
    if( vp ) vp[0] = zListHead(pl)->data;
    return 1;
  case 2:
    zDisk2DFrom2( bb, zListHead(pl)->data, zListTail(pl)->data );
    if( vp ){
      vp[0] = zListHead(pl)->data;
      vp[1] = zListTail(pl)->data;
    }
    return 2;
  case 3:
    v[0] = zListTail(pl)->data;
    v[1] = zListCellNext(zListTail(pl))->data;
    v[2] = zListHead(pl)->data;
    return _zBoundingBall2D3( bb, v, vp );
  default:
    ZRUNERROR( ZEO_ERR_FATAL );
  }
  return 0;
}

/* a recursive implementation */
ZEO_BOUNDINGBALL_RECURSIVE_DEF_METHOD( 2D, 3, zDisk2D )

/* a non-recursive implementation */
ZEO_BOUNDINGBALL_DEF_METHOD( 2D, 3, zDisk2D )
