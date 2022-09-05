/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_brep - B-Rep (boundary representation)
 */

#include <zeo/zeo_brep.h>

/* ********************************************************** */
/* CLASS: zBREPVert, zBREPEdge, zBREPFace
 * B-Rep vertex, edge and face class
 * ********************************************************** */

/* find vertex list cell. */
zBREPVertListCell *zBREPVertListFind(zBREPVertList *vlist, zVec3D *v)
{
  zBREPVertListCell *vp;

  zListForEach( vlist, vp )
    if( zVec3DEqual( &vp->data.p, v ) ) return vp;
  return NULL;
}

/* find or register a vertex list cell. */
static zBREPVertListCell *_zBREPVertListFindnReg(zBREPVertList *vlist, zVec3D *v)
{
  zBREPVertListCell *vp;

  if( ( vp = zBREPVertListFind( vlist, v ) ) ) return vp;
  if( !( vp = zAlloc( zBREPVertListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zBREPVertListCellInit( vp, v );
  zListInsertHead( vlist, vp );
  return vp;
}

/* find edge list cell. */
zBREPEdgeListCell *zBREPEdgeListFind(zBREPEdgeList *elist, zBREPVertListCell *v1, zBREPVertListCell *v2)
{
  zBREPEdgeListCell *ep;

  zListForEach( elist, ep )
    if( ( ep->data.v[0] == v1 && ep->data.v[1] == v2 ) ||
        ( ep->data.v[0] == v2 && ep->data.v[1] == v1 ) ) return ep;
  return NULL;
}

/* find or register an edge list cell. */
static zBREPEdgeListCell *_zBREPEdgeListFindnReg(zBREPEdgeList *elist, zBREPVertListCell *v1, zBREPVertListCell *v2)
{
  zBREPEdgeListCell *ep;

  if( ( ep = zBREPEdgeListFind( elist, v1, v2 ) ) ) return ep;
  if( !( ep = zAlloc( zBREPEdgeListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zBREPEdgeListCellInit( ep, v1, v2 );
  zListInsertHead( elist, ep );
  return ep;
}

/* insert face list cell. */
static bool _zBREPFaceInsert(zTri3D *face, zBREPFaceList *flist, zBREPEdgeList *elist, zBREPVertList *vlist)
{
  zBREPFaceListCell *f;
  int i;

  if( !( f = zAlloc( zBREPFaceListCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  for( i=0; i<3; i++ )
    f->data.v[i] = _zBREPVertListFindnReg( vlist, zTri3DVert(face,i) );
  for( i=0; i<3; i++ )
    f->data.e[i] = _zBREPEdgeListFindnReg( elist, f->data.v[(i+1)%3], f->data.v[(i+2)%3] );
  zVec3DCopy( zTri3DNorm(face), &f->data.norm );
  zListInsertHead( flist, f );
  return true;
}

/* ********************************************************** */
/* CLASS: zBREP
 * B-Rep class
 * ********************************************************** */

/* convert polyhedron to B-Rep solid. */
zBREP *zPH3D2BREP(zPH3D *ph, zBREP *brep)
{
  uint i;

  zListInit( &brep->vlist );
  zListInit( &brep->elist );
  zListInit( &brep->flist );
  for( i=0; i<zPH3DFaceNum(ph); i++ )
    if( !_zBREPFaceInsert( zPH3DFace(ph,i), &brep->flist, &brep->elist, &brep->vlist ) ){
      ZRUNERROR( ZEO_ERR_BREP_CONV );
      zBREPDestroy( brep );
      return NULL;
    }
  return brep;
}

/* convert polyhedron restricted in a box to B-Rep solid. */
zBREP *zPH3D2BREPInBox(zPH3D *ph, zAABox3D *box, zBREP *brep)
{
  zTri3D *tri;
  uint i;

  zListInit( &brep->vlist );
  zListInit( &brep->elist );
  zListInit( &brep->flist );
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    if( zColChkTriAABox3D( ( tri = zPH3DFace(ph,i) ), box ) ){
      if( !_zBREPFaceInsert( zPH3DFace(ph,i), &brep->flist, &brep->elist, &brep->vlist ) ){
        ZRUNERROR( ZEO_ERR_BREP_CONV );
        zBREPDestroy( brep );
        return NULL;
      }
    }
  }
  return brep;
}

/* convert B-Rep solid to polyhedron. */
zPH3D *zBREP2PH3D(zBREP *brep, zPH3D *ph)
{
  uint i;
  zBREPVertListCell *vp;
  zBREPFaceListCell *fp;

  if( !zPH3DAlloc( ph, zListSize(&brep->vlist), zListSize(&brep->flist) ) ){
    ZALLOCERROR();
    return NULL;
  }
  i = 0;
  zListForEach( &brep->vlist, vp ){
    zVec3DCopy( &vp->data.p, zPH3DVert(ph,i) );
    vp->data._p = (void *)zPH3DVert(ph,i);
    i++;
  }
  i = 0;
  zListForEach( &brep->flist, fp ){
    zTri3DCreate( zPH3DFace(ph,i), (zVec3D*)fp->data.v[0]->data._p, (zVec3D*)fp->data.v[1]->data._p, (zVec3D*)fp->data.v[2]->data._p );
    i++;
  }
  return ph;
}

/* destroy B-Rep solid. */
void zBREPDestroy(zBREP *brep)
{
  zListDestroy( zBREPFaceListCell, &brep->flist );
  zListDestroy( zBREPEdgeListCell, &brep->elist );
  zListDestroy( zBREPVertListCell, &brep->vlist );
}
