/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d - 3D shape elements.
 */

#include <zeo/zeo_mat2d.h>
#include <zeo/zeo_elem3d.h>

/* ********************************************************** */
/* CLASS: zPlane3D
 * 3D plane class
 * ********************************************************** */

/* create a plane. */
zPlane3D* zPlane3DCreate(zPlane3D *p, zVec3D *v, zVec3D *n)
{
  if( zVec3DIsTiny( n ) ){
    ZRUNERROR( ZEO_ERR_ELEM_DEGP );
    return NULL;
  }
  zPlane3DSetVert( p, v );
  zPlane3DSetNorm( p, n );
  return p;
}

/* signed distance between a point and a plane. */
double zPlane3DPointDist(zPlane3D *p, zVec3D *v)
{
  zVec3D tmp;

  zVec3DSub( v, zPlane3DVert(p), &tmp );
  return zVec3DInnerProd( &tmp, zPlane3DNorm(p) );
}

/* check if a point is on a plane. */
bool zPlane3DPointIsOn(zPlane3D *p, zVec3D *v)
{
  zVec3D tmp;

  zVec3DSub( v, zPlane3DVert(p), &tmp );
  if( zVec3DIsTiny( &tmp ) ) return true;
  zVec3DNormalizeDRC( &tmp );
  return zIsTiny( zVec3DInnerProd( &tmp, zPlane3DNorm(p) ) );
}

/* projection of a point to a plane. */
double zPlane3DProj(zPlane3D *p, zVec3D *v, zVec3D *cp)
{
  double d;

  d = zPlane3DPointDist( p, v );
  zVec3DCat( v, -d, zPlane3DNorm(p), cp );
  return d;
}

/* mean plane of a set of points. */
zPlane3D *zPlane3DMean(zPlane3D *pl, zVec3D *pc, zVec3D v[], int n)
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
  return zPlane3DCreate( pl, pc, &evec[i] );
}

/* print information of a plane to a file. */
void zPlane3DFPrint(FILE *fp, zPlane3D *p)
{
  fprintf( fp, "vert: " );
  zVec3DFPrint( fp, zPlane3DVert(p) );
  fprintf( fp, "norm: " );
  zVec3DFPrint( fp, zPlane3DNorm(p) );
}

/* ********************************************************** */
/* CLASS: zEdge3D
 * 3D edge class
 * ********************************************************** */

/* initialize a 3D edge. */
ZEDGEXD_INIT( 3D )

/* create a 3D edge. */
ZEDGEXD_CREATE( 3D )

/* path vector of a 3D edge. */
ZEDGEXD_CALC_VEC( 3D )

/* a set of unit edge direction vector and displacement vector. */
_ZEDGEXD_PROJ_SET( 3D )

/* project a point to a 3D edge. */
ZEDGEXD_PROJ( 3D )

/* distance between a point and a 3D edge. */
ZEDGEXD_POINT_DIST( 3D )

/* check if a point is on the line. */
bool zEdge3DPointIsOn(zEdge3D *e, zVec3D *p)
{
  zVec3D v, dp;

  if( !_zEdge3DProjSet( e, p, &v, &dp ) ) return true;
  zVec3DNormalizeDRC( &dp );
  return zVec3DIsTiny( zVec3DOuterProd( &dp, &v, &v ) );
}

/* compute a pair of linear scale factors of a point on a 3D edge.
 * when p is on e, p = l0*e->v0 + l1*e->v1. If p is not on e, the result does not make sense. */
ZEDGEXD_LINSCALE( 3D )

/* the closest point from point to a 3D edge. */
ZEDGEXD_CLOSEST( 3D )

/* contiguous vertix of a 3D edge to a point. */
zVec3D *zEdge3DContigVert(zEdge3D *e, zVec3D *p, double *d)
{
  double d0, d1;

  d0 = zVec3DSqrDist( p, zEdge3DVert(e,0) );
  d1 = zVec3DSqrDist( p, zEdge3DVert(e,1) );
  if( d0 <= d1 ){
    if( d ) *d = sqrt( d0 );
    return zEdge3DVert(e,0);
  }{
    if( d ) *d = sqrt( d1 );
    return zEdge3DVert(e,1);
  }
}

/* print information of a 3D edge to a file. */
void zEdge3DFPrint(FILE *fp, zEdge3D *e)
{
  if( !e )
    fprintf( fp, "(null edge)\n" );
  else{
    fprintf( fp, "vert: 0 " );
    zVec3DFPrint( fp, zEdge3DVert(e,0) );
    fprintf( fp, "vert: 1 " );
    zVec3DFPrint( fp, zEdge3DVert(e,1) );
    fprintf( fp, "vec: " );
    zVec3DFPrint( fp, zEdge3DVec(e) );
  }
}

/* ********************************************************** */
/* CLASS: zTri3D
 * 3D triangle class
 * ********************************************************** */

/* initialize a triangle. */
zTri3D *zTri3DInit(zTri3D *t)
{
  zTri3DSetVert( t, 0, NULL );
  zTri3DSetVert( t, 1, NULL );
  zTri3DSetVert( t, 2, NULL );
  zTri3DSetNorm( t, ZVEC3DZERO );
  return t;
}

/* create a triangle. */
zTri3D *zTri3DCreate(zTri3D *t, zVec3D *v1, zVec3D *v2, zVec3D *v3)
{
  zTri3DSetVert( t, 0, v1 );
  zTri3DSetVert( t, 1, v2 );
  zTri3DSetVert( t, 2, v3 );
  if( v1 && v2 && v3 )
    if( !zTri3DCalcNorm(t) ) return NULL;
  return t;
}

/* create a triangle in the reversed order. */
zTri3D *zTri3DCreateRev(zTri3D *t, zVec3D *v1, zVec3D *v2, zVec3D *v3)
{
  return zTri3DCreate( t, v1, v3, v2 );
}

/* reverse a triangle. */
zTri3D *zTri3DRev(zTri3D *src, zTri3D *dest)
{
  return zTri3DCreate( dest,
    zTri3DVert(src,0), zTri3DVert(src,2), zTri3DVert(src,1) );
}

/* outer product of two edges of a triangle. */
static zVec3D *_zTri3DOuterProd(zTri3D *t, zVec3D *n)
{
  zVec3D e1, e2;

  zVec3DSub( zTri3DVert(t,1), zTri3DVert(t,0), &e1 );
  zVec3DSub( zTri3DVert(t,2), zTri3DVert(t,1), &e2 );
  return zVec3DOuterProd( &e1, &e2, n );
}

/* area of a triangle. */
double zTri3DArea(zTri3D *t)
{
  zVec3D norm;

  _zTri3DOuterProd( t, &norm );
  return 0.5 * zVec3DNorm( &norm );
}

/* normal vector of a triangle. */
zVec3D *zTri3DCalcNorm(zTri3D *t)
{
  _zTri3DOuterProd( t, zTri3DNorm(t) );
  if( zVec3DIsTiny( zTri3DNorm(t) ) ) return NULL;
  zVec3DNormalizeDRC( zTri3DNorm(t) );
  return zTri3DNorm(t);
}

/* barycenter of a triangle. */
ZTRIXD_BARYCENTER( 3D )

#define _zTri3DWeightedCenter(t,w1,w2,w3,p) do{\
  double __d;\
  __d = (w1) + (w2) + (w3);\
  (p)->c.x = ( (w1)*zTri3DVert(t,0)->c.x + (w2)*zTri3DVert(t,1)->c.x + (w3)*zTri3DVert(t,2)->c.x ) / __d;\
  (p)->c.y = ( (w1)*zTri3DVert(t,0)->c.y + (w2)*zTri3DVert(t,1)->c.y + (w3)*zTri3DVert(t,2)->c.y ) / __d;\
  (p)->c.z = ( (w1)*zTri3DVert(t,0)->c.z + (w2)*zTri3DVert(t,1)->c.z + (w3)*zTri3DVert(t,2)->c.z ) / __d;\
} while(0)

/* circumcenter of a triangle */
ZTRIXD_CIRCUMCENTER( 3D )

/* incenter of a triangle */
ZTRIXD_INCENTER( 3D )

/* orthocenter of a triangle */
ZTRIXD_ORTHOCENTER( 3D )

/* contiguous vertix of a triangle to a point. */
zVec3D *zTri3DContigVert(zTri3D *t, zVec3D *p, double *d)
{
  double d0, d1, d2;

  d0 = zVec3DSqrDist( p, zTri3DVert(t,0) );
  d1 = zVec3DSqrDist( p, zTri3DVert(t,1) );
  d2 = zVec3DSqrDist( p, zTri3DVert(t,2) );
  if( d0 <= d1 ){
    if( d0 <= d2 ){
      if( d ) *d = sqrt( d0 );
      return zTri3DVert(t,0);
    }{
      if( d ) *d = sqrt( d2 );
      return zTri3DVert(t,2);
    }
  }{
    if( d1 <= d2 ){
      if( d ) *d = sqrt( d1 );
      return zTri3DVert(t,1);
    }{
      if( d ) *d = sqrt( d2 );
      return zTri3DVert(t,2);
    }
  }
}

/* signed distance from a point to a triangle. */
double zTri3DPointDist(zTri3D *t, zVec3D *v)
{
  zVec3D tmp;
  double d[3];
  int i;

  for( i=0; i<3; i++ ){
    zVec3DSub( v, zTri3DVert(t,i), &tmp );
    d[i] = zVec3DInnerProd( &tmp, zTri3DNorm(t) );
  }
  return ( d[0] + d[1] + d[2] ) / 3;
}

/* check if a point is on a triangle. */
bool zTri3DPointIsOn(zTri3D *t, zVec3D *v)
{
  zVec3D tmp;

  zVec3DSub( v, zTri3DVert(t,0), &tmp );
  if( zVec3DIsTiny( &tmp ) ) return true;
  zVec3DNormalizeDRC( &tmp );
  return zIsTiny( zVec3DInnerProd( &tmp, zTri3DNorm(t) ) );
}

/* project a point to a triangle. */
double zTri3DProj(zTri3D *t, zVec3D *v, zVec3D *cp)
{
  double d;

  d = zTri3DPointDist( t, v );
  zVec3DCat( v, -d, zTri3DNorm(t), cp );
  return d;
}

/* check if a point is inside of a triangle. */
bool zTri3DPointIsInside(zTri3D *t, zVec3D *v, double margin)
{
  int i;
  zVec3D vp, e, n, d;

  zTri3DProj( t, v, &vp );
  for( i=0; i<3; i++ ){
    zVec3DSub( zTri3DVertNext(t,i), zTri3DVert(t,i), &e );
    zVec3DOuterProd( &e, zTri3DNorm(t), &n );
    zVec3DNormalizeDRC( &n );
    zVec3DSub( &vp, zTri3DVert(t,i), &d );
    if( zVec3DInnerProd( &n, &d ) > margin ) return false;
  }
  return true;
}

/* compute a trio of linear scale factors of a point on a triangle.
 * when p is on t, p = l0*t->v0 + l1*t->v1 + l2*t->v2. If p is not on t, the result does not make sense. */

static void _zTri3DLinScaleVal(zVec2D *l, double d1, double d2, double *l0, double *l1, double *l2)
{
  *l0 = 1 - ( ( *l1 = l->c.x / d1 ) + ( *l2 = l->c.y / d2 ) );
}

static void _zTri3DLinScaleDeg2(zTri3D *t, zVec3D *p, int i1, int i2, double *l0, double *l1, double *l2, zVec3D *cp)
{
  zEdge3D e;
  zVec3D pp;

  *l0 = 0;
  zEdge3DCreate( &e, zTri3DVert(t,i1), zTri3DVert(t,i2) );
  zEdge3DProj( &e, p, &pp );
  zEdge3DLinScale( &e, &pp, l1, l2, cp );
}

static void _zTri3DLinScaleDeg1(zTri3D *t, int i, double *l0, double *l1, double *l2, zVec3D *cp)
{
  *l0 = *l1 = 0; *l2 = 1;
  zVec3DCopy( zTri3DVert(t,i), cp );
}

double zTri3DLinScale(zTri3D *t, zVec3D *p, double *l0, double *l1, double *l2, zVec3D *cp)
{
  zVec3D e1, e2, dp;
  zVec2D v, l;
  zMat2D m;
  double d0, d1, d2, mc;
  int i0, i1, i2;

  d0 = zVec3DSqrDist( p, zTri3DVert(t,0) );
  d1 = zVec3DSqrDist( p, zTri3DVert(t,1) );
  d2 = zVec3DSqrDist( p, zTri3DVert(t,2) );
  i0 = ( d0 <= d1 ) ? ( ( d0 <= d2 ) ? 0 : 2 ) : ( ( d1 <= d2 ) ? 1 : 2 );
  i1 = ( i0 + 1 ) % 3;
  i2 = ( i0 + 2 ) % 3;

  zVec3DSub( zTri3DVert(t,i1), zTri3DVert(t,i0), &e1 );
  d1 = zVec3DNormalizeDRC( &e1 );
  zVec3DSub( zTri3DVert(t,i2), zTri3DVert(t,i0), &e2 );
  d2 = zVec3DNormalizeDRC( &e2 );
  zVec3DSub( p, zTri3DVert(t,i0), &dp );
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
  case 0x1: _zTri3DLinScaleDeg2( t, p, 1, 2, l0, l1, l2, cp ); break;
  case 0x2: _zTri3DLinScaleDeg2( t, p, 2, 0, l1, l2, l0, cp ); break;
  case 0x3: _zTri3DLinScaleDeg1( t, 2, l0, l1, l2, cp );       break;
  case 0x4: _zTri3DLinScaleDeg2( t, p, 0, 1, l2, l0, l1, cp ); break;
  case 0x5: _zTri3DLinScaleDeg1( t, 1, l2, l0, l1, cp );       break;
  case 0x6: _zTri3DLinScaleDeg1( t, 0, l1, l2, l0, cp );       break;
  case 0x7: ZRUNERROR( ZEO_ERR_FATAL ); break;
  }
  return zVec3DDist( p, cp );
}

/* the closest point from a point to a triangle. */
double zTri3DClosest(zTri3D *t, zVec3D *v, zVec3D *cp)
{
  zVec3D tmp;
  double l0, l1, l2;

  zTri3DProj( t, v, &tmp );
  zTri3DLinScale( t, &tmp, &l0, &l1, &l2, cp );
  return zVec3DDist( v, cp );
}

/* volume of a cone. */
double zTri3DConeVolume(zTri3D *t, zVec3D *v)
{
  zVec3D v1, v2, v3;

  zVec3DSub( zTri3DVert(t,0), v, &v1 );
  zVec3DSub( zTri3DVert(t,1), v, &v2 );
  zVec3DSub( zTri3DVert(t,2), v, &v3 );
  return zVec3DGrassmannProd( &v1, &v2, &v3 ) / 6;
}

/* barycenter of a cone. */
zVec3D *zTri3DConeBarycenter(zTri3D *t, zVec3D *v, zVec3D *c)
{
  zVec3DAdd( zTri3DVert(t,0), zTri3DVert(t,1), c );
  zVec3DAddDRC( c, zTri3DVert(t,2) );
  zVec3DAddDRC( c, v );
  return zVec3DMulDRC( c, 0.25 );
}

/* inertia tensor of a cone. */
zMat3D *zTri3DConeInertia(zTri3D *t, double density, zMat3D *i)
{
  int j;
  zMat3D m;
  zVec3D *v1, *v2;

  zMat3DZero( i );
  for( j=0; j<3; j++ ){
    v1 = zTri3DVert(t,j);
    v2 = zTri3DVertNext(t,j);
    _zVec3DTripleProd2Mat3D( v1, v1, &m );
    _zMat3DSubDRC( i, &m );
    _zVec3DTripleProd2Mat3D( v1, v2, &m );
    _zMat3DMulDRC( &m, 0.5 );
    _zMat3DSubDRC( i, &m );
    _zVec3DTripleProd2Mat3D( v2, v1, &m );
    _zMat3DMulDRC( &m, 0.5 );
    _zMat3DSubDRC( i, &m );
  }
  zMat3DMulDRC( i, density * zVec3DGrassmannProd( zTri3DVert(t,0), zTri3DVert(t,1), zTri3DVert(t,2) ) / 60 );
  return i;
}

/* circumcenter of a cone. */
zVec3D *zTri3DConeCircumcenter(zTri3D *t, zVec3D *c)
{
  zVec3D v;

  zVec3DZero( c );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(t,0)), zVec3DOuterProd(zTri3DVert(t,1),zTri3DVert(t,2),&v) );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(t,1)), zVec3DOuterProd(zTri3DVert(t,2),zTri3DVert(t,0),&v) );
  zVec3DCatDRC( c, zVec3DSqrNorm(zTri3DVert(t,2)), zVec3DOuterProd(zTri3DVert(t,0),zTri3DVert(t,1),&v) );
  return zVec3DDivDRC( c, 2*zVec3DGrassmannProd(zTri3DVert(t,0),zTri3DVert(t,1),zTri3DVert(t,2)) );
}

/* print information of a triangle to a file. */
void zTri3DFPrint(FILE *fp, zTri3D *t)
{
  if( !t )
    fprintf( fp, "(null triangle)\n" );
  else{
    fprintf( fp, "vert: 0 " );
    zVec3DFPrint( fp, zTri3DVert(t,0) );
    fprintf( fp, "vert: 1 " );
    zVec3DFPrint( fp, zTri3DVert(t,1) );
    fprintf( fp, "vert: 2 " );
    zVec3DFPrint( fp, zTri3DVert(t,2) );
    fprintf( fp, "norm: " );
    zVec3DFPrint( fp, zTri3DNorm(t) );
  }
}
