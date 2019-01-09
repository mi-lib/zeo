/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs3d - NURBS surface
 */
#include <zeo/zeo_vec3d.h>

#ifndef __ZEO_NURBS3D_H__
#define __ZEO_NURBS3D_H__

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
  /*! \cond */
  zVec knot[2]; /* knot vector */
  zNURBS3DCPNet cpnet; /* a net of control points */
  /*! \endcond */
} zNURBS3D;

#define zNURBS3DKnotNum(n,i)       ( zVecSizeNC((n)->knot[i])-1 )
#define zNURBS3DKnot(n,i,j)        zVecElemNC((n)->knot[i],j)
#define zNURBS3DSetKnot(n,i,j,v)   ( zNURBS3DKnot(n,i,j) = (v) )
#define zNURBS3DKnot0(n,i)         zNURBS3DKnot(n,i,0)
#define zNURBS3DKnotE(n,i)         zNURBS3DKnot(n,i,zNURBS3DKnotNum(n,i)-1)
#define zNURBS3DKnotSlice(n,i,k,s) ( ( zNURBS3DKnotE(n,i) - zNURBS3DKnot0(n,i) ) * k / s + zNURBS3DKnot0(n,i) )

#define zNURBS3DCPNum(n,i)         zArray2Size(&(n)->cpnet,i)
#define zNURBS3DWeight(n,i,j)      ( zArray2ElemNC(&(n)->cpnet,i,j)->w )
#define zNURBS3DSetWeight(n,i,j,v) ( zNURBS3DWeight(n,i,j) = (v) )
#define zNURBS3DCP(n,i,j)          ( &zArray2ElemNC(&(n)->cpnet,i,j)->cp )
#define zNURBS3DSetCP(n,i,j,v)     zVec3DCopy( v, zNURBS3DCP(n,i,j) )

#define zNURBS3D1KnotNum(n)        zNURBS3DKnotNum(n,1)
#define zNURBS3D1Knot(n,j)         zNURBS3DKnot(n,1,j)
#define zNURBS3D1SetKnot(n,j,v)    zNURBS3DSetKnot(n,1,j,v)
#define zNURBS3D1Knot0(n)          zNURBS3DKnot0(n,1)
#define zNURBS3D1KnotE(n)          zNURBS3DKnotE(n,1)
#define zNURBS3D1KnotSlice(n,k,s)  zNURBS3DKnotSlice(n,1,k,s)

#define zNURBS3D1CPNum(n)          zNURBS3DCPNum(n,1)
#define zNURBS3D1Weight(n,j)       zNURBS3DWeight(n,0,j)
#define zNURBS3D1SetWeight(n,j,v)  zNURBS3DSetWeight(n,0,j,v)
#define zNURBS3D1CP(n,j)           zNURBS3DCP(n,0,j)
#define zNURBS3D1SetCP(n,j,v)      zNURBS3DSetCP(n,0,j,v)

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

/*! \brief destroy a NURBS curve.
 *
 * zNURBS3DDestroy() destroys a NURBS curve / surface \a nurbs.
 */
__EXPORT void zNURBS3DDestroy(zNURBS3D *nurbs);

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

/*! \brief nearest neighbor of a 3D point on a NURBS surface.
 *
 * zNURBS3DVecNN() finds the nearest-neighbor vector on a NURBS curve
 * defined by \a nurbs from a point \a p based on Levenberg-Merquardt
 * method. The result is put into \a nn.
 * If \a u and \a v are non-null, the corresponding parameters are stored.
 * \return
 * zNURBS3D1VecNN() returns the parameter corresponding to the nearest-
 * neighbor vector found by this function.
 */
__EXPORT double zNURBS3DVecNN(zNURBS3D *nurbs, zVec3D *p, zVec3D *nn, double *u, double *v);

/* for debug */

__EXPORT void zNURBS3DKnotFWrite(FILE *fp, zNURBS3D *nurbs);
__EXPORT void zNURBS3DCPFWrite(FILE *fp, zNURBS3D *nurbs);

__END_DECLS

#endif /* __ZNURBS3D_H__ */
