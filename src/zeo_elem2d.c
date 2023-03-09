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
bool zTri2DPointIsInside(zTri2D *t, zVec2D *v, bool rim)
{
  int i;
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
