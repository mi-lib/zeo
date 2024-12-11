/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_sphere - 3D shapes: sphere.
 */

#ifndef __ZEO_SHAPE3D_SPHERE_H__
#define __ZEO_SHAPE3D_SPHERE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zSphere3D
 * 3D sphere class
 * ********************************************************** */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zSphere3D ){
  zVec3D center;
  double radius;
  int div;
};

#define zSphere3DCenter(s)      ( &(s)->center )
#define zSphere3DRadius(s)      (s)->radius
#define zSphere3DDiv(s)         (s)->div

#define zSphere3DSetCenter(s,c) zVec3DCopy( c, zSphere3DCenter(s) )
#define zSphere3DSetRadius(s,r) ( zSphere3DRadius(s) = (r) )
#define zSphere3DSetDiv(s,d)    ( zSphere3DDiv(s) = (d) )

/*! \brief initialize, create and copy a 3D sphere.
 *
 * zSphere3DInit() initializes a 3D sphere \a sphere by setting
 * its center for the original point and radius for zero.
 *
 * zSphere3DCreate() creates a 3D sphere whose center is at \a c
 * and radius is \a r.
 * \a div is the number of division for polyhedral approximation.
 * When zero is given for \a div, ZEO_SHAPE_DEFAULT_DIV is set
 * instead.
 *
 * zSphere3DCopy() copies a 3D sphere \a src to the other \a dest.
 * \return
 * Each of zSphere3DInit() and zSphere3DCreate() returns a
 * pointer \a sphere.
 * zSphere3DCopy() returns the pointer \a dest.
 */
__ZEO_EXPORT zSphere3D *zSphere3DCreate(zSphere3D *sphere, const zVec3D *c, double r, int div);
__ZEO_EXPORT zSphere3D *zSphere3DInit(zSphere3D *sphere);
__ZEO_EXPORT ZDEF_ALLOC_FUNCTION_PROTOTYPE( zSphere3D );
__ZEO_EXPORT zSphere3D *zSphere3DCopy(const zSphere3D *src, zSphere3D *dest);
__ZEO_EXPORT zSphere3D *zSphere3DMirror(const zSphere3D *src, zSphere3D *dest, zAxis axis);

/*! \brief transform coordinates of a 3D sphere.
 *
 * zSphere3DXform() transforms coordinates of a 3D sphere \a src by
 * a frame \a f and puts it into \a dest.
 *
 * zSphere3DXformInv() transforms \a src by the inverse of a frame
 * \a f and puts it into \a dest.
 * \return
 * zSphere3DXform() and zSphere3DXformInv() return the pointer \a dest.
 */
__ZEO_EXPORT zSphere3D *zSphere3DXform(const zSphere3D *src, const zFrame3D *f, zSphere3D *dest);
__ZEO_EXPORT zSphere3D *zSphere3DXformInv(const zSphere3D *src, const zFrame3D *f, zSphere3D *dest);

/*! \brief distance from a 3D point to a 3D sphere.
 *
 * zSphere3DClosest() calculates the closest point from a 3D point
 * \a p to a 3D sphere \a sphere and puts it into \a cp. When \a p
 * is inside of \a sphere, it copies \a p to \a cp.
 *
 * zSphere3DDistFromPoint() calculates the distance from a 3D point \a p
 * to a 3D sphere \a sphere.
 *
 * zSphere3DPointIsInside() checks if a 3D point \a p is inside of
 * a 3D sphere \a sphere. \a margin is a margin of the inside area
 * outward from the boundary of \a sphere.
 * \return
 * zSphere3DClosest() and zSphere3DDistFromPoint() return the signed
 * distance from \a p to \a sphere. The result is
 *  - a positive value when \a p is outside of \a sphere, or
 *  - a negative value when \a p is inside of \a sphere.
 *
 * zSphere3DPointIsInside() returns the true value if \a p is inside
 * of \a sphere, or the false value otherwise.
 */
__ZEO_EXPORT double zSphere3DClosest(const zSphere3D *sphere, const zVec3D *p, zVec3D *cp);
__ZEO_EXPORT double zSphere3DDistFromPoint(const zSphere3D *sphere, const zVec3D *p);
__ZEO_EXPORT bool zSphere3DPointIsInside(const zSphere3D *sphere, const zVec3D *p, double margin);

/*! \brief create a 3D sphere from two points at both ends of diameter. */
__ZEO_EXPORT zSphere3D *zSphere3DFrom2(zSphere3D *sphere, const zVec3D *v1, const zVec3D *v2);

/*! \brief create a 3D sphere from three points to include their circumcircle as the great circle. */
__ZEO_EXPORT zSphere3D *zSphere3DFrom3(zSphere3D *sphere, const zVec3D *v1, const zVec3D *v2, const zVec3D *v3);

/*! \brief create a 3D sphere from four points as the circumscribing sphere of them. */
__ZEO_EXPORT zSphere3D *zSphere3DFrom4(zSphere3D *sphere, const zVec3D *v1, const zVec3D *v2, const zVec3D *v3, const zVec3D *v4);

/*! \brief volume of a 3D sphere.
 *
 * zSphere3DVolume() calculates the volume of a 3D sphere \a sphere.
 * \return
 * zSphere3DVolume() returns the calculated volume.
 */
__ZEO_EXPORT double zSphere3DVolume(const zSphere3D *sphere);

/*! \brief inertia tensor of a 3D sphere.
 *
 * zSphere3DBaryInertia() calculates the inertia tensor of a sphere \a sphere about
 * its barycenter, supposing it is a solid. Its density has to be specified by
 * \a density.
 * zSphere3DBaryInertiaMass() calculates the inertia tensor of \a sphere about its
 * barycenter, where its mass instead of density has to be specified by \a mass.
 * For the both functions, the result is put into \a inertia.
 * \return
 * zSphere3DBaryInertia() and zSphere3DBaryInertiaMass() return a pointer \a inertia.
 * \sa
 * zSphere3DVolume()
 */
__ZEO_EXPORT zMat3D *zSphere3DBaryInertiaMass(const zSphere3D *sphere, double mass, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zSphere3DBaryInertia(const zSphere3D *sphere, double density, zMat3D *inertia);

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

/*! \brief print a 3D sphere out to a file in a ZTK format. */
__ZEO_EXPORT void zSphere3DFPrintZTK(FILE *fp, const zSphere3D *sphere);

/*! \brief fit a sphere to point cloud.
 *
 * zSphere3DFit() fits a sphere to given point cloud \a data. The result is stored in \a sphere.
 * \return
 * The pointer \a sphere is returned.
 */
__ZEO_EXPORT zSphere3D *zSphere3DFit(zSphere3D *sphere, zVec3DData *data);

/* methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_sphere_com;

#define zShape3DSphere(sphere) ( (zSphere3D*)(sphere)->body )

__ZEO_EXPORT zShape3D *zShape3DSphereCreate(zShape3D *shape, const zVec3D *center, double radius, int div);

__END_DECLS

#endif /* __ZEO_SHAPE3D_SPHERE_H__ */
