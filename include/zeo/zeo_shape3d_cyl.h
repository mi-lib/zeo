/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_cyl - 3D shapes: cylinder.
 */

#ifndef __ZEO_SHAPE3D_CYL_H__
#define __ZEO_SHAPE3D_CYL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zCyl3D
 * 3D cylinder class
 * ********************************************************** */

typedef struct{
  zVec3D center[2];
  double radius;
  uint div;
} zCyl3D;

#define zCyl3DCenter(c,i)     ( &(c)->center[i] )
#define zCyl3DRadius(c)       (c)->radius
#define zCyl3DDiv(c)          (c)->div

#define zCyl3DSetCenter(c,i,p) zVec3DCopy(p,zCyl3DCenter(c,i))
#define zCyl3DSetRadius(c,r)   ( zCyl3DRadius(c) = (r) )
#define zCyl3DSetDiv(c,d)      ( zCyl3DDiv(c) = (d) )

/*! \brief initialize, create and copy a 3D cylinder.
 *
 * zCyl3DInit() initializes a 3D cylinder \a cyl, setting both of its
 * centers on the bases for the original point and radius for zero.
 *
 * zCyl3DCreate() creates a 3D cylinder whose two centers point on the
 * bases are \a c1 and \a c2, and radius is \a r.
 * \a div is the number of division for polyhedral approximation.
 * When zero is given for \a div, ZEO_SHAPE_DEFAULT_DIV is
 * set instead.
 *
 * zCyl3DCopy() copies a 3D cylinder \a src to the other \a dest.
 * \return
 * zCyl3DInit() and zCyl3DCreate() return a pointer \a cyl.
 * zCyl3DCopy() returns a pointer \a dest.
 */
__EXPORT zCyl3D *zCyl3DCreate(zCyl3D *cyl, zVec3D *c1, zVec3D *c2, double r, int div);
__EXPORT zCyl3D *zCyl3DInit(zCyl3D *cyl);
__EXPORT ZDEF_ALLOC_FUNCTION_PROTOTYPE( zCyl3D );
__EXPORT zCyl3D *zCyl3DCopy(zCyl3D *src, zCyl3D *dest);
__EXPORT zCyl3D *zCyl3DMirror(zCyl3D *src, zCyl3D *dest, zAxis axis);

/*! \brief transfer a 3D cylinder.
 *
 * zCyl3DXform() transforms a 3D cylinder \a src by a frame \a f and
 * puts it into \a dest.
 *
 * zCyl3DXformInv() transforms \a src by the inverse of a frame \a f
 * and puts it into \a dest.
 * \return
 * zCyl3DXform() and zCyl3DXformInv() return a pointer \a dest.
 */
__EXPORT zCyl3D *zCyl3DXform(zCyl3D *src, zFrame3D *f, zCyl3D *dest);
__EXPORT zCyl3D *zCyl3DXformInv(zCyl3D *src, zFrame3D *f, zCyl3D *dest);

/*! \brief check if a point is inside of a 3D cylinder.
 *
 * zCyl3DPointIsInside() checks if a 3D point \a p is inside of a 3D
 * cylinder \a cyl. \a margin is a margin of the inside area outward
 * from the boundary of \a box.
 * \return
 * zCyl3DPointIsInside() returns the true value if \a p is inside of \a cyl,
 * or the false value otherwise.
 */
__EXPORT double zCyl3DClosest(zCyl3D *cyl, zVec3D *p, zVec3D *cp);
__EXPORT double zCyl3DPointDist(zCyl3D *cyl, zVec3D *p);
__EXPORT bool zCyl3DPointIsInside(zCyl3D *cyl, zVec3D *p, double margin);

/*! \brief axis vector and height of a 3D cylinder.
 *
 * zCyl3DAxis() calculates the axis vector of a 3D cylinder \a cyl;
 * the axis from the center point on the bottom base to the center point
 * on thetop base.
 *
 * zCyl3DHeight() calculates the height from the bottom base to the top
 * base of a 3D cylinder \a cyl.
 * \return
 * zCyl3DAxis() returns a pointer \a axis.
 * zCyl3DHeight() returns the calculated height.
 */
#define zCyl3DAxis(c,a) \
  zVec3DSub( zCyl3DCenter(c,1), zCyl3DCenter(c,0), a )
__EXPORT double zCyl3DHeight(zCyl3D *cyl);

/*! \brief volume of a 3D cylinder.
 *
 * zCyl3DVolume() calculates the volume of a 3D cylinder \a cyl.
 * \return
 * zCyl3DVolume() returns the calculated volume.
 */
__EXPORT double zCyl3DVolume(zCyl3D *cyl);

/*! \brief barycenter of a cylinder.
 *
 * zCyl3DBarycenter() calculates the barycenter of a 3D cylinder \a cyl.
 * The result is put into \a c.
 * \return
 * zCyl3DBarycenter() returns a pointer \a c.
 */
__EXPORT zVec3D *zCyl3DBarycenter(zCyl3D *cyl, zVec3D *c);

/*! \brief inertia tensor of a cylinder.
 *
 * zCyl3DBaryInertia() calculates the inertia tensor of a cylinder \a cyl about
 * its barycenter, supposing it is a solid. Its density has to be specified
 * by \a density.
 * zCyl3DBaryInertiaMass() calculates the inertia tensor of \a cyl about its
 * barycenter, where its mass instead of density has to be specified by \a mass.
 * For the both functions, the result is put into \a inertia.
 * \return
 * zCyl3DBaryInertia() and zCyl3DBaryInertiaMass() return a pointer \a inertia.
 * \sa
 * zCyl3DVolume(), zCyl3DBarycenter()
 */
__EXPORT zMat3D *zCyl3DBaryInertiaMass(zCyl3D *cyl, double mass, zMat3D *inertia);
__EXPORT zMat3D *zCyl3DBaryInertia(zCyl3D *cyl, double density, zMat3D *inertia);

/*! \brief convert a 3D cylinder to a polyhedron.
 *
 * zCyl3DToPH() converts a 3D cylinder \a cyl to a polyhedron \a ph as
 * a polygon model. It approximately divides the side face into rectangles
 * by the stored number of division.
 * \a ph should be initialized in advance.
 * \return
 * zCyl3DToPH() returns a pointer \a ph.
 * \sa
 * zSphere3DToPH, zSphere3DToPH,
 * zCone3DToPH, zCone3DToPHDRC
 */
__EXPORT zPH3D *zCyl3DToPH(zCyl3D *cyl, zPH3D *ph);

/*! \brief print a 3D cylinder out to a file in a ZTK format. */
__EXPORT void zCyl3DFPrintZTK(FILE *fp, zCyl3D *cyl);

/* methods for abstraction */
__EXPORT zShape3DCom zeo_shape3d_cyl_com;

#define zShape3DCyl(s) ( (zCyl3D*)(s)->body )

__EXPORT zShape3D *zShape3DCylCreate(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r, int div);

__END_DECLS

#endif /* __ZEO_SHAPE3D_CYL_H__ */
