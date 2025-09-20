/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_box - 3D shapes: box.
 */

#ifndef __ZEO_SHAPE3D_BOX_H__
#define __ZEO_SHAPE3D_BOX_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief convert a box to a polyhedron.
 *
 * zBox3DToPH() converts a box \a box to a polyhedron \a ph as a
 * polygon by allocating memories for eight vertices and twelve
 * polygons. The order of the vertices is according to zBox3DVert().
 *
 * \a ph should be initialized in advance.
 * \return
 * zBox3DToPH() returns a pointer \a ph.
 * \sa
 * zBox3DVert
 */
__ZEO_EXPORT zPH3D *zBox3DToPH(const zBox3D *box, zPH3D *ph);

/*! \brief print a 3D box out to a file in a ZTK format. */
__ZEO_EXPORT void zBox3DFPrintZTK(FILE *fp, const zBox3D *box);

/* methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_box_com;

#define zShape3DBox(s) ( (zBox3D*)(s)->body )

__ZEO_EXPORT zShape3D *zShape3DBoxCreate(zShape3D *shape, const zVec3D *c, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double d, double w, double h);

__ZEO_EXPORT zShape3D *zShape3DBoxCreateAlign(zShape3D *shape, const zVec3D *c, double d, double w, double h);

__END_DECLS

#endif /* __ZEO_SHAPE3D_BOX_H__ */
