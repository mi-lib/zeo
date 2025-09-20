/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d_disk - 2D disk.
 */

#include <zeo/zeo_elem2d.h>

/* ********************************************************** */
/* 2D disk class.
 * ********************************************************** */

/* create a 2D disk. */
zDisk2D *zDisk2DCreate(zDisk2D *disk, const zVec2D *c, double r)
{
  zDisk2DSetCenter( disk, c );
  zDisk2DSetRadius( disk, r );
  return disk;
}

/* signed distance from a 2D point to a disk. */
double zDisk2DDistFromPoint(const zDisk2D *disk, const zVec2D *p)
{
  return zVec2DDist( zDisk2DCenter(disk), p ) - zDisk2DRadius(disk);
}

/* check if a 2D point is inside of a disk. */
bool zDisk2DPointIsInside(const zDisk2D *disk, const zVec2D *v, double margin)
{
  return zDisk2DDistFromPoint( disk, v ) < margin ? true : false;
}

/* create a 2D disk from two points at both ends of diameter. */
zDisk2D *zDisk2DFrom2(zDisk2D *disk, const zVec2D *v1, const zVec2D *v2)
{
  zVec2DMid( v1, v2, zDisk2DCenter(disk) );
  zDisk2DSetRadius( disk, zVec2DDist(zDisk2DCenter(disk),v1) );
  return disk;
}

/* create a 2D disk from three points as the circumcircle of them. */
zDisk2D *zDisk2DFrom3(zDisk2D *disk, const zVec2D *v1, const zVec2D *v2, const zVec2D *v3)
{
  zTri2D tri;

  zTri2DCreate( &tri, v1, v2, v3 );
  zTri2DCircumcenter( &tri, zDisk2DCenter(disk) );
  zDisk2DSetRadius( disk, zVec2DDist(zDisk2DCenter(disk),v3) );
  return disk;
}

/* ********************************************************** */
/* 2D ellipse class.
 * ********************************************************** */

/* create an ellipse. */
zEllips2D *zEllips2DCreate(zEllips2D *ellips, const zVec2D *c, double r1, double r2)
{
  zEllips2DSetCenter( ellips, c );
  zEllips2DSetRadius( ellips, 0, r1 );
  zEllips2DSetRadius( ellips, 1, r2 );
  return ellips;
}

/* closest point to a 2D point on an ellipse with a margin. */
static zVec2D *_zEllips2DClosest(const zEllips2D *ellips, const zVec2D *p, double margin, zVec2D *cp)
{
  zVec2D v;
  double x2, y2, r02, r12, l;
  zPex pex;
  zComplex ans_array[4];
  zCVecStruct ans;
  int i;

  _zVec2DSub( p, zEllips2DCenter(ellips), &v );
  x2 = zSqr( v.c.x );
  y2 = zSqr( v.c.y );
  r02 = zSqr( zEllips2DRadius(ellips,0) + margin );
  r12 = zSqr( zEllips2DRadius(ellips,1) + margin );
  if( x2/r02 + y2/r12 > 1 ){
    if( !( pex = zPexAlloc( 4 ) ) ) return NULL;
    zCVecSetSizeNC( &ans, 4 );
    zCVecBufNC(&ans) = ans_array;
    zPexSetCoeff( pex, 4, 1 );
    zPexSetCoeff( pex, 3, 2 * ( r02 + r12 ) );
    zPexSetCoeff( pex, 2, r02*r02+4*r02*r12+r12*r12-x2*r02-y2*r12 );
    zPexSetCoeff( pex, 1, 2 * ( r02 + r12 - x2 - y2 ) * r02 * r12 );
    zPexSetCoeff( pex, 0, ( r02*r12 - x2*r12 - y2*r02 ) * r02 * r12 );
    zPexBH( pex, &ans, zTOL, 0 );
    zPexFree( pex );
    for( i=0; i<4; i++ )
      if( ( l = zCVecElemNC(&ans,i)->re ) >= 0 ) break;
    if( i == 4 || !zIsTiny( zCVecElemNC(&ans,i)->im ) ){
      ZRUNERROR( ZEO_ERR_FATAL );
      return NULL;
    }
    v.c.x /= 1 + l / r02;
    v.c.y /= 1 + l / r12;
    _zVec2DAdd( &v, zEllips2DCenter(ellips), cp );
  } else
    zVec2DCopy( p, cp );
  return cp;
}

/* closest point to a 2D point on an ellipse. */
zVec2D *zEllips2DClosest(const zEllips2D *ellips, const zVec2D *p, zVec2D *cp)
{
  return _zEllips2DClosest( ellips, p, 0, cp );
}

/* distance from a 2D ellipse to a 2D point. */
double zEllips2DDistFromPoint(const zEllips2D *ellips, const zVec2D *p)
{
  zVec2D cp, *ret;

  if( !( ret = zEllips2DClosest(ellips,p,&cp) ) ) return HUGE_VAL;
  return zVec2DDist( &cp, p );
}

/* check if a 2D point is inside of an ellipse. */
bool zEllips2DPointIsInside(const zEllips2D *ellips, const zVec2D *p, double margin)
{
  return zSqr( ( p->c.x - zEllips2DCenter(ellips)->c.x ) / ( zEllips2DRadius(ellips,0) + margin ) )
       + zSqr( ( p->c.y - zEllips2DCenter(ellips)->c.y ) / ( zEllips2DRadius(ellips,1) + margin ) ) < 1 ?
    true : false;
}
