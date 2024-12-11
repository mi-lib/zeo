/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_boundingdisk - 2D bounding volume: bounding disk.
 */

#include <zeo/zeo_bv2d.h>

/* ********************************************************** */
/* bounding disk / smallest enclosing disk
 * ********************************************************** */

/* test if a disk with its poles at the first two points bounds the last two points. */
static zDisk2D *_zBoundingDisk2DTest2(zDisk2D *bd, zVec2D *v1, zVec2D *v2, zVec2D *v3, zVec2D **vp)
{
  zDisk2DFrom2( bd, v1, v2 );
  if( !zDisk2DPointIsInside( bd, v3, zTOL ) ) return NULL;
  if( vp ){
    vp[0] = v1; vp[1] = v2;
  }
  return bd;
}

/* a circum disk of the three points. */
static zDisk2D *_zBoundingDisk2DTest3(zDisk2D *bd, zVec2D *v1, zVec2D *v2, zVec2D *v3, zVec2D **vp)
{
  zDisk2DFrom3( bd, v1, v2, v3 );
  if( vp ){
    vp[0] = v1; vp[1] = v2; vp[2] = v3;
  }
  return bd;
}

/* bounding disk of the three points. */
static int _zBoundingDisk2D3(zDisk2D *bd, zVec2D *v[], zVec2D **vp)
{
  if( _zBoundingDisk2DTest2( bd, v[0], v[1], v[2], vp ) ||
      _zBoundingDisk2DTest2( bd, v[1], v[2], v[0], vp ) ||
      _zBoundingDisk2DTest2( bd, v[2], v[0], v[1], vp ) ) return 2;
  _zBoundingDisk2DTest3( bd, v[0], v[1], v[2], vp );
  return 3;
}

/* bounding disk of up to three points. */
static int _zBoundingDisk2DPrim(zDisk2D *bd, zVec2DAddrList *pl, zVec2D **vp)
{
  zVec2D *v[3];

  switch( zListSize(pl) ){
  case 0:
    zDisk2DCreate( bd, ZVEC2DZERO, -1 ); /* vague */
    return 0;
  case 1:
    zDisk2DCreate( bd, zListHead(pl)->data, 0 );
    if( vp ) vp[0] = zListHead(pl)->data;
    return 1;
  case 2:
    zDisk2DFrom2( bd, zListHead(pl)->data, zListTail(pl)->data );
    if( vp ){
      vp[0] = zListHead(pl)->data;
      vp[1] = zListTail(pl)->data;
    }
    return 2;
  case 3:
    v[0] = zListTail(pl)->data;
    v[1] = zListCellNext(zListTail(pl))->data;
    v[2] = zListHead(pl)->data;
    return _zBoundingDisk2D3( bd, v, vp );
  default:
    ZRUNERROR( ZEO_ERR_FATAL );
  }
  return 0;
}

/* a recursive procedure to find bounding disk of two sets of points,
 * where the latter is the set of those on the surface of the disk. */
static int _zBoundingDisk2DInc(zDisk2D *bd, zVec2DAddrList *pl, zVec2DAddrList *shell, zVec2D **vp)
{
  zVec2DAddrListCell *cp;
  int num;

  if( zListIsEmpty(pl) || zListSize(shell) == 3 )
    return _zBoundingDisk2DPrim( bd, shell, vp );
  zListDeleteTail( pl, &cp );
  num = _zBoundingDisk2DInc( bd, pl, shell, vp );
  if( !zDisk2DPointIsInside( bd, cp->data, zTOL ) ){
    zListInsertTail( shell, cp );
    num = _zBoundingDisk2DInc( bd, pl, shell, vp );
    zListPurge( shell, cp );
  }
  zListInsertTail( pl, cp );
  return num;
}

/* a recursive procedure to find bounding disk of a list of 2D points. */
static int _zBoundingDisk2D(zDisk2D *bd, zVec2DAddrList *pl, zVec2D **vp)
{
  zVec2DAddrList shell;
  zVec2DAddrListCell *cp;
  int num;

  zListInit( &shell );
  if( zListSize(pl) <= 3 )
    return _zBoundingDisk2DPrim( bd, pl, vp );
  zListDeleteTail( pl, &cp );
  num = _zBoundingDisk2D( bd, pl, vp );
  if( !zDisk2DPointIsInside( bd, cp->data, zTOL ) ){
    zListInsertTail( &shell, cp );
    num = _zBoundingDisk2DInc( bd, pl, &shell, vp );
    zListPurge( &shell, cp );
  }
  zListInsertTail( pl, cp );
  return num;
}

/* bounding ball of 3D points. */
int zVec2DDataBoundingDisk(zVec2DData *data, zDisk2D *bd, zVec2D **vp)
{
  zVec2DAddrList addrlist;
  int num;

  if( !zVec2DDataToAddrList( data, &addrlist ) ) return 0;
  num = _zBoundingDisk2D( bd, &addrlist, vp );
  zVec2DAddrListDestroy( &addrlist );
  return num;
}
