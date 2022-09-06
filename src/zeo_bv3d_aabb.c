/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_aabb - 3D bounding volume: axis-aligned bounding box.
 */

#include <zeo/zeo_bv3d.h>

/* ********************************************************** */
/* zAABox3D - 3D axis-aligned box
 * ********************************************************** */

/* initialize an 3D axis-aligned box */
zAABox3D *zAABox3DInit(zAABox3D *box)
{
  zVec3DZero( &box->min );
  zVec3DZero( &box->max );
  return box;
}

/* create an 3D axis-aligned box */
zAABox3D *zAABox3DCreate(zAABox3D *box, double x1, double y1, double z1, double x2, double y2, double z2)
{
  zVec3DCreate( &box->min, _zMin(x1,x2), _zMin(y1,y2), _zMin(z1,z2) );
  zVec3DCreate( &box->max, _zMax(x1,x2), _zMax(y1,y2), _zMax(z1,z2) );
  return box;
}

/* copy an 3D axis-aligned box to another. */
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

/* check if a point is inside of an 3D axis-aligned box. */
bool zAABox3DPointIsInside(zAABox3D *box, zVec3D *p, bool rim)
{
  double eps;

  eps = rim ? zTOL : 0;
  return p->e[zX] >= box->min.e[zX] - eps && p->e[zX] <= box->max.e[zX] + eps &&
         p->e[zY] >= box->min.e[zY] - eps && p->e[zY] <= box->max.e[zY] + eps &&
         p->e[zZ] >= box->min.e[zZ] - eps && p->e[zZ] <= box->max.e[zZ] + eps ?
    true : false;
}

/* compute volume of an 3D axis-aligned box. */
double zAABox3DVolume(zAABox3D *box)
{
  return fabs( ( box->max.e[zX] - box->min.e[zX] )
             * ( box->max.e[zY] - box->min.e[zY] )
             * ( box->max.e[zZ] - box->min.e[zZ] ) );
}

/* convert an 3D axis-aligned box to a general box. */
zBox3D *zAABox3DToBox3D(zAABox3D *aab, zBox3D *box)
{
  zVec3DMid( &aab->max, &aab->min, zBox3DCenter(box) );
  zVec3DSub( &aab->max, &aab->min, &box->dia );
  zBox3DSetAxis( box, 0, ZVEC3DX );
  zBox3DSetAxis( box, 1, ZVEC3DY );
  zBox3DSetAxis( box, 2, ZVEC3DZ );
  return box;
}

/* compute an 3D axis-aligned box of a 3D box. */
zAABox3D *zBox3DToAABox3D(zBox3D *box, zAABox3D *aabox)
{
  zVec3D v[8];
  int i;

  for( i=0; i<8; i++ )
    zBox3DVert( box, i, &v[i] );
  return zAABB3D( aabox, v, 8, NULL );
}

/* print an 3D axis-aligned box out to a file in a format to be plotted. */
void zAABox3DDataFPrint(FILE *fp, zAABox3D *box)
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

/* *** array version *** */

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
zAABox3D *zAABB3D(zAABox3D *bb, zVec3D p[], int num, zVec3D **vp)
{
  int i;

  zAABox3DInit( bb );
  if( num <= 0 ) return NULL;

  zVec3DCopy( &p[0], &bb->min );
  zVec3DCopy( &p[0], &bb->max );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = vp[4] = vp[5] = &p[0];
  for( i=1; i<num; i++ )
    _zAABB3DInc( bb, &p[i], vp );
  return bb;
}

/* bounding box of 3D points in a specified frame. */
zAABox3D *zAABB3DXform(zAABox3D *bb, zVec3D p[], int num, zFrame3D *f)
{
  int i;
  zVec3D px;

  zAABox3DInit( bb );
  if( num <= 0 ) return NULL;

  zXform3D( f, &p[0], &px );
  zVec3DCopy( &px, &bb->min );
  zVec3DCopy( &px, &bb->max );
  for( i=1; i<num; i++ ){
    zXform3D( f, &p[i], &px );
    _zAABB3DInc( bb, &px, NULL );
  }
  return bb;
}

/* *** list version *** */

/* enlarge bounding box if a 3D point is outside of the box. */
static zAABox3D *_zAABB3DPLInc(zAABox3D *bb, zVec3DListCell *pc, zVec3DListCell **vp)
{
  int s;

  if( ( s = _zAABB3DTest( bb, pc->data, zX ) ) != -1 && vp ) vp[zX+s] = pc;
  if( ( s = _zAABB3DTest( bb, pc->data, zY ) ) != -1 && vp ) vp[zY+s] = pc;
  if( ( s = _zAABB3DTest( bb, pc->data, zZ ) ) != -1 && vp ) vp[zZ+s] = pc;
  return bb;
}

/* bounding box of a list of 3D points. */
zAABox3D *zAABB3DPL(zAABox3D *bb, zVec3DList *pl, zVec3DListCell **vp)
{
  zVec3DListCell *pc;

  zAABox3DInit( bb );
  if( zListIsEmpty(pl) ) return NULL;

  pc = zListTail( pl );
  zVec3DCopy( pc->data, &bb->min );
  zVec3DCopy( pc->data, &bb->max );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = vp[4] = vp[5] = pc;
  zListForEach( pl, pc )
    _zAABB3DPLInc( bb, pc, vp );
  return bb;
}

/* bounding box of a list of 3D points in a specified frame. */
zAABox3D *zAABB3DXformPL(zAABox3D *bb, zVec3DList *pl, zFrame3D *f)
{
  zVec3D px;
  zVec3DListCell *pc;

  zAABox3DInit( bb );
  if( zListIsEmpty(pl) ) return NULL;

  pc = zListTail( pl );
  zXform3D( f, pc->data, &px );
  zVec3DCopy( &px, &bb->min );
  zVec3DCopy( &px, &bb->max );
  zListForEach( pl, pc ){
    zXform3D( f, pc->data, &px );
    _zAABB3DInc( bb, &px, NULL );
  }
  return bb;
}
