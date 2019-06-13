/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv_aabb - bounding volume: axis-aligned bounding box.
 */

#include <zeo/zeo_bv.h>

/* ********************************************************** */
/* zAABox3D - axis-aligned box
 * ********************************************************** */

/* initialize an axis-aligned box */
zAABox3D *zAABox3DInit(zAABox3D *box)
{
  zVec3DClear( &box->pmin );
  zVec3DClear( &box->pmax );
  return box;
}

/* create an axis-aligned box */
zAABox3D *zAABox3DCreate(zAABox3D *box, double x1, double y1, double z1, double x2, double y2, double z2)
{
  zVec3DCreate( &box->pmin, zMin(x1,x2), zMin(y1,y2), zMin(z1,z2) );
  zVec3DCreate( &box->pmax, zMax(x1,x2), zMax(y1,y2), zMax(z1,z2) );
  return box;
}

/* copy an axis-aligned box to another. */
zAABox3D *zAABox3DCopy(zAABox3D *src, zAABox3D *dst)
{
  zVec3DCopy( &src->pmin, &dst->pmin );
  zVec3DCopy( &src->pmax, &dst->pmax );
  return dst;
}

/* merge two axis-aligned boxes. */
zAABox3D *zAABox3DMerge(zAABox3D *dst, zAABox3D *src1, zAABox3D *src2)
{
  return zAABox3DCreate( dst,
    zMin( src1->pmin.e[zX], src2->pmin.e[zX] ),
    zMin( src1->pmin.e[zY], src2->pmin.e[zY] ),
    zMin( src1->pmin.e[zZ], src2->pmin.e[zZ] ),
    zMax( src1->pmax.e[zX], src2->pmax.e[zX] ),
    zMax( src1->pmax.e[zY], src2->pmax.e[zY] ),
    zMax( src1->pmax.e[zZ], src2->pmax.e[zZ] ) );
}

/* check if a point is inside of an axis-aligned box. */
bool zAABox3DPointIsInside(zAABox3D *box, zVec3D *p, bool rim)
{
  double eps;

  eps = rim ? zTOL : 0;
  return p->e[zX] >= box->pmin.e[zX] - eps &&
         p->e[zX] <= box->pmax.e[zX] + eps &&
         p->e[zY] >= box->pmin.e[zY] - eps &&
         p->e[zY] <= box->pmax.e[zY] + eps &&
         p->e[zZ] >= box->pmin.e[zZ] - eps &&
         p->e[zZ] <= box->pmax.e[zZ] + eps ?
    true : false;
}

/* compute volume of an axis-aligned box. */
double zAABox3DVolume(zAABox3D *box)
{
  return fabs( ( box->pmax.e[zX] - box->pmin.e[zX] )
             * ( box->pmax.e[zY] - box->pmin.e[zY] )
             * ( box->pmax.e[zZ] - box->pmin.e[zZ] ) );
}

/* convert an axis-aligned box to a general box. */
zBox3D *zAABox3DToBox3D(zAABox3D *aab, zBox3D *box)
{
  zVec3DMid( &aab->pmax, &aab->pmin, zBox3DCenter(box) );
  zVec3DSub( &aab->pmax, &aab->pmin, &box->dia );
  zBox3DSetAxis( box, 0, ZVEC3DX );
  zBox3DSetAxis( box, 1, ZVEC3DY );
  zBox3DSetAxis( box, 2, ZVEC3DZ );
  return box;
}

/* compute an axis-aligned box of a 3D box. */
zAABox3D *zBox3DToAABox3D(zBox3D *box, zAABox3D *aabox)
{
  zVec3D v[8];
  register int i;

  for( i=0; i<8; i++ )
    zBox3DVert( box, i, &v[i] );
  return zAABB( aabox, v, 8, NULL );
}

/* print an axis-aligned box out to a file in a format to be plotted. */
void zAABox3DDataFPrint(FILE *fp, zAABox3D *box)
{
  double x0, y0, z0, x1, y1, z1;

  x0 = box->pmin.e[zX];
  y0 = box->pmin.e[zY];
  z0 = box->pmin.e[zZ];
  x1 = box->pmax.e[zX];
  y1 = box->pmax.e[zY];
  z1 = box->pmax.e[zZ];
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
/* AABB - axis-aligned bounding box
 * ********************************************************** */

static int _zAABBTest(zAABox3D *bb, zVec3D *p, zDir u);

/* (static)
 * enlarge bounding box if a point is out of the current box. */
int _zAABBTest(zAABox3D *bb, zVec3D *p, zDir u)
{
  if( p->e[u] > bb->pmax.e[u] ){
    bb->pmax.e[u] = p->e[u];
    return 0;
  }
  if( p->e[u] < bb->pmin.e[u] ){
    bb->pmin.e[u] = p->e[u];
    return 3;
  }
  return -1;
}

/* *** array version *** */

static void _zAABBIncElem(zAABox3D *bb, zVec3D *p, zDir u, zVec3D **vp);
static zAABox3D *_zAABBInc(zAABox3D *bb, zVec3D *p, zVec3D **vp);

/* (static)
 * enlarge bounding box along each axis if the point is outside of the box. */
void _zAABBIncElem(zAABox3D *bb, zVec3D *p, zDir u, zVec3D **vp)
{
  int s;

  if( ( s = _zAABBTest( bb, p, u ) ) != -1 && vp )
    vp[u+s] = p;
}

/* (static)
 * enlarge bounding box if the point is outside of the box. */
zAABox3D *_zAABBInc(zAABox3D *bb, zVec3D *p, zVec3D **vp)
{
  _zAABBIncElem( bb, p, zX, vp );
  _zAABBIncElem( bb, p, zY, vp );
  _zAABBIncElem( bb, p, zZ, vp );
  return bb;
}

/* bounding box of points. */
zAABox3D *zAABB(zAABox3D *bb, zVec3D p[], int num, zVec3D **vp)
{
  register int i;

  zAABox3DInit( bb );
  if( num <= 0 ) return NULL;

  zVec3DCopy( &p[0], &bb->pmin );
  zVec3DCopy( &p[0], &bb->pmax );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = vp[4] = vp[5] = &p[0];
  for( i=1; i<num; i++ )
    _zAABBInc( bb, &p[i], vp );
  return bb;
}

/* bounding box of points with a transfer matrix. */
zAABox3D *zAABBXfer(zAABox3D *bb, zVec3D p[], int num, zFrame3D *f)
{
  register int i;
  zVec3D px;

  zAABox3DInit( bb );
  if( num <= 0 ) return NULL;

  zXfer3D( f, &p[0], &px );
  zVec3DCopy( &px, &bb->pmin );
  zVec3DCopy( &px, &bb->pmax );
  for( i=1; i<num; i++ ){
    zXfer3D( f, &p[i], &px );
    _zAABBInc( bb, &px, NULL );
  }
  return bb;
}

/* *** list version *** */

static void _zAABBPLIncElem(zAABox3D *bb, zVec3DListCell *p, zDir u, zVec3DListCell **vp);
static zAABox3D *_zAABBPLInc(zAABox3D *bb, zVec3DListCell *p, zVec3DListCell **vp);

/* (static)
 * enlarge bounding box along each axis if the point is outside of the box. */
void _zAABBPLIncElem(zAABox3D *bb, zVec3DListCell *p, zDir u, zVec3DListCell **vp)
{
  int s;

  if( ( s = _zAABBTest( bb, p->data, u ) ) != -1 && vp )
    vp[u+s] = p;
}

/* (static)
 * enlarge bounding box if the point is outside of the box. */
zAABox3D *_zAABBPLInc(zAABox3D *bb, zVec3DListCell *p, zVec3DListCell **vp)
{
  _zAABBPLIncElem( bb, p, zX, vp );
  _zAABBPLIncElem( bb, p, zY, vp );
  _zAABBPLIncElem( bb, p, zZ, vp );
  return bb;
}

/* bounding box of a list of points. */
zAABox3D *zAABBPL(zAABox3D *bb, zVec3DList *pl, zVec3DListCell **vp)
{
  zVec3DListCell *pc;

  zAABox3DInit( bb );
  if( zListIsEmpty(pl) ) return NULL;

  pc = zListTail( pl );
  memcpy( &bb->pmin, &pc->data, sizeof(zVec3D) );
  memcpy( &bb->pmax, &pc->data, sizeof(zVec3D) );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = vp[4] = vp[5] = pc;
  zListForEach( pl, pc )
    _zAABBPLInc( bb, pc, vp );
  return bb;
}

/* bounding box of a list of points with a transfer matrix. */
zAABox3D *zAABBXferPL(zAABox3D *bb, zVec3DList *pl, zFrame3D *f)
{
  zVec3D px;
  zVec3DListCell *pc;

  zAABox3DInit( bb );
  if( zListIsEmpty(pl) ) return NULL;

  pc = zListTail( pl );
  zXfer3D( f, pc->data, &px );
  memcpy( &bb->pmin, pc->data, sizeof(zVec3D) );
  memcpy( &bb->pmax, pc->data, sizeof(zVec3D) );
  zListForEach( pl, pc ){
    zXfer3D( f, pc->data, &px );
    _zAABBInc( bb, &px, NULL );
  }
  return bb;
}
