/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d - 2D shape element.
 */

#ifndef __ZEO_ELEM2D_H__
#define __ZEO_ELEM2D_H__

#include <zeo/zeo_vec2d.h>

#include <zeo/zeo_elemxd.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 2D line class.
 *//********************************************************* */
ZEO_ELEM_LINEXD_DEF_STRUCT( 2D );

#define zLine2DOrg(line)       zLineXDOrg( 2D, line )
#define zLine2DDir(line)       zLineXDDir( 2D, line )
#define zLine2DSetOrg(line,p)  zLineXDSetOrg( 2D, line, p )
#define zLine2DSetDir(line,d)  zLineXDSetDir( 2D, line, d )

/*! \brief initialize a 2D line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_INIT_PROTOTYPE( 2D );
/*! \brief create a 2D line from a point and a direction vector. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_CREATE_PROTOTYPE( 2D );
/*! \brief create a 2D line from two passing points. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_CREATE_TWOPOINT_PROTOTYPE( 2D );
/*! \brief a 2D point on a line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_POINT_PROTOTYPE( 2D );
/*! \brief projection of a 2D point onto a line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_PROJ_POINT_PROTOTYPE( 2D );
/*! \brief distance from a 2D point to a line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_DIST_FROM_POINT_PROTOTYPE( 2D );
/*! \brief check if a 2D point is on a line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_POINT_IS_ON_PROTOTYPE( 2D );

/*! \brief the intersection of two 2D lines. */
__ZEO_EXPORT zVec2D *zIntersectLine2D(const zLine2D *line1, const zLine2D *line2, zVec2D *intersection);

/* ********************************************************** */
/*! \brief 2D edge class.
 *//********************************************************* */
ZEO_ELEM_EDGEXD_DEF_STRUCT( 2D );

#define zEdge2DVert(edge,i)        zEdgeXDVert( 2D, edge, i )
#define zEdge2DVec(edge)           zEdgeXDVec( 2D, edge )

#define zEdge2DSetVert(edge,i,ptr) zEdgeXDSetVert( 2D, edge, i, ptr )
#define zEdge2DSetVec(edge,v)      zEdgeXDSetVec( 2D, edge, v )

/*! \brief initialize and create of a 2D edge.
 *
 * zEdge2DInit() initializes an edge \a e, setting both endpoints for
 * the null vectors.
 *
 * zEdge2DCreate() creates a edge from two endpoints \a v1 and \a v2.
 * \return
 * Each of zEdge2DInit() and zEdge2DCreate() returns a pointer \a e.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_INIT_PROTOTYPE( 2D );
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CREATE_PROTOTYPE( 2D );

/*! \brief path vector of a 2D edge.
 *
 * zEdge2DCalcVec() calculates the path vector from the first endpoint
 * to the second of an edge \a e. The vector is contained by the edge
 * itself within. One can access the path vector by calling zEdge2DVec().
 * \return
 * zEdge2DCalcVec() returns the pointer to the path vector.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CALC_VEC_PROTOTYPE( 2D );

/*! \brief length of a 2D edge. */
#define zEdge2DLen(edge)     ZEO_ELEM_EDGEXD_LEN( 2D, edge )
/*! \brief unit direction vector of a 2D edge. */
#define zEdge2DDir(edge,dir) ZEO_ELEM_EDGEXD_DIR( 2D, edge, dir )

/*! \brief check if a point is on the identical line with a 2D edge. */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE_PROTOTYPE( 2D );

__ZEO_EXPORT ZEO_ELEM_EDGEXD_POINT_IS_ON_PROTOTYPE( 2D );

/*! \brief distance between a point and a 2D edge.
 *
 * zEdge2DProj() finds the projection of a given 2D point \a p onto
 * a 2D edge \a e. The result is put into \a cp.
 *
 * zEdge2DDistFromPoint() calculates a distance between an edge \a e and a
 * point \a p. \a e is regarded as an infinite-length edge, namely, it
 * returns the length of the perpendicular from \a p to \a e.
 *
 * zEdge2DClosest(), on the contrary, calculates the actual closest point
 * on \a e from \a p and sets it into \a cp. When the perpendicular from
 * \a p to \a e does not cross with \a e, it returns the closest endpoint
 * of \a e from \a p.
 * \return
 * zEdge2DDistFromPoint() returns the distance calculated, nameyl, the length
 * of the perpendicular from \a p to \a e.
 *
 * zEdge2DClosest() returns the distance between \a p and \a cp.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_PROJ_POINT_PROTOTYPE( 2D );
__ZEO_EXPORT ZEO_ELEM_EDGEXD_DIST_FROM_POINT_PROTOTYPE( 2D );

/*! \brief closest point to a 2D edge.
 *
 * zEdge2DClosest() finds the closest point on \a edge from \a point and sets it into \a cp.
 * When the perpendicular from \a point to \a edge is not on \a edge, it returns the closest
 * endpoint of \a edge from \a point.
 * \return
 * zEdge3DClosest() returns the distance between \a point and \a cp.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CLOSEST_PROTOTYPE( 2D );

/*! \brief contiguous vertix of a 2D edge to a point.
 *
 * zEdge2DContigVert() returns the contiguous vertix of an edge \a edge to a given point \a point.
 * Namely, the value returned has to be either the first or the second vertix of \a edge.
 * If \a distance is not the null pointer, the distance between \a point and the detected contiguous
 * vertex is stored where \a distance points.
 * \return
 * zEdge2DContigVert() returns a pointer to the found vertix.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CONTIG_VERT_PROTOTYPE( 2D );

/*! \brief print a 2D edge.
 *
 * zEdge2DFPrint() prints coordinates of the endpoints of a 2D edge \a edge to the current position
 * of a file \a fp in the following format.
 *
 *  vert: 0 ( x0, y0, z0 )
 *  vert: 1 ( x1, y1, z1 )
 *  vec: ( vx, vy, vz )
 *
 * where vert 0 is the first endpoint, vert 1 is the second and vec is for the path vector.
 * \return
 * zEdge2DFPrint() and zEdge2DPrint() return no values.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_FPRINT_PROTOTYPE( 2D );
#define zEdge2DPrint(edge) zEdge2DFPrint( stdout, (edge) )

/* ********************************************************** */
/*! \brief 2D triangle class.
 *//********************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zTri2D ){
  zVec2D *v[3]; /*!< \brief vertices */
};

#define zTri2DVert(tri,i)      (tri)->v[(i)]
#define zTri2DSetVert(tri,i,v) ( zTri2DVert(tri,i) = (v) )

#define zTri2DVertNext(tri,i)  zTri2DVert( tri, ( (i)+1 ) % 3 )

/*! \brief initialize and create 2D triangle.
 *
 * zTri2DInit() initializes a triangle instance \a tri, setting three
 * vertices for the null vector.
 *
 * zTri2DCreate() creates a triangle from three vertices \a v1, \a v2
 * and \a v3.
 * \return
 * zTri2DInit() and zTri2DCreate() return a pointer \a tri.
 */
__ZEO_EXPORT zTri2D *zTri2DCreate(zTri2D *tri, const zVec2D *v1, const zVec2D *v2, const zVec2D *v3);
#define zTri2DInit(t) zTri2DCreate( t, NULL, NULL, NULL )

/*! \brief various centers of a 2D triangle.
 *
 * zTri2DBarycenter(), zTri2DCircumcenter(), zTri2DIncenter() and
 * zTri2DOrthocenter() calculate barycenter, circumcenter, incenter
 * and orthocenter of \a tri, respectively.
 * The result is put into \a c.
 * \return
 * zTri2DBarycenter(), zTri2DCircumcenter(), zTri2DIncenter() and
 * zTri2DOrthocenter() return a pointer \a c.
 */
__ZEO_EXPORT ZEO_ELEM_TRIXD_BARYCENTER_PROTOTYPE( 2D );
__ZEO_EXPORT ZEO_ELEM_TRIXD_CIRCUMCENTER_PROTOTYPE( 2D );
__ZEO_EXPORT ZEO_ELEM_TRIXD_INCENTER_PROTOTYPE( 2D );
__ZEO_EXPORT ZEO_ELEM_TRIXD_ORTHOCENTER_PROTOTYPE( 2D );

/*! \brief contiguous vertix of triangle to a point.
 *
 * zTri2DContigVert() returns the contiguous vertix of a triangle \a tri
 * to a given point \a p. Namely, the value returned has to be any of
 * three vertices of \a tri.
 * \return
 * zTri2DContigVert() returns a pointer to the found vertix.
 */
__ZEO_EXPORT ZEO_ELEM_TRIXD_CONTIG_VERT_PROTOTYPE( 2D );

/*! \brief check if a point is inside of a triangle.
 *
 * zTri2DPointIsInside() checks if a point \a v is inside of a 2D
 * triangle \a tri. \a margin is a margin of the inside area outward from
 * the boundary of \a tri.
 * \return
 * zTri2DPointIsInside() returns the true value if \a v is inside of \a tri
 * with a margin \a margin. Otherwise, the false value is returned.
 */
__ZEO_EXPORT bool zTri2DPointIsInside(const zTri2D *tri, const zVec2D *v, double margin);

/*! \brief the closest point from a point to a 2D triangle.
 *
 * zTri2DClosest() calculates the closest point on a triangle \a tri from
 * the given point \a v.
 * It sets the point calculated into \a cp.
 * \return
 * zTri2DClosest() returns the distance from \a v to the closest point
 * calculated.
 */
__ZEO_EXPORT ZEO_ELEM_TRIXD_CLOSEST_PROTOTYPE( 2D );

/*! \brief destance between a 2D point and a 2D triangle.
 *
 * zTri2DDistFromPoint() calculates the distance from a 2D point \a v to a 2D triangle \a tri, namely
 * the distance between the closest point on \a tri to \a v.
 * \return
 * zTri2DDistFromPoint() returns the distance between \a v and \a tri.
 */
__ZEO_EXPORT ZEO_ELEM_TRIXD_DIST_FROM_POINT_PROTOTYPE( 2D );

/*! \brief print a 2D triangle.
 *
 * zTri2DFPrint() prints coordinates of the vertices of a 2D triangle
 * \a tri to the current position of a file \a fp in the following format.
 *
 *  vert: 0 ( x0, y0 )
 *  vert: 1 ( x1, y1 )
 *  vert: 2 ( x2, y2 )
 *
 * zTri2DPrint() prints the same information of \a tri out to the
 * standard output.
 * \return
 * zTri2DFPrint() and zTri2DPrint() do not return any values.
 */
__ZEO_EXPORT void zTri2DFPrint(FILE *fp, const zTri2D *tri);
#define zTri2DPrint(tri) zTri2DFPrint( stdout, (tri) )

/*! \struct zTri2DArray
 * \brief array class of 2D triangles.
 */
zArrayClass( zTri2DArray, zTri2D );

/* NOTE: are the following classes to be moved to zeo_shape2d? */

/* ********************************************************** */
/*! \brief 2D disk class.
 *//********************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zDisk2D ){
  zVec2D center;
  double radius;
};

#define zDisk2DCenter(disk)      ( &(disk)->center )
#define zDisk2DRadius(disk)      (disk)->radius

#define zDisk2DSetCenter(disk,c) zVec2DCopy( c, zDisk2DCenter(disk) )
#define zDisk2DSetRadius(disk,r) ( zDisk2DRadius(disk) = (r) )

/*! \brief create a 2D disk. */
__ZEO_EXPORT zDisk2D *zDisk2DCreate(zDisk2D *disk, const zVec2D *c, double r);

/*! \brief signed distance from a 2D point to a disk. */
__ZEO_EXPORT double zDisk2DDistFromPoint(const zDisk2D *disk, const zVec2D *p);

/*! \brief check if a 2D point is inside of a disk. */
__ZEO_EXPORT bool zDisk2DPointIsInside(const zDisk2D *disk, const zVec2D *v, double margin);

/*! \brief create a 2D disk from two points at both ends of diameter. */
__ZEO_EXPORT zDisk2D *zDisk2DFrom2(zDisk2D *disk, const zVec2D *v1, const zVec2D *v2);

/*! \brief create a 2D disk from three points as a circumcircle of them. */
__ZEO_EXPORT zDisk2D *zDisk2DFrom3(zDisk2D *disk, const zVec2D *v1, const zVec2D *v2, const zVec2D *v3);

/* ********************************************************** */
/*! \brief 2D ellipse class.
 *//********************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zEllips2D ){
  zVec2D center;
  double radius[2];
};

#define zEllips2DCenter(ellips)        ( &(ellips)->center )
#define zEllips2DRadius(ellips,i)      (ellips)->radius[(i)]

#define zEllips2DSetCenter(ellips,c)   zVec2DCopy( c, zEllips2DCenter(ellips) )
#define zEllips2DSetRadius(ellips,i,r) ( zEllips2DRadius(ellips,i) = (r) )

/*! \brief create an ellipse. */
__ZEO_EXPORT zEllips2D *zEllips2DCreate(zEllips2D *ellips, const zVec2D *c, double r1, double r2);

/*! \brief closest point to a 2D point on an ellipse. */
__ZEO_EXPORT zVec2D *zEllips2DClosest(const zEllips2D *ellips, const zVec2D *p, zVec2D *cp);

/*! \brief signed distance from a 2D point to an ellipse. */
__ZEO_EXPORT double zEllips2DDistFromPoint(const zEllips2D *ellips, const zVec2D *p);

/*! \brief check if a 2D point is inside of an ellipse. */
__ZEO_EXPORT bool zEllips2DPointIsInside(const zEllips2D *ellips, const zVec2D *p, double margin);

__END_DECLS

#include <zeo/zeo_elem2d_aabox.h> /* 2D axis-aligned box */

#endif /* __ZEO_ELEM2D_H__ */
