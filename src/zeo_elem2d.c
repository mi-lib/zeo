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
zVec2D *zIntersectLine2D(const zLine2D *line1, const zLine2D *line2, zVec2D *intersection)
{
  zVec2D dp, s;
  zMat2D m;

  zVec2DSub( zLine2DOrg(line2), zLine2DOrg(line1), &dp );
  zVec2DCopy( zLine2DDir(line1), zMat2DVec(&m,0) );
  zVec2DRev(  zLine2DDir(line2), zMat2DVec(&m,1) );
  if( !zMulInvMat2DVec2D( &m, &dp, &s ) ){
    ZRUNERROR( ZEO_ERR_COLCHK_LINE_PARALLEL );
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
zTri2D *zTri2DCreate(zTri2D *tri, const zVec2D *v1, const zVec2D *v2, const zVec2D *v3)
{
  zTri2DSetVert( tri, 0, (zVec2D*)v1 );
  zTri2DSetVert( tri, 1, (zVec2D*)v2 );
  zTri2DSetVert( tri, 2, (zVec2D*)v3 );
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
bool zTri2DPointIsInside(const zTri2D *tri, const zVec2D *v, double margin)
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
void zTri2DFPrint(FILE *fp, const zTri2D *tri)
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
