/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_profile - profiling of 3D pointcloud.
 */

#ifndef __ZEO_VEC3D_PROFILE_H__
#define __ZEO_VEC3D_PROFILE_H__

#include <zeo/zeo_bv3d.h>

__BEGIN_DECLS

#define ZEO_VEC3DDATA_NORMALVEC_NUM_MIN  5
#define ZEO_VEC3DDATA_NORMALVEC_NUM_MAX 30

/*! \brief normal vector cloud of a 3D point cloud.
 *
 * zVec3DDataNormalVec() computes normal vectors of a 3D point cloud \a pointdata by approximating
 * vicinity of each point within a radius \a radius by a 3D plane.
 * The identified normal vectors are stored in \a normaldata in the corresponding order with \a pointdata.
 *
 * zVec3DDataNormalVec_Tree() and zVec3DDataNormalVec_Octree() find the vicinities by using kd-tree and
 * octree, respectively.
 * zVec3DDataNormalVec() uses the latter by default.
 * \return
 * zVec3DDataNormalVec() returns the pointer \a normaldata.
 */
__ZEO_EXPORT zVec3DData *zVec3DDataNormalVec_Tree(zVec3DData *pointdata, double radius, zVec3DData *normaldata);
__ZEO_EXPORT zVec3DData *zVec3DDataNormalVec_Octree(zVec3DData *pointdata, double radius, zVec3DData *normaldata);
__ZEO_EXPORT zVec3DData *(* zVec3DDataNormalVec)(zVec3DData*, double, zVec3DData*);

#define ZEO_VEC3DDATA_IDENT_FRAME_MINSIZ 3

/*! \brief identify a transformation frame from two sets of 3D points.
 *
 * zVec3DDataIdentFrame() identifies a 3D coordinate frame that transforms a point cloud \a src to
 * another \a dest based on Umeyama's method:
 * Shinji Umeyama, Least-Squares Estimation of Transformation Parameters Between Two Point Patterns,
 * IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 13, No. 4, pp. 376-380, 1991.
 * \return
 * zVec3DDataIdentFrame() returns the pointer \a frame if it succeeds. If the given points are too few
 * or the sizes of \a src and \a dest mismatches, it returns the null pointer.
 */
__ZEO_EXPORT zFrame3D *zVec3DDataIdentFrame(zVec3DData *src, zVec3DData *dest, zFrame3D *frame);

#define ZEO_VEC3DDATA_ICP_MAXITERNUM 200

/*! \brief iterative closest point method.
 *
 * zVec3DDataICP() finds a transformation frame that gets a set of 3D points \a src the closest to another
 * \a dest based on the iterative closest point method:
 * Chen Yang and Gerard Medioni, "Object modelling by registration of multiple range images",
 * Image Vision Computation, Vol. 10, No. 3, pp. 145-155, 1991.
 * The result is stored where \a frame points.
 * \a sample_rate is the rate of samples to be evaluated at each iteration.
 * \a tol is the tolerance of the maximum distance between corresponding points of \a src and \a dest
 * to stop iterations.
 * \return
 * zVec3DDataICP() returns the pointer \a frame.
 * \sa
 * zVec3DDataIdentFrame
 */
__ZEO_EXPORT zFrame3D *zVec3DDataICP(zVec3DData *src, zVec3DData *dest, zFrame3D *frame, double sample_rate, double tol);

__END_DECLS

#endif /* __ZEO_VEC3D_PROFILE_H__ */
