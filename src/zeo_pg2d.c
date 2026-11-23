/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_pg2d - 2D polygon.
 */

#include <zeo/zeo_pg2d.h>

/* initialize a 2D polygon. */
zPG2D *zPG2DInit(zPG2D *pg)
{
  zArrayInit( zPG2DVertArray(pg) );
  zArrayInit( zPG2DFaceArray(pg) );
  return pg;
}

/* allocate vertices of a 2D polygon. */
bool zPG2DAllocVert(zPG2D *pg, int num_vert)
{
  zArrayAlloc( zPG2DVertArray(pg), zVec2D, num_vert );
  return zPG2DVertNum(pg) == num_vert;
}

/* allocate faces of a 2D polygon. */
bool zPG2DAllocFace(zPG2D *pg, int num_face)
{
  zArrayAlloc( zPG2DFaceArray(pg), zTri2D, num_face );
  return zPG2DFaceNum(pg) == num_face;
}

/* allocate a 2D polygon. */
zPG2D *zPG2DAlloc(zPG2D *pg, int num_vert, int num_face)
{
  zPG2DInit( pg );
  if( num_vert > 0 ){ /* vertices */
    if( !zPG2DAllocVert( pg, num_vert ) ) goto ZPG2DALLOC_ERROR;
  }
  if( num_face > 0 ){ /* faces */
    if( !zPG2DAllocFace( pg, num_face ) ) goto ZPG2DALLOC_ERROR;
  }
  return pg;

 ZPG2DALLOC_ERROR:
  ZALLOCERROR();
  zPG2DDestroy( pg );
  return NULL;
}

/* destroy a 2D polygon. */
void zPG2DDestroy(zPG2D *pg)
{
  if( !pg ) return;
  zFree( zPG2DVertBuf(pg) );
  zFree( zPG2DFaceBuf(pg) );
  zPG2DInit( pg );
}

/* clone a 2D polygon. */
zPG2D *zPG2DClone(const zPG2D *src, zPG2D *dest)
{
  int i;

  if( !zPG2DAlloc( dest, zPG2DVertNum(src), zPG2DFaceNum(src) ) )
    return NULL;
  for( i=0; i<zPG2DVertNum(src); i++ )
    zVec2DCopy( zPG2DVert(src,i), zPG2DVert(dest,i) );
  for( i=0; i<zPG2DFaceNum(src); i++ )
    zTri2DCreate( zPG2DFace(dest,i),
      zPG2DVert(dest,zPG2DFaceVert(src,i,0)-zPG2DVertBuf(src)),
      zPG2DVert(dest,zPG2DFaceVert(src,i,1)-zPG2DVertBuf(src)),
      zPG2DVert(dest,zPG2DFaceVert(src,i,2)-zPG2DVertBuf(src)) );
  return dest;
}

/* mirror a 2D polygon. */
zPG2D *zPG2DMirror(const zPG2D *src, zPG2D *dest, zAxis axis)
{
  int i;

  if( axis != zX && axis != zY ){
    ZRUNERROR( ZEO_ERR_INVALID_AXIS, axis );
    return NULL;
  }
  if( !zPG2DClone( src, dest ) ) return NULL;
  for( i=0; i<zPG2DVertNum(dest); i++ )
    zPG2DVert(dest,i)->e[(int)axis] *= -1;
  for( i=0; i<zPG2DFaceNum(dest); i++ )
    zSwap( zVec2D*, zPG2DFaceVert(dest,i,1), zPG2DFaceVert(dest,i,2) );
  return dest;
}

/* directly scale a 2D polygon. */
zPG2D *zPG2DScaleDRC(zPG2D *pg, double scale)
{
  int i;

  for( i=0; i<zPG2DVertNum(pg); i++ )
    zVec2DMulDRC( zPG2DVert(pg,i), scale );
  return pg;
}

/* scale a 2D polygon. */
zPG2D *zPG2DScale(const zPG2D *src, zPG2D *dest, double scale)
{
  if( !zPG2DClone( src, dest ) ) return NULL;
  return zPG2DScaleDRC( dest, scale );
}

/* transform coordinates of vertices of a 2D polygon. */
zPG2D *zPG2DXform(const zPG2D *src, const zFrame2D *frame, zPG2D *dest)
{
  int i;

  for( i=0; i<zPG2DVertNum(dest); i++ )
    zXform2D( frame, zPG2DVert(src,i), zPG2DVert(dest,i) );
  return dest;
}

/* inversely transforms coordinates of vertices of a 2D polygon. */
zPG2D *zPG2DXformInv(const zPG2D *src, const zFrame2D *frame, zPG2D *dest)
{
  int i;

  for( i=0; i<zPG2DVertNum(dest); i++ )
    zXform2DInv( frame, zPG2DVert(src,i), zPG2DVert(dest,i) );
  return dest;
}

/* find the contiguous vertex of a 2D polygon to a point. */
const zVec2D *zPG2DContigVert(const zPG2D *pg, const zVec2D *point, double *distance)
{
  int i;
  zVec2D *v, *nv;
  double _d, dist_min;

  if( zPG2DVertNum(pg) == 0 ) return NULL;
  if( !distance ) distance = &_d;
  v = zPG2DVert(pg,0);
  dist_min = zVec2DDist( point, v );
  for( i=1; i<zPG2DVertNum(pg); i++ ){
    if( ( *distance = zVec2DDist( ( nv = zPG2DVert(pg,i) ), point ) ) < dist_min ){
      v = nv;
      dist_min = *distance;
    }
  }
  *distance = dist_min;
  return v;
}

/* the closest point to a 2D polygon. */
double zPG2DClosest(const zPG2D *pg, const zVec2D *point, zVec2D *closestpoint)
{
  int i;
  double d, dist_min = HUGE_VAL;
  zVec2D pc;

  zVec2DCopy( point, closestpoint );
  if( zPG2DFaceNum(pg) == 0 ){
    ZRUNWARN( ZEO_ERR_NOFACE );
    return 0;
  }
  for( i=0; i<zPG2DFaceNum(pg); i++ ){
    if( ( d = zTri2DClosest( zPG2DFace(pg,i), point, &pc ) ) < dist_min ){
      dist_min = d;
      zVec2DCopy( &pc, closestpoint );
    }
  }
  return dist_min;
}

/* distance from a point to a 2D polygon. */
double zPG2DDistFromPoint(const zPG2D *pg, const zVec2D *point)
{
  zVec2D cp;
  return zPG2DClosest( pg, point, &cp );
}

/* check if an edge from a to b of the i-th face of a 2D polygon is shared by another face. */
static bool _zPG2DEdgeIsShared(const zPG2D *pg, int i, const zVec2D *a, const zVec2D *b)
{
  int k, j;

  for( k=0; k<zPG2DFaceNum(pg); k++ ){
    if( k == i ) continue;
    for( j=0; j<3; j++ )
      if( ( zPG2DFaceVert(pg,k,j) == a && zTri2DVertNext(zPG2DFace(pg,k),j) == b ) ||
          ( zPG2DFaceVert(pg,k,j) == b && zTri2DVertNext(zPG2DFace(pg,k),j) == a ) ) return true;
  }
  return false;
}

/* distance from a point to the boundary of a 2D polygon, which consists of edges not shared by two
 * faces. */
static double _zPG2DBoundaryDist(const zPG2D *pg, const zVec2D *point)
{
  zEdge2D edge;
  zVec2D *a, *b;
  double d, dist_min = HUGE_VAL;
  int i, j;

  for( i=0; i<zPG2DFaceNum(pg); i++ )
    for( j=0; j<3; j++ ){
      a = zPG2DFaceVert(pg,i,j);
      b = zTri2DVertNext(zPG2DFace(pg,i),j);
      if( _zPG2DEdgeIsShared( pg, i, a, b ) ) continue;
      zEdge2DCreate( &edge, a, b );
      if( ( d = zEdge2DDistFromPoint( &edge, point ) ) < dist_min ) dist_min = d;
    }
  return dist_min;
}

/* check if a point is inside of a polygon. */
bool zPG2DPointIsInside(const zPG2D *pg, const zVec2D *point, double margin)
{
  zVec2D cp;
  double d;

  if( zPG2DFaceNum(pg) == 0 ) return false;
  /* the signed distance from the boundary, which is negative inside, is compared with the margin. */
  if( ( d = zPG2DClosest( pg, point, &cp ) ) > 0 ) return d < margin; /* outside */
  return margin > 0 || _zPG2DBoundaryDist( pg, point ) > -margin;
}

/* area of a 2D polygon. */
double zPG2DArea(const zPG2D *pg)
{
  int i;
  double a;

  for( a=0, i=0; i<zPG2DFaceNum(pg); i++ )
    a += zTri2DArea( zPG2DFace(pg,i) );
  return a;
}

/* barycenter of a 2D polygon. */
zVec2D *zPG2DBarycenter(const zPG2D *pg, zVec2D *c)
{
  int i;
  zVec2D bc;
  double s, area;

  zVec2DZero( c );
  for( area=0, i=0; i<zPG2DFaceNum(pg); i++ ){
    area += ( s = zTri2DArea( zPG2DFace(pg,i) ) );
    zTri2DBarycenter( zPG2DFace(pg,i), &bc );
    zVec2DMulDRC( &bc, s );
    zVec2DAddDRC( c, &bc );
  }
  if( area > 0 )
    zVec2DDivDRC( c, area );
  return c;
}
