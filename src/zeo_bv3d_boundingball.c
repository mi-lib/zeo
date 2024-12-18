/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_boundingball - 3D bounding volume: bounding ball.
 */

#include <zeo/zeo_bv3d.h>

/* ********************************************************** */
/* bounding ball / smallest enclosing ball
 * ********************************************************** */

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
static int _zBoundingBall3DPrim(zSphere3D *bb, zVec3DAddrList *pl, zVec3D **vp)
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

/* a recursive procedure to find bounding ball of two sets of points,
 * where the latter is the set of those on the surface of the ball. */
static int _zBoundingBall3DInc(zSphere3D *bb, zVec3DAddrList *pl, zVec3DAddrList *shell, zVec3D **vp)
{
  zVec3DAddrListCell *cp;
  int num;

  if( zListIsEmpty(pl) || zListSize(shell) == 4 )
    return _zBoundingBall3DPrim( bb, shell, vp );
  zListDeleteTail( pl, &cp );
  num = _zBoundingBall3DInc( bb, pl, shell, vp );
  if( !zSphere3DPointIsInside( bb, cp->data, zTOL ) ){
    zListInsertTail( shell, cp );
    num = _zBoundingBall3DInc( bb, pl, shell, vp );
    zListPurge( shell, cp );
  }
  zListInsertTail( pl, cp );
  return num;
}

/* a recursive procedure to find bounding ball of a list of 3D points. */
static int _zBoundingBall3D(zSphere3D *bb, zVec3DAddrList *pl, zVec3D **vp)
{
  zVec3DAddrList shell;
  zVec3DAddrListCell *cp;
  int num;

  zListInit( &shell );
  if( zListSize(pl) <= 4 )
    return _zBoundingBall3DPrim( bb, pl, vp );
  zListDeleteTail( pl, &cp );
  num = _zBoundingBall3D( bb, pl, vp );
  if( !zSphere3DPointIsInside( bb, cp->data, zTOL ) ){
    zListInsertTail( &shell, cp );
    num = _zBoundingBall3DInc( bb, pl, &shell, vp );
    zListPurge( &shell, cp );
  }
  zListInsertTail( pl, cp );
  return num;
}

/* bounding ball of 3D points. */
int zVec3DDataBoundingBall(zVec3DData *data, zSphere3D *bb, zVec3D **vp)
{
  zVec3DAddrList addrlist;
  zPH3D ch;
  int ret;

  if( zVec3DDataSize(data) > ZEO_BOUNDINGBALL_PN_THRESHOLD ){
    /* discard points inside of the convex hull from the list. */
    if( !zVec3DDataConvexHull( data, &ch ) ){
      ret = 0;
    } else{
      zArrayFree( &ch.face );
      zVec3DAddrListCreate( &addrlist, &ch.vert );
      ret = _zBoundingBall3D( bb, &addrlist, vp );
      zVec3DAddrListDestroy( &addrlist );
    }
    zPH3DDestroy( &ch );
  } else{
    if( !zVec3DDataToAddrList( data, &addrlist ) ) return 0;
    ret = _zBoundingBall3D( bb, &addrlist, vp );
    zVec3DAddrListDestroy( &addrlist );
  }
  return ret;
}
