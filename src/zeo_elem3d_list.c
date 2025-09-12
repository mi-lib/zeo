/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_list - 3D shape element list.
 */

#include <zeo/zeo_elem3d.h>

/* ********************************************************** */
/* CLASS: zTri3DList
 * 3D triangle list
 * ********************************************************** */

/* add a 3D triangle list cell. */
zTri3DListCell *zTri3DListAdd(zTri3DList *list, const zTri3D *t)
{
  zTri3DListCell *cell;

  if( !( cell = zAlloc( zTri3DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zCopy( zTri3D, t, &cell->data );
  zListInsertHead( list, cell );
  return cell;
}

/* align triangles to a direction referred by a vector. */
void zTri3DListAlign(zTri3DList *list, const zVec3D *ref)
{
  zTri3DListCell *tp;

  zListForEach( list, tp )
    if( zVec3DInnerProd( ref, zTri3DNorm(&tp->data) ) < 0 )
      zTri3DFlipDRC( &tp->data );
}

/* copy triangles in a list to array. */
void zTri3DListCopyArray(const zTri3DList *list, zTri3D t[], int n)
{
  zTri3DListCell *tp;
  int i = 0;

  zListForEach( list, tp ){
    zCopy( zTri3D, &tp->data, &t[i] );
    if( ++i >= n ) break;
  }
}

/* ********************************************************** */
/* triangulation of non-convex.
 * ********************************************************** */

/* normal vector of non-convex for inside-outside judgement. */
static zVec3D *_zLoop3DTriangulateNorm(const zLoop3D *loop, zVec3D *norm)
{
  double x_min;
  zLoop3DCell *vp, *st, *pre, *pst;
  zVec3D e1, e2;

  /* find x-extreme */
  x_min = ( st = zListTail(loop) )->data->e[zX];
  zListForEach( loop, vp )
    if( vp->data->e[zX] < x_min ){
      st = vp;
      x_min = vp->data->e[zX];
    }
  /* compute normal vector */
  pre = st == zListTail(loop) ? zListHead(loop) : zListCellPrev(st);
  pst = st == zListHead(loop) ? zListTail(loop) : zListCellNext(st);
  zVec3DSub( pre->data, st->data, &e1 );
  zVec3DSub( pst->data, st->data, &e2 );
  zVec3DOuterProd( &e2, &e1, norm );
  zVec3DNormalizeDRC( norm );
  return norm;
}

/* check if a triangle piece is valid. */
static bool _zLoop3DTriangulateCheck(const zLoop3D *loop, const zTri3D *t, zLoop3DCell *pre, zLoop3DCell *pst)
{
  zLoop3DCell *vp;

  vp = pst == zListHead(loop) ? zListTail(loop) : zListCellNext(pst);
  while( vp != pre ){
    if( zTri3DPointIsInside( t, vp->data, zTOL ) )
      return false;
    vp = vp == zListHead(loop) ? zListTail(loop) : zListCellNext(vp);
  }
  return true;
}

/* triangulate a simple loop of vertices. */
static int _zLoop3DTriangulate(zLoop3D *loop, zTri3DList *tlist)
{
  zLoop3DCell *vp, *pre, *pst;
  zVec3D norm;
  zTri3D t;

  zListInit( tlist );
  /* normal vector for reference */
  _zLoop3DTriangulateNorm( loop, &norm );
  /* incremental triangulation */
  while( zListSize(loop) > 2 ){
    vp = zListTail(loop);
    pre = zListHead(loop);
    pst = zListCellNext(vp);
    while( pst != zListRoot(loop) ){
      zTri3DCreate( &t, pre->data, vp->data, pst->data );
      if( zVec3DInnerProd( zTri3DNorm(&t), &norm ) > 0 &&
          _zLoop3DTriangulateCheck( loop, &t, pre, pst ) ) break;
      vp = zListCellNext(vp);
      pre = zListCellPrev(vp);
      pst = zListCellNext(vp);
    }
    if( !zTri3DListAdd( tlist, &t ) ) break;
    zListPurge( loop, vp );
    zFree( vp );
  }
  return zListSize(tlist);
}

/* triangulate a simple loop of vertices. */
int zLoop3DTriangulate(const zLoop3D *loop, zTri3DList *tlist)
{
  zLoop3D loop_copy;
  int ret = 0;

  if( zVec3DAddrListClone( loop, &loop_copy ) )
    ret = _zLoop3DTriangulate( &loop_copy, tlist );
  zLoop3DDestroy( &loop_copy );
  return ret;
}

/* triangulate a non-convex loop of vertices. */
int zTriangulate(const zVec3DArray *array, zTri3DList *tlist)
{
  zLoop3D loop;

  if( !zLoop3DCreate( &loop, array ) ){
    ZALLOCERROR();
    return 0;
  }
  _zLoop3DTriangulate( &loop, tlist );
  zLoop3DDestroy( &loop );
  return zListSize(tlist);
}
