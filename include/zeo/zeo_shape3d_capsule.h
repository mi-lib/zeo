/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_capsule - 3D shapes: capsule.
 */

#include <zeo/zeo_shape3d.h>

#ifndef __ZEO_SHAPE3D_CAPSULE_H__
#define __ZEO_SHAPE3D_CAPSULE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zCapsule3D
 * 3D capsule class
 * ********************************************************** */

typedef zCyl3D zCapsule3D;

#define zCapsule3DCenter(c,i)      zCyl3DCenter( c, i )
#define zCapsule3DRadius(c)        zCyl3DRadius( c )
#define zCapsule3DDiv(c)           zCyl3DDiv( c )

#define zCapsule3DSetCenter(c,i,p) zCyl3DSetCenter( c, i, p )
#define zCapsule3DSetRadius(c,r)   zCyl3DSetRadius( c, r )
#define zCapsule3DSetDiv(c,d)      zCyl3DSetDiv( c, d )

/*! \brief initialize, create and copy a 3D capsule.
 *
 * zCapsule3DInit() initializes a 3D capsule \a capsule, setting both
 * centers on the bases for the original point and radius for zero.
 *
 * zCapsule3DCreate() creates a 3D capsule whose two center points on the
 * bases are \a c1 and \a c2, and radius is \a r.
 * \a div is the number of division for polyhedral approximation.
 * When zero is given for \a div, ZEO_SHAPE_DEFAULT_DIV is set instead.
 *
 * zCapsule3DCopy() copies a 3D capsule \a src to the other \a dest.
 * \return
 * zCapsule3DInit() and zCapsule3DCreate() return a pointer \a capsule.
 * zCapsule3DCopy() returns a pointer \a dest.
 */
#define zCapsule3DCreate(c,c1,c2,r,div) zCyl3DCreate( c, c1, c2, r, div )
#define zCapsule3DInit(c)               zCyl3DInit( c )
#define zCapsule3DAlloc()               zCyl3DAlloc()
#define zCapsule3DCopy(s,d)             zCyl3DCopy( s, d )
#define zCapsule3DMirror(s,d,a)         zCyl3DMirror( s, d, a )

/*! \brief transform a 3D capsule.
 *
 * zCapsule3DXform() transforms a 3D capsule \a src by a frame \a f and
 * puts it into \a dest.
 *
 * zCapsule3DXformInv() transforms \a src by the inverse of a frame \a f
 * and puts it into \a dest.
 * \return
 * zCapsule3DXform() and zCapsule3DXformInv() return a pointer \a dest.
 */
#define zCapsule3DXform(s,f,d)    zCyl3DXform( s, f, d )
#define zCapsule3DXformInv(s,f,d) zCyl3DXformInv( s, f, d )

/*! \brief check if a point is inside of a 3D capsule.
 *
 * zCapsule3DPointIsInside() checks if a 3D point \a p is inside of a 3D
 * capsule \a capsule. \a margin is a margin of the inside area outward
 * from the boundary of \a capsule.
 * \return
 * zCapsule3DPointIsInside() returns the true value if \a p is inside of \a capsule,
 * or the false value otherwise.
 */
__ZEO_EXPORT double zCapsule3DClosest(zCapsule3D *capsule, zVec3D *p, zVec3D *cp);
__ZEO_EXPORT double zCapsule3DPointDist(zCapsule3D *capsule, zVec3D *p);
__ZEO_EXPORT bool zCapsule3DPointIsInside(zCapsule3D *capsule, zVec3D *p, double margin);

/*! \brief axis vector and height of a 3D capsule.
 *
 * zCapsule3DAxis() calculates the axis vector of a 3D capsule \a capsule;
 * the axis from the center point on the bottom base to the center point
 * on the top base.
 *
 * zCapsule3DHeight() calculates the height from the bottom base to the top
 * base of a 3D capsule \a capsule.
 * \return
 * zCapsule3DAxis() returns a pointer \a axis.
 * zCapsule3DHeight() returns the calculated height.
 */
#define zCapsule3DAxis(c,a) zCyl3DAxis( c, a )
__ZEO_EXPORT double zCapsule3DHeight(zCapsule3D *capsule);

/*! \brief volume of a 3D capsule.
 *
 * zCapsule3DVolume() calculates the volume of a 3D capsule \a capsule.
 * \return
 * zCapsule3DVolume() returns the calculated volume.
 */
__ZEO_EXPORT double zCapsule3DVolume(zCapsule3D *capsule);

/*! \brief barycenter of a capsule.
 *
 * zCapsule3DBarycenter() calculates the barycenter of a 3D capsule \a capsule.
 * The result is put into \a c.
 * \return
 * zCapsule3DBarycenter() returns a pointer \a c.
 */
#define zCapsule3DBarycenter(c,p) zCyl3DBarycenter( c, p )

/*! \brief inertia tensor of a capsule.
 *
 * zCapsule3DBaryInertia() calculates the inertia tensor of a capsuleinder \a capsule about
 * its barycenter, supposing it is a solid. Its density has to be specified
 * by \a density.
 * zCapsule3DBaryInertiaMass() calculates the inertia tensor of \a capsule about its
 * barycenter, where its mass instead of density has to be specified by \a mass.
 * For the both functions, the result is put into \a inertia.
 * \return
 * zCapsule3DBaryInertia() and zCapsule3DBaryInertiaMass() return a pointer \a inertia.
 * \sa
 * zCapsule3DVolume(), zCapsule3DBarycenter()
 */
__ZEO_EXPORT zMat3D *zCapsule3DBaryInertiaMass(zCapsule3D *capsule, double mass, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zCapsule3DBaryInertia(zCapsule3D *capsule, double density, zMat3D *inertia);

/*! \brief convert a 3D capsule to a polyhedron.
 *
 * zCapsule3DToPH() converts a 3D capsule \a capsule to a polyhedron \a ph as
 * a polygon model. It approximately divides the side face into rectangles
 * by the stored number of division.
 * \a ph should be initialized in advance.
 * \return
 * zCapsule3DToPH() returns a pointer \a ph.
 * \sa
 * zSphere3DToPH, zSphere3DToPH,
 * zCone3DToPH, zCone3DToPHDRC
 */
__ZEO_EXPORT zPH3D *zCapsule3DToPH(zCapsule3D *capsule, zPH3D *ph);

/*! \brief print a 3D capsule out to a file in a ZTK format. */
__ZEO_EXPORT void zCapsule3DFPrintZTK(FILE *fp, zCapsule3D *capsule);

/* methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_capsule_com;

#define zShape3DCapsule(s) ( (zCapsule3D*)(s)->body )

__ZEO_EXPORT zShape3D *zShape3DCapsuleCreate(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r, int div);

__END_DECLS

#endif /* __ZEO_SHAPE3D_CAPSULE_H__ */
