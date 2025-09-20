/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_cone - 3D shapes: cone.
 */

#ifdef ZEO_SHAPE_DECL_METHOD
zShape3D *createCone(const zVec3D *center, const zVec3D *vert, double radius, int div = 0);
#else

#ifndef __ZEO_SHAPE3D_CONE_H__
#define __ZEO_SHAPE3D_CONE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 3D cone class
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zCone3D ){
  zVec3D center;
  zVec3D vert;
  double radius;
  int div;
};

#define zCone3DCenter(cone)        ( &(cone)->center )
#define zCone3DVert(cone)          ( &(cone)->vert )
#define zCone3DRadius(cone)        (cone)->radius
#define zCone3DDiv(cone)           (cone)->div

#define zCone3DSetCenter(cone,p)   zVec3DCopy( p, zCone3DCenter(cone) )
#define zCone3DSetVert(cone,v)     zVec3DCopy( v, zCone3DVert(cone) )
#define zCone3DSetRadius(cone,r)   ( zCone3DRadius(cone) = (r) )
#define zCone3DSetDiv(cone,d)      ( zCone3DDiv(cone) = (d) )

/*! \brief initialization, creation and copy of 3D cone.
 *
 * zCone3DInit() initializes a 3D cone \a cone by setting both the center on the
 * base and the vertex for the original point and radius of the base for zero.
 *
 * zCone3DCreate() creates a 3D cone whose center point on the base is \a center,
 * vertex is \a vert, and radius of the base is \a radius.
 * \a div is the number of division for polyhedral approximation.
 * When zero is given for \a div, ZEO_SHAPE_DEFAULT_DIV is set instead.
 *
 * zCone3DCopy() copies a 3D cone \a src to the other \a dest.
 * \return
 * Each of zCone3DInit() and zCone3DCreate() returns a pointer \a cone.
 * zCone3DCopy() returns a pointer the copied \a dest.
 */
__ZEO_EXPORT zCone3D *zCone3DCreate(zCone3D *cone, const zVec3D *center, const zVec3D *vert, double radius, int div);
__ZEO_EXPORT zCone3D *zCone3DInit(zCone3D *cone);
__ZEO_EXPORT ZDEF_ALLOC_FUNCTION_PROTOTYPE( zCone3D );
__ZEO_EXPORT zCone3D *zCone3DCopy(const zCone3D *src, zCone3D *dest);
__ZEO_EXPORT zCone3D *zCone3DMirror(const zCone3D *src, zCone3D *dest, zAxis axis);

/*! \brief transformation of a 3D cone.
 *
 * zCone3DXform() transforms a 3D cone \a src by a frame \a frame, and puts it into \a dest.
 *
 * zCone3DXformInv() transforms \a src by the inverse of a frame \a frame and puts it into \a dest.
 * \return
 * zCone3DXform() and zCone3DXformInv() return a pointer \a dest.
 */
__ZEO_EXPORT zCone3D *zCone3DXform(const zCone3D *src, const zFrame3D *frame, zCone3D *dest);
__ZEO_EXPORT zCone3D *zCone3DXformInv(const zCone3D *src, const zFrame3D *frame, zCone3D *dest);

/*! \brief check if a point is inside of a cone.
 *
 * zCone3DPointIsInside() checks if a 3D point \a point is inside of a 3D cone
 * \a cone. \a margin is a margin of the inside area outward from the
 * boundary of \a cone.
 * \return
 * zCone3DPointIsInside() returns the true value when \a point is inside of \a cone,
 * or the false value otherwise.
 */
__ZEO_EXPORT double zCone3DClosest(const zCone3D *cone, const zVec3D *point, zVec3D *closestpoint);
__ZEO_EXPORT double zCone3DDistFromPoint(const zCone3D *cone, const zVec3D *point);
__ZEO_EXPORT bool zCone3DPointIsInside(const zCone3D *cone, const zVec3D *point, double margin);

/*! \brief axis vector and height of a 3D cone.
 *
 * zCone3DAxis() calculates the axis vector of a 3D cone \a cone; the axis
 * from the center point on the base to the vertex.
 *
 * zCone3DHeight() calculates the height from the base to the vertex of a 3D cone \a cone.
 * \return
 * zCone3DAxis() returns a pointer \a axis.
 * zCone3DHeight() returns the height calculated.
 */
#define zCone3DAxis(cone,axis) zVec3DSub( zCone3DVert(cone), zCone3DCenter(cone), axis )
__ZEO_EXPORT double zCone3DHeight(const zCone3D *cone);

/*! \brief volume of a cone.
 *
 * zCone3DVolume() calculates the volume of a 3D cone \a cone.
 * \return
 * zCone3DVolume() returns the calculated volume.
 */
__ZEO_EXPORT double zCone3DVolume(const zCone3D *cone);

/*! \brief barycenter of a cone.
 *
 * zCone3DBarycenter() calculates the barycenter of a 3D cone \a cone.
 * The result is put into \a c.
 * \return
 * zCone3DBarycenter() returns a pointer \a c.
 */
__ZEO_EXPORT zVec3D *zCone3DBarycenter(const zCone3D *cone, zVec3D *center);

/*! \brief inertia tensor of a cone.
 *
 * zCone3DBaryInertia() calculates the inertia tensor of a cone \a cone about
 * its barycenter, supposing it is a solid. Its density has to be specified
 * by \a density.
 * zCone3DBaryInertiaMass() calculates the inertia tensor of \a cone about its
 * barycenter, where its mass instead of density has to be specified by \a mass.
 * For the both functions, the result is put into \a inertia.
 * \return
 * zCone3DBaryInertia() and zCone3DBaryInertiaMass() return a pointer \a inertia.
 * \sa
 * zCone3DVolume(), zCone3DBarycenter()
 */
__ZEO_EXPORT zMat3D *zCone3DBaryInertiaMass(const zCone3D *cone, double mass, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zCone3DBaryInertia(const zCone3D *cone, double density, zMat3D *inertia);

/*! \brief convert cone to polyhedron.
 *
 * zCone3DToPH() converts a cone \a cone to a polyhedron \a ph as a polygon model.
 * It approximately divides the side face into rectangles by the stored number of division.
 * \a ph should be initialized in advance.
 * \return
 * zCone3DToPH() returns a pointer \a ph.
 * \sa
 * zSphere3DToPH, zSphere3DToPH, zCyl3DToPH, zCyl3DToPHDRC
 */
__ZEO_EXPORT zPH3D *zCone3DToPH(const zCone3D *cone, zPH3D *ph);

/*! \brief print a 3D cone out to a file in a ZTK format. */
__ZEO_EXPORT void zCone3DFPrintZTK(FILE *fp, const zCone3D *cone);

/* methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_cone_com;

#define zShape3DCone(s) ( (zCone3D*)(s)->body )

__ZEO_EXPORT zShape3D *zShape3DConeCreate(zShape3D *shape, const zVec3D *center, const zVec3D *vert, double radius, int div);

__END_DECLS

#ifdef __cplusplus
inline zShape3D *zShape3D::createCone(const zVec3D *center, const zVec3D *vert, double radius, int div){ return zShape3DConeCreate( this, center, vert, radius, div ); }
#endif /* __cplusplus */

#endif /* __ZEO_SHAPE3D_CONE_H__ */

#endif /* ZEO_SHAPE_DECL_METHOD */
