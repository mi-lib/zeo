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
void zLine3DCommonPerpEnd(zLine3D *line1, zLine3D *line2, zVec3D *p1, zVec3D *p2)
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
zLine3D *zLine3DCommonPerp(zLine3D *line1, zLine3D *line2, zVec3D *origin, zLine3D *commonperp)
{
  zVec3D p2;

  zLine3DCommonPerpEnd( line1, line2, origin, &p2 );
  return zLine3DCreateTwoPoints( commonperp, origin, &p2 );
}

/* distance between two 3D lines. */
double zDistLine3DLine3D(zLine3D *line1, zLine3D *line2)
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

/* the closest point from point to a 3D edge. */
ZEO_ELEM_EDGEXD_CLOSEST( 3D )

/* contiguous vertix of a 3D edge to a point. */
ZEO_ELEM_EDGEXD_CONTIG_VERT( 3D )

/* print information of a 3D edge to a file. */
ZEO_ELEM_EDGEXD_FPRINT( 3D )

/* compute a pair of linear scale factors of a point on a 3D edge.
 * if the point is on the edge, p = l0*e->v0 + l1*e->v1. Otherwise, the result does not make sense. */
_ZEO_ELEM_EDGEXD_LINSCALE( 3D )

/* ********************************************************** */
/* 3D plane class
 * ********************************************************** */

/* create a plane. */
zPlane3D* zPlane3DCreate(zPlane3D *plane, zVec3D *origin, zVec3D *norm)
{
  if( zVec3DIsTiny( norm ) ){
    ZRUNERROR( ZEO_ERR_ELEM_DEGP );
    return NULL;
  }
  zPlane3DSetOrg( plane, origin );
  zPlane3DSetNorm( plane, norm );
  return plane;
}

/* signed distance between a point and a plane. */
double zPlane3DDistFromPoint(zPlane3D *plane, zVec3D *point)
{
  zVec3D tmp;

  zVec3DSub( point, zPlane3DOrg(plane), &tmp );
  return zVec3DInnerProd( &tmp, zPlane3DNorm(plane) );
}

/* check if a 3D point is on a 3D plane. */
bool zPlane3DPointIsOn(zPlane3D *plane, zVec3D *point, double margin)
{
  zVec3D tmp;

  zVec3DSub( point, zPlane3DOrg(plane), &tmp );
  if( zVec3DIsTiny( &tmp ) ) return true;
  zVec3DNormalizeDRC( &tmp );
  return zIsTol( zVec3DInnerProd( &tmp, zPlane3DNorm(plane) ), margin );
}

/* project a 3D point onto a 3D plane. */
double zPlane3DProjPoint(zPlane3D *plane, zVec3D *p, zVec3D *projection)
{
  double d;

  d = zPlane3DDistFromPoint( plane, p );
  zVec3DCat( p, -d, zPlane3DNorm(plane), projection );
  return d;
}

/* mean plane of a set of points. */
zPlane3D *zPlane3DMean(zPlane3D *plane, zVec3D *pc, zVec3D v[], int n)
{
  int i;
  double eval[3];
  zVec3D p, evec[3];
  zMat3D m;

  zVec3DZero( &p );
  zMat3DZero( &m );
  for( i=0; i<n; i++ ){
    zVec3DAddDRC( &p, &v[i] );
    zMat3DAddDyad( &m, &v[i], &v[i] );
  }
  zVec3DDiv( &p, n, pc );
  zMat3DSubDyad( &m, &p, pc );
  zMat3DSymEig( &m, eval, evec );

  if( eval[0] < eval[1] )
    i = eval[0] < eval[2] ? 0 : 2;
  else
    i = eval[1] < eval[2] ? 1 : 2;
  return zPlane3DCreate( plane, pc, &evec[i] );
}

/* print information of a plane to a file. */
void zPlane3DFPrint(FILE *fp, zPlane3D *plane)
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
zTri3D *zTri3DCreate(zTri3D *tri, zVec3D *v1, zVec3D *v2, zVec3D *v3)
{
  zTri3DSetVert( tri, 0, v1 );
  zTri3DSetVert( tri, 1, v2 );
  zTri3DSetVert( tri, 2, v3 );
  if( v1 && v2 && v3 )
    if( !zTri3DCalcNorm(tri) ) return NULL;
  return tri;
}

/* create a 3D triangle in the reversed order. */
zTri3D *zTri3DCreateRev(zTri3D *tri, zVec3D *v1, zVec3D *v2, zVec3D *v3)
{
  return zTri3DCreate( tri, v1, v3, v2 );
}

/* reverse a 3D triangle. */
zTri3D *zTri3DRev(zTri3D *src, zTri3D *dest)
{
  return zTri3DCreate( dest, zTri3DVert(src,0), zTri3DVert(src,2), zTri3DVert(src,1) );
}

/* outer product of two edges of a 3D triangle. */
static zVec3D *_zTri3DOuterProd(zTri3D *tri, zVec3D *n)
{
  zVec3D edge1, edge2;

  zVec3DSub( zTri3DVert(tri,1), zTri3DVert(tri,0), &edge1 );
  zVec3DSub( zTri3DVert(tri,2), zTri3DVert(tri,1), &edge2 );
  return zVec3DOuterProd( &edge1, &edge2, n );
}

/* area of a 3D triangle. */
double zTri3DArea(zTri3D *tri)
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
double zTri3DDistFromPointToPlane(zTri3D *tri, zVec3D *v)
{
  zVec3D tmp;
  double d[3];
  int i;

  for( i=0; i<3; i++ ){
    zVec3DSub( v, zTri3DVert(tri,i), &tmp );
    d[i] = zVec3DInnerProd( &tmp, zTri3DNorm(tri) );
  }
  return ( d[0] + d[1] + d[2] ) / 3;
}

/* check if a 3D point is on the identical plane of a 3D triangle. */
bool zTri3DPointIsOnPlane(zTri3D *tri, zVec3D *v, double margin)
{
  zVec3D tmp;

  zVec3DSub( v, zTri3DVert(tri,0), &tmp );
  if( zVec3DIsTiny( &tmp ) ) return true;
  zVec3DNormalizeDRC( &tmp );
  return zIsTol( zVec3DInnerProd( &tmp, zTri3DNorm(tri) ), margin );
}

/* project a 3D point to a 3D triangle. */
double zTri3DProjPoint(zTri3D *tri, zVec3D *v, zVec3D *cp)
{
  double d;

  d = zTri3DDistFromPointToPlane( tri, v );
  zVec3DCat( v, -d, zTri3DNorm(tri), cp );
  return d;
}

/* check if a 3D point is inside of a 3D triangle. */
bool zTri3DPointIsInside(zTri3D *tri, zVec3D *v, double margin)
{
  int i;
  zVec3D vp, e, n, d;

  zTri3DProjPoint( tri, v, &vp );
  for( i=0; i<3; i++ ){
    zVec3DSub( zTri3DVertNext(tri,i), zTri3DVert(tri,i), &e );
    zVec3DOuterProd( &e, zTri3DNorm(tri), &n );
    zVec3DNormalizeDRC( &n );
    zVec3DSub( &vp, zTri3DVert(tri,i), &d );
    if( zVec3DInnerProd( &n, &d ) > margin ) return false;
  }
  return true;
}

/* compute a trio of linear scale factors of a point on a 3D triangle.
 * when p is on t, p = l0*t->v0 + l1*t->v1 + l2*t->v2. If p is not on t, the result does not make sense. */

static void _zTri3DLinScaleVal(zVec2D *l, double d1, double d2, double *l0, double *l1, double *l2)
{
  *l0 = 1 - ( ( *l1 = l->c.x / d1 ) + ( *l2 = l->c.y / d2 ) );
}

static void _zTri3DLinScaleDeg2(zTri3D *tri, zVec3D *p, int i1, int i2, double *l0, double *l1, double *l2, zVec3D *cp)
{
  zEdge3D edge;
  zVec3D pp;

  *l0 = 0;
  zEdge3DCreate( &edge, zTri3DVert(tri,i1), zTri3DVert(tri,i2) );
  zEdge3DProjPoint( &edge, p, &pp );
  _zEdge3DLinScale( &edge, &pp, l1, l2, cp );
}

static void _zTri3DLinScaleDeg1(zTri3D *tri, int i, double *l0, double *l1, double *l2, zVec3D *cp)
{
  *l0 = *l1 = 0; *l2 = 1;
  zVec3DCopy( zTri3DVert(tri,i), cp );
}

double zTri3DLinScale(zTri3D *tri, zVec3D *p, double *l0, double *l1, double *l2, zVec3D *cp)
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

/* the closest point from a 3D point to a 3D triangle. */
ZEO_ELEM_TRIXD_CLOSEST( 3D )

/* distance from a 3D point to a 3D triangle. */
ZEO_ELEM_TRIXD_DIST_FROM_POINT( 3D )

/* volume of a 3D cone. */
double zTri3DConeVolume(zTri3D *tri, zVec3D *v)
{
  zVec3D v1, v2, v3;

  zVec3DSub( zTri3DVert(tri,0), v, &v1 );
  zVec3DSub( zTri3DVert(tri,1), v, &v2 );
  zVec3DSub( zTri3DVert(tri,2), v, &v3 );
  return zVec3DGrassmannProd( &v1, &v2, &v3 ) / 6;
}

/* barycenter of a 3D cone. */
zVec3D *zTri3DConeBarycenter(zTri3D *tri, zVec3D *v, zVec3D *c)
{
  zVec3DAdd( zTri3DVert(tri,0), zTri3DVert(tri,1), c );
  zVec3DAddDRC( c, zTri3DVert(tri,2) );
  zVec3DAddDRC( c, v );
  return zVec3DMulDRC( c, 0.25 );
}

/* inertia tensor of a 3D cone. */
zMat3D *zTri3DConeInertia(zTri3D *tri, double density, zMat3D *i)
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
zVec3D *zTri3DConeCircumcenter(zTri3D *tri, zVec3D *c)
{
  zVec3D v;

  zVec3DZero( c );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(tri,0)), zVec3DOuterProd(zTri3DVert(tri,1),zTri3DVert(tri,2),&v) );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(tri,1)), zVec3DOuterProd(zTri3DVert(tri,2),zTri3DVert(tri,0),&v) );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(tri,2)), zVec3DOuterProd(zTri3DVert(tri,0),zTri3DVert(tri,1),&v) );
  return zVec3DDivDRC( c, 2*zVec3DGrassmannProd(zTri3DVert(tri,0),zTri3DVert(tri,1),zTri3DVert(tri,2)) );
}

/* print information of a 3D triangle to a file. */
void zTri3DFPrint(FILE *fp, zTri3D *tri)
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
