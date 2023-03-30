/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_obb - 3D bounding volume: oriented bounding box.
 */

#include <zeo/zeo_bv2d.h>
#include <zeo/zeo_bv3d.h>

/* minimum axis direction of oriented bounding box. */
static void _zOBB3DMinDir(zBox3D *obb, zPH3D *ch, zPlane3D *pl)
{
  zTri3D *bot;
  double l, ls;
  int i, j;

  zBox3DSetHeight( obb, HUGE_VAL );
  for( bot=NULL, i=0; i<zPH3DFaceNum(ch); i++ ){
    for( l=0, j=0; j<zPH3DVertNum(ch); j++ )
      if( ( ls = -zTri3DPointDist( zPH3DFace(ch,i), zPH3DVert(ch,j) ) ) > l )
        l = ls;
    if( l < zBox3DHeight(obb) ){
      bot = zPH3DFace(ch,i);
      zBox3DSetHeight( obb, l );
    }
  }
#if 0 /* GCC 11 doesn't compile the following line correctly. I don't know why. */
  zVec3DCopy( zTri3DNorm(bot), zBox3DAxis(obb,zZ) );
#else
  zVec3DCreate( zBox3DAxis(obb,zZ), zTri3DNorm(bot)->c.x, zTri3DNorm(bot)->c.y, zTri3DNorm(bot)->c.z );
#endif
  zVec3DCat( zTri3DVert(bot,0), -0.5*zBox3DHeight(obb), zBox3DAxis(obb,zZ), zBox3DCenter(obb) ); /* tentative origin */
  zPlane3DCreate( pl, zBox3DCenter(obb), zBox3DAxis(obb,zZ) );
}

/* minimum bounding rectangle of a planar convex hull. */
static double _zOBB3DRect(zLoop3D *edge, zVec3D *c, zVec3D *d1, zVec3D *d2, double *l1, double *l2)
{
  double min1, min2, max1, max2, d;
  zLoop3DCell *vp;

  min1 = max1 = zVec3DInnerProd( d1, zListTail(edge)->data );
  min2 = max2 = zVec3DInnerProd( d2, zListTail(edge)->data );
  zListForEach( edge, vp ){
    d = zVec3DInnerProd( d1, vp->data );
    if( d < min1 ) min1 = d;
    if( d > max1 ) max1 = d;
    d = zVec3DInnerProd( d2, vp->data );
    if( d < min2 ) min2 = d;
    if( d > max2 ) max2 = d;
  }
  zVec3DMul( d1, 0.5 * ( max1 + min1 ), c );
  zVec3DCatDRC( c, 0.5 * ( max2 + min2 ), d2 );
  return ( *l1 = max1 - min1 ) * ( *l2 = max2 - min2 );
}

/* maximum (and the second maximum) axis direction of oriented bounding box. */
static bool _zOBB3DMaxDir(zBox3D *obb, zPH3D *ch, zPlane3D *pl)
{
  zLoop3D rim;
  zLoop3DCell *vp1, *vp2;
  zVec3D e1, e2, c, dc;
  double l1, l2;
  double area, area_min;
  int i;

  for( i=0; i<zPH3DVertNum(ch); i++ ){
    zPlane3DProj( pl, zPH3DVert(ch,i), zPH3DVert(ch,i) );
    /* shift to the plane passing the original point */
    zVec3DSubDRC( zPH3DVert(ch,i), zBox3DCenter(obb) );
  }
  if( !zCH2D( &rim, zPH3DVertBuf(ch), zPH3DVertNum(ch) ) ) return false;
  /* minimum rectangle on minimum-eigenvalue plane */
  vp1 = zListTail(&rim);
  vp2 = zListHead(&rim);
  zBox3DDepth(obb) = zBox3DWidth(obb) = area_min = HUGE_VAL;
  zVec3DZero( &dc );
  for( ; vp1!=zListRoot(&rim); vp1=zListCellNext(vp1), vp2=zListCellPrev(vp1) ){
    zVec3DSub( vp2->data, vp1->data, &e1 );
    zVec3DOuterProd( zBox3DAxis(obb,zZ), &e1, &e2 );
    zVec3DNormalizeDRC( &e1 );
    zVec3DNormalizeDRC( &e2 );
    area = _zOBB3DRect( &rim, &c, &e1, &e2, &l1, &l2 );
    if( area < area_min ){
      zVec3DCopy( &e1, zBox3DAxis(obb,zX) );
      zVec3DCopy( &e2, zBox3DAxis(obb,zY) );
      zVec3DCopy( &c, &dc );
      zBox3DSetDepth( obb, l1 );
      zBox3DSetWidth( obb, l2 );
      area_min = area;
    }
  }
  zLoop3DDestroy( &rim );
  /* shift-back the center point */
  zVec3DAddDRC( zBox3DCenter(obb), &dc );
  return true;
}

/* oriented bounding box. */
zBox3D *zOBB3D(zBox3D *obb, zVec3D p[], int n)
{
  zPH3D ch;
  zPlane3D pln;

  if( !zCH3D( &ch, p, n ) ) return NULL;
  _zOBB3DMinDir( obb, &ch, &pln );
  if( !_zOBB3DMaxDir( obb, &ch, &pln ) ) obb = NULL;
  zPH3DDestroy( &ch );
  return obb;
}

/* oriented bounding box of a list of points. */
zBox3D *zOBB3DPL(zBox3D *obb, zVec3DList *pl)
{
  zPH3D ch;
  zPlane3D pln;

  if( !zCH3DPL( &ch, pl ) ) return NULL;
  _zOBB3DMinDir( obb, &ch, &pln );
  if( !_zOBB3DMaxDir( obb, &ch, &pln ) ) obb = NULL;
  zPH3DDestroy( &ch );
  return obb;
}
