/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem - 3D shape elements.
 */

#include <zeo/zeo_elem.h>

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
  register int i;
  double eval[3];
  zVec3D p, evec[3];
  zMat3D m;

  zVec3DClear( &p );
  zMat3DClear( &m );
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

static bool _zEdge3DProjSet(zEdge3D *e, zVec3D *p, zVec3D *v, zVec3D *dp);

/* initialize an edge. */
zEdge3D *zEdge3DInit(zEdge3D *e)
{
  zEdge3DSetVert( e, 0, NULL );
  zEdge3DSetVert( e, 1, NULL );
  zEdge3DSetVec( e, ZVEC3DZERO );
  return e;
}

/* create an edge. */
zEdge3D *zEdge3DCreate(zEdge3D *e, zVec3D *v1, zVec3D *v2)
{
  zEdge3DSetVert( e, 0, v1 );
  zEdge3DSetVert( e, 1, v2 );
  zEdge3DCalcVec( e );
  return e;
}

/* path vector of an edge. */
zVec3D *zEdge3DCalcVec(zEdge3D *e)
{
  return zVec3DSub( zEdge3DVert(e,1), zEdge3DVert(e,0), zEdge3DVec(e) );
}

/* a set of unit edge direction vector and displacement vector. */
bool _zEdge3DProjSet(zEdge3D *e, zVec3D *p, zVec3D *v, zVec3D *dp)
{
  zVec3DSub( p, zEdge3DVert(e,0), dp );
  zVec3DNormalize( zEdge3DVec(e), v );
  return !zVec3DIsTiny( dp );
}

/* project a point to an edge. */
zVec3D *zEdge3DProj(zEdge3D *e, zVec3D *p, zVec3D *cp)
{
  zVec3D v, dp;

  _zEdge3DProjSet( e, p, &v, &dp );
  zVec3DMul( &v, zVec3DInnerProd(&dp,&v), cp );
  return zVec3DAddDRC( cp, zEdge3DVert(e,0) );
}

/* distance between a point and an edge. */
double zEdge3DPointDist(zEdge3D *e, zVec3D *p)
{
  zVec3D v, dp;

  _zEdge3DProjSet( e, p, &v, &dp );
  return zVec3DOuterProdNorm( &dp, &v );
}

/* check if a point is on the line. */
bool zEdge3DPointIsOn(zEdge3D *e, zVec3D *p)
{
  zVec3D v, dp;

  if( !_zEdge3DProjSet( e, p, &v, &dp ) ) return true;
  zVec3DNormalizeDRC( &dp );
  return zVec3DIsTiny( zVec3DOuterProd( &dp, &v, &v ) );
}

/* compute a pair of linear scale factors of a point on an edge.
 * when p is on e, p = l0*e->v0 + l1*e->v1. If p is not on e, the result does not make sense. */
double zEdge3DLinScale(zEdge3D *e, zVec3D *p, double *l0, double *l1, zVec3D *cp)
{
  zVec3D dp0, dp1, v;
  double d, ret;
  int ni;

  zVec3DSub( p, zEdge3DVert(e,0), &dp0 );
  zVec3DSub( p, zEdge3DVert(e,1), &dp1 );
  ni = ( zVec3DSqrNorm(&dp0) <= zVec3DSqrNorm(&dp1) ) ? 0 : 1;
  if( ( d = zVec3DNormalize( zEdge3DVec(e), &v ) ) < 0 ){
    ZRUNERROR( ZEO_ERR_ELEM_DEGE );
    return -1;
  }
  if( ni == 0 ){
    *l1 = zVec3DInnerProd( &dp0, &v ) / d;
    *l0 = 1 - *l1;
  } else{
    *l0 =-zVec3DInnerProd( &dp1, &v ) / d;
    *l1 = 1 - *l0;
  }
  if( *l1 < 0 ){
    ret = -*l1 * d;
    *l1 = 0; *l0 = 1;
    zVec3DCopy( zEdge3DVert(e,0), cp );
    return ret;
  } else
  if( *l0 < 0 ){
    ret = -*l0 * d;
    *l0 = 0; *l1 = 1;
    zVec3DCopy( zEdge3DVert(e,1), cp );
    return ret;
  }
  zVec3DCopy( p, cp );
  return 0;
}

/* the closest point from point to an edge. */
double zEdge3DClosest(zEdge3D *e, zVec3D *p, zVec3D *cp)
{
  zVec3D tmp;
  double l0, l1;

  zEdge3DProj( e, p, &tmp );
  zEdge3DLinScale( e, &tmp, &l0, &l1, cp );
  return zVec3DDist( p, cp );
}

/* contiguous vertix of an edge to a point. */
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

/* print information of an edge to a file. */
void zEdge3DFPrint(FILE *fp, zEdge3D *e)
{
  if( !e )
    fprintf( fp, "(null edge)\n" );
  else{
    fprintf( fp, "vert 0: " );
    zVec3DFPrint( fp, zEdge3DVert(e,0) );
    fprintf( fp, "vert 1: " );
    zVec3DFPrint( fp, zEdge3DVert(e,1) );
    fprintf( fp, "vec: " );
    zVec3DFPrint( fp, zEdge3DVec(e) );
  }
}

/* ********************************************************** */
/* CLASS: zTri3D
 * 3D triangle class
 * ********************************************************** */

static zVec3D *_zTri3DOuterProd(zTri3D *t, zVec3D *n);

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
zVec3D *_zTri3DOuterProd(zTri3D *t, zVec3D *n)
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
zVec3D *zTri3DBarycenter(zTri3D *t, zVec3D *c)
{
  zVec3DAdd( zTri3DVert(t,0), zTri3DVert(t,1), c );
  zVec3DAddDRC( c, zTri3DVert(t,2) );
  return zVec3DDivDRC( c, 3 );
}

/* circumcenter of a triangle */
static double __z_tri3D_angle(zTri3D *t, int i, int j, int k){
  zVec3D e[2];
  zVec3DSub( zTri3DVert(t,j), zTri3DVert(t,i), &e[0] );
  zVec3DSub( zTri3DVert(t,k), zTri3DVert(t,i), &e[1] );
  return sin( 2 * zVec3DAngle( &e[0], &e[1], NULL ) );
}
zVec3D *zTri3DCircumcenter(zTri3D *t, zVec3D *c)
{
  double s[3];

  s[0] = __z_tri3D_angle( t, 0, 1, 2 );
  s[1] = __z_tri3D_angle( t, 1, 2, 0 );
  s[2] = __z_tri3D_angle( t, 2, 0, 1 );
  zVec3DClear( c );
  zVec3DCatDRC( c, s[0], zTri3DVert(t,0) );
  zVec3DCatDRC( c, s[1], zTri3DVert(t,1) );
  zVec3DCatDRC( c, s[2], zTri3DVert(t,2) );
  return zVec3DDivDRC( c, s[0] + s[1] + s[2] );
}

/* incenter of a triangle */
zVec3D *zTri3DIncenter(zTri3D *t, zVec3D *c)
{
  double s[3];

  s[0] = zVec3DDist( zTri3DVert(t,1), zTri3DVert(t,2) );
  s[1] = zVec3DDist( zTri3DVert(t,2), zTri3DVert(t,0) );
  s[2] = zVec3DDist( zTri3DVert(t,0), zTri3DVert(t,1) );
  zVec3DClear( c );
  zVec3DCatDRC( c, s[0], zTri3DVert(t,0) );
  zVec3DCatDRC( c, s[1], zTri3DVert(t,1) );
  zVec3DCatDRC( c, s[2], zTri3DVert(t,2) );
  return zVec3DDivDRC( c, s[0] + s[1] + s[2] );
}

/* orthocenter of a triangle */
zVec3D *zTri3DOrthocenter(zTri3D *t, zVec3D *c)
{
  zVec3D cg, cc;

  zTri3DBarycenter( t, &cg );
  zTri3DCircumcenter( t, &cc );
  zVec3DMulDRC( &cg, 3 );
  zVec3DMulDRC( &cc, 2 );
  return zVec3DSub( &cg, &cc, c );
}

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
  zVec3D tmp[3];
  double d[3];
  register int i;

  for( i=0; i<3; i++ ){
    zVec3DSub( v, zTri3DVert(t,i), &tmp[i] );
    d[i] = zVec3DInnerProd( &tmp[i], zTri3DNorm(t) );
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
bool zTri3DPointIsInside(zTri3D *t, zVec3D *v, bool rim)
{
  register int i;
  zVec3D e, *v1, *v2, tmp;
  double d;

  for( i=0; i<3; i++ ){
    v1 = zTri3DVert(t,i);
    v2 = zTri3DVertNext(t,i);
    zVec3DSub( v, v1, &tmp );
    if( zVec3DIsTiny( &tmp ) ) /* coincide with a vertex */
      return rim;
    zVec3DNormalizeNCDRC( &tmp );
    zVec3DSub( v2, v1, &e );
    zVec3DNormalizeDRC( &e );
    zVec3DOuterProd( &e, &tmp, &tmp );
    d = zVec3DInnerProd( &tmp, zTri3DNorm(t) );
    if( d < 0 || ( !rim && d <= zTOL ) ) return false;
  }
  return true;
}

/* compute a trio of linear scale factors of a point on a triangle.
 * when p is on t, p = l0*t->v0 + l1*t->v1 + l2*t->v2. If p is not on t, the result does not make sense. */
static void _zTri3DLinScaleVal(double l[], double d1, double d2, double *l0, double *l1, double *l2);
static void _zTri3DLinScaleDeg2(zTri3D *t, zVec3D *p, int i1, int i2, double *l0, double *l1, double *l2, zVec3D *cp);
static void _zTri3DLinScaleDeg1(zTri3D *t, int i, double *l0, double *l1, double *l2, zVec3D *cp);

void _zTri3DLinScaleVal(double l[], double d1, double d2, double *l0, double *l1, double *l2)
{
  *l0 = 1 - ( ( *l1 = l[0] / d1 ) + ( *l2 = l[1] / d2 ) );
}
void _zTri3DLinScaleDeg2(zTri3D *t, zVec3D *p, int i1, int i2, double *l0, double *l1, double *l2, zVec3D *cp)
{
  zEdge3D e;
  zVec3D pp;

  *l0 = 0;
  zEdge3DCreate( &e, zTri3DVert(t,i1), zTri3DVert(t,i2) );
  zEdge3DProj( &e, p, &pp );
  zEdge3DLinScale( &e, &pp, l1, l2, cp );
}
void _zTri3DLinScaleDeg1(zTri3D *t, int i, double *l0, double *l1, double *l2, zVec3D *cp)
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
  zVec2DCreate( v, zVec3DInnerProd(&e1,&dp), zVec3DInnerProd(&e2,&dp) );
  mc = zVec3DInnerProd(&e1,&e2);
  zMat2DCreate( m, 1.0, mc, mc, 1.0 );
  zMulInvMat2DVec2D( m, v, l );
  switch( i0 ){
  case 0: _zTri3DLinScaleVal( l, d1, d2, l0, l1, l2 ); break;
  case 1: _zTri3DLinScaleVal( l, d1, d2, l1, l2, l0 ); break;
  case 2: _zTri3DLinScaleVal( l, d1, d2, l2, l0, l1 ); break;
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

/* inertial tensor of a cone. */
zMat3D *zTri3DConeInertia(zTri3D *t, zMat3D *i)
{
  register int j;
  zMat3D m;
  zVec3D *v1, *v2;

  zMat3DClear( i );
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
  zMat3DMulDRC( i, 0.1*zTri3DConeVolume( t, ZVEC3DZERO ) );
  return i;
}

/* circumcenter of a cone. */
zVec3D *zTri3DConeCircumcenter(zTri3D *t, zVec3D *c)
{
  zVec3D v;

  zVec3DClear( c );
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
    fprintf( fp, "vert 0: " );
    zVec3DFPrint( fp, zTri3DVert(t,0) );
    fprintf( fp, "vert 1: " );
    zVec3DFPrint( fp, zTri3DVert(t,1) );
    fprintf( fp, "vert 2: " );
    zVec3DFPrint( fp, zTri3DVert(t,2) );
    fprintf( fp, "norm: " );
    zVec3DFPrint( fp, zTri3DNorm(t) );
  }
}
