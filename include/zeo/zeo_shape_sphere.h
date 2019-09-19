/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_sphere - 3D shapes: sphere.
 */

#ifndef __ZEO_SHAPE_SPHERE_H__
#define __ZEO_SHAPE_SPHERE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zSphere3D
 * 3D sphere class
 * ********************************************************** */

typedef struct{
  zVec3D center;
  double radius;
  int div;
} zSphere3D;

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
__EXPORT zSphere3D *zSphere3DCreate(zSphere3D *sphere, zVec3D *c, double r, int div);
__EXPORT zSphere3D *zSphere3DInit(zSphere3D *sphere);
__EXPORT zSphere3D *zSphere3DAlloc(void);
__EXPORT zSphere3D *zSphere3DCopy(zSphere3D *src, zSphere3D *dest);
__EXPORT zSphere3D *zSphere3DMirror(zSphere3D *src, zSphere3D *dest, zAxis axis);

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
__EXPORT zSphere3D *zSphere3DXform(zSphere3D *src, zFrame3D *f, zSphere3D *dest);
__EXPORT zSphere3D *zSphere3DXformInv(zSphere3D *src, zFrame3D *f, zSphere3D *dest);

/*! \brief distance from a 3D point to a 3D sphere.
 *
 * zSphere3DClosest() calculates the closest point from a 3D point
 * \a p to a 3D sphere \a sphere and puts it into \a cp. When \a p
 * is inside of \a sphere, it copies \a p to \a cp.
 *
 * zSphere3DPointDist() calculates the distance from a 3D point \a p
 * to a 3D sphere \a sphere.
 *
 * zSphere3DPointIsInside() checks if a 3D point \a p is inside of
 * a 3D sphere \a sphere. The point on the surface of \a sphere is
 * judged to be inside of \a sphere if the true value is given for
 * \a rim.
 * \return
 * zSphere3DClosest() and zSphere3DPointDist() return the signed
 * distance from \a p to \a sphere. The result is
 *  - a positive value when \a p is outside of \a sphere, or
 *  - a negative value when \a p is inside of \a sphere.
 *
 * zSphere3DPointIsInside() returns the true value if \a p is
 * inside of \a sphere, or the false value otherwise.
 */
__EXPORT double zSphere3DClosest(zSphere3D *sphere, zVec3D *p, zVec3D *cp);
__EXPORT double zSphere3DPointDist(zSphere3D *sphere, zVec3D *p);
__EXPORT bool zSphere3DPointIsInside(zSphere3D *sphere, zVec3D *p, bool rim);

/*! \brief volume and inertia of a 3D sphere.
 *
 * zSphere3DVolume() calculates the volume of a 3D sphere \a sphere.
 *
 * zSphere3DInertia() calculates the inertia tensor of a 3D sphere
 * \a sphere around its center. It treats \a sphere as a solid model.
 * The result is put into \a inertia.
 * \return
 * zSphere3DVolume() returns the volume calculated.
 * zSphere3DInertia() returns a pointer \a inertia.
 */
__EXPORT double zSphere3DVolume(zSphere3D *sphere);
__EXPORT zMat3D *zSphere3DInertia(zSphere3D *sphere, zMat3D *inertia);

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
__EXPORT zPH3D *zSphere3DToPH(zSphere3D *sphere, zPH3D *ph);

/*! \brief fit a sphere to point cloud.
 *
 * zSphere3DFit() fits a sphere to given point cloud \a pc. The result
 * is stored in \a s.
 * \return
 * The pointer \a s is returned.
 */
__EXPORT zSphere3D *zSphere3DFit(zSphere3D *s, zVec3DList *pc);

/* methods for abstraction */
extern zShape3DCom zeo_shape3d_sphere_com;

#define zShape3DSphere(s) ( (zSphere3D*)(s)->body )

__EXPORT zShape3D *zShape3DSphereCreate(zShape3D *shape, zVec3D *c, double r, int div);

__END_DECLS

#endif /* __ZEO_SHAPE_SPHERE_H__ */
