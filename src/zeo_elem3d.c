/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d - 3D shape elements.
 */

#include <zeo/zeo_elem3d.h>
#include <zeo/zeo_mat2d.h>

/* ********************************************************** */
/* 3D line class.
 * ********************************************************** */

/* initialize a 3D line. */
ZEO_ELEM_LINEXD_INIT( 3D );

/* create a 3D line from a point and a direction vector. */
ZEO_ELEM_LINEXD_CREATE( 3D )

/* create a 3D line from two passing points. */
ZEO_ELEM_LINEXD_CREATE_TWOPOINT( 3D )

/* a 3D point on a line. */
ZEO_ELEM_LINEXD_POINT( 3D )

/* projection of a 3D point onto a line. */
ZEO_ELEM_LINEXD_PROJ_POINT( 3D )

/* distance from a 3D point to a line. */
ZEO_ELEM_LINEXD_DIST_FROM_POINT( 3D )

/* check if a 3D point is on a line. */
ZEO_ELEM_LINEXD_POINT_IS_ON( 3D )

/* find two end points of the common perpendicular of two 3D lines. */
void zLine3DCommonPerpEnd(const zLine3D *line1, const zLine3D *line2, zVec3D *p1, zVec3D *p2)
{
  double r11, r12, r22, det;
  zVec3D dp, d1, d2;

  r11 = zVec3DInnerProd( zLine3DDir(line1), zLine3DDir(line1) );
  r12 = zVec3DInnerProd( zLine3DDir(line1), zLine3DDir(line2) );
  r22 = zVec3DInnerProd( zLine3DDir(line2), zLine3DDir(line2) );
  if( zIsTiny( ( det = r11 * r22 - r12 * r12 ) ) ){ /* parallel case */
    ZRUNWARN( ZEO_WARN_LINE3D_NOT_UNIQUE_COMMONPERP );
    zVec3DCopy( zLine3DOrg(line1), p1 );
    zLine3DProjPoint( line2, p1, p2 );
  } else{
    zVec3DSub( zLine3DOrg(line2), zLine3DOrg(line1), &dp );
    zVec3DMul( zLine3DDir(line1), r22, &d1 );
    zVec3DCatDRC( &d1, -r12, zLine3DDir(line2) );
    zVec3DMul( zLine3DDir(line1), r12, &d2 );
    zVec3DCatDRC( &d2, -r11, zLine3DDir(line2) );
    zLine3DPoint( line1, zVec3DInnerProd( &d1, &dp ) / det, p1 );
    zLine3DPoint( line2, zVec3DInnerProd( &d2, &dp ) / det, p2 );
  }
}

/* create the common perpendicular of two 3D lines. */
zLine3D *zLine3DCommonPerp(const zLine3D *line1, const zLine3D *line2, zVec3D *origin, zLine3D *commonperp)
{
  zVec3D p2;

  zLine3DCommonPerpEnd( line1, line2, origin, &p2 );
  return zLine3DCreateTwoPoints( commonperp, origin, &p2 );
}

/* distance between two 3D lines. */
double zDistLine3DLine3D(const zLine3D *line1, const zLine3D *line2)
{
  zVec3D p1, p2;

  zLine3DCommonPerpEnd( line1, line2, &p1, &p2 );
  return zVec3DDist( &p1, &p2 );
}

/* ********************************************************** */
/* 3D edge class
 * ********************************************************** */

/* initialize a 3D edge. */
ZEO_ELEM_EDGEXD_INIT( 3D )

/* create a 3D edge. */
ZEO_ELEM_EDGEXD_CREATE( 3D )

/* path vector of a 3D edge. */
ZEO_ELEM_EDGEXD_CALC_VEC( 3D )

/* a set of unit edge direction vector and displacement vector. */
_ZEO_ELEM_EDGEXD_PROJ_SET( 3D )

/* project a point to a 3D edge. */
ZEO_ELEM_EDGEXD_PROJ_POINT( 3D )

/* distance between a point and a 3D edge. */
ZEO_ELEM_EDGEXD_DIST_FROM_POINT( 3D )

/* check if a point is on the identical line with a 3D edge. */
ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE( 3D )

/* check if a point is on the edge (to be debugged). */
ZEO_ELEM_EDGEXD_POINT_IS_ON( 3D )

/* the closest point from the origin to a 3D edge, and find coefficents of linear sum. */
ZEO_ELEM_EDGEXD_CLOSEST_FROM_ORIGIN( 3D )

/* the closest point from point to a 3D edge. */
ZEO_ELEM_EDGEXD_CLOSEST( 3D )

/* contiguous vertix of a 3D edge to a point. */
ZEO_ELEM_EDGEXD_CONTIG_VERT( 3D )

/* print information of a 3D edge to a file. */
ZEO_ELEM_EDGEXD_FPRINT( 3D )

/* TO BE REMOVED. */
/* compute a pair of linear scale factors of a point on a 3D edge.
 * if the point is on the edge, p = l0*e->v0 + l1*e->v1. Otherwise, the result does not make sense. */
_ZEO_ELEM_EDGEXD_LINSCALE( 3D )

/* ********************************************************** */
/* 3D plane class
 * ********************************************************** */

/* create a plane. */
zPlane3D* zPlane3DCreate(zPlane3D *plane, const zVec3D *origin, const zVec3D *norm)
{
  if( zVec3DIsTiny( norm ) ){
    ZRUNERROR( ZEO_ERR_ELEM_DEG_PLANE );
    return NULL;
  }
  zPlane3DSetOrg( plane, (zVec3D*)origin );
  zPlane3DSetNorm( plane, norm );
  return plane;
}

/* signed distance between a point and a plane. */
double zPlane3DDistFromPoint(const zPlane3D *plane, const zVec3D *point)
{
  zVec3D tmp;

  zVec3DSub( point, zPlane3DOrg(plane), &tmp );
  return zVec3DInnerProd( &tmp, zPlane3DNorm(plane) );
}

/* check if a 3D point is on a 3D plane. */
bool zPlane3DPointIsOn(const zPlane3D *plane, const zVec3D *point, double margin)
{
  zVec3D tmp;

  zVec3DSub( point, zPlane3DOrg(plane), &tmp );
  if( zVec3DIsTiny( &tmp ) ) return true;
  zVec3DNormalizeDRC( &tmp );
  return zIsTol( zVec3DInnerProd( &tmp, zPlane3DNorm(plane) ), margin );
}

/* project a 3D point onto a 3D plane. */
double zPlane3DProjPoint(const zPlane3D *plane, const zVec3D *p, zVec3D *projection)
{
  double d;

  d = zPlane3DDistFromPoint( plane, p );
  zVec3DCat( p, -d, zPlane3DNorm(plane), projection );
  return d;
}

/* print information of a plane to a file. */
void zPlane3DFPrint(FILE *fp, const zPlane3D *plane)
{
  fprintf( fp, "origin: " );
  zVec3DFPrint( fp, zPlane3DOrg(plane) );
  fprintf( fp, "norm: " );
  zVec3DFPrint( fp, zPlane3DNorm(plane) );
}

/* ********************************************************** */
/* 3D triangle class
 * ********************************************************** */

/* initialize a 3D triangle. */
zTri3D *zTri3DInit(zTri3D *tri)
{
  zTri3DSetVert( tri, 0, NULL );
  zTri3DSetVert( tri, 1, NULL );
  zTri3DSetVert( tri, 2, NULL );
  zTri3DSetNorm( tri, ZVEC3DZERO );
  return tri;
}

/* create a 3D triangle. */
zTri3D *zTri3DCreate(zTri3D *tri, const zVec3D *v1, const zVec3D *v2, const zVec3D *v3)
{
  zTri3DSetVert( tri, 0, (zVec3D*)v1 );
  zTri3DSetVert( tri, 1, (zVec3D*)v2 );
  zTri3DSetVert( tri, 2, (zVec3D*)v3 );
  if( v1 && v2 && v3 )
    if( !zTri3DCalcNorm(tri) ) return NULL;
  return tri;
}

/* create a 3D triangle in the flipped order. */
zTri3D *zTri3DCreateFlip(zTri3D *tri, const zVec3D *v1, const zVec3D *v2, const zVec3D *v3)
{
  return zTri3DCreate( tri, v1, v3, v2 );
}

/* flip a 3D triangle. */
zTri3D *zTri3DFlip(const zTri3D *src, zTri3D *dest)
{
  return zTri3DCreate( dest, zTri3DVert(src,0), zTri3DVert(src,2), zTri3DVert(src,1) );
}

/* outer product of two edges of a 3D triangle. */
static zVec3D *_zTri3DOuterProd(const zTri3D *tri, zVec3D *n)
{
  zVec3D edge1, edge2;

  zVec3DSub( zTri3DVert(tri,1), zTri3DVert(tri,0), &edge1 );
  zVec3DSub( zTri3DVert(tri,2), zTri3DVert(tri,1), &edge2 );
  return zVec3DOuterProd( &edge1, &edge2, n );
}

/* area of a 3D triangle. */
double zTri3DArea(const zTri3D *tri)
{
  zVec3D norm;

  _zTri3DOuterProd( tri, &norm );
  return 0.5 * zVec3DNorm( &norm );
}

/* normal vector of a 3D triangle. */
zVec3D *zTri3DCalcNorm(zTri3D *tri)
{
  _zTri3DOuterProd( tri, zTri3DNorm(tri) );
  if( zVec3DIsTiny( zTri3DNorm(tri) ) ) return NULL;
  zVec3DNormalizeDRC( zTri3DNorm(tri) );
  return zTri3DNorm(tri);
}

/* barycenter of a 3D triangle. */
ZEO_ELEM_TRIXD_BARYCENTER( 3D )

#define _zTri3DWeightedCenter(tri,w1,w2,w3,p) do{\
  double __d;\
  __d = (w1) + (w2) + (w3);\
  (p)->c.x = ( (w1)*zTri3DVert(tri,0)->c.x + (w2)*zTri3DVert(tri,1)->c.x + (w3)*zTri3DVert(tri,2)->c.x ) / __d;\
  (p)->c.y = ( (w1)*zTri3DVert(tri,0)->c.y + (w2)*zTri3DVert(tri,1)->c.y + (w3)*zTri3DVert(tri,2)->c.y ) / __d;\
  (p)->c.z = ( (w1)*zTri3DVert(tri,0)->c.z + (w2)*zTri3DVert(tri,1)->c.z + (w3)*zTri3DVert(tri,2)->c.z ) / __d;\
} while(0)

/* circumcenter of a 3D triangle */
ZEO_ELEM_TRIXD_CIRCUMCENTER( 3D )

/* incenter of a 3D triangle */
ZEO_ELEM_TRIXD_INCENTER( 3D )

/* orthocenter of a 3D triangle */
ZEO_ELEM_TRIXD_ORTHOCENTER( 3D )

/* contiguous vertix of a 3D triangle to a 3D point. */
ZEO_ELEM_TRIXD_CONTIG_VERT( 3D )

/* signed distance from a 3D point to the identical plane of a 3D triangle. */
double zTri3DDistFromPointToPlane(const zTri3D *tri, const zVec3D *point)
{
  zVec3D tmp;
  double d[3];
  int i;

  for( i=0; i<3; i++ ){
    zVec3DSub( point, zTri3DVert(tri,i), &tmp );
    d[i] = zVec3DInnerProd( &tmp, zTri3DNorm(tri) );
  }
  return ( d[0] + d[1] + d[2] ) / 3;
}

/* project a 3D point to a 3D triangle. */
double zTri3DProjPoint(const zTri3D *tri, const zVec3D *point, zVec3D *projection)
{
  double d;

  d = zTri3DDistFromPointToPlane( tri, point );
  zVec3DCat( point, -d, zTri3DNorm(tri), projection );
  return d;
}

/* check if a 3D point is on the identical plane of a 3D triangle. */
bool zTri3DPointIsOnPlane(const zTri3D *tri, const zVec3D *point, double margin)
{
  zVec3D tmp;

  zVec3DSub( point, zTri3DVert(tri,0), &tmp );
  if( zVec3DIsTiny( &tmp ) ) return true;
  /* ARGUMENT: Is this normalization necessary? */
  zVec3DNormalizeDRC( &tmp );
  return zIsTol( zVec3DInnerProd( &tmp, zTri3DNorm(tri) ), margin );
}

/* check if the projection of a 3D point to a 3D triangle is inside of the triangle (without margin). */
static bool _zTri3DProjIsInside(const zTri3D *tri, const zVec3D *point, zVec3D *projection)
{
  int i;
  zVec3D edge, n, d;

  zTri3DProjPoint( tri, point, projection );
  for( i=0; i<3; i++ ){
    zVec3DSub( zTri3DVertNext(tri,i), zTri3DVert(tri,i), &edge );
    zVec3DOuterProd( &edge, zTri3DNorm(tri), &n );
    zVec3DNormalizeDRC( &n );
    zVec3DSub( projection, zTri3DVert(tri,i), &d );
    if( zVec3DInnerProd( &n, &d ) > 0 ) return false;
  }
  return true;
}

/* test if the closest point on a triangle composed by three given 3D points to the origin is on an edge. */
static bool _zTri3DClosestFromOrigin_TestEdge(const zVec3D *vert0, const zVec3D *vert1, double *s0, double *s1, double *s2, zVec3D *closestpoint, double *dist_min)
{
  double dist;
  zVec3D cp;
  if( ( dist = zEdge3DClosestFromOrigin( vert0, vert1, s0, s1, &cp ) ) < *dist_min ){
    *s2 = 0;
    *dist_min = dist;
    zVec3DCopy( &cp, closestpoint );
    return true;
  }
  return false;
}

/* closest point on a triangle composed by three given 3D points to the origin. */
double zTri3DClosestFromOrigin(const zVec3D *vert0, const zVec3D *vert1, const zVec3D *vert2, double *s0, double *s1, double *s2, zVec3D *closestpoint)
{
  zTri3D tri;
  double inv_det, dist_min = HUGE_VAL;
  zVec3D s, t1, t2;
  zVec2D v[3];
  int i;

  if( zTri3DCreate( &tri, vert0, vert1, vert2 ) &&
      _zTri3DProjIsInside( &tri, ZVEC3DZERO, closestpoint ) ){
    zVec3DOrthoSpace( zTri3DNorm(&tri), &t1, &t2 );
    for( i=0; i<3; i++ ){
      zVec3DSub( zTri3DVert(&tri,i), closestpoint, &s );
      zVec2DCreate( &v[i], zVec3DInnerProd( &t1, &s ), zVec3DInnerProd( &t2, &s ) );
    }
    *s0 = _zVec2DOuterProd( &v[1], &v[2] );
    *s1 = _zVec2DOuterProd( &v[2], &v[0] );
    *s2 = _zVec2DOuterProd( &v[0], &v[1] );
    inv_det = 1.0 / ( *s0 + *s1 + *s2 );
    *s0 *= inv_det;
    *s1 *= inv_det;
    *s2 *= inv_det;
    return zVec3DNorm( closestpoint );
  }
  for( i=0; i<3; i++ )
    if( _zTri3DClosestFromOrigin_TestEdge( zTri3DVert(&tri,i), zTri3DVertNext(&tri,i), &s.e[i], &s.e[(i+1)%3], &s.e[(i+2)%3], closestpoint, &dist_min ) ){
      *s0 = s.e[0]; *s1 = s.e[1]; *s2 = s.e[2];
    }
  return dist_min;
}

/* closest point to a 3D triangle. */
double zTri3DClosest(const zTri3D *tri, const zVec3D *point, zVec3D *closestpoint)
{
  zVec3D v[3];
  double dist, s[3];

  zVec3DSub( zTri3DVert(tri,0), point, &v[0] );
  zVec3DSub( zTri3DVert(tri,1), point, &v[1] );
  zVec3DSub( zTri3DVert(tri,2), point, &v[2] );
  dist = zTri3DClosestFromOrigin( &v[0], &v[1], &v[2], &s[0], &s[1], &s[2], closestpoint );
  zVec3DAddDRC( closestpoint, point );
  return dist;
}

/* signed closest point to a 3D triangle. */
double zTri3DSignedClosest(const zTri3D *tri, const zVec3D *point, zVec3D *closestpoint)
{
  double dist;

  dist = zTri3DClosest( tri, point, closestpoint );
  return zTri3DDistFromPointToPlane( tri, point ) >= 0 ? dist : -dist;
}

/* signed distance from a 3D point to a 3D triangle. */
double zTri3DSignedDistFromPoint(const zTri3D *tri, const zVec3D *point)
{
  zVec3D cp;
  return zTri3DSignedClosest( tri, point, &cp );
}

/* compute a trio of linear scale factors of a point on a 3D triangle.
 * when p is on t, p = l0*t->v0 + l1*t->v1 + l2*t->v2. If p is not on t, the result does not make sense. */

static void _zTri3DLinScaleVal(const zVec2D *l, double d1, double d2, double *l0, double *l1, double *l2)
{
  *l0 = 1 - ( ( *l1 = l->c.x / d1 ) + ( *l2 = l->c.y / d2 ) );
}

static void _zTri3DLinScaleDeg2(const zTri3D *tri, const zVec3D *p, int i1, int i2, double *l0, double *l1, double *l2, zVec3D *cp)
{
  zEdge3D edge;
  zVec3D pp;

  *l0 = 0;
  zEdge3DCreate( &edge, zTri3DVert(tri,i1), zTri3DVert(tri,i2) );
  zEdge3DProjPoint( &edge, p, &pp );
  _zEdge3DLinScale( &edge, &pp, l1, l2, cp );
}

static void _zTri3DLinScaleDeg1(const zTri3D *tri, int i, double *l0, double *l1, double *l2, zVec3D *cp)
{
  *l0 = *l1 = 0; *l2 = 1;
  zVec3DCopy( zTri3DVert(tri,i), cp );
}

double zTri3DLinScale(const zTri3D *tri, const zVec3D *p, double *l0, double *l1, double *l2, zVec3D *cp)
{
  zVec3D e1, e2, dp;
  zVec2D v, l;
  zMat2D m;
  double d0, d1, d2, mc;
  int i0, i1, i2;

  d0 = zVec3DSqrDist( p, zTri3DVert(tri,0) );
  d1 = zVec3DSqrDist( p, zTri3DVert(tri,1) );
  d2 = zVec3DSqrDist( p, zTri3DVert(tri,2) );
  i0 = ( d0 <= d1 ) ? ( ( d0 <= d2 ) ? 0 : 2 ) : ( ( d1 <= d2 ) ? 1 : 2 );
  i1 = ( i0 + 1 ) % 3;
  i2 = ( i0 + 2 ) % 3;

  zVec3DSub( zTri3DVert(tri,i1), zTri3DVert(tri,i0), &e1 );
  d1 = zVec3DNormalizeDRC( &e1 );
  zVec3DSub( zTri3DVert(tri,i2), zTri3DVert(tri,i0), &e2 );
  d2 = zVec3DNormalizeDRC( &e2 );
  zVec3DSub( p, zTri3DVert(tri,i0), &dp );
  zVec2DCreate( &v, zVec3DInnerProd(&e1,&dp), zVec3DInnerProd(&e2,&dp) );
  mc = zVec3DInnerProd(&e1,&e2);
  zMat2DCreate( &m, 1.0, mc, mc, 1.0 );
  zMulInvMat2DVec2D( &m, &v, &l );
  switch( i0 ){
  case 0: _zTri3DLinScaleVal( &l, d1, d2, l0, l1, l2 ); break;
  case 1: _zTri3DLinScaleVal( &l, d1, d2, l1, l2, l0 ); break;
  case 2: _zTri3DLinScaleVal( &l, d1, d2, l2, l0, l1 ); break;
  default: ;
  }
  switch( ( *l0 < 0 ? 1 : 0 ) | ( *l1 < 0 ? 2 : 0 ) | ( *l2 < 0 ? 4 : 0 ) ){
  case 0x0: zVec3DCopy( p, cp ); return 0;
  case 0x1: _zTri3DLinScaleDeg2( tri, p, 1, 2, l0, l1, l2, cp ); break;
  case 0x2: _zTri3DLinScaleDeg2( tri, p, 2, 0, l1, l2, l0, cp ); break;
  case 0x3: _zTri3DLinScaleDeg1( tri, 2, l0, l1, l2, cp );       break;
  case 0x4: _zTri3DLinScaleDeg2( tri, p, 0, 1, l2, l0, l1, cp ); break;
  case 0x5: _zTri3DLinScaleDeg1( tri, 1, l2, l0, l1, cp );       break;
  case 0x6: _zTri3DLinScaleDeg1( tri, 0, l1, l2, l0, cp );       break;
  case 0x7: ZRUNERROR( ZEO_ERR_FATAL ); break;
  }
  return zVec3DDist( p, cp );
}

/* distance from a 3D point to a 3D triangle. */
ZEO_ELEM_TRIXD_DIST_FROM_POINT( 3D )

/* check if a 3D point is inside of a 3D triangle. */
ZEO_ELEM_TRIXD_POINT_IS_INSIDE( 3D )

/* volume of a 3D cone. */
double zTri3DConeVolume(const zTri3D *tri, const zVec3D *v)
{
  zVec3D v1, v2, v3;

  zVec3DSub( zTri3DVert(tri,0), v, &v1 );
  zVec3DSub( zTri3DVert(tri,1), v, &v2 );
  zVec3DSub( zTri3DVert(tri,2), v, &v3 );
  return zVec3DGrassmannProd( &v1, &v2, &v3 ) / 6;
}

/* barycenter of a 3D cone. */
zVec3D *zTri3DConeBarycenter(const zTri3D *tri, const zVec3D *v, zVec3D *c)
{
  zVec3DAdd( zTri3DVert(tri,0), zTri3DVert(tri,1), c );
  zVec3DAddDRC( c, zTri3DVert(tri,2) );
  zVec3DAddDRC( c, v );
  return zVec3DMulDRC( c, 0.25 );
}

/* inertia tensor of a 3D cone. */
zMat3D *zTri3DConeInertia(const zTri3D *tri, double density, zMat3D *i)
{
  int j;
  zMat3D m;
  zVec3D *v1, *v2;

  zMat3DZero( i );
  for( j=0; j<3; j++ ){
    v1 = zTri3DVert(tri,j);
    v2 = zTri3DVertNext(tri,j);
    _zVec3DTripleProd2Mat3D( v1, v1, &m );
    _zMat3DSubDRC( i, &m );
    _zVec3DTripleProd2Mat3D( v1, v2, &m );
    _zMat3DMulDRC( &m, 0.5 );
    _zMat3DSubDRC( i, &m );
    _zVec3DTripleProd2Mat3D( v2, v1, &m );
    _zMat3DMulDRC( &m, 0.5 );
    _zMat3DSubDRC( i, &m );
  }
  zMat3DMulDRC( i, density * zVec3DGrassmannProd( zTri3DVert(tri,0), zTri3DVert(tri,1), zTri3DVert(tri,2) ) / 60 );
  return i;
}

/* circumcenter of a 3D cone. */
zVec3D *zTri3DConeCircumcenter(const zTri3D *tri, zVec3D *c)
{
  zVec3D v;

  zVec3DZero( c );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(tri,0)), zVec3DOuterProd(zTri3DVert(tri,1),zTri3DVert(tri,2),&v) );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(tri,1)), zVec3DOuterProd(zTri3DVert(tri,2),zTri3DVert(tri,0),&v) );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(tri,2)), zVec3DOuterProd(zTri3DVert(tri,0),zTri3DVert(tri,1),&v) );
  return zVec3DDivDRC( c, 2*zVec3DGrassmannProd(zTri3DVert(tri,0),zTri3DVert(tri,1),zTri3DVert(tri,2)) );
}

/* print information of a 3D triangle to a file. */
void zTri3DFPrint(FILE *fp, const zTri3D *tri)
{
  if( !tri )
    fprintf( fp, "(null triangle)\n" );
  else{
    fprintf( fp, "vert: 0 " );
    zVec3DFPrint( fp, zTri3DVert(tri,0) );
    fprintf( fp, "vert: 1 " );
    zVec3DFPrint( fp, zTri3DVert(tri,1) );
    fprintf( fp, "vert: 2 " );
    zVec3DFPrint( fp, zTri3DVert(tri,2) );
    fprintf( fp, "norm: " );
    zVec3DFPrint( fp, zTri3DNorm(tri) );
  }
}

/* ********************************************************** */
/* 3D tetrahedron.
 * ********************************************************** */

/* test if the closest point on a tetrahedron composed by four given 3D points to the origin is on a facet. */
static bool _zTetra3DClosestFromOrigin_TestTri(const zVec3D *vert0, const zVec3D *vert1, const zVec3D *vert2, double *s0, double *s1, double *s2, double *s3, zVec3D *closestpoint, double *dist_min)
{
  double dist;
  zVec3D cp;
  if( ( dist = zTri3DClosestFromOrigin( vert0, vert1, vert2, s0, s1, s2, &cp ) ) < *dist_min ){
    *s3 = 0;
    *dist_min = dist;
    zVec3DCopy( &cp, closestpoint );
    return true;
  }
  return false;
}

/* closest point on a tetrahedron composed by four given 3D points to the origin. */
double zTetra3DClosestFromOrigin(const zVec3D *vert0, const zVec3D *vert1, const zVec3D *vert2, const zVec3D *vert3, double *s0, double *s1, double *s2, double *s3, zVec3D *closestpoint)
{
  double s[4][4], inv_det, dist_min = HUGE_VAL;
  int tri_case = -1;

  *s0 = zVec3DGrassmannProd( vert3, vert2, vert1 );
  *s1 = zVec3DGrassmannProd( vert2, vert3, vert0 );
  *s2 = zVec3DGrassmannProd( vert1, vert0, vert3 );
  *s3 = zVec3DGrassmannProd( vert0, vert1, vert2 );
  inv_det = 1.0 / ( *s0 + *s1 + *s2 + *s3 );
  *s0 *= inv_det;
  *s1 *= inv_det;
  *s2 *= inv_det;
  *s3 *= inv_det;
  if( *s0 < 0 && _zTetra3DClosestFromOrigin_TestTri( vert1, vert2, vert3, &s[0][1], &s[0][2], &s[0][3], &s[0][0], closestpoint, &dist_min ) )
    tri_case = 0;
  if( *s1 < 0 && _zTetra3DClosestFromOrigin_TestTri( vert2, vert3, vert0, &s[1][2], &s[1][3], &s[1][0], &s[1][1], closestpoint, &dist_min ) )
    tri_case = 1;
  if( *s2 < 0 && _zTetra3DClosestFromOrigin_TestTri( vert3, vert0, vert1, &s[2][3], &s[2][0], &s[2][1], &s[2][2], closestpoint, &dist_min ) )
    tri_case = 2;
  if( *s3 < 0 && _zTetra3DClosestFromOrigin_TestTri( vert0, vert1, vert2, &s[3][0], &s[3][1], &s[3][2], &s[3][3], closestpoint, &dist_min ) )
    tri_case = 3;
  if( tri_case == -1 ){
    zVec3DZero( closestpoint );
    return 0;
  }
  *s0 = s[tri_case][0];
  *s1 = s[tri_case][1];
  *s2 = s[tri_case][2];
  *s3 = s[tri_case][3];
  return zVec3DNorm( closestpoint );
}

/* closest point on a tetrahedron composed by four given 3D points to a 3D point. */
double zTetra3DClosest(const zVec3D *vert0, const zVec3D *vert1, const zVec3D *vert2, const zVec3D *vert3, const zVec3D *point, zVec3D *closestpoint)
{
  zVec3D v[4];
  double dist, s[4];

  zVec3DSub( vert0, point, &v[0] );
  zVec3DSub( vert1, point, &v[1] );
  zVec3DSub( vert2, point, &v[2] );
  zVec3DSub( vert3, point, &v[3] );
  dist = zTetra3DClosestFromOrigin( &v[0], &v[1], &v[2], &v[3], &s[0], &s[1], &s[2], &s[3], closestpoint );
  zVec3DAddDRC( closestpoint, point );
  return dist;
}
