/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_brep_bool - boolean operations of B-Rep.
 */

#include <zeo/zeo_brep.h>

/* zBREPMerge
 * - merge a B-Rep to another (destructive).
 */
zBREP *zBREPMerge(zBREP *target, zBREP *sub)
{
  zBREPVertListCell *vp;
  zBREPEdgeListCell *ep, *en;
  zBREPFaceListCell *fp;
  register int i;

  /* mark vertices */
  zListForEach( &sub->vlist, vp )
    vp->data._p = (void *)zBREPVertListFind( &target->vlist, &vp->data.p );
  /* mark ridges */
  zListForEach( &sub->elist, ep ){
    ep->data._v = NULL;
    if( ep->data.v[0]->data._p && ep->data.v[1]->data._p )
      if( !( ep->data._v = (void *)zBREPEdgeListFind( &target->elist, ep->data.v[0]->data._p, ep->data.v[1]->data._p ) ) ){
        if( !( en = zAlloc( zBREPEdgeListCell, 1 ) ) ){
          ZALLOCERROR();
          return NULL;
        }
        zBREPEdgeListCellInit( en, ep->data.v[0]->data._p, ep->data.v[1]->data._p );
        ep->data._v = en;
        zListInsertHead( &target->elist, en );
      }
  }
  /* reassign boundaries of faces */
  zListForEach( &sub->flist, fp )
    for( i=0; i<3; i++ ){
      if( fp->data.v[i]->data._p )
        fp->data.v[i] = (zBREPVertListCell *)fp->data.v[i]->data._p;
      if( fp->data.e[i]->data._v )
        fp->data.e[i] = (zBREPEdgeListCell *)fp->data.e[i]->data._v;
    }
  /* merge vertices */
  while( !zListIsEmpty(&sub->vlist) ){
    zListDeleteHead( &sub->vlist, &vp );
    if( vp->data._p )
      zFree( vp );
    else
      zListInsertHead( &target->vlist, vp );
  }
  /* merge edges */
  while( !zListIsEmpty(&sub->elist) ){
    zListDeleteHead( &sub->elist, &ep );
    if( ep->data._v )
      zFree( ep );
    else
      zListInsertHead( &target->elist, ep );
  }
  /* merge faces */
  zListAppend( &target->flist, &sub->flist );

  return target;
}

/* zPH3DTruncBREP
 * - truncate a polyhedron by another.
 */
zPH3D *zPH3DTruncBREP(zPH3D *src, zPH3D *cut, zPH3D *dest)
{
  zBREP bs;

  if( !zPH3D2BREP( src, &bs ) || !zBREPTruncPH3D( &bs, cut ) ||
      !zBREP2PH3D( &bs, dest ) )
    return NULL;
  zBREPDestroy( &bs );
  return dest;
}

/* zIntersectPH3DBREP
 * - intersection of two convex polyhedra.
 */
zPH3D *zIntersectPH3DBREP(zPH3D *src1, zPH3D *src2, zPH3D *dest)
{
  zBREP b1, b2;

  if( !zPH3D2BREP( src1, &b1 ) || !zPH3D2BREP( src2, &b2 ) ||
      !zBREPTruncPH3D( &b1, src2 ) || !zBREPTruncPH3D( &b2, src1 ) ||
      !zBREPMerge( &b1, &b2 ) ||
      !zBREP2PH3D( &b1, dest ) )
    return NULL;
  zBREPDestroy( &b1 );
  zBREPDestroy( &b2 );
  return dest;
}

/* zIntersectPH3DBREPFast
 * - intersection of two convex polyhedra.
 */
zPH3D *zIntersectPH3DBREPFast(zPH3D *src1, zPH3D *src2, zPH3D *dest)
{
  zAABox3D ib;
  zBREP b1, b2;

  if( !zIntersectPH3DBox( src1, src2, &ib ) ||
      !zPH3D2BREPInBox( src1, &ib, &b1 ) ||
      !zPH3D2BREPInBox( src2 , &ib, &b2 ) ||
      !zBREPTruncPH3D( &b1, src2 ) || !zBREPTruncPH3D( &b2, src1 ) ||
      !zBREPMerge( &b1, &b2 ) ||
      !zBREP2PH3D( &b1, dest ) )
    return NULL;
  zBREPDestroy( &b1 );
  zBREPDestroy( &b2 );
  return dest;
}
