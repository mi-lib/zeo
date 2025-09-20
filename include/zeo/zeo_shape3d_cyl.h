/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_cyl - 3D shapes: cylinder.
 */

#ifdef ZEO_SHAPE_DECL_METHOD
zShape3D *createCylinder(const zVec3D *center1, const zVec3D *center2, double radius, int div = 0);
#else

#ifndef __ZEO_SHAPE3D_CYL_H__
#define __ZEO_SHAPE3D_CYL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 3D cylinder class
 *//* ******************************************************* */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zCyl3D ){
  zVec3D center[2];
  double radius;
  int div;
};

#define zCyl3DCenter(cyl,i)      ( &(cyl)->center[i] )
#define zCyl3DRadius(cyl)        (cyl)->radius
#define zCyl3DDiv(cyl)           (cyl)->div

#define zCyl3DSetCenter(cyl,i,p) zVec3DCopy(p,zCyl3DCenter(cyl,i))
#define zCyl3DSetRadius(cyl,r)   ( zCyl3DRadius(cyl) = (r) )
#define zCyl3DSetDiv(cyl,d)      ( zCyl3DDiv(cyl) = (d) )

/*! \brief initialize, create and copy a 3D cylinder.
 *
 * zCyl3DInit() initializes a 3D cylinder \a cyl, setting both centers
 * on the bases for the original point and radius for zero.
 *
 * zCyl3DCreate() creates a 3D cylinder whose two center points on the
 * bases are \a center1 and \a center2, and radius is \a radius.
 * \a div is the number of division for polyhedral approximation.
 * When zero is given for \a div, ZEO_SHAPE_DEFAULT_DIV is set instead.
 *
 * zCyl3DCopy() copies a 3D cylinder \a src to the other \a dest.
 * \return
 * zCyl3DInit() and zCyl3DCreate() return a pointer \a cyl.
 * zCyl3DCopy() returns a pointer \a dest.
 */
__ZEO_EXPORT zCyl3D *zCyl3DCreate(zCyl3D *cyl, const zVec3D *center1, const zVec3D *center2, double radius, int div);
__ZEO_EXPORT zCyl3D *zCyl3DInit(zCyl3D *cyl);
__ZEO_EXPORT ZDEF_ALLOC_FUNCTION_PROTOTYPE( zCyl3D );
__ZEO_EXPORT zCyl3D *zCyl3DCopy(const zCyl3D *src, zCyl3D *dest);
__ZEO_EXPORT zCyl3D *zCyl3DMirror(const zCyl3D *src, zCyl3D *dest, zAxis axis);

/*! \brief transform a 3D cylinder.
 *
 * zCyl3DXform() transforms a 3D cylinder \a src by a frame \a frame and
 * puts it into \a dest.
 *
 * zCyl3DXformInv() transforms \a src by the inverse of a frame \a frame
 * and puts it into \a dest.
 * \return
 * zCyl3DXform() and zCyl3DXformInv() return a pointer \a dest.
 */
__ZEO_EXPORT zCyl3D *zCyl3DXform(const zCyl3D *src, const zFrame3D *frame, zCyl3D *dest);
__ZEO_EXPORT zCyl3D *zCyl3DXformInv(const zCyl3D *src, const zFrame3D *frame, zCyl3D *dest);

/*! \brief check if a point is inside of a 3D cylinder.
 *
 * zCyl3DPointIsInside() checks if a 3D point \a point is inside of a 3D
 * cylinder \a cyl. \a margin is a margin of the inside area outward
 * from the boundary of \a cyl.
 * \return
 * zCyl3DPointIsInside() returns the true value if \a point is inside of \a cyl,
 * or the false value otherwise.
 */
__ZEO_EXPORT double zCyl3DClosest(const zCyl3D *cyl, const zVec3D *point, zVec3D *closestpoint);
__ZEO_EXPORT double zCyl3DDistFromPoint(const zCyl3D *cyl, const zVec3D *point);
__ZEO_EXPORT bool zCyl3DPointIsInside(const zCyl3D *cyl, const zVec3D *point, double margin);

/*! \brief axis vector and height of a 3D cylinder.
 *
 * zCyl3DAxis() calculates the axis vector of a 3D cylinder \a cyl;
 * the axis from the center point on the bottom base to the center point
 * on the top base.
 *
 * zCyl3DHeight() calculates the height from the bottom base to the top
 * base of a 3D cylinder \a cyl.
 * \return
 * zCyl3DAxis() returns a pointer \a axis.
 * zCyl3DHeight() returns the calculated height.
 */
#define zCyl3DAxis(cyl,axis) zVec3DSub( zCyl3DCenter(cyl,1), zCyl3DCenter(cyl,0), axis )
__ZEO_EXPORT double zCyl3DHeight(const zCyl3D *cyl);

/*! \brief volume of a 3D cylinder.
 *
 * zCyl3DVolume() calculates the volume of a 3D cylinder \a cyl.
 * \return
 * zCyl3DVolume() returns the calculated volume.
 */
__ZEO_EXPORT double zCyl3DVolume(const zCyl3D *cyl);

/*! \brief barycenter of a cylinder.
 *
 * zCyl3DBarycenter() calculates the barycenter of a 3D cylinder \a cyl.
 * The result is put into \a center.
 * \return
 * zCyl3DBarycenter() returns a pointer \a center.
 */
__ZEO_EXPORT zVec3D *zCyl3DBarycenter(const zCyl3D *cyl, zVec3D *center);

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
__ZEO_EXPORT zMat3D *zCyl3DBaryInertiaMass(const zCyl3D *cyl, double mass, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zCyl3DBaryInertia(const zCyl3D *cyl, double density, zMat3D *inertia);

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
__ZEO_EXPORT zPH3D *zCyl3DToPH(const zCyl3D *cyl, zPH3D *ph);

/*! \brief print a 3D cylinder out to a file in a ZTK format. */
__ZEO_EXPORT void zCyl3DFPrintZTK(FILE *fp, const zCyl3D *cyl);

/* methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_cyl_com;

#define zShape3DCyl(s) ( (zCyl3D*)(s)->body )

__ZEO_EXPORT zShape3D *zShape3DCylCreate(zShape3D *shape, const zVec3D *center1, const zVec3D *center2, double radius, int div);

__END_DECLS

#ifdef __cplusplus
inline zShape3D *zShape3D::createCylinder(const zVec3D *center1, const zVec3D *center2, double radius, int div){ return zShape3DCylCreate( this, center1, center2, radius, div ); }
#endif /* __cplusplus */

#endif /* __ZEO_SHAPE3D_CYL_H__ */

#endif /* ZEO_SHAPE_DECL_METHOD */
