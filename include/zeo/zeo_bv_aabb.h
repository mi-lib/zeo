/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv_aabb - bounding volume: axis-aligned bounding box.
 */

#ifndef __ZEO_BV_AABB_H__
#define __ZEO_BV_AABB_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief axis-aligned box class.
 *//* ******************************************************* */
typedef struct{
  zVec3D min; /*!< minimum coordinates */
  zVec3D max; /*!< maximum coordinates */
} zAABox3D;

/*! \brief initialize an axis-aligned box */
__EXPORT zAABox3D *zAABox3DInit(zAABox3D *box);

/*! \brief create an axis-aligned box */
__EXPORT zAABox3D *zAABox3DCreate(zAABox3D *box, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);

/*! \brief copy an axis-aligned box. */
__EXPORT zAABox3D *zAABox3DCopy(zAABox3D *src, zAABox3D *dst);

/*! \brief merge two axis-aligned boxes. */
__EXPORT zAABox3D *zAABox3DMerge(zAABox3D *box, zAABox3D *b1, zAABox3D *b2);

/*! \brief check if a point is inside of an axis-aligned box. */
__EXPORT bool zAABox3DPointIsInside(zAABox3D *box, zVec3D *p, bool rim);

/*! \brief compute volume of an axis-aligned box. */
__EXPORT double zAABox3DVolume(zAABox3D *box);

/*! \brief print an axis-aligned box out to a file. */
__EXPORT void zAABox3DDataFPrint(FILE *fp, zAABox3D *box);

/*! \brief convert an axis-aligned box to a general box. */
__EXPORT zBox3D *zAABox3DToBox3D(zAABox3D *aab, zBox3D *box);

/*! \brief compute an axis-aligned box of a 3D box. */
__EXPORT zAABox3D *zBox3DToAABox3D(zBox3D *box, zAABox3D *aabox);

/* ********************************************************** */
/* AABB - axis-aligned bounding box
 * ********************************************************** */

/*! \brief axis-aligned bounding box of points.
 *
 * zAABB() computes the axis-aligned bounding box of a set of
 * points \a p. \a num is the number of the points. The result
 * is put into \a bb.
 *
 * The pointers to the extreme points (i.e. points on the faces
 * of \a bb) will be stored into the array pointed by \a vp,
 * unless \a vp is the null pointer. The correspondency of
 * pointers and faces is depicted as follows.
 *       /~~~~~/|0
 *      / 2 z|/ |
 *     |-----| 4|/x
 *    1|     |  /
 *     |  3  | /
 *  y _|_____|/
 *        5
 *
 * zAABBPL() also computes the axis-aligned bounding box of a
 * set of points given by a list of pointers to the points \a pl.
 * This function stores the pointers to the cell of extreme
 * points into the array pointed by \a vc, unless \a vc is the
 * null pointer. The correspondency follows the above, too.
 *
 * zAABBXform() computes the axis-aligned bouding box of a set
 * of points. \a p is an array of the points in a frame \a f.
 * Namely, the actual positions of the points are obtained by
 * transforming \a p by \a f. \a num is the number of the points.
 * The result is put into \a bb.
 *
 * zAABBXformPL() also computes the axis-aligned bounding box
 * of a set of points given by a list of vectors \a pl in a
 * frame \a f. The result is put into \a bb. This function does
 * not store addresses of the extreme points.
 * \return
 * zAABB(), zAABBPL(), zAABBXform() and zAABBXFormPL()  return
 * a pointer \a bb.
 * \notes
 * Arrays pointed by \a vp and \a vc must have more than six
 * elements, when they point non-null addresses.
 * Since some of extreme points are possibly at edges or corners
 * of \a bb, some of the elements in \a vp and \a vc could point
 * the same points.
 */
__EXPORT zAABox3D *zAABB(zAABox3D *bb, zVec3D p[], int num, zVec3D **vp);
__EXPORT zAABox3D *zAABBPL(zAABox3D *bb, zVec3DList *pl, zVec3DListCell **vc);
__EXPORT zAABox3D *zAABBXform(zAABox3D *bb, zVec3D p[], int num, zFrame3D *f);
__EXPORT zAABox3D *zAABBXformPL(zAABox3D *bb, zVec3DList *pl, zFrame3D *f);

__END_DECLS

#endif /* __ZEO_BV_AABB_H__ */
