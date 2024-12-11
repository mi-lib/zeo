/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_pca - principal component analysis.
 */

#ifndef __ZEO_VEC3D_PCA_H__
#define __ZEO_VEC3D_PCA_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief barycenter of and PCA to vector cloud.
 *
 * zVec3DDataBarycenter() computes the barycenter of a set of 3D vectors \a data.
 * The result is put where \a c points.
 *
 * zVec3DDataPCA() conducts principal component analysis (PCA) on a set of 3D vectors \a data.
 * Each of the three principal components passes through the original point.
 * The result PCs are stored into the array \a evec.
 *
 * zVec3DDataBaryPCA() a combination of the above two functions. Each of the three PCs \a evec
 * passes through the barycenter \a c of the vectors.
 * \return
 * zVec3DDataBarycenter() and zVec3DDataBaryPCA() return the null pointer if the given \a data
 * is empty. Otherwise, they return the pointer \a c.
 *
 * zVec3DDataPCA() returns a pointer to the head of \a evec.
 */
__ZEO_EXPORT zVec3D *zVec3DDataBarycenter(zVec3DData *data, zVec3D *c);
__ZEO_EXPORT zVec3D *zVec3DDataPCA(zVec3DData *data, zVec3D evec[]);
__ZEO_EXPORT zVec3D *zVec3DDataBaryPCA(zVec3DData *data, zVec3D *c, zVec3D evec[]);

__END_DECLS

#endif /* __ZEO_VEC3D_PCA_H__ */
