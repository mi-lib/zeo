/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d_aabox - 2D axis-aligned box.
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

/* copy a 2D axis-aligned box to another. */
zAABox2D *zAABox2DCopy(zAABox2D *src, zAABox2D *dst)
{
  zVec2DCopy( &src->min, &dst->min );
  zVec2DCopy( &src->max, &dst->max );
  return dst;
}

/* merge two 2D axis-aligned boxes. */
zAABox2D *zAABox2DMerge(zAABox2D *dst, zAABox2D *src1, zAABox2D *src2)
{
  return zAABox2DCreate( dst,
    _zMin( src1->min.e[zX], src2->min.e[zX] ),
    _zMin( src1->min.e[zY], src2->min.e[zY] ),
    _zMax( src1->max.e[zX], src2->max.e[zX] ),
    _zMax( src1->max.e[zY], src2->max.e[zY] ) );
}

/* check if a point is inside of a 2D axis-aligned box. */
bool zAABox2DPointIsInside(zAABox2D *box, zVec2D *p, double margin)
{
  return p->e[zX] > box->min.e[zX] - margin && p->e[zX] < box->max.e[zX] + margin &&
         p->e[zY] > box->min.e[zY] - margin && p->e[zY] < box->max.e[zY] + margin ? true : false;
}

/* compute area of a 2D axis-aligned box. */
double zAABox2DArea(zAABox2D *box)
{
  return fabs( ( box->max.e[zX] - box->min.e[zX] )
             * ( box->max.e[zY] - box->min.e[zY] ) );
}

/* print out a 2D axis-aligned box to a file in a format to be plotted. */
void zAABox2DValueFPrint(FILE *fp, zAABox2D *box)
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
