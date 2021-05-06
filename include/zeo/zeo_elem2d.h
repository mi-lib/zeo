/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d - 2D element.
 */

#ifndef __ZEO_ELEM2D_H__
#define __ZEO_ELEM2D_H__

#include <zeo/zeo_vec2d.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 2D triangle class.
 *//********************************************************* */

typedef struct{
  zVec2D *v[3]; /*!< vertices */
} zTri2D;

#define zTri2DVert(t,i)      (t)->v[i]
#define zTri2DSetVert(t,i,v) ( zTri2DVert(t,i) = (v) )

/*! \brief initialize and create 2D triangle.
 *
 * zTri2DInit() initializes a triangle instance \a t, setting three
 * vertices for the null vector.
 *
 * zTri2DCreate() creates a triangle from three vertices \a v1, \a v2
 * and \a v3.
 * \return
 * zTri2DInit() and zTri2DCreate() return a pointer \a t.
 */
__EXPORT zTri2D *zTri2DCreate(zTri2D *t, zVec2D *v1, zVec2D *v2, zVec2D *v3);
#define zTri2DInit(t) zTri2DCreate( t, NULL, NULL, NULL )

/*! \brief create a triangle. */
__EXPORT zTri2D *zTri2DCreate(zTri2D *t, zVec2D *v1, zVec2D *v2, zVec2D *v3);

/*! \brief barycenter of a triangle. */
__EXPORT zVec2D *zTri2DBarycenter(zTri2D *t, zVec2D *c);

/*! \brief circumcenter of a 2D triangle */
__EXPORT zVec2D *zTri2DCircumcenter(zTri2D *t, zVec2D *c);

/*! \brief incenter of a triangle */
__EXPORT zVec2D *zTri2DIncenter(zTri2D *t, zVec2D *c);

/*! \brief orthocenter of a triangle */
__EXPORT zVec2D *zTri2DOrthocenter(zTri2D *t, zVec2D *c);

/*! \brief print a 2D triangle.
 *
 * zTri2DFPrint() prints coordinates of the vertices of a 2D triangle
 * \a t to the current position of a file \a fp in the following format.
 *
 *  vert: 0 ( x0, y0 )
 *  vert: 1 ( x1, y1 )
 *  vert: 2 ( x2, y2 )
 *
 * zTri2DPrint() prints the same information of \a t out to the
 * standard output.
 * \return
 * zTri2DFPrint() and zTri2DPrint() do not return any values.
 */
__EXPORT void zTri2DFPrint(FILE *fp, zTri2D *t);
#define zTri2DPrint(t) zTri2DFPrint( stdout, (t) )

/*! \struct zTri2DArray
 * \brief array class of 2D triangles.
 */
zArrayClass( zTri2DArray, zTri2D );

/* NOTE: is the following to be moved to zeo_shape2d? */

/* ********************************************************** */
/*! \brief 2D disk class.
 *//********************************************************* */

typedef struct{
  zVec2D center;
  double radius;
} zDisk2D;

#define zDisk2DCenter(d)      ( &(d)->center )
#define zDisk2DRadius(d)      (d)->radius

#define zDisk2DSetCenter(d,c) zVec2DCopy( c, zDisk2DCenter(d) )
#define zDisk2DSetRadius(d,r) ( zDisk2DRadius(d) = (r) )

/*! \brief create a 2D disk. */
__EXPORT zDisk2D *zDisk2DCreate(zDisk2D *d, zVec2D *c, double r);

/*! \brief signed distance from a 2D point to a disk. */
__EXPORT double zDisk2DPointDist(zDisk2D *d, zVec2D *p);

/*! \brief check if a 2D point is inside of a disk. */
__EXPORT bool zDisk2DPointIsInside(zDisk2D *d, zVec2D *p, bool rim);

/*! \brief create a 2D disk from two points at both ends of diameter. */
__EXPORT zDisk2D *zDisk2DFrom2(zDisk2D *d, zVec2D *v1, zVec2D *v2);

/*! \brief create a 2D disk from three points as a circumcircle of them. */
__EXPORT zDisk2D *zDisk2DFrom3(zDisk2D *d, zVec2D *v1, zVec2D *v2, zVec2D *v3);

__END_DECLS

#endif /* __ZEO_ELEM2D_H__ */
