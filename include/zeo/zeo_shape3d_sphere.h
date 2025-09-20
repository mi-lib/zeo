/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_sphere - 3D shapes: sphere.
 */

#ifdef ZEO_SHAPE_DECL_METHOD
zShape3D *createSphere(const zVec3D *center, double radius, int div = 0);
#else

#ifndef __ZEO_SHAPE3D_SPHERE_H__
#define __ZEO_SHAPE3D_SPHERE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief convert a 3D sphere to a polyhedron.
 *
 * zSphere3DToPH() converts a sphere \a sphere to a polyhedron \a ph
 * as a polygon model. \a ph should be initialized in advance.
 * It approximately divides the face into rectangles by the stored
 * number of division.
 * \return
 * zSphere3DToPH() returns a pointer \a ph.
 * \sa
 * zCyl3DToPH, zCyl3DToPHDRC,
 * zCone3DToPH, zCone3DToPHDRC
 */
/* default longitudinal & latitudinal division number are the same. */
__ZEO_EXPORT zPH3D *zSphere3DToPH(const zSphere3D *sphere, zPH3D *ph);

/*! \brief fit a sphere to point cloud.
 *
 * zSphere3DFit() fits a sphere to given point cloud \a data. The result is stored in \a sphere.
 * \return
 * The pointer \a sphere is returned.
 */
__ZEO_EXPORT zSphere3D *zSphere3DFit(zSphere3D *sphere, zVec3DData *data);

/*! \brief print a 3D sphere out to a file in a ZTK format. */
__ZEO_EXPORT void zSphere3DFPrintZTK(FILE *fp, const zSphere3D *sphere);

/* methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_sphere_com;

#define zShape3DSphere(sphere) ( (zSphere3D*)(sphere)->body )

__ZEO_EXPORT zShape3D *zShape3DSphereCreate(zShape3D *shape, const zVec3D *center, double radius, int div);

__END_DECLS

#ifdef __cplusplus
inline zShape3D *zShape3D::createSphere(const zVec3D *center, double radius, int div){ return zShape3DSphereCreate( this, center, radius, div ); }
#endif /* __cplusplus */

#endif /* __ZEO_SHAPE3D_SPHERE_H__ */

#endif /* ZEO_SHAPE_DECL_METHOD */
