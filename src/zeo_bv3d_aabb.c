/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_aabb - 3D bounding volume: axis-aligned bounding box.
 */

#include <zeo/zeo_bv3d.h>

/* ********************************************************** */
/* zAABox3D - 3D axis-aligned box
 * ********************************************************** */

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
zAABox3D *zAABox3DCopy(zAABox3D *src, zAABox3D *dst)
{
  zVec3DCopy( &src->min, &dst->min );
  zVec3DCopy( &src->max, &dst->max );
  return dst;
}

/* merge two axis-aligned boxes. */
zAABox3D *zAABox3DMerge(zAABox3D *dst, zAABox3D *src1, zAABox3D *src2)
{
  return zAABox3DCreate( dst,
    _zMin( src1->min.e[zX], src2->min.e[zX] ),
    _zMin( src1->min.e[zY], src2->min.e[zY] ),
    _zMin( src1->min.e[zZ], src2->min.e[zZ] ),
    _zMax( src1->max.e[zX], src2->max.e[zX] ),
    _zMax( src1->max.e[zY], src2->max.e[zY] ),
    _zMax( src1->max.e[zZ], src2->max.e[zZ] ) );
}

/* check if a point is inside of a 3D axis-aligned box. */
bool zAABox3DPointIsInside(zAABox3D *box, zVec3D *p, double margin)
{
  return p->e[zX] > box->min.e[zX] - margin && p->e[zX] <= box->max.e[zX] + margin &&
         p->e[zY] > box->min.e[zY] - margin && p->e[zY] <= box->max.e[zY] + margin &&
         p->e[zZ] > box->min.e[zZ] - margin && p->e[zZ] <= box->max.e[zZ] + margin ? true : false;
}

/* compute volume of a 3D axis-aligned box. */
double zAABox3DVolume(zAABox3D *box)
{
  return fabs( ( box->max.e[zX] - box->min.e[zX] )
             * ( box->max.e[zY] - box->min.e[zY] )
             * ( box->max.e[zZ] - box->min.e[zZ] ) );
}

/* convert a 3D axis-aligned box to a general box. */
zBox3D *zAABox3DToBox3D(zAABox3D *aab, zBox3D *box)
{
  zVec3DMid( &aab->max, &aab->min, zBox3DCenter(box) );
  zVec3DSub( &aab->max, &aab->min, &box->dia );
  zBox3DSetAxis( box, 0, ZVEC3DX );
  zBox3DSetAxis( box, 1, ZVEC3DY );
  zBox3DSetAxis( box, 2, ZVEC3DZ );
  return box;
}

/* compute a 3D axis-aligned box of a 3D box. */
zAABox3D *zBox3DToAABox3D(zBox3D *box, zAABox3D *aabox)
{
  zVec3D v[8];
  zVec3DData data;
  int i;

  for( i=0; i<8; i++ )
    zBox3DVert( box, i, &v[i] );
  zVec3DDataAssignArrayDirect( &data, v, 8 );
  return zVec3DDataAABB( &data, aabox, NULL );
}

/* print out a 3D axis-aligned box to a file in a format to be plotted. */
void zAABox3DValueFPrint(FILE *fp, zAABox3D *box)
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
/* AABB3D - 3D axis-aligned bounding box
 * ********************************************************** */

/* enlarge bounding box if a 3D point is out of the current box. */
static int _zAABB3DTest(zAABox3D *bb, zVec3D *p, zDir u)
{
  if( p->e[(int)u] > bb->max.e[(int)u] ){
    bb->max.e[(int)u] = p->e[(int)u];
    return 0;
  }
  if( p->e[(int)u] < bb->min.e[(int)u] ){
    bb->min.e[(int)u] = p->e[(int)u];
    return 3;
  }
  return -1;
}

/* enlarge bounding box if a 3D point is outside of the box. */
static zAABox3D *_zAABB3DInc(zAABox3D *bb, zVec3D *p, zVec3D **vp)
{
  int s;

  if( ( s = _zAABB3DTest( bb, p, zX ) ) != -1 && vp ) vp[zX+s] = p;
  if( ( s = _zAABB3DTest( bb, p, zY ) ) != -1 && vp ) vp[zY+s] = p;
  if( ( s = _zAABB3DTest( bb, p, zZ ) ) != -1 && vp ) vp[zZ+s] = p;
  return bb;
}

/* bounding box of 3D points. */
zAABox3D *zVec3DDataAABB(zVec3DData *data, zAABox3D *bb, zVec3D **vp)
{
  zVec3D *v;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zAABox3DInit( bb );
  zVec3DDataRewind( data );
  v = zVec3DDataFetch( data );
  zVec3DCopy( v, &bb->min );
  zVec3DCopy( v, &bb->max );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = vp[4] = vp[5] = v;
  while( ( v = zVec3DDataFetch( data ) ) ){
    _zAABB3DInc( bb, v, vp );
  }
  return bb;
}

/* bounding box of 3D points in a specified frame. */
zAABox3D *zVec3DDataAABBXform(zVec3DData *data, zAABox3D *bb, zFrame3D *frame)
{
  zVec3D *v, px;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zAABox3DInit( bb );
  zVec3DDataRewind( data );
  v = zVec3DDataFetch( data );
  zXform3D( frame, v, &px );
  zVec3DCopy( &px, &bb->min );
  zVec3DCopy( &px, &bb->max );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zXform3D( frame, v, &px );
    _zAABB3DInc( bb, &px, NULL );
  }
  return bb;
}
