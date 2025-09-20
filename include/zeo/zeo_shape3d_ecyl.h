/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_ecyl - 3D shapes: elliptic cylinder.
 */

#ifdef ZEO_SHAPE_DECL_METHOD
zShape3D *createEllipticCylinder(const zVec3D *center1, const zVec3D *center2, double radius1, double radius2, const zVec3D *ref, int div = 0);
#else

#ifndef __ZEO_SHAPE3D_ECYL_H__
#define __ZEO_SHAPE3D_ECYL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 3D elliptic cylinder class
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zECyl3D ){
  zVec3D center[2];
  double radius[2];
  zVec3D rv[2];
  int div;
};

#define zECyl3DCenter(cyl,i)      ( &(cyl)->center[(i)] )
#define zECyl3DRadius(cyl,i)      (cyl)->radius[(i)]
#define zECyl3DRadVec(cyl,i)      ( &(cyl)->rv[(i)] )
#define zECyl3DDiv(cyl)           (cyl)->div

#define zECyl3DSetCenter(cyl,i,p) zVec3DCopy( p, zECyl3DCenter(cyl,i) )
#define zECyl3DSetRadius(cyl,i,r) ( zECyl3DRadius(cyl,i) = (r) )
#define zECyl3DSetRadVec(cyl,i,v) zVec3DCopy( v, zECyl3DRadVec(cyl,i) )
#define zECyl3DSetDiv(cyl,d)      ( zECyl3DDiv(cyl) = (d) )

/*! \brief create a 3D elliptic cylinder. */
__ZEO_EXPORT zECyl3D *zECyl3DCreate(zECyl3D *cyl, const zVec3D *center1, const zVec3D *center2, double radius1, double radius2, const zVec3D *ref, int div);
/*! \brief initialize a 3D elliptic cylinder. */
__ZEO_EXPORT zECyl3D *zECyl3DInit(zECyl3D *cyl);
/*! \brief allocate memory for a 3D elliptic cylinder. */
__ZEO_EXPORT ZDEF_ALLOC_FUNCTION_PROTOTYPE( zECyl3D );
/*! \brief copy a 3D elliptic cylinder to another. */
__ZEO_EXPORT zECyl3D *zECyl3DCopy(const zECyl3D *src, zECyl3D *dest);
/*! \brief mirror a 3D elliptic cylinder. */
__ZEO_EXPORT zECyl3D *zECyl3DMirror(const zECyl3D *src, zECyl3D *dest, zAxis axis);

/*! \brief define orthonormal axes of the bottom face of a 3D elliptic cylinder. */
__ZEO_EXPORT void zECyl3DDefAxis(zECyl3D *cyl, const zVec3D *ref);

/*! \brief transform coordinates of a 3D elliptic cylinder. */
__ZEO_EXPORT zECyl3D *zECyl3DXform(const zECyl3D *src, const zFrame3D *frame, zECyl3D *dest);
/*! \brief inversely transform coordinates of a 3D elliptic cylinder. */
__ZEO_EXPORT zECyl3D *zECyl3DXformInv(const zECyl3D *src, const zFrame3D *frame, zECyl3D *dest);

/*! \brief the closest point to a 3D elliptic cylinder. */
__ZEO_EXPORT double zECyl3DClosest(const zECyl3D *cyl, const zVec3D *point, zVec3D *closestpoint);
/*! \brief distance from a point to a 3D elliptic cylinder. */
__ZEO_EXPORT double zECyl3DDistFromPoint(const zECyl3D *cyl, const zVec3D *point);
/*! \brief check if a point is inside of an elliptic cylinder. */
__ZEO_EXPORT bool zECyl3DPointIsInside(const zECyl3D *ecyl, const zVec3D *point, double margin);

/*! \brief axis vector a 3D elliptic cylinder. */
#define zECyl3DAxis(cyl,axis) \
  zVec3DSub( zECyl3DCenter(cyl,1), zECyl3DCenter(cyl,0), axis )

/*! \brief height of a 3D elliptic cylinder. */
__ZEO_EXPORT double zECyl3DHeight(const zECyl3D *cyl);

/*! \brief volume of a 3D elliptic cylinder. */
__ZEO_EXPORT double zECyl3DVolume(const zECyl3D *cyl);

/*! \brief barycenter of a 3D elliptic cylinder. */
__ZEO_EXPORT zVec3D *zECyl3DBarycenter(const zECyl3D *cyl, zVec3D *center);

/*! \brief inertia tensor of a 3D elliptic cylinder.
 *
 * zECyl3DBaryInertia() calculates the inertia tensor of an elliptic cylinder
 * \a cyl about its barycenter, supposing it is a solid. Its density has to be
 * specified by \a density.
 * zECyl3DBaryInertiaMass() calculates the inertia tensor of \a cyl about its
 * barycenter, where its mass instead of density has to be specified by \a mass.
 * For the both functions, the result is put into \a inertia.
 * \return
 * zECyl3DBaryInertia() and zECyl3DBaryInertiaMass() return a pointer \a inertia.
 * \sa
 * zECyl3DVolume(), zECyl3DBarycenter()
 */
__ZEO_EXPORT zMat3D *zECyl3DBaryInertiaMass(const zECyl3D *cyl, double mass, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zECyl3DBaryInertia(const zECyl3D *cyl, double density, zMat3D *inertia);

/*! \brief convert a cylinder to an instance of elliptic cylinder. */
__ZEO_EXPORT zECyl3D *zCyl3DToECyl3D(const zCyl3D *cylinder, zECyl3D *ecyl);

/*! \brief convert an elliptic cylinder to a polyhedron. */
__ZEO_EXPORT zPH3D *zECyl3DToPH(const zECyl3D *cyl, zPH3D *ph);

/*! \brief print a 3D elliptic cylinder out to a file in a ZTK format. */
__ZEO_EXPORT void zECyl3DFPrintZTK(FILE *fp, const zECyl3D *ecyl);

/*! \brief methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_ecyl_com;

#define zShape3DECyl(s) ( (zECyl3D*)(s)->body )

__ZEO_EXPORT zShape3D *zShape3DECylCreate(zShape3D *shape, const zVec3D *center1, const zVec3D *center2, double radius1, double radius2, const zVec3D *ref, int div);

__END_DECLS

#ifdef __cplusplus
inline zShape3D *zShape3D::createEllipticCylinder(const zVec3D *center1, const zVec3D *center2, double radius1, double radius2, const zVec3D *ref, int div){ return zShape3DECylCreate( this, center1, center2, radius1, radius2, ref, div ); }
#endif /* __cplusplus */

#endif /* __ZEO_SHAPE3D_ECYL_H__ */

#endif /* ZEO_SHAPE_DECL_METHOD */
