/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs - NURBS curve / surface
 */
#include <zeo/zeo_ph.h>

#ifndef __ZEO_NURBS_H__
#define __ZEO_NURBS_H__

__BEGIN_DECLS

/* ********************************************************** */
/*! \struct zNURBS3DCPCell
 * \brief cell of 3D NURBS containing a control point and weight.
 *
 * zNURBS3DCPCell is a cell of 3D NURBS that contains a control
 * point and associated weight.
 *//* ******************************************************* */
typedef struct{
  zVec3D cp; /*!< control point */
  double w;  /*!< weight */
} zNURBS3DCPCell;

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
typedef struct{
  int dim[2];   /*!< \brief dimensions of a curve in two axes */
  zVec knot[2]; /*!< \brief knot vectors */
  int ns[2];    /*!< \brief number of slices in each axis */
  /*! \cond */
  zNURBS3DCPNet cpnet; /* a net of control points */
  /*! \endcond */
} zNURBS3D;

#define zNURBS3DKnotNum(n,i)       zVecSizeNC((n)->knot[i])
#define zNURBS3DKnot(n,i,j)        zVecElemNC((n)->knot[i],j)
#define zNURBS3DSetKnot(n,i,j,v)   ( zNURBS3DKnot(n,i,j) = (v) )
#define zNURBS3DKnotS(n,i)         zNURBS3DKnot(n,i,(n)->dim[i])
#define zNURBS3DKnotE(n,i)         zNURBS3DKnot(n,i,zNURBS3DCPNum(n,i))
#define zNURBS3DKnotSlice(n,i,k)   ( ( zNURBS3DKnotE(n,i) - zNURBS3DKnotS(n,i) ) * k / (n)->ns[i] + zNURBS3DKnotS(n,i) )

#define zNURBS3DCPNum(n,i)         zArray2Size(&(n)->cpnet,i)
#define zNURBS3DWeight(n,i,j)      ( zArray2ElemNC(&(n)->cpnet,i,j)->w )
#define zNURBS3DSetWeight(n,i,j,v) ( zNURBS3DWeight(n,i,j) = (v) )
#define zNURBS3DCP(n,i,j)          ( &zArray2ElemNC(&(n)->cpnet,i,j)->cp )
#define zNURBS3DSetCP(n,i,j,v)     zVec3DCopy( v, zNURBS3DCP(n,i,j) )

#define zNURBS3D1KnotNum(n)        zNURBS3DKnotNum(n,1)
#define zNURBS3D1Knot(n,j)         zNURBS3DKnot(n,1,j)
#define zNURBS3D1SetKnot(n,j,v)    zNURBS3DSetKnot(n,1,j,v)
#define zNURBS3D1KnotS(n)          zNURBS3DKnotS(n,1)
#define zNURBS3D1KnotE(n)          zNURBS3DKnotE(n,1)
#define zNURBS3D1KnotSlice(n,k)    zNURBS3DKnotSlice(n,1,k)

#define zNURBS3D1CPNum(n)          zNURBS3DCPNum(n,1)
#define zNURBS3D1Weight(n,j)       zNURBS3DWeight(n,0,j)
#define zNURBS3D1SetWeight(n,j,v)  zNURBS3DSetWeight(n,0,j,v)
#define zNURBS3D1CP(n,j)           zNURBS3DCP(n,0,j)
#define zNURBS3D1SetCP(n,j,v)      zNURBS3DSetCP(n,0,j,v)

#define ZEO_NURBS3D_DEFAULT_NS     20

/*! \brief allocate a NURBS curve / surface.
 *
 * zNURBS3D1Alloc() allocates a NURBS curve \a nurbs from a given
 * number of control points and the dimension in the axis. The
 * control points and associated weights can be assigned afterward.
 * \a dim is the dimension of parameter for the curve, which has to
 * satisfy \a dim < \a size.
 * Knots are initialized as a uniform Bezier spline curve with fixed
 * boundary points. The knot vector can be modified later.
 *
 * zNURBS3DAlloc() allocates a NURBS surface \a nurbs from given
 * two numbers for a net of control points and two dimensions in
 * each axis. The control points and associated weights can be
 * assigned afterward.
 * \a dim1 and \a dim2 are the dimensions of each parameter for the
 * surface, which have to satisfy \a dim1 < \a size1 and \a dim2 < \a size2.
 * Knots are initialized as a uniform Bezier spline surface with
 * fixed boundary points. The knot vectors can be modified later.
 * \return
 * zNURBS3D1Alloc() and zNURBS3D2Alloc() return the true value if
 * they succeed to allocate memory of the NURBS3D curve / surface.
 * If the dimension is larger than or equal to the number of control
 * points in corresponding axis plus one or they fail to allocate
 * memory, the false value is returned.
 */
__EXPORT bool zNURBS3DAlloc(zNURBS3D *nurbs, int size1, int size2, int dim1, int dim2);
#define zNURBS3D1Alloc(nurbs,size,dim) zNURBS3DAlloc( nurbs, 1, size, 0, dim )

/*! \brief set numbers of slices of a NURBS curve / surface. */
#define zNURBS3DSetSliceNum(nurbs,nu,nv) do{\
  (nurbs)->ns[0] = (nu);\
  (nurbs)->ns[1] = (nv);\
} while(0)

/*! \brief initialize a NURBS curve / surface.
 *
 * zNURBS3DInit() initializes a NURBS curve / surface \a nurbs
 * by reseting properties.
 */
__EXPORT zNURBS3D *zNURBS3DInit(zNURBS3D *nurbs);

/*! \brief destroy a NURBS curve / surface.
 *
 * zNURBS3DDestroy() destroys a NURBS curve / surface \a nurbs.
 */
__EXPORT void zNURBS3DDestroy(zNURBS3D *nurbs);

/*! \brief copy a NURBS curve / surface. */
__EXPORT zNURBS3D *zNURBS3DCopy(zNURBS3D *src, zNURBS3D *dest);
/*! \brief clone a NURBS curve / surface. */
__EXPORT zNURBS3D *zNURBS3DClone(zNURBS3D *src, zNURBS3D *dest);
/*! \brief mirror a NURBS curve / surface about specified axis. */
__EXPORT zNURBS3D *zNURBS3DMirror(zNURBS3D *src, zNURBS3D *dest, zAxis axis);
/*! \brief transfer control points of a NURBS curve / surface. */
__EXPORT zNURBS3D *zNURBS3DXfer(zNURBS3D *src, zFrame3D *f, zNURBS3D *dest);
/*! \brief inversely transfer control points of a NURBS curve / surface. */
__EXPORT zNURBS3D *zNURBS3DXferInv(zNURBS3D *src, zFrame3D *f, zNURBS3D *dest);

/*! \brief normalize the knot vectors of a NURBS curve / surface.
 *
 * zNURBS3DKnotNormalize() normalizes knot vectors of a NURBS
 * curve / surface \a nurbs so that it starts from 0 and ends to 1.
 */
__EXPORT void zNURBS3DKnotNormalize(zNURBS3D *nurbs);

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
__EXPORT zVec3D *zNURBS3DVec(zNURBS3D *nurbs, double u, double v, zVec3D *p);
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
__EXPORT zVec3D *zNURBS3DVecNorm(zNURBS3D *nurbs, double u, double v, zVec3D *p, zVec3D *n, zVec3D *t1, zVec3D *t2);

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
__EXPORT double zNURBS3DClosest(zNURBS3D *nurbs, zVec3D *p, zVec3D *cp, double *u, double *v);

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
__EXPORT zPH3D *zNURBS3DToPH(zNURBS3D *nurbs, zPH3D *ph);

/* I/O */

/*! \brief scan information of a 3D NURBS surface from a file. */
__EXPORT zNURBS3D *zNURBS3DFScan(FILE *fp, zNURBS3D *nurbs);

/*! \brief print information of a 3D NURBS surface out to a file. */
__EXPORT void zNURBS3DFPrint(FILE *fp, zNURBS3D *nurbs);

__END_DECLS

#endif /* __ZEO_NURBS_H__ */
