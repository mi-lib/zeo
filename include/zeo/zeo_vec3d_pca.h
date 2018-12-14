/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec_pca - principal component analysis.
 */

#ifndef __ZEO_VEC3D_PCA_H__
#define __ZEO_VEC3D_PCA_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* utilities
 * ********************************************************** */

/*! \brief barycenter of and PCA against vector cloud.
 *
 * zVec3DBarycenterPL() and zVec3DBarycenter() computes the
 * barycenter of a set of vectors. For zVec3DBarycenterPL(),
 * vectors are given by a list \a vl, while given by an array
 * \a v for zVec3DBarycenter().
 * \a num is the number of vectors in \a v. The result vector
 * is put where pointed by \a c.
 *
 * zVec3DPCA_PL() and zVec3DPCA() examines principal component
 * analysis (PCA) for a set of vectors. Each of the three principal
 * components passes through the original point. The vectors are
 * also given by \a vl for zVec3DPCA_PL(), and \a v and \a num
 * for zVec3DPCA(), respectively. The result PCs are stored into
 * the array \a evec.
 *
 * zVec3DBaryPCA_PL() and zVec3DBaryPCA() are combinations of
 * the above functions. Each of the three PCs \a evec passes
 * through the barycenter \a c of the vectors. The vectors are
 * given by \a vl for zVec3DBaryPCA_PL(), and \a v and \a num
 * for zVec3DBaryPCA(), respectively.
 * \return
 * zVec3DBarycenterPL(), zVec3DBarycenter(), zVec3DBaryPCA_PL()
 * and zVec3DBaryPCA() return a pointer \a c.
 *
 * zVec3DPCA_PL() and zVec3DPCA() return a pointer to the head of
 * \a evec.
 */
__EXPORT zVec3D *zVec3DBarycenterPL(zVec3DList *vl, zVec3D *c);
__EXPORT zVec3D *zVec3DBarycenter(zVec3D v[], int num, zVec3D *c);
__EXPORT zVec3D *zVec3DPCA_PL(zVec3DList *vl, zVec3D evec[]);
__EXPORT zVec3D *zVec3DPCA(zVec3D v[], int num, zVec3D evec[]);
__EXPORT zVec3D *zVec3DBaryPCA_PL(zVec3DList *vl, zVec3D *c, zVec3D evec[]);
__EXPORT zVec3D *zVec3DBaryPCA(zVec3D v[], int num, zVec3D *c, zVec3D evec[]);

__END_DECLS

#endif /* __ZEO_VEC3D_PCA_H__ */
