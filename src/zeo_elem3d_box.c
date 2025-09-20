/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_aabox - 3D axis-aligned box.
 */

#include <zeo/zeo_elem3d.h>

/* ********************************************************** */
/* 3D axis-aligned box class.
 *//* ******************************************************* */

/* initialize a 3D axis-aligned box. */
zAABox3D *zAABox3DInit(zAABox3D *box)
{
  zVec3DZero( &box->min );
  zVec3DZero( &box->max );
  return box;
}

/* create a 3D axis-aligned box. */
zAABox3D *zAABox3DCreate(zAABox3D *box, double x1, double y1, double z1, double x2, double y2, double z2)
{
  zVec3DCreate( &box->min, _zMin(x1,x2), _zMin(y1,y2), _zMin(z1,z2) );
  zVec3DCreate( &box->max, _zMax(x1,x2), _zMax(y1,y2), _zMax(z1,z2) );
  return box;
}

/* create a 3D axis-aligned box from its center, depth, width, and height. */
zAABox3D *zAABox3DCreateFromSize(zAABox3D *box, zVec3D *center, double depth, double width, double height)
{
  zAABox3DXMin(box) = center->c.x - 0.5 * fabs(depth);
  zAABox3DYMin(box) = center->c.y - 0.5 * fabs(width);
  zAABox3DZMin(box) = center->c.z - 0.5 * fabs(height);
  zAABox3DXMax(box) = center->c.x + 0.5 * fabs(depth);
  zAABox3DYMax(box) = center->c.y + 0.5 * fabs(width);
  zAABox3DZMax(box) = center->c.z + 0.5 * fabs(height);
  return box;
}

/* check if two 3D axis-aligned boxes are equal. */
bool zAABox3DEqual(const zAABox3D *box1, const zAABox3D *box2)
{
  return zVec3DEqual( &box1->min, &box2->min ) && zVec3DEqual( &box1->max, &box2->max );
}

/* copy a 3D axis-aligned box to another. */
zAABox3D *zAABox3DCopy(const zAABox3D *src, zAABox3D *dest)
{
  zVec3DCopy( &src->min, &dest->min );
  zVec3DCopy( &src->max, &dest->max );
  return dest;
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

/* expand a 3D axis-aligned box. */
zAABox3D *zAABox3DExpand(const zAABox3D *src, double magnitude, zAABox3D *dest)
{
  zVec3D center;

  if( magnitude < 0 ){
    ZRUNWARN( ZEO_WARN_AABOX3D_NEGATIVE_MAGNITUDE );
    magnitude *= -1;
  }
  zAABox3DCenter( src, &center );
  return zAABox3DCreateFromSize( dest, &center,
    zAABox3DDepth(src) * magnitude, zAABox3DWidth(src) * magnitude, zAABox3DHeight(src) * magnitude );
}

/* the closest point from a 3D point to a 3D axis-aligned box. */
double zAABox3DClosest(const zAABox3D *box, const zVec3D *point, zVec3D *closestpoint)
{
  zVec3DCreate( closestpoint,
    zLimit( point->c.x, zAABox3DXMin(box), zAABox3DXMax(box) ),
    zLimit( point->c.y, zAABox3DYMin(box), zAABox3DYMax(box) ),
    zLimit( point->c.z, zAABox3DZMin(box), zAABox3DZMax(box) ) );
  return zVec3DDist( point, closestpoint );
}

/* distance from a point to a 3D axis-aligned box. */
double zAABox3DSqrDistFromPoint(const zAABox3D *box, const zVec3D *point)
{
  int i;
  double d2 = 0;

  for( d2=0, i=zX; i<=zZ; i++ ){
    if( point->e[i] < box->min.e[i] )
      d2 += zSqr( point->e[i] - box->min.e[i] );
    else
    if( point->e[i] > box->max.e[i] )
      d2 += zSqr( point->e[i] - box->max.e[i] );
  }
  return d2;
}

/* check if a point is inside of a 3D axis-aligned box. */
bool zAABox3DPointIsInside(const zAABox3D *box, const zVec3D *point, double margin)
{
  return point->e[zX] > box->min.e[zX] - margin && point->e[zX] <= box->max.e[zX] + margin &&
         point->e[zY] > box->min.e[zY] - margin && point->e[zY] <= box->max.e[zY] + margin &&
         point->e[zZ] > box->min.e[zZ] - margin && point->e[zZ] <= box->max.e[zZ] + margin ? true : false;
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

/* enlarge a 3D axis-aligned box. */
zAABox3D *zAABox3DEnlarge(zAABox3D *aabox, const zVec3D *v)
{
  if( zAABox3DXMin(aabox) > v->c.x ) zAABox3DXMin(aabox) = v->c.x;
  if( zAABox3DXMax(aabox) < v->c.x ) zAABox3DXMax(aabox) = v->c.x;
  if( zAABox3DYMin(aabox) > v->c.y ) zAABox3DYMin(aabox) = v->c.y;
  if( zAABox3DYMax(aabox) < v->c.y ) zAABox3DYMax(aabox) = v->c.y;
  if( zAABox3DZMin(aabox) > v->c.z ) zAABox3DZMin(aabox) = v->c.z;
  if( zAABox3DZMax(aabox) < v->c.z ) zAABox3DZMax(aabox) = v->c.z;
  return aabox;
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

/* ********************************************************** */
/* 3D box class.
 *//* ******************************************************* */

/* create a 3D box. */
zBox3D *zBox3DCreate(zBox3D *box, const zVec3D *center, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double depth, double width, double height)
{
  zVec3D ux, uy, uz;

  zBox3DSetCenter( box, center );
  zVec3DNormalize( ax, &ux ); zBox3DSetAxis( box, 0, &ux );
  zVec3DNormalize( ay, &uy ); zBox3DSetAxis( box, 1, &uy );
  zVec3DNormalize( az, &uz ); zBox3DSetAxis( box, 2, &uz );
  zBox3DSetDepth( box, fabs(depth) );
  zBox3DSetWidth( box, fabs(width) );
  zBox3DSetHeight( box, fabs(height) );
  return box;
}

/* initialize a 3D box. */
zBox3D *zBox3DInit(zBox3D *box)
{
  return zBox3DCreateAlign( box, ZVEC3DZERO, 0, 0, 0 );
}

/* allocate memory for a 3D box. */
ZDEF_ALLOC_FUNCTION( zBox3D )

/* copy a 3D box to another. */
zBox3D *zBox3DCopy(const zBox3D *src, zBox3D *dest)
{
  return zBox3DCreate( dest, zBox3DCenter(src),
    zBox3DAxis(src,0), zBox3DAxis(src,1), zBox3DAxis(src,2),
    zBox3DDepth(src), zBox3DWidth(src), zBox3DHeight(src) );
}

/* mirror a 3D box along an axis. */
zBox3D *zBox3DMirror(const zBox3D *src, zBox3D *dest, zAxis axis)
{
  zBox3DCopy( src, dest );
  zBox3DCenter(dest)->e[(int)axis] *= -1;
  zBox3DAxis(dest,0)->e[(int)axis] *= -1;
  zBox3DAxis(dest,1)->e[(int)axis] *= -1;
  zBox3DAxis(dest,2)->e[(int)axis] *= -1;
  return dest;
}

/* transform coordinates of a 3D box. */
zBox3D *zBox3DXform(const zBox3D *src, const zFrame3D *frame, zBox3D *dest)
{
  zXform3D( frame, zBox3DCenter(src), zBox3DCenter(dest) );
  zMulMat3DVec3D( zFrame3DAtt(frame), zBox3DAxis(src,zX), zBox3DAxis(dest,zX) );
  zMulMat3DVec3D( zFrame3DAtt(frame), zBox3DAxis(src,zY), zBox3DAxis(dest,zY) );
  zMulMat3DVec3D( zFrame3DAtt(frame), zBox3DAxis(src,zZ), zBox3DAxis(dest,zZ) );
  zBox3DSetDepth( dest, zBox3DDepth(src) );
  zBox3DSetWidth( dest, zBox3DWidth(src) );
  zBox3DSetHeight( dest, zBox3DHeight(src) );
  return dest;
}

/* inversely transform coordinates of a 3D box. */
zBox3D *zBox3DXformInv(const zBox3D *src, const zFrame3D *frame, zBox3D *dest)
{
  zXform3DInv( frame, zBox3DCenter(src), zBox3DCenter(dest) );
  zMulMat3DTVec3D( zFrame3DAtt(frame), zBox3DAxis(src,zX), zBox3DAxis(dest,zX) );
  zMulMat3DTVec3D( zFrame3DAtt(frame), zBox3DAxis(src,zY), zBox3DAxis(dest,zY) );
  zMulMat3DTVec3D( zFrame3DAtt(frame), zBox3DAxis(src,zZ), zBox3DAxis(dest,zZ) );
  zBox3DSetDepth( dest, zBox3DDepth(src) );
  zBox3DSetWidth( dest, zBox3DWidth(src) );
  zBox3DSetHeight( dest, zBox3DHeight(src) );
  return dest;
}

/* the closest point from a 3D point to a 3D box. */
double zBox3DClosest(const zBox3D *box, const zVec3D *p, zVec3D *cp)
{
  zVec3D _p;
  double min, max;
  zAxis axis;

  zXform3DInv( &box->frame, p, &_p );
  for( axis=zX; axis<=zZ; axis++ ){
    min =-0.5 * zBox3DSpan(box,axis);
    max = 0.5 * zBox3DSpan(box,axis);
    cp->e[(int)axis] = _zLimit( _p.e[(int)axis], min, max );
  }
  zXform3DDRC( &box->frame, cp );
  return zVec3DDist( p, cp );
}

/* distance from a point to a 3D box. */
double zBox3DDistFromPoint(const zBox3D *box, const zVec3D *p)
{
  zVec3D cp;
  return zBox3DClosest( box, p, &cp );
}

/* check if a point is inside of a box. */
bool zBox3DPointIsInside(const zBox3D *box, const zVec3D *p, double margin)
{
  zAxis axis;
  zVec3D pl;
  double l;

  zXform3DInv( &box->frame, p, &pl );
  for( axis=zX; axis<=zZ; axis++ ){
    l = 0.5 * zBox3DSpan(box,axis) + margin;
    if( pl.e[(int)axis] >= l || pl.e[(int)axis] <= -l ) return false;
  }
  return true;
}

/* volume of a 3D box. */
double zBox3DVolume(const zBox3D *box)
{
  return zBox3DDepth(box) * zBox3DWidth(box) * zBox3DHeight(box);
}

/* inertia tensor about barycenter of a 3D box from mass. */
zMat3D *zBox3DBaryInertiaMass(const zBox3D *box, double mass, zMat3D *inertia)
{
  zMat3D i;
  double xx, yy, zz, c;

  c = mass / 12;
  xx = zSqr( zBox3DDepth(box) ) * c;
  yy = zSqr( zBox3DWidth(box) ) * c;
  zz = zSqr( zBox3DHeight(box) ) * c;
  zMat3DCreate( &i,
    yy+zz, 0, 0,
    0, zz+xx, 0,
    0, 0, xx+yy );
  return zRotMat3D( zFrame3DAtt(&box->frame), &i, inertia );
}

/* inertia tensor about barycenter of a 3D box. */
zMat3D *zBox3DBaryInertia(const zBox3D *box, double density, zMat3D *inertia)
{
  return zBox3DBaryInertiaMass( box, density * zBox3DVolume( box ), inertia );
}

/* get vertex of a box. */
zVec3D *zBox3DVert(const zBox3D *box, int i, zVec3D *v)
{
  _zVec3DCreate( v,
    ( (i&0x1)^(i>>1&0x1) ) ? -0.5*zBox3DDepth(box) : 0.5*zBox3DDepth(box),
    (  i&0x2             ) ? -0.5*zBox3DWidth(box) : 0.5*zBox3DWidth(box),
    (  i&0x4             ) ? -0.5*zBox3DHeight(box): 0.5*zBox3DHeight(box) );
  return zXform3DDRC( &box->frame, v );
}

/* convert a 3D axis-aligned box to a general box. */
zBox3D *zAABox3DToBox3D(const zAABox3D *aab, zBox3D *box)
{
  zVec3DMid( &aab->max, &aab->min, zBox3DCenter(box) );
  zVec3DSub( &aab->max, &aab->min, &box->edgespan );
  zBox3DSetAxis( box, 0, ZVEC3DX );
  zBox3DSetAxis( box, 1, ZVEC3DY );
  zBox3DSetAxis( box, 2, ZVEC3DZ );
  return box;
}

/* compute a 3D axis-aligned bounding box of a 3D box. */
zAABox3D *zBox3DToAABox3D(const zBox3D *box, zAABox3D *aabox)
{
  zVec3D v;
  int i;

  zVec3DCreate( &aabox->min, HUGE_VAL, HUGE_VAL, HUGE_VAL );
  zVec3DCreate( &aabox->max,-HUGE_VAL,-HUGE_VAL,-HUGE_VAL );
  for( i=0; i<8; i++ ){
    zBox3DVert( box, i, &v );
    zAABox3DEnlarge( aabox, &v );
  }
  return aabox;
}

/* print a 3D box out to a file in a plottable format. */
void zBox3DValueFPrint(FILE *fp, const zBox3D *box)
{
  zVec3D v[8];
  int i;

  for( i=0; i<8; i++ )
    zBox3DVert( box, i, &v[i] );
  zVec3DValueNLFPrint( fp, &v[0] );
  zVec3DValueNLFPrint( fp, &v[1] );
  zVec3DValueNLFPrint( fp, &v[2] );
  zVec3DValueNLFPrint( fp, &v[3] );
  zVec3DValueNLFPrint( fp, &v[0] );
  zVec3DValueNLFPrint( fp, &v[4] );
  zVec3DValueNLFPrint( fp, &v[5] );
  zVec3DValueNLFPrint( fp, &v[6] );
  zVec3DValueNLFPrint( fp, &v[7] );
  zVec3DValueNLFPrint( fp, &v[4] );
  fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[1] );
  zVec3DValueNLFPrint( fp, &v[5] );
  fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[2] );
  zVec3DValueNLFPrint( fp, &v[6] );
  fprintf( fp, "\n" );
  zVec3DValueNLFPrint( fp, &v[3] );
  zVec3DValueNLFPrint( fp, &v[7] );
  fprintf( fp, "\n\n" );
}
