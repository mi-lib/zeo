/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_aabox - 3D axis-aligned box.
 */

#include <zeo/zeo_elem3d.h>

/* initialize a 3D axis-aligned box */
zAABox3D *zAABox3DInit(zAABox3D *box)
{
  zVec3DZero( &box->min );
  zVec3DZero( &box->max );
  return box;
}

/* create a 3D axis-aligned box */
zAABox3D *zAABox3DCreate(zAABox3D *box, double x1, double y1, double z1, double x2, double y2, double z2)
{
  zVec3DCreate( &box->min, _zMin(x1,x2), _zMin(y1,y2), _zMin(z1,z2) );
  zVec3DCreate( &box->max, _zMax(x1,x2), _zMax(y1,y2), _zMax(z1,z2) );
  return box;
}

/* copy a 3D axis-aligned box to another. */
zAABox3D *zAABox3DCopy(const zAABox3D *src, zAABox3D *dst)
{
  zVec3DCopy( &src->min, &dst->min );
  zVec3DCopy( &src->max, &dst->max );
  return dst;
}

/* merge two axis-aligned boxes. */
zAABox3D *zAABox3DMerge(zAABox3D *dst, const zAABox3D *src1, const zAABox3D *src2)
{
  return zAABox3DCreate( dst,
    _zMin( src1->min.e[zX], src2->min.e[zX] ),
    _zMin( src1->min.e[zY], src2->min.e[zY] ),
    _zMin( src1->min.e[zZ], src2->min.e[zZ] ),
    _zMax( src1->max.e[zX], src2->max.e[zX] ),
    _zMax( src1->max.e[zY], src2->max.e[zY] ),
    _zMax( src1->max.e[zZ], src2->max.e[zZ] ) );
}

/* the closest point from a 3D point to a 3D axis-aligned box. */
double zAABox3DClosest(const zAABox3D *box, const zVec3D *point, zVec3D *cp)
{
  zVec3DCreate( cp,
    zLimit( point->c.x, zAABox3DXMin(box), zAABox3DXMax(box) ),
    zLimit( point->c.y, zAABox3DYMin(box), zAABox3DYMax(box) ),
    zLimit( point->c.z, zAABox3DZMin(box), zAABox3DZMax(box) ) );
  return zVec3DDist( point, cp );
}

/* distance from a point to a 3D axis-aligned box. */
double zAABox3DDistFromPoint(const zAABox3D *box, const zVec3D *point)
{
  zVec3D cp;
  return zAABox3DClosest( box, point, &cp );
}

/* check if a point is inside of a 3D axis-aligned box. */
bool zAABox3DPointIsInside(const zAABox3D *box, const zVec3D *p, double margin)
{
  return p->e[zX] > box->min.e[zX] - margin && p->e[zX] <= box->max.e[zX] + margin &&
         p->e[zY] > box->min.e[zY] - margin && p->e[zY] <= box->max.e[zY] + margin &&
         p->e[zZ] > box->min.e[zZ] - margin && p->e[zZ] <= box->max.e[zZ] + margin ? true : false;
}

/* compute volume of a 3D axis-aligned box. */
double zAABox3DVolume(const zAABox3D *box)
{
  return fabs( ( box->max.e[zX] - box->min.e[zX] )
             * ( box->max.e[zY] - box->min.e[zY] )
             * ( box->max.e[zZ] - box->min.e[zZ] ) );
}

/* get a vertex of a 3D axis-aligned box. */
zVec3D *zAABox3DVert(const zAABox3D *box, int i, zVec3D *v)
{
  _zVec3DCreate( v,
    ( (i&0x1)^(i>>1&0x1) ) ? zAABox3DXMin(box) : zAABox3DXMax(box),
    (  i&0x2             ) ? zAABox3DYMin(box) : zAABox3DYMax(box),
    (  i&0x4             ) ? zAABox3DZMin(box) : zAABox3DZMax(box) );
  return v;
}

/* print out a 3D axis-aligned box to a file. */
void zAABox3DFPrint(FILE *fp, const zAABox3D *box)
{
  fprintf( fp, "min: " ); zVec3DFPrint( fp, &box->min );
  fprintf( fp, "max: " ); zVec3DFPrint( fp, &box->max );
}

/* print out a 3D axis-aligned box to a file in a gnuplot-friendly format. */
void zAABox3DValueFPrint(FILE *fp, const zAABox3D *box)
{
  double x0, y0, z0, x1, y1, z1;

  x0 = box->min.e[zX];
  y0 = box->min.e[zY];
  z0 = box->min.e[zZ];
  x1 = box->max.e[zX];
  y1 = box->max.e[zY];
  z1 = box->max.e[zZ];
  fprintf( fp, "%g %g %g\n", x0, y0, z0 );
  fprintf( fp, "%g %g %g\n", x1, y0, z0 );
  fprintf( fp, "%g %g %g\n", x1, y1, z0 );
  fprintf( fp, "%g %g %g\n", x0, y1, z0 );
  fprintf( fp, "%g %g %g\n", x0, y0, z0 );
  fprintf( fp, "%g %g %g\n", x0, y0, z1 );
  fprintf( fp, "%g %g %g\n", x1, y0, z1 );
  fprintf( fp, "%g %g %g\n", x1, y1, z1 );
  fprintf( fp, "%g %g %g\n", x0, y1, z1 );
  fprintf( fp, "%g %g %g\n", x0, y0, z1 );
  fprintf( fp, "\n" );
  fprintf( fp, "%g %g %g\n", x1, y0, z0 );
  fprintf( fp, "%g %g %g\n", x1, y0, z1 );
  fprintf( fp, "\n" );
  fprintf( fp, "%g %g %g\n", x1, y1, z0 );
  fprintf( fp, "%g %g %g\n", x1, y1, z1 );
  fprintf( fp, "\n" );
  fprintf( fp, "%g %g %g\n", x0, y1, z0 );
  fprintf( fp, "%g %g %g\n", x0, y1, z1 );
  fprintf( fp, "\n\n" );
}
