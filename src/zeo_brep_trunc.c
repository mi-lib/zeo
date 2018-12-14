/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_brep_trunc - truncation of B-Rep.
 */

#include <zeo/zeo_brep.h>

static int _zBREPTruncBBCheck(zBREPVertList *vlist, zPlane3D *pl);
static bool _zBREPTruncIntersect(zBREP *brep, zPlane3D *pl);

static bool _zBREPTruncFaceA(zBREP *brep, zBREPFace *f, int i1, int i2, int i3);
static bool _zBREPTruncFaceV(zBREP *brep, zBREPFace *f, int i1, int i2, int i3);
static bool _zBREPTruncFaceE(zBREP *brep, zBREPFace *f, int i1, int i2, int i3);
static bool _zBREPTruncFace(zBREP *brep, zBREPFaceListCell **fp);
static void _zBREPTruncEdgeShrink(zBREPEdgeList *elist);
static void _zBREPTruncGR(zBREP *brep);

/* (static)
 * _zBREPTruncBBCheck
 * - beneath-beyond check with respect to a plane.
 */
int _zBREPTruncBBCheck(zBREPVertList *vlist, zPlane3D *pl)
{
  int count = 0;
  zBREPVertListCell *vp;
  zVec3D tmp;

  zListForEach( vlist, vp ){
    _zVec3DSub( &vp->data.p, zPlane3DVert(pl), &tmp );
    vp->data._d = _zVec3DInnerProd( &tmp, zPlane3DNorm(pl) );
    if( zIsTiny( vp->data._d ) ) vp->data._d = 0;
    if( vp->data._d > 0 ) count++;
  }
  return count;
}

/* (static)
 * _zBREPTruncIntersect
 * - compute intersections of edges and a cutting plane.
 */
bool _zBREPTruncIntersect(zBREP *brep, zPlane3D *pl)
{
  zBREPEdgeListCell *ep;
  zBREPVertListCell *vp;
  double d1, d2;

  zListForEach( &brep->elist, ep ){
    if( ep->data.v[0]->data._d * ep->data.v[1]->data._d < 0 ){
      if( !( vp = zAlloc( zBREPVertListCell, 1 ) ) ){
        ZALLOCERROR();
        return false;
      }
      vp->data._d = 0;
      vp->data._p = NULL;
      d1 = fabs( ep->data.v[0]->data._d );
      d2 = fabs( ep->data.v[1]->data._d );
      zVec3DInterDiv( &ep->data.v[0]->data.p, &ep->data.v[1]->data.p,
        d1/(d1+d2), &vp->data.p );
      zListInsertHead( &brep->vlist, vp );
      ep->data._v = vp;
    }
  }
  return true;
}

/* (static)
 * _zBREPTruncFaceA
 * - A type face reconfiguration (2-beneath, 1-beyond).
 */
bool _zBREPTruncFaceA(zBREP *brep, zBREPFace *f, int i1, int i2, int i3)
{
  zBREPEdgeListCell *e4, *e5;
  zBREPFaceListCell *fp;

  e4 = zAlloc( zBREPEdgeListCell, 1 );
  e5 = zAlloc( zBREPEdgeListCell, 1 );
  fp = zAlloc( zBREPFaceListCell, 1 );
  if( !e4 || !e5 || !fp ){
    ZALLOCERROR();
    zFree( e4 );
    zFree( e5 );
    zFree( fp );
    return false;
  }
  /* trim edge */
  zBREPEdgeListCellInit( e4, f->e[i2]->data._v, f->e[i3]->data._v );
  zListInsertHead( &brep->elist, e4 );
  /* bracing edge */
  zBREPEdgeListCellInit( e5, f->e[i2]->data._v, f->v[i2] );
  zListInsertHead( &brep->elist, e5 );
  /* upper triangle */
  fp->data.v[0] = f->v[i2];
  fp->data.v[1] = f->e[i2]->data._v;
  fp->data.v[2] = f->e[i3]->data._v;
  fp->data.e[0] = e4;
  fp->data.e[1] = f->e[i3];
  fp->data.e[2] = e5;
  zListInsertTail( &brep->flist, fp );
  /* lower triangle */
  f->e[i3] = e5;
  f->v[i1] = f->e[i2]->data._v;
  return true;
}

/* (static)
 * _zBREPTruncFaceV
 * - V type face reconfiguration (1-beneath, 2-beyond).
 */
bool _zBREPTruncFaceV(zBREP *brep, zBREPFace *f, int i1, int i2, int i3)
{
  zBREPEdgeListCell *e4;

  if( !( e4 = zAlloc( zBREPEdgeListCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  /* trim edge */
  zBREPEdgeListCellInit( e4, f->e[i3]->data._v, f->e[i2]->data._v );
  zListInsertHead( &brep->elist, e4 );
  /* renewal triangle */
  f->e[i1] = e4;
  f->v[i2] = f->e[i3]->data._v;
  f->v[i3] = f->e[i2]->data._v;
  return true;
}

/* (static)
 * _zBREPTruncFaceE
 * - E type face reconfiguration (1-beneath, 1-beyond, 1-border).
 */
bool _zBREPTruncFaceE(zBREP *brep, zBREPFace *f, int i1, int i2, int i3)
{
  zBREPEdgeListCell *e4;

  if( !( e4 = zAlloc( zBREPEdgeListCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  /* trim edge */
  zBREPEdgeListCellInit( e4, f->v[i1], f->e[i1]->data._v );
  zListInsertHead( &brep->elist, e4 );
  /* renewal triangle */
  f->v[i3] = f->e[i1]->data._v;
  f->e[i2] = e4;
  return true;
}

/* (static)
 * _zBREPTruncEdgeShrink
 * - shrink intersecting edges.
 */
void _zBREPTruncEdgeShrink(zBREPEdgeList *elist)
{
  zBREPEdgeListCell *ep;

  zListForEach( elist, ep ){
    if( !ep->data._v ) continue;
    if( ep->data.v[0]->data._d > 0 )
      ep->data.v[0] = ep->data._v;
    else
      ep->data.v[1] = ep->data._v;
    ep->data._v = NULL;
  }
}

/* (static)
 * _zBREPTruncGR
 * - release garbages.
 */
void _zBREPTruncGR(zBREP *brep)
{
  zBREPVertListCell *vp, *vpp;
  zBREPEdgeListCell *ep, *epp;
  zBREPFaceListCell *fp;
  register int i;

  /* initialize mark */
  zListForEach( &brep->vlist, vp ) vp->data._p = NULL;
  zListForEach( &brep->elist, ep ) ep->data._v = NULL;
  /* mark */
  zListForEach( &brep->flist, fp )
    for( i=0; i<3; i++ ){
      fp->data.v[i]->data._p = (void *)0xffff;
      fp->data.e[i]->data._v = (void *)0xffff;
    }
  /* garbage release */
  zListForEach( &brep->vlist, vp )
    if( vp->data._p == (void *)0xffff )
      vp->data._p = NULL;
    else{
      vp = zListCellPrev( vp );
      zListDeleteNext( &brep->vlist, vp, &vpp );
      zFree( vpp );
    }
  zListForEach( &brep->elist, ep )
    if( ep->data._v == (void *)0xffff )
      ep->data._v = NULL;
    else{
      ep = zListCellPrev( ep );
      zListDeleteNext( &brep->elist, ep, &epp );
      zFree( epp );
    }
}

/* (static)
 * _zBREPTruncFace
 * - face reconfiguration.
 */
#define __z_brep_trunc_pat(d) ( (d)<0 ? 3: ( (d)>0 ? 1 : 0 ) )
bool _zBREPTruncFace(zBREP *brep, zBREPFaceListCell **fp)
{
  zBREPFaceListCell *fpp;
  zBREPFace *f;

  f = &(*fp)->data;
  switch( __z_brep_trunc_pat( f->v[0]->data._d )
        | __z_brep_trunc_pat( f->v[1]->data._d ) << 2
        | __z_brep_trunc_pat( f->v[2]->data._d ) << 4 ){
  case 0x01: case 0x04: case 0x05:
  case 0x10: case 0x11: case 0x14: case 0x15:
    (*fp) = zListCellPrev( (*fp) );
    zListDeleteNext( &brep->flist, (*fp), &fpp );
    zFree( fpp );
  case 0x00: case 0x03: case 0x0c: case 0x0f:
  case 0x30: case 0x33: case 0x3c: case 0x3f:
    return true;
  case 0x1f: return _zBREPTruncFaceA( brep, f, 2, 0, 1 );
  case 0x37: return _zBREPTruncFaceA( brep, f, 1, 2, 0 );
  case 0x3d: return _zBREPTruncFaceA( brep, f, 0, 1, 2 );
  case 0x07: return _zBREPTruncFaceE( brep, f, 2, 0, 1 );
  case 0x0d: return _zBREPTruncFaceE( brep, f, 2, 1, 0 );
  case 0x13: return _zBREPTruncFaceE( brep, f, 1, 0, 2 );
  case 0x1c: return _zBREPTruncFaceE( brep, f, 0, 1, 2 );
  case 0x31: return _zBREPTruncFaceE( brep, f, 1, 2, 0 );
  case 0x34: return _zBREPTruncFaceE( brep, f, 0, 2, 1 );
  case 0x17: return _zBREPTruncFaceV( brep, f, 0, 1, 2 );
  case 0x1d: return _zBREPTruncFaceV( brep, f, 1, 2, 0 );
  case 0x35: return _zBREPTruncFaceV( brep, f, 2, 0, 1 );
  default:
    ZRUNERROR( ZEO_ERR_FATAL );
  }
  return false;
}

/* zBREPTrunc
 * - truncate B-Rep by a plane.
 */
zBREP *zBREPTrunc(zBREP *brep, zPlane3D *pl)
{
  zBREPFaceListCell *fp;

  if( _zBREPTruncBBCheck( &brep->vlist, pl ) == 0 )
    return brep; /* no need to truncate */
  if( !_zBREPTruncIntersect( brep, pl ) ) return NULL;
  zListForEach( &brep->flist, fp )
    if( !_zBREPTruncFace( brep, &fp ) ) return NULL;

  _zBREPTruncEdgeShrink( &brep->elist );
  _zBREPTruncGR( brep );
  return brep;
}

/* zBREPTruncPH3D
 * - truncate B-Rep by a polyhedron.
 */
zBREP *zBREPTruncPH3D(zBREP *brep, zPH3D *ph)
{
  register int i;
  zPlane3D pl;

  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    zTri3DToPlane3D( zPH3DFace(ph,i), &pl );
    if( !zBREPTrunc( brep, &pl ) ) return NULL;
  }
  return brep;
}
