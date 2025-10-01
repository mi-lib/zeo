/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d - 3D shape elements.
 */

#ifndef __ZEO_ELEM3D_H__
#define __ZEO_ELEM3D_H__

#include <zeo/zeo_frame3d.h>
#include <zeo/zeo_elemxd.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 3D line class.
 *//********************************************************* */
ZEO_ELEM_LINEXD_DEF_STRUCT( 3D );

#define zLine3DOrg(line)       zLineXDOrg( 3D, line )
#define zLine3DDir(line)       zLineXDDir( 3D, line )
#define zLine3DSetOrg(line,p)  zLineXDSetOrg( 3D, line, p )
#define zLine3DSetDir(line,d)  zLineXDSetDir( 3D, line, d )

/*! \brief initialize a 3D line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_INIT_PROTOTYPE( 3D );
/*! \brief create a 3D line from a point and a direction vector. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_CREATE_PROTOTYPE( 3D );
/*! \brief create a 3D line from two passing points. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_CREATE_TWOPOINT_PROTOTYPE( 3D );
/*! \brief a 3D point on a line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_POINT_PROTOTYPE( 3D );
/*! \brief projection of a 3D point onto a line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_PROJ_POINT_PROTOTYPE( 3D );
/*! \brief distance from a 3D point to a line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_DIST_FROM_POINT_PROTOTYPE( 3D );
/*! \brief check if a 3D point is on a line. */
__ZEO_EXPORT ZEO_ELEM_LINEXD_POINT_IS_ON_PROTOTYPE( 3D );

/*! \brief find two end points of the common perpendicular of two 3D lines. */
__ZEO_EXPORT void zLine3DCommonPerpEnd(const zLine3D *line1, const zLine3D *line2, zVec3D *p1, zVec3D *p2);

/*! \brief create the common perpendicular of two 3D lines. */
__ZEO_EXPORT zLine3D *zLine3DCommonPerp(const zLine3D *line1, const zLine3D *line2, zVec3D *origin, zLine3D *commonperp);

/*! \brief distance between two 3D lines. */
__ZEO_EXPORT double zDistLine3DLine3D(const zLine3D *line1, const zLine3D *line2);

/* ********************************************************** */
/*! \brief 3D edge class.
 *//********************************************************* */
ZEO_ELEM_EDGEXD_DEF_STRUCT( 3D );

#define zEdge3DVert(edge,i)        zEdgeXDVert( 3D, edge, i )
#define zEdge3DVec(edge)           zEdgeXDVec( 3D, edge )

#define zEdge3DSetVert(edge,i,ptr) zEdgeXDSetVert( 3D, edge, i, ptr )
#define zEdge3DSetVec(edge,v)      zEdgeXDSetVec( 3D, edge, v )

/*! \brief initialize and create of a 3D edge.
 *
 * zEdge3DInit() initializes an edge \a e, setting both endpoints for
 * the null vectors.
 *
 * zEdge3DCreate() creates a edge from two endpoints \a v1 and \a v2.
 * \return
 * Each of zEdge3DInit() and zEdge3DCreate() returns a pointer \a e.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_INIT_PROTOTYPE( 3D );
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CREATE_PROTOTYPE( 3D );

/*! \brief path vector of a 3D edge.
 *
 * zEdge3DCalcVec() calculates the path vector from the first endpoint
 * to the second of an edge \a e. The vector is contained by the edge
 * itself within. One can access the path vector by calling zEdge3DVec().
 * \return
 * zEdge3DCalcVec() returns the pointer to the path vector.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CALC_VEC_PROTOTYPE( 3D );

/*! \brief length of a 3D edge. */
#define zEdge3DLen(edge) ZEO_ELEM_EDGEXD_LEN( 3D, edge )
/*! \brief unit direction vector of a 3D edge. */
#define zEdge3DDir(edge,dir) ZEO_ELEM_EDGEXD_DIR( 3D, edge, dir )

/*! \brief distance between a point and a 3D edge.
 *
 * zEdge3DProjPoint() finds the projection of a given 3D point \a p onto
 * a 3D edge \a e. The result is put into \a cp.
 *
 * zEdge3DDistFromPoint() calculates a distance between an edge \a e and a
 * point \a p. \a e is regarded as an infinite-length edge, namely, it
 * returns the length of the perpendicular from \a p to \a e.
 * \return
 * zEdge3DDistFromPoint() returns the distance calculated, nameyl, the length
 * of the perpendicular from \a p to \a e.
 */
/* TO BE REMOVED. */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_PROJ_POINT_PROTOTYPE( 3D );
__ZEO_EXPORT ZEO_ELEM_EDGEXD_DIST_FROM_POINT_PROTOTYPE( 3D );

/* TO BE REMOVED. */
/*! \brief check if a point is on the identical line with a 3D edge. */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE_PROTOTYPE( 3D );

/*! \brief check if a point is on a 3D edge. */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_POINT_IS_ON_PROTOTYPE( 3D );

/*! \brief closest point from a 3D point to a 3D edge.
 *
 * zEdge3DClosestFromOrigin() finds the closest point from the origin to a 3D edge composed of two
 * 3D points \a vert0 and \a vert1. The result is put into \a closestpoint.
 * When the perpendicular from the origin to the edge is not in the edge span, it returns the closest
 * point of \a vert0 and \a vert1 to the origin.
 * Moreover, zEdge3DClosestFromOrigin() finds coefficients of the linear sum of the closest point,
 * namely, s0 and s1 that satisfy \a closestpoint = s0 \a vert0 + s1 \a vert1. The values are stored
 * where \a s0 and \a s1 point, respectively.
 * \return
 * zEdge3DClosestFromOrigin() returns the distance between the origin and \a closestpoint.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CLOSEST_FROM_ORIGIN_PROTOTYPE( 3D );

/*! \brief closest point from a 3D point to a 3D edge.
 *
 * zEdge3DClosest() finds the closest point from a 3D point \a point to a 3D edge \a edge.
 * The result is put into \a closestpoint.
 * When the perpendicular from \a point to \a edge is not on \a edge, it returns the contiguous
 * endpoint of \a edge to \a point.
 * \return
 * zEdge3DClosest() returns the distance between \a point and \a closestpoint.
 * \sa
 * zEdge3DClosestFromOrigin
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CLOSEST_PROTOTYPE( 3D );

/*! \brief contiguous vertix of a 3D edge to a 3D point.
 *
 * zEdge3DContigVert() returns the contiguous vertix of an edge \a edge to a given point \a point.
 * Namely, the value returned has to be either the first or the second vertix of \a edge.
 * If \a distance is not the null pointer, the distance between \a point and the detected contiguous
 * vertex is stored where \a distance points.
 * \return
 * zEdge3DContigVert() returns a pointer to the found vertix.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_CONTIG_VERT_PROTOTYPE( 3D );

/*! \brief print a 3D edge.
 *
 * zEdge3DFPrint() prints coordinates of the endpoints of a 3D edge \a edge to the current position
 * of a file \a fp in the following format.
 *
 *  vert: 0 ( x0, y0, z0 )
 *  vert: 1 ( x1, y1, z1 )
 *  vec: ( vx, vy, vz )
 *
 * where vert 0 is the first endpoint, vert 1 is the second and vec is for the path vector.
 * \return
 * zEdge3DFPrint() and zEdge3DPrint() return no values.
 */
__ZEO_EXPORT ZEO_ELEM_EDGEXD_FPRINT_PROTOTYPE( 3D );
#define zEdge3DPrint(edge) zEdge3DFPrint( stdout, (edge) )

/* ********************************************************** */
/*! \brief 3D plane class.
 *//********************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zPlane3D ){
  zVec3D *origin; /*!< original point */
  zVec3D norm;    /*!< normal vector */
};

#define zPlane3DOrg(plane)        ( (plane)->origin )
#define zPlane3DNorm(plane)       ( &(plane)->norm )

#define zPlane3DSetOrg(plane,org) ( zPlane3DOrg(plane) = (org) )
#define zPlane3DSetNorm(plane,n)  zVec3DNormalize( (n), zPlane3DNorm(plane) )

/*! \brief initialize and create a 3D plane.
 *
 * zPlane3DInit() initializes a 3D plane \a plane as the x-y plane.
 *
 * zPlane3DCreate() creates a 3D plane which is defined by the pass
 * point \a v and the normal vector \a n.
 * \return
 * zPlane3DInit() and zPlane3DCreate() return a pointer \a plane.
 */
__ZEO_EXPORT zPlane3D *zPlane3DCreate(zPlane3D *plane, const zVec3D *v, const zVec3D *n);
#define zPlane3DInit(plane) zPlane3DCreate( (plane), ZVEC3DZERO, ZVEC3DZ )

/*! \brief distance between a 3D point and a 3D plane.
 *
 * zPlane3DDistFromPoint() calculates the distance between a 3D point \a point and a 3D plane \a plane.
 * \return
 * zPlane3DDistFromPoint() returns the distance between a 3D point \a point and a 3D plane \a plane.
 */
__ZEO_EXPORT double zPlane3DDistFromPoint(const zPlane3D *plane, const zVec3D *point);

/*! \brief check if a 3D point is on a 3D plane.
 *
 * zPlane3DPointIsOn() checks if a point \a point is on a plane \a plane.
 * \a margin is the margin from the exact surface of \a plane. Namely, the distance from \a point
 * to \a plane is shorter than \a margin, zPlane3DPointIsOn() judges \a point as a point on \a plane.
 * \return
 * zPlane3DPointIsOn() returns the true value if \a point is on \a plane, or the false value otherwise.
 */
__ZEO_EXPORT bool zPlane3DPointIsOn(const zPlane3D *plane, const zVec3D *point, double margin);

/*! \brief projection of a 3D point to a 3D plane.
 *
 * zPlane3DProjPoint() calculates the projection point of a 3D point \a v to a 3D plane \a plane.
 * The result is put into the vector pointed by \a cp.
 * \return
 * zPlane3DProjPoint() returns the distance between \a plane and \a v.
 */
__ZEO_EXPORT double zPlane3DProjPoint(const zPlane3D *plane, const zVec3D *v, zVec3D *cp);

/*! \brief print a 3D plane.
 *
 * zPlane3DFPrint() prints information of a 3D plane \a plane out
 * to the current position of the file \a fp in the following format:
 *
 * origin: ( x, y, z )
 * norm:   ( x, y, z )
 *
 * zPlane3DPrint() prints information of the plane \a plane out to
 * the standard out.
 * \return
 * Neither zPlane3DFPrint() nor zPlane3DPrint() return any values.
 */
__ZEO_EXPORT void zPlane3DFPrint(FILE *fp, const zPlane3D *plane);
#define zPlane3DPrint(plane) zPlane3DFPrint( stdout, (plane) )

/* ********************************************************** */
/*! \brief 3D triangle class.
 *//********************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zTri3D ){
  zVec3D *v[3]; /*!< vertices */
  zVec3D norm;  /*!< normal vector */
};

#define zTri3DNorm(tri)        ( &(tri)->norm )
#define zTri3DVert(tri,i)      (tri)->v[(i)]
#define zTri3DVertNext(tri,i)  zTri3DVert( tri, ( (i)+1 ) % 3 )

#define zTri3DSetNorm(tri,n)   zVec3DCopy( n, zTri3DNorm(tri) )
#define zTri3DSetVert(tri,i,v) ( zTri3DVert(tri,i) = (v) )

/*! \brief initialize and create a 3D triangle.
 *
 * zTri3DInit() initializes a 3D triangle instance \a tri by setting the three vertices for the null vector.
 *
 * zTri3DCreate() creates a 3D triangle from three vertices \a v1, \a v2 and \a v3.
 * zTri3DCreateFlip() creates a 3D triangle from \a v1, \a v2 and \a v3 in a flipped order of zTri3DCreate().
 * Namely, it is equivalent with zTri3DCreate( v1, v3, v2 ).
 *
 * zTri3DFlip() creates a 3D triangle \a dest by flipping the given triangle \a src.
 * zTri3DFlipDRC() directly flips a 3D triangle \a tri.
 *
 * It allows \a dest to point the same address with \a src. In fact, zTri3DFlipDRC( tri ) is defined as
 * zTri3DRev( tri, tri ).
 * \return
 * zTri3DInit(), zTri3DCreate(), zTri3DCreateFlip(), and zTri3DFlipDRC() return a pointer \a tri.
 *
 * zTri3DFlip() returns a pointer \a dest.
 */
__ZEO_EXPORT zTri3D *zTri3DInit(zTri3D *tri);
__ZEO_EXPORT zTri3D *zTri3DCreate(zTri3D *tri, const zVec3D *v1, const zVec3D *v2, const zVec3D *v3);
__ZEO_EXPORT zTri3D *zTri3DCreateFlip(zTri3D *tri, const zVec3D *v1, const zVec3D *v2, const zVec3D *v3);
__ZEO_EXPORT zTri3D *zTri3DFlip(const zTri3D *src, zTri3D *dest);
#define zTri3DFlipDRC(tri) zTri3DFlip( tri, tri )

/*! \brief area and normal vector of a 3D triangle.
 *
 * zTri3DArea() calculates the area of a triangle \a tri.
 *
 * zTri3DCalcNorm() calculates the normal vector of triangle \a tri.
 * The vector is contained by the triangle itself within. One can access
 * the normal vector by calling zTri3DNorm() (see zeo_elem.h).
 * \return
 * zTri3DArea() returns the area calculated.
 * zTri3DCalcNorm() returns a pointer to the normal vector.
 */
__ZEO_EXPORT double zTri3DArea(const zTri3D *tri);
__ZEO_EXPORT zVec3D *zTri3DCalcNorm(zTri3D *tri);

/*! \brief various centers of a 3D triangle.
 *
 * zTri3DBarycenter(), zTri3DCircumcenter(), zTri3DIncenter() and
 * zTri3DOrthocenter() calculate barycenter, circumcenter, incenter
 * and orthocenter of \a tri, respectively.
 * The result is put into \a c.
 * \return
 * zTri3DBarycenter(), zTri3DCircumcenter(), zTri3DIncenter() and
 * zTri3DOrthocenter() return a pointer \a c.
 */
__ZEO_EXPORT ZEO_ELEM_TRIXD_BARYCENTER_PROTOTYPE( 3D );
__ZEO_EXPORT ZEO_ELEM_TRIXD_CIRCUMCENTER_PROTOTYPE( 3D );
__ZEO_EXPORT ZEO_ELEM_TRIXD_INCENTER_PROTOTYPE( 3D );
__ZEO_EXPORT ZEO_ELEM_TRIXD_ORTHOCENTER_PROTOTYPE( 3D );

/*! \brief convert a triangle to a plane.
 *
 * zTri3DToPlane3D() converts a given 3D triangle \a tri to an infinite
 * 3D plane \a plane which \a tri is on.
 * \return
 * zTri3DToPlane3D() returns a pointer \a tri.
 */
#define zTri3DToPlane3D(tri,plane) zPlane3DCreate( plane, zTri3DVert(tri,0), zTri3DNorm(tri) )

/*! \brief contiguous vertix of triangle to a point.
 *
 * zTri3DContigVert() returns the contiguous vertix of a triangle \a tri
 * to a given point \a p. Namely, the value returned has to be any of
 * three vertices of \a tri.
 * \return
 * zTri3DContigVert() returns a pointer to the found vertix.
 */
__ZEO_EXPORT ZEO_ELEM_TRIXD_CONTIG_VERT_PROTOTYPE( 3D );

/*! \brief geometric relation between a 3D point and the identical plane of a 3D triangle.
 *
 * zTri3DDistFromPointToPlane() calculates the signed distance from a 3D point \a point to the identical
 * plane of a 3D triangle \a tri. The result is
 *  - a positive value when \a point is above \a tri (\a point exists in the direction of the normal vector of \a tri),
 *  - a negative value when \a point is below \a tri (\a point exists in the opposite direction of the normal vector
 *    of \a tri),
 *  - zero when \a point is on \a tri.
 * The funciton does not care if the projection point of \a point is inside of \a tri or not, but only
 * calculates the length of the perpendicular from \a point to the identical 3D plae with \a tri.
 *
 * zTri3DPointIsOnPlane() checks if a 3D point \a point is on the identical plane with a 3D triangle \a tri,
 * namely, whether the distance from \a point to the plane is less than a given margin \a margin.
 *
 * zTri3DProjPoint() calculates the projection point of \a point into a triangle \a tri - the footpoint
 * of the perpendicular from \a point to \a tri, and sets it into \a projection.
 * \return
 * zTri3DDistFromPointToPlane() and zTri3DProjPoint() return the signed distance between \a point and \a tri.
 *
 * zTri3DPointIsOnPlane() returns the true value if the signed distance is larger than \a margin.
 * Otherwise, it returns the false value.
 */
__ZEO_EXPORT double zTri3DDistFromPointToPlane(const zTri3D *tri, const zVec3D *point);
__ZEO_EXPORT bool zTri3DPointIsOnPlane(const zTri3D *tri, const zVec3D *point, double margin);
__ZEO_EXPORT double zTri3DProjPoint(const zTri3D *tri, const zVec3D *point, zVec3D *projection);

/* TO BE REMOVED. */
__ZEO_EXPORT double zTri3DLinScale(const zTri3D *tri, const zVec3D *p, double *l0, double *l1, double *l2, zVec3D *cp);

/*! \brief closest point from a 3D point to a 3D triangle.
 *
 * zTri3DClosestFromOrigin() finds the closest point from the origin to a 3D triangle composed of three
 * 3D points \a vert0, \a vert1, and \a vert2. The result is put into \a closestpoint.
 * When the perpendicular from the origin to the triangle is outside of the area, it returns the closest
 * vertex of \a vert0, \a vert1, and \a vert2 to the origin.
 * Moreover, zTri3DClosestFromOrigin() finds coefficients of the linear sum of the closest point,
 * namely, s0, s1, and s2 that satisfy \a closestpoint = s0 \a vert0 + s1 \a vert1 + s2 \a vert2.
 * The values are stored where \a s0, \a s1, and \a s2 point, respectively.
 * \return
 * zTri3DClosestFromOrigin() returns the distance between the origin and \a closestpoint.
 */
__ZEO_EXPORT double zTri3DClosestFromOrigin(const zVec3D *vert0, const zVec3D *vert1, const zVec3D *vert2, double *s0, double *s1, double *s2, zVec3D *closestpoint);

/*! \brief the closest point from a point to a 3D triangle.
 *
 * zTri3DClosest() calculates the closest point on a triangle \a tri from
 * the given point \a v.
 * It sets the point calculated into \a cp.
 * \return
 * zTri3DClosest() returns the distance from \a v to the closest point
 * calculated.
 */
__ZEO_EXPORT double zTri3DClosest(const zTri3D *tri, const zVec3D *point, zVec3D *closestpoint);

/*! \brief destance between a 3D point and a 3D triangle.
 *
 * zTri3DDistFromPoint() calculates the distance from a 3D point \a point to a 3D triangle \a tri, namely
 * the distance between the closest point on \a tri to \a point.
 * \return
 * zTri3DDistFromPoint() returns the distance between \a point and \a tri.
 */
__ZEO_EXPORT ZEO_ELEM_TRIXD_DIST_FROM_POINT_PROTOTYPE( 3D );

/*! \brief signed closest point to a 3D triangle. */
__ZEO_EXPORT double zTri3DSignedClosest(const zTri3D *tri, const zVec3D *point, zVec3D *closestpoint);
/*! \brief signed distance from a 3D point to a 3D triangle. */
__ZEO_EXPORT double zTri3DSignedDistFromPoint(const zTri3D *tri, const zVec3D *point);

/*! \brief check if a point is inside of a triangle.
 *
 * zTri3DPointIsInside() checks if the given point \a point is inside of a triangle \a tri.
 * "\a point is inside of \a tri" means that the projection point of \a point to \a tri is inside of \a tri.
 * \a margin is a margin of the inside area outward from the boundary of \a tri.
 * \return
 * zTri3DPointIsInside() returns the true value if \a point is inside of \a tri with a margin \a margin.
 * Otherwise, it returns the false value.
 */
__ZEO_EXPORT ZEO_ELEM_TRIXD_POINT_IS_INSIDE_PROTOTYPE( 3D );

/*! \brief volume, inertia, barycenter and circumcenter of cone.
 *
 * zTri3DConeVolume() calculates the volume of a cone which consists
 * of a triangle \a tri as the base and a vector \a v as the vertex.
 *
 * zTri3DConeInertia() calculates the inertia tensor of a cone which
 * consists of \a tri and the original point, given its density by
 * \a density. The result is put into a 3x3 matrix \a i.
 *
 * zTri3DConeBarycenter() calculates the barycenter of a cone which
 * consists of \a tri and \a v and sets it into \a c.
 *
 * zTri3DConeCircumcenter() calculates the circumcenter of a cone which
 * consists of \a tri and the original point and puts it into \a c.
 * \return
 * zTri3DConeVolume() returns the volum calculated.
 *
 * zTri3DConeInertia() returns a pointer \a i.
 *
 * zTri3DConeBarycenter() returns a pointer \a c.
 *
 * zTri3DConeCircumcenter() returns a pointer \a c.
 */
__ZEO_EXPORT double zTri3DConeVolume(const zTri3D *tri, const zVec3D *v);
__ZEO_EXPORT zMat3D *zTri3DConeInertia(const zTri3D *tri, double density, zMat3D *i);
__ZEO_EXPORT zVec3D *zTri3DConeBarycenter(const zTri3D *tri, const zVec3D *v, zVec3D *c);
__ZEO_EXPORT zVec3D *zTri3DConeCircumcenter(const zTri3D *tri, zVec3D *c);

/*! \brief print a 3D triangle.
 *
 * zTri3DFPrint() prints coordinates of the vertices of a 3D triangle
 * \a tri to the current position of a file \a fp in the following format.
 *
 *  vert: 0 ( x0, y0, z0 )
 *  vert: 1 ( x1, y1, z1 )
 *  vert: 2 ( x2, y2, z2 )
 *  norm: ( nx, ny, nz )
 *
 * where vert 0 is the first vertex, vert 1 is the second, vert 2 is
 * the third and norm is for the normal vector.
 * zTri3DPrint() prints the same information of \a tri out to the
 * standard output.
 * \return
 * zTri3DFPrint() and zTri3DPrint() do not return any values.
 */
__ZEO_EXPORT void zTri3DFPrint(FILE *fp, const zTri3D *tri);
#define zTri3DPrint(tri) zTri3DFPrint( stdout, (tri) )

/*! \struct zTri3DArray
 * \brief array class of 3D triangles.
 */
zArrayClass( zTri3DArray, zTri3D );

/* ********************************************************** */
/* 3D tetrahedron.
 * ********************************************************** */

/*! \brief closest point on a tetrahedron composed by four given 3D points to the origin. */
__ZEO_EXPORT double zTetra3DClosestFromOrigin(const zVec3D *vert0, const zVec3D *vert1, const zVec3D *vert2, const zVec3D *vert3, double *s0, double *s1, double *s2, double *s3, zVec3D *closestpoint);
/*! \brief closest point on a tetrahedron composed by four given 3D points to a 3D point. */
__ZEO_EXPORT double zTetra3DClosest(const zVec3D *vert0, const zVec3D *vert1, const zVec3D *vert2, const zVec3D *vert3, const zVec3D *point, zVec3D *closestpoint);

__END_DECLS

#include <zeo/zeo_elem3d_box.h>    /* 3D box */
#include <zeo/zeo_elem3d_sphere.h> /* 3D sphere */

#include <zeo/zeo_elem3d_list.h> /* 3D shape element list */

#endif /* __ZEO_ELEM3D_H__ */
