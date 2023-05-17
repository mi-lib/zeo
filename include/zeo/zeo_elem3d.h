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
/*! \brief classifier for tight/strict/loose in/out check
 *//* ******************************************************* */
typedef enum{
  ZEO_RIM_TIGHT, ZEO_RIM_STRICT, ZEO_RIM_LOOSE,
} zRimType;

/* ********************************************************** */
/*! \brief 3D plane class.
 *//********************************************************* */
typedef struct{
  zVec3D *vert, norm;
} zPlane3D;

#define zPlane3DVert(p)      ( (p)->vert )
#define zPlane3DNorm(p)      ( &(p)->norm )

#define zPlane3DSetVert(p,v) ( zPlane3DVert(p) = (v) )
#define zPlane3DSetNorm(p,n) zVec3DNormalize( (n), zPlane3DNorm(p) )

/*! \brief initialize and create a 3D plane.
 *
 * zPlane3DInit() initializes a 3D plane \a p as the x-y plane.
 *
 * zPlane3DCreate() creates a 3D plane which is defined by the pass
 * point \a v and the normal vector \a n.
 * \return
 * zPlane3DInit() and zPlane3DCreate() return a pointer \a p.
 */
__ZEO_EXPORT zPlane3D *zPlane3DCreate(zPlane3D *p, zVec3D *v, zVec3D *n);
#define zPlane3DInit(p) zPlane3DCreate( (p), ZVEC3DZERO, ZVEC3DZ )

/*! \brief distance between a 3D point and a 3D plane, and projection of
 *   a point to a 3D plane.
 *
 * zPlane3DPointDist() calculates the distance between a 3D point \a v
 * and a 3D plane \a p.
 *
 * zPlane3DPointIsOn() checks if a point \a v is on a plane \a p.
 *
 * zPlane3DProj() calculates the projection point of a 3D point \a v
 * to a 3D plane \a p. The result is put into the vector pointed by \a cp.
 * \return
 * zPlane3DPointDist() returns the distance calculated.
 *
 * zPlane3DPointIsOn() returns the true value if \a v is on \a p,
 * or the false value otherwise.
 *
 * zPlane3DProj() returns the distance between \a p and \a v.
 */
__ZEO_EXPORT double zPlane3DPointDist(zPlane3D *p, zVec3D *v);
__ZEO_EXPORT bool zPlane3DPointIsOn(zPlane3D *p, zVec3D *v);
__ZEO_EXPORT double zPlane3DProj(zPlane3D *p, zVec3D *v, zVec3D *cp);

/*! \brief mean plane of set of points.
 *
 * zPlane3DMean() calculates the mean plane \a pl of a set of points \a v.
 * \a n is the number of points. Namely, \a pl is the plane to which the
 * mean distance from every points is minimized.
 *
 * the mean point of \a v, which is also on \a pl, is stored where \a pc
 * points.
 * \return
 * zPlane3DMean() returns a pointer \a pl.
 */
__ZEO_EXPORT zPlane3D *zPlane3DMean(zPlane3D *pl, zVec3D *pc, zVec3D v[], int n);

/*! \brief print a 3D plane.
 *
 * zPlane3DFPrint() prints information of a 3D plane \a p out
 * to the current position of the file \a fp in the following format:
 *
 * vert: ( x, y, z ) <- the passing point
 * norm: ( x, y, z ) <- the normal vector
 *
 * zPlane3DPrint() prints information of the plane \a p out to
 * the standard out.
 * \return
 * Neither zPlane3DFPrint() nor zPlane3DPrint() return any values.
 */
__ZEO_EXPORT void zPlane3DFPrint(FILE *fp, zPlane3D *p);
#define zPlane3DPrint(p) zPlane3DFPrint( stdout, (p) )

/* ********************************************************** */
/*! \brief 3D edge class.
 *//********************************************************* */
typedef struct{
  zVec3D *vert[2]; /*!< \brief two vertices */
  zVec3D vec;      /*!< \brief edge vector */
} zEdge3D;

#define zEdge3DVert(e,i)      (e)->vert[(i)]
#define zEdge3DVec(e)         ( &(e)->vec )

#define zEdge3DSetVert(e,i,p) ( (e)->vert[(i)] = (p) )
#define zEdge3DSetVec(e,v)    zVec3DCopy( v, zEdge3DVec(e) )

/*! \brief initialize and create of a 3D edge.
 *
 * zEdge3DInit() initializes an edge \a e, setting both endpoints for
 * the null vectors.
 *
 * zEdge3DCreate() creates a edge from two endpoints \a v1 and \a v2.
 * \return
 * Each of zEdge3DInit() and zEdge3DCreate() returns a pointer \a e.
 */
__ZEO_EXPORT ZEDGEXD_INIT_PROTOTYPE( 3D );
__ZEO_EXPORT ZEDGEXD_CREATE_PROTOTYPE( 3D );

/*! \brief path vector of a 3D edge.
 *
 * zEdge3DCalcVec() calculates the path vector from the first endpoint
 * to the second of an edge \a e. The vector is contained by the edge
 * itself within. One can access the path vector by calling zEdge3DVec().
 * \return
 * zEdge3DCalcVec() returns the pointer to the path vector.
 */
__ZEO_EXPORT ZEDGEXD_CALC_VEC_PROTOTYPE( 3D );

/*! \brief distance between a point and a 3D edge.
 *
 * zEdge3DProj() finds the projection of a given 3D point \a p onto
 * a 3D edge \a e. The result is put into \a cp.
 *
 * zEdge3DPointDist() calculates a distance between an edge \a e and a
 * point \a p. \a e is regarded as an infinite-length edge, namely, it
 * returns the length of the perpendicular from \a p to \a e.
 *
 * zEdge3DClosest(), on the contrary, calculates the actual closest point
 * on \a e from \a p and sets it into \a cp. When the perpendicular from
 * \a p to \a e does not cross with \a e, it returns the closest endpoint
 * of \a e from \a p.
 * \return
 * zEdge3DPointDist() returns the distance calculated, nameyl, the length
 * of the perpendicular from \a p to \a e.
 *
 * zEdge3DClosest() returns the distance between \a p and \a cp.
 */
__ZEO_EXPORT ZEDGEXD_PROJ_PROTOTYPE( 3D );
__ZEO_EXPORT ZEDGEXD_POINT_DIST_PROTOTYPE( 3D );

__ZEO_EXPORT bool zEdge3DPointIsOn(zEdge3D *e, zVec3D *p);

__ZEO_EXPORT ZEDGEXD_LINSCALE_PROTOTYPE( 3D );
__ZEO_EXPORT ZEDGEXD_CLOSEST_PROTOTYPE( 3D );

/*! \brief contiguous vertix of edge to a point.
 *
 * zEdge3DContigVert() returns the contiguous vertix of an edge \a e
 * to a given point \a p (namely, the value returned has to be either
 * the first or the second vertix of \a e).
 * \return
 * zEdge3DContigVert() returns a pointer to the found vertix.
 */
__ZEO_EXPORT zVec3D *zEdge3DContigVert(zEdge3D *e, zVec3D *p, double *d);

/*! \brief print a 3D edge.
 *
 * zEdge3DFPrint() prints coordinates of the endpoints of a 3D
 * edge to the current position of a file \a fp in the following format.
 *
 *  vert: 0 ( x0, y0, z0 )
 *  vert: 1 ( x1, y1, z1 )
 *  vec: ( vx, vy, vz )
 *
 * where vert 0 is the first endpoint, vert 1 is the second and
 * vec is for the path vector.
 * \return
 * zEdge3DFPrint() and zEdge3DPrint() return no values.
 */
__ZEO_EXPORT void zEdge3DFPrint(FILE *fp, zEdge3D *e);
#define zEdge3DPrint(e) zEdge3DFPrint( stdout, (e) )

/* ********************************************************** */
/*! \brief 3D triangle class.
 *//********************************************************* */
typedef struct{
  zVec3D *v[3]; /*!< vertices */
  zVec3D norm;  /*!< normal vector */
} zTri3D;

#define zTri3DNorm(t)        ( &(t)->norm )
#define zTri3DVert(t,i)      (t)->v[i]
#define zTri3DVertNext(t,i)  zTri3DVert( t, ( (i)+1 ) % 3 )

#define zTri3DSetNorm(t,n)   zVec3DCopy( n, zTri3DNorm(t) )
#define zTri3DSetVert(t,i,v) ( zTri3DVert(t,i) = (v) )

/*! \brief initialize and create a 3D triangle.
 *
 * zTri3DInit() initializes a 3D triangle instance \a t, setting three
 * vertices for the null vector.
 *
 * zTri3DCreate() creates a 3D triangle from three vertices \a v1, \a v2
 * and \a v3.
 * zTri3DCreateRev() creates a 3D triangle from \a v1, \a v2 and \a v3 in
 * reversed order of zTri3DCreate(), i.e. it is equivalent to
 * zTri3DCreate( v1, v3, v2 ).
 *
 * zTri3DRev() reverses the source triangle \a src, and create the
 * destination triangle \a dest. It is permitted to let \a dest point
 * to the same address with \a src. Actually, zTri3DRevDRC(), which is
 * the destructive version of zTri3DRev() is defined as so.
 * \return
 * zTri3DInit(), zTri3DCreate(), zTri3DCreateRev() and zTri3DRevDRC()
 * return a pointer \a t.
 *
 * zTri3DRev() returns a pointer \a dest.
 */
__ZEO_EXPORT zTri3D *zTri3DInit(zTri3D *t);
__ZEO_EXPORT zTri3D *zTri3DCreate(zTri3D *t, zVec3D *v1, zVec3D *v2, zVec3D *v3);
__ZEO_EXPORT zTri3D *zTri3DCreateRev(zTri3D *t, zVec3D *v1, zVec3D *v2, zVec3D *v3);
__ZEO_EXPORT zTri3D *zTri3DRev(zTri3D *src, zTri3D *dest);
#define zTri3DRevDRC(t) zTri3DRev( t, t )

/*! \brief area and normal vector of a 3D triangle.
 *
 * zTri3DArea() calculates the area of a triangle \a t.
 *
 * zTri3DCalcNorm() calculates the normal vector of triangle \a t.
 * The vector is contained by the triangle itself within. One can access
 * the normal vector by calling zTri3DNorm() (see zeo_elem.h).
 * \return
 * zTri3DArea() returns the area calculated.
 * zTri3DCalcNorm() returns a pointer to the normal vector.
 */
__ZEO_EXPORT double zTri3DArea(zTri3D *t);
__ZEO_EXPORT zVec3D *zTri3DCalcNorm(zTri3D *t);

/*! \brief various centers of a 3D triangle.
 *
 * zTri3DBarycenter(), zTri3DCircumcenter(), zTri3DIncenter() and
 * zTri3DOrthocenter() calculate barycenter, circumcenter, incenter
 * and orthocenter of \a t, respectively.
 * The result is put into \a c.
 * \return
 * zTri3DBarycenter(), zTri3DCircumcenter(), zTri3DIncenter() and
 * zTri3DOrthocenter() return a pointer \a c.
 */
__ZEO_EXPORT ZTRIXD_BARYCENTER_PROTOTYPE( 3D );
__ZEO_EXPORT ZTRIXD_CIRCUMCENTER_PROTOTYPE( 3D );
__ZEO_EXPORT ZTRIXD_INCENTER_PROTOTYPE( 3D );
__ZEO_EXPORT ZTRIXD_ORTHOCENTER_PROTOTYPE( 3D );

/*! \brief convert a triangle to a plane.
 *
 * zTri3DToPlane3D() converts a given 3D triangle \a t to an infinite
 * 3D plane \a p which \a t is on.
 * \return
 * zTri3DToPlane3D() returns a pointer \a t.
 */
#define zTri3DToPlane3D(t,p) zPlane3DCreate( p, zTri3DVert(t,0), zTri3DNorm(t) )

/*! \brief contiguous vertix of triangle to a point.
 *
 * zTri3DContigVert() returns the contiguous vertix of a triangle \a t
 * to a given point \a p. Namely, the value returned has to be any of
 * three vertices of \a t.
 * \return
 * zTri3DContigVert() returns a pointer to the found vertix.
 */
__ZEO_EXPORT zVec3D *zTri3DContigVert(zTri3D *t, zVec3D *p, double *d);

/*! \brief distance between a point and a triangle.
 *
 * zTri3DPointDist() calculates the signed distance between a triangle
 * \a t and a point \a v. The result is
 *  - a positive value when \a v is above \a t (\a v exists in
 *    the direction of the normal vector of \a t),
 *  - a negative value when \a v is below \a t (\a v exists in
 *    the opposite direction of the normal vector of \a t),
 *  - zero when \a v is on \a t.
 * The funciton does not care if the projection point of \a v is
 * inside of \a t or not, but only calculates the length of the
 * perpendicular from \a v to \a t.
 *
 * zTri3DProj() calculates the projection point of \a v into a triangle
 * \a t - the footpoint of the perpendicular from \a v to \a t, and sets
 * it into \a cp.
 * \return
 * zTri3DPointDist() and zTri3DProj() return the distance between \a v
 * and \a t.
 */
__ZEO_EXPORT double zTri3DPointDist(zTri3D *t, zVec3D *v);
__ZEO_EXPORT bool zTri3DPointIsOn(zTri3D *t, zVec3D *v);
__ZEO_EXPORT double zTri3DProj(zTri3D *t, zVec3D *v, zVec3D *cp);

/*! \brief check if a point is inside of a triangle.
 *
 * zTri3DPointIsInside() checks if the given point \a v is inside of
 * a triangle \a t. "\a v is inside of \a p" means that the projection
 * point of \a v to \a t is inside of \a t.
 *
 * \a margin is a margin of the inside area outward from the boundary
 * of \a t.
 * \return
 * zTri3DPointIsInside() returns the true value if \a v is inside of \a t
 * with a margin \a margin. Otherwise, the false value is returned.
 */
__ZEO_EXPORT bool zTri3DPointIsInside(zTri3D *t, zVec3D *v, double margin);

/*! \brief the closest point from a point to a 3D triangle.
 *
 * zTri3DClosest() calculates the closest point on a triangle \a t from
 * the given point \a v.
 * It sets the point calculated into \a cp.
 * \return
 * zTri3DClosest() returns the distance from \a v to the closest point
 * calculated.
 */
__ZEO_EXPORT double zTri3DLinScale(zTri3D *t, zVec3D *p, double *l0, double *l1, double *l2, zVec3D *cp);
__ZEO_EXPORT double zTri3DClosest(zTri3D *t, zVec3D *v, zVec3D *cp);

/*! \brief volume, inertia, barycenter and circumcenter of cone.
 *
 * zTri3DConeVolume() calculates the volume of a cone which consists
 * of a triangle \a t as the base and a vector \a v as the vertex.
 *
 * zTri3DConeInertia() calculates the inertia tensor of a cone which
 * consists of \a t and the original point, given its density by
 * \a density. The result is put into a 3x3 matrix \a i.
 *
 * zTri3DConeBarycenter() calculates the barycenter of a cone which
 * consists of \a t and \a v and sets it into \a c.
 *
 * zTri3DConeCircumcenter() calculates the circumcenter of a cone which
 * consists of \a t and the original point and puts it into \a c.
 * \return
 * zTri3DConeVolume() returns the volum calculated.
 *
 * zTri3DConeInertia() returns a pointer \a i.
 *
 * zTri3DConeBarycenter() returns a pointer \a c.
 *
 * zTri3DConeCircumcenter() returns a pointer \a c.
 */
__ZEO_EXPORT double zTri3DConeVolume(zTri3D *t, zVec3D *v);
__ZEO_EXPORT zMat3D *zTri3DConeInertia(zTri3D *t, double density, zMat3D *i);
__ZEO_EXPORT zVec3D *zTri3DConeBarycenter(zTri3D *t, zVec3D *v, zVec3D *c);
__ZEO_EXPORT zVec3D *zTri3DConeCircumcenter(zTri3D *t, zVec3D *c);

/*! \brief print a 3D triangle.
 *
 * zTri3DFPrint() prints coordinates of the vertices of a 3D triangle
 * \a t to the current position of a file \a fp in the following format.
 *
 *  vert: 0 ( x0, y0, z0 )
 *  vert: 1 ( x1, y1, z1 )
 *  vert: 2 ( x2, y2, z2 )
 *  norm: ( nx, ny, nz )
 *
 * where vert 0 is the first vertex, vert 1 is the second, vert 2 is
 * the third and norm is for the normal vector.
 * zTri3DPrint() prints the same information of \a t out to the
 * standard output.
 * \return
 * zTri3DFPrint() and zTri3DPrint() do not return any values.
 */
__ZEO_EXPORT void zTri3DFPrint(FILE *fp, zTri3D *t);
#define zTri3DPrint(t) zTri3DFPrint( stdout, (t) )

/*! \struct zTri3DArray
 * \brief array class of 3D triangles.
 */
zArrayClass( zTri3DArray, zTri3D );

__END_DECLS

#include <zeo/zeo_elem3d_list.h> /* 3D shape element list */

#endif /* __ZEO_ELEM3D_H__ */
