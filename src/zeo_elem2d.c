/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d - 2D shape element.
 */

#include <zeo/zeo_elem2d.h>

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

/* TO BE REMOVED. */
/* a set of unit edge direction vector and displacement vector. */
_ZEO_ELEM_EDGEXD_PROJ_SET( 2D )

/* TO BE REMOVED. */
/* project a point to a 2D edge. */
ZEO_ELEM_EDGEXD_PROJ_POINT( 2D )

/* distance between a point and a 2D edge. */
ZEO_ELEM_EDGEXD_DIST_FROM_POINT( 2D )

/* TO BE REMOVED. */
/* check if a point is on the identical line with a 2D edge. */
ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE( 2D )

/* check if a point is on a 2D edge. */
ZEO_ELEM_EDGEXD_POINT_IS_ON( 2D )

/* the closest point from the origin to a 2D edge, and find coefficents of linear sum. */
ZEO_ELEM_EDGEXD_CLOSEST_FROM_ORIGIN( 2D )

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

static bool _zTri2DClosestFromOrigin_TestEdge(const zVec2D *vert0, const zVec2D *vert1, double *s0, double *s1, double *s2, zVec2D *closestpoint, double *dist_min)
{
  double dist;
  zVec2D cp;
  if( ( dist = zEdge2DClosestFromOrigin( vert0, vert1, s0, s1, &cp ) ) < *dist_min ){
    *s2 = 0;
    *dist_min = dist;
    zVec2DCopy( &cp, closestpoint );
    return true;
  }
  return false;
}

/* the closest point from the origin to a 2D triangle. */
double zTri2DClosestFromOrigin(const zVec2D *vert0, const zVec2D *vert1, const zVec2D *vert2, double *s0, double *s1, double *s2, zVec2D *closestpoint)
{
  double inv_det, dist_min = HUGE_VAL;
  double s[3][3];
  int edge_case = -1;

  *s0 = _zVec2DOuterProd( vert1, vert2 );
  *s1 = _zVec2DOuterProd( vert2, vert0 );
  *s2 = _zVec2DOuterProd( vert0, vert1 );
  inv_det = 1.0 / ( *s0 + *s1 + *s2 );
  *s0 *= inv_det;
  *s1 *= inv_det;
  *s2 *= inv_det;
  if( *s0 < 0 && _zTri2DClosestFromOrigin_TestEdge( vert1, vert2, &s[0][1], &s[0][2], &s[0][0], closestpoint, &dist_min ) )
    edge_case = 0;
  if( *s1 < 0 && _zTri2DClosestFromOrigin_TestEdge( vert2, vert0, &s[1][2], &s[1][0], &s[1][1], closestpoint, &dist_min ) )
    edge_case = 1;
  if( *s2 < 0 && _zTri2DClosestFromOrigin_TestEdge( vert0, vert1, &s[2][0], &s[2][1], &s[2][2], closestpoint, &dist_min ) )
    edge_case = 2;
  if( edge_case == -1 ){
    zVec2DZero( closestpoint );
    return 0;
  }
  *s0 = s[edge_case][0];
  *s1 = s[edge_case][1];
  *s2 = s[edge_case][2];
  return zVec2DNorm( closestpoint );
}

/* the closest point from a 2D point to a 2D triangle. */
double zTri2DClosest(const zTri2D *tri, const zVec2D *point, zVec2D *closestpoint)
{
  zVec2D vert[3];
  double s0, s1, s2, dist;

  zVec2DSub( zTri2DVert(tri,0), point, &vert[0] );
  zVec2DSub( zTri2DVert(tri,1), point, &vert[1] );
  zVec2DSub( zTri2DVert(tri,2), point, &vert[2] );
  dist = zTri2DClosestFromOrigin( &vert[0], &vert[1], &vert[2], &s0, &s1, &s2, closestpoint );
  zVec2DAddDRC( closestpoint, point );
  return dist;
}

/* distance from a 2D point to a 2D triangle. */
ZEO_ELEM_TRIXD_DIST_FROM_POINT( 2D )

/* check if a 2D point is inside of a 2D triangle. */
ZEO_ELEM_TRIXD_POINT_IS_INSIDE( 2D )

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
