/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs3d_shape - conversion from 3D shape to NURBS surface.
 */
#ifndef __ZEO_NURBS3D_SHAPE_H__
#define __ZEO_NURBS3D_SHAPE_H__

#include <zeo/zeo_shape3d.h>

__BEGIN_DECLS

/* closed NURBS surface that represents a box. */
__ZEO_EXPORT zNURBS3D *zNURBS3DBox(zNURBS3D *nurbs, const zBox3D *box);

/*! \brief closed NURBS surface that represents an ellipsoid. */
__ZEO_EXPORT zNURBS3D *zNURBS3DEllips(zNURBS3D *nurbs, const zEllips3D *ellips);

/* closed NURBS surface that represents a sphere. */
__ZEO_EXPORT zNURBS3D *zNURBS3DSphere(zNURBS3D *nurbs, const zSphere3D *sphere);

/*! \brief closed NURBS surface that represents an elliptic cylinder. */
__ZEO_EXPORT zNURBS3D *zNURBS3DECyl(zNURBS3D *nurbs, const zECyl3D *ecyl);

/* closed NURBS surface that represents a cylinder. */
__ZEO_EXPORT zNURBS3D *zNURBS3DCyl(zNURBS3D *nurbs, const zCyl3D *cylinder);

/*! \brief closed NURBS surface that represents a cone. */
__ZEO_EXPORT zNURBS3D *zNURBS3DCone(zNURBS3D *nurbs, const zCone3D *cone);

/*! \brief closed NURBS surface that represents a torus. */
__ZEO_EXPORT zNURBS3D *zNURBS3DTorus(zNURBS3D *nurbs, const zVec3D *center, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double rring, double rintersection);

__END_DECLS

#endif /* __ZEO_NURBS3D_SHAPE_H__ */
