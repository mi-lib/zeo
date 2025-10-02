/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d_box - 2D box.
 */

#include <zeo/zeo_elem2d.h>

/* initialize a 2D axis-aligned box */
zAABox2D *zAABox2DInit(zAABox2D *box)
{
  zVec2DZero( &box->min );
  zVec2DZero( &box->max );
  return box;
}

/* create a 2D axis-aligned box */
zAABox2D *zAABox2DCreate(zAABox2D *box, double x1, double y1, double x2, double y2)
{
  zVec2DCreate( &box->min, _zMin(x1,x2), _zMin(y1,y2) );
  zVec2DCreate( &box->max, _zMax(x1,x2), _zMax(y1,y2) );
  return box;
}

/* create a 2D axis-aligned box from its center, depth, and width. */
zAABox2D *zAABox2DCreateFromSize(zAABox2D *box, zVec2D *center, double depth, double width)
{
  zAABox2DXMin(box) = center->c.x - 0.5 * fabs(depth);
  zAABox2DYMin(box) = center->c.y - 0.5 * fabs(width);
  zAABox2DXMax(box) = center->c.x + 0.5 * fabs(depth);
  zAABox2DYMax(box) = center->c.y + 0.5 * fabs(width);
  return box;
}

/* check if two 2D axis-aligned boxes are equal. */
bool zAABox2DEqual(const zAABox2D *box1, const zAABox2D *box2)
{
  return _zVec2DEqual( &box1->min, &box2->min ) && _zVec2DEqual( &box1->max, &box2->max );
}

/* copy a 2D axis-aligned box to another. */
zAABox2D *zAABox2DCopy(const zAABox2D *src, zAABox2D *dst)
{
  zVec2DCopy( &src->min, &dst->min );
  zVec2DCopy( &src->max, &dst->max );
  return dst;
}

/* merge two 2D axis-aligned boxes. */
zAABox2D *zAABox2DMerge(zAABox2D *dst, const zAABox2D *src1, const zAABox2D *src2)
{
  return zAABox2DCreate( dst,
    _zMin( src1->min.e[zX], src2->min.e[zX] ),
    _zMin( src1->min.e[zY], src2->min.e[zY] ),
    _zMax( src1->max.e[zX], src2->max.e[zX] ),
    _zMax( src1->max.e[zY], src2->max.e[zY] ) );
}

/* expand a 2D axis-aligned box. */
zAABox2D *zAABox2DExpand(const zAABox2D *src, double magnitude, zAABox2D *dest)
{
  zVec2D center;

  if( magnitude < 0 ){
    ZRUNWARN( ZEO_WARN_AABOX3D_NEGATIVE_MAGNITUDE );
    magnitude *= -1;
  }
  zAABox2DCenter( src, &center );
  return zAABox2DCreateFromSize( dest, &center, zAABox2DDepth(src) * magnitude, zAABox2DWidth(src) * magnitude );
}

/* the closest point from a 2D point to a 2D axis-aligned box. */
double zAABox2DClosest(const zAABox2D *box, const zVec2D *point, zVec2D *closestpoint)
{
  zVec2DCreate( closestpoint,
    zLimit( point->c.x, zAABox2DXMin(box), zAABox2DXMax(box) ),
    zLimit( point->c.y, zAABox2DYMin(box), zAABox2DYMax(box) ) );
  return zVec2DDist( point, closestpoint );
}

/* distance from a point to a 2D axis-aligned box. */
double zAABox2DSqrDistFromPoint(const zAABox2D *box, const zVec2D *point)
{
  int i;
  double d2 = 0;

  for( i=zX; i<=zY; i++ ){
    if( point->e[i] < box->min.e[i] )
      d2 += zSqr( point->e[i] - box->min.e[i] );
    else
    if( point->e[i] > box->max.e[i] )
      d2 += zSqr( point->e[i] - box->max.e[i] );
  }
  return d2;
}

/* check if a point is inside of a 2D axis-aligned box. */
bool zAABox2DPointIsInside(const zAABox2D *box, const zVec2D *p, double margin)
{
  return p->e[zX] > box->min.e[zX] - margin && p->e[zX] < box->max.e[zX] + margin &&
         p->e[zY] > box->min.e[zY] - margin && p->e[zY] < box->max.e[zY] + margin ? true : false;
}

/* compute area of a 2D axis-aligned box. */
double zAABox2DArea(const zAABox2D *box)
{
  return fabs( ( box->max.e[zX] - box->min.e[zX] )
             * ( box->max.e[zY] - box->min.e[zY] ) );
}

/* get a vertex of a 2D axis-aligned box. */
zVec2D *zAABox2DVert(const zAABox2D *box, int i, zVec2D *v)
{
  _zVec2DCreate( v,
    ( (i&0x1)^(i>>1&0x1) ) ? zAABox2DXMin(box) : zAABox2DXMax(box),
    (  i&0x2             ) ? zAABox2DYMin(box) : zAABox2DYMax(box) );
  return v;
}

/* enlarge a 2D axis-aligned box. */
zAABox2D *zAABox2DEnlarge(zAABox2D *aabox, const zVec2D *v)
{
  if( zAABox2DXMin(aabox) > v->c.x ) zAABox2DXMin(aabox) = v->c.x;
  if( zAABox2DXMax(aabox) < v->c.x ) zAABox2DXMax(aabox) = v->c.x;
  if( zAABox2DYMin(aabox) > v->c.y ) zAABox2DYMin(aabox) = v->c.y;
  if( zAABox2DYMax(aabox) < v->c.y ) zAABox2DYMax(aabox) = v->c.y;
  return aabox;
}

/* print out a 2D axis-aligned box to a file. */
void zAABox2DFPrint(FILE *fp, const zAABox2D *box)
{
  fprintf( fp, "min: " ); zVec2DFPrint( fp, &box->min );
  fprintf( fp, "max: " ); zVec2DFPrint( fp, &box->max );
}

/* print out a 2D axis-aligned box to a file in a format to be plotted. */
void zAABox2DValueFPrint(FILE *fp, const zAABox2D *box)
{
  double x0, y0, x1, y1;

  x0 = box->min.e[zX];
  y0 = box->min.e[zY];
  x1 = box->max.e[zX];
  y1 = box->max.e[zY];
  fprintf( fp, "%g %g\n", x0, y0 );
  fprintf( fp, "%g %g\n", x1, y0 );
  fprintf( fp, "%g %g\n", x1, y1 );
  fprintf( fp, "%g %g\n", x0, y1 );
  fprintf( fp, "%g %g\n", x0, y0 );
  fprintf( fp, "\n" );
}
