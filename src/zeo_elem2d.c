/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d - 2D element.
 */

#include <zeo/zeo_elem2d.h>

/* ********************************************************** */
/* CLASS: zEdge2D
 * 2D edge class
 * ********************************************************** */

/* initialize a 2D edge. */
zEdge2D *zEdge2DInit(zEdge2D *e)
{
  zEdge2DSetVert( e, 0, NULL );
  zEdge2DSetVert( e, 1, NULL );
  zEdge2DSetVec( e, ZVEC2DZERO );
  return e;
}

/* create a 2D edge. */
zEdge2D *zEdge2DCreate(zEdge2D *e, zVec2D *v1, zVec2D *v2)
{
  zEdge2DSetVert( e, 0, v1 );
  zEdge2DSetVert( e, 1, v2 );
  zEdge2DCalcVec( e );
  return e;
}

/* path vector of a 2D edge. */
zVec2D *zEdge2DCalcVec(zEdge2D *e)
{
  return zVec2DSub( zEdge2DVert(e,1), zEdge2DVert(e,0), zEdge2DVec(e) );
}

/* a set of unit edge direction vector and displacement vector. */
static bool _zEdge2DProjSet(zEdge2D *e, zVec2D *p, zVec2D *v, zVec2D *dp)
{
  zVec2DSub( p, zEdge2DVert(e,0), dp );
  zVec2DNormalize( zEdge2DVec(e), v );
  return !zVec2DIsTiny( dp );
}

/* project a point to a 2D edge. */
zVec2D *zEdge2DProj(zEdge2D *e, zVec2D *p, zVec2D *cp)
{
  zVec2D v, dp;

  _zEdge2DProjSet( e, p, &v, &dp );
  zVec2DMul( &v, zVec2DInnerProd(&dp,&v), cp );
  return zVec2DAddDRC( cp, zEdge2DVert(e,0) );
}

/* distance between a point and a 2D edge. */
double zEdge2DPointDist(zEdge2D *e, zVec2D *p)
{
  zVec2D v, dp;

  _zEdge2DProjSet( e, p, &v, &dp );
  return zVec2DOuterProd( &dp, &v );
}

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

/* check if a point is inside of a triangle. */
bool zTri2DPointIsInside(zTri2D *t, zVec2D *v, bool rim)
{
  register int i;
  zVec2D e, tmp;
  double d[3];

  for( i=0; i<3; i++ ){
    if( zVec2DIsTiny( zVec2DSub( v, zTri2DVert(t,i), &tmp ) ) ) return rim; /* coincide with a vertex */
    zVec2DNormalizeDRC( &tmp );
    zVec2DNormalizeDRC( zVec2DSub( zTri2DVertNext(t,i), zTri2DVert(t,i), &e ) );
    d[i] = _zVec2DOuterProd( &e, &tmp );
  }
  return rim ? ( d[0] > -zTOL && d[1] > -zTOL && d[2] > -zTOL ) : ( d[0] > zTOL && d[1] > zTOL && d[2] > zTOL );
}

/* barycenter of a triangle. */
zVec2D *zTri2DBarycenter(zTri2D *t, zVec2D *c)
{
  zVec2DAdd( zTri2DVert(t,0), zTri2DVert(t,1), c );
  zVec2DAddDRC( c, zTri2DVert(t,2) );
  return zVec2DDivDRC( c, 3 );
}

#define _zTri2DWeightedCenter(t,w1,w2,w3,p) do{\
  double __d;\
  __d = (w1) + (w2) + (w3);\
  (p)->c.x = ( (w1)*zTri2DVert(t,0)->c.x + (w2)*zTri2DVert(t,1)->c.x + (w3)*zTri2DVert(t,2)->c.x ) / __d;\
  (p)->c.y = ( (w1)*zTri2DVert(t,0)->c.y + (w2)*zTri2DVert(t,1)->c.y + (w3)*zTri2DVert(t,2)->c.y ) / __d;\
} while(0)

/* circumcenter of a triangle */
zVec2D *zTri2DCircumcenter(zTri2D *t, zVec2D *c)
{
  double r[3], s[3];

  r[0] = zVec2DSqrDist( zTri2DVert(t,2), zTri2DVert(t,1) );
  r[1] = zVec2DSqrDist( zTri2DVert(t,0), zTri2DVert(t,2) );
  r[2] = zVec2DSqrDist( zTri2DVert(t,1), zTri2DVert(t,0) );
  s[0] = r[0] * ( r[1] + r[2] - r[0] );
  s[1] = r[1] * ( r[2] + r[0] - r[1] );
  s[2] = r[2] * ( r[0] + r[1] - r[2] );
  _zTri2DWeightedCenter( t, s[0], s[1], s[2], c );
  return c;
}

/* incenter of a triangle */
zVec2D *zTri2DIncenter(zTri2D *t, zVec2D *c)
{
  double s[3];

  s[0] = zVec2DDist( zTri2DVert(t,1), zTri2DVert(t,2) );
  s[1] = zVec2DDist( zTri2DVert(t,2), zTri2DVert(t,0) );
  s[2] = zVec2DDist( zTri2DVert(t,0), zTri2DVert(t,1) );
  _zTri2DWeightedCenter( t, s[0], s[1], s[2], c );
  return c;
}

/* orthocenter of a triangle */
zVec2D *zTri2DOrthocenter(zTri2D *t, zVec2D *c)
{
  zVec2D cg, cc;

  zTri2DCircumcenter( t, &cc );
  zTri2DBarycenter( t, &cg );
  zVec2DMul( &cg, 3, c );
  return zVec2DCatDRC( c, -2, &cc );
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
bool zDisk2DPointIsInside(zDisk2D *d, zVec2D *p, bool rim)
{
  return zDisk2DPointDist( d, p ) < ( rim ? zTOL : 0 ) ? true : false;
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
