/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_triangulate - trianglation of non-convex.
 */

#include <zeo/zeo_triangulate.h>

static zVec3D *_zTriangulateNorm(zLoop3D *vlist, zVec3D *norm);
static bool _zTriangulateCheck(zTri3D *t, zLoop3D *vlist, zLoop3DCell *pre, zLoop3DCell *pst);

/* (static)
 * _zTriangulateNorm
 * - normal vector of non-convex for inside-outside judgement.
 */
zVec3D *_zTriangulateNorm(zLoop3D *vlist, zVec3D *norm)
{
  double x_min;
  zLoop3DCell *vp, *st, *pre, *pst;
  zVec3D e1, e2;

  /* find x-extreme */
  x_min = ( st = zListTail(vlist) )->data->e[zX];
  zListForEach( vlist, vp )
    if( vp->data->e[zX] < x_min ){
      st = vp;
      x_min = vp->data->e[zX];
    }
  /* compute normal vector */
  pre = st == zListTail(vlist) ? zListHead(vlist) : zListCellPrev(st);
  pst = st == zListHead(vlist) ? zListTail(vlist) : zListCellNext(st);
  zVec3DSub( pre->data, st->data, &e1 );
  zVec3DSub( pst->data, st->data, &e2 );
  zVec3DOuterProd( &e2, &e1, norm );
  zVec3DNormalizeDRC( norm );
  return norm;
}

/* (static)
 * _zTriangulateCheck
 * - check if a triangle piece is valid.
 */
bool _zTriangulateCheck(zTri3D *t, zLoop3D *vlist, zLoop3DCell *pre, zLoop3DCell *pst)
{
  zLoop3DCell *vp;

  vp = pst == zListHead(vlist) ? zListTail(vlist) : zListCellNext(pst);
  while( vp != pre ){
    if( zTri3DPointIsInside( t, vp->data, true ) )
      return false;
    vp = vp == zListHead(vlist) ? zListTail(vlist) : zListCellNext(vp);
  }
  return true;
}

/* zTriangulate
 * - triangulate a non-convex loop of vertices.
 */
int zTriangulate(zVec3D v[], int n, zTri3DList *tlist)
{
  zLoop3D vlist;
  zLoop3DCell *vp, *pre, *pst;
  zVec3D norm;
  zTri3D t;

  if( !zLoop3DFromArray( &vlist, v, n ) ){
    ZALLOCERROR();
    return 0;
  }
  zListInit( tlist );
  /* normal vector for reference */
  _zTriangulateNorm( &vlist, &norm );
  /* incremental triangulation */
  while( zListNum(&vlist) > 2 ){
    vp = zListTail(&vlist);
    pre = zListHead(&vlist);
    pst = zListCellNext(vp);
    while( pst != zListRoot(&vlist) ){
      zTri3DCreate( &t, pre->data, vp->data, pst->data );
      if( zVec3DInnerProd( zTri3DNorm(&t), &norm ) > 0 &&
          _zTriangulateCheck( &t, &vlist, pre, pst ) ) break;
      vp = zListCellNext(vp);
      pre = zListCellPrev(vp);
      pst = zListCellNext(vp);
    }
    if( !zTri3DListInsert( tlist, &t ) ) break;
    zListPurge( &vlist, vp );
    zFree( vp );
  }
  zLoop3DDestroy( &vlist );
  return zListNum(tlist);
}
