#include <zeo/zeo_bv2d.h>

/* check if a point is inside of a 2D axis-aligned box. */
bool _zAABox2DPointIsInside(zAABox2D *box, zVec2D *p, double margin)
{
  return p->e[zX] > box->min.e[zX] - margin && p->e[zX] < box->max.e[zX] + margin &&
         p->e[zY] > box->min.e[zY] - margin && p->e[zY] < box->max.e[zY] + margin ? true : false;
}
