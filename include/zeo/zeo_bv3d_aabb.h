/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_aabb - 3D bounding volume: axis-aligned bounding box.
 */

#ifndef __ZEO_BV3D_AABB_H__
#define __ZEO_BV3D_AABB_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 3D axis-aligned box class.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zAABox3D ){
  zVec3D min; /*!< minimum coordinates */
  zVec3D max; /*!< maximum coordinates */
};

/*! \brief initializea a 3D axis-aligned box */
__ZEO_EXPORT zAABox3D *zAABox3DInit(zAABox3D *box);

/*! \brief createa a 3D axis-aligned box */
__ZEO_EXPORT zAABox3D *zAABox3DCreate(zAABox3D *box, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);

/*! \brief copya a 3D axis-aligned box. */
__ZEO_EXPORT zAABox3D *zAABox3DCopy(zAABox3D *src, zAABox3D *dst);

/*! \brief merge two 3D axis-aligned boxes. */
__ZEO_EXPORT zAABox3D *zAABox3DMerge(zAABox3D *box, zAABox3D *b1, zAABox3D *b2);

/*! \brief check if a point is inside of a 3D axis-aligned box. */
__ZEO_EXPORT bool zAABox3DPointIsInside(zAABox3D *box, zVec3D *p, double margin);

/*! \brief compute volume of a 3D axis-aligned box. */
__ZEO_EXPORT double zAABox3DVolume(zAABox3D *box);

/*! \brief print out a 3D axis-aligned box to a file. */
__ZEO_EXPORT void zAABox3DValueFPrint(FILE *fp, zAABox3D *box);

/*! \brief convert a 3D axis-aligned box to a general box. */
__ZEO_EXPORT zBox3D *zAABox3DToBox3D(zAABox3D *aab, zBox3D *box);

/*! \brief compute a 3D axis-aligned box of a 3D box. */
__ZEO_EXPORT zAABox3D *zBox3DToAABox3D(zBox3D *box, zAABox3D *aabox);

/*! \brief axis-aligned bounding box of 3D points.
 *
 * zVec3DDataAABB() computes the axis-aligned bounding box of a set of 3D points \a data.
 * The result is put into \a bb.
 *
 * The pointers to the extreme points (i.e. points on the faces of \a bb) will be stored into
 * the array pointed by \a vp, unless \a vp is the null pointer. The correspondency of the
 * pointers and faces is depicted as follows.
 *       /~~~~~/|0
 *      / 2 z|/ |
 *     |-----| 4|/x
 *    1|     |  /
 *     |  3  | /
 *  y _|_____|/
 *        5
 *
 * zVec3DDataAABBXform() computes the axis-aligned bouding box of a set of 3D points \a data
 * in a frame \a f. Namely, the actual positions of the points are obtained by transforming
 * individual points in \a data by \a f. The result is put into \a bb.
 * This function does not store addresses of the extreme points.
 * \return
 * zVec3DDataAABB() and zVec3DDataAABBXform() return a pointer \a bb.
 * \notes
 * Arrays pointed by \a vp must have more than six elements, unless it is the null pointer.
 * Since some of the extreme points are possibly at edges or corners of \a bb, some of the
 * elements of \a vp could point same points.
 */
__ZEO_EXPORT zAABox3D *zVec3DDataAABB(zVec3DData *data, zAABox3D *bb, zVec3D **vp);
__ZEO_EXPORT zAABox3D *zVec3DDataAABBXform(zVec3DData *data, zAABox3D *bb, zFrame3D *frame);

__END_DECLS

#endif /* __ZEO_BV3D_AABB_H__ */
