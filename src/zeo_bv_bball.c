/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv_bball - bounding volume: bounding ball.
 */

#include <zeo/zeo_bv.h>

/* ********************************************************** */
/* bounding ball / smallest enclosing ball
 * ********************************************************** */

/* test if a ball with its poles at the first two points bounds the last two points. */
static zSphere3D *_zBBallTest2(zSphere3D *bb, zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v4, zVec3D **vp)
{
  zVec3D c;
  double r;

  zVec3DMid( v1, v2, &c );
  r = zVec3DDist( &c, v1 ) + zTOL;
  if( zVec3DDist(&c,v3) > r || zVec3DDist(&c,v4) > r ) return NULL;
  if( vp ){
    vp[0] = v1; vp[1] = v2;
  }
  return zSphere3DCreate( bb, &c, r, 0 );
}

/* test if a circum ball of the first three points bounds the last point. */
static zSphere3D *_zBBallTest3(zSphere3D *bb, zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v4, zVec3D **vp)
{
  zVec3D c;
  zTri3D t;
  double r;

  zTri3DCreate( &t, v1, v2, v3 );
  zTri3DCircumcenter( &t, &c );
  r = zVec3DDist( &c, v3 ) + zTOL;
  if( zVec3DDist(&c,v4) > r ) return NULL;
  if( vp ){
    vp[0] = v1; vp[1] = v2; vp[2] = v3;
  }
  return zSphere3DCreate( bb, &c, r, 0 );
}

/* a circum ball of the four points. */
static zSphere3D *_zBBallTest4(zSphere3D *bb, zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v4, zVec3D **vp)
{
  zVec3D v[3], c;
  zTri3D t;
  double r;

  zVec3DSub( v2, v1, &v[0] );
  zVec3DSub( v3, v1, &v[1] );
  zVec3DSub( v4, v1, &v[2] );
  zTri3DCreate( &t, &v[0], &v[1], &v[2] );
  zTri3DConeCircumcenter( &t, &c );
  r = zVec3DNorm( &c ) + zTOL;
  zVec3DAddDRC( &c, v1 );
  if( vp ){
    vp[0] = v1; vp[1] = v2; vp[2] = v3; vp[3] = v4;
  }
  return zSphere3DCreate( bb, &c, r, 0 );
}

/* bounding ball of the four points. */
static int _zBBall4(zSphere3D *bb, zVec3D *v[], zVec3D **vp)
{
  if( _zBBallTest2( bb, v[0], v[1], v[2], v[3], vp ) ||
      _zBBallTest2( bb, v[0], v[2], v[1], v[3], vp ) ||
      _zBBallTest2( bb, v[0], v[3], v[1], v[2], vp ) ||
      _zBBallTest2( bb, v[1], v[2], v[0], v[3], vp ) ||
      _zBBallTest2( bb, v[1], v[3], v[0], v[2], vp ) ||
      _zBBallTest2( bb, v[2], v[3], v[0], v[1], vp ) ) return 2;
  if( _zBBallTest3( bb, v[0], v[1], v[2], v[3], vp ) ||
      _zBBallTest3( bb, v[0], v[1], v[3], v[2], vp ) ||
      _zBBallTest3( bb, v[1], v[2], v[3], v[0], vp ) ) return 3;
  if( _zBBallTest4( bb, v[0], v[1], v[2], v[3], vp ) ) return 4;
  return 0;
}

/* bounding ball of up to four points. */
static int _zBBallPrim(zSphere3D *bb, zVec3DList *p, zVec3D **vp)
{
  zVec3D c, *v[4];

  switch( zListSize(p) ){
  case 0:
    zSphere3DCreate( bb, ZVEC3DZERO, -1, 0 ); /* vague */
    return 0;
  case 1:
    zSphere3DCreate( bb, zListHead(p)->data, 0, 0 );
    if( vp ) vp[0] = zListHead(p)->data;
    return 1;
  case 2:
    zVec3DMid( zListHead(p)->data, zListTail(p)->data, &c );
    zSphere3DCreate( bb, &c, zVec3DDist(zListTail(p)->data,&c)+zTOL, 0 );
    if( vp ){
      vp[0] = zListHead(p)->data;
      vp[1] = zListTail(p)->data;
    }
    return 2;
  case 3:
  case 4:
    v[0] = zListTail(p)->data;
    v[1] = zListCellNext(zListTail(p))->data;
    v[2] = zListCellNext(zListCellNext(zListTail(p)))->data;
    v[3] = zListHead(p)->data;
    return _zBBall4( bb, v, vp );
  default:
    ZRUNERROR( ZEO_ERR_FATAL );
  }
  return 0;
}

/* a recursive procedure to find bounding ball of two sets of points,
 * where the latter is the set of those on the surface of the ball. */
static int _zBBallInc(zSphere3D *bb, zVec3DList *p, zVec3DList *shell, zVec3D **vp)
{
  zVec3DListCell *cp;
  int num;

  if( zListIsEmpty(p) || zListSize(shell) == 4 )
    return _zBBallPrim( bb, shell, vp );
  zListDeleteTail( p, &cp );
  num = _zBBallInc( bb, p, shell, vp );
  if( !zSphere3DPointIsInside( bb, cp->data, true ) ){
    zListInsertTail( shell, cp );
    num = _zBBallInc( bb, p, shell, vp );
    zListPurge( shell, cp );
  }
  zListInsertTail( p, cp );
  return num;
}

/* a recursive procedure to find bounding ball of a set of 3D points. */
int zBBallPL(zSphere3D *bb, zVec3DList *p, zVec3D **vp)
{
  zVec3DList shell;
  zVec3DListCell *cp;
  int num;

  zListInit( &shell );
  if( zListSize(p) <= 4 )
    return _zBBallPrim( bb, p, vp );
  zListDeleteTail( p, &cp );
  num = zBBallPL( bb, p, vp );
  if( !zSphere3DPointIsInside( bb, cp->data, true ) ){
    zListInsertTail( &shell, cp );
    num = _zBBallInc( bb, p, &shell, vp );
    zListPurge( &shell, cp );
  }
  zListInsertTail( p, cp );
  return num;
}

/* bounding ball of 3D points. */
int zBBall(zSphere3D *bb, zVec3D p[], int num, zVec3D **vp)
{
  zVec3DAddrList pl;

  if( !zVec3DAddrListCreate( &pl, p, num ) ) return 0;
  num = zBBallPL( bb, &pl, vp );
  zVec3DAddrListDestroy( &pl );
  return num;
}
