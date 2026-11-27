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

/* the closest point from a 2D point to a disk. */
double zDisk2DClosest(const zDisk2D *disk, const zVec2D *p, zVec2D *cp)
{
  zVec2D d;
  double l;

  zVec2DSub( p, zDisk2DCenter(disk), &d );
  if( ( l = zVec2DNorm(&d) ) <= zDisk2DRadius(disk) ){
    zVec2DCopy( p, cp );
    return 0;
  }
  zVec2DCat( zDisk2DCenter(disk), zDisk2DRadius(disk)/l, &d, cp );
  return l - zDisk2DRadius(disk);
}

/* signed distance from a 2D point to a disk. */
double zDisk2DSignedDistFromPoint(const zDisk2D *disk, const zVec2D *p)
{
  return zVec2DDist( zDisk2DCenter(disk), p ) - zDisk2DRadius(disk);
}

/* distance from a 2D point to a disk. */
double zDisk2DDistFromPoint(const zDisk2D *disk, const zVec2D *p)
{
  double d;

  return ( d = zDisk2DSignedDistFromPoint( disk, p ) ) > 0 ? d : 0;
}

/* check if a 2D point is inside of a disk. */
bool zDisk2DPointIsInside(const zDisk2D *disk, const zVec2D *v, double margin)
{
  return zDisk2DSignedDistFromPoint( disk, v ) < margin;
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
  if( !zTri2DCircumcenter( &tri, zDisk2DCenter(disk) ) ) return NULL;
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

/* the closest point on the boundary of an axis-aligned ellipse, the signed distance from it and the
 * inside check of it. */
ZEO_ELEM_ELLIPSXD_CLOSEST( 2 )
ZEO_ELEM_ELLIPSXD_SIGNED_DIST( 2 )
ZEO_ELEM_ELLIPSXD_IS_INSIDE( 2 )

/* closest point to a 2D point on an ellipse. */
zVec2D *zEllips2DClosest(const zEllips2D *ellips, const zVec2D *p, zVec2D *cp)
{
  zVec2D v, vc;

  _zVec2DSub( p, zEllips2DCenter(ellips), &v );
  if( zSqr( v.c.x / zEllips2DRadius(ellips,0) ) + zSqr( v.c.y / zEllips2DRadius(ellips,1) ) <= 1 )
    return zVec2DCopy( p, cp ); /* inside */
  _zEllips2DClosest( ellips->radius, &v, &vc );
  _zVec2DAdd( &vc, zEllips2DCenter(ellips), cp );
  return cp;
}

/* signed distance from a 2D point to a 2D ellipse. */
double zEllips2DSignedDistFromPoint(const zEllips2D *ellips, const zVec2D *p)
{
  zVec2D v;

  _zVec2DSub( p, zEllips2DCenter(ellips), &v );
  return _zEllips2DSignedDist( ellips->radius, &v );
}

/* distance from a 2D ellipse to a 2D point. */
double zEllips2DDistFromPoint(const zEllips2D *ellips, const zVec2D *p)
{
  zVec2D cp;

  zEllips2DClosest( ellips, p, &cp );
  return zVec2DDist( &cp, p );
}

/* check if a 2D point is inside of an ellipse. */
bool zEllips2DPointIsInside(const zEllips2D *ellips, const zVec2D *p, double margin)
{
  zVec2D v;

  _zVec2DSub( p, zEllips2DCenter(ellips), &v );
  return _zEllips2DIsInside( ellips->radius, &v, margin );
}
