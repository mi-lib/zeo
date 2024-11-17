/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs - NURBS curve / surface
 */
#ifndef __ZEO_NURBS_H__
#define __ZEO_NURBS_H__

#include <zeo/zeo_ph.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \struct zNURBS3DCPCell
 * \brief cell of 3D NURBS containing a control point and weight.
 *
 * zNURBS3DCPCell is a cell of 3D NURBS that contains a control
 * point and associated weight.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zNURBS3DCPCell ){
  zVec3D cp; /*!< control point */
  double w;  /*!< weight */
};

/* ********************************************************** */
/*! \struct zNURBS3DCPNet
 * \brief a net of control points for 3D NURBS.
 *
 * zNURBS3DCPNet is a net of control points for 3D NURBS.
 * It is defined with a macro zArray2Class.
 * \sa zArray2Class.
 *//* ******************************************************* */
zArray2Class( zNURBS3DCPNet, zNURBS3DCPCell );

/* ********************************************************** */
/*! \struct zNURBS3D
 * \brief 3D NURBS curve and surface.
 *
 * zNURBS3D is a 3D NURBS that represents curve and surface.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zNURBS3D ){
  zBSplineParam param[2]; /*!< \brief B-spline parameter */
  zNURBS3DCPNet cpnet;    /*!< \brief a net of control points */
};

#define zNURBS3DOrder(nurbs,i)       (nurbs)->param[(i)].order
#define zNURBS3DKnotNum(nurbs,i)     zBSplineParamKnotNum( &(nurbs)->param[(i)] )
#define zNURBS3DKnot(nurbs,i,j)      zBSplineParamKnot( &(nurbs)->param[(i)], j )
#define zNURBS3DSetKnot(nurbs,i,j,v) zBSplineParamSetKnot( &(nurbs)->param[(i)], j, v )
#define zNURBS3DKnotS(nurbs,i)       zBSplineParamKnotS( &(nurbs)->param[(i)] )
#define zNURBS3DKnotE(nurbs,i)       zBSplineParamKnotE( &(nurbs)->param[(i)] )
#define zNURBS3DKnotSlice(nurbs,i,k) zBSplineParamKnotSlice( &(nurbs)->param[(i)], k )
#define zNURBS3DSlice(nurbs,i)       (nurbs)->param[(i)].slice

/*! \brief set numbers of slices of a NURBS curve / surface. */
#define zNURBS3DSetSlice(nurbs,nu,nv) do{\
  zBSplineParamSetSlice( &(nurbs)->param[0], nu );\
  zBSplineParamSetSlice( &(nurbs)->param[1], nv );\
} while(0)

#define zNURBS3DCPNum(nurbs,i)         zArray2Size(&(nurbs)->cpnet,i)
#define zNURBS3DWeight(nurbs,i,j)      ( zArray2ElemNC(&(nurbs)->cpnet,i,j)->w )
#define zNURBS3DSetWeight(nurbs,i,j,v) ( zNURBS3DWeight(nurbs,i,j) = (v) )
#define zNURBS3DCP(nurbs,i,j)          ( &zArray2ElemNC(&(nurbs)->cpnet,i,j)->cp )
#define zNURBS3DSetCP(nurbs,i,j,v)     zVec3DCopy( v, zNURBS3DCP(nurbs,i,j) )

#define zNURBS3D1KnotNum(nurbs)        zNURBS3DKnotNum(nurbs,1)
#define zNURBS3D1Knot(nurbs,j)         zNURBS3DKnot(nurbs,1,j)
#define zNURBS3D1SetKnot(nurbs,j,v)    zNURBS3DSetKnot(nurbs,1,j,v)
#define zNURBS3D1KnotS(nurbs)          zNURBS3DKnotS(nurbs,1)
#define zNURBS3D1KnotE(nurbs)          zNURBS3DKnotE(nurbs,1)
#define zNURBS3D1KnotSlice(nurbs,k)    zNURBS3DKnotSlice(nurbs,1,k)
#define zNURBS3D1Slice(nurbs)          zNURBS3DSlice(nurbs,1)
#define zNURBS3D1SetSlice(nurbs,k)     zNURBS3DSetSlice(nurbs,0,k)

#define zNURBS3D1CPNum(nurbs)          zNURBS3DCPNum(nurbs,1)
#define zNURBS3D1Weight(nurbs,j)       zNURBS3DWeight(nurbs,0,j)
#define zNURBS3D1SetWeight(nurbs,j,v)  zNURBS3DSetWeight(nurbs,0,j,v)
#define zNURBS3D1CP(nurbs,j)           zNURBS3DCP(nurbs,0,j)
#define zNURBS3D1SetCP(nurbs,j,v)      zNURBS3DSetCP(nurbs,0,j,v)

#define ZEO_NURBS3D_DEFAULT_SLICE  20

/*! \brief allocate a NURBS curve / surface.
 *
 * zNURBS3D1Alloc() allocates a NURBS curve \a nurbs from a given
 * number of control points and the order in the axis. The control
 * points and associated weights can be assigned afterward.
 * \a order is the order of parameter for the curve, which has to
 * satisfy \a order < \a size.
 * Knots are initialized as a uniform Bezier spline curve with fixed
 * boundary points. The knot vector can be modified later.
 *
 * zNURBS3DAlloc() allocates a NURBS surface \a nurbs from given
 * two numbers for a net of control points and two orders in each
 * axis. The control points and associated weights can be assigned
 * afterward.
 * \a order1 and \a order2 are the orders of each parameter for the
 * surface, which have to satisfy \a order1 < \a size1 and \a order2 < \a size2.
 * Knots are initialized as a uniform Bezier spline surface with
 * fixed boundary points. The knot vectors can be modified later.
 * \return
 * zNURBS3D1Alloc() and zNURBS3D2Alloc() return the true value if
 * they succeed to allocate memory of the NURBS3D curve / surface.
 * If the order is larger than or equal to the number of control
 * points in corresponding axis plus one or they fail to allocate
 * memory, the false value is returned.
 */
__ZEO_EXPORT bool zNURBS3DAlloc(zNURBS3D *nurbs, int size1, int size2, int order1, int order2);
#define zNURBS3D1Alloc(nurbs,size,order) zNURBS3DAlloc( nurbs, 1, size, 0, order )

/*! \brief initialize a NURBS curve / surface.
 *
 * zNURBS3DInit() initializes a NURBS curve / surface \a nurbs
 * by reseting properties.
 */
__ZEO_EXPORT zNURBS3D *zNURBS3DInit(zNURBS3D *nurbs);

/*! \brief destroy a NURBS curve / surface.
 *
 * zNURBS3DDestroy() destroys a NURBS curve / surface \a nurbs.
 */
__ZEO_EXPORT void zNURBS3DDestroy(zNURBS3D *nurbs);

/*! \brief copy a NURBS curve / surface. */
__ZEO_EXPORT zNURBS3D *zNURBS3DCopy(zNURBS3D *src, zNURBS3D *dest);
/*! \brief clone a NURBS curve / surface. */
__ZEO_EXPORT zNURBS3D *zNURBS3DClone(zNURBS3D *src, zNURBS3D *dest);
/*! \brief mirror a NURBS curve / surface about specified axis. */
__ZEO_EXPORT zNURBS3D *zNURBS3DMirror(zNURBS3D *src, zNURBS3D *dest, zAxis axis);
/*! \brief transform control points of a NURBS curve / surface. */
__ZEO_EXPORT zNURBS3D *zNURBS3DXform(zNURBS3D *src, zFrame3D *f, zNURBS3D *dest);
/*! \brief inversely transform control points of a NURBS curve / surface. */
__ZEO_EXPORT zNURBS3D *zNURBS3DXformInv(zNURBS3D *src, zFrame3D *f, zNURBS3D *dest);

/*! \brief normalize the knot vectors of a NURBS curve / surface.
 *
 * zNURBS3DKnotNormalize() normalizes knot vectors of a NURBS
 * curve / surface \a nurbs so that it starts from 0 and ends to 1.
 */
__ZEO_EXPORT void zNURBS3DKnotNormalize(zNURBS3D *nurbs);

/*! \brief compute a vector on NURBS curve / surface.
 *
 * zNURBS3D1Vec() computes a 3D vector on a NURBS curve \a nurbs
 * with respect to a given parameter \a t. The result is put into
 * \a p.
 *
 * zNURBS3DVec() computes a 3D vector on a NURBS curve / surface
 * \a nurbs with respect to given parameters \a u and \a v. The
 * result is put into \a p.
 * \return
 * zNURBS3D1Vec() returns a pointer \a v if \a t is valid.
 * zNURBS3DVec() returns a pointer \a v if \a u and \a v are valid.
 * Otherwise, the null vector is returned.
 */
__ZEO_EXPORT zVec3D *zNURBS3DVec(zNURBS3D *nurbs, double u, double v, zVec3D *p);
#define zNURBS3D1Vec(nurbs,t,p) zNURBS3DVec( nurbs, 0, t, p )

/*! \brief compute a position and normal vectors on NURBS surface.
 *
 * zNURBS3DVecNorm() computes a 3D position vector on a NURBS surface
 * \a nurbs together with its normal and tangential vectors at a given
 * parameter \a u and \a v. The position vector and the normal vector
 * are put into \a p and \a n, respectively. The two tangential vectors
 * are put into \a t1 and \a t2, unless the null pointers are given
 * for them.
 * \return
 * zNURBS3DVecNorm() returns a pointer \a p.
 */
__ZEO_EXPORT zVec3D *zNURBS3DVecTSpace(zNURBS3D *nurbs, double u, double v, zVec3D *p, zVec3D *n, zVec3D *t1, zVec3D *t2);

#define zNURBS3DVecTan(nurbs,u,v,p,t1,t2) zNURBS3DVecTSpace( nurbs, u, v, p, NULL, t1, t2 )
#define zNURBS3DVecNorm(nurbs,u,v,p,n)    zNURBS3DVecTSpace( nurbs, u, v, p, n, NULL, NULL )
#define zNURBS3D1VecTan(nurbs,u,v,p,t)    zNURBS3DVecTan( nurbs, u, v, p, NULL, t )

/*! \brief closest point of a 3D point on a NURBS surface.
 *
 * zNURBS3DClosest() finds the closest point on a NURBS surface
 * \a nurbs from a point \a p based on Levenberg-Merquardt method.
 * The result is put into \a cp.
 * If \a u and \a v are non-null, the corresponding parameters are
 * stored.
 * \return
 * zNURBS3DClosest() returns the distance between \a p and \a cp.
 */
__ZEO_EXPORT double zNURBS3DClosest(zNURBS3D *nurbs, zVec3D *p, zVec3D *cp, double *u, double *v);

/*! \brief convert NURBS surface to a polyhedron.
 *
 * zNURBS3DToPH() converts a NURBS surface to a polyhedron.
 * The surface is divided by the number of slices in each axis
 * into triangles.
 * \return
 * zNURBS3DToPH() returns a pointer \a ph if it succeeds to
 * convert the polyhedron. If it fails to allocate memory for
 * the polyhedron, the null pointer is returned.
 */
__ZEO_EXPORT zPH3D *zNURBS3DToPH(zNURBS3D *nurbs, zPH3D *ph);

/* I/O */

#define ZTK_KEY_ZEO_NURBS_ORDER "order"
#define ZTK_KEY_ZEO_NURBS_UKNOT "uknot"
#define ZTK_KEY_ZEO_NURBS_VKNOT "vknot"
#define ZTK_KEY_ZEO_NURBS_SIZE  "size"
#define ZTK_KEY_ZEO_NURBS_CP    "cp"
#define ZTK_KEY_ZEO_NURBS_SLICE "slice"

/*! \brief read a 3D NURBS from a ZTK format processor. */
__ZEO_EXPORT zNURBS3D *zNURBS3DFromZTK(zNURBS3D *nurbs, ZTK *ztk);

/*! \brief print information of a 3D NURBS surface out to a file. */
__ZEO_EXPORT void zNURBS3DFPrintZTK(FILE *fp, zNURBS3D *nurbs);

__END_DECLS

#endif /* __ZEO_NURBS_H__ */
