/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_aabb - 2D bounding volume: axis-aligned bounding box.
 */

#include <zeo/zeo_bv2d.h>

/* ********************************************************** */
/* zAABox2D - 2D axis-aligned box
 * ********************************************************** */

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

/* check if a point is inside of a 2D axis-aligned box. */
bool zAABox2DPointIsInside(zAABox2D *box, zVec2D *p, bool rim)
{
  double eps;

  eps = rim ? zTOL : 0;
  return p->e[zX] >= box->min.e[zX] - eps && p->e[zX] <= box->max.e[zX] + eps &&
         p->e[zY] >= box->min.e[zY] - eps && p->e[zY] <= box->max.e[zY] + eps ?
    true : false;
}

/* print a 2D axis-aligned box out to a file in a format to be plotted. */
void zAABox2DDataFPrint(FILE *fp, zAABox2D *box)
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

/* ********************************************************** */
/* AABB2D - 2D axis-aligned bounding box
 * ********************************************************** */

/* enlarge bounding box if a 2D point is out of the current box. */
static int _zAABB2DTest(zAABox2D *bb, zVec2D *p, zDir u)
{
  if( p->e[u] > bb->max.e[u] ){
    bb->max.e[u] = p->e[u];
    return 0;
  }
  if( p->e[u] < bb->min.e[u] ){
    bb->min.e[u] = p->e[u];
    return 2;
  }
  return -1;
}

/* bounding box of 2D points. */
zAABox2D *zAABB2D(zAABox2D *bb, zVec2D p[], int num, zVec2D **vp)
{
  int i, s;

  zAABox2DInit( bb );
  if( num <= 0 ) return NULL;
  zVec2DCopy( &p[0], &bb->min );
  zVec2DCopy( &p[0], &bb->max );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = &p[0];
  for( i=1; i<num; i++ ){
    if( ( s = _zAABB2DTest( bb, &p[i], zX ) ) != -1 && vp ) vp[zX+s] = &p[i];
    if( ( s = _zAABB2DTest( bb, &p[i], zY ) ) != -1 && vp ) vp[zY+s] = &p[i];
  }
  return bb;
}

/* bounding box of a list of 2D points. */
zAABox2D *zAABB2DPL(zAABox2D *bb, zVec2DList *pl, zVec2DListCell **vp)
{
  zVec2DListCell *pc;
  int s;

  zAABox2DInit( bb );
  if( zListIsEmpty(pl) ) return NULL;
  zVec2DCopy( zListTail(pl)->data, &bb->min );
  zVec2DCopy( zListTail(pl)->data, &bb->max );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = zListTail(pl);
  zListForEach( pl, pc ){
    if( ( s = _zAABB2DTest( bb, pc->data, zX ) ) != -1 && vp ) vp[zX+s] = pc;
    if( ( s = _zAABB2DTest( bb, pc->data, zY ) ) != -1 && vp ) vp[zY+s] = pc;
  }
  return bb;
}
