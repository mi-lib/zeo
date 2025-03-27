/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_pca - principal component analysis on 3D points.
 */

#ifndef __ZEO_VEC3D_PCA_H__
#define __ZEO_VEC3D_PCA_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief barycenter of 3D points.
 *
 * zVec3DDataBarycenter() computes the barycenter of a set of 3D points \a data.
 * The result is put where \a center points.
 * \return
 * zVec3DDataBarycenter() returns the null pointer if the given \a data is empty. Otherwise,
 * it returns the pointer \a center.
 */
__ZEO_EXPORT zVec3D *zVec3DDataBarycenter(zVec3DData *data, zVec3D *center);

/*! \brief barycenter and covariance matrix of a set of 3D points.
 *
 * zVec3DDataCov() computes the variance-covariance matrix of a set of 3D points \a data about
 * the given center point \a center. The matrix is stored in \a cov.
 *
 * zVec3DDataBaryCov() computes the barycenter and the variance-covariance matrix of a set of
 * 3D points \a data. The barycenter and the matrix are stored in \a center and \a cov, respectively.
 * \return
 * zVec3DDataCov() and zVec3DDataBaryCov() return a boolean value. If \a data is empty, they return
 * the false value. Otherwise, they return the true value.
 */
__ZEO_EXPORT bool zVec3DDataCov(zVec3DData *data, const zVec3D *center, zMat3D *cov);
__ZEO_EXPORT bool zVec3DDataBaryCov(zVec3DData *data, zVec3D *center, zMat3D *cov);

/*! \brief add a new 3D point to a set of 3D points, and update its barycenter and covariance matrix.
 *
 * zVec3DDataAddAndUpdateBaryCov() adds a new 3D point \a v to a set of 3D points \a data.
 * At the same time, it updates the barycenter of \a data \a center and a variance-covariance matrix
 * \a cov of \a data.
 * \return
 * zVec3DDataAddAndUpdateBaryCov() returns the false value if it fails to allocate memory to add the
 * new 3D point to \a data. Otherwise, it returns the true value.
 * \notes
 * It assumes that the barycenter and the variance-covariance matrix of \a data are stored in \a center
 * and \a cov before update. It this is not true, the result does not make sense.
 */
__ZEO_EXPORT bool zVec3DDataAddAndUpdateBaryCov(zVec3DData *data, const zVec3D *v, zVec3D *center, zMat3D *cov);

/*! \brief principal component analysis to 3D points.
 *
 * zVec3DDataPCA() conducts principal component analysis (PCA) on a set of 3D points \a data.
 * The three principal axes pass through the given point \a center.
 * The principal components are stored into the array \a eval, while the corresponding axis vectors
 * are stored in the array \a evec.
 *
 * zVec3DDataBaryPCA() finds the barycenter and the principal components about the barycenter of a
 * set of 3D points \a data. Namely, the three principle axes pass through the barycenter, which
 * is stored in \a center.
 * The principal components are stored into the array \a eval, while the corresponding axis vectors
 * are stored in the array \a evec.
 * \return
 * zVec3DDataPCA() and zVec3DDataBaryPCA() return the false value if the given set of points is
 * empty. Otherwise, they return the true value.
 */
__ZEO_EXPORT bool zVec3DDataPCA(zVec3DData *data, const zVec3D *center, double eval[3], zVec3D evec[3]);
__ZEO_EXPORT bool zVec3DDataBaryPCA(zVec3DData *data, zVec3D *center, double eval[3], zVec3D evec[3]);

/*! \brief find a coordinate frame that spans principal components of a set of 3D points from its barycenter.
 *
 * zVec3DDataPCAFrame() finds a coordinate frame that spans principal components of a set of 3D points
 * \a data from its barycenter. The result is stored in \a frame.
 * \return
 * zVec3DDataPCAFrame() returns the pointer \a frame.
 * \sa
 * zVec3DDataBaryPCA
 */
__ZEO_EXPORT zFrame3D *zVec3DDataPCAFrame(zVec3DData *data, zFrame3D *frame);

/*! \brief mean plane of a set of points.
 *
 * zVec3DDataMeanNormal() computes the mean normal vector of a set of 3D points \a data.
 * The mean normal vector means the unit principal axis vector corresponding to the minimum principal
 * component, where the principal component analysis is conducted about the given point \a center.
 * The result is stored where \a normal points.
 *
 * zVec3DDataMeanPlane() computes the mean plane of a set of 3D points \a data.
 * The plane passes through the barycenter of the points, while its normal vector points the principal
 * axis corresponding to the minimum principal component.
 * The barycenter is stored where \a center points, while the result plane is stored in \a plane.
 * \return
 * zVec3DDataMeanNormal() returns the pointer \a normal. If \a data is empty, it returns the null pointer.
 *
 * zVec3DDataMeanPlane() returns the pointer \a plane. If \a data is empty, it returns the null pointer.
 * \sa
 * zVec3DDataBaryPCA
 */
__ZEO_EXPORT zVec3D *zVec3DDataMeanNormal(zVec3DData *data, const zVec3D *center, zVec3D *normal);
__ZEO_EXPORT zPlane3D *zVec3DDataMeanPlane(zVec3DData *data, zVec3D *center, zPlane3D *plane);

__END_DECLS

#endif /* __ZEO_VEC3D_PCA_H__ */
