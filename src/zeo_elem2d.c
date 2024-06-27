/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d - 2D shape element.
 */

#include <zeo/zeo_elem2d.h>
#include <zeo/zeo_mat2d.h>

/* ********************************************************** */
/* 2D line class
 * ********************************************************** */

/* initialize a 2D line. */
ZEO_ELEM_LINEXD_INIT( 2D );

/* create a 2D line from a point and a direction vector. */
ZEO_ELEM_LINEXD_CREATE( 2D )

/* create a 2D line from two passing points. */
ZEO_ELEM_LINEXD_CREATE_TWOPOINT( 2D )

/* a 2D point on a line. */
ZEO_ELEM_LINEXD_POINT( 2D )

/* projection of a 2D point onto a line. */
ZEO_ELEM_LINEXD_PROJ_POINT( 2D )

/* distance from a 2D point to a line. */
ZEO_ELEM_LINEXD_DIST_FROM_POINT( 2D )

/* check if a 2D point is on a line. */
ZEO_ELEM_LINEXD_POINT_IS_ON( 2D )

/* the intersection of two 2D lines. */
zVec2D *zIntersectLine2D(zLine2D *line1, zLine2D *line2, zVec2D *intersection)
{
  zVec2D dp, s;
  zMat2D m;

  zVec2DSub( zLine2DOrg(line2), zLine2DOrg(line1), &dp );
  zVec2DCopy( zLine2DDir(line1), zMat2DVec(&m,0) );
  zVec2DRev(  zLine2DDir(line2), zMat2DVec(&m,1) );
  if( !zMulInvMat2DVec2D( &m, &dp, &s ) ){
    ZRUNERROR( "parallel 2D lines do not intersect" );
    return NULL;
  }
  return zLine2DPoint( line1, s.c.x, intersection );
}

/* ********************************************************** */
/* 2D edge class
 * ********************************************************** */

/* initialize a 2D edge. */
ZEO_ELEM_EDGEXD_INIT( 2D )

/* create a 2D edge. */
ZEO_ELEM_EDGEXD_CREATE( 2D )

/* path vector of a 2D edge. */
ZEO_ELEM_EDGEXD_CALC_VEC( 2D )

/* a set of unit edge direction vector and displacement vector. */
_ZEO_ELEM_EDGEXD_PROJ_SET( 2D )

/* project a point to a 2D edge. */
ZEO_ELEM_EDGEXD_PROJ_POINT( 2D )

/* distance between a point and a 2D edge. */
ZEO_ELEM_EDGEXD_DIST_FROM_POINT( 2D )

/* check if a point is on the identical line with a 2D edge. */
ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE( 2D )

/* check if a point is on a 2D edge. */
ZEO_ELEM_EDGEXD_POINT_IS_ON( 2D )

/* the closest point from point to a 2D edge. */
ZEO_ELEM_EDGEXD_CLOSEST( 2D )

/* contiguous vertix of a 2D edge to a point. */
ZEO_ELEM_EDGEXD_CONTIG_VERT( 2D )

/* print information of a 2D edge to a file. */
ZEO_ELEM_EDGEXD_FPRINT( 2D )

/* ********************************************************** */
/* 2D triangle class
 * ********************************************************** */

/* create a triangle. */
zTri2D *zTri2DCreate(zTri2D *tri, zVec2D *v1, zVec2D *v2, zVec2D *v3)
{
  zTri2DSetVert( tri, 0, v1 );
  zTri2DSetVert( tri, 1, v2 );
  zTri2DSetVert( tri, 2, v3 );
  return tri;
}

/* barycenter of a triangle. */
ZEO_ELEM_TRIXD_BARYCENTER( 2D )

#define _zTri2DWeightedCenter(tri,w1,w2,w3,p) do{\
  double __d;\
  __d = (w1) + (w2) + (w3);\
  (p)->c.x = ( (w1)*zTri2DVert(tri,0)->c.x + (w2)*zTri2DVert(tri,1)->c.x + (w3)*zTri2DVert(tri,2)->c.x ) / __d; \
  (p)->c.y = ( (w1)*zTri2DVert(tri,0)->c.y + (w2)*zTri2DVert(tri,1)->c.y + (w3)*zTri2DVert(tri,2)->c.y ) / __d;\
} while(0)

/* circumcenter of a triangle */
ZEO_ELEM_TRIXD_CIRCUMCENTER( 2D )

/* incenter of a triangle */
ZEO_ELEM_TRIXD_INCENTER( 2D )

/* orthocenter of a triangle */
ZEO_ELEM_TRIXD_ORTHOCENTER( 2D )

/* contiguous vertix of a 2D triangle to a 2D point. */
ZEO_ELEM_TRIXD_CONTIG_VERT( 2D )

/* check if a point is inside of a triangle. */
bool zTri2DPointIsInside(zTri2D *tri, zVec2D *v, double margin)
{
  int i;
  zVec2D e, n, d;

  for( i=0; i<3; i++ ){
    zVec2DSub( zTri2DVert(tri,(i+1)%3), zTri2DVert(tri,i), &e );
    zVec2DRot90CW( &e, &n );
    zVec2DNormalizeDRC( &n );
    zVec2DSub( v, zTri2DVert(tri,i), &d );
    if( zVec2DInnerProd( &n, &d ) > margin ) return false;
  }
  return true;
}

/* the closest point from a 2D point to a 2D triangle. */
#define zTri2DProjPoint(tri,v,cp) zVec2DCopy( v, cp )
ZEO_ELEM_TRIXD_CLOSEST( 2D )

/* distance from a 2D point to a 2D triangle. */
ZEO_ELEM_TRIXD_DIST_FROM_POINT( 2D )

/* print information of a triangle to a file. */
void zTri2DFPrint(FILE *fp, zTri2D *tri)
{
  if( !tri )
    fprintf( fp, "(null triangle)\n" );
  else{
    fprintf( fp, "vert: 0 " );
    zVec2DFPrint( fp, zTri2DVert(tri,0) );
    fprintf( fp, "vert: 1 " );
    zVec2DFPrint( fp, zTri2DVert(tri,1) );
    fprintf( fp, "vert: 2 " );
    zVec2DFPrint( fp, zTri2DVert(tri,2) );
  }
}

/* ********************************************************** */
/* 2D disk class.
 * ********************************************************** */

/* create a 2D disk. */
zDisk2D *zDisk2DCreate(zDisk2D *disk, zVec2D *c, double r)
{
  zDisk2DSetCenter( disk, c );
  zDisk2DSetRadius( disk, r );
  return disk;
}

/* signed distance from a 2D point to a disk. */
double zDisk2DDistFromPoint(zDisk2D *disk, zVec2D *p)
{
  return zVec2DDist( zDisk2DCenter(disk), p ) - zDisk2DRadius(disk);
}

/* check if a 2D point is inside of a disk. */
bool zDisk2DPointIsInside(zDisk2D *disk, zVec2D *v, double margin)
{
  return zDisk2DDistFromPoint( disk, v ) < margin ? true : false;
}

/* create a 2D disk from two points at both ends of diameter. */
zDisk2D *zDisk2DFrom2(zDisk2D *disk, zVec2D *v1, zVec2D *v2)
{
  zVec2DMid( v1, v2, zDisk2DCenter(disk) );
  zDisk2DSetRadius( disk, zVec2DDist(zDisk2DCenter(disk),v1) );
  return disk;
}

/* create a 2D disk from three points as the circumcircle of them. */
zDisk2D *zDisk2DFrom3(zDisk2D *disk, zVec2D *v1, zVec2D *v2, zVec2D *v3)
{
  zTri2D tri;

  zTri2DCreate( &tri, v1, v2, v3 );
  zTri2DCircumcenter( &tri, zDisk2DCenter(disk) );
  zDisk2DSetRadius( disk, zVec2DDist(zDisk2DCenter(disk),v3) );
  return disk;
}

/* ********************************************************** */
/* 2D ellipse class.
 * ********************************************************** */

/* create an ellipse. */
zEllips2D *zEllips2DCreate(zEllips2D *ellips, zVec2D *c, double r1, double r2)
{
  zEllips2DSetCenter( ellips, c );
  zEllips2DSetRadius( ellips, 0, r1 );
  zEllips2DSetRadius( ellips, 1, r2 );
  return ellips;
}

/* closest point to a 2D point on an ellipse with a margin. */
static zVec2D *_zEllips2DClosest(zEllips2D *ellips, zVec2D *p, double margin, zVec2D *cp)
{
  zVec2D v;
  double x2, y2, r02, r12, l;
  zPex pex;
  zComplex ans_array[4];
  zCVecStruct ans;
  int i;

  _zVec2DSub( p, zEllips2DCenter(ellips), &v );
  x2 = zSqr( v.c.x );
  y2 = zSqr( v.c.y );
  r02 = zSqr( zEllips2DRadius(ellips,0) + margin );
  r12 = zSqr( zEllips2DRadius(ellips,1) + margin );
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
    _zVec2DAdd( &v, zEllips2DCenter(ellips), cp );
  } else
    zVec2DCopy( p, cp );
  return cp;
}

/* closest point to a 2D point on an ellipse. */
zVec2D *zEllips2DClosest(zEllips2D *ellips, zVec2D *p, zVec2D *cp)
{
  return _zEllips2DClosest( ellips, p, 0, cp );
}

/* distance from a 2D ellipse to a 2D point. */
double zEllips2DDistFromPoint(zEllips2D *ellips, zVec2D *p)
{
  zVec2D cp, *ret;

  if( !( ret = zEllips2DClosest(ellips,p,&cp) ) ) return HUGE_VAL;
  return zVec2DDist( &cp, p );
}

/* check if a 2D point is inside of an ellipse. */
bool zEllips2DPointIsInside(zEllips2D *ellips, zVec2D *p, double margin)
{
  return zSqr( ( p->c.x - zEllips2DCenter(ellips)->c.x ) / ( zEllips2DRadius(ellips,0) + margin ) )
       + zSqr( ( p->c.y - zEllips2DCenter(ellips)->c.y ) / ( zEllips2DRadius(ellips,1) + margin ) ) < 1 ?
    true : false;
}
