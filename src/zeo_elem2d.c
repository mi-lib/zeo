/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d - 2D shape element.
 */

#include <zeo/zeo_elem2d.h>

/* ********************************************************** */
/* CLASS: zEdge2D
 * 2D edge class
 * ********************************************************** */

/* initialize a 2D edge. */
ZEDGEXD_INIT( 2D )

/* create a 2D edge. */
ZEDGEXD_CREATE( 2D )

/* path vector of a 2D edge. */
ZEDGEXD_CALC_VEC( 2D )

/* a set of unit edge direction vector and displacement vector. */
_ZEDGEXD_PROJ_SET( 2D )

/* project a point to a 2D edge. */
ZEDGEXD_PROJ( 2D )

/* distance between a point and a 2D edge. */
ZEDGEXD_POINT_DIST( 2D )

/* compute a pair of linear scale factors of a point on a 2D edge. */
ZEDGEXD_LINSCALE( 2D )

/* the closest point from point to a 2D edge. */
ZEDGEXD_CLOSEST( 2D )

/* ********************************************************** */
/* CLASS: zTri2D
 * 2D triangle class
 * ********************************************************** */

/* create a triangle. */
zTri2D *zTri2DCreate(zTri2D *t, zVec2D *v1, zVec2D *v2, zVec2D *v3)
{
  zTri2DSetVert( t, 0, v1 );
  zTri2DSetVert( t, 1, v2 );
  zTri2DSetVert( t, 2, v3 );
  return t;
}

/* barycenter of a triangle. */
ZTRIXD_BARYCENTER( 2D )

#define _zTri2DWeightedCenter(t,w1,w2,w3,p) do{\
  double __d;\
  __d = (w1) + (w2) + (w3);\
  (p)->c.x = ( (w1)*zTri2DVert(t,0)->c.x + (w2)*zTri2DVert(t,1)->c.x + (w3)*zTri2DVert(t,2)->c.x ) / __d;\
  (p)->c.y = ( (w1)*zTri2DVert(t,0)->c.y + (w2)*zTri2DVert(t,1)->c.y + (w3)*zTri2DVert(t,2)->c.y ) / __d;\
} while(0)

/* circumcenter of a triangle */
ZTRIXD_CIRCUMCENTER( 2D )

/* incenter of a triangle */
ZTRIXD_INCENTER( 2D )

/* orthocenter of a triangle */
ZTRIXD_ORTHOCENTER( 2D )

/* check if a point is inside of a triangle. */
bool zTri2DPointIsInside(zTri2D *t, zVec2D *v, double margin)
{
  int i;
  zVec2D e, n, d;

  for( i=0; i<3; i++ ){
    zVec2DSub( zTri2DVert(t,(i+1)%3), zTri2DVert(t,i), &e );
    zVec2DRot90CW( &e, &n );
    zVec2DNormalizeDRC( &n );
    zVec2DSub( v, zTri2DVert(t,i), &d );
    if( zVec2DInnerProd( &n, &d ) > margin ) return false;
  }
  return true;
}

/* print information of a triangle to a file. */
void zTri2DFPrint(FILE *fp, zTri2D *t)
{
  if( !t )
    fprintf( fp, "(null triangle)\n" );
  else{
    fprintf( fp, "vert: 0 " );
    zVec2DFPrint( fp, zTri2DVert(t,0) );
    fprintf( fp, "vert: 1 " );
    zVec2DFPrint( fp, zTri2DVert(t,1) );
    fprintf( fp, "vert: 2 " );
    zVec2DFPrint( fp, zTri2DVert(t,2) );
  }
}

/* ********************************************************** */
/* CLASS: zDisk2D
 * 2D disk class.
 * ********************************************************** */

/* create a 2D disk. */
zDisk2D *zDisk2DCreate(zDisk2D *d, zVec2D *c, double r)
{
  zDisk2DSetCenter( d, c );
  zDisk2DSetRadius( d, r );
  return d;
}

/* signed distance from a 2D point to a disk. */
double zDisk2DPointDist(zDisk2D *d, zVec2D *p)
{
  return zVec2DDist( zDisk2DCenter(d), p ) - zDisk2DRadius(d);
}

/* check if a 2D point is inside of a disk. */
bool zDisk2DPointIsInside(zDisk2D *d, zVec2D *v, double margin)
{
  return zDisk2DPointDist( d, v ) < margin ? true : false;
}

/* create a 2D disk from two points at both ends of diameter. */
zDisk2D *zDisk2DFrom2(zDisk2D *d, zVec2D *v1, zVec2D *v2)
{
  zVec2DMid( v1, v2, zDisk2DCenter(d) );
  zDisk2DSetRadius( d, zVec2DDist(zDisk2DCenter(d),v1) );
  return d;
}

/* create a 2D disk from three points as the circumcircle of them. */
zDisk2D *zDisk2DFrom3(zDisk2D *d, zVec2D *v1, zVec2D *v2, zVec2D *v3)
{
  zTri2D t;

  zTri2DCreate( &t, v1, v2, v3 );
  zTri2DCircumcenter( &t, zDisk2DCenter(d) );
  zDisk2DSetRadius( d, zVec2DDist(zDisk2DCenter(d),v3) );
  return d;
}

/* ********************************************************** */
/* 2D ellipse class.
 * ********************************************************** */

/* create an ellipse. */
zEllips2D *zEllips2DCreate(zEllips2D *e, zVec2D *c, double r1, double r2)
{
  zEllips2DSetCenter( e, c );
  zEllips2DSetRadius( e, 0, r1 );
  zEllips2DSetRadius( e, 1, r2 );
  return e;
}

/* closest point to a 2D point on an ellipse with a margin. */
static zVec2D *_zEllips2DClosest(zEllips2D *e, zVec2D *p, double margin, zVec2D *cp)
{
  zVec2D v;
  double x2, y2, r02, r12, l;
  zPex pex;
  zComplex ans_array[4];
  zCVecStruct ans;
  int i;

  _zVec2DSub( p, zEllips2DCenter(e), &v );
  x2 = zSqr( v.c.x );
  y2 = zSqr( v.c.y );
  r02 = zSqr( zEllips2DRadius(e,0) + margin );
  r12 = zSqr( zEllips2DRadius(e,1) + margin );
  if( x2/r02 + y2/r12 > 1 ){
    if( !( pex = zPexAlloc( 4 ) ) ) return NULL;
    zCVecSetSizeNC( &ans, 4 );
    zCVecBufNC(&ans) = ans_array;
    zPexSetCoeff( pex, 4, 1 );
    zPexSetCoeff( pex, 3, 2 * ( r02 + r12 ) );
    zPexSetCoeff( pex, 2, r02*r02+4*r02*r12+r12*r12-x2*r02-y2*r12 );
    zPexSetCoeff( pex, 1, 2 * ( r02 + r12 - x2 - y2 ) * r02 * r12 );
    zPexSetCoeff( pex, 0, ( r02*r12 - x2*r12 - y2*r02 ) * r02 * r12 );
    zPexBH( pex, &ans, zTOL, 0 );
    zPexFree( pex );
    for( i=0; i<4; i++ )
      if( ( l = zCVecElemNC(&ans,i)->re ) >= 0 ) break;
    if( i == 4 || !zIsTiny( zCVecElemNC(&ans,i)->im ) ){
      ZRUNERROR( ZEO_ERR_FATAL );
      return NULL;
    }
    v.c.x /= 1 + l / r02;
    v.c.y /= 1 + l / r12;
    _zVec2DAdd( &v, zEllips2DCenter(e), cp );
  } else
    zVec2DCopy( p, cp );
  return cp;
}

/* closest point to a 2D point on an ellipse. */
zVec2D *zEllips2DClosest(zEllips2D *e, zVec2D *p, zVec2D *cp)
{
  return _zEllips2DClosest( e, p, 0, cp );
}

/* distance from a 2D ellipse to a 2D point. */
double zEllips2DPointDist(zEllips2D *e, zVec2D *p)
{
  zVec2D cp, *ret;

  if( !( ret = zEllips2DClosest(e,p,&cp) ) ) return HUGE_VAL;
  return zVec2DDist( &cp, p );
}

/* check if a 2D point is inside of an ellipse. */
bool zEllips2DPointIsInside(zEllips2D *e, zVec2D *p, double margin)
{
  return zSqr( ( p->c.x - zEllips2DCenter(e)->c.x ) / ( zEllips2DRadius(e,0) + margin ) )
       + zSqr( ( p->c.y - zEllips2DCenter(e)->c.y ) / ( zEllips2DRadius(e,1) + margin ) ) < 1 ? true : false;
}
