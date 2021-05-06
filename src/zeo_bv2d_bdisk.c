/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_bdisk - 2D bounding volume: bounding disk.
 */

#include <zeo/zeo_bv2d.h>

/* ********************************************************** */
/* bounding disk / smallest enclosing disk
 * ********************************************************** */

/* test if a disk with its poles at the first two points bounds the last two points. */
static zDisk2D *_zBDiskTest2(zDisk2D *bd, zVec2D *v1, zVec2D *v2, zVec2D *v3, zVec2D **vp)
{
  zDisk2DFrom2( bd, v1, v2 );
  if( !zDisk2DPointIsInside( bd, v3, true ) ) return NULL;
  if( vp ){
    vp[0] = v1; vp[1] = v2;
  }
  return bd;
}

/* a circum disk of the three points. */
static zDisk2D *_zBDiskTest3(zDisk2D *bd, zVec2D *v1, zVec2D *v2, zVec2D *v3, zVec2D **vp)
{
  zDisk2DFrom3( bd, v1, v2, v3 );
  if( vp ){
    vp[0] = v1; vp[1] = v2; vp[2] = v3;
  }
  return bd;
}

/* bounding disk of the three points. */
static int _zBDisk3(zDisk2D *bd, zVec2D *v[], zVec2D **vp)
{
  if( _zBDiskTest2( bd, v[0], v[1], v[2], vp ) ||
      _zBDiskTest2( bd, v[1], v[2], v[0], vp ) ||
      _zBDiskTest2( bd, v[2], v[0], v[1], vp ) ) return 2;
  _zBDiskTest3( bd, v[0], v[1], v[2], vp );
  return 3;
}

/* bounding disk of up to three points. */
static int _zBDiskPrim(zDisk2D *bd, zVec2DList *pl, zVec2D **vp)
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
    return _zBDisk3( bd, v, vp );
  default:
    ZRUNERROR( ZEO_ERR_FATAL );
  }
  return 0;
}

/* a recursive procedure to find bounding disk of two sets of points,
 * where the latter is the set of those on the surface of the disk. */
static int _zBDiskInc(zDisk2D *bd, zVec2DList *pl, zVec2DList *shell, zVec2D **vp)
{
  zVec2DListCell *cp;
  int num;

  if( zListIsEmpty(pl) || zListSize(shell) == 3 )
    return _zBDiskPrim( bd, shell, vp );
  zListDeleteTail( pl, &cp );
  num = _zBDiskInc( bd, pl, shell, vp );
  if( !zDisk2DPointIsInside( bd, cp->data, true ) ){
    zListInsertTail( shell, cp );
    num = _zBDiskInc( bd, pl, shell, vp );
    zListPurge( shell, cp );
  }
  zListInsertTail( pl, cp );
  return num;
}

/* a recursive procedure to find bounding disk of a list of 2D points. */
static int _zBDiskPL(zDisk2D *bd, zVec2DList *pl, zVec2D **vp)
{
  zVec2DList shell;
  zVec2DListCell *cp;
  int num;

  zListInit( &shell );
  if( zListSize(pl) <= 3 )
    return _zBDiskPrim( bd, pl, vp );
  zListDeleteTail( pl, &cp );
  num = _zBDiskPL( bd, pl, vp );
  if( !zDisk2DPointIsInside( bd, cp->data, true ) ){
    zListInsertTail( &shell, cp );
    num = _zBDiskInc( bd, pl, &shell, vp );
    zListPurge( &shell, cp );
  }
  zListInsertTail( pl, cp );
  return num;
}

/* bounding disk of a list of 2D points. */
int zBDiskPL(zDisk2D *bd, zVec2DList *pl, zVec2D **vp)
{
  return _zBDiskPL( bd, pl, vp );
}

/* bounding disk of 2D points. */
int zBDisk(zDisk2D *bd, zVec2D p[], int num, zVec2D **vp)
{
  zVec2DAddrList pl;

  if( !zVec2DAddrListCreate( &pl, p, num ) ) return 0;
  num = zBDiskPL( bd, &pl, vp );
  zVec2DAddrListDestroy( &pl );
  return num;
}
