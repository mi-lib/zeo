/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_aabb - 2D bounding volume: axis-aligned bounding box.
 */

#ifndef __ZEO_BV2D_AABB_H__
#define __ZEO_BV2D_AABB_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 2D axis-aligned box class.
 *//* ******************************************************* */
typedef struct{
  zVec2D min; /*!< minimum coordinates */
  zVec2D max; /*!< maximum coordinates */
} zAABox2D;

/*! \brief initializea 2D axis-aligned box */
__EXPORT zAABox2D *zAABox2DInit(zAABox2D *box);

/*! \brief createa 2D axis-aligned box */
__EXPORT zAABox2D *zAABox2DCreate(zAABox2D *box, double xmin, double ymin, double xmax, double ymax);

/*! \brief check if a point is inside ofa 2D axis-aligned box. */
__EXPORT bool zAABox2DPointIsInside(zAABox2D *box, zVec2D *p, double margin);

/*! \brief printa 2D axis-aligned box out to a file. */
__EXPORT void zAABox2DDataFPrint(FILE *fp, zAABox2D *box);

/* ********************************************************** */
/* AABB2D - 2D axis-aligned bounding box
 * ********************************************************** */

/*! \brief axis-aligned bounding box of 2D points.
 *
 * zAABB2D() computes the axis-aligned bounding box of a set of
 * 2D points \a p. \a num is the number of the points. The result
 * is put into \a bb.
 *
 * The pointers to the extreme points (i.e. points on the faces
 * of \a bb) will be stored into the array pointed by \a vp,
 * unless \a vp is the null pointer. The correspondency of
 * pointers and faces is depicted as follows.
 *  y|_____
 *   |  1  |
 *  2|     |0
 *  _|_____|_ x
 *      3
 *
 * zAABB2DPL() also computes the axis-aligned bounding box of a
 * set of 2D points given by a list of pointers to the points
 * \a pl. This function stores the pointers to the cell of extreme
 * points into the array pointed by \a vc, unless \a vc is the
 * null pointer. The correspondency follows the above, too.
 * \return
 * zAABB2D() and zAABB2DPL() return a pointer \a bb.
 * \notes
 * Arrays pointed by \a vp and \a vc must have more than six
 * elements, when they point non-null addresses.
 * Since some of the extreme points are possibly at edges or
 * corners of \a bb, some of the elements in \a vp and \a vc
 * could point the same points.
 */
__EXPORT zAABox2D *zAABB2D(zAABox2D *bb, zVec2D p[], int num, zVec2D **vp);
__EXPORT zAABox2D *zAABB2DPL(zAABox2D *bb, zVec2DList *pl, zVec2DListCell **vc);

__END_DECLS

#endif /* __ZEO_BV2D_AABB_H__ */
